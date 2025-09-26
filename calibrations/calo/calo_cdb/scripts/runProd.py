#!/usr/bin/env python3
"""
This module generates a list of run / datasets given a run type and event threshold.
"""
import argparse
import datetime
import logging
import os
import shutil
import subprocess
import sys
import textwrap

from pathlib import Path
from multiprocessing import Pool, cpu_count
import pandas as pd
from sqlalchemy import create_engine, text

logger = logging.getLogger(__name__)

parser = argparse.ArgumentParser()

parser.add_argument('-i'
                    , '--run-type', type=str
                    , default='run3auau'
                    , choices=['run2pp','run2auau','run3auau']
                    , help='Run Type. Default: run3auau')

parser.add_argument('-f'
                    , '--bin-filter-datasets', type=str
                    , default=''
                    , help='Filter Datasets Bin. Default: Official')

parser.add_argument('-f2'
                    , '--bin-genStatus', type=str
                    , default=''
                    , help='Gen Status Bin. Default: Official')

parser.add_argument('-e'
                    , '--condor-script', type=str
                    , default='scripts/genStatus.sh'
                    , help='Condor Script. Default: scripts/genStatus.sh')

parser.add_argument('-o'
                    , '--output', type=str
                    , default='test'
                    , help='Output directory for condor.')

parser.add_argument('-m'
                    , '--memory', type=float
                    , default=0.5
                    , help='Memory (units of GB) to request per condor submission. Default: 0.5 GB.')

parser.add_argument('-l'
                    , '--condor-log-dir', type=str
                    , default=''
                    , help='Condor log file.')

parser.add_argument('-s'
                    , '--do-condor-submit', action='store_true'
                    , help='Run the Condor Submission.')

parser.add_argument('-v'
                    , '--verbose', action='store_true'
                    , help='Verbose.')

def get_file_paths(engine, runtype='run3auau'):
    """
    Generate file paths from given minimum events and run type.
    """

    # Identify run range from the run type
    run_ranges = {'run2pp': (47286, 53880), 'run2auau': (54128, 54974), 'run3auau': (66457, 20000000)}
    params = {'run_start': run_ranges[runtype][0], 'run_end': run_ranges[runtype][1]}

    query = """
    -- Use a Common Table Expression (CTE) to find the winning tag for each runnumber
    WITH WinningTags AS (
        SELECT
            runnumber,
            tag
        FROM (
            -- This inner query ranks the tags within each runnumber group
            SELECT
                d.runnumber,
                d.tag,
                -- The tag with the latest max timestamp gets rank '1'
                ROW_NUMBER() OVER (PARTITION BY d.runnumber ORDER BY MAX(f.time) DESC) as rn
            FROM
                datasets d
            JOIN
                files f
            ON
                d.filename = f.lfn
            WHERE
                d.dsttype LIKE 'HIST_CALOQA%'
                AND d.dsttype NOT LIKE 'HIST_CALOQASKIMMED%'
                AND d.tag IS NOT NULL AND d.tag != ''
                AND d.runnumber >= :run_start AND d.runnumber <= :run_end
            GROUP BY
                d.runnumber, d.tag
        ) AS RankedTags
        WHERE
            rn = 1
    )
    -- Now, join the original table with the list of winning tags
    SELECT
        d.tag, d.runnumber, f.full_file_path
    FROM
        datasets d
    JOIN
        files f
    ON
        d.filename = f.lfn
    JOIN
        WinningTags wt
    ON
        d.runnumber = wt.runnumber AND d.tag = wt.tag
    WHERE
        d.dsttype LIKE 'HIST_CALOQA%'
        AND d.dsttype NOT LIKE 'HIST_CALOQASKIMMED%'
        AND d.segment != 9999;
    """

    df = pd.DataFrame()

    try:
        with engine.connect() as connection:
            df = pd.read_sql_query(text(query), connection, params=params)

    except Exception as e:
        print(f"An error occurred: {e}")
        print("Ensure database is running, tables exist, and query syntax is correct.")
        sys.exit()

    return df

def setup_logging(log_file, log_level):
    """Configures the logging system to output to a file and console."""

    logger.setLevel(log_level)

    # Clear existing handlers to prevent duplicate output if run multiple times
    if logger.hasHandlers():
        logger.handlers.clear()

    # # Create a formatter for log messages
    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')

    # Create a FileHandler to save logs to a file
    file_handler = logging.FileHandler(log_file)
    file_handler.setLevel(log_level)
    file_handler.setFormatter(formatter)
    logger.addHandler(file_handler)

def run_command_and_log(command, current_dir = '.', description="Executing command"):
    """
    Runs an external command using subprocess and logs its stdout, stderr, and return code.
    """
    logger.info(f"{description}: '{command}'")

    try:
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

    # Catch specific OS-related errors
    except OSError as e:
        logger.critical(f"An unexpected error occurred while running '{command}': {e}")
        return False

    except Exception as e:
        logger.critical(f"An unexpected error occurred while running '{command}': {e}")
        return False

def check_file_validity(path):
    """
    A simple helper function for the multiprocessing pool.
    It checks if the path exists AND if the file size is non-zero.
    """
    if os.path.exists(path):
        # Path exists, now check the size
        try:
            # os.path.getsize returns the size in bytes
            return os.path.getsize(path) > 0
        except OSError:
            # Handle cases where the path exists but we can't get the size (e.g., permissions issue, or it's a directory)
            return False
    else:
        # Path does not exist
        return False

def process_df(df, run_type, bin_filter_datasets, output, verbose=False):
    """
    Filter df and get a reduced df that contains the necessary runs with missing / outdated bad tower maps
    """
    if verbose:
        logger.info("Original")
        logger.info(df.head().to_string())
        logger.info(f'size: {len(df)}')
        logger.info(f'Runs: {df['runnumber'].nunique()}')
        logger.info("\n" + "="*70 + "\n")

    # Save CSV of unique run and tag pairs
    df[['runnumber', 'tag']].drop_duplicates().sort_values(by='runnumber').to_csv(output / f'{run_type}.csv', index=False, header=True)

    ## DEBUG
    command = f'{bin_filter_datasets} {output / f"{run_type}.csv"} {output}'
    run_command_and_log(command)

    processed_df = pd.read_csv(output / f'{run_type}-process.csv')

    # Check if any new runs need new cdb maps
    if len(processed_df) == 0:
        logger.info('No new CDB maps to process. Quitting.')
        sys.exit()

    if len(processed_df) > 20000:
        logger.critical(f'ERROR: Too many Runs: {len(processed_df)}. Quitting.')
        sys.exit()

    reduced_process_df = df.merge(processed_df)

    # Ensure that the file paths from the database actually exist
    logger.info(f'Current files: {len(reduced_process_df)}')
    logger.info('Checking file status')

    # Get the list of file paths to check
    file_paths = reduced_process_df['full_file_path'].tolist()

    # Determine the number of processes to use (usually the number of CPU cores)
    num_processes = cpu_count()
    logger.info(f'Using {num_processes} cores for parallel file checking.')

    # Create a pool of worker processes
    with Pool(processes=num_processes) as pool:
        # pool.map applies the check_path_exists function to each item in file_paths and returns the results as a list of booleans (True/False)
        mask_exists_list = pool.map(check_file_validity, file_paths)

    # The list of booleans can now be directly used as the mask
    mask_exists = pd.Series(mask_exists_list, index=reduced_process_df.index)

    df_filtered = reduced_process_df[mask_exists].copy()

    logger.info(f'Clean files: {len(df_filtered)}')
    logger.info(f'Missing files: {len(reduced_process_df[~mask_exists])}, {len(reduced_process_df[~mask_exists])*100//len(reduced_process_df)} %')

    reduced_process_df[~mask_exists].to_csv(output / f'{run_type}-missing.csv', columns=['full_file_path'], index=False, header=True)

    if verbose:
        logger.info("Final Reduced DataFrame that needs CDB Maps:")
        logger.info(df_filtered.head().to_string())
        logger.info(f'Runs: {df_filtered["runnumber"].nunique()}')

    return df_filtered

def generate_run_list(reduced_process_df, output):
    """
    Generate lists of CaloValid histogram for each run.
    """
    dataset_dir = output / 'datasets'
    dataset_dir.mkdir(parents=True, exist_ok=True)

    # 7. Group by 'runnumber' and 'dataset'
    # Iterating over this grouped object is efficient.
    grouped = reduced_process_df.groupby(['runnumber', 'tag'])

    # 8. Loop through each unique group
    for (run, tag), group_df in grouped:
        logger.info(f'Processing: {run},{tag}')

        filepath = dataset_dir / f'{run}_{tag}.list'

        group_df['full_file_path'].to_csv(filepath, index=False, header=False)

def generate_condor(output, condor_log_dir, condor_log_file, condor_memory, bin_genStatus, condor_script, do_condor_submit):
    """
    Generate condor submission directory to generate the CDB files for the runs.
    """
    # 9. Condor Submission
    if os.path.exists(condor_log_dir):
        shutil.rmtree(condor_log_dir)
        logger.info(f"Directory '{condor_log_dir}' and its contents removed.")

    condor_log_dir.mkdir(parents=True, exist_ok=True)

    shutil.copy(bin_genStatus, output)
    shutil.copy(condor_script, output)

    dataset_dir = output / 'datasets'
    list_files = list(dataset_dir.glob('*.list'))
    with open(output / 'jobs.list', 'w', encoding="utf-8") as f:
        for file_path in list_files:
            f.write(str(file_path.resolve()) + '\n')

    # list of subdirectories to create
    subdirectories = ['stdout', 'error', 'output']

    # Loop through the list and create each one
    for subdir in subdirectories:
        (output / subdir).mkdir(parents=True, exist_ok=True)

    submit_file_content = textwrap.dedent(f"""\
        arguments      = {output / os.path.basename(bin_genStatus)} $(input_run) {output / "output"}
        executable     = {os.path.basename(condor_script)}
        log            = {condor_log_file}
        output         = stdout/job-$(ClusterId)-$(Process).out
        error          = error/job-$(ClusterId)-$(Process).err
        request_memory = {condor_memory}GB
    """)

    with open(output / 'genStatus.sub', mode="w", encoding="utf-8") as file:
        file.write(submit_file_content)

    command = f'rm -rf {condor_log_dir} && mkdir {condor_log_dir} && cd {output} && condor_submit genStatus.sub -queue "input_run from jobs.list"'

    if do_condor_submit:
        run_command_and_log(command, output)
    else:
        logger.info(f'\nSubmission Command: {command}')

def main():
    """
    Main Function
    """
    args = parser.parse_args()
    run_type   = args.run_type
    CURRENT_DATE = str(datetime.date.today())
    output = Path(args.output).resolve()
    condor_memory = args.memory
    USER = os.environ.get('USER')
    condor_log_dir = Path(args.condor_log_dir).resolve() if args.condor_log_dir else Path(f'/tmp/{USER}/dump')
    condor_log_file = condor_log_dir / 'job-$(ClusterId)-$(Process).log'
    do_condor_submit = args.do_condor_submit
    verbose    = args.verbose

    # Append timestamp if the automated condor submission is enabled.
    # This will ensure the output directory is unique for each call of the cron job.
    if do_condor_submit:
        output += '-' + datetime.datetime.now().strftime("%Y-%m-%d-%H-%M-%S")

    log_file = output / f'log-{CURRENT_DATE}.txt'

    condor_script = Path(args.condor_script).resolve()
    offline_main = Path(os.environ.get('OFFLINE_MAIN'))
    if not offline_main:
        logger.critical("OFFLINE_MAIN environment variable not set, exiting.")
        sys.exit(1)
    OFFLINE_MAIN_BIN    = offline_main / 'bin'
    bin_filter_datasets = Path(args.bin_filter_datasets).resolve() if args.bin_filter_datasets else OFFLINE_MAIN_BIN / 'CaloCDB-FilterDatasets'
    bin_genStatus       = Path(args.bin_genStatus).resolve() if args.bin_genStatus else OFFLINE_MAIN_BIN / 'CaloCDB-GenStatus'

    output.mkdir(parents=True, exist_ok=True)

    setup_logging(log_file, logging.DEBUG)

    # Database Connection
    DB_NAME = "FileCatalog"
    DATABASE_URL = f"postgresql+psycopg2:///{DB_NAME}"
    logging.basicConfig()

    logger.info('#'*40)
    logger.info(f'LOGGING: {str(datetime.datetime.now())}')
    logger.info(f'Run Type: {run_type}')
    logger.info(f'Output Directory: {output}')
    logger.info(f'Condor Memory: {condor_memory}')
    logger.info(f'Do Condor Submission: {do_condor_submit}')
    logger.info(f'Filter Datasets Bin: {bin_filter_datasets}')
    logger.info(f'genStatus Bin: {bin_genStatus}')
    logger.info(f'Condor Script: {condor_script}')
    logger.info(f'Log File: {log_file}')
    logger.info(f'Condor Log File: {condor_log_file}')
    logger.info(f'Verbose: {verbose}')

    if not (os.path.exists(bin_filter_datasets) and os.path.exists(bin_genStatus) and os.path.exists(condor_script)):
        logger.info(f'One of {bin_filter_datasets} or {bin_genStatus} or {condor_script} does NOT exist!')
        sys.exit()

    if verbose:
        logging.getLogger('sqlalchemy.engine').setLevel(logging.INFO) # Set to logging.DEBUG for even more detail

    engine = create_engine(DATABASE_URL)

    # 1. Get the dataframe from the database
    df = get_file_paths(engine, run_type)

    # filter and process the initial dataframe
    reduced_process_df = process_df(df, run_type, bin_filter_datasets, output, verbose)

    # if there are no new runs to process then exit
    if reduced_process_df.empty:
        logger.info('No new runs to process. Quitting...')
        sys.exit()

    # generate the lists of CaloValid histograms for each identified run
    generate_run_list(reduced_process_df, output)

    # generate condor jobs / submit them
    generate_condor(output, condor_log_dir, condor_log_file, condor_memory, bin_genStatus, condor_script, do_condor_submit)

if __name__ == "__main__":
    main()
