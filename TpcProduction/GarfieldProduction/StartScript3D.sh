#! /bin/bash

set -euo pipefail

emin=360
emax=440
ne=50

bmin=1.15
bmax=1.45
nb=50

amin=0.0
amax=0.2
na=50

jobid="$1"

ie=$(( jobid / nb ))
ib=$(( jobid % nb ))

if (( jobid < 0 || jobid >= ne * nb )); then
  echo "ERROR: jobid=$jobid outside allowed range 0 to $((ne * nb - 1))"
  exit 1
fi

enow=$(awk -v emin="$emin" \
             -v emax="$emax" \
             -v ne="$ne" \
             -v i="$ie" \
'BEGIN {
   if (ne == 1) print emin;
   else print emin + (emax - emin) * i / (ne - 1);
}')

bnow=$(awk -v bmin="$bmin" \
             -v bmax="$bmax" \
             -v nb="$nb" \
             -v i="$ib" \
'BEGIN {
   if (nb == 1) print bmin;
   else print bmin + (bmax - bmin) * i / (nb - 1);
}')

echo "jobid=$jobid ie=$ie ib=$ib enow=$enow bnow=$bnow"

script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [[ ! -d "${script_dir}/gasfiles" ]]; then
  echo "ERROR: ${script_dir}/gasfiles does not exist."
  echo "Create it or make it a symbolic link before launching jobs."
  exit 1
fi

output="${script_dir}/gasfiles/PART_${jobid}.gas"

echo "$output"

echo GasModel "$enow" "$enow" 1 "$bnow" "$bnow" 1 "$amin" "$amax" "$na" "$output"
GasModel "$enow" "$enow" 1 "$bnow" "$bnow" 1 "$amin" "$amax" "$na" "$output"

echo "all done"
