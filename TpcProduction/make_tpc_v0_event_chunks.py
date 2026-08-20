#!/usr/bin/env python3
"""Build non-overlapping DST event chunks for TPC V0 Condor jobs."""

from __future__ import annotations

import argparse
import math
from pathlib import Path
import sys

import uproot


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("input_list", type=Path)
    parser.add_argument("output_manifest", type=Path)
    parser.add_argument("--events-per-job", type=int, default=7)
    parser.add_argument("--max-jobs", type=int, default=10000)
    return parser.parse_args()


def main() -> int:
    args = parse_args()
    if args.events_per_job <= 0:
        raise ValueError("events-per-job must be positive")
    if args.max_jobs <= 0:
        raise ValueError("max-jobs must be positive")

    input_list = args.input_list.resolve()
    base = Path.cwd()
    paths: list[Path] = []
    for raw in input_list.read_text().splitlines():
        raw = raw.strip()
        if not raw or raw.startswith("#"):
            continue
        path = Path(raw).expanduser()
        paths.append(path if path.is_absolute() else (base / path).resolve())

    rows: list[tuple[int, Path, int, int]] = []
    skipped: list[tuple[Path, str]] = []
    total_events = 0
    for file_index, path in enumerate(paths):
        try:
            with uproot.open(path) as root_file:
                entries = int(root_file["T"].num_entries)
        except Exception as error:
            skipped.append((path, str(error).splitlines()[0]))
            continue

        total_events += entries
        for event_skip in range(0, entries, args.events_per_job):
            nevents = min(args.events_per_job, entries - event_skip)
            rows.append((file_index, path, event_skip, nevents))

    if len(rows) > args.max_jobs:
        minimum = math.ceil(total_events / args.max_jobs)
        print(
            f"Error: {len(rows)} jobs exceeds max-jobs={args.max_jobs}. "
            f"Try --events-per-job {minimum} or larger.",
            file=sys.stderr,
        )
        return 2

    args.output_manifest.parent.mkdir(parents=True, exist_ok=True)
    with args.output_manifest.open("w") as output:
        for file_index, path, event_skip, nevents in rows:
            output.write(f"{file_index}\t{path}\t{event_skip}\t{nevents}\n")

    print(f"[event-chunks] input files: {len(paths)}")
    print(f"[event-chunks] usable files: {len(paths) - len(skipped)}")
    print(f"[event-chunks] skipped files: {len(skipped)}")
    print(f"[event-chunks] total events: {total_events}")
    print(f"[event-chunks] events/job: {args.events_per_job}")
    print(f"[event-chunks] jobs: {len(rows)}")
    print(f"[event-chunks] manifest: {args.output_manifest.resolve()}")
    for path, reason in skipped:
        print(f"[event-chunks] skipped {path}: {reason}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
