#!/usr/bin/env python3
"""
This module generates a list of run / datasets given a run type and event threshold.
"""
import argparse
import datetime
import glob
import logging
import os
import shutil
import subprocess
import sys

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

def get_file_paths(engine, runtype='run3auau', threshold=500000):
    """
    Generate file paths from given minimum events and run type.
    """

    # Identify run range from the run type
    run_ranges = {'run2pp': (47286, 53880), 'run2auau': (54128, 54974), 'run3auau': (66457, 20000000)}
    params = {'run_start': run_ranges[runtype][0], 'run_end': run_ranges[runtype][1], 'threshold': threshold}

    create_temp_table_query = """
    CREATE TEMPORARY TABLE t1 AS
    SELECT
        d.tag, d.runnumber, d.segment, d.events
    FROM
        datasets d
    JOIN (
        SELECT
            tag, runnumber, segment
        FROM
            datasets
        WHERE
            dsttype like 'HIST_CALOQA%' and runnumber >= :run_start and runnumber <= :run_end) h
    ON
        d.tag = h.tag AND d.runnumber = h.runnumber AND d.segment = h.segment
    WHERE
        d.dsttype like 'DST_CALOFITTING%';
    """

    query = """
    SELECT
        a.tag, a.runnumber, f.full_file_path, t1.events, f.time
    FROM
        datasets a
    JOIN (
        SELECT
            tag, runnumber
        FROM t1
        GROUP BY
            tag, runnumber
        HAVING
            SUM(events) > :threshold) k
    ON
        k.tag = a.tag AND k.runnumber = a.runnumber
    JOIN t1
    ON
        t1.tag = a.tag AND t1.runnumber = a.runnumber AND t1.segment = a.segment
    JOIN
        files f
    ON
        f.lfn = a.filename
    WHERE
        a.filename LIKE 'HIST_CALOQA%';
    """

    df = pd.DataFrame()

    try:
        with engine.connect() as connection:
            # Create the temporary table
            connection.execute(text(create_temp_table_query), params)

            # Use the temporary table
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

def process_df(df, run_type, bin_filter_datasets, output, threshold, verbose=False):
    """
    Filter df and get a reduced df that contains the necessary runs with missing / outdated bad tower maps
    """
    if verbose:
        logger.info("Original")
        logger.info(df.head().to_string())
        logger.info(f'size: {len(df)}')
        logger.info("\n" + "="*70 + "\n")

    # 2. Calculate the minimum time for each tag
    min_times_per_tag = df.groupby('tag')['time'].min().sort_values(ascending=False)

    if verbose:
        logger.info("Minimum time for each tag:")
        logger.info(min_times_per_tag.head().to_string())
        logger.info(f'size: {len(min_times_per_tag)}')
        logger.info("\n" + "="*70 + "\n")

    # 3. Add this minimum time back to the original DataFrame as 'tag_min_time'
    df_processed = df.merge(min_times_per_tag.rename('tag_min_time'),
                            left_on='tag',
                            right_index=True)

    if verbose:
        logger.info("DataFrame with 'tag_min_time' column:")
        logger.info(df_processed.head().to_string())
        logger.info(f'size: {len(df_processed)}')
        logger.info("\n" + "="*70 + "\n")

    # 4. For each 'runnumber', find the 'tag_min_time' of the HIGHEST PRIORITY tag containing it.
    #    "Highest priority" means the tag with the LATEST (maximum) 'tag_min_time'.
    highest_priority_time_for_runnumber = df_processed.groupby('runnumber')['tag_min_time'].max()
    highest_priority_time_for_runnumber.name = 'highest_priority_tag_min_time_for_runnumber'

    if verbose:
        logger.info("Highest Priority 'tag_min_time' for each 'runnumber':")
        logger.info(highest_priority_time_for_runnumber.head().to_string())
        logger.info(f'size: {len(highest_priority_time_for_runnumber)}')
        logger.info("\n" + "="*70 + "\n")

    # 5. Merge this information back to the DataFrame
    df_processed = df_processed.merge(highest_priority_time_for_runnumber,
                                    left_on='runnumber',
                                    right_index=True)

    if verbose:
        logger.info("DataFrame with 'highest_priority_tag_min_time_for_runnumber' column:")
        logger.info(df_processed[['tag', 'runnumber', 'time', 'full_file_path', 'tag_min_time', 'highest_priority_tag_min_time_for_runnumber']].head().to_string())
        logger.info(f'size: {len(df_processed)}')
        logger.info("\n" + "="*70 + "\n")

    # 6. Filter the DataFrame: Keep only rows where the row's 'tag_min_time'
    #    matches the 'highest_priority_tag_min_time_for_runnumber'.
    #    This ensures we keep ALL rows for a runnumber from its highest-priority tag.
    reduced_df = df_processed[
        df_processed['tag_min_time'] == df_processed['highest_priority_tag_min_time_for_runnumber']
    ]

    if verbose:
        logger.info("Final Reduced DataFrame (sorted by time for readability):")
        logger.info(reduced_df.sort_values(by='time').reset_index(drop=True).head().to_string())

    # Save CSV of unique run and tag pairs
    reduced_df[['runnumber', 'tag']].drop_duplicates().sort_values(by='runnumber').to_csv(os.path.join(output, f'{run_type}.csv'), index=False, header=True)

    ## DEBUG
    command = f'{bin_filter_datasets} {os.path.join(output, f"{run_type}.csv")} {output}'
    run_command_and_log(command)

    processed_df = pd.read_csv(os.path.join(output, f'{run_type}-process.csv'))

    # Check if any new runs need new cdb maps
    if len(processed_df) == 0:
        logger.info('No new CDB maps to process. Quitting.')
        sys.exit()

    if len(processed_df) > 20000:
        logger.critical(f'ERROR: Too many Runs: {len(processed_df)}. Quitting.')
        sys.exit()

    reduced_process_df = reduced_df.merge(processed_df)

    # Ensure that the file paths from the database actually exist
    logger.info(f'Current files: {len(reduced_process_df)}')
    logger.info('Checking file status')

    mask_exists = reduced_process_df['full_file_path'].apply(os.path.exists)

    df_filtered = reduced_process_df[mask_exists]

    logger.info(f'Clean files: {len(df_filtered)}')
    logger.info(f'Missing files: {len(reduced_process_df[~mask_exists])}, {len(reduced_process_df[~mask_exists])*100//len(reduced_process_df)} %')

    reduced_process_df[~mask_exists].to_csv(os.path.join(output, f'{run_type}-missing.csv'), columns=['full_file_path'], index=False, header=True)

    df_filtered['group_total_events'] = df_filtered.groupby(['tag', 'runnumber'])['events'].transform('sum')
    df_final = df_filtered[df_filtered['group_total_events'] > threshold].copy()
    df_drop = df_filtered[df_filtered['group_total_events'] < threshold].copy()

    if verbose:
        logger.info("Final Reduced DataFrame that needs CDB Maps:")
        logger.info(df_final.head().to_string())
        logger.info(f'Runs: {df_final["runnumber"].nunique()}')
        logger.info(f'Runs Dropped: {df_drop["runnumber"].nunique()}')

    return df_final

def generate_run_list(reduced_process_df, output):
    """
    Generate lists of CaloValid histogram for each run.
    """
    dataset_dir = os.path.join(output, 'datasets')
    os.makedirs(dataset_dir, exist_ok=True)

    # 7. Group by 'runnumber' and 'dataset'
    # Iterating over this grouped object is efficient.
    grouped = reduced_process_df.groupby(['runnumber', 'tag'])

    # 8. Loop through each unique group
    for (run, tag), group_df in grouped:
        logger.info(f'Processing: {run},{tag}')

        filepath = os.path.join(dataset_dir, f'{run}_{tag}.list')

        group_df['full_file_path'].to_csv(filepath, index=False, header=False)


def generate_condor(output, condor_log_dir, condor_log_file, condor_memory, bin_genStatus, condor_script, do_condor_submit):
    """
    Generate condor submission directory to generate the CDB files for the runs.
    """
    # 9. Condor Submission
    if os.path.exists(condor_log_dir):
        shutil.rmtree(condor_log_dir)
        logger.info(f"Directory '{condor_log_dir}' and its contents removed.")

    os.makedirs(condor_log_dir, exist_ok=True)

    shutil.copy(bin_genStatus, output)
    shutil.copy(condor_script, output)

    dataset_dir = os.path.join(output, 'datasets')
    list_files = glob.glob(os.path.join(dataset_dir, '*.list'))
    with open(os.path.join(output, 'jobs.list'), 'w', encoding="utf-8") as f:
        for file_path in list_files:
            f.write(os.path.realpath(file_path) + '\n')

    os.makedirs(os.path.join(output,'stdout'),exist_ok=True)
    os.makedirs(os.path.join(output,'error'),exist_ok=True)
    os.makedirs(os.path.join(output,'output'),exist_ok=True)

    with open(os.path.join(output,'genStatus.sub'), mode="w", encoding="utf-8") as file:
        file.write(f'arguments      = {os.path.join(output, os.path.basename(bin_genStatus))} $(input_run) {os.path.join(output, "output")}\n')
        file.write(f'executable     = {os.path.basename(condor_script)}\n')
        file.write(f'log            = {condor_log_file}\n')
        file.write('output          = stdout/job-$(ClusterId)-$(Process).out\n')
        file.write('error           = error/job-$(ClusterId)-$(Process).err\n')
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
    args = parser.parse_args()
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

    # Append timestamp if the automated condor submission is enabled.
    # This will ensure the output directory is unique for each call of the cron job.
    if do_condor_submit:
        output += '-' + datetime.datetime.now().strftime("%Y-%m-%d-%H-%M-%S")

    log_file = os.path.join(output, f'log-{CURRENT_DATE}.txt')

    condor_script       = os.path.realpath(args.condor_script)
    offline_main = os.environ.get('OFFLINE_MAIN')
    if not offline_main:
        logger.critical("OFFLINE_MAIN environment variable not set, exiting.")
        sys.exit(1)
    OFFLINE_MAIN_BIN    = os.path.join(offline_main, 'bin')
    bin_filter_datasets = os.path.realpath(args.bin_filter_datasets) if args.bin_filter_datasets else os.path.join(OFFLINE_MAIN_BIN, 'CaloCDB-FilterDatasets')
    bin_genStatus       = os.path.realpath(args.bin_genStatus) if args.bin_genStatus else os.path.join(OFFLINE_MAIN_BIN, 'CaloCDB-GenStatus')

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
    df = get_file_paths(engine, run_type, min_events)

    # filter and process the initial dataframe
    reduced_process_df = process_df(df, run_type, bin_filter_datasets, output, min_events, verbose)

    # generate the lists of CaloValid histograms for each identified run
    generate_run_list(reduced_process_df, output)

    # generate condor jobs / submit them
    generate_condor(output, condor_log_dir, condor_log_file, condor_memory, bin_genStatus, condor_script, do_condor_submit)

if __name__ == "__main__":
    main()
