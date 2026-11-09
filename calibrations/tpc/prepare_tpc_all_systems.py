#!/usr/bin/env python3
"""Locate the chosen production tag for pp/OO/AuAu and prepare an all-system QA inventory."""
import argparse
import json
from pathlib import Path
import subprocess
import sys
from tpc_qa_production import atomic_json


def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--base',action='append',help='Production parent; repeat for multiple disks')
 p.add_argument('--tag',default='ana581_2026p005_v001')
 p.add_argument('--system-tag',action='append',default=[],metavar='SYSTEM=TAG',help='Override tag for one system')
 p.add_argument('--source',action='append',default=[],metavar='SYSTEM=PATH',help='Explicit source overrides automatic location')
 p.add_argument('--systems',default='run3pp,run3oo,run3auau')
 p.add_argument('--fast',action='store_true',help='Use supplied per-system run lists and bounded filename probes')
 p.add_argument('--run-list',action='append',default=[],metavar='SYSTEM=FILE')
 p.add_argument('--max-files',type=int,default=10)
 p.add_argument('--probe-limit',type=int,default=200)
 p.add_argument('--segment-start',type=int,default=0)
 p.add_argument('--out',type=Path,required=True)
 a=p.parse_args()
 systems=a.systems.split(',')
 if len(set(systems))!=len(systems) or not set(systems)<={'run3pp','run3oo','run3auau'}:raise ValueError('Invalid/duplicate systems')
 tags={s:a.tag for s in systems};tags.update(dict(x.split('=',1) for x in a.system_tag))
 explicit=dict(x.split('=',1) for x in a.source)
 bases=a.base or ['/sphenix/data/data02/sphnxpro/production']
 sources={};problems=[]
 for system in systems:
  if system in explicit:
   candidate=Path(explicit[system]).expanduser()
   if candidate.name!='DST_TRKR_SEED':candidate=candidate/'DST_TRKR_SEED'
   options=[candidate] if candidate.is_dir() else []
  else:options=[Path(b)/system/'physics'/tags[system]/'DST_TRKR_SEED' for b in bases if (Path(b)/system/'physics'/tags[system]/'DST_TRKR_SEED').is_dir()]
  options=sorted({str(x.resolve()) for x in options})
  if len(options)!=1:
   problems.append(f'{system}: expected one source, found {len(options)}: {options}. Use --source {system}=PATH or --system-tag {system}=TAG.')
  else:sources[system]=options[0]
 if problems:
  for problem in problems:print(problem,file=sys.stderr)
  print('Available alternatives under the supplied disks:',file=sys.stderr)
  for b in bases:
   for system in systems:
    for path in sorted(Path(b).glob(system+'/physics/*/DST_TRKR_SEED')):print(path,file=sys.stderr)
  return 1
 out=a.out.expanduser().resolve()
 command=[sys.executable,str(Path(__file__).with_name('tpc_qa_production.py')),'discover','--out',str(out)]
 for system,path in sources.items():command+=['--source',f'{system}={path}']
 if a.fast:
  run_lists=dict(value.split('=',1) for value in a.run_list)
  if set(run_lists)!=set(systems):raise ValueError('Fast preparation requires --run-list SYSTEM=FILE for every requested system')
  command+=['--fast','--max-files',str(a.max_files),'--probe-limit',str(a.probe_limit),'--segment-start',str(a.segment_start)]
  for system,filename in run_lists.items():command+=['--system-run-list',f'{system}={filename}']
 elif a.run_list:
  raise ValueError('Use --fast with the per-system run-list options')
 result=subprocess.run(command)
 if result.returncode:return result.returncode
 value=json.loads((out/'inventory.json').read_text())
 groups={}
 for row in value['runs']:groups.setdefault(row['group'],[]).append(row['run'])
 plan={'sources':sources,'groups':{g:{'runs':len(v),'available_runs':sorted(v),'reference_run':None} for g,v in groups.items()},'segments_per_run':a.max_files,'note':'Choose reference runs after inspecting availability. Production directory labels do not independently certify collision species.'}
 atomic_json(out/'next_pass_plan.json',plan)
 print('Plan:',out/'next_pass_plan.json')
 print('Use the inventory with export_tpc_run_conditions.py, export_phgarfield_references.py, then process --max-files 10 --fit-model gaussian-background.')
 return 0
if __name__=='__main__':
 try:sys.exit(main())
 except (OSError,ValueError,KeyError) as error:print(f'ERROR: {error}',file=sys.stderr);sys.exit(1)
