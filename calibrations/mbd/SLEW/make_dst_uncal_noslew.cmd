#!/usr/bin/env bash
#
# usage:
#  calmbd.sh <prdf or dst or list> <nevts> <nskip>

infile=$1
echo "input file " $infile

nevt=0
nskip=0
if [[ $# -gt 1 ]] 
then
  nevts=$2
  echo $nevts
fi
if [[ $# -gt 2 ]] 
then
  nskip=$3
  echo skipping $nskip
fi

# get run number
run=""
if [[ "$infile" =~ prdf$ || "$infile" =~ root$ ]]
then
  run=${infile##*/}
  run=${run%-*}
  run=${run#*-}
  run=$((10#$run))
elif [[ "$infile" =~ list$ ]]
then
  run=${infile##*/}
  run=${run%.list}
  nevt=2000000
fi

dir=${run}/${infile##*/}
dir=${dir%.*}
mkdir -p $dir
cd $dir
ln -sf ../../run_mbdcalpass.sh .
ln -sf ../../Fun4All_MBD_CalPass.C .

echo processing run $run in directory $dir

# Set PASS0DIR to bypass default in CDB calibrations
#export PASS0DIR=/sphenix/user/chiu/sphenix_bbc/CDB/PASS0/
#echo "Setting PASS0DIR to $PASS0DIR"

# do the pass1 calibrations (only 20K events)
echo run_mbdcalpass.sh $infile 1
run_mbdcalpass.sh $infile 1

# process waveforms, output a DST_UNCALMBD
# here time is in ns and charge in adc
echo run_mbdcalpass.sh $infile 2 $nevts $nskip
run_mbdcalpass.sh $infile 2 $nevts $nskip

