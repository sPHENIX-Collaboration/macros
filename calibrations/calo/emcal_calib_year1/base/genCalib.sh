#!/usr/bin/env bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

f4a_bin=${1}
nEvents=${2}
input=${3}
iter=${4}
calib=${5}
calib_field=${6}
submitDir=${7}

if [[ ! -z "$_CONDOR_SCRATCH_DIR" && -d $_CONDOR_SCRATCH_DIR ]]
 then
   cd $_CONDOR_SCRATCH_DIR
   cp -v $calib .
   getinputfiles.pl $input
   echo $input > test.list
 else
   echo "condor scratch NOT set"
   exit -1
fi

calib=$(basename $calib)

# print the environment - needed for debugging
printenv

# root -b -l -q "$f4a_macro($nEvents, \"test.list\", $iter, \"$calib\", \"$calib_field\")"
$f4a_bin "$nEvents" "test.list" "$iter" "$calib" "$calib_field"

echo "All Done and Transferring Files Back"
cp -v OUT*.root $submitDir

echo "Finished"
