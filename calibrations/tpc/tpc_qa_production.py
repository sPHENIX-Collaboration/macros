#!/usr/bin/env python3
"""Inventory existing production QA, cache once/run, resume fitting, collect a portable ROOT file.

Uses Python stdlib and the ROOT executable. Never submits jobs or changes production files.
"""
import argparse
from concurrent.futures import ThreadPoolExecutor, as_completed
import csv
import hashlib
import json
import os
from pathlib import Path
import re
import shutil
import stat as stat_module
import subprocess
import sys
import time
from run_tpc_distortion_qa import windows as parse_windows

CODE = Path(__file__).resolve().parent
PATTERN = re.compile(r"HIST_DST_TRKR_SEED_(?P<system>run3(?:pp|oo|auau))_(?P<tag>[A-Za-z0-9_.]+)-(?P<run>\d{8})-(?P<segment>\d+)\.root$")
SAFE = re.compile(r"[A-Za-z0-9_.-]+$")


def atomic_json(path, value):
    temp = path.with_name(path.name + '.tmp')
    temp.write_text(json.dumps(value, indent=2, sort_keys=True) + '\n')
    temp.replace(path)


def digest(value):
    return hashlib.sha256(json.dumps(value, sort_keys=True).encode()).hexdigest()


def checked_path(path):
    path = Path(path).expanduser().resolve(strict=True)
    if any(x in str(path) for x in ('\t', '\n', '\r')):
        raise ValueError('Paths cannot contain tabs or newlines')
    return path


def runs(text):
    values = set()
    for token in re.split(r'[,\s]+', text.strip()):
        if token:
            if not token.isdecimal():
                raise ValueError('Run list must contain integers; # comments allowed')
            values.add(int(token))
    return values


def selected(args):
    text = args.runs or ''
    if args.run_list:
        text += ' ' + ' '.join(x.split('#', 1)[0] for x in args.run_list.read_text().splitlines())
    return runs(text)


def catalog(args):
    bases = args.base or ['/sphenix/data/data02/sphnxpro/production']
    found = []
    for base in bases:
        for path in sorted(Path(base).glob('run3*/physics/*/DST_TRKR_SEED')):
            if path.is_dir() and path.parts[-4] in ('run3pp', 'run3oo', 'run3auau'):
                found.append(path)
    for path in found:
        print(f'--source {path.parts[-4]}={path}')
    if not found:
        raise ValueError('No DST_TRKR_SEED directories found under the supplied base(s)')
    return 0


def fast_candidates(source, system, tag, wanted_runs, limit, start, budget):
    """List only run-range directories; directly probe a bounded set of file names.

    Both endpoints are considered because productions differ in their handling
    of exact hundred-run boundaries. No histogram directory is enumerated.
    """
    ranges = []
    with os.scandir(source) as entries:
        for entry in entries:
            match = re.fullmatch(r'run_(\d{8})_(\d{8})', entry.name)
            if match and entry.is_dir():
                lo, hi = map(int, match.groups())
                if lo >= hi:
                    raise ValueError(f'Invalid production run range: {entry.path}')
                ranges.append((lo, hi, Path(entry.path) / 'hist'))
    for run in sorted(wanted_runs):
        folders = sorted({path for lo, hi, path in ranges if lo <= run <= hi})
        if not folders:
            raise ValueError(f'{system}/{run}: no matching run-range directory')
        print(f'{system}/{run}: probing filenames for up to {limit} files (segments {start}..{start+budget-1})', flush=True)
        found = 0
        probes = 0
        for segment in range(start, start + budget):
            hit = False
            for folder in folders:
                path = folder / f'HIST_DST_TRKR_SEED_{system}_{tag}-{run:08d}-{segment:05d}.root'
                probes += 1
                try:
                    info = path.stat()
                except FileNotFoundError:
                    continue
                if not stat_module.S_ISREG(info.st_mode):
                    raise ValueError(f'Expected a regular ROOT file: {path}')
                hit = True
                yield path
            if hit:
                found += 1
                if found >= limit:
                    break
        print(f'{system}/{run}: selected {found} segment IDs after {probes} filename probes', flush=True)
        if not found:
            raise ValueError(f'{system}/{run}: no QA files inside the bounded probe range. Check the filename tag/source, or set --segment-start / --probe-limit. No full scan was attempted.')


def discover(args):
    selection = selected(args)
    system_runs = {}
    for spec in args.system_run_list:
        system, filename = spec.split('=', 1)
        if system in system_runs:
            raise ValueError(f'Duplicate run list for {system}')
        text = ' '.join(line.split('#', 1)[0] for line in Path(filename).expanduser().read_text().splitlines())
        system_runs[system] = runs(text)
        if not system_runs[system]:
            raise ValueError(f'Empty run list for {system}')
    source_systems = [spec.split('=', 1)[0] for spec in args.source]
    if set(system_runs) - set(source_systems):
        raise ValueError('A per-system run list has no matching --source')
    if args.fast:
        if args.max_files is None or args.max_files < 1 or args.probe_limit < 1 or args.segment_start < 0:
            raise ValueError('Fast discovery needs --max-files >=1, --probe-limit >=1 and --segment-start >=0')
        if len(source_systems) != len(set(source_systems)):
            raise ValueError('Fast mode needs one source per system; select the desired production source explicitly')
        if len(source_systems) > 1 and any(system not in system_runs for system in source_systems):
            raise ValueError('Fast multi-system discovery needs --system-run-list SYSTEM=FILE for each source')
        if not selection and not system_runs:
            raise ValueError('Fast discovery requires --runs / --run-list or --system-run-list; it does not guess unknown runs')
    elif args.max_files is not None:
        raise ValueError('Discovery --max-files is available with --fast; full discovery otherwise inventories every file')
    requested_runs = selection | set().union(*system_runs.values()) if system_runs else selection
    groups = {}
    logical, canonical = {}, set()
    canonical_parents = {}
    sources = []
    for spec in args.source:
        system, path = spec.split('=', 1)
        if system not in ('run3pp', 'run3oo', 'run3auau'):
            raise ValueError('Source must be run3pp=PATH, run3oo=PATH or run3auau=PATH')
        source = checked_path(path)
        if source.name != 'DST_TRKR_SEED':
            source = checked_path(source / 'DST_TRKR_SEED')
        sources.append({'system': system, 'path': str(source)})
        wanted_runs = system_runs.get(system, selection)
        if args.fast:
            tag = args.filename_tag or source.parent.name
            if not SAFE.fullmatch(tag) or tag in ('.', '..'):
                raise ValueError('Invalid inferred filename tag; set --filename-tag')
            candidates = fast_candidates(source, system, tag, wanted_runs, args.max_files, args.segment_start, args.probe_limit)
        else:
            candidates = sorted(source.glob('run_*/hist/HIST_DST_TRKR_SEED_*.root'))
        for path in candidates:
            match = PATTERN.fullmatch(path.name)
            if not match:
                raise ValueError(f'Unrecognized QA filename: {path.name}')
            fields = match.groupdict()
            run, seg = int(fields['run']), int(fields['segment'])
            if fields['system'] != system:
                raise ValueError(f'System differs from --source: {path}')
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
                raise ValueError(f'Conflicting copies of {key}; select one production location')
            if key in logical:
                continue
            if str(resolved) in canonical:
                raise ValueError(f'One physical input assigned to multiple segments: {resolved}')
            logical[key] = str(resolved)
            canonical.add(str(resolved))
            stat = resolved.stat()
            groups.setdefault((group, run), []).append({'segment': seg, 'path': str(resolved), 'size': stat.st_size, 'mtime_ns': stat.st_mtime_ns})
    if not groups:
        raise ValueError('No QA files matched; check --source and the requested runs')
    missing = requested_runs - {run for _, run in groups}
    if missing:
        raise ValueError(f'Requested runs without discovered QA: {sorted(missing)}')
    out = args.out.expanduser().resolve()
    out.mkdir(parents=True, exist_ok=False)
    rows, by_group = [], {}
    for (group, run), files in sorted(groups.items()):
        files.sort(key=lambda f: f['segment'])
        segments = [f['segment'] for f in files]
        gap_count = segments[-1] - segments[0] + 1 - len(segments)
        directory = out / group
        directory.mkdir(parents=True, exist_ok=True)
        listing = directory / f'{run}.inputs.txt'
        listing.write_text(''.join(f["path"] + '\n' for f in files))
        rows.append({'group': group, 'run': run, 'files': files, 'file_list': str(listing), 'observed_segment_gaps': gap_count})
        by_group.setdefault(group, []).append(run)
    for group, group_runs in by_group.items():
        (out / group / 'runs.txt').write_text(''.join(f'{r}\n' for r in group_runs))
    discovery = {'mode': 'bounded_filename_probes' if args.fast else 'full_directory_scan', 'max_files_per_run': args.max_files, 'segment_start': args.segment_start if args.fast else None, 'probe_limit': args.probe_limit if args.fast else None, 'run_list_scope': 'explicit supplied run lists' if args.fast else 'QA files discovered on disk'}
    atomic_json(out / 'inventory.json', {'schema': 1, 'discovery': discovery, 'created_utc': time.strftime('%Y-%m-%dT%H:%M:%SZ', time.gmtime()), 'sources': sources, 'runs': rows,
        'coverage_note': 'Selected QA only. Fast mode inventories at most the requested cap within its probe range for the supplied runs; it does not discover new runs or determine total segment counts. Gaps are internal to selected min/max. Production completeness is NOT established.'})
    with (out / 'runs.csv').open('w', newline='') as stream:
        writer = csv.writer(stream)
        writer.writerow(['group', 'run', 'nfiles', 'first_segment', 'last_segment', 'internal_gaps', 'bytes'])
        for row in rows:
            writer.writerow([row['group'], row['run'], len(row['files']), row['files'][0]['segment'], row['files'][-1]['segment'], row['observed_segment_gaps'], sum(f['size'] for f in row['files'])])
    print(f"Discovered {len(rows)} run/group combinations, {sum(len(r['files']) for r in rows)} segments")
    for group in by_group:
        print(f"Run list: {out / group / 'runs.txt'}")
    print(f'Inventory: {out / "inventory.json"}')
    return 0


def inventory(path):
    value = json.loads(path.read_text())
    if value.get('schema') != 1 or not value.get('runs'):
        raise ValueError('Unsupported or empty inventory')
    seen = set()
    for row in value['runs']:
        pieces = row['group'].split('/')
        if len(pieces) != 2 or any(not SAFE.fullmatch(p) or p in ('.', '..') for p in pieces):
            raise ValueError('Invalid inventory group')
        key = row['group'], int(row['run'])
        if key in seen or not row['files']:
            raise ValueError('Duplicate/empty inventory run')
        seen.add(key)
    return value


def root_call(executable, work, expression, label):
    driver = work / f'{label}.C'
    driver.write_text(f'#include {json.dumps(str(CODE / "TPCProductionQA.C"))}\nvoid {label}() {{ try {{ {expression}; gSystem->Exit(0); }} catch(const std::exception &e) {{ std::cerr << e.what() << std::endl; gSystem->Exit(1); }} }}\n')
    with (work / f'{label}.log').open('w') as log:
        done = subprocess.run([executable, '-l', '-b', '-q', str(driver)], stdout=log, stderr=subprocess.STDOUT)
    if done.returncode:
        raise RuntimeError(f'ROOT failed; see {work / (label + ".log")}')


def tables(path):
    if not path:
        return []
    with path.open(newline='') as stream:
        reader = csv.DictReader(stream)
        if not {'group', 'run'}.issubset(reader.fieldnames or []):
            raise ValueError('Production tables need group and run columns')
        return list(reader)


def check_inputs(row, files, stage="checking inputs"):
    label = f"{row['group']}/{row['run']}"
    print(f"{label}: {stage}: {len(files)} selected files", flush=True)
    for number, item in enumerate(files, 1):
        # Discovery already records canonical absolute paths. Resolving every
        # parent again causes many unnecessary GPFS metadata requests per file.
        p = Path(item['path'])
        if not p.is_absolute():
            raise ValueError(f'Inventory input path must be absolute: {p}')
        stat = p.stat()
        if stat.st_size != item['size'] or stat.st_mtime_ns != item['mtime_ns']:
            raise ValueError(f'Input changed since discovery: {p}. Create a fresh inventory.')
        if number % 1000 == 0:
            print(f"{label}: {stage}: {number}/{len(files)}", flush=True)


def process(args):
    data = inventory(args.inventory)
    executable = shutil.which(args.root)
    if not executable:
        raise ValueError('ROOT not found; source your sPHENIX environment or set --root')
    if args.jobs < 1 or args.jobs > 16:
        raise ValueError('--jobs must be between 1 and 16 (start with 2 on shared hosts)')
    pt_windows = parse_windows(args.pt_windows)
    if args.min_entries < 20 or args.phi_group < 1 or args.eta_group < 1 or not __import__('math').isfinite(args.raw_time_shift):
        raise ValueError('Invalid fit grouping, min-entries or time shift')
    choice = selected(args)
    rows = [r for r in data['runs'] if not choice or r['run'] in choice]
    if not rows or choice - {r['run'] for r in rows}:
        raise ValueError('Requested run is absent from inventory')
    phg, conditions = tables(args.phg_reference_csv), tables(args.conditions_csv)
    def failed_exports(path):
        if path is None:
            return set()
        report = path.parent / 'export_status.json'
        if not report.exists():
            return set()
        value = json.loads(report.read_text())
        return {(r['group'], int(r['run'])) for r in value.get('failed', [])}
    phg_failures = failed_exports(args.phg_reference_csv)
    condition_failures = failed_exports(args.conditions_csv)
    signatures = {p.name: hashlib.sha256(p.read_bytes()).hexdigest() for p in (CODE / 'TPCProductionQA.C', CODE / 'TPCDistortionQA.C', CODE / 'TPCDriftQA.h', CODE / 'run_tpc_distortion_qa.py', CODE / 'tpc_qa_production.py')}
    settings = {key: getattr(args, key) for key in ('pt_windows', 'min_entries', 'phi_group', 'eta_group', 'prefix', 'raw_time_shift', 'phi_qpt', 'plots', 'max_files', 'fit_model', 'save_slices', 'require_phg', 'require_conditions', 'require_rdca')}
    settings['code'] = signatures
    settings['environment'] = {k: os.getenv(k, '') for k in ('OFFLINE_MAIN', 'ROOTSYS')}
    settings['root'] = str(Path(executable).resolve())
    work = args.work.expanduser().resolve()
    work.mkdir(parents=True, exist_ok=True)
    def worker(row):
        group, run = row['group'], row['run']
        files = row['files'][:args.max_files] if args.max_files else row['files']
        effective_row = dict(row, files=files)
        matched = lambda table: [r for r in table if r['group'] == group and int(r['run']) == run]
        cond, predictions = matched(conditions), matched(phg)
        condition_sides = set()
        for condition_row in cond:
            side = condition_row.get('side', 'all').strip().lower() or 'all'
            side = {'0': 'south', '1': 'north', 's': 'south', 'n': 'north'}.get(side, side)
            if side not in ('south', 'north', 'all') or side in condition_sides:
                raise ValueError(f'Invalid/duplicate conditions side for {group}/{run}')
            condition_sides.add(side)
        if args.require_conditions:
            if (group, run) in condition_failures:
                raise ValueError(f'{group}/{run}: conditions exporter marked this run failed; inspect export_status.json')
            if condition_sides != {'south', 'north'}:
                raise ValueError(f'{group}/{run}: need one conditions row per physical side')
            for condition_row in cond:
                if condition_row.get('conditions_status') != 'complete_snapshot':
                    raise ValueError(f'{group}/{run}: incomplete conditions snapshot')
                for field in ('gem_current_uA', 'keff_base'):
                    if not __import__('math').isfinite(float(condition_row.get(field) or 'nan')):
                        raise ValueError(f'{group}/{run}: missing conditions {field}')
        if args.require_phg:
            if (group, run) in phg_failures:
                raise ValueError(f'{group}/{run}: PHGarfield exporter marked this run failed; inspect export_status.json')
            if {r.get('side') for r in predictions} != {'0', '1'} or len(predictions) != 2:
                raise ValueError(f'{group}/{run}: require PHGarfield reference velocity for both sides')
            for prediction in predictions:
                value = float(prediction.get('velocity_cm_us') or 'nan')
                if not __import__('math').isfinite(value) or value <= 0:
                    raise ValueError(f'{group}/{run}: invalid PHGarfield velocity')
        fingerprint = digest({'row': effective_row, 'settings': settings, 'conditions': cond, 'phg': predictions})
        destination = work / group / str(run) / fingerprint
        destination.mkdir(parents=True, exist_ok=True)
        status = destination / 'status.json'
        check_inputs(row, files)
        if status.exists() and json.loads(status.read_text()).get('state') == 'complete' and (destination / 'analysis/SUCCESS').is_file() and all((destination / 'analysis' / tag / 'SUCCESS').is_file() and (destination / 'analysis' / tag / 'results.root').is_file() for tag, _, _ in pt_windows):
            print(f'{group}/{run}: already complete', flush=True)
            return json.loads(status.read_text())
        lock = destination / '.running'
        lock.mkdir(exist_ok=False)
        try:
            atomic_json(status, {'state': 'running', 'fingerprint': fingerprint, 'settings': settings, 'run': run, 'group': group, 'source_segments': len(row['files'])})
            listing = destination / 'inputs.txt'
            listing.write_text(''.join(f['path'] + '\n' for f in files))
            cache = destination / 'cache.root'
            if not cache.exists():
                cache_key = digest({'files': files, 'prefix': args.prefix, 'reader_code': {k: signatures[k] for k in ('TPCProductionQA.C', 'TPCDistortionQA.C', 'TPCDriftQA.h')}})
                cache_work = work / group / str(run) / '_cache' / cache_key
                cache_work.mkdir(parents=True, exist_ok=True)
                shared_cache = cache_work / 'cache.root'
                if not shared_cache.exists():
                    cache_lock = cache_work / '.running'
                    cache_lock.mkdir(exist_ok=False)
                    try:
                        print(f'{group}/{run}: caching {len(files)} segments', flush=True)
                        temp = cache_work / 'cache.partial.root'
                        root_call(executable, cache_work, 'TPCBatchQA::cache(' + ','.join(map(json.dumps, (str(listing), str(temp), args.prefix))) + ')', 'make_cache')
                        check_inputs(row, files, "checking inputs after caching")
                        temp.replace(shared_cache)
                    finally:
                        cache_lock.rmdir()
                cache.symlink_to(shared_cache)
            analysis = destination / 'analysis'
            # Preserve failed attempts; the existing runner deliberately refuses existing output dirs.
            if analysis.exists():
                analysis.rename(destination / f'failed_analysis_{time.time_ns()}')
            command = [sys.executable, str(CODE / 'run_tpc_distortion_qa.py'), '--dataset', f'{run}={cache}', '--reference', str(run), '--out', str(analysis), '--root', executable,
                '--pt-windows', args.pt_windows, '--min-entries', str(args.min_entries), '--phi-group', str(args.phi_group), '--eta-group', str(args.eta_group), '--prefix', args.prefix, '--raw-time-shift', str(args.raw_time_shift), '--fit-model', args.fit_model]
            if args.save_slices:
                command.append('--save-slices')
            if not args.plots:
                command.append('--no-plots')
            if args.phi_qpt:
                command.append('--phi-qpt')
            for name, table, option in [('conditions', cond, '--conditions-csv'), ('phgarfield', predictions, '--phg-reference-csv')]:
                if not table:
                    continue
                table = [dict(r, label=str(run)) for r in table]
                path = destination / (name + '.csv')
                with path.open('w', newline='') as stream:
                    writer = csv.DictWriter(stream, fieldnames=sorted(table[0]))
                    writer.writeheader(); writer.writerows(table)
                command += [option, str(path)]
            with (destination / 'analysis.log').open('w') as log:
                completed = subprocess.run(command, stdout=log, stderr=subprocess.STDOUT)
            if completed.returncode or not (analysis / 'SUCCESS').is_file():
                raise RuntimeError(f'Analysis failed; see {destination / "analysis.log"}')
            phi_pairs = {}
            for tag, _, _ in pt_windows:
                fits_file = analysis / tag / 'slice_fits.csv'
                if not fits_file.exists():
                    phi_pairs[tag] = {}
                    continue
                charges = {}
                with fits_file.open(newline='') as stream:
                    for fit_row in csv.DictReader(stream):
                        observable = fit_row['observable']
                        if '_phi_' not in observable or 'phiBin' in observable or int(fit_row['status']) != 0:
                            continue
                        if observable.endswith('_pos') or observable.endswith('_neg'):
                            base, charge = observable.rsplit('_', 1)
                            charges.setdefault(base, {}).setdefault(charge, set()).add(fit_row['bin'])
                phi_pairs[tag] = {name: len(values.get('pos', set()) & values.get('neg', set())) for name, values in charges.items()}
            atomic_json(destination / 'qa_checks.json', {'matched_positive_negative_phi_bins': phi_pairs, 'minimum_for_require_rdca': 8})
            if args.require_rdca and not any(count >= 8 for values in phi_pairs.values() for count in values.values()):
                raise RuntimeError(f'{group}/{run}: no population/window has eight accepted charge-paired phi bins; inspect slice_fits.csv and diagnose_tpc_qa.py')
            check_inputs(row, files, "checking inputs after analysis")
            value = {'state': 'complete', 'fingerprint': fingerprint, 'group': group, 'run': run, 'directory': str(destination), 'source_segments': len(files), 'limited_input': bool(args.max_files), 'settings': settings}
            atomic_json(status, value)
            print(f'{group}/{run}: completed ({len(files)} segments)', flush=True)
            return value
        except Exception as error:
            atomic_json(status, {'state': 'failed', 'error': str(error), 'fingerprint': fingerprint})
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
                failed.append({'group': row['group'], 'run': row['run'], 'error': str(error), 'directory': getattr(error, 'qa_directory', None)})
    name = f'batch_{time.time_ns()}.json'
    report = {'schema': 1, 'inventory': str(args.inventory.resolve()), 'settings': settings, 'complete': sorted(records, key=lambda r: (r['group'], r['run'])), 'failed': failed,
              'selection_note': 'Only runs selected in this invocation. Each run is isolated; failed runs are excluded from complete.'}
    atomic_json(work / name, report)
    atomic_json(work / 'latest_batch.json', report)
    print(f'Batch record: {work / name}')
    print(f'Completed {len(records)}; failed {len(failed)}')
    return int(bool(failed))


def collect(args):
    batch = json.loads(args.batch.read_text())
    if batch.get('schema') != 1:
        raise ValueError('Unsupported batch record')
    if batch['failed'] and not args.allow_partial:
        raise ValueError('Batch has failed runs. Retry process, or explicitly use --allow-partial.')
    records = batch['complete']
    if not records:
        raise ValueError('No completed runs')
    references = {}
    groups = {r['group'] for r in records}
    for value in args.reference:
        group, run = value.split('=', 1)
        matches = [g for g in groups if g == group or g.split('/')[0] == group]
        if len(matches) != 1 or matches[0] in references:
            raise ValueError(f'Ambiguous/duplicate reference group {group}; specify system/tag')
        references[matches[0]] = int(run)
    if set(references) != groups:
        raise ValueError(f'Provide --reference GROUP=RUN for each group: {sorted(groups)}')
    for group, run in references.items():
        if not any(r['group'] == group and r['run'] == run for r in records):
            raise ValueError(f'Reference {group}/{run} is absent from completed runs')
    out = args.out.expanduser().resolve()
    out.mkdir(parents=True, exist_ok=False)
    manifest = out / 'collection.tsv'
    paths = []
    with manifest.open('w') as stream:
        for record in records:
            directory = checked_path(record['directory'])
            status = json.loads((directory / 'status.json').read_text())
            if status['state'] != 'complete' or status['fingerprint'] != record['fingerprint']:
                raise ValueError('Per-run state/fingerprint differs from batch record')
            for tag, _, _ in parse_windows(batch['settings']['pt_windows']):
                window = directory / 'analysis' / tag
                if not (window / 'SUCCESS').is_file():
                    raise ValueError(f'Missing completed pT window: {window}')
                path = checked_path(window / 'results.root')
                paths.append((record, window))
                stream.write(f"{record['group']}\t{window.name}\t{record['run']}\t{path}\n")
    refs = out / 'references.tsv'
    refs.write_text(''.join(f'{g}\t{r}\n' for g, r in sorted(references.items())))
    executable = shutil.which(args.root)
    if not executable:
        raise ValueError('ROOT executable not found')
    final = out / 'tpc_qa_summary.root'
    temp = out / 'tpc_qa_summary.partial.root'
    expr = 'TPCBatchQA::collect(' + ','.join(map(json.dumps, (str(manifest), str(refs), str(temp)))) + f', {str(args.include_maps).lower()}, {str(args.plots).lower()}, {args.ratio_floor!r})'
    root_call(executable, out, expr, 'collect_summary')
    # Embed the portable processing record, including all failures and input limits.
    # Paths in this record are provenance; the final ROOT file itself is self-contained.
    condition_scalars = out / 'conditions_scalars.tsv'
    with condition_scalars.open('w') as stream:
        for record, window in paths:
            table_path = Path(record['directory']) / 'conditions.csv'
            if not table_path.exists():
                continue
            with table_path.open(newline='') as source:
                for row in csv.DictReader(source):
                    side = {'south': '0', 'north': '1'}.get(row.get('side', '').lower(), row.get('side', ''))
                    if side not in ('0', '1'):
                        continue
                    values = []
                    for field in ('gem_current_uA', 'keff_base', 'keff_applied'):
                        try:
                            value = float(row.get(field) or 'nan')
                        except ValueError:
                            raise ValueError(f'Invalid numeric condition {field}')
                        values.append(str(value))
                    stream.write('\t'.join([record['group'], window.name, str(record['run']), side, *values]) + '\n')
    record_json = json.dumps({'batch': batch, 'references': references, 'compact': not args.include_maps, 'ratio_floor_cm': args.ratio_floor})
    expr = f'TPCBatchQA::attachConditions({json.dumps(str(condition_scalars))},{json.dumps(str(temp))}); TFile f({json.dumps(str(temp))},"UPDATE"); f.cd("metadata"); TNamed n("production_record_json",{json.dumps(record_json)}); n.Write(); f.Close()'
    root_call(executable, out, expr, 'record_summary')
    temp.replace(final)
    # Machine-readable parameter tables are also convenient for fitting response to k_eff offline.
    for name in ('trends.csv', 'drift_summary.csv'):
        output_rows = []
        for record, window in paths:
            source = window / name
            if not source.exists():
                continue
            with source.open(newline='') as stream:
                for row in csv.DictReader(stream):
                    output_rows.append(dict(row, group=record['group'], pt_window=window.name))
        if output_rows:
            with (out / name).open('w', newline='') as stream:
                writer = csv.DictWriter(stream, fieldnames=list(output_rows[0]))
                writer.writeheader(); writer.writerows(output_rows)
    (out / 'SUCCESS').write_text('Combined summary complete\n')
    print(f'Download this file: {final} ({final.stat().st_size / 1024**2:.2f} MiB)')
    return 0


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest='action', required=True)
    p = sub.add_parser('catalog', help='List production source directories on this host')
    p.add_argument('--base', action='append', help='Production parent, e.g. /sphenix/data/data02/sphnxpro/production; repeat for other disks')
    p = sub.add_parser('discover', help='Create QA-based run lists and immutable segment inventory')
    p.add_argument('--source', action='append', required=True, metavar='SYSTEM=PATH')
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--runs'); p.add_argument('--run-list', type=Path)
    p.add_argument('--system-run-list', action='append', default=[], metavar='SYSTEM=FILE')
    p.add_argument('--fast', action='store_true', help='Probe known filenames for explicit runs; never list histogram directories')
    p.add_argument('--max-files', type=int, help='Fast discovery: up to this many files per run')
    p.add_argument('--segment-start', type=int, default=0)
    p.add_argument('--probe-limit', type=int, default=200, help='Maximum segment IDs to try per run in fast mode')
    p.add_argument('--filename-tag', help='Override filename tag inferred from production directory name')
    p = sub.add_parser('process', help='One cache per run; resume completed identical settings')
    p.add_argument('--inventory', type=Path, required=True)
    p.add_argument('--work', type=Path, required=True)
    p.add_argument('--root', default='root.exe'); p.add_argument('--jobs', type=int, default=2)
    p.add_argument('--runs'); p.add_argument('--run-list', type=Path)
    p.add_argument('--pt-windows', default='0.5:1,1:2,2:4')
    p.add_argument('--min-entries', type=int, default=100)
    p.add_argument('--phi-group', type=int, default=3); p.add_argument('--eta-group', type=int, default=4)
    p.add_argument('--require-phg', action='store_true', help='Require supplied PHGarfield velocities for both sides')
    p.add_argument('--require-conditions', action='store_true', help='Require complete side-separated current/base-kEff snapshots')
    p.add_argument('--require-rdca', action='store_true', help='Require >=8 accepted charge-paired phi bins in at least one population/window')
    p.add_argument('--fit-model', choices=['gaussian','gaussian-background'], default='gaussian')
    p.add_argument('--save-slices', action='store_true')
    p.add_argument('--prefix', default='h_TpcSeedsQA_')
    p.add_argument('--raw-time-shift', type=float, default=40.)
    p.add_argument('--conditions-csv', type=Path)
    p.add_argument('--phg-reference-csv', type=Path)
    p.add_argument('--phi-qpt', action='store_true'); p.add_argument('--plots', action='store_true')
    p.add_argument('--max-files', type=int, help='Quick test only: truncate each run; recorded in summary')
    p = sub.add_parser('collect', help='Combine per-run profiles and compare to a reference per production')
    p.add_argument('--batch', type=Path, required=True)
    p.add_argument('--reference', action='append', required=True, metavar='SYSTEM[/TAG]=RUN')
    p.add_argument('--out', type=Path, required=True)
    p.add_argument('--root', default='root.exe')
    p.add_argument('--allow-partial', action='store_true')
    p.add_argument('--include-maps', action='store_true'); p.add_argument('--plots', action='store_true')
    p.add_argument('--ratio-floor', type=float, default=.02)
    args = parser.parse_args()
    try:
        if getattr(args, 'max_files', None) is not None and args.max_files < 1:
            raise ValueError('--max-files must be positive')
        if hasattr(args, 'ratio_floor') and (not __import__('math').isfinite(args.ratio_floor) or args.ratio_floor < 0):
            raise ValueError('--ratio-floor must be finite and nonnegative')
        return globals()[args.action](args)
    except (ValueError, OSError, RuntimeError, KeyError) as error:
        print(f'ERROR: {error}', file=sys.stderr)
        return 1


if __name__ == '__main__':
    sys.exit(main())
