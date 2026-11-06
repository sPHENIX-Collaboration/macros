#!/usr/bin/env bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

f4a_macro=${1}
input=${2}
QAhist=${3}
QVecCalibHist=${4}
pass=${5}
charge_threshold=${6}
noise_threshold=${7}
dst_tag=${8}
submitDir=${9}

# extract runnumber from file name
run=$(echo "$input" | grep -oP '(?<=/)\d+(?=/tree)')
if [[ -z "$run" ]]; then
    run=$(basename "$(dirname "$input")")
fi
if [[ -z "$run" || ! "$run" =~ ^[0-9]+$ ]]; then
    echo "Failed to parse run number from input: $input" >&2
    exit 1
fi

QAhist_file=$(basename "$QAhist")
QVecCalibHist_file=$(basename "$QVecCalibHist")

if [[ -n "$_CONDOR_SCRATCH_DIR" && -d "$_CONDOR_SCRATCH_DIR" ]]
then
    cd "$_CONDOR_SCRATCH_DIR" || { echo "Failed to cd to $_CONDOR_SCRATCH_DIR" >&2; exit 1; }

    echo "Reading inputs from: $input"

    cp -rv "$input" input
    readlink -f input/* > input.list

    cp -v "$QAhist" .
    test -e "$QVecCalibHist" && cp -v "$QVecCalibHist" .
    ls -lah
else
    echo "condor scratch NOT set" >&2
    exit 1
fi

# print the environment - needed for debugging
printenv

mkdir -p "output/hist"

if [ "$pass" -eq 2 ]; then
    mkdir -p "output/CDB/$run"
fi

echo "Starting ROOT macro at $(date) on $(hostname)"
root -b -l -q "$f4a_macro(\"input.list\", \"$QAhist_file\", \"$QVecCalibHist_file\", $pass, 0, \"output/hist/QVecCalib-$run.root\", \"$dst_tag\", $charge_threshold, $noise_threshold, \"output/CDB/$run\")"

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
