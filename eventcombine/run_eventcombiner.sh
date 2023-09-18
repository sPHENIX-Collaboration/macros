#!/usr/bin/bash

export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${USER}

hostname

this_script=$BASH_SOURCE
this_script=`readlink -f $this_script`
this_dir=`dirname $this_script`
echo rsyncing from $this_dir
echo running: $this_script $*

source /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/bin/sphenix_setup.sh -n 
#source /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/bin/setup_local.sh /sphenix/u/sphnxpro/chp/eventcombine/install

if [[ ! -z "$_CONDOR_SCRATCH_DIR" && -d $_CONDOR_SCRATCH_DIR ]]
then
    cd $_CONDOR_SCRATCH_DIR
    rsync -av $this_dir/* .
else
   echo condor scratch NOT set
   exit -1
fi

# arguments 
# $1: run number
# $2: hepmc input file
# $3: output file
# $4: no events to skip
# $5: output dir
# $6: runnumber
# $7: sequence

echo 'here comes your environment'
printenv

sh makelist.sh $1

root.exe -q -b Fun4All_Combiner.C\(0\)

echo "script done"
