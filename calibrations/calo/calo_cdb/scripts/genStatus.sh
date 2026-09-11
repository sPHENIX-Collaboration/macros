#!/usr/bin/env bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

exe=${1}
input=${2}
submitDir=${3}

if [[ -n "$_CONDOR_SCRATCH_DIR" && -d "$_CONDOR_SCRATCH_DIR" ]]
then
    cd "$_CONDOR_SCRATCH_DIR" || { echo "Failed to cd to $_CONDOR_SCRATCH_DIR" >&2; exit 1; }
    echo "Reading inputs from: $input"

    ls -lah
else
    echo "condor scratch NOT set" >&2
    exit 1
fi

# print the environment - needed for debugging
printenv

$exe "$input"

root_exit=$?
if [ $root_exit -ne 0 ]; then
    echo "Error: program failed with exit code $root_exit at $(date) on $(hostname)! Aborting transfer." >&2
    mkdir -p "$submitDir/failures"
    echo "ROOT failure (exit code $root_exit) for $input on $(hostname) at $(date)" >> "$submitDir/failures/failure-log.txt"
    exit $root_exit
fi

# Define maximum retries and a counter
max_retries=5
count=0
success=0

while [ $count -lt $max_retries ]; do
    if cp -rv output/* "$submitDir"; then
        success=1
        break
    else
        count=$((count + 1))
        echo "cp failed (likely GPFS lag). Retrying ($count/$max_retries) in 15 seconds..." >&2
        sleep 15
    fi
done

if [ $success -eq 0 ]; then
    echo "Error: cp failed permanently after $max_retries attempts at $(date)." >&2
    mkdir -p "$submitDir/failures"
    echo "CP transfer failure for $input on $(hostname) at $(date)" >> "$submitDir/failures/failure-log.txt"
    exit 1
fi

echo "Finished successfully at $(date)"
