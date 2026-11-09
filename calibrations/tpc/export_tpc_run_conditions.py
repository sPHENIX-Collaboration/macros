#!/usr/bin/env python3
"""Export side-separated current and base k_eff CDB snapshots without reading events."""
import argparse
import csv
import hashlib
import json
import math
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time
from tpc_qa_production import inventory, selected, atomic_json, digest
SOURCE_COMMIT='b241b6413a2de90cba3b6d415579db62bc81a153'


def main():
    p=argparse.ArgumentParser(description=__doc__)
    p.add_argument('--inventory',type=Path,required=True)
    p.add_argument('--out',type=Path,required=True)
    p.add_argument('--cdb-tag',required=True)
    p.add_argument('--root',default='root.exe')
    p.add_argument('--runs');p.add_argument('--run-list',type=Path)
    p.add_argument('--keff-fallback',choices=['none','phgarfield-defaults'],default='none',help='Only when the kEff payload URL is empty; always labeled derived, never verified applied')
    p.add_argument('--allow-missing',action='store_true',help='Return success with explicitly incomplete snapshots')
    args=p.parse_args()
    try:
        data=inventory(args.inventory);choice=selected(args)
        rows=[r for r in data['runs'] if not choice or r['run'] in choice]
        if not rows or choice-{r['run'] for r in rows}:raise ValueError('Requested runs absent from inventory')
        exe=shutil.which(args.root)
        if not exe:raise ValueError('ROOT executable not found')
        out=args.out.expanduser().resolve();out.mkdir(parents=True,exist_ok=True)
        macro=Path(__file__).with_name('ExportTPCRunConditions.C').resolve()
        header=macro.with_name('TPCRunConditionsSnapshot.h')
        settings={'cdb_tag':args.cdb_tag,'keff_fallback':args.keff_fallback,'macro_sha256':hashlib.sha256(macro.read_bytes()).hexdigest(),'header_sha256':hashlib.sha256(header.read_bytes()).hexdigest(),'definition_source_commit':SOURCE_COMMIT}
        summaries=[];series_rows=[];failed=[]
        for item in rows:
            group,run=item['group'],item['run']
            work=out/group/str(run)/digest(settings);work.mkdir(parents=True,exist_ok=True)
            summary=work/'conditions.csv';series=work/'current_series.csv'
            if not summary.exists():
                temp=work/'conditions.partial.csv';series_temp=work/'current_series.partial.csv'
                driver=work/'export_conditions.C'
                cpp=','.join([str(run),json.dumps(group),json.dumps(str(temp)),json.dumps(str(series_temp)),json.dumps(args.cdb_tag),'true' if args.keff_fallback=='phgarfield-defaults' else 'false'])
                driver.write_text(f'#include {json.dumps(str(macro))}\nvoid export_conditions() {{ try {{ ExportTPCRunConditions({cpp}); gSystem->Exit(0); }} catch (const std::exception &e) {{ std::cerr << e.what() << std::endl; gSystem->Exit(1); }} }}\n')
                with (work/'export.log').open('w') as log:
                    result=subprocess.run([exe,'-l','-b','-q',str(driver)],stdout=log,stderr=subprocess.STDOUT)
                if result.returncode or not temp.exists() or not series_temp.exists():
                    failed.append({'group':group,'run':run,'error':'ROOT export failed','log':str(work/'export.log')});continue
                with temp.open(newline='') as stream: table=list(csv.DictReader(stream))
                if len(table)!=2 or {r['side'] for r in table}!={'0','1'}:raise ValueError(f'Invalid conditions output: {temp}')
                stamp=time.strftime('%Y-%m-%dT%H:%M:%SZ',time.gmtime())
                for record in table:
                    record.update(snapshot_utc=stamp,definition_source_commit=SOURCE_COMMIT,export_offline_main=os.getenv('OFFLINE_MAIN',''),export_myinstall=os.getenv('MYINSTALL',''))
                with temp.open('w',newline='') as stream:
                    writer=csv.DictWriter(stream,fieldnames=list(table[0]));writer.writeheader();writer.writerows(table)
                series_temp.replace(series);temp.replace(summary);atomic_json(work/'settings.json',settings)
            with summary.open(newline='') as stream: table=list(csv.DictReader(stream))
            for record in table:
                record['qa_inventory_segments']=str(len(item['files']))
                record['qa_first_available_segment']=str(item['files'][0]['segment'])
            summaries.extend(table)
            with series.open(newline='') as stream:series_rows.extend(csv.DictReader(stream))
            incomplete=[r['conditions_status'] for r in table if r['conditions_status']!='complete_snapshot']
            if incomplete:failed.append({'group':group,'run':run,'error':';'.join(incomplete),'log':str(work/'export.log')})
            print(f'{group}/{run}: '+', '.join(f"side{r['side']}: Igem={r['gem_current_uA'] or 'missing'} uA; k_base={r['keff'] or 'missing'} ({r['keff_kind']})" for r in table),flush=True)
        for name,table in [('conditions.csv',summaries),('current_series.csv',series_rows)]:
            if table:
                temp=out/(name+'.partial')
                with temp.open('w',newline='') as stream:
                    writer=csv.DictWriter(stream,fieldnames=list(table[0]));writer.writeheader();writer.writerows(table)
                temp.replace(out/name)
        atomic_json(out/'export_status.json',{'settings':settings,'failed':failed,'side_rows':len(summaries),'meaning':'CDB snapshot; keff_applied unavailable; full-run currents do not claim selected-segment coverage'})
        print(f"Conditions: {out/'conditions.csv'}; incomplete/failed runs: {len(failed)}")
        return int(bool(failed) and not args.allow_missing)
    except (OSError,ValueError,KeyError) as error:
        print(f'ERROR: {error}',file=sys.stderr);return 1

if __name__=='__main__':sys.exit(main())
