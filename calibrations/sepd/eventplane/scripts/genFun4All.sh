#!/usr/bin/env bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

f4a_macro=${1}
input=${2}
output=${3}
output_tree=${4}
nEvents=${5}
dbtag=${6}
submitDir=${7}

# extract runnumber from file name
file=$(basename "$input")
IFS='-' read -r p1 p2 p3 <<< "$file"
run=$(echo "$p2" | sed 's/^0*//') # Remove leading zeros using sed
if [[ -z "$run" || ! "$run" =~ ^[0-9]+$ ]]; then
    echo "Failed to parse run number from input: $input" >&2
    exit 1
fi

if [[ -n "$_CONDOR_SCRATCH_DIR" && -d "$_CONDOR_SCRATCH_DIR" ]]
then
    cd "$_CONDOR_SCRATCH_DIR" || { echo "Failed to cd to $_CONDOR_SCRATCH_DIR" >&2; exit 1; }

    echo "Reading inputs from: $input"

    cut -d ',' -f 1 "$input" > dst_calofit.list
    cut -s -d ',' -f 2 "$input" > dst_zdc.list
    cut -s -d ',' -f 3 "$input" > dst_sepd.list

    for list in dst_calofit.list dst_zdc.list dst_sepd.list; do
        if [[ ! -s "$list" ]]; then
            echo "Error: $list is empty; check the columns of $input" >&2
            mkdir -p "$submitDir/failures"
            echo "empty input list ($list) for $input on $(hostname) at $(date)" >> "$submitDir/failures/failure-log.txt"
            exit 1
        fi

        getinputfiles.pl --verbose --filelist "$list" || {
            echo "Error: getinputfiles.pl failed for $list at $(date) on $(hostname)" >&2
            mkdir -p "$submitDir/failures"
            echo "getinputfiles failure ($list) for $input on $(hostname) at $(date)" >> "$submitDir/failures/failure-log.txt"
            exit 1
        }
    done

    ls -lah
else
    echo "condor scratch NOT set" >&2
    exit 1
fi

# print the environment - needed for debugging
printenv

mkdir -p "$run/hist" "$run/tree"

echo "Starting ROOT macro at $(date) on $(hostname)"
root -b -l -q "$f4a_macro($nEvents, \"dst_calofit.list\", \"dst_zdc.list\", \"dst_sepd.list\", \"$run/hist/$output\", \"$run/tree/$output_tree\", \"$dbtag\")"

root_exit=$?
if [ $root_exit -ne 0 ]; then
    echo "Error: ROOT macro failed with exit code $root_exit at $(date) on $(hostname)! Aborting transfer." >&2
    mkdir -p "$submitDir/failures"
    echo "ROOT failure (exit code $root_exit) for $input on $(hostname) at $(date)" >> "$submitDir/failures/failure-log.txt"
    exit $root_exit
fi

echo "All Done and Transferring Files Back at $(date)"

# Define maximum retries and a counter
max_retries=5
count=0
success=0

while [ $count -lt $max_retries ]; do
    if cp -rv "$run" "$submitDir"; then
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
