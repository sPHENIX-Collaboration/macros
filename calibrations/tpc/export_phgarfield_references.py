#!/usr/bin/env python3
"""SDCC-only: evaluate PHGarfield directly from our ROOT macro for each run, without reading events."""
import argparse
import csv
import hashlib
import json
import math
from pathlib import Path
import shutil
import subprocess
import sys
from tpc_qa_production import inventory, atomic_json, selected, digest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--inventory', required=True, type=Path)
    parser.add_argument('--out', required=True, type=Path, help='Working directory; identical completed exports are reused')
    parser.add_argument('--cdb-tag', required=True, help='Match the conditions tag intended for this comparison')
    parser.add_argument('--reference-field', required=True, type=float, help='PHGarfield reference CM field in V/cm; choose explicitly, e.g. 377 or 375')
    parser.add_argument('--reference-pressure', type=float, default=30.0968906)
    parser.add_argument('--reference-temperature-K', dest='reference_temperature_K', type=float, default=299.98646566)
    parser.add_argument('--t0-bins', type=float, default=8., help='Configured model t0; NOT a measurement')
    parser.add_argument('--time-bin-ns', type=float, default=56.8)
    parser.add_argument('--step-ns', type=float, default=5.)
    parser.add_argument('--runs'); parser.add_argument('--run-list', type=Path)
    parser.add_argument('--root', default='root.exe')
    args = parser.parse_args()
    try:
        for key in ('reference_field', 'reference_pressure', 'reference_temperature_K', 'time_bin_ns', 'step_ns'):
            if not math.isfinite(getattr(args,key)) or getattr(args,key)<=0:
                raise ValueError(f'{key} must be positive and finite')
        if not math.isfinite(args.t0_bins):
            raise ValueError('t0-bins must be finite')
        executable = shutil.which(args.root)
        if not executable:
            raise ValueError('ROOT executable not found')
        data=inventory(args.inventory)
        choice=selected(args)
        rows=[r for r in data['runs'] if not choice or r['run'] in choice]
        if not rows or choice-{r['run'] for r in rows}:
            raise ValueError('Requested runs absent from inventory')
        out=args.out.expanduser().resolve(); out.mkdir(parents=True,exist_ok=True)
        macro=Path(__file__).with_name('ExportTPCPHGarfieldReference.C').resolve()
        settings={k:v for k,v in vars(args).items() if k not in ('inventory','out','runs','run_list')}
        settings['macro_sha256']=hashlib.sha256(macro.read_bytes()).hexdigest()
        settings['conditions_helper_sha256']=hashlib.sha256(macro.with_name('TPCRunAverageConditions.h').read_bytes()).hexdigest()
        settings['snapshot_helper_sha256']=hashlib.sha256(macro.with_name('TPCRunConditionsSnapshot.h').read_bytes()).hexdigest()
        # Installation and CDB content can change even at the same path. Use a new --out
        # after changing installed PHGarfield libraries or CDB payloads; defaults are recorded below.
        all_rows,failed=[],[]
        for row in rows:
            group,run=row['group'],row['run']
            work=out/group/str(run)/digest(settings);work.mkdir(parents=True,exist_ok=True)
            csv_path=work/'reference.csv'
            if not csv_path.exists():
                driver=work/'export_reference.C'
                cpp=','.join(map(json.dumps,(run,group,str(csv_path.with_suffix('.partial.csv')),args.cdb_tag,args.reference_field,args.reference_pressure,args.reference_temperature_K,args.t0_bins,args.time_bin_ns,args.step_ns)))
                driver.write_text(f'#include {json.dumps(str(macro))}\nvoid export_reference() {{ try {{ ExportTPCPHGarfieldReference({cpp}); gSystem->Exit(0); }} catch (const std::exception &e) {{ std::cerr << e.what() << std::endl; gSystem->Exit(1); }} }}\n')
                with (work/'export.log').open('w') as log:
                    result=subprocess.run([executable,'-l','-b','-q',str(driver)],stdout=log,stderr=subprocess.STDOUT)
                temp=csv_path.with_suffix('.partial.csv')
                if result.returncode or not temp.exists():
                    failed.append({'group':group,'run':run,'log':str(work/'export.log')})
                    print(f'FAILED {group}/{run}: {work / "export.log"}',file=sys.stderr,flush=True)
                    continue
                temp.replace(csv_path)
                atomic_json(work/'settings.json',settings)
            with csv_path.open(newline='') as stream:
                table=list(csv.DictReader(stream))
            if len(table)!=2 or {r['side'] for r in table}!={'0','1'}:
                raise ValueError(f'Invalid reference export: {csv_path}')
            all_rows.extend(table)
            print(f'{group}/{run}: PHGarfield reference available',flush=True)
        if all_rows:
            temp=out/'phgarfield_references.partial.csv'
            with temp.open('w',newline='') as stream:
                writer=csv.DictWriter(stream,fieldnames=list(all_rows[0]));writer.writeheader();writer.writerows(all_rows)
            temp.replace(out/'phgarfield_references.csv')
        atomic_json(out/'export_status.json',{'settings':settings,'failed':failed,'complete_side_rows':len(all_rows)})
        if all_rows:
            print(f"Reference table written: {out / 'phgarfield_references.csv'}; failed runs: {len(failed)}")
        else:
            print(f"No valid reference table produced in this invocation; failed runs: {len(failed)}. Inspect the export.log path(s) above.")
        return int(bool(failed))
    except (ValueError,OSError,KeyError) as error:
        print(f'ERROR: {error}',file=sys.stderr);return 1


if __name__=='__main__':
    sys.exit(main())
