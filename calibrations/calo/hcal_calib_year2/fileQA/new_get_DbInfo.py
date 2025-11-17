import psycopg2
import csv
from datetime import datetime

def get_run_data(run_number, conn):
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

    # Prepare the query to fetch rate_avg_value for the same run_number
    query2 = """
    SELECT runnumber, raw
    FROM gl1_scalers
    WHERE runnumber = %s
    AND index = 10
    ORDER BY runnumber DESC;
    """
    cursor.execute(query2, (run_number,))
    rate_value = cursor.fetchone()  # Get the run number and rate_avg_value data

    # Close the cursor
    cursor.close()

    return timestamp, rate_value

def main():
    # Read run numbers from runList.txt
    with open('runList.txt', 'r') as f:
        run_list = [line.strip() for line in f]

    # Connect to the PostgreSQL database
    conn = psycopg2.connect(
        host="sphnxdaqdbreplica",
        database="daq"
    )

    # Initialize an empty list to store the merged data
    merged_data = []

    # Loop through each run number in the run_list
    for run_number in run_list:
        # Get run data (timestamps and rate_avg_value) for the current run_number
        timestamp, rate_value = get_run_data(run_number, conn)

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
    with open('run_timestamps_with_rate.csv', 'w', newline='') as csvfile:
        csvwriter = csv.writer(csvfile)
        # Write the header
        csvwriter.writerow(['runnumber', 'timestamp', 'rate_avg_value'])
        # Write the merged data
        for row in merged_data:
            csvwriter.writerow(row)

    # Close the database connection
    conn.close()

if __name__ == "__main__":
    main()

