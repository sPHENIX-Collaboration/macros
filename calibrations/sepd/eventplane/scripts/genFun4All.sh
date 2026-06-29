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
    getinputfiles.pl --filelist "$input"
    ls -lah
    ls DST*.root > test.list
else
    echo "condor scratch NOT set" >&2
    exit 1
fi

# print the environment - needed for debugging
printenv

mkdir -p "$run/hist" "$run/tree"

root -b -l -q "$f4a_macro($nEvents, \"test.list\", \"$run/hist/$output\", \"$run/tree/$output_tree\", \"$dbtag\")"

echo "All Done and Transferring Files Back"
cp -rv "$run" "$submitDir"

echo "Finished"
