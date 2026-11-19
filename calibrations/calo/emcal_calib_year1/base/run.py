#!/usr/bin/env python3
"""
This module automates the EMCal calibration procedure.
"""

import argparse
import os
import sys
import subprocess
import datetime
import time
import shutil
import logging

parser = argparse.ArgumentParser()

parser.add_argument('-i'
                    , '--input-list', type=str
                    , required=True
                    , help='Input DST List.')

parser.add_argument('-o'
                    , '--project-dir', type=str
                    , default='/gpfs02/sphenix/user/anarde/EMCal-Calib-Test'
                    , help='Project Directory. Default: .')

parser.add_argument('-it1'
                    , '--iter-start', type=int
                    , default=0
                    , help='Starting Iteration. Default: 0')

parser.add_argument('-it2'
                    , '--iter-end', type=int
                    , default=10
                    , help='Ending Iteration. Default: 10')

parser.add_argument('-s'
                    , '--memory', type=float
                    , default=2
                    , help='Memory (units of GB) to request per condor submission. Default: 2 GB.')

parser.add_argument('-l'
                    , '--log-file', type=str
                    , default='log.txt'
                    , help='Log File. Default: log.txt')

parser.add_argument('-l2'
                    , '--condor-log-dir', type=str
                    , default='/tmp/anarde/dump'
                    , help='Condor Log Directory. Default: /tmp/anarde/dump')

parser.add_argument('-p'
                    , '--files-per-hadd', type=int
                    , default=20
                    , help='Files Per hadd. Default: 20')

parser.add_argument('-s2'
                    , '--sleep-interval', type=int
                    , default=60
                    , help='Sleep Interval. Default: 60 seconds.')

parser.add_argument('-f'
                    , '--f4a-macro', type=str
                    , default='/gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/macros/Fun4All_EMCal.C'
                    , help='Fun4All Macro. Default: /gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/macros/Fun4All_EMCal.C')

parser.add_argument('-f1'
                    , '--fit-calib-macro', type=str
                    , default='/gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/macros/doFitAndCalibUpdate.C'
                    , help='Fit Calib Macro. Default: /gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/macros/doFitAndCalibUpdate.C')

parser.add_argument('-f2'
                    , '--tsc-fit-macro', type=str
                    , default='/gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/macros/doTscFit.C'
                    , help='TSC Calib Macro. Default: /gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/macros/doTscFit.C')

parser.add_argument('-f3'
                    , '--condor-script', type=str
                    , default='/gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/scripts/genCalib.sh'
                    , help='Condor Script. Default: /gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/scripts/genCalib.sh')

parser.add_argument('-b'
                    , '--f4a-bin', type=str
                    , default='/gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/bin/Fun4All_EMCal'
                    , help='Fun4All Bin. Default: /gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/bin/Fun4All_EMCal')

parser.add_argument('-b1'
                    , '--fit-calib-bin', type=str
                    , default='/gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/bin/doFitAndCalibUpdate'
                    , help='Fit Calib Bin. Default: /gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/bin/doFitAndCalibUpdate')

parser.add_argument('-b2'
                    , '--tsc-fit-bin', type=str
                    , default='/gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/bin/doTscFit'
                    , help='TSC Calib Bin. Default: /gpfs02/sphenix/user/anarde/sPHENIX/analysis-EMCal-Calibration/EMCal-Calibration-Study/bin/doTscFit')

parser.add_argument('-f4'
                    , '--calib-field', type=str
                    , default='CEMC_calib_ADC_to_ETower'
                    , help='Calib Field. Default: CEMC_calib_ADC_to_ETower')

args = parser.parse_args()

def setup_logging(log_file, log_level):
    """Configures the logging system to output to a file and console."""

    # Create a logger instance
    logger = logging.getLogger(__name__) # Use __name__ to get a logger specific to this module
    logger.setLevel(log_level)

    # Clear existing handlers to prevent duplicate output if run multiple times
    if logger.hasHandlers():
        logger.handlers.clear()

    # Create a formatter for log messages
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    # Create a FileHandler to save logs to a file
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(log_level)
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)

    # Create a StreamHandler to also output logs to the console (optional)
    console_handler = logging.StreamHandler(sys.stdout)
    console_handler.setLevel(logging.INFO) # Console might only show INFO and above
    console_handler.setFormatter(formatter)
    logger.addHandler(console_handler)

    return logger

def run_command_and_log(command, logger, current_dir = '.', description="Executing command"):
    """
    Runs an external command using subprocess and logs its stdout, stderr, and return code.
    """
    logger.info(f"{description}: '{command}'")

    try:
        # subprocess.run is the recommended high-level API
        # capture_output=True: captures stdout and stderr
        # text=True: decodes output as text (usually UTF-8)
        # check=False: do NOT raise an exception for non-zero exit codes immediately.
        #              We want to log stderr even on failure before deciding to raise.
        result = subprocess.run(['bash','-c',command], cwd=current_dir, capture_output=True, text=True, check=False)

        # Log stdout if any
        if result.stdout:
            # Using logger.debug allows capturing even verbose outputs
            logger.debug(f"  STDOUT from '{command}':\n{result.stdout.strip()}")

        # Log stderr if any
        if result.stderr:
            # Using logger.error for stderr, as it often indicates problems
            logger.error(f"  STDERR from '{command}':\n{result.stderr.strip()}")

        # Log the return code
        logger.info(f"  Command exited with code: {result.returncode}")

        # You can choose to raise an exception here if the command failed
        if result.returncode != 0:
            logger.error(f"Command failed: '{command}' exited with non-zero code {result.returncode}")
            # Optionally, raise an error to stop execution
            # raise subprocess.CalledProcessError(result.returncode, command, output=result.stdout, stderr=result.stderr)
            return False
        return True

    except FileNotFoundError:
        logger.critical(f"Error: Command '{command}' not found. Is it in your PATH?")
        return False
    except Exception as e:
        logger.critical(f"An unexpected error occurred while running '{command}': {e}")
        return False

def main():
    """
    Main Function
    """
    input_list = os.path.realpath(args.input_list)
    total_jobs = int(subprocess.run(['bash','-c',f'wc -l {input_list}'], capture_output=True, encoding='utf-8', check=False).stdout.strip().split()[0])
    project_dir = os.path.realpath(args.project_dir)
    iter_start = args.iter_start
    iter_end = args.iter_end
    memory = args.memory
    log_file  = os.path.realpath(args.log_file)
    log_dir   = os.path.dirname(log_file)
    condor_log_dir = os.path.realpath(args.condor_log_dir)
    files_per_hadd = args.files_per_hadd
    sleep_interval = args.sleep_interval # seconds
    f4a_macro = os.path.realpath(args.f4a_macro)
    fit_calib_macro = os.path.realpath(args.fit_calib_macro)
    tsc_fit_macro = os.path.realpath(args.tsc_fit_macro)
    f4a_bin = os.path.realpath(args.f4a_bin)
    fit_calib_bin = os.path.realpath(args.fit_calib_bin)
    tsc_fit_bin = os.path.realpath(args.tsc_fit_bin)
    condor_script = os.path.realpath(args.condor_script)
    calib_field = args.calib_field

    # Check args
    if iter_start > iter_end or iter_start < 0:
        parser.error(f'ERROR: {iter_start} > {iter_end} or {iter_start} is negative.')

    if sleep_interval <= 0:
        parser.error(f'ERROR: Negative sleep interval: {sleep_interval}.')

    # Create Dirs
    os.makedirs(log_dir,exist_ok=True)
    os.makedirs(project_dir, exist_ok=True)

    # Initialize the logger
    logger = setup_logging(log_file, logging.DEBUG)

    if os.path.exists(condor_log_dir):
        shutil.rmtree(condor_log_dir)
        os.makedirs(condor_log_dir)

    # Print Logs
    logger.info('#'*40)
    logger.info(f'LOGGING: {datetime.datetime.now()}')
    logger.info(f'Input DST List: {input_list}')
    logger.info(f'Total Jobs per Iteration: {total_jobs}')
    logger.info(f'Project Directory: {project_dir}')
    logger.info(f'Iteration Start: {iter_start}')
    logger.info(f'Iteration End: {iter_end}')
    logger.info(f'Condor Memory (per Job): {memory}')
    logger.info(f'Log File: {log_file}')
    logger.info(f'Files Per hadd: {files_per_hadd}')
    logger.info(f'Fun4All Macro: {f4a_macro}')
    logger.info(f'Fit Calib Macro: {fit_calib_macro}')
    logger.info(f'TSC Fit Macro: {tsc_fit_macro}')
    logger.info(f'Fun4All Bin: {f4a_bin}')
    logger.info(f'Fit Calib Bin: {fit_calib_bin}')
    logger.info(f'TSC Fit Bin: {tsc_fit_bin}')
    logger.info(f'Condor Script: {condor_script}')
    logger.info(f'Calib Field: {calib_field}')

    # Iteration Loop
    for it in range(iter_start,iter_end+1):
        logger.info(f'Iteration: {it}')

        if it == 0:
            # command = f'root -b -l -q \'{f4a_macro}(0,"{input_list}",{it})\''
            command = f'{f4a_bin} 0 {input_list} {it}'
            run_command_and_log(command, logger, project_dir)
            continue

        iter_dir = os.path.join(project_dir, f'test-iter-{it}')

        os.makedirs(iter_dir, exist_ok=True)

        os.makedirs(f'{iter_dir}/output', exist_ok=True)
        os.makedirs(f'{iter_dir}/stdout', exist_ok=True)
        os.makedirs(f'{iter_dir}/error', exist_ok=True)
        if it == 3:
            os.makedirs(f'{iter_dir}/figures', exist_ok=True)

        if it == 1:
            shutil.copy(f'{project_dir}/local_calib_copy.root', iter_dir)
        else:
            prev_iter_dir = os.path.join(project_dir, f'test-iter-{it-1}')
            shutil.copy(f'{prev_iter_dir}/local_calib_copy.root', iter_dir)

        shutil.copy(f'{condor_script}', iter_dir)
        shutil.copy(f'{f4a_macro}', iter_dir)
        shutil.copy(f'{input_list}', iter_dir)

        if it <= 3:
            shutil.copy(f'{tsc_fit_macro}', iter_dir)
        else:
            shutil.copy(f'{fit_calib_macro}', iter_dir)

        with open(f'{iter_dir}/genCalib.sub', mode='w', encoding='utf-8') as file:
            file.write(f'executable    = {os.path.basename(condor_script)}\n')
            file.write(f'arguments     = {f4a_bin} 0 $(input_dst) {it} {iter_dir}/local_calib_copy.root {calib_field} {iter_dir}/output\n')
            file.write(f'log           = {condor_log_dir}/job-$(ClusterId)-$(Process).log\n')
            file.write('output         = stdout/job-$(ClusterId)-$(Process).out\n')
            file.write('error          = error/job-$(ClusterId)-$(Process).err\n')
            file.write(f'request_memory = {memory}GB\n')

        logger.info(f'Iter: {it}, Condor Directory Generated: {datetime.datetime.now()}')

        command = f'condor_submit genCalib.sub -queue "input_dst from {input_list}"'
        logger.info(command)
        run_command_and_log(command, logger, iter_dir)

        while True:
            jobs = int(subprocess.run(['bash','-c','ls output | wc -l'], capture_output=True, encoding='utf-8', cwd=iter_dir, check=False).stdout.strip())

            if jobs == total_jobs:
                logger.info(f'Iter: {it}, All Jobs Finished: {datetime.datetime.now()}')
                break

            logger.info(f'Checking Condor Output Status: {datetime.datetime.now()}, Jobs: {jobs} out of {total_jobs}, {jobs * 100 / total_jobs:0.2f} %')

            time.sleep(sleep_interval)


        logger.info(f'Iter: {it}, hadd start: {datetime.datetime.now()}')

        command = f'hadd -n {files_per_hadd+1} test-iter{it}.root output/*'
        run_command_and_log(command, logger, iter_dir)

        logger.info(f'Iter: {it}, hadd finish: {datetime.datetime.now()}')

        shutil.copy(f'{iter_dir}/local_calib_copy.root', f'{iter_dir}/local_calib_copy_{it-1}.root')

        # macro = fit_calib_macro if it >= 4 else tsc_fit_macro
        calib_bin = fit_calib_bin if it >= 4 else tsc_fit_bin

        # command = f'root -b -l -q \'{macro}("test-iter{it}.root", "local_calib_copy.root", {it}, "{calib_field}")\''
        command = f'{calib_bin} test-iter{it}.root local_calib_copy.root {it} {calib_field}'
        run_command_and_log(command, logger, iter_dir)

        logger.info(f'Iter: {it}, local_calib_copy.root updated: {datetime.datetime.now()}')

if __name__ == "__main__":
    main()
