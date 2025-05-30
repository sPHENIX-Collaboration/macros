#!/usr/bin/bash

#inputs=dst_triggered_event_run2auau-00054920.list

if [[ $# -lt 1 ]]
then
  echo Error, need to specify input file
  exit -1
fi

inputs=$1
runno=${inputs##*-}
runno=${runno%.list}
runno=$((10#${runno}))  # convert to decimal number

nevents=0
if [[ $# -ge 2 ]]
then
  nevents=$2
fi
echo Processing $nevents events

build=none          # use current environment
dbtag=ProdA_2024
outbase=DST_MBD_CALIBRATION_run3auau
outdir=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/TEST
logbase=DST_MBD_CALIBRATION_run3auau
logdir=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/TEST/log

run_mbdcal.sh --outbase "$outbase" \
              --outdir  "$outdir" \
              --logbase "$logbase" \
              --logdir  "$logdir" \
              --build    $build \
              --dbtag    $dbtag \
              --run      $runno \
              --nevents  $nevents \
              $inputs


