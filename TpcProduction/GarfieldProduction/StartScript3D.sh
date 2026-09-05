#! /bin/bash

set -euo pipefail

# Electric field grid
emin=320
emax=480
ne=41

# Magnetic field grid
bmin=1.15
bmax=1.45
nb=16

# E-B angle grid
amin=0.0
amax=0.2
na=20

jobid="${1:?Usage: $0 jobid}"
normal_jobs=$(( ne * nb ))
total_jobs=$(( normal_jobs + ne ))

if (( jobid < 0 || jobid >= total_jobs )); then
    echo "ERROR: jobid=$jobid outside allowed range 0 to $((total_jobs - 1))"
    exit 1
fi

# Interpret the process ID to define the GasModel arguments.
if (( jobid < normal_jobs )); then
    ie=$(( jobid / nb ))
    ib=$(( jobid % nb ))

    enow=$(( emin + 4 * ie ))
    bnow=$(awk -v ib="$ib" \
        'BEGIN { printf "%.2f", 1.15 + 0.02 * ib }')

    efile=$(printf "E%03d_B%03d.gas" "$ie" "$ib")

    echo "jobid=$jobid ie=$ie ib=$ib enow=$enow bnow=$bnow"
else
    ie=$(( jobid - normal_jobs ))

    enow=$(( emin + 4 * ie ))
    bnow="0.00"

    efile=$(printf "ZERO_E%03d.gas" "$ie")

    echo "jobid=$jobid ie=$ie ZERO_B enow=$enow bnow=$bnow"
fi

# Determine the output location and launch GasModel.
script_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if [[ ! -d "${script_dir}/gasfiles" ]]; then
    echo "ERROR: ${script_dir}/gasfiles does not exist."
    echo "Create it or make it a symbolic link before launching jobs."
    exit 1
fi

output="${script_dir}/gasfiles/${efile}"

echo GasModel \
    "$enow" "$enow" 1 \
    "$bnow" "$bnow" 1 \
    "$amin" "$amax" "$na" \
    "$output"

GasModel \
    "$enow" "$enow" 1 \
    "$bnow" "$bnow" 1 \
    "$amin" "$amax" "$na" \
    "$output"
