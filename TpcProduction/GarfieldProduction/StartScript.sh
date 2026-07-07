#! /bin/bash

set -euo pipefail

emin=400
emax=400
ne=1

bmin=1.15
bmax=1.45
nb=50

amin=0.0
amax=0.2
na=50

jobid="$1"

bnow=$(awk -v bmin="$bmin" \
             -v bmax="$bmax" \
             -v nb="$nb" \
             -v i="$1" \
'BEGIN {
   print bmin + (bmax - bmin) * i / nb
}')

echo $bnow

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [[ ! -d "${script_dir}/gasfiles" ]]; then
  echo "ERROR: ${script_dir}/gasfiles does not exist."
  echo "Create it or make it a symbolic link before launching jobs."
  exit 1
fi

output="${script_dir}/gasfiles/PART_${jobid}.gas"
echo $output

echo GasModel "$emin" "$emax" "$ne" "$bnow" "$bnow" 1 "$amin" "$amax" "$na" "$output"
GasModel "$emin" "$emax" "$ne" "$bnow" "$bnow" 1 "$amin" "$amax" "$na" "$output"

echo "all done"
