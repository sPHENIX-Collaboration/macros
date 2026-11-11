#!/usr/bin/env python3
"""Run compiled TPC calibration QA on existing production histograms.

The run command exports conditions and PHGarfield references, processes the
selected segments, then writes the portable summary. Requires an installed
libtpccalibrationqa and ROOT; Python uses only the standard library.
"""

import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import csv
import glob
import hashlib
import json
import math
import os
from pathlib import Path
import re
import shutil
import stat as stat_module
import subprocess
import sys
import time

SOURCE_COMMIT = "b241b6413a2de90cba3b6d415579db62bc81a153"


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
        if not (0.2 <= lo < hi <= 20):
            raise ValueError("Each pT window must satisfy 0.2 <= low < high <= 20")
        tag = (f"pt_{lo:g}_{hi:g}").replace(".", "p")
        if any(tag == r[0] for r in result):
            raise ValueError("Duplicate pT window")
        result.append((tag, lo, hi))
    return result


def analyze_main(argv):
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog="Direct file lists: analyze --help. Export only: conditions --help or phgarfield --help.",
    )
    parser.add_argument("--runs", help="Comma-separated production runs")
    parser.add_argument(
        "--run-list", type=Path, help="Run numbers, whitespace-separated; # comments allowed"
    )
    parser.add_argument(
        "--production",
        default="/sphenix/data/data02/sphnxpro/production/run3pp/physics/ana581_2026p005_v001",
    )
    parser.add_argument(
        "--input-template",
        help="Glob with {run:08d}, {lo:08d}, {hi:08d}; overrides production layout",
    )
    parser.add_argument(
        "--dataset",
        action="append",
        default=[],
        metavar="LABEL=GLOB",
        help="Explicit dataset glob; repeat for alternate configurations of the same run",
    )
    parser.add_argument(
        "--file-list",
        action="append",
        default=[],
        metavar="LABEL=LIST",
        help="Explicit newline-delimited list of ROOT paths; paths relative to LIST",
    )
    parser.add_argument(
        "--drift-dataset",
        action="append",
        default=[],
        metavar="LABEL=GLOB",
        help="Optional separate drift-QA files for an existing sample label",
    )
    parser.add_argument(
        "--drift-file-list",
        action="append",
        default=[],
        metavar="LABEL=LIST",
        help="Optional separate drift-QA file list for an existing label",
    )
    parser.add_argument(
        "--reference", required=True, help="Run number or explicit dataset label in this invocation"
    )
    parser.add_argument(
        "--out", type=Path, required=True, help="New output directory (must not exist)"
    )
    parser.add_argument(
        "--pt-windows",
        default="0.5:1",
        help="Comma-separated low:high windows at existing bin edges",
    )
    parser.add_argument("--min-entries", type=int, default=100)
    parser.add_argument(
        "--phi-group",
        type=int,
        default=3,
        help="Merge this many adjacent phi bins before fitting/projecting",
    )
    parser.add_argument("--eta-group", type=int, default=4)
    parser.add_argument("--prefix", default="h_TpcSeedsQA_")
    parser.add_argument(
        "--conditions-csv",
        type=Path,
        help="Optional table with a label column and known run/keff/geometry settings; embedded as JSON metadata",
    )
    parser.add_argument(
        "--fit-model",
        choices=["gaussian", "gaussian-background"],
        default="gaussian",
        help="Gaussian-background targets the modal core with a nonnegative constant local background",
    )
    parser.add_argument(
        "--no-plots", action="store_true", help="Write numerical ROOT/CSV outputs without PDFs"
    )
    parser.add_argument(
        "--phg-reference-csv",
        type=Path,
        help="Optional label,side,velocity_cm_us,t0_bins,source table; side 0 South, 1 North; t0 is configured, not measured",
    )
    parser.add_argument(
        "--save-slices",
        action="store_true",
        help="Also store detailed DCA slices/input maps; larger ROOT file",
    )
    parser.add_argument(
        "--phi-qpt",
        action="store_true",
        help="Also fit signed q/pT profiles in each phi group using new QA",
    )
    parser.add_argument(
        "--ratio-floor",
        type=float,
        default=0.02,
        help="Minimum absolute reference DCA mean [cm] for a signed ratio; also require >3 sigma",
    )
    parser.add_argument(
        "--raw-time-shift",
        type=float,
        default=40.0,
        help="Raw TpcRawHitQA -> unpacked time: subtract this many bins (presample shift + unpacker t0); never applied to already unpacked drift QA",
    )
    parser.add_argument("--root", default="root.exe", help="ROOT executable, e.g. root or root.exe")
    parser.add_argument(
        "--max-files",
        type=int,
        help="Explicit quick-test limit per dataset, recorded in provenance",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Validate/discover inputs and print counts; write nothing",
    )
    args = parser.parse_args(argv)
    if (
        args.min_entries < 20
        or args.phi_group < 1
        or args.eta_group < 1
        or args.ratio_floor < 0
        or not math.isfinite(args.ratio_floor)
        or not math.isfinite(args.raw_time_shift)
    ):
        parser.error("min-entries>=20 and grouping>=1 required")
    if args.max_files is not None and args.max_files < 1:
        parser.error("max-files must be positive")
    try:
        pt_windows = windows(args.pt_windows)
        run_text = args.runs or ""
        if args.run_list:
            run_text += " " + " ".join(
                line.split("#", 1)[0] for line in args.run_list.read_text().splitlines()
            )
        runs = parse_runs(run_text)
        specs = []
        for run in runs:
            lo = (run // 100) * 100
            template = args.input_template or str(
                Path(args.production)
                / "DST_TRKR_SEED"
                / "run_{lo:08d}_{hi:08d}"
                / "hist"
                / "HIST_DST_TRKR_SEED_*-{run:08d}-*.root"
            )
            specs.append(
                (str(run), sorted(glob.glob(template.format(run=run, lo=lo, hi=lo + 100))))
            )
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
            if (
                not re.fullmatch(r"[A-Za-z0-9_.-]+", label)
                or label in (".", "..", "configuration", "run_conditions_json", "drift_run_trends")
                or label in datasets
            ):
                raise ValueError(f"Invalid/duplicate label: {label}")
            if not paths:
                raise ValueError(f"No input files for {label}")
            canonical = [str(Path(p).resolve(strict=True)) for p in paths]
            if len(set(canonical)) != len(canonical):
                raise ValueError(f"Duplicate file in {label}")
            if args.max_files:
                canonical = canonical[: args.max_files]
            for p in canonical:
                if "\n" in p or "\t" in p or not Path(p).is_file():
                    raise ValueError(f"Unsupported input path: {p}")
                if p in all_seen:
                    raise ValueError(
                        f"Input shared by {all_seen[p]} and {label}; independent-error comparisons require disjoint files"
                    )
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
                if not {"label", "side", "velocity_cm_us", "t0_bins", "source"}.issubset(
                    reader.fieldnames or []
                ):
                    raise ValueError(
                        "PHGarfield CSV needs label,side,velocity_cm_us,t0_bins,source"
                    )
                phg_rows = list(reader)
            seen_phg = set()
            for row in phg_rows:
                pair = row["label"], row["side"]
                if (
                    row["label"] not in datasets
                    or row["side"] not in ("0", "1")
                    or pair in seen_phg
                ):
                    raise ValueError("Invalid/duplicate PHGarfield label/side")
                seen_phg.add(pair)
                for key in ("velocity_cm_us", "t0_bins"):
                    value = row[key].strip()
                    row[key] = value
                    if value and (
                        not math.isfinite(float(value))
                        or (key == "velocity_cm_us" and float(value) <= 0)
                    ):
                        raise ValueError("Invalid PHGarfield reference value")
                if not row["source"].strip() or any(
                    "\t" in row[k] or "\n" in row[k] for k in ("source",)
                ):
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
                raise ValueError(
                    f"Drift label must identify one existing dataset exactly once: {label}"
                )
            paths = [str(Path(p).resolve(strict=True)) for p in paths]
            if not paths or len(paths) != len(set(paths)):
                raise ValueError(f"Empty/duplicate drift inputs: {label}")
            if args.max_files:
                paths = paths[: args.max_files]
            if any(not Path(p).is_file() or "\t" in p or "\n" in p for p in paths):
                raise ValueError("Invalid drift input path")
            drift_datasets[label] = paths
        for label, files in datasets.items():
            print(
                f"{label}: {len(files)} files"
                + (" [REFERENCE]" if label == args.reference else ""),
                flush=True,
            )
        if args.dry_run:
            return 0
        executable = shutil.which(args.root)
        if not executable:
            raise ValueError(
                f"ROOT executable not found: {args.root}; source your sPHENIX environment or use --root root"
            )
        macro = Path(__file__).resolve().with_name("Fun4All_TpcCalibrationQA.C")
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
        provenance = {
            "arguments": {k: str(v) if isinstance(v, Path) else v for k, v in vars(args).items()},
            "datasets": {
                k: [
                    {
                        "path": p,
                        "size": Path(p).stat().st_size,
                        "mtime_ns": Path(p).stat().st_mtime_ns,
                    }
                    for p in v
                ]
                for k, v in datasets.items()
            },
        }
        provenance["drift_datasets"] = {
            k: [
                {"path": p, "size": Path(p).stat().st_size, "mtime_ns": Path(p).stat().st_mtime_ns}
                for p in v
            ]
            for k, v in drift_datasets.items()
        }
        phg_table = out / "phgarfield_references.tsv"
        phg_table.write_text(
            "".join(
                "\t".join(
                    [
                        row["label"],
                        row["side"],
                        row["velocity_cm_us"],
                        row["t0_bins"],
                        json.dumps(row, sort_keys=True),
                        row.get("time_bin_ns", ""),
                        row.get("z_pad_cm", ""),
                        row.get("cm_halfwidth_cm", ""),
                    ]
                )
                + "\n"
                for row in phg_rows
            )
        )
        provenance["phgarfield_references"] = phg_rows
        provenance["conditions"] = conditions
        (out / "provenance.json").write_text(json.dumps(provenance, indent=2) + "\n")
        # An interpreted driver gives ROOT an explicit process exit status on errors.
        # C++ string literals are JSON-escaped; subprocess never invokes a shell.
        for tag, low, high in pt_windows:
            destination = out / tag
            driver = out / f"run_{tag}.C"
            args_cpp = ",".join(
                [
                    json.dumps(str(manifest)),
                    json.dumps(str(destination)),
                    json.dumps(args.reference),
                    repr(low),
                    repr(high),
                    str(args.min_entries),
                    str(args.phi_group),
                    str(args.eta_group),
                    json.dumps(args.prefix),
                    "true" if args.save_slices else "false",
                    repr(args.ratio_floor),
                    "true" if args.phi_qpt else "false",
                    json.dumps(json.dumps(conditions)),
                    repr(args.raw_time_shift),
                    "false" if args.no_plots else "true",
                    json.dumps(str(phg_table)),
                    "true" if args.fit_model == "gaussian-background" else "false",
                ]
            )
            driver.write_text(
                f'#include {json.dumps(str(macro))}\nvoid run_{tag}() {{\n  try {{ gSystem->Exit(Fun4All_TpcCalibrationQA({args_cpp})); }}\n  catch (const std::exception &e) {{ std::cerr << "TPCDistortionQA ERROR: " << e.what() << std::endl; gSystem->Exit(1); }}\n}}\n'
            )
            print(f"Analyzing {tag}; log: {out / (tag + '.log')}", flush=True)
            with (out / f"{tag}.log").open("w") as log:
                process = subprocess.run(
                    [executable, "-l", "-b", "-q", str(driver)],
                    stdout=log,
                    stderr=subprocess.STDOUT,
                    check=False,
                )
            if process.returncode or not (destination / "SUCCESS").is_file():
                raise RuntimeError(f"Analysis failed for {tag}; inspect {out / (tag + '.log')}")
            status_file = destination / "analysis_summary.tsv"
            if status_file.is_file():
                for row in status_file.read_text().splitlines()[1:]:
                    print(row.replace("\t", ": "), flush=True)
        (out / "SUCCESS").write_text("All pT windows completed\n")
        print(f"Completed: {out}")
        return 0
    except (ValueError, OSError, RuntimeError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1


parse_windows = windows


def compiled_preamble():
    return (
        "#include <tpccalibrationqa/TpcCalibrationQA.h>\n"
        "#include <tpccalibrationqa/TpcConditionsReference.h>\n"
        "#include <TFile.h>\n#include <TNamed.h>\n#include <TSystem.h>\n"
        "#include <iostream>\n#include <stdexcept>\n"
        "R__LOAD_LIBRARY(libtpccalibrationqa.so)\n"
    )


def library_signature(executable):
    # Fingerprint the actual resolved library, including local-install overrides.
    query = (
        'const char *p = gSystem->DynamicPathName("libtpccalibrationqa.so", true); '
        "if (!p) { gSystem->Exit(1); } "
        'std::cout << "TPCQA_LIBRARY=" << p << std::endl; gSystem->Exit(0);'
    )
    result = subprocess.run(
        [executable, "-l", "-b", "-q", "-e", query], capture_output=True, text=True
    )
    paths = [
        line.split("TPCQA_LIBRARY=", 1)[1].strip()
        for line in result.stdout.splitlines()
        if "TPCQA_LIBRARY=" in line
    ]
    if result.returncode or len(paths) != 1:
        raise ValueError(
            "Cannot resolve libtpccalibrationqa.so; build/install the package and source your local environment"
        )
    library = Path(paths[0]).resolve(strict=True)
    signature = {library.name: hashlib.sha256(library.read_bytes()).hexdigest()}
    for path in (Path(__file__).resolve(), CODE / "Fun4All_TpcCalibrationQA.C"):
        signature[path.name] = hashlib.sha256(path.read_bytes()).hexdigest()
    return signature


def reference_runs(groups, values):
    references = {}
    for value in values:
        group, run = value.split("=", 1)
        matches = [g for g in groups if g == group or g.split("/")[0] == group]
        if len(matches) != 1 or matches[0] in references:
            raise ValueError(f"Ambiguous/duplicate reference group {group}; specify system/tag")
        references[matches[0]] = int(run)
    if set(references) != groups:
        raise ValueError(f"Provide --reference GROUP=RUN for each group: {sorted(groups)}")
    return references


def run(args):
    """Export conditions and references before processing, then collect only completed runs."""
    if not args.cdb_tag or args.reference_field is None or not args.out or not args.reference:
        raise ValueError(
            "run requires --cdb-tag, --reference-field, --out and --reference SYSTEM[/TAG]=RUN"
        )
    if args.conditions_csv or args.phg_reference_csv:
        raise ValueError("run exports its own tables; use process for externally supplied CSVs")
    # Validate run/reference selection before any expensive CDB or PHGarfield work.
    data = inventory(args.inventory)
    choice = selected(args)
    rows = [row for row in data["runs"] if not choice or row["run"] in choice]
    if not rows or choice - {row["run"] for row in rows}:
        raise ValueError("Requested runs absent from inventory")
    refs = reference_runs({row["group"] for row in rows}, args.reference)
    for group, reference in refs.items():
        if reference not in {row["run"] for row in rows if row["group"] == group}:
            raise ValueError(f"Missing/absent reference run for {group}")
    if not math.isfinite(args.reference_field) or args.reference_field <= 0:
        raise ValueError("reference-field must be positive and finite")
    root = args.work.expanduser().resolve()
    root.mkdir(parents=True, exist_ok=True)
    common = ["--inventory", str(args.inventory), "--cdb-tag", args.cdb_tag, "--root", args.root]
    if args.runs:
        common += ["--runs", args.runs]
    if args.run_list:
        common += ["--run-list", str(args.run_list)]
    if conditions_main(
        common + ["--out", str(root / "conditions"), "--keff-fallback", args.keff_fallback]
    ):
        raise RuntimeError("Conditions export failed; processing was not started")
    if phgarfield_main(
        common + ["--out", str(root / "phgarfield"), "--reference-field", str(args.reference_field)]
    ):
        raise RuntimeError("PHGarfield export failed; processing was not started")
    args.conditions_csv = root / "conditions/conditions.csv"
    args.phg_reference_csv = root / "phgarfield/phgarfield_references.csv"
    args.require_conditions = True
    args.require_phg = True
    result = process(args)
    if result and not args.allow_partial:
        return result
    args.batch = root / "latest_batch.json"
    return collect(args)


def conditions_main(argv):
    p = argparse.ArgumentParser(description=__doc__)
    p.add_argument("--inventory", type=Path, required=True)
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--cdb-tag", required=True)
    p.add_argument("--root", default="root.exe")
    p.add_argument("--runs")
    p.add_argument("--run-list", type=Path)
    p.add_argument(
        "--keff-fallback",
        choices=["none", "phgarfield-defaults"],
        default="none",
        help="Only when the kEff payload URL is empty; always labeled derived, never verified applied",
    )
    p.add_argument(
        "--allow-missing",
        action="store_true",
        help="Return success with explicitly incomplete snapshots",
    )
    args = p.parse_args(argv)
    try:
        data = inventory(args.inventory)
        choice = selected(args)
        rows = [r for r in data["runs"] if not choice or r["run"] in choice]
        if not rows or choice - {r["run"] for r in rows}:
            raise ValueError("Requested runs absent from inventory")
        exe = shutil.which(args.root)
        if not exe:
            raise ValueError("ROOT executable not found")
        out = args.out.expanduser().resolve()
        out.mkdir(parents=True, exist_ok=True)
        signature = library_signature(exe)
        settings = {
            "cdb_tag": args.cdb_tag,
            "keff_fallback": args.keff_fallback,
            "compiled_code": signature,
            "definition_source_commit": SOURCE_COMMIT,
        }
        summaries = []
        series_rows = []
        failed = []
        for item in rows:
            group, run = item["group"], item["run"]
            work = out / group / str(run) / digest(settings)
            work.mkdir(parents=True, exist_ok=True)
            summary = work / "conditions.csv"
            series = work / "current_series.csv"
            if not summary.exists():
                temp = work / "conditions.partial.csv"
                series_temp = work / "current_series.partial.csv"
                driver = work / "export_conditions.C"
                driver.write_text(
                    compiled_preamble()
                    + "\nvoid export_conditions() { try { TpcConditionsReference qa; "
                    + "qa.set_cdb_tag("
                    + json.dumps(args.cdb_tag)
                    + "); qa.set_use_keff_fallback("
                    + ("true" if args.keff_fallback == "phgarfield-defaults" else "false")
                    + "); "
                    + f"qa.export_conditions({run},{json.dumps(group)},{json.dumps(str(temp))},{json.dumps(str(series_temp))}); "
                    + "gSystem->Exit(0); } catch(const std::exception &e) { std::cerr << e.what() << std::endl; gSystem->Exit(1); } }\n"
                )
                with (work / "export.log").open("w") as log:
                    result = subprocess.run(
                        [exe, "-l", "-b", "-q", str(driver)], stdout=log, stderr=subprocess.STDOUT
                    )
                if result.returncode or not temp.exists() or not series_temp.exists():
                    failed.append(
                        {
                            "group": group,
                            "run": run,
                            "error": "ROOT export failed",
                            "log": str(work / "export.log"),
                        }
                    )
                    continue
                with temp.open(newline="") as stream:
                    table = list(csv.DictReader(stream))
                if len(table) != 2 or {r["side"] for r in table} != {"0", "1"}:
                    raise ValueError(f"Invalid conditions output: {temp}")
                stamp = time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime())
                for record in table:
                    record.update(
                        snapshot_utc=stamp,
                        definition_source_commit=SOURCE_COMMIT,
                        export_offline_main=os.getenv("OFFLINE_MAIN", ""),
                        export_myinstall=os.getenv("MYINSTALL", ""),
                    )
                with temp.open("w", newline="") as stream:
                    writer = csv.DictWriter(stream, fieldnames=list(table[0]))
                    writer.writeheader()
                    writer.writerows(table)
                series_temp.replace(series)
                temp.replace(summary)
                atomic_json(work / "settings.json", settings)
            with summary.open(newline="") as stream:
                table = list(csv.DictReader(stream))
            for record in table:
                record["qa_inventory_segments"] = str(len(item["files"]))
                record["qa_first_available_segment"] = str(item["files"][0]["segment"])
            summaries.extend(table)
            with series.open(newline="") as stream:
                series_rows.extend(csv.DictReader(stream))
            incomplete = [
                r["conditions_status"]
                for r in table
                if r["conditions_status"] != "complete_snapshot"
            ]
            if incomplete:
                failed.append(
                    {
                        "group": group,
                        "run": run,
                        "error": ";".join(incomplete),
                        "log": str(work / "export.log"),
                    }
                )
            print(
                f"{group}/{run}: "
                + ", ".join(
                    f"side{r['side']}: Igem={r['gem_current_uA'] or 'missing'} uA; k_base={r['keff'] or 'missing'} ({r['keff_kind']})"
                    for r in table
                ),
                flush=True,
            )
        for name, table in [("conditions.csv", summaries), ("current_series.csv", series_rows)]:
            if table:
                temp = out / (name + ".partial")
                with temp.open("w", newline="") as stream:
                    writer = csv.DictWriter(stream, fieldnames=list(table[0]))
                    writer.writeheader()
                    writer.writerows(table)
                temp.replace(out / name)
        atomic_json(
            out / "export_status.json",
            {
                "settings": settings,
                "failed": failed,
                "side_rows": len(summaries),
                "meaning": "CDB snapshot; keff_applied unavailable; full-run currents do not claim selected-segment coverage",
            },
        )
        print(f"Conditions: {out/'conditions.csv'}; incomplete/failed runs: {len(failed)}")
        return int(bool(failed) and not args.allow_missing)
    except (OSError, ValueError, KeyError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1


def phgarfield_main(argv):
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog="Direct file lists: analyze --help. Export only: conditions --help or phgarfield --help.",
    )
    parser.add_argument("--inventory", required=True, type=Path)
    parser.add_argument(
        "--out",
        required=True,
        type=Path,
        help="Working directory; identical completed exports are reused",
    )
    parser.add_argument(
        "--cdb-tag", required=True, help="Match the conditions tag intended for this comparison"
    )
    parser.add_argument(
        "--reference-field",
        required=True,
        type=float,
        help="PHGarfield reference CM field in V/cm; choose explicitly, e.g. 377 or 375",
    )
    parser.add_argument("--reference-pressure", type=float, default=30.0968906)
    parser.add_argument(
        "--reference-temperature-K",
        dest="reference_temperature_K",
        type=float,
        default=299.98646566,
    )
    parser.add_argument(
        "--t0-bins", type=float, default=8.0, help="Configured model t0; NOT a measurement"
    )
    parser.add_argument("--time-bin-ns", type=float, default=56.8)
    parser.add_argument("--step-ns", type=float, default=5.0)
    parser.add_argument("--runs")
    parser.add_argument("--run-list", type=Path)
    parser.add_argument("--root", default="root.exe")
    args = parser.parse_args(argv)
    try:
        for key in (
            "reference_field",
            "reference_pressure",
            "reference_temperature_K",
            "time_bin_ns",
            "step_ns",
        ):
            if not math.isfinite(getattr(args, key)) or getattr(args, key) <= 0:
                raise ValueError(f"{key} must be positive and finite")
        if not math.isfinite(args.t0_bins):
            raise ValueError("t0-bins must be finite")
        executable = shutil.which(args.root)
        if not executable:
            raise ValueError("ROOT executable not found")
        data = inventory(args.inventory)
        choice = selected(args)
        rows = [r for r in data["runs"] if not choice or r["run"] in choice]
        if not rows or choice - {r["run"] for r in rows}:
            raise ValueError("Requested runs absent from inventory")
        out = args.out.expanduser().resolve()
        out.mkdir(parents=True, exist_ok=True)
        settings = {
            k: v for k, v in vars(args).items() if k not in ("inventory", "out", "runs", "run_list")
        }
        settings["compiled_code"] = library_signature(executable)
        # Installation and CDB content can change even at the same path. Use a new --out
        # after changing installed PHGarfield libraries or CDB payloads; defaults are recorded below.
        all_rows, failed = [], []
        for row in rows:
            group, run = row["group"], row["run"]
            work = out / group / str(run) / digest(settings)
            work.mkdir(parents=True, exist_ok=True)
            csv_path = work / "reference.csv"
            if not csv_path.exists():
                driver = work / "export_reference.C"
                setters = "".join(
                    f"qa.{method}({json.dumps(value)}); "
                    for method, value in (
                        ("set_cdb_tag", args.cdb_tag),
                        ("set_reference_field", args.reference_field),
                        ("set_reference_pressure", args.reference_pressure),
                        ("set_reference_temperature", args.reference_temperature_K),
                        ("set_t0_bins", args.t0_bins),
                        ("set_time_bin_ns", args.time_bin_ns),
                        ("set_step_ns", args.step_ns),
                    )
                )
                driver.write_text(
                    compiled_preamble()
                    + "\nvoid export_reference() { try { TpcConditionsReference qa; "
                    + setters
                    + f'qa.export_phgarfield({run},{json.dumps(group)},{json.dumps(str(csv_path.with_suffix(".partial.csv")))}); '
                    + "gSystem->Exit(0); } catch(const std::exception &e) { std::cerr << e.what() << std::endl; gSystem->Exit(1); } }\n"
                )
                with (work / "export.log").open("w") as log:
                    result = subprocess.run(
                        [executable, "-l", "-b", "-q", str(driver)],
                        stdout=log,
                        stderr=subprocess.STDOUT,
                    )
                temp = csv_path.with_suffix(".partial.csv")
                if result.returncode or not temp.exists():
                    failed.append({"group": group, "run": run, "log": str(work / "export.log")})
                    print(
                        f'FAILED {group}/{run}: {work / "export.log"}', file=sys.stderr, flush=True
                    )
                    continue
                temp.replace(csv_path)
                atomic_json(work / "settings.json", settings)
            with csv_path.open(newline="") as stream:
                table = list(csv.DictReader(stream))
            if len(table) != 2 or {r["side"] for r in table} != {"0", "1"}:
                raise ValueError(f"Invalid reference export: {csv_path}")
            all_rows.extend(table)
            print(f"{group}/{run}: PHGarfield reference available", flush=True)
        if all_rows:
            temp = out / "phgarfield_references.partial.csv"
            with temp.open("w", newline="") as stream:
                writer = csv.DictWriter(stream, fieldnames=list(all_rows[0]))
                writer.writeheader()
                writer.writerows(all_rows)
            temp.replace(out / "phgarfield_references.csv")
        atomic_json(
            out / "export_status.json",
            {"settings": settings, "failed": failed, "complete_side_rows": len(all_rows)},
        )
        if all_rows:
            print(
                f"Reference table written: {out / 'phgarfield_references.csv'}; failed runs: {len(failed)}"
            )
        else:
            print(
                f"No valid reference table produced in this invocation; failed runs: {len(failed)}. Inspect the export.log path(s) above."
            )
        return int(bool(failed))
    except (ValueError, OSError, KeyError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1


CODE = Path(__file__).resolve().parent
PATTERN = re.compile(
    r"HIST_DST_TRKR_SEED_(?P<system>run3(?:pp|oo|auau))_(?P<tag>[A-Za-z0-9_.]+)-(?P<run>\d{8})-(?P<segment>\d+)\.root$"
)
SAFE = re.compile(r"[A-Za-z0-9_.-]+$")


def atomic_json(path, value):
    temp = path.with_name(path.name + ".tmp")
    temp.write_text(json.dumps(value, indent=2, sort_keys=True) + "\n")
    temp.replace(path)


def digest(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True).encode()).hexdigest()


def checked_path(path):
    path = Path(path).expanduser().resolve(strict=True)
    if any(x in str(path) for x in ("\t", "\n", "\r")):
        raise ValueError("Paths cannot contain tabs or newlines")
    return path


def runs(text):
    values = set()
    for token in re.split(r"[,\s]+", text.strip()):
        if token:
            if not token.isdecimal():
                raise ValueError("Run list must contain integers; # comments allowed")
            values.add(int(token))
    return values


def selected(args):
    text = args.runs or ""
    if args.run_list:
        text += " " + " ".join(x.split("#", 1)[0] for x in args.run_list.read_text().splitlines())
    return runs(text)


def catalog(args):
    bases = args.base or ["/sphenix/data/data02/sphnxpro/production"]
    found = []
    for base in bases:
        for path in sorted(Path(base).glob("run3*/physics/*/DST_TRKR_SEED")):
            if path.is_dir() and path.parts[-4] in ("run3pp", "run3oo", "run3auau"):
                found.append(path)
    for path in found:
        print(f"--source {path.parts[-4]}={path}")
    if not found:
        raise ValueError("No DST_TRKR_SEED directories found under the supplied base(s)")
    return 0


def fast_candidates(source, system, tag, wanted_runs, limit, start, budget):
    """List only run-range directories; directly probe a bounded set of file names.

    Both endpoints are considered because productions differ in their handling
    of exact hundred-run boundaries. No histogram directory is enumerated.
    """
    ranges = []
    with os.scandir(source) as entries:
        for entry in entries:
            match = re.fullmatch(r"run_(\d{8})_(\d{8})", entry.name)
            if match and entry.is_dir():
                lo, hi = map(int, match.groups())
                if lo >= hi:
                    raise ValueError(f"Invalid production run range: {entry.path}")
                ranges.append((lo, hi, Path(entry.path) / "hist"))
    for run in sorted(wanted_runs):
        folders = sorted({path for lo, hi, path in ranges if lo <= run <= hi})
        if not folders:
            raise ValueError(f"{system}/{run}: no matching run-range directory")
        print(
            f"{system}/{run}: probing filenames for up to {limit} files (segments {start}..{start+budget-1})",
            flush=True,
        )
        found = 0
        probes = 0
        for segment in range(start, start + budget):
            hit = False
            for folder in folders:
                path = folder / f"HIST_DST_TRKR_SEED_{system}_{tag}-{run:08d}-{segment:05d}.root"
                probes += 1
                try:
                    info = path.stat()
                except FileNotFoundError:
                    continue
                if not stat_module.S_ISREG(info.st_mode):
                    raise ValueError(f"Expected a regular ROOT file: {path}")
                hit = True
                yield path
            if hit:
                found += 1
                if found >= limit:
                    break
        print(
            f"{system}/{run}: selected {found} segment IDs after {probes} filename probes",
            flush=True,
        )
        if not found:
            raise ValueError(
                f"{system}/{run}: no QA files inside the bounded probe range. Check the filename tag/source, or set --segment-start / --probe-limit. No full scan was attempted."
            )


def discover(args):
    selection = selected(args)
    system_runs = {}
    for spec in args.system_run_list:
        system, filename = spec.split("=", 1)
        if system in system_runs:
            raise ValueError(f"Duplicate run list for {system}")
        text = " ".join(
            line.split("#", 1)[0] for line in Path(filename).expanduser().read_text().splitlines()
        )
        system_runs[system] = runs(text)
        if not system_runs[system]:
            raise ValueError(f"Empty run list for {system}")
    source_systems = [spec.split("=", 1)[0] for spec in args.source]
    if set(system_runs) - set(source_systems):
        raise ValueError("A per-system run list has no matching --source")
    if args.fast:
        if (
            args.max_files is None
            or args.max_files < 1
            or args.probe_limit < 1
            or args.segment_start < 0
        ):
            raise ValueError(
                "Fast discovery needs --max-files >=1, --probe-limit >=1 and --segment-start >=0"
            )
        if len(source_systems) != len(set(source_systems)):
            raise ValueError(
                "Fast mode needs one source per system; select the desired production source explicitly"
            )
        if len(source_systems) > 1 and any(system not in system_runs for system in source_systems):
            raise ValueError(
                "Fast multi-system discovery needs --system-run-list SYSTEM=FILE for each source"
            )
        if not selection and not system_runs:
            raise ValueError(
                "Fast discovery requires --runs / --run-list or --system-run-list; it does not guess unknown runs"
            )
    elif args.max_files is not None:
        raise ValueError(
            "Discovery --max-files is available with --fast; full discovery otherwise inventories every file"
        )
    requested_runs = selection | set().union(*system_runs.values()) if system_runs else selection
    groups = {}
    logical, canonical = {}, set()
    canonical_parents = {}
    sources = []
    for spec in args.source:
        system, path = spec.split("=", 1)
        if system not in ("run3pp", "run3oo", "run3auau"):
            raise ValueError("Source must be run3pp=PATH, run3oo=PATH or run3auau=PATH")
        source = checked_path(path)
        if source.name != "DST_TRKR_SEED":
            source = checked_path(source / "DST_TRKR_SEED")
        sources.append({"system": system, "path": str(source)})
        wanted_runs = system_runs.get(system, selection)
        if args.fast:
            tag = args.filename_tag or source.parent.name
            if not SAFE.fullmatch(tag) or tag in (".", ".."):
                raise ValueError("Invalid inferred filename tag; set --filename-tag")
            candidates = fast_candidates(
                source,
                system,
                tag,
                wanted_runs,
                args.max_files,
                args.segment_start,
                args.probe_limit,
            )
        else:
            candidates = sorted(source.glob("run_*/hist/HIST_DST_TRKR_SEED_*.root"))
        for path in candidates:
            match = PATTERN.fullmatch(path.name)
            if not match:
                raise ValueError(f"Unrecognized QA filename: {path.name}")
            fields = match.groupdict()
            run, seg = int(fields["run"]), int(fields["segment"])
            if fields["system"] != system:
                raise ValueError(f"System differs from --source: {path}")
            if wanted_runs and run not in wanted_runs:
                continue
            group = f"{system}/{fields['tag']}"
            if args.fast:
                if path.parent not in canonical_parents:
                    canonical_parents[path.parent] = checked_path(path.parent)
                resolved = canonical_parents[path.parent] / path.name
                if resolved.is_symlink():
                    resolved = checked_path(resolved)
            else:
                resolved = checked_path(path)
            key = (group, run, seg)
            if key in logical and logical[key] != str(resolved):
                raise ValueError(f"Conflicting copies of {key}; select one production location")
            if key in logical:
                continue
            if str(resolved) in canonical:
                raise ValueError(f"One physical input assigned to multiple segments: {resolved}")
            logical[key] = str(resolved)
            canonical.add(str(resolved))
            stat = resolved.stat()
            groups.setdefault((group, run), []).append(
                {
                    "segment": seg,
                    "path": str(resolved),
                    "size": stat.st_size,
                    "mtime_ns": stat.st_mtime_ns,
                }
            )
    if not groups:
        raise ValueError("No QA files matched; check --source and the requested runs")
    missing = requested_runs - {run for _, run in groups}
    if missing:
        raise ValueError(f"Requested runs without discovered QA: {sorted(missing)}")
    out = args.out.expanduser().resolve()
    out.mkdir(parents=True, exist_ok=False)
    rows, by_group = [], {}
    for (group, run), files in sorted(groups.items()):
        files.sort(key=lambda f: f["segment"])
        segments = [f["segment"] for f in files]
        gap_count = segments[-1] - segments[0] + 1 - len(segments)
        directory = out / group
        directory.mkdir(parents=True, exist_ok=True)
        listing = directory / f"{run}.inputs.txt"
        listing.write_text("".join(f["path"] + "\n" for f in files))
        rows.append(
            {
                "group": group,
                "run": run,
                "files": files,
                "file_list": str(listing),
                "observed_segment_gaps": gap_count,
            }
        )
        by_group.setdefault(group, []).append(run)
    for group, group_runs in by_group.items():
        (out / group / "runs.txt").write_text("".join(f"{r}\n" for r in group_runs))
    discovery = {
        "mode": "bounded_filename_probes" if args.fast else "full_directory_scan",
        "max_files_per_run": args.max_files,
        "segment_start": args.segment_start if args.fast else None,
        "probe_limit": args.probe_limit if args.fast else None,
        "run_list_scope": (
            "explicit supplied run lists" if args.fast else "QA files discovered on disk"
        ),
    }
    atomic_json(
        out / "inventory.json",
        {
            "schema": 1,
            "discovery": discovery,
            "created_utc": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()),
            "sources": sources,
            "runs": rows,
            "coverage_note": "Selected QA only. Fast mode inventories at most the requested cap within its probe range for the supplied runs; it does not discover new runs or determine total segment counts. Gaps are internal to selected min/max. Production completeness is NOT established.",
        },
    )
    with (out / "runs.csv").open("w", newline="") as stream:
        writer = csv.writer(stream)
        writer.writerow(
            ["group", "run", "nfiles", "first_segment", "last_segment", "internal_gaps", "bytes"]
        )
        for row in rows:
            writer.writerow(
                [
                    row["group"],
                    row["run"],
                    len(row["files"]),
                    row["files"][0]["segment"],
                    row["files"][-1]["segment"],
                    row["observed_segment_gaps"],
                    sum(f["size"] for f in row["files"]),
                ]
            )
    print(
        f"Discovered {len(rows)} run/group combinations, {sum(len(r['files']) for r in rows)} segments"
    )
    for group in by_group:
        print(f"Run list: {out / group / 'runs.txt'}")
    print(f'Inventory: {out / "inventory.json"}')
    return 0


def inventory(path):
    value = json.loads(path.read_text())
    if value.get("schema") != 1 or not value.get("runs"):
        raise ValueError("Unsupported or empty inventory")
    seen = set()
    for row in value["runs"]:
        pieces = row["group"].split("/")
        if len(pieces) != 2 or any(not SAFE.fullmatch(p) or p in (".", "..") for p in pieces):
            raise ValueError("Invalid inventory group")
        key = row["group"], int(row["run"])
        if key in seen or not row["files"]:
            raise ValueError("Duplicate/empty inventory run")
        seen.add(key)
    return value


def root_call(executable, work, expression, label):
    driver = work / f"{label}.C"
    driver.write_text(
        f"{compiled_preamble()}\nvoid {label}() {{ try {{ {expression}; gSystem->Exit(0); }} catch(const std::exception &e) {{ std::cerr << e.what() << std::endl; gSystem->Exit(1); }} }}\n"
    )
    with (work / f"{label}.log").open("w") as log:
        done = subprocess.run(
            [executable, "-l", "-b", "-q", str(driver)], stdout=log, stderr=subprocess.STDOUT
        )
    if done.returncode:
        raise RuntimeError(f'ROOT failed; see {work / (label + ".log")}')


def tables(path):
    if not path:
        return []
    with path.open(newline="") as stream:
        reader = csv.DictReader(stream)
        if not {"group", "run"}.issubset(reader.fieldnames or []):
            raise ValueError("Production tables need group and run columns")
        return list(reader)


def check_inputs(row, files, stage="checking inputs"):
    label = f"{row['group']}/{row['run']}"
    print(f"{label}: {stage}: {len(files)} selected files", flush=True)
    for number, item in enumerate(files, 1):
        # Discovery already records canonical absolute paths. Resolving every
        # parent again causes many unnecessary GPFS metadata requests per file.
        p = Path(item["path"])
        if not p.is_absolute():
            raise ValueError(f"Inventory input path must be absolute: {p}")
        stat = p.stat()
        if stat.st_size != item["size"] or stat.st_mtime_ns != item["mtime_ns"]:
            raise ValueError(f"Input changed since discovery: {p}. Create a fresh inventory.")
        if number % 1000 == 0:
            print(f"{label}: {stage}: {number}/{len(files)}", flush=True)


def process(args):
    data = inventory(args.inventory)
    executable = shutil.which(args.root)
    if not executable:
        raise ValueError("ROOT not found; source your sPHENIX environment or set --root")
    if args.jobs < 1 or args.jobs > 16:
        raise ValueError("--jobs must be between 1 and 16 (start with 2 on shared hosts)")
    pt_windows = parse_windows(args.pt_windows)
    if (
        args.min_entries < 20
        or args.phi_group < 1
        or args.eta_group < 1
        or not __import__("math").isfinite(args.raw_time_shift)
    ):
        raise ValueError("Invalid fit grouping, min-entries or time shift")
    choice = selected(args)
    rows = [r for r in data["runs"] if not choice or r["run"] in choice]
    if not rows or choice - {r["run"] for r in rows}:
        raise ValueError("Requested run is absent from inventory")
    phg, conditions = tables(args.phg_reference_csv), tables(args.conditions_csv)

    def failed_exports(path):
        if path is None:
            return set()
        report = path.parent / "export_status.json"
        if not report.exists():
            return set()
        value = json.loads(report.read_text())
        return {(r["group"], int(r["run"])) for r in value.get("failed", [])}

    phg_failures = failed_exports(args.phg_reference_csv)
    condition_failures = failed_exports(args.conditions_csv)
    signatures = library_signature(executable)
    settings = {
        key: getattr(args, key)
        for key in (
            "pt_windows",
            "min_entries",
            "phi_group",
            "eta_group",
            "prefix",
            "raw_time_shift",
            "phi_qpt",
            "plots",
            "max_files",
            "fit_model",
            "save_slices",
            "require_phg",
            "require_conditions",
            "require_rdca",
            "ratio_floor",
        )
    }
    settings["code"] = signatures
    settings["environment"] = {k: os.getenv(k, "") for k in ("OFFLINE_MAIN", "ROOTSYS")}
    settings["root"] = str(Path(executable).resolve())
    work = args.work.expanduser().resolve()
    work.mkdir(parents=True, exist_ok=True)

    def worker(row):
        group, run = row["group"], row["run"]
        files = row["files"][: args.max_files] if args.max_files else row["files"]
        effective_row = dict(row, files=files)
        matched = lambda table: [r for r in table if r["group"] == group and int(r["run"]) == run]
        cond, predictions = matched(conditions), matched(phg)
        condition_sides = set()
        for condition_row in cond:
            side = condition_row.get("side", "all").strip().lower() or "all"
            side = {"0": "south", "1": "north", "s": "south", "n": "north"}.get(side, side)
            if side not in ("south", "north", "all") or side in condition_sides:
                raise ValueError(f"Invalid/duplicate conditions side for {group}/{run}")
            condition_sides.add(side)
        if args.require_conditions:
            if (group, run) in condition_failures:
                raise ValueError(
                    f"{group}/{run}: conditions exporter marked this run failed; inspect export_status.json"
                )
            if condition_sides != {"south", "north"}:
                raise ValueError(f"{group}/{run}: need one conditions row per physical side")
            for condition_row in cond:
                if condition_row.get("conditions_status") != "complete_snapshot":
                    raise ValueError(f"{group}/{run}: incomplete conditions snapshot")
                for field in ("gem_current_uA", "keff_base"):
                    if not __import__("math").isfinite(float(condition_row.get(field) or "nan")):
                        raise ValueError(f"{group}/{run}: missing conditions {field}")
        if args.require_phg:
            if (group, run) in phg_failures:
                raise ValueError(
                    f"{group}/{run}: PHGarfield exporter marked this run failed; inspect export_status.json"
                )
            if {r.get("side") for r in predictions} != {"0", "1"} or len(predictions) != 2:
                raise ValueError(
                    f"{group}/{run}: require PHGarfield reference velocity for both sides"
                )
            for prediction in predictions:
                value = float(prediction.get("velocity_cm_us") or "nan")
                if not __import__("math").isfinite(value) or value <= 0:
                    raise ValueError(f"{group}/{run}: invalid PHGarfield velocity")
        fingerprint = digest(
            {"row": effective_row, "settings": settings, "conditions": cond, "phg": predictions}
        )
        destination = work / group / str(run) / fingerprint
        destination.mkdir(parents=True, exist_ok=True)
        status = destination / "status.json"
        check_inputs(row, files)
        if (
            status.exists()
            and json.loads(status.read_text()).get("state") == "complete"
            and (destination / "analysis/SUCCESS").is_file()
            and all(
                (destination / "analysis" / tag / "SUCCESS").is_file()
                and (destination / "analysis" / tag / "results.root").is_file()
                for tag, _, _ in pt_windows
            )
        ):
            print(f"{group}/{run}: already complete", flush=True)
            return json.loads(status.read_text())
        lock = destination / ".running"
        lock.mkdir(exist_ok=False)
        try:
            atomic_json(
                status,
                {
                    "state": "running",
                    "fingerprint": fingerprint,
                    "settings": settings,
                    "run": run,
                    "group": group,
                    "source_segments": len(row["files"]),
                },
            )
            listing = destination / "inputs.txt"
            listing.write_text("".join(f["path"] + "\n" for f in files))
            cache = destination / "cache.root"
            if not cache.exists():
                cache_key = digest(
                    {"files": files, "prefix": args.prefix, "reader_code": signatures}
                )
                cache_work = work / group / str(run) / "_cache" / cache_key
                cache_work.mkdir(parents=True, exist_ok=True)
                shared_cache = cache_work / "cache.root"
                if not shared_cache.exists():
                    cache_lock = cache_work / ".running"
                    cache_lock.mkdir(exist_ok=False)
                    try:
                        print(f"{group}/{run}: caching {len(files)} segments", flush=True)
                        temp = cache_work / "cache.partial.root"
                        root_call(
                            executable,
                            cache_work,
                            "TpcCalibrationQA::cache("
                            + ",".join(map(json.dumps, (str(listing), str(temp), args.prefix)))
                            + ")",
                            "make_cache",
                        )
                        check_inputs(row, files, "checking inputs after caching")
                        temp.replace(shared_cache)
                    finally:
                        cache_lock.rmdir()
                cache.symlink_to(shared_cache)
            analysis = destination / "analysis"
            # Preserve failed attempts; the existing runner deliberately refuses existing output dirs.
            if analysis.exists():
                analysis.rename(destination / f"failed_analysis_{time.time_ns()}")
            command = [
                sys.executable,
                str(Path(__file__).resolve()),
                "analyze",
                "--dataset",
                f"{run}={cache}",
                "--reference",
                str(run),
                "--out",
                str(analysis),
                "--root",
                executable,
                "--pt-windows",
                args.pt_windows,
                "--min-entries",
                str(args.min_entries),
                "--phi-group",
                str(args.phi_group),
                "--eta-group",
                str(args.eta_group),
                "--prefix",
                args.prefix,
                "--raw-time-shift",
                str(args.raw_time_shift),
                "--fit-model",
                args.fit_model,
                "--ratio-floor",
                str(args.ratio_floor),
            ]
            if args.save_slices:
                command.append("--save-slices")
            if not args.plots:
                command.append("--no-plots")
            if args.phi_qpt:
                command.append("--phi-qpt")
            for name, table, option in [
                ("conditions", cond, "--conditions-csv"),
                ("phgarfield", predictions, "--phg-reference-csv"),
            ]:
                if not table:
                    continue
                table = [dict(r, label=str(run)) for r in table]
                path = destination / (name + ".csv")
                with path.open("w", newline="") as stream:
                    writer = csv.DictWriter(stream, fieldnames=sorted(table[0]))
                    writer.writeheader()
                    writer.writerows(table)
                command += [option, str(path)]
            with (destination / "analysis.log").open("w") as log:
                completed = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT)
            if completed.returncode or not (analysis / "SUCCESS").is_file():
                raise RuntimeError(f'Analysis failed; see {destination / "analysis.log"}')
            phi_pairs = {}
            for tag, _, _ in pt_windows:
                fits_file = analysis / tag / "slice_fits.csv"
                if not fits_file.exists():
                    phi_pairs[tag] = {}
                    continue
                charges = {}
                with fits_file.open(newline="") as stream:
                    for fit_row in csv.DictReader(stream):
                        observable = fit_row["observable"]
                        if (
                            "_phi_" not in observable
                            or "phiBin" in observable
                            or int(fit_row["status"]) != 0
                        ):
                            continue
                        if observable.endswith("_pos") or observable.endswith("_neg"):
                            base, charge = observable.rsplit("_", 1)
                            charges.setdefault(base, {}).setdefault(charge, set()).add(
                                fit_row["bin"]
                            )
                phi_pairs[tag] = {
                    name: len(values.get("pos", set()) & values.get("neg", set()))
                    for name, values in charges.items()
                }
            atomic_json(
                destination / "qa_checks.json",
                {"matched_positive_negative_phi_bins": phi_pairs, "minimum_for_require_rdca": 8},
            )
            if args.require_rdca and not any(
                count >= 8 for values in phi_pairs.values() for count in values.values()
            ):
                raise RuntimeError(
                    f"{group}/{run}: no population/window has eight accepted charge-paired phi bins; inspect slice_fits.csv and the per-window fit diagnostics"
                )
            check_inputs(row, files, "checking inputs after analysis")
            value = {
                "state": "complete",
                "fingerprint": fingerprint,
                "group": group,
                "run": run,
                "directory": str(destination),
                "source_segments": len(files),
                "limited_input": bool(args.max_files),
                "settings": settings,
            }
            atomic_json(status, value)
            print(f"{group}/{run}: completed ({len(files)} segments)", flush=True)
            return value
        except Exception as error:
            atomic_json(
                status, {"state": "failed", "error": str(error), "fingerprint": fingerprint}
            )
            failure = RuntimeError(str(error))
            failure.qa_directory = str(destination)
            raise failure from error
        finally:
            lock.rmdir()

    records, failed = [], []
    with ThreadPoolExecutor(max_workers=args.jobs) as pool:
        futures = {pool.submit(worker, row): row for row in rows}
        for future in as_completed(futures):
            row = futures[future]
            try:
                records.append(future.result())
            except Exception as error:
                print(f"FAILED {row['group']}/{row['run']}: {error}", file=sys.stderr, flush=True)
                failed.append(
                    {
                        "group": row["group"],
                        "run": row["run"],
                        "error": str(error),
                        "directory": getattr(error, "qa_directory", None),
                    }
                )
    name = f"batch_{time.time_ns()}.json"
    report = {
        "schema": 1,
        "inventory": str(args.inventory.resolve()),
        "settings": settings,
        "complete": sorted(records, key=lambda r: (r["group"], r["run"])),
        "failed": failed,
        "selection_note": "Only runs selected in this invocation. Each run is isolated; failed runs are excluded from complete.",
    }
    atomic_json(work / name, report)
    atomic_json(work / "latest_batch.json", report)
    print(f"Batch record: {work / name}")
    print(f"Completed {len(records)}; failed {len(failed)}")
    return int(bool(failed))


def collect(args):
    batch = json.loads(args.batch.read_text())
    if batch.get("schema") != 1:
        raise ValueError("Unsupported batch record")
    if batch["failed"] and not args.allow_partial:
        raise ValueError("Batch has failed runs. Retry process, or explicitly use --allow-partial.")
    records = batch["complete"]
    if not records:
        raise ValueError("No completed runs")
    references = reference_runs({r["group"] for r in records}, args.reference)
    for group, run in references.items():
        if not any(r["group"] == group and r["run"] == run for r in records):
            raise ValueError(f"Reference {group}/{run} is absent from completed runs")
    out = args.out.expanduser().resolve()
    out.mkdir(parents=True, exist_ok=False)
    manifest = out / "collection.tsv"
    paths = []
    with manifest.open("w") as stream:
        for record in records:
            directory = checked_path(record["directory"])
            status = json.loads((directory / "status.json").read_text())
            if status["state"] != "complete" or status["fingerprint"] != record["fingerprint"]:
                raise ValueError("Per-run state/fingerprint differs from batch record")
            for tag, _, _ in parse_windows(batch["settings"]["pt_windows"]):
                window = directory / "analysis" / tag
                if not (window / "SUCCESS").is_file():
                    raise ValueError(f"Missing completed pT window: {window}")
                path = checked_path(window / "results.root")
                paths.append((record, window))
                stream.write(f"{record['group']}\t{window.name}\t{record['run']}\t{path}\n")
    refs = out / "references.tsv"
    refs.write_text("".join(f"{g}\t{r}\n" for g, r in sorted(references.items())))
    executable = shutil.which(args.root)
    if not executable:
        raise ValueError("ROOT executable not found")
    final = out / "tpc_qa_summary.root"
    temp = out / "tpc_qa_summary.partial.root"
    expr = (
        "TpcCalibrationQA::collect("
        + ",".join(map(json.dumps, (str(manifest), str(refs), str(temp))))
        + f", {str(args.include_maps).lower()}, {str(args.plots).lower()}, {args.ratio_floor!r})"
    )
    root_call(executable, out, expr, "collect_summary")
    # Embed the portable processing record, including all failures and input limits.
    # Paths in this record are provenance; the final ROOT file itself is self-contained.
    condition_scalars = out / "conditions_scalars.tsv"
    with condition_scalars.open("w") as stream:
        for record, window in paths:
            table_path = Path(record["directory"]) / "conditions.csv"
            if not table_path.exists():
                continue
            with table_path.open(newline="") as source:
                for row in csv.DictReader(source):
                    side = {"south": "0", "north": "1"}.get(
                        row.get("side", "").lower(), row.get("side", "")
                    )
                    if side not in ("0", "1"):
                        continue
                    values = []
                    for field in ("gem_current_uA", "keff_base", "keff_applied"):
                        try:
                            value = float(row.get(field) or "nan")
                        except ValueError:
                            raise ValueError(f"Invalid numeric condition {field}")
                        values.append(str(value))
                    stream.write(
                        "\t".join([record["group"], window.name, str(record["run"]), side, *values])
                        + "\n"
                    )
    record_json = json.dumps(
        {
            "batch": batch,
            "references": references,
            "compact": not args.include_maps,
            "ratio_floor_cm": args.ratio_floor,
        }
    )
    expr = f'TpcCalibrationQA::attach_conditions({json.dumps(str(condition_scalars))},{json.dumps(str(temp))}); TFile f({json.dumps(str(temp))},"UPDATE"); f.cd("metadata"); TNamed n("production_record_json",{json.dumps(record_json)}); n.Write(); f.Close()'
    root_call(executable, out, expr, "record_summary")
    temp.replace(final)
    # Machine-readable parameter tables are also convenient for fitting response to k_eff offline.
    for name in ("trends.csv", "drift_summary.csv"):
        output_rows = []
        for record, window in paths:
            source = window / name
            if not source.exists():
                continue
            with source.open(newline="") as stream:
                for row in csv.DictReader(stream):
                    output_rows.append(dict(row, group=record["group"], pt_window=window.name))
        if output_rows:
            with (out / name).open("w", newline="") as stream:
                writer = csv.DictWriter(stream, fieldnames=list(output_rows[0]))
                writer.writeheader()
                writer.writerows(output_rows)
    (out / "SUCCESS").write_text("Combined summary complete\n")
    print(f"Download this file: {final} ({final.stat().st_size / 1024**2:.2f} MiB)")
    return 0


def main(argv=None):
    parser = argparse.ArgumentParser(
        description=__doc__,
        epilog="Direct file lists: analyze --help. Export only: conditions --help or phgarfield --help.",
    )
    sub = parser.add_subparsers(dest="action", required=True)
    p = sub.add_parser("catalog", help="List production source directories on this host")
    p.add_argument(
        "--base",
        action="append",
        help="Production parent, e.g. /sphenix/data/data02/sphnxpro/production; repeat for other disks",
    )
    p = sub.add_parser("discover", help="Create QA-based run lists and immutable segment inventory")
    p.add_argument("--source", action="append", required=True, metavar="SYSTEM=PATH")
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--runs")
    p.add_argument("--run-list", type=Path)
    p.add_argument("--system-run-list", action="append", default=[], metavar="SYSTEM=FILE")
    p.add_argument(
        "--fast",
        action="store_true",
        help="Probe known filenames for explicit runs; never list histogram directories",
    )
    p.add_argument("--max-files", type=int, help="Fast discovery: up to this many files per run")
    p.add_argument("--segment-start", type=int, default=0)
    p.add_argument(
        "--probe-limit",
        type=int,
        default=200,
        help="Maximum segment IDs to try per run in fast mode",
    )
    p.add_argument(
        "--filename-tag", help="Override filename tag inferred from production directory name"
    )
    p = sub.add_parser(
        "process",
        aliases=["run"],
        help="Process QA; run also exports conditions/references and collects",
    )
    p.add_argument("--inventory", type=Path, required=True)
    p.add_argument("--work", type=Path, required=True)
    p.add_argument("--root", default="root.exe")
    p.add_argument("--jobs", type=int, default=2)
    p.add_argument("--runs")
    p.add_argument("--run-list", type=Path)
    p.add_argument("--pt-windows", default="0.5:1,1:2,2:4")
    p.add_argument("--ratio-floor", type=float, default=0.02)
    p.add_argument("--min-entries", type=int, default=100)
    p.add_argument("--phi-group", type=int, default=3)
    p.add_argument("--eta-group", type=int, default=4)
    p.add_argument(
        "--require-phg",
        action="store_true",
        help="Require supplied PHGarfield velocities for both sides",
    )
    p.add_argument(
        "--require-conditions",
        action="store_true",
        help="Require complete side-separated current/base-kEff snapshots",
    )
    p.add_argument(
        "--require-rdca",
        action="store_true",
        help="Require >=8 accepted charge-paired phi bins in at least one population/window",
    )
    p.add_argument("--fit-model", choices=["gaussian", "gaussian-background"], default="gaussian")
    p.add_argument("--save-slices", action="store_true")
    p.add_argument("--prefix", default="h_TpcSeedsQA_")
    p.add_argument("--raw-time-shift", type=float, default=40.0)
    p.add_argument("--conditions-csv", type=Path)
    p.add_argument("--phg-reference-csv", type=Path)
    p.add_argument("--phi-qpt", action="store_true")
    p.add_argument("--plots", action="store_true")
    p.add_argument(
        "--max-files", type=int, help="Quick test only: truncate each run; recorded in summary"
    )
    p.add_argument("--out", type=Path, help="run: output directory for the portable ROOT summary")
    p.add_argument("--cdb-tag", help="run: explicit conditions tag")
    p.add_argument(
        "--reference-field", type=float, help="run: explicit PHGarfield reference field in V/cm"
    )
    p.add_argument("--keff-fallback", choices=["none", "phgarfield-defaults"], default="none")
    p.add_argument(
        "--reference",
        action="append",
        default=[],
        metavar="SYSTEM[/TAG]=RUN",
        help="run: reference for each selected production",
    )
    p.add_argument("--allow-partial", action="store_true")
    p.add_argument("--include-maps", action="store_true")

    p = sub.add_parser(
        "collect", help="Combine per-run profiles and compare to a reference per production"
    )
    p.add_argument("--batch", type=Path, required=True)
    p.add_argument("--reference", action="append", required=True, metavar="SYSTEM[/TAG]=RUN")
    p.add_argument("--out", type=Path, required=True)
    p.add_argument("--root", default="root.exe")
    p.add_argument("--allow-partial", action="store_true")
    p.add_argument("--include-maps", action="store_true")
    p.add_argument("--plots", action="store_true")
    p.add_argument("--ratio-floor", type=float, default=0.02)
    args = parser.parse_args(argv)
    try:
        if getattr(args, "max_files", None) is not None and args.max_files < 1:
            raise ValueError("--max-files must be positive")
        if hasattr(args, "ratio_floor") and (
            not __import__("math").isfinite(args.ratio_floor) or args.ratio_floor < 0
        ):
            raise ValueError("--ratio-floor must be finite and nonnegative")
        return globals()[args.action](args)
    except (ValueError, OSError, RuntimeError, KeyError) as error:
        print(f"ERROR: {error}", file=sys.stderr)
        return 1


if __name__ == "__main__":
    handlers = {
        "analyze": analyze_main,
        "conditions": conditions_main,
        "phgarfield": phgarfield_main,
    }
    if len(sys.argv) > 1 and sys.argv[1] in handlers:
        sys.exit(handlers[sys.argv[1]](sys.argv[2:]))
    sys.exit(main())
