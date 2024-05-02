#!/usr/bin/env bash
#
# run the mbd calibration passes
# usage:
# runmbd_calpass.sh <prdf> <calpass> <nevts>
#

prdf=$1
calpass=$2

nevts=0
if [[ $# -gt 2 ]] 
then
  nevts=$3
elif [[ $2 -eq 1 ]]
then
  # for calpass 1, we set nevts to 20K by default
  nevts=20000
fi

# get run number from prdf file
run=${prdf##*/}
run=${run%-*}
run=${run#*-}
run=${run#000}

# make calibration directory and fill it
caldir=${PWD}/results/${run}
echo mkdir -p ${caldir}
mkdir -p ${caldir}

# link pass0 calibs
# use existing PASS0DIR, otherwise default to
if [ -z "$PASS0DIR" ]
then
  export PASS0DIR=/sphenix/user/chiu/sphenix_bbc/CDB/PASS0/
  echo "Setting PASS0DIR to $PASS0DIR"
fi

ln -sf ${PASS0DIR}/mbd_shape.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_sherr.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_timecorr.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_slewcorr.calib ${caldir}/

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

echo root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${calpass},${nevts}\)
root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${calpass},${nevts}\)


