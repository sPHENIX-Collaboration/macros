#!/usr/bin/bash

#inputs=dst_triggered_event_run2auau-00054920.list, for example

if [[ $# -lt 1 ]]
then
  echo Error, need to specify input file
  exit -1
fi

#generate the run_xxx_yyy string
get_rundirname() {
  runtemp=$1
  temp=$(( ${runtemp}/100 * 100 ))
  low=$(printf "%08d" $temp)
  temp=$(( ${temp}+100 ))
  high=$(printf "%08d" $temp)
  echo run_${low}_${high}
}

inputs=$1
runno=${inputs##*/}
runno=${runno#*-}
runno=${runno%-*}
runno=${runno%.list}
runno=${runno%.root}
runno=${runno%.prdf}
runno=$((10#${runno}))  # convert to decimal number
echo processing run $runno

nevents=3000000
if [[ $# -ge 2 ]]
then
  nevents=$2
fi
echo Processing $nevents eventsxxx

build=none          # use current environment
#dbtag=newcdbtag
dbtag=""

if [[ $USER == "sphnxpro" ]]
then
  pass0dir=/sphenix/user/chiu/sphenix_bbc/CDB/PASS0
  outbase=DST_MBD_CALIBRATION_run3auau
  outdir=/sphenix/lustre01/sphnxpro/physics/mbdcalib/new_2025/$(get_rundirname ${runno})/
  logbase=DST_MBD_CALIBRATION_run3auau
  logdir=/sphenix/data/data02/sphnxpro/mbdcalib/new_2025/$(get_rundirname ${runno})/
  build=new
  dbtag=newcdbtag
else
  pass0dir=/sphenix/user/chiu/sphenix_bbc/CDB/PASS0
  outbase=DST_MBD_CALIBRATION_run3auau
  outdir=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/TEST
  logbase=DST_MBD_CALIBRATION_run3auau
  logdir=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/TEST/log

  if [[ $runno -le 53880 ]]
  then
    # 2024 Run2pp
    outbase=DST_MBD_CALIBRATION_run2pp
    outdir=/sphenix/user/chiu/sphenix_bbc/run2024/CALIBPRODUCTION/TEST
    logbase=DST_MBD_CALIBRATION_run2pp
    logdir=/sphenix/user/chiu/sphenix_bbc/run2024/CALIBPRODUCTION/TEST/log
  elif [[ $runno -le 54962 ]]
  then
    # 2024 Run2auau
    outbase=DST_MBD_CALIBRATION_run2auau
    outdir=/sphenix/user/chiu/sphenix_bbc/run2024/CALIBPRODUCTION/TEST
    logbase=DST_MBD_CALIBRATION_run2auau
    logdir=/sphenix/user/chiu/sphenix_bbc/run2024/CALIBPRODUCTION/TEST/log
  fi
fi

ORIG_DIR=${PWD}
#_CONDOR_SCRATCH_DIR=AAA
#echo $_CONDOR_SCRATCH_DIR

if [[ ! -z "${_CONDOR_SCRATCH_DIR}" ]]
then
  mkdir -p ${_CONDOR_SCRATCH_DIR}
  cp ${inputs} *.sh *.h *.C ${_CONDOR_SCRATCH_DIR}/
  cd ${_CONDOR_SCRATCH_DIR}
  pwd
  ls -lR
  #for f in /sphenix/user/chiu/sphenix_bbc/offline/macros/calibrations/mbd/*
  #do
  #  ln -sf $f .
  #done
fi

./run_mbdcal.sh --outbase "$outbase" \
  --outdir  "$outdir" \
  --logbase "$logbase" \
  --logdir  "$logdir" \
  --build    $build \
  --run      $runno \
  --dbtag    "$dbtag" \
  --nevents  $nevents \
  $inputs


if [[ ! -z "${_CONDOR_SCRATCH_DIR}" ]]
then
  ls -lR
fi

