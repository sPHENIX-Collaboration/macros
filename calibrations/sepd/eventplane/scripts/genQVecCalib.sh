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
dst_tag=${6}
submitDir=${7}

# extract runnumber from file name
run=$(echo "$input" | grep -oP 'output/\K\d+(?=/tree)')
if [[ -z "$run" ]]; then
    echo "Failed to parse run number from input: $input" >&2
    exit 1
fi

QAhist_file=$(basename "$QAhist")
QVecCalibHist_file=$(basename "$QVecCalibHist")

if [[ -n "$_CONDOR_SCRATCH_DIR" && -d "$_CONDOR_SCRATCH_DIR" ]]
then
    cd "$_CONDOR_SCRATCH_DIR" || { echo "Failed to cd to $_CONDOR_SCRATCH_DIR" >&2; exit 1; }

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

root -b -l -q "$f4a_macro(\"input.list\", \"$QAhist_file\", \"$QVecCalibHist_file\", $pass, 0, \"output/hist/QVecCalib-$run.root\", \"$dst_tag\", \"output/CDB/$run\")"

echo "All Done and Transferring Files Back"
cp -rv output/* "$submitDir"

echo "Finished"
