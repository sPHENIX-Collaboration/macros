import sys
import requests
import numpy as np
import os
import csv
from collections import defaultdict
from sqlalchemy import create_engine, MetaData, Table, text
from sqlalchemy.orm import sessionmaker
from datetime import datetime
from environment_history import EnvironmentHistory
from trip_history import TripHistory

variable_names = ['sphenix_tpc_hv_caen_IMon']
output_folder = 'ConditionsCSVFiles'

# BCO conversion constant
S = 1672549200


def to_bco(ts):
    return int((ts - S) * 56299000 // 6)

def fill_current_gaps(bco_table, bco_to_time, step=60):
    """
    Fill missing Prometheus current samples by carrying the previous
    measured current forward.

    A missing Prometheus record means that the current was not measured;
    it does NOT mean that the physical GEM current dropped to zero.
    Filling the gap preserves the last known physical state until a new
    measurement becomes available.
    """

    for channel, series in bco_table.items():

        # Work in timestamp space because Prometheus sampling is defined
        # in seconds, while BCO is only the final CDB coordinate.
        time_values = []

        for bco, current in series:
            time_values.append(
                (bco_to_time[bco], current)
            )

        time_values.sort(key=lambda x: x[0])

        filled_values = []

        for i in range(len(time_values) - 1):

            timestamp, current = time_values[i]
            next_timestamp, next_current = time_values[i + 1]

            filled_values.append(
                (to_bco(timestamp), current)
            )

            gap_width = (
                int(round(
                    (next_timestamp - timestamp) / step
                )) - 1
            )

            for j in range(1, gap_width + 1):

                fill_time = timestamp + j * step
                fill_bco = to_bco(fill_time)

                filled_values.append(
                    (fill_bco, current)
                )

                bco_to_time[fill_bco] = fill_time

        # Don't forget the final real measurement.
        if time_values:
            timestamp, current = time_values[-1]

            filled_values.append(
                (to_bco(timestamp), current)
            )

        bco_table[channel] = filled_values

def process_run_data(runnumber, environment_history):

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

    MONITORING_PADDING = 120
    query_begin = begin_run - MONITORING_PADDING
    query_end = end_run + MONITORING_PADDING

    print("Run Begin:", begin_run)
    print("Run End:", end_run)
    print("Query Begin:", query_begin)
    print("Query End:", query_end)

    trip_history = TripHistory(query_begin, query_end, runnumber)

    # ---------------------------
    # storage
    # ---------------------------
    bco_table = defaultdict(list)
    bco_to_time = {}
    
    # ---------------------------
    # Prometheus query
    # ---------------------------
    for variable_name in variable_names:

        params = {
            'query': variable_name,
            'start': query_begin,
            'end': query_end,
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

            #key = (side, sector, R_Module, HV_Layer)
            key = side + "_" + sector + "_" + R_Module + "_" + HV_Layer + "_IMon"

            values = []

            for v in metric['values']:
                timestamp = float(v[0])
                bco = to_bco(timestamp)
                bco_to_time[bco] = timestamp
                values.append((bco, float(v[1])))
                
            values.sort(key=lambda x: x[0])

            bco_table[key].extend(values)

    # ----------------------------------------------------------
    # Fill missing current measurements before constructing the
    # CDB history.
    #
    # A monitoring gap means "no measurement", not zero current.
    # Carry the previous measured value forward so that downstream
    # space-charge corrections do not interpret missing telemetry
    # as a physical disappearance of the GEM current.
    # ----------------------------------------------------------
    fill_current_gaps(
        bco_table,
        bco_to_time
    )
    
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

            # lead with the environment and trip status...
            timestamp = bco_to_time[bco]
            temperature, pressure = environment_history.get(timestamp)
            f.write(f"gas_temperature {temperature}\n")
            f.write(f"gas_pressure {pressure}\n")
            FieldOK, GainOK = trip_history.get(timestamp)
            f.write(f"FieldOK {FieldOK}\n")
            f.write(f"GainOK {GainOK}\n")
            
            # Add in the individual GEM currents.
            for channel in sorted_channels:
                current = channel_values[channel].get(bco)
                if current == "" or current is None:
                    continue
                f.write(f"{channel} {current}\n")
                
    print("Wrote:", out_file)


if __name__ == "__main__":

    if len(sys.argv) != 2:
        print("Usage: python3 script.py <runnumber>")
        sys.exit(1)
        
    print("Loading Temperature and Pressure History.  Please exercise patience...")
    environment_history = EnvironmentHistory("/sphenix/user/hemmick/TemperatureAndPressure/tpc_gas_history.tsv")
    print("Temperature and Pressure Loaded")
    
    process_run_data(int(sys.argv[1]), environment_history)
