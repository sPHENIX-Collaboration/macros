#!/usr/bin/env bash
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

exe=${1}
input=${2}
submitDir=${3}

if [[ ! -z "$_CONDOR_SCRATCH_DIR" && -d $_CONDOR_SCRATCH_DIR ]]
 then
   cd $_CONDOR_SCRATCH_DIR
 else
   echo "condor scratch NOT set"
   exit -1
fi

# print the environment - needed for debugging
printenv

$exe $input

echo "All Done and Transferring Files Back"
cp -rv output/* $submitDir

echo "Finished"
