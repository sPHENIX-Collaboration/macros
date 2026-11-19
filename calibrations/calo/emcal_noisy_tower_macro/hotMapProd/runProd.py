#!/usr/bin/env python
import os
import pyodbc
import subprocess

def get_unique_run_dataset_pairs(cursor):
    query = """
    SELECT runnumber, dataset
    FROM datasets
    WHERE filename LIKE 'DST_CALO%' AND runnumber > 42635
    GROUP BY runnumber, dataset
    HAVING SUM(events) > 500000;
    """
    cursor.execute(query)
    run_dataset_pairs = {(row.runnumber, row.dataset) for row in cursor.fetchall()}
    return run_dataset_pairs

def get_total_events(cursor, run):
    query = f"""
    SELECT
        d.dataset,
        SUM(d.events) AS total_events
    FROM
        datasets d
    JOIN
        (SELECT DISTINCT segment, dataset
         FROM datasets
         WHERE filename LIKE 'HIST_CALO%{run}%') h
    ON
        d.segment = h.segment AND d.dataset = h.dataset
    WHERE
        d.filename LIKE 'DST_CALO%{run}%'
    GROUP BY
        d.dataset;
    """
    cursor.execute(query)
    results = cursor.fetchall()
    return results

def get_file_paths(cursor, run, dataset):
    query = f"""
    SELECT full_file_path
    FROM files
    WHERE lfn LIKE 'HIST_CALO%{dataset}%-000{run}%.root';
    """
    cursor.execute(query)
    file_paths = [row.full_file_path for row in cursor.fetchall()]
    return file_paths

def main():
    conn = pyodbc.connect("DSN=FileCatalog;UID=phnxrc;READONLY=True")
    cursor = conn.cursor()

    if not os.path.exists('completedruns.txt'):
        open('completedruns.txt', 'w').close()

    if not os.path.exists('mergedQA'):
        os.makedirs('mergedQA')
    if not os.path.exists('hotMaps'):
        os.makedirs('hotMaps')

    unique_run_dataset_pairs = get_unique_run_dataset_pairs(cursor)

    with open('completedruns.txt') as f:
        completed_runs_datasets = set(line.strip() for line in f)

    remaining_runs_datasets = [(run, dataset) for run, dataset in unique_run_dataset_pairs if f"{run},{dataset}" not in completed_runs_datasets]

    for run, dataset in remaining_runs_datasets:
        results = get_total_events(cursor, run)
        print(f"looking at run {run}")

        if not results:
            print(f"No histograms for run {run}")
            continue

        for result in results:
            result_dataset, total_events = result
            if result_dataset != dataset:
                continue  # Skip if the dataset does not match
            print(f"run: {run}  dataset: {result_dataset}  total events in hists = {total_events}")

            if total_events > 500000:
                file_paths = get_file_paths(cursor, run, result_dataset)
                output_file = f"mergedQA/HIST_CALO_{result_dataset}-{run}.root"
            
                if not os.path.exists(output_file):
                    with open('files.txt', 'w') as f:
                        for path in file_paths:
                            f.write(f"{path}\n")
            
                    subprocess.run(["hadd", "-ff", output_file] + file_paths)
                else:
                    print(f"{output_file} already exists, skipping hadd command.")

                subprocess.run(["root", "-b", "-q", f"doFindTowersEMCal.C(\"{output_file}\",\"hotMaps/EMCalHotMap_{result_dataset}-{run}.root\")"])

                with open('completedruns.txt', 'a') as f:
                    f.write(f"{run},{result_dataset}\n")
            else:
                print("waiting on that run")

    conn.close()
    print("All done")

if __name__ == "__main__":
    main()

