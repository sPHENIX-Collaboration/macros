#!/usr/bin/bash

#inputs=dst_triggered_event_run2auau-00054920.list, for example

if [[ $# -lt 1 ]]
then
  echo Error, need to specify input file
  exit -1
fi

inputs=$1
runno=${inputs##*/}
runno=${runno#*-}
runno=${runno%-*}
runno=${runno%.list}
runno=${runno%.root}
runno=${runno%.prdf}
runno=$((10#${runno}))  # convert to decimal number
echo processing run $runno

nevents=0
if [[ $# -ge 2 ]]
then
  nevents=$2
fi
echo Processing $nevents events

build=none          # use current environment
#dbtag=ProdA_2024
dbtag=""
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

ORIG_DIR=${PWD}

if [[ ! -z "${_CONDOR_SCRATCH_DIR}" ]]
then
  mkdir -p ${_CONDOR_SCRATCH_DIR}
  cd ${_CONDOR_SCRATCH_DIR}
  for f in /sphenix/user/chiu/sphenix_bbc/offline/macros/calibrations/mbd/*
  do
    ln -sf $f .
  done
fi

./run_mbdcal.sh --outbase "$outbase" \
              --outdir  "$outdir" \
              --logbase "$logbase" \
              --logdir  "$logdir" \
              --build    $build \
              --run      $runno \
              --nevents  $nevents \
              $inputs


if [[ ! -z "${_CONDOR_SCRATCH_DIR}" ]]
then
  ls -lR
fi

