#!/usr/bin/env python3
"""Discover production QA segments, then run the ROOT-only calibration macro.

No PyROOT or sPHENIX build is needed by this runner: only Python 3 and root.
"""

import argparse
import csv
import glob
import json
import math
import os
from pathlib import Path
import re
import shutil
import subprocess
import sys


def parse_runs(text):
    values = []
    for item in re.split(r"[,\s]+", text.strip()):
        if not item:
            continue
        if not item.isdecimal():
            raise ValueError("Runs must be integers separated by commas or whitespace")
        run = int(item)
        if run in values:
            raise ValueError(f"Repeated run: {run}")
        values.append(run)
    return values


def windows(text):
    result = []
    for item in text.split(","):
        lo, hi = map(float, item.split(":"))
        if not (.2 <= lo < hi <= 20):
            raise ValueError("Each pT window must satisfy 0.2 <= low < high <= 20")
        tag = (f"pt_{lo:g}_{hi:g}").replace(".", "p")
        if any(tag == r[0] for r in result):
            raise ValueError("Duplicate pT window")
        result.append((tag, lo, hi))
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--runs", help="Comma-separated production runs")
    parser.add_argument("--run-list", type=Path, help="Run numbers, whitespace-separated; # comments allowed")
    parser.add_argument("--production", default="/sphenix/data/data02/sphnxpro/production/run3pp/physics/ana581_2026p005_v001")
    parser.add_argument("--input-template", help="Glob with {run:08d}, {lo:08d}, {hi:08d}; overrides production layout")
    parser.add_argument("--dataset", action="append", default=[], metavar="LABEL=GLOB", help="Explicit dataset glob; repeat for alternate configurations of the same run")
    parser.add_argument("--file-list", action="append", default=[], metavar="LABEL=LIST", help="Explicit newline-delimited list of ROOT paths; paths relative to LIST")
    parser.add_argument("--drift-dataset", action="append", default=[], metavar="LABEL=GLOB", help="Optional separate drift-QA files for an existing sample label")
    parser.add_argument("--drift-file-list", action="append", default=[], metavar="LABEL=LIST", help="Optional separate drift-QA file list for an existing label")
    parser.add_argument("--reference", required=True, help="Run number or explicit dataset label in this invocation")
    parser.add_argument("--out", type=Path, required=True, help="New output directory (must not exist)")
    parser.add_argument("--pt-windows", default="0.5:1", help="Comma-separated low:high windows at existing bin edges")
    parser.add_argument("--min-entries", type=int, default=100)
    parser.add_argument("--phi-group", type=int, default=3, help="Merge this many adjacent phi bins before fitting/projecting")
    parser.add_argument("--eta-group", type=int, default=4)
    parser.add_argument("--prefix", default="h_TpcSeedsQA_")
    parser.add_argument("--conditions-csv", type=Path, help="Optional table with a label column and known run/keff/geometry settings; embedded as JSON metadata")
    parser.add_argument("--fit-model", choices=["gaussian", "gaussian-background"], default="gaussian", help="Gaussian-background targets the modal core with a nonnegative constant local background")
    parser.add_argument("--no-plots", action="store_true", help="Write numerical ROOT/CSV outputs without PDFs")
    parser.add_argument("--phg-reference-csv", type=Path, help="Optional label,side,velocity_cm_us,t0_bins,source table; side 0 South, 1 North; t0 is configured, not measured")
    parser.add_argument("--save-slices", action="store_true", help="Also store detailed DCA slices/input maps; larger ROOT file")
    parser.add_argument("--phi-qpt", action="store_true", help="Also fit signed q/pT profiles in each phi group using new QA")
    parser.add_argument("--ratio-floor", type=float, default=.02, help="Minimum absolute reference DCA mean [cm] for a signed ratio; also require >3 sigma")
    parser.add_argument("--raw-time-shift", type=float, default=40., help="Raw TpcRawHitQA -> unpacked time: subtract this many bins (presample shift + unpacker t0); never applied to already unpacked drift QA")
    parser.add_argument("--root", default="root.exe", help="ROOT executable, e.g. root or root.exe")
    parser.add_argument("--max-files", type=int, help="Explicit quick-test limit per dataset, recorded in provenance")
    parser.add_argument("--dry-run", action="store_true", help="Validate/discover inputs and print counts; write nothing")
    args = parser.parse_args()
    if args.min_entries < 20 or args.phi_group < 1 or args.eta_group < 1 or args.ratio_floor < 0 or not math.isfinite(args.ratio_floor) or not math.isfinite(args.raw_time_shift):
        parser.error("min-entries>=20 and grouping>=1 required")
    if args.max_files is not None and args.max_files < 1:
        parser.error("max-files must be positive")
    try:
        pt_windows = windows(args.pt_windows)
        run_text = args.runs or ""
        if args.run_list:
            run_text += " " + " ".join(line.split("#", 1)[0] for line in args.run_list.read_text().splitlines())
        runs = parse_runs(run_text)
        specs = []
        for run in runs:
            lo = (run // 100) * 100
            template = args.input_template or str(Path(args.production) / "DST_TRKR_SEED" / "run_{lo:08d}_{hi:08d}" / "hist" / "HIST_DST_TRKR_SEED_*-{run:08d}-*.root")
            specs.append((str(run), sorted(glob.glob(template.format(run=run, lo=lo, hi=lo + 100)))))
        for spec in args.dataset:
            label, pattern = spec.split("=", 1)
            specs.append((label, sorted(glob.glob(os.path.expanduser(pattern)))))
        for spec in args.file_list:
            label, list_path = spec.split("=", 1)
            source = Path(list_path).expanduser().resolve()
            paths = []
            for line in source.read_text().splitlines():
                if not line.strip() or line.lstrip().startswith("#"):
                    continue
                p = Path(line.strip()).expanduser()
                paths.append(str(p if p.is_absolute() else source.parent / p))
            specs.append((label, paths))
        if not specs:
            raise ValueError("Provide --runs, --run-list, --dataset, or --file-list")
        datasets, all_seen = {}, {}
        for label, paths in specs:
            if not re.fullmatch(r"[A-Za-z0-9_.-]+", label) or label in (".", "..", "configuration", "run_conditions_json", "drift_run_trends") or label in datasets:
                raise ValueError(f"Invalid/duplicate label: {label}")
            if not paths:
                raise ValueError(f"No input files for {label}")
            canonical = [str(Path(p).resolve(strict=True)) for p in paths]
            if len(set(canonical)) != len(canonical):
                raise ValueError(f"Duplicate file in {label}")
            if args.max_files:
                canonical = canonical[:args.max_files]
            for p in canonical:
                if "\n" in p or "\t" in p or not Path(p).is_file():
                    raise ValueError(f"Unsupported input path: {p}")
                if p in all_seen:
                    raise ValueError(f"Input shared by {all_seen[p]} and {label}; independent-error comparisons require disjoint files")
                all_seen[p] = label
            datasets[label] = canonical
        if args.reference not in datasets:
            raise ValueError("Reference must be a label/run included in this invocation")
        conditions = []
        raw_shifts = {}
        if args.conditions_csv:
            with args.conditions_csv.expanduser().open(newline="") as stream:
                reader = csv.DictReader(stream)
                if not reader.fieldnames or "label" not in reader.fieldnames:
                    raise ValueError("Conditions CSV requires a label column")
                conditions = list(reader)
            labels = [row["label"] for row in conditions]
            if set(labels) != set(datasets):
                raise ValueError("Conditions CSV must cover each dataset label")
            condition_keys = set()
            for row in conditions:
                side = row.get("side", "all").strip().lower() or "all"
                side = {"0": "south", "1": "north", "s": "south", "n": "north"}.get(side, side)
                if side not in ("south", "north", "all") or (row["label"], side) in condition_keys:
                    raise ValueError("Invalid/duplicate conditions label and side")
                condition_keys.add((row["label"], side))
                if row.get("raw_time_shift", "").strip():
                    shift = float(row["raw_time_shift"])
                    if not math.isfinite(shift):
                        raise ValueError("raw_time_shift must be finite")
                    if row["label"] in raw_shifts and raw_shifts[row["label"]] != shift:
                        raise ValueError("Side rows disagree on raw_time_shift")
                    raw_shifts[row["label"]] = shift
        phg_rows = []
        if args.phg_reference_csv:
            with args.phg_reference_csv.open(newline="") as stream:
                reader = csv.DictReader(stream)
                if not {"label", "side", "velocity_cm_us", "t0_bins", "source"}.issubset(reader.fieldnames or []):
                    raise ValueError("PHGarfield CSV needs label,side,velocity_cm_us,t0_bins,source")
                phg_rows = list(reader)
            seen_phg = set()
            for row in phg_rows:
                pair = row["label"], row["side"]
                if row["label"] not in datasets or row["side"] not in ("0", "1") or pair in seen_phg:
                    raise ValueError("Invalid/duplicate PHGarfield label/side")
                seen_phg.add(pair)
                for key in ("velocity_cm_us", "t0_bins"):
                    value = row[key].strip()
                    row[key] = value
                    if value and (not math.isfinite(float(value)) or (key == "velocity_cm_us" and float(value) <= 0)):
                        raise ValueError("Invalid PHGarfield reference value")
                if not row["source"].strip() or any("\t" in row[k] or "\n" in row[k] for k in ("source",)):
                    raise ValueError("PHGarfield source must be nonempty, single-line text")
        drift_datasets = {}
        drift_specs = []
        for spec in args.drift_dataset:
            label, pattern = spec.split("=", 1)
            drift_specs.append((label, sorted(glob.glob(os.path.expanduser(pattern)))))
        for spec in args.drift_file_list:
            label, path = spec.split("=", 1)
            listing = Path(path).expanduser().resolve()
            paths = []
            for line in listing.read_text().splitlines():
                if not line.strip() or line.lstrip().startswith("#"):
                    continue
                p = Path(line.strip()).expanduser()
                paths.append(str(p if p.is_absolute() else listing.parent / p))
            drift_specs.append((label, paths))
        for label, paths in drift_specs:
            if label not in datasets or label in drift_datasets:
                raise ValueError(f"Drift label must identify one existing dataset exactly once: {label}")
            paths = [str(Path(p).resolve(strict=True)) for p in paths]
            if not paths or len(paths) != len(set(paths)):
                raise ValueError(f"Empty/duplicate drift inputs: {label}")
            if args.max_files:
                paths = paths[:args.max_files]
            if any(not Path(p).is_file() or "\t" in p or "\n" in p for p in paths):
                raise ValueError("Invalid drift input path")
            drift_datasets[label] = paths
        for label, files in datasets.items():
            print(f"{label}: {len(files)} files" + (" [REFERENCE]" if label == args.reference else ""), flush=True)
        if args.dry_run:
            return 0
        executable = shutil.which(args.root)
        if not executable:
            raise ValueError(f"ROOT executable not found: {args.root}; source your sPHENIX environment or use --root root")
        macro = Path(__file__).resolve().with_name("TPCDistortionQA.C")
        if not macro.is_file():
            raise ValueError(f"Missing macro: {macro}")
        out = args.out.expanduser().resolve()
        out.mkdir(parents=True, exist_ok=False)
        manifest = out / "samples.tsv"
        with manifest.open("w") as stream:
            for label, files in datasets.items():
                listing = out / f"{label}.inputs.txt"
                listing.write_text("\n".join(files) + "\n")
                separate = ""
                if label in drift_datasets:
                    drift_list = out / f"{label}.drift_inputs.txt"
                    drift_list.write_text("\n".join(drift_datasets[label]) + "\n")
                    separate = f"\t{drift_list}"
                if label in raw_shifts:
                    if not separate:
                        separate = "\t"
                    separate += f"\t{raw_shifts[label]}"
                stream.write(f"{label}\t{listing}{separate}\n")
        provenance = {"arguments": {k: str(v) if isinstance(v, Path) else v for k, v in vars(args).items()}, "datasets": {k: [{"path": p, "size": Path(p).stat().st_size, "mtime_ns": Path(p).stat().st_mtime_ns} for p in v] for k, v in datasets.items()}}
        provenance["drift_datasets"] = {k: [{"path": p, "size": Path(p).stat().st_size, "mtime_ns": Path(p).stat().st_mtime_ns} for p in v] for k, v in drift_datasets.items()}
        phg_table = out / "phgarfield_references.tsv"
        phg_table.write_text("".join("\t".join([row["label"], row["side"], row["velocity_cm_us"], row["t0_bins"], json.dumps(row, sort_keys=True), row.get("time_bin_ns", ""), row.get("z_pad_cm", ""), row.get("cm_halfwidth_cm", "")]) + "\n" for row in phg_rows))
        provenance["phgarfield_references"] = phg_rows
        provenance["conditions"] = conditions
        (out / "provenance.json").write_text(json.dumps(provenance, indent=2) + "\n")
        # An interpreted driver gives ROOT an explicit process exit status on errors.
        # C++ string literals are JSON-escaped; subprocess never invokes a shell.
        for tag, low, high in pt_windows:
            destination = out / tag
            driver = out / f"run_{tag}.C"
            args_cpp = ",".join([json.dumps(str(manifest)), json.dumps(str(destination)), json.dumps(args.reference), repr(low), repr(high), str(args.min_entries), str(args.phi_group), str(args.eta_group), json.dumps(args.prefix), "true" if args.save_slices else "false", repr(args.ratio_floor), "true" if args.phi_qpt else "false", json.dumps(json.dumps(conditions)), repr(args.raw_time_shift), "false" if args.no_plots else "true", json.dumps(str(phg_table)), "true" if args.fit_model == "gaussian-background" else "false"])
            driver.write_text(f'#include {json.dumps(str(macro))}\nvoid run_{tag}() {{\n  try {{ gSystem->Exit(TPCDistortionQA({args_cpp})); }}\n  catch (const std::exception &e) {{ std::cerr << "TPCDistortionQA ERROR: " << e.what() << std::endl; gSystem->Exit(1); }}\n}}\n')
            print(f"Analyzing {tag}; log: {out / (tag + '.log')}", flush=True)
            with (out / f"{tag}.log").open("w") as log:
                process = subprocess.run([executable, "-l", "-b", "-q", str(driver)], stdout=log, stderr=subprocess.STDOUT, check=False)
            if process.returncode or not (destination / "SUCCESS").is_file():
                raise RuntimeError(f"Analysis failed for {tag}; inspect {out / (tag + '.log')}")
            status_file=destination/"analysis_summary.tsv"
            if status_file.is_file():
                for row in status_file.read_text().splitlines()[1:]:
                    print(row.replace("\t",": "),flush=True)
        (out / "SUCCESS").write_text("All pT windows completed\n")
        print(f"Completed: {out}")
        return 0
    except (ValueError, OSError, RuntimeError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    sys.exit(main())
