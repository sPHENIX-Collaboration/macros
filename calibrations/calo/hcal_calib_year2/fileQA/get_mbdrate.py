import argparse
import csv
import os
import subprocess

import psycopg2

# Start time and MBD rate of each beam run, from the DAQ DB, for plot_hits.C.
# rate = scaler counts of the MBD trigger / run duration (Hz)
#
# usage: python3 get_mbdrate.py run3oo              run list of the dataset in ../HcalCalibDatasetinputs.h
#        python3 get_mbdrate.py run3oo -r runList.txt
#
# output: output/run_timestamps_with_rate_<dataset>.csv  (runnumber, timestamp, rate_avg_value)
#
# The GL1 index of a trigger changes between datasets (e.g. "MBD N&S >= 1" is index 10 in p+p 2024 and
# O+O 2025, index 11 in Au+Au), so the index is looked up by the trigger name for each run.

HERE = os.path.dirname(os.path.abspath(__file__))


def dataset_runlist(dataset):
    # run list of the dataset from ../HcalCalibDatasetinputs.h
    out = subprocess.run(['root', '-l', '-b', '-q', f'PrintHcalCalibDatasetinputs.C("{dataset}")'],
                         cwd=os.path.join(HERE, '..'), capture_output=True, text=True).stdout
    for line in out.splitlines():
        if line.startswith('HCALCALIB_RUNLIST='):
            return line.split('=', 1)[1].strip()
    raise SystemExit(f"no run list for dataset {dataset} in HcalCalibDatasetinputs.h:\n{out}")


def get_trigger_index(run_number, trigger_name, cursor):
    # GL1 index of the trigger in this run, None if the trigger is not there
    query = """
    SELECT index
    FROM gl1_triggernames
    WHERE runnumber <= %s AND runnumber_last >= %s AND triggername = %s;
    """
    cursor.execute(query, (run_number, run_number, trigger_name))
    row = cursor.fetchone()
    return None if row is None else row[0]


def get_run_data(run_number, trigger_index, conn):
    # Prepare a cursor
    cursor = conn.cursor()

    # Prepare the query to fetch the timestamps for a specific run_number
    query1 = """
    SELECT runnumber, brtimestamp, ertimestamp
    FROM run
    WHERE runnumber = %s
    ORDER BY runnumber DESC;
    """
    cursor.execute(query1, (run_number,))
    timestamp = cursor.fetchone()  # Get the run number and timestamp data

    # Prepare the query to fetch the scaler counts of the trigger for the same run_number
    rate_value = None
    if trigger_index is not None:
        query2 = """
        SELECT runnumber, raw
        FROM gl1_scalers
        WHERE runnumber = %s
        AND index = %s
        ORDER BY runnumber DESC;
        """
        cursor.execute(query2, (run_number, trigger_index))
        rate_value = cursor.fetchone()  # Get the run number and rate_avg_value data

    # Close the cursor
    cursor.close()

    return timestamp, rate_value


def main():
    parser = argparse.ArgumentParser(description='start time and MBD rate of each beam run')
    parser.add_argument('dataset', help='dataset in ../HcalCalibDatasetinputs.h (run2pp, run2auau, run3oo, ...)')
    parser.add_argument('-r', '--runlist', default='', help='run list, one run per line (default: run list of the dataset)')
    parser.add_argument('-t', '--trigger', default='MBD N&S >= 1', help='GL1 trigger name of the rate (default: "MBD N&S >= 1")')
    parser.add_argument('-o', '--output', default='', help='output csv (default: output/run_timestamps_with_rate_<dataset>.csv)')
    args = parser.parse_args()

    runlist = args.runlist or dataset_runlist(args.dataset)
    output = args.output or os.path.join('output', f'run_timestamps_with_rate_{args.dataset}.csv')
    os.makedirs(os.path.dirname(output) or '.', exist_ok=True)
    print(f'dataset {args.dataset}, run list {runlist}, trigger "{args.trigger}", output {output}')

    # Read run numbers from the run list
    with open(runlist, 'r') as f:
        run_list = [line.strip() for line in f if line.strip() and not line.startswith('#')]

    # Connect to the PostgreSQL database
    conn = psycopg2.connect(
        host="sphnxdaqdbreplica",
        database="daq"
    )

    # Initialize an empty list to store the merged data
    merged_data = []

    # Loop through each run number in the run_list
    for run_number in run_list:
        cursor = conn.cursor()
        trigger_index = get_trigger_index(int(run_number), args.trigger, cursor)
        cursor.close()
        if trigger_index is None:
            print(f'Run {run_number} has no trigger "{args.trigger}". Setting default rate value to 0.')

        # Get run data (timestamps and scaler counts) for the current run_number
        timestamp, rate_value = get_run_data(run_number, trigger_index, conn)

        # Initialize default values
        begin_time = 0
        end_time = 0
        rate_avg_value = 0

        # Check if timestamp or rate_value is None and assign default values
        if timestamp is None:
            print(f"Run {run_number} has missing timestamp data. Setting default timestamp values.")
        else:
            begin_time, end_time = timestamp[1], timestamp[2]

        if begin_time is None or end_time is None:
            print(f"Run {run_number} has missing timestamp. Setting default timestamp values.")
            begin_time = 0
            end_time = 0

        # Calculate the duration (in seconds) as a float, handling invalid times
        if begin_time != 0 and end_time != 0:
            duration = (end_time - begin_time).total_seconds()
        else:
            duration = 0  # Set to 0 if there's an issue with timestamps

        # Check and assign rate_avg_value
        if rate_value is None or rate_value[1] is None:
            print(f"Run {run_number} has missing rate data. Setting default rate value to 0.")
            rate_avg_value = 0
        else:
            rate_avg_value = rate_value[1] / duration if duration > 0 else 0

        # Append the result for this run
        merged_data.append([run_number, begin_time, rate_avg_value])

    # Write the results to a CSV file
    with open(output, 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        # Write the header
        csvwriter.writerow(['runnumber', 'timestamp', 'rate_avg_value'])
        # Write the merged data
        for row in merged_data:
            csvwriter.writerow(row)

    # Close the database connection
    conn.close()
    print(f'wrote {len(merged_data)} runs to {output}')


if __name__ == "__main__":
    main()
