#!/usr/bin/env python3
"""
This module generates a list of run / datasets given a run type and event threshold.
"""
import argparse
import os
import logging
import sys
import subprocess
import datetime
import shutil
import pandas as pd
from sqlalchemy import create_engine

parser = argparse.ArgumentParser()

parser.add_argument('-i'
                    , '--run-type', type=str
                    , default='run3auau'
                    , choices=['run2pp','run2auau','run3auau']
                    , help='Run Type. Default: run3auau')

parser.add_argument('-f'
                    , '--bin-filter-datasets', type=str
                    , default='/direct/sphenix+u/anarde/Documents/sPHENIX/analysis-CaloCDB/CaloCDB/bin/filter-datasets'
                    , help='Filter Datasets Bin. Default: /direct/sphenix+u/anarde/Documents/sPHENIX/analysis-CaloCDB/CaloCDB/bin/filter-datasets')

parser.add_argument('-f2'
                    , '--bin-genStatus', type=str
                    , default='/direct/sphenix+u/anarde/Documents/sPHENIX/analysis-CaloCDB/CaloCDB/bin/genStatus'
                    , help='Gen Status Bin. Default: /direct/sphenix+u/anarde/Documents/sPHENIX/analysis-CaloCDB/CaloCDB/bin/genStatus')

parser.add_argument('-e'
                    , '--executable', type=str
                    , default='/direct/sphenix+u/anarde/Documents/sPHENIX/analysis-CaloCDB/CaloCDB/scripts/genStatus.sh'
                    , help='Condor Script. Default: /direct/sphenix+u/anarde/Documents/sPHENIX/analysis-CaloCDB/CaloCDB/scripts/genStatus.sh')

parser.add_argument('-n'
                    , '--min-events', type=int
                    , default=500000
                    , help='Minimum Events (for Run). Default: 500k')

parser.add_argument('-o'
                    , '--output', type=str
                    , default='test'
                    , help='Output directory for condor.')

parser.add_argument('-m'
                    , '--memory', type=float
                    , default=0.2
                    , help='Memory (units of GB) to request per condor submission. Default: 0.2 GB.')

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

args = parser.parse_args()

def get_file_paths(engine, runtype='run3auau', threshold=500000):
    """
    Generate file paths from given minimum events and run type.
    """

    query = f"""
    SELECT
        a.dataset, a.runnumber, f.full_file_path, f.time
    FROM
        datasets a
    JOIN (
            SELECT
                d.dataset, d.runnumber
            FROM
                datasets d
            JOIN (
                SELECT
                    dataset, runnumber, segment
                FROM
                    datasets
                WHERE
                    dsttype = 'HIST_CALOQA_{runtype}') h
            ON
                d.dataset = h.dataset AND d.runnumber = h.runnumber AND d.segment = h.segment
            WHERE
                d.dsttype LIKE 'DST_CALO_{runtype}'
            GROUP BY
                d.dataset, d.runnumber
            HAVING
                SUM(d.events) > {threshold}) k
    ON
        k.dataset = a.dataset AND k.runnumber = a.runnumber
    JOIN
        files f
    ON
        f.lfn = a.filename
    WHERE
        a.filename LIKE %(filename_pattern)s;
    """
    parameters = {'filename_pattern': f'HIST_CALOQA_{runtype}%'}

    return pd.read_sql_query(query, engine, params=parameters)

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

def process_df(df, run_type, bin_filter_datasets, output, verbose=False):
    """
    Filter df and get a reduced df that contains the necessary runs with missing / outdated bad tower maps
    """
    if verbose:
        logger.info("Original")
        logger.info(df.head().to_string())
        logger.info("\n" + "="*70 + "\n")

    # 2. Calculate the minimum time for each dataset
    min_times_per_dataset = df.groupby('dataset')['time'].min().sort_values(ascending=False)

    if verbose:
        logger.info("Minimum time for each dataset:")
        logger.info(min_times_per_dataset.head().to_string())
        logger.info("\n" + "="*70 + "\n")

    # 3. Add this minimum time back to the original DataFrame as 'dataset_min_time'
    df_processed = df.merge(min_times_per_dataset.rename('dataset_min_time'),
                            left_on='dataset',
                            right_index=True)

    if verbose:
        logger.info("DataFrame with 'dataset_min_time' column:")
        logger.info(df_processed[['dataset', 'runnumber', 'time', 'full_file_path', 'dataset_min_time']].head().to_string())
        logger.info("\n" + "="*70 + "\n")

    # 4. For each 'runnumber', find the 'dataset_min_time' of the HIGHEST PRIORITY dataset containing it.
    #    "Highest priority" means the dataset with the LATEST (maximum) 'dataset_min_time'.
    highest_priority_time_for_runnumber = df_processed.groupby('runnumber')['dataset_min_time'].max()
    highest_priority_time_for_runnumber.name = 'highest_priority_dataset_min_time_for_runnumber'

    if verbose:
        logger.info("Highest Priority 'dataset_min_time' for each 'runnumber':")
        logger.info(highest_priority_time_for_runnumber.head().to_string())
        logger.info("\n" + "="*70 + "\n")

    # 5. Merge this information back to the DataFrame
    df_processed = df_processed.merge(highest_priority_time_for_runnumber,
                                    left_on='runnumber',
                                    right_index=True)

    if verbose:
        logger.info("DataFrame with 'highest_priority_dataset_min_time_for_runnumber' column:")
        logger.info(df_processed[['dataset', 'runnumber', 'time', 'full_file_path', 'dataset_min_time', 'highest_priority_dataset_min_time_for_runnumber']].head().to_string())
        logger.info("\n" + "="*70 + "\n")

    # 6. Filter the DataFrame: Keep only rows where the row's 'dataset_min_time'
    #    matches the 'highest_priority_dataset_min_time_for_runnumber'.
    #    This ensures we keep ALL rows for a runnumber from its highest-priority dataset.
    reduced_df = df_processed[
        df_processed['dataset_min_time'] == df_processed['highest_priority_dataset_min_time_for_runnumber']
    ]

    if verbose:
        logger.info("Final Reduced DataFrame (sorted by time for readability):")
        logger.info(reduced_df.sort_values(by='time').reset_index(drop=True).head().to_string())

    # Save CSV of unique run and dataset pairs
    reduced_df[['runnumber', 'dataset']].drop_duplicates().sort_values(by='runnumber').to_csv(f'{output}/{run_type}.csv', index=False, header=True)

    ## DEBUG
    command = f'{bin_filter_datasets} {output}/{run_type}.csv {output}'
    run_command_and_log(command)

    processed_df = pd.read_csv(f'{output}/{run_type}-process.csv')

    # Check if any new runs need new cdb maps
    if len(processed_df) == 0:
        logger.info('No new CDB maps to process. Quitting.')
        sys.exit()

    if len(processed_df) > 20000:
        logger.critical(f'ERROR: Too many Runs: {len(processed_df)}. Quitting.')
        sys.exit()

    reduced_process_df = reduced_df.merge(processed_df)

    if verbose:
        logger.info("Final Reduced DataFrame that needs CDB Maps:")
        logger.info(reduced_process_df.head().to_string())
        logger.info(f'Runs: {len(processed_df)}')

    return reduced_process_df

def generate_run_list(reduced_process_df, output):
    """
    Generate lists of CaloValid histogram for each run.
    """
    dataset_dir = f'{output}/datasets'
    os.makedirs(dataset_dir, exist_ok=True)

    # 7. Group by 'runnumber' and 'dataset'
    # Iterating over this grouped object is efficient.
    grouped = reduced_process_df.groupby(['runnumber', 'dataset'])

    # 8. Loop through each unique group
    for (run, dataset), group_df in grouped:
        logger.info(f'Processing: {run},{dataset}')

        filepath = os.path.join(dataset_dir, f'{run}_{dataset}.list')

        group_df['full_file_path'].to_csv(filepath, index=False, header=False)

def generate_condor(output, condor_log_dir, condor_log_file, condor_memory, bin_genStatus, executable, do_condor_submit):
    """
    Generate condor submission directory to generate the CDB files for the runs.
    """
    # 9. Condor Submission
    if os.path.exists(condor_log_dir):
        shutil.rmtree(condor_log_dir)
        logger.info(f"Directory '{condor_log_dir}' and its contents removed.")

    os.makedirs(condor_log_dir, exist_ok=True)

    shutil.copy(bin_genStatus, output)
    shutil.copy(executable, output)

    command = f'readlink -f {output}/datasets/* > jobs.list'
    subprocess.run(['bash','-c',command], cwd=output, check=False)

    os.makedirs(f'{output}/stdout',exist_ok=True)
    os.makedirs(f'{output}/error',exist_ok=True)
    os.makedirs(f'{output}/output',exist_ok=True)

    with open(f'{output}/genStatus.sub', mode="w", encoding="utf-8") as file:
        file.write(f'executable     = {os.path.basename(executable)}\n')
        file.write(f'arguments      = {output}/{os.path.basename(bin_genStatus)} $(input_run) {output}/output\n')
        file.write(f'log            = {condor_log_file}\n')
        file.write('output          = stdout/job-$(Process).out\n')
        file.write('error           = error/job-$(Process).err\n')
        file.write(f'request_memory = {condor_memory}GB\n')

    command = f'rm -rf {condor_log_dir} && mkdir {condor_log_dir} && cd {output} && condor_submit genStatus.sub -queue "input_run from jobs.list"'

    if do_condor_submit:
        run_command_and_log(command, output)
    else:
        logger.info(f'\nSubmission Command: {command}')

def main():
    """
    Main Function
    """

    run_type   = args.run_type
    min_events = args.min_events
    CURRENT_DATE = str(datetime.date.today())
    output = os.path.realpath(args.output)
    condor_memory = args.memory
    USER = os.environ.get('USER')
    condor_log_dir = os.path.realpath(args.condor_log_dir) if args.condor_log_dir else f'/tmp/{USER}/dump'
    condor_log_file = os.path.join(condor_log_dir, 'job-$(ClusterId)-$(Process).log')
    do_condor_submit = args.do_condor_submit
    verbose    = args.verbose

    log_file = os.path.join(output, f'log-{CURRENT_DATE}.txt')

    bin_filter_datasets = os.path.realpath(args.bin_filter_datasets)
    bin_genStatus       = os.path.realpath(args.bin_genStatus)
    executable          = os.path.realpath(args.executable)

    os.makedirs(output, exist_ok=True)

    setup_logging(log_file, logging.DEBUG)

    # Database Connection
    DB_NAME = "FileCatalog"
    DATABASE_URL = f"postgresql+psycopg2:///{DB_NAME}"
    logging.basicConfig()

    logger.info('#'*40)
    logger.info(f'LOGGING: {str(datetime.datetime.now())}')
    logger.info(f'Run Type: {run_type}')
    logger.info(f'Min Events: {min_events}')
    logger.info(f'Output Directory: {output}')
    logger.info(f'Condor Memory: {condor_memory}')
    logger.info(f'Do Condor Submission: {do_condor_submit}')
    logger.info(f'Filter Datasets Bin: {bin_filter_datasets}')
    logger.info(f'Log File: {log_file}')
    logger.info(f'Condor Log File: {condor_log_file}')
    logger.info(f'Verbose: {verbose}')

    if verbose:
        logging.getLogger('sqlalchemy.engine').setLevel(logging.INFO) # Set to logging.DEBUG for even more detail

    engine = create_engine(DATABASE_URL)

    # 1. Get the dataframe from the database
    df = get_file_paths(engine, run_type, min_events)

    # filter and process the initial dataframe
    reduced_process_df = process_df(df, run_type, bin_filter_datasets, output, verbose)

    # generate the lists of CaloValid histograms for each identified run
    generate_run_list(reduced_process_df, output)

    # generate condor jobs / submit them
    generate_condor(output, condor_log_dir, condor_log_file, condor_memory, bin_genStatus, executable, do_condor_submit)

logger = logging.getLogger(__name__)

if __name__ == "__main__":
    main()
