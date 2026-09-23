#!/usr/bin/env python3
"""Build non-overlapping DST event chunks for TPC V0 Condor jobs."""

from __future__ import annotations

import argparse
from pathlib import Path
import sys

import ROOT


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser()
    parser.add_argument("input_list", type=Path)
    parser.add_argument("output_manifest", type=Path)
    parser.add_argument("--events-per-job", type=int, default=7)
    parser.add_argument("--max-jobs", type=int, default=10000)
    return parser.parse_args()


def initialize_file_catalog() -> None:
    """Expose the Fun4All file-catalog resolver to PyROOT."""
    if ROOT.gSystem.Load("libfun4all.so") < 0:
        raise RuntimeError("failed to load libfun4all.so")

    declaration_ok = ROOT.gInterpreter.Declare(
        r"""
#include <fun4all/DBInterface.h>

#include <string>

namespace tpc_v0_event_chunks
{
  std::string resolve_input(const std::string& logical_name)
  {
    return DBInterface::instance()->location(logical_name);
  }
}
"""
    )
    if not declaration_ok:
        raise RuntimeError("failed to initialize the Fun4All file-catalog resolver")


def read_input_names(input_list: Path) -> list[str]:
    """Read catalog logical names or explicit locations without rewriting them."""
    if not input_list.is_file():
        raise FileNotFoundError(f"input list does not exist: {input_list}")

    input_names: list[str] = []
    for line_number, raw in enumerate(input_list.read_text().splitlines(), start=1):
        input_name = raw.strip()
        if not input_name or input_name.startswith("#"):
            continue
        if "\t" in input_name:
            raise ValueError(
                f"input list line {line_number} contains a tab, which is reserved "
                "for the manifest schema"
            )
        if "/" in input_name and not input_name.startswith("/") and "://" not in input_name:
            raise ValueError(
                f"input list line {line_number} is an ambiguous relative path: "
                f"{input_name}. Use a catalog logical name or an explicit absolute path/URI."
            )
        input_names.append(input_name)

    if not input_names:
        raise ValueError(f"input list contains no usable entries: {input_list}")
    return input_names


def count_events(input_name: str) -> int:
    """Resolve one input through FROG and return the number of DST events."""
    resolved_name = str(ROOT.tpc_v0_event_chunks.resolve_input(input_name))
    root_file = ROOT.TFile.Open(resolved_name, "READ")
    if not root_file or root_file.IsZombie():
        raise OSError(f"cannot open ROOT file: {resolved_name}")
    try:
        tree = root_file.Get("T")
        if not tree or not tree.InheritsFrom("TTree"):
            raise KeyError(f"TTree 'T' is missing from: {resolved_name}")
        return int(tree.GetEntries())
    finally:
        root_file.Close()


def job_count(entries_by_file: list[int], events_per_job: int) -> int:
    return sum(
        (entries + events_per_job - 1) // events_per_job
        for entries in entries_by_file
        if entries > 0
    )


def minimum_events_per_job(entries_by_file: list[int], max_jobs: int) -> int | None:
    nonempty_files = sum(entries > 0 for entries in entries_by_file)
    if nonempty_files > max_jobs:
        return None

    low = 1
    high = max(entries_by_file, default=1)
    while low < high:
        candidate = (low + high) // 2
        if job_count(entries_by_file, candidate) <= max_jobs:
            high = candidate
        else:
            low = candidate + 1
    return low


def main() -> int:
    args = parse_args()
    if args.events_per_job <= 0:
        raise ValueError("events-per-job must be positive")
    if args.max_jobs <= 0:
        raise ValueError("max-jobs must be positive")

    input_list = args.input_list.resolve()
    try:
        input_names = read_input_names(input_list)
        initialize_file_catalog()
    except (FileNotFoundError, RuntimeError, ValueError) as error:
        print(f"Error: {error}", file=sys.stderr)
        return 2

    failures: list[tuple[str, str]] = []
    entries_by_file: list[int] = []
    for input_name in input_names:
        try:
            entries = count_events(input_name)
        except Exception as error:
            failures.append((input_name, str(error).splitlines()[0]))
            entries_by_file.append(0)
            continue

        entries_by_file.append(entries)

    if failures:
        print(
            f"Error: failed to inspect {len(failures)} of {len(input_names)} input files; "
            "no manifest was written.",
            file=sys.stderr,
        )
        for input_name, reason in failures:
            print(f"  {input_name}: {reason}", file=sys.stderr)
        return 2

    total_events = sum(entries_by_file)
    n_jobs = job_count(entries_by_file, args.events_per_job)
    if n_jobs == 0:
        print("Error: the input files contain no events; no manifest was written.", file=sys.stderr)
        return 2

    if n_jobs > args.max_jobs:
        minimum = minimum_events_per_job(entries_by_file, args.max_jobs)
        if minimum is None:
            nonempty_files = sum(entries > 0 for entries in entries_by_file)
            print(
                f"Error: {n_jobs} jobs exceeds max-jobs={args.max_jobs}, and the "
                f"sample has {nonempty_files} nonempty files. At least one job per file "
                "is required, so increasing --events-per-job cannot satisfy this limit.",
                file=sys.stderr,
            )
        else:
            suggested_jobs = job_count(entries_by_file, minimum)
            print(
                f"Error: {n_jobs} jobs exceeds max-jobs={args.max_jobs}. "
                f"Use --events-per-job {minimum} or larger "
                f"({suggested_jobs} jobs at {minimum}).",
                file=sys.stderr,
            )
        return 2

    args.output_manifest.parent.mkdir(parents=True, exist_ok=True)
    with args.output_manifest.open("w") as output:
        for file_index, (input_name, entries) in enumerate(zip(input_names, entries_by_file)):
            for event_skip in range(0, entries, args.events_per_job):
                nevents = min(args.events_per_job, entries - event_skip)
                output.write(f"{file_index}\t{input_name}\t{event_skip}\t{nevents}\n")

    print(f"[event-chunks] input files: {len(input_names)}")
    print(f"[event-chunks] usable files: {len(input_names)}")
    print(f"[event-chunks] total events: {total_events}")
    print(f"[event-chunks] events/job: {args.events_per_job}")
    print(f"[event-chunks] jobs: {n_jobs}")
    print(f"[event-chunks] manifest: {args.output_manifest.resolve()}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
