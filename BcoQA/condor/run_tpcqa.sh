#!/usr/bin/bash

export USER="$(id -u -n)"
export LOGNAME=${USER}
# check if we run an sphenix account or legacy phenix
if [[ -d /sphenix/u/${USER} ]]
then
  export HOME=/sphenix/u/${USER}
else
  export HOME=/phenix/u/${USER}
fi

hostname

this_script=$BASH_SOURCE
this_script=`readlink -f $this_script`
this_dir=`dirname $this_script`
echo rsyncing from $this_dir
echo running: $this_script $*

anabuild=new

source /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/bin/sphenix_setup.sh -n $anabuild

#source /opt/sphenix/core/bin/setup_local.sh $HOME/workarea/sPHENIX/gitrepov5/installv1

if [[ ! -z "$_CONDOR_SCRATCH_DIR" && -d $_CONDOR_SCRATCH_DIR ]]
then
    cd $_CONDOR_SCRATCH_DIR
    rsync -av $this_dir/* .
else
    echo condor scratch NOT set
    exit -1
fi

# arguments 
# $1: input file
# $2: output csv file
# $3: output ntuple

echo 'here comes your environment'
printenv
echo arg1 \(input file\) : $1
echo arg2 \(output file\): $2
echo arg3 \(output dir\): $3

echo running root.exe -q -b Fun4All_TpcQA.C\(0,\"$1\"\)

root.exe -q -b  Fun4All_TpcQA.C\(0,\"$1\"\)

ls -l

cp bcodiff.root $3/$2

echo "script done"
