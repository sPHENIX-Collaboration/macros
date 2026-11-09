#!/usr/bin/env python3
"""Explain empty rDCA profiles from saved slice-fit diagnostics (no ROOT required)."""
import argparse
from collections import Counter
import csv
import json
from pathlib import Path
import sys

STATUS={0:'accepted',1:'too few total counts',2:'invalid/short fit window or too few window counts',3:'ROOT minimizer/result failed',4:'poor covariance',5:'invalid/boundary mean/width/error',6:'final fit quality or insufficient ndf (old versions also tested the seed pass)'}
def main():
 p=argparse.ArgumentParser(description=__doc__)
 p.add_argument('--batch',type=Path,required=True);p.add_argument('--runs',default='79513');p.add_argument('--window',default='pt_0p5_1')
 a=p.parse_args()
 wanted={int(r) for r in a.runs.replace(',',' ').split()}
 data=json.loads(a.batch.read_text());found=0
 records = data['complete'] + [r for r in data.get('failed', []) if r.get('directory')]
 for record in records:
  if record['run'] not in wanted:continue
  found+=1;folder=Path(record['directory'])/'analysis'/a.window
  print(f"\n{record['group']}/{record['run']} — {folder}")
  summary=folder/'analysis_summary.tsv'
  if summary.exists():print(summary.read_text().strip())
  if not (folder/'slice_fits.csv').exists():
   print('No slice-fit table; inspect',Path(record['directory'])/'analysis.log')
   continue
  with (folder/'slice_fits.csv').open(newline='') as stream:
   rows=[r for r in csv.DictReader(stream) if '_phi_' in r['observable'] and 'phiBin' not in r['observable']]
  for name in sorted({r['observable'] for r in rows}):
   selected=[r for r in rows if r['observable']==name]
   statuses=Counter(int(r['status']) for r in selected)
   print(name, '; '.join(f'{n} x {STATUS.get(s,str(s))}' for s,n in sorted(statuses.items())))
   print('  ROOT statuses:',dict(Counter(r['root_status'] for r in selected)), 'covariance:',dict(Counter(r['covariance'] for r in selected)))
   bad=next((r for r in selected if int(r['status'])!=0),None)
   if bad:print('  example rejected slice:',{k:bad.get(k,'not recorded') for k in ('bin','in_range_counts','status','mean','sigma','chi2_ndf','ndf','fit_low','fit_high','fit_model')})
 if not found:raise ValueError('Requested run has no recorded analysis directory in this batch record')
 return 0
if __name__=='__main__':
 try:sys.exit(main())
 except (OSError,ValueError,KeyError) as error:print(f'ERROR: {error}',file=sys.stderr);sys.exit(1)
