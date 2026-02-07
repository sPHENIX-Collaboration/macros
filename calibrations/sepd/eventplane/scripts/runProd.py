#!/usr/bin/env python3
"""
sEPD Q-Vector Calibration Pipeline
==================================
This script automates the end-to-end multi-stage calibration process for the sPHENIX
Event Plane Detector (sEPD). It manages a high-throughput workflow that interfaces
with the HTCondor batch system and utilizes local parallel processing to handle
large nuclear physics datasets.

Scientific Context:
-------------------
The pipeline performs a three-pass calibration followed by database payload generation
to correct for detector anisotropies and acceptance effects:
1. ComputeRecentering: Calculates the 1st-order <Q> vector offsets.
2. ApplyRecentering: Applies offsets and calculates the 2nd-order
   flattening/re-gain matrix (X-matrix).
3. ApplyFlattening: Applies the full correction (re-centering + flattening)
   and performs final validation.
4. CDB Generation: Formats calibration constants and bad tower maps into
   sPHENIX Calibration Database (CDB) payloads.

Workflow Architecture:
----------------------
- Stage-QA: Synchronizes DST lists and submits Fun4All jobs to produce initial
  TTrees and QA histograms.
- Stage-QVecCalib: Iteratively executes the three calibration passes, pairing
  TTrees with previous calibration constants.
- Stage-QVecCDB: Transforms final merged calibration results and QA maps into
  CDB-formatted ROOT files.
- Parallelization: Leverages Python's multiprocessing.Pool for simultaneous
  'hadd' merging and concurrent CDB generation.
- Monitoring: Implements a passive log-polling strategy to track HTCondor
  job completion via .log files for improved reliability.

Requirements:
-------------
- Python 3.13+ (Utilizes modern type hinting and directory-level path handling).
- sPHENIX Environment (OFFLINE_MAIN must be correctly set in the environment).
- Access to CreateDstList.pl for automated DST list synchronization.

Author: Apurva Narde
Date: 2026
"""

import argparse
import logging
import os
import shlex
import shutil
import subprocess
import sys
import textwrap
import time
import re
from pathlib import Path
from dataclasses import dataclass, fields
from multiprocessing import Pool
from collections.abc import Callable

# -----------------------------------------------------------------------------
# Configuration & Dataclasses
# -----------------------------------------------------------------------------


@dataclass
class PipelineConfig:
    """
    Central configuration container for the sEPD Calibration pipeline.

    This class manages all filesystem paths, Condor resource requirements,
    and experiment-specific tags. Using a dataclass ensures that all
    stages of the pipeline have a consistent view of the environment.
    """

    run_list_file: Path
    f4a_macro: Path
    bin_QVecCalib: Path
    bin_QVecCDB: Path
    f4a_script: Path
    QVecCalib_script: Path
    output_dir: Path
    dst_list_dir: Path
    condor_log_dir: Path

    # Strings and primitives
    dst_tag: str
    cdb_tag: str
    segments: int
    events: int
    f4a_condor_memory: float
    QVecCalib_condor_memory: float
    n_cores: int
    verbose: bool

    # Helper properties to standardize paths across the script
    @property
    def stage_qa_dir(self) -> Path:
        """Path: Working directory for the initial QA and TTree generation stage."""
        return self.output_dir / "stage-QA"

    @property
    def stage_QVecCalib_dir(self) -> Path:
        """Path: Working directory for the multi-pass Q-Vector calibration."""
        return self.output_dir / "stage-QVecCalib"

    @property
    def qa_output_dir(self) -> Path:
        """Path: Final destination for merged QA root files."""
        return self.output_dir / "QA"

    @property
    def QVecCalib_output_dir(self) -> Path:
        """Path: Final destination for merged calibration results."""
        return self.output_dir / "QVecCalib"

    @property
    def log_file(self) -> Path:
        """Path: The main execution log for the python script."""
        return self.output_dir / "log.txt"

    def __str__(self):
        """
        Generates a formatted string representing the current configuration state.

        Returns:
            str: An aligned, multi-line string listing all configuration fields.
        """
        lines = ["PipelineConfig Configuration:"]

        # Calculate the longest field name for alignment padding
        max_len = max(len(f.name) for f in fields(self))

        for f in fields(self):
            value = getattr(self, f.name)
            # Format:   field_name   : value
            lines.append(f"  {f.name:<{max_len}} : {value}")

        return "\n".join(lines)


# -----------------------------------------------------------------------------
# Logging Setup
# -----------------------------------------------------------------------------

logger = logging.getLogger(__name__)


def setup_logging(log_file: Path, verbose: bool = False) -> None:
    """
    Configures the global logging setup.

    Sets up a file handler for persistent, detailed logs (DEBUG/INFO) and a
    console handler for high-level progress updates (INFO only).

    Args:
        log_file: The path where the log file will be created/overwritten.
        verbose: If True, sets the file logging level to DEBUG.
    """
    log_level = logging.DEBUG if verbose else logging.INFO

    logger.setLevel(log_level)
    if logger.hasHandlers():
        logger.handlers.clear()

    formatter = logging.Formatter("%(asctime)s - %(levelname)s - %(message)s")

    # File Handler
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(log_level)
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)

    # Console Handler (Optional: helps see progress in real-time)
    console = logging.StreamHandler()
    console.setLevel(logging.INFO)  # Keep console clean, file detailed
    console.setFormatter(formatter)
    logger.addHandler(console)


def run_command_and_log(
    command: str | list[str],
    current_dir: Path | str = ".",
    do_logging: bool = True,
    description: str = "Executing command",
    allow_shell: bool = False,
) -> bool:
    """
    Executes a shell command safely using subprocess and logs the output.

    Args:
        command: The shell command string or list to execute.
        current_dir: The directory to execute the command in. Defaults to current directory.
        do_logging: If True, logs the command and its STDOUT/STDERR.
                    Set to False to keep logs clean during parallel execution.
        description: A brief label for the log entry describing the action.
        allow_shell: If True, executes the command through the system shell (bash).
                     WARNING: This is susceptible to shell injection attacks if
                     user-supplied strings like {config.dst_tag} are interpolated
                     without escaping.
                     Only use this when shell-specific features like pipes (|),
                     redirects (>), or globbing (*) are strictly necessary and the
                     input is fully trusted.
                     Defaults to False to ensure arguments are treated as literal
                     text rather than executable code.

    Returns:
        bool: True if the command returned exit code 0, False otherwise.
    """
    if do_logging:
        logger.info(f"{description}: '{command}'")

    try:
        if isinstance(command, str):
            cmd = ["bash", "-c", command] if allow_shell else shlex.split(command)
        else:
            cmd = command

        result = subprocess.run(
            cmd,
            cwd=current_dir,
            capture_output=True,
            text=True,
            check=False,
        )

        if result.stdout and do_logging:
            logger.debug(f"  STDOUT:\n{result.stdout.strip()}")

        if result.stderr:
            logger.error(f"  STDERR:\n{result.stderr.strip()}")

        if result.returncode != 0:
            logger.error(f"Command failed with code {result.returncode}")
            return False

        return True

    except Exception as e:
        logger.critical(f"Execution error: {e}")
        return False


# -----------------------------------------------------------------------------
# Helper Logic
# -----------------------------------------------------------------------------


def find_missing_runnumbers(dir_p: Path, input_p: Path) -> list[str]:
    """
    Identifies runs from a reference list that are missing in the target directory.

    Args:
        dir_p: The directory containing existing files (filenames are checked).
        input_p: A text file containing the master list of run numbers.

    Returns:
        list[str]: A list of run numbers that do not have corresponding files in dir_p.
    """
    if not dir_p.is_dir():
        logger.critical(f"Error: {dir_p} is not a valid directory.")
        sys.exit(1)

    filenames = [f.name for f in dir_p.iterdir() if f.is_file()]
    missing_runs = []

    try:
        content = input_p.read_text().splitlines()
        for run_num in content:
            run_num = run_num.strip()
            if not run_num:
                continue

            if not any(run_num in name for name in filenames):
                missing_runs.append(run_num)
    except FileNotFoundError:
        logger.critical(f"Error: {input_p} not found.")
        sys.exit(1)

    return missing_runs


def gen_dst_list(config: PipelineConfig, working_dir: Path) -> None:
    """
    Generates and splits DST input lists for Condor jobs.

    1. Checks for missing DST lists and generates them using `CreateDstList.pl` if needed.
    2. Splits large DST lists into smaller chunks (based on `config.segments`) into the `working_dir`.
    3. Generates a master `jobs.list` for the Condor submission.

    Args:
        config: The main pipeline configuration object.
        working_dir: The directory where the split lists and jobs.list will be stored.
    """
    missing_runs = find_missing_runnumbers(config.dst_list_dir, config.run_list_file)

    if missing_runs:
        logger.info(f"Generating DST List for {len(missing_runs)} missing run(s)...")
        run_list_missing = config.output_dir / "runs.list"
        run_list_missing.write_text("\n".join(missing_runs) + "\n")

        command = ["CreateDstList.pl", "--tag", config.dst_tag, "--list", str(run_list_missing), "DST_CALOFITTING"]
        run_command_and_log(command, config.dst_list_dir)
    else:
        logger.info("All runs have DST Lists.")

    # Identify relevant DST lists
    runs = {line.strip() for line in config.run_list_file.read_text().splitlines() if line.strip()}
    dst_lists = [f for f in config.dst_list_dir.iterdir() if f.is_file() and any(run in f.name for run in runs)]

    # Prepare file splits for parallel processing
    files_dir = working_dir / "files"
    files_dir.mkdir(parents=True, exist_ok=True)

    for dst_list in dst_lists:
        stem = dst_list.stem
        # 1. Read the DST list and take only the requested segments
        try:
            lines = dst_list.read_text().splitlines()[:config.segments]
        except Exception as e:
            logger.error(f"Failed to read {dst_list}: {e}")
            continue

        # 2. Create individual files (replaces 'split')
        for i, line in enumerate(lines):
            # Using a zero-padded suffix (000, 001)
            split_file = files_dir / f"{stem}-{i:03d}.list"
            split_file.write_text(line + "\n")

    job_paths = [str(f.resolve()) for f in files_dir.glob("*.list")]

    jobs_list_path = working_dir / "jobs.list"
    jobs_list_path.write_text("\n".join(job_paths) + "\n")

    logger.info(f"Generated jobs.list with {len(job_paths)} entries.")

def generate_QVecCalib_job_list(dir_qa: Path, dir_ttree: Path, output_file: Path) -> None:
    """
    Generates the primary job list for QVecCalib by pairing TTree files with QA files.

    Iterates through run subdirectories in `dir_ttree`. If a corresponding QA file
    exists in `dir_qa`, it adds the pair to the job list.

    Args:
        dir_qa: Directory containing the QA root files (e.g., QA-12345.root).
        dir_ttree: Directory containing subfolders of run trees.
        output_file: The path where the generated jobs.list will be written.
    """
    results = []

    for ttree_subdir in dir_ttree.iterdir():
        if ttree_subdir.is_dir():
            run = ttree_subdir.name

            target_root_file = dir_qa / f"QA-{run}.root"

            if target_root_file.exists():
                tree_path = ttree_subdir / "tree"

                if tree_path.exists() and tree_path.is_dir():
                    for file_in_tree in tree_path.iterdir():
                        if file_in_tree.is_file():
                            abs_tree_file = file_in_tree.resolve()
                            abs_root_file = target_root_file.resolve()

                            results.append(f"{abs_tree_file},{abs_root_file}")

    output_file.write_text("\n".join(results) + "\n")


def generate_QVecCalib_local_jobs_file(input_file: Path, output_file: Path, calib_dir: Path | None) -> None:
    """
    Generates a localized job list for a specific calibration pass.

    Parses the run number from nested directory structures (e.g., .../output/68144/tree/...)
    to correctly associate previous calibration constants with current jobs.

    Args:
        input_file: The base jobs list containing TTree and QA file paths.
        output_file: The destination for the pass-specific job list.
        calib_dir: Path to merged root files from a previous pass. If None, 'none'
                   is passed as the calibration argument.
    """
    lines = input_file.read_text().splitlines()
    results = []

    for line in lines:
        path_str = line.split(",")[0]
        path_obj = Path(path_str)

        try:
            # Go up two levels: file -> tree -> RUN_NUMBER
            run_num = path_obj.parents[1].name
        except IndexError:
            # Fallback if path structure is shallower than expected
            try:
                idx = path_obj.parts.index("output")
                run_num = path_obj.parts[idx + 1]
            except ValueError:
                logger.warning(f"Skipping malformed path: {path_str}")
                continue

        calib_arg = "none"
        if calib_dir:
            calib_arg = calib_dir / f"test-{run_num}.root"

        results.append(f"{line},{calib_arg}")

    output_file.write_text("\n".join(results) + "\n")


def worker_exec_task(args: tuple[str, str]) -> tuple[str, bool]:
    """
    Generic worker function for parallel command execution.

    Args:
        args: A tuple containing:
              1. cmd (str): The shell command to execute.
              2. run (str): The run identifier.

    Returns:
        tuple[str, bool]: A tuple containing the run identifier and a boolean success status.
    """
    cmd, run = args
    # execute without verbose logging to prevent console interleaving
    success = run_command_and_log(cmd, do_logging=False)
    return (run, success)


def run_parallel_hadd(
    runs: list[str],
    output_dir: Path,
    input_dir_finder: Callable[[str], Path],
    file_prefix: str = "",
    n_cores: int = 8,
) -> None:
    """
    Parallelizes the hadd step.

    Args:
        runs (list): List of run numbers (strings).
        output_dir (Path): Where to save the merged files.
        input_dir_finder (func): A lambda/function that takes 'run' and returns the input Path.
        file_prefix (str): Prefix for the output file (e.g., "QA-" or "test-").
        n_cores (int): Number of parallel processes.
    """
    tasks = []
    output_dir.mkdir(parents=True, exist_ok=True)

    for run in runs:
        run = run.strip()
        if not run:
            continue

        job_dir = input_dir_finder(run)
        if not job_dir.is_dir():
            logger.error(f"Missing output directory for run {run}: {job_dir}")
            continue

        input_files = [str(f) for f in job_dir.glob("*.root")]

        if not input_files:
            logger.warning(f"No .root files found in {job_dir} for run {run}")
            continue

        # Customizable output filename
        target_file = output_dir / f"{file_prefix}{run}.root"
        command = ["hadd", "-f", "-n", "11", str(target_file)] + input_files
        tasks.append((command, run))

    if not tasks:
        logger.warning("No runs to merge.")
        return

    logger.info(f"Starting parallel merge for {len(tasks)} runs using {n_cores} cores...")

    with Pool(n_cores) as p:
        results = p.map(worker_exec_task, tasks)

    failed = [r for r, success in results if not success]
    if failed:
        logger.error(f"Failed to merge runs: {failed}")
    else:
        logger.info("All merges completed successfully.")


# -----------------------------------------------------------------------------
# Monitoring Logic
# -----------------------------------------------------------------------------


def monitor_condor_logs(log_dir: Path, total_jobs: int) -> None:
    """
    Polls Condor user log files to track job completion status.

    Unlike 'condor_q', this method parses the .log files directly to
    identify successful termination (event code 005). This is
    more resilient for high-throughput pipelines.

    Args:
        log_dir: The directory containing the .log files for the current batch.
        total_jobs: The expected number of terminated jobs to reach before returning.
    """
    logger.info(f"Monitoring {total_jobs} jobs in: {log_dir}")

    # Regex to find: "Normal termination (return value 0)"
    # This is standard HTCondor log format
    success_pattern = re.compile(r"Normal termination \(return value 0\)")
    failure_pattern = re.compile(r"Abnormal termination")
    start_time = time.time()
    max_wait_sec = 2 * 60 * 60  # 2 hours

    # Track which jobs (files) have finished to avoid re-reading them constantly
    finished_files = set()

    while len(finished_files) < total_jobs:
        # Get list of all log files currently in directory
        log_files = list(log_dir.glob("*.log"))

        current_success_count = 0

        for log_file in log_files:
            # If we already marked this as done, skip it
            if log_file in finished_files:
                current_success_count += 1
                continue

            # Read file content
            try:
                # We open in 'errors=ignore' to avoid crashing on weird binary garbage
                with open(log_file, "r", encoding="utf-8", errors="ignore") as f:
                    content = f.read()

                    # Check for failure
                    if failure_pattern.search(content):
                        logger.error(f"Job failed: {log_file}")
                        sys.exit(1)
                    # Check for success
                    if success_pattern.search(content):
                        finished_files.add(log_file)
                        current_success_count += 1

                    # Optional: Add check for "Abnormal termination" here to fail early
            except Exception as e:
                logger.warning(f"Could not read log {log_file}: {e}")

        percent = (len(finished_files) / total_jobs) * 100
        logger.info(f"Progress: {len(finished_files)}/{total_jobs} ({percent:.1f}%) finished.")

        if len(finished_files) == total_jobs:
            logger.info("All jobs finished successfully.")
            break

        if time.time() - start_time > max_wait_sec:
            logger.critical("Timeout while waiting for Condor jobs.")
            sys.exit(1)
        time.sleep(15) # Wait 15 seconds before next check


def submit_and_monitor(config: PipelineConfig, submit_file: Path, input_source: str, job_dir: Path) -> None:
    """
    Handles the full lifecycle of a Condor batch submission.

    1. Cleans the global Condor log directory to prevent reading stale logs.
    2. Estimates the total job count by reading the input file.
    3. Submits the `.sub` file using `condor_submit`.
    4. Blocks execution and monitors logs until all jobs complete successfully.

    Args:
        config: Pipeline configuration (used for log paths).
        submit_file: Path to the .sub file.
        input_source: The Condor queue string (e.g., "input from jobs.list") used to estimate job count.
        job_dir: The working directory for the submission.
    """
    # 1. Clean Logs
    shutil.rmtree(config.condor_log_dir, ignore_errors=True)
    config.condor_log_dir.mkdir(parents=True, exist_ok=True)

    # 2. Count Jobs (approximate based on input file lines)
    input_file = job_dir / input_source.split(" from ")[1]  # crude parse to find the file
    total_jobs = len(input_file.read_text().strip().splitlines())

    # 3. Submit
    logger.info(f"Submitting {total_jobs} jobs from {submit_file.name}...")
    command = ["condor_submit", submit_file.name, "-queue", input_source]

    if not run_command_and_log(command, job_dir):
        logger.critical("Condor submission failed.")
        sys.exit(1)

    # 4. Monitor
    monitor_condor_logs(config.condor_log_dir, total_jobs)


# -----------------------------------------------------------------------------
# Stage Execution
# -----------------------------------------------------------------------------


def run_qa_stage(config: PipelineConfig) -> None:
    """
    Executes the Quality Assurance (QA) and TTree production stage.

    This stage performs the following:
    1. Synchronizes DST lists with the provided run list.
    2. Splits processing into manageable segments for parallel execution.
    3. Submits Fun4All jobs to Condor to produce histograms and TTrees.
    4. Blocks until completion and merges the resulting QA files in parallel.

    Args:
        config: The global configuration object.
    """
    logger.info(">>> Starting QA Stage")

    # 1. Setup Directories
    config.stage_qa_dir.mkdir(parents=True, exist_ok=True)

    for subdir in ["stdout", "error", "output"]:
        (config.stage_qa_dir / subdir).mkdir(parents=True, exist_ok=True)

    # 2. Generate Inputs
    gen_dst_list(config, config.stage_qa_dir)

    # Copy macro and script to run dir
    shutil.copy(config.f4a_macro, config.stage_qa_dir)
    shutil.copy(config.f4a_script, config.stage_qa_dir)

    # 3. Create Submit File
    # Using the Config object makes this string interpolation much cleaner
    f4a_condor = textwrap.dedent(f"""\
        executable     = {config.f4a_script.name}
        arguments      = {config.f4a_macro} $(input_dst) test-$(ClusterId)-$(Process).root tree-$(ClusterId)-$(Process).root {config.events} {config.cdb_tag} {config.stage_qa_dir}/output
        log            = {config.condor_log_dir}/job-$(ClusterId)-$(Process).log
        output         = stdout/job-$(ClusterId)-$(Process).out
        error          = error/job-$(ClusterId)-$(Process).err
        request_memory = {config.f4a_condor_memory}GB
    """)

    (config.stage_qa_dir / "genFun4All.sub").write_text(f4a_condor)

    submit_and_monitor(
        config,
        config.stage_qa_dir / "genFun4All.sub",
        "input_dst from jobs.list",
        config.stage_qa_dir,
    )

    # 6. Merge Results (Hadd)
    config.qa_output_dir.mkdir(parents=True, exist_ok=True)
    runs = config.run_list_file.read_text().splitlines()

    # Use a default argument to capture the path value immediately
    base_dir = config.stage_qa_dir / "output"

    def input_finder(run_num: str, path: Path = base_dir) -> Path:
        """Helper to locate histogram directory for a run."""
        return path / f"{run_num}/hist"

    run_parallel_hadd(
        runs,
        config.qa_output_dir,
        input_finder,
        file_prefix="QA-",
        n_cores=config.n_cores,
    )


def run_QVecCalib_stage(config: PipelineConfig) -> None:
    """
    Orchestrates the multi-pass Q-Vector Calibration stage.

    Executes three sequential calibration passes:
    1. ComputeRecentering: Calculates Q-vector averages.
    2. ApplyRecentering: Applies offsets and calculates flattening matrix.
    3. ApplyFlattening: Applies full corrections and validates.

    For each pass, it generates specific job lists, submits to Condor, monitors progress,
    and merges the results in parallel before proceeding to the next pass.
    """

    logger.info(">>> Starting Q Vector Calibration Stage")

    # 1. Setup Directories
    config.stage_QVecCalib_dir.mkdir(parents=True, exist_ok=True)

    jobs_file = config.stage_QVecCalib_dir / "jobs.list"

    # Initial Job List
    generate_QVecCalib_job_list(config.qa_output_dir, config.stage_qa_dir / "output", jobs_file)

    # list of subdirectories to create
    subdirectories = ["stdout", "error", "output", "merged"]
    calib_types = ["ComputeRecentering", "ApplyRecentering", "ApplyFlattening"]

    # Loop through the list and create each one
    for calib in calib_types:
        for subdir in subdirectories:
            (config.stage_QVecCalib_dir / calib / subdir).mkdir(parents=True, exist_ok=True)

    condor_script = shutil.copy(config.QVecCalib_script, config.stage_QVecCalib_dir)
    runs = config.run_list_file.read_text().splitlines()

    merge_output_dir = None

    for idx, calib in enumerate(calib_types):
        logger.info(f">>> Starting Q Vector Calibration Stage: {calib}")

        job_dir = config.stage_QVecCalib_dir / calib
        condor_submit_file = job_dir / "genQVecCalib.sub"
        output = job_dir / "output"
        calib_dir = config.stage_QVecCalib_dir / calib_types[idx - 1] / "merged" if idx != 0 else None
        local_jobs_file = job_dir / "jobs.list"
        generate_QVecCalib_local_jobs_file(jobs_file, local_jobs_file, calib_dir)

        QVecCalib_condor = textwrap.dedent(f"""\
            executable     = {condor_script}
            arguments      = {config.bin_QVecCalib} $(input_tree) $(input_QA) $(input_calib) {calib} {output}
            log            = {config.condor_log_dir}/job-$(ClusterId)-$(Process).log
            output         = stdout/job-$(ClusterId)-$(Process).out
            error          = error/job-$(ClusterId)-$(Process).err
            request_memory = {config.QVecCalib_condor_memory}GB
        """)

        condor_submit_file.write_text(QVecCalib_condor)

        submit_and_monitor(
            config,
            condor_submit_file,
            "input_tree,input_QA,input_calib from jobs.list",
            job_dir,
        )

        merge_output_dir = job_dir / "merged"

        def input_finder(run_num: str, path: Path = output) -> Path:
            """Helper to locate output directory for a specific run."""
            return path / run_num

        # Run parallel merge
        run_parallel_hadd(
            runs,
            merge_output_dir,
            input_finder,
            file_prefix="test-",
            n_cores=config.n_cores,
        )

    # Transfer merged output to final output directory
    if merge_output_dir:
        shutil.copytree(merge_output_dir, config.QVecCalib_output_dir, dirs_exist_ok=True)


def run_QVecCDB_stage(config: PipelineConfig) -> None:
    """
    Create CDB files for sEPD Q Vector Calibrations and sEPD Bad Towers.
    """
    logger.info(">>> Starting CDB Generation Stage")

    runs = config.run_list_file.read_text().splitlines()
    cdb_dir = config.output_dir / "CDB"

    cdb_dir.mkdir(parents=True, exist_ok=True)

    tasks = []
    for run in runs:
        run = run.strip()
        if not run:
            continue

        input_file = config.QVecCalib_output_dir / f"test-{run}.root"

        # Check input exists to avoid launching doomed processes
        if not input_file.exists():
            logger.warning(f"Skipping CDB gen for {run}: Missing input {input_file.name}")
            continue

        command = [
            str(config.bin_QVecCDB),
            str(input_file),
            str(run),
            str(cdb_dir),
            config.dst_tag
        ]
        tasks.append((command, run))

    if not tasks:
        logger.warning("No CDB generation tasks created.")
        return

    logger.info(f"Starting parallel CDB generation for {len(tasks)} runs...")

    # Reuse the same generic worker
    with Pool(config.n_cores) as p:
        results = p.map(worker_exec_task, tasks)

    failed = [r for r, success in results if not success]
    if failed:
        logger.error(f"Failed to generate CDB for runs: {failed}")
    else:
        logger.info("All CDB files generated successfully.")


# -----------------------------------------------------------------------------
# Main Execution
# -----------------------------------------------------------------------------


def main():
    """
    The main entry point for the production script.

    Handles command-line argument parsing, environment validation (checking
    OFFLINE_MAIN), resolves absolute paths, and initializes the PipelineConfig.
    Sequentially triggers the QA and Q-Vector calibration stages.
    """
    parser = argparse.ArgumentParser(description="Fun4All Production Pipeline")

    # Grouping arguments makes help output easier to read
    req_grp = parser.add_argument_group("Required")
    req_grp.add_argument("-i", "--run-list-file", type=str, required=True, help="List of runs.")

    opt_grp = parser.add_argument_group("Optional")
    opt_grp.add_argument("-i2", "--f4a-macro", type=str, default="macros/Fun4All_sEPD.C")
    opt_grp.add_argument("-i3", "--dst-list-dir", type=str, default="")
    opt_grp.add_argument("-i4", "--bin-QVecCalib", type=str, default="")
    opt_grp.add_argument("-i5", "--bin-QVecCDB", type=str, default="")
    opt_grp.add_argument("-n1", "--segments", type=int, default=15)
    opt_grp.add_argument("-n2", "--events", type=int, default=0)
    opt_grp.add_argument("-n3", "--n-cores", type=int, default=8)
    opt_grp.add_argument("-t1", "--dst-tag", type=str, default="new_newcdbtag_v008")
    opt_grp.add_argument("-t2", "--cdb-tag", type=str, default="newcdbtag")
    opt_grp.add_argument("-e1", "--f4a-script", type=str, default="scripts/genFun4All.sh")
    opt_grp.add_argument("-e2", "--QVecCalib-script", type=str, default="scripts/genQVecCalib.sh")
    opt_grp.add_argument("-o", "--output", type=str, default="test")
    opt_grp.add_argument("-m1", "--f4a-memory", type=float, default=3)
    opt_grp.add_argument("-m2", "--QVecCalib-memory", type=float, default=0.5)
    opt_grp.add_argument("-l", "--condor-log-dir", type=str, default="")
    opt_grp.add_argument("-v", "--verbose", action="store_true")

    args = parser.parse_args()

    # Resolve paths immediately
    output_dir = Path(args.output).resolve()

    # Determine defaults for paths that depend on others
    dst_list_dir = Path(args.dst_list_dir).resolve() if args.dst_list_dir else output_dir / "dst-lists"

    user_name = os.environ.get("USER", "unknown")
    condor_log_default = Path(f"/tmp/{user_name}/dump")
    condor_log_dir = Path(args.condor_log_dir).resolve() if args.condor_log_dir else condor_log_default

    offline_main = os.environ.get("OFFLINE_MAIN")
    if not offline_main:
        logger.critical("OFFLINE_MAIN environment variable not set, exiting.")
        sys.exit(1)

    offline_main_bin = Path(offline_main) / "bin"

    bin_QVecCalib = Path(args.bin_QVecCalib).resolve() if args.bin_QVecCalib else offline_main_bin / "GenQVecCalib"
    bin_QVecCDB = Path(args.bin_QVecCDB).resolve() if args.bin_QVecCDB else offline_main_bin / "GenQVecCDB"

    # Dependency Checking
    required_tools = ["CreateDstList.pl", "condor_submit", "hadd"]
    for tool in required_tools:
        if not shutil.which(tool):
            logger.critical(f"Required tool not found in PATH: {tool}")
            sys.exit(1)

    # Initialize the Configuration Dataclass
    config = PipelineConfig(
        run_list_file=Path(args.run_list_file).resolve(),
        f4a_macro=Path(args.f4a_macro).resolve(),
        bin_QVecCalib=bin_QVecCalib,
        bin_QVecCDB=bin_QVecCDB,
        f4a_script=Path(args.f4a_script).resolve(),
        QVecCalib_script=Path(args.QVecCalib_script).resolve(),
        output_dir=output_dir,
        dst_list_dir=dst_list_dir,
        condor_log_dir=condor_log_dir,
        dst_tag=args.dst_tag,
        cdb_tag=args.cdb_tag,
        segments=args.segments,
        events=args.events,
        n_cores=args.n_cores,
        f4a_condor_memory=args.f4a_memory,
        QVecCalib_condor_memory=args.QVecCalib_memory,
        verbose=args.verbose,
    )

    # Create base directories
    config.output_dir.mkdir(parents=True, exist_ok=True)
    config.dst_list_dir.mkdir(parents=True, exist_ok=True)

    # Setup Logging
    setup_logging(config.log_file, config.verbose)

    # Validation
    for f in [
        config.run_list_file,
        config.f4a_macro,
        config.bin_QVecCalib,
        config.bin_QVecCDB,
        config.f4a_script,
        config.QVecCalib_script,
    ]:
        if not f.is_file():
            logger.critical(f"File missing: {f}")
            sys.exit(1)

    # Log Configuration State
    logger.info("=" * 40)
    logger.info(f"Pipeline Config:\n{config}")
    logger.info("=" * 40)

    # Run QA Stage
    run_qa_stage(config)

    # Run Q Vector Calibration Stage
    run_QVecCalib_stage(config)

    # Run CDB Generation Stage
    run_QVecCDB_stage(config)


if __name__ == "__main__":
    main()
