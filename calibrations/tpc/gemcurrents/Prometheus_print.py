#!/usr/bin/env python3

import sys
import requests
import pyodbc
from collections import defaultdict

PROMETHEUS = "http://130.199.6.28:9090/api/v1"
PADDING = 120
STEP = 10


def get_run_times(runnumber):
    conn = pyodbc.connect("DSN=daq", readonly=True)
    cursor = conn.cursor()

    #cursor.execute(
    #    """
    #    SELECT
    #        EXTRACT(EPOCH FROM brtimestamp),
    #        EXTRACT(EPOCH FROM ertimestamp)
    #    FROM run
    #    WHERE runnumber = ?
    #    """,
    #    runnumber,
    #)
    cursor.execute(
        """
        SELECT
            EXTRACT(EPOCH FROM brtimestamp AT TIME ZONE 'America/New_York'),
            EXTRACT(EPOCH FROM ertimestamp AT TIME ZONE 'America/New_York')
        FROM run
        WHERE runnumber = ?
        """,
        runnumber,
    )
    row = cursor.fetchone()

    cursor.close()
    conn.close()

    if row is None:
        raise RuntimeError(f"Run {runnumber} not found")

    return int(row[0]), int(row[1])


def prometheus_query(metric, begin, end):
    response = requests.get(
        f"{PROMETHEUS}/query_range",
        params={
            "query": metric,
            "start": begin,
            "end": end,
            "step": STEP,
        },
        timeout=60,
    )

    response.raise_for_status()
    payload = response.json()

    if payload["status"] != "success":
        raise RuntimeError(payload)

    return payload["data"]["result"]


def organize(series):
    """
    Return:
        values[timestamp][channel] = value
    """

    values = defaultdict(dict)

    for item in series:
        labels = item["metric"]

        channel = labels.get("ch_name", "UNKNOWN")

        # Ignore the 12 non-TPC/reference extras here if desired later.
        for timestamp, value in item["values"]:
            values[int(float(timestamp))][channel] = float(value)

    return values


def print_points(name, values):
    print()
    print("=" * 80)
    print(name)
    print("=" * 80)

    for timestamp in sorted(values):
        channels = values[timestamp]

        print()
        print(f"TIME {timestamp}   CHANNELS {len(channels)}")

        for channel in sorted(channels):
            print(f"{channel:20s} {channels[channel]:10.3f}")


if __name__ == "__main__":

    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} runnumber")
        sys.exit(1)

    runnumber = int(sys.argv[1])

    begin, end = get_run_times(runnumber)

    query_begin = begin - PADDING
    query_end = end + PADDING

    print(f"Run: {runnumber}")
    print(f"Run Begin: {begin}")
    print(f"Run End: {end}")
    print(f"Query Begin: {query_begin}")
    print(f"Query End: {query_end}")

    v0_series = prometheus_query(
        "sphenix_tpc_hv_caen_V0Set",
        query_begin,
        query_end,
    )

    vmon_series = prometheus_query(
        "sphenix_tpc_hv_caen_VMon",
        query_begin,
        query_end,
    )

    print(f"V0Set series: {len(v0_series)}")
    print(f"VMon series: {len(vmon_series)}")

    v0 = organize(v0_series)
    vmon = organize(vmon_series)

    print_points("V0Set", v0)
    print_points("VMon", vmon)
