#!/usr/bin/env bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

QVecCalib_bin=${1}
input=${2}
QAhist=${3}
QVecCalibHist=${4}
QVecAna=${5}
submitDir=${6}

# extract runnumber from file name
run=$(echo "$input" | grep -oP 'output/\K\d+(?=/tree)')
input_file=$(basename "$input")
QAhist_file=$(basename "$QAhist")
QVecCalibHist_file=$(basename "$QVecCalibHist")

if [[ ! -z "$_CONDOR_SCRATCH_DIR" && -d $_CONDOR_SCRATCH_DIR ]]
then
    cd $_CONDOR_SCRATCH_DIR
    cp -v "$input" .
    cp -v "$QAhist" .
    test -e "$QVecCalibHist" && cp -v "$QVecCalibHist" .
    ls -lah
else
    echo "condor scratch NOT set"
    exit -1
fi

# print the environment - needed for debugging
printenv

mkdir -p "$run"

"$QVecCalib_bin" "$input_file" "$QAhist_file" "$QVecCalibHist_file" "$QVecAna" 0 "$run"

echo "All Done and Transferring Files Back"
cp -rv "$run" "$submitDir"

echo "Finished"
