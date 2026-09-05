import sys
import requests
import numpy as np
import os
import csv
from collections import defaultdict
from sqlalchemy import create_engine, MetaData, Table, text
from sqlalchemy.orm import sessionmaker
from datetime import datetime

variable_names = ['sphenix_tpc_hv_caen_IMon']
output_folder = 'tpc_GEM_current_status'

# BCO conversion constant
S = 1672549200


def to_bco(ts):
    return int((ts - S) * 56299000 // 6)


def process_run_data(runnumber):

    # ---------------------------
    # DB query run time
    # ---------------------------
    source_engine = create_engine('postgresql://phnxrc@sphnxdaqdbreplica.sdcc.bnl.gov/daq')
    Session = sessionmaker(bind=source_engine)
    session = Session()

    query = text("""
        SELECT
            extract(epoch from brtimestamp AT TIME ZONE 'America/New_York') AS begin_run,
            extract(epoch from ertimestamp AT TIME ZONE 'America/New_York') AS end_run
        FROM run
        WHERE runnumber = :runnumber
    """)

    result = session.execute(query, {'runnumber': runnumber}).fetchone()
    session.close()

    if not result or result.begin_run is None or result.end_run is None:
        print("No valid run timing")
        return

    begin_run = int(result.begin_run)
    end_run = int(result.end_run)

    print("Begin:", begin_run)
    print("End:", end_run)

    # ---------------------------
    # storage
    # ---------------------------
    bco_table = defaultdict(list)

    # ---------------------------
    # Prometheus query
    # ---------------------------
    for variable_name in variable_names:

        params = {
            'query': variable_name,
            'start': begin_run,
            'end': end_run,
            'step': '1m'
        }

        resp = requests.get(
            "http://promspx01.sdcc.bnl.local:9090/api/v1/query_range",
            params=params
        )

        if resp.status_code != 200:
            continue

        result_json = resp.json()
        if result_json['status'] != 'success':
            continue

        metrics = result_json['data']['result']

        # ---------------------------
        # loop over detector channels
        # ---------------------------
        for metric in metrics:

            labels = metric['metric']

            HV_Layer = labels.get('HV_Layer', 'unknown')
            if HV_Layer != 'G4':
                continue

            side = labels.get('side', 'unknown')
            sector = labels.get('sector', '-1')
            R_Module = labels.get('R_Module', 'unknown')

            key = (side, sector, R_Module, HV_Layer)

            values = [
                (to_bco(v[0]), float(v[1]))
                for v in metric['values']
            ]

            values.sort(key=lambda x: x[0])

            bco_table[key].extend(values)

    # ---------------------------
    # build BCO axis and channel lookup
    # ---------------------------
    if not bco_table:
        print("No data")
        return

    all_bcos = set()
    channel_values = {}

    for key, series in bco_table.items():
        values_by_bco = {}

        for bco, current in series:
            values_by_bco[bco] = current
            all_bcos.add(bco)

        channel_values[key] = values_by_bco

    sorted_bcos = sorted(all_bcos)
    sorted_channels = sorted(channel_values)

    # ---------------------------
    # write CSV
    # ---------------------------
    os.makedirs(output_folder, exist_ok=True)

    out_file = f"{output_folder}/run_{runnumber}_GEM_BCO.csv"

    with open(out_file, "w") as f:

        for bco in sorted_bcos:
            f.write(f"bco {bco}\n")

            for channel in sorted_channels:
                current = channel_values[channel].get(bco)
                if current == "" or current is None:
                    continue

                label = "".join(channel)
                f.write(f"{label} {current}\n")

    print("Wrote:", out_file)


if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Usage: python3 script.py <runnumber>")
        sys.exit(1)

    process_run_data(int(sys.argv[1]))
