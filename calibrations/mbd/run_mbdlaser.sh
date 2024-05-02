#!/usr/bin/env bash
#
# run the mbd calibration passes
# usage:
# runmbd_calpass.sh <prdf> <calpass> <nevts>
#

prdf=$1

nevts=0
if [[ $# -gt 1 ]] 
then
  nevts=$2
fi

# get run number from prdf file
run=${prdf##*/}
run=${run%-*}
run=${run#*-}
run=${run#000}

# make calibration directory and fill it
caldir=results/${run}
echo mkdir -p ${caldir}
mkdir -p ${caldir}

# link pass0 calibs
export PASS0DIR=/sphenix/user/chiu/sphenix_bbc/CDB/PASS0/
ln -sf ${PASS0DIR}/mbd_shape.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_sherr.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_timecorr.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_slewcorr.calib ${caldir}/

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

# get the samp_max
pass=1
smax_nevts=20000
echo root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${pass},${smax_nevts}\)
root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${pass},${smax_nevts}\)

# now process the laser events and write out a DST
pass=2
echo root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${pass},${nevts}\)
root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${pass},${nevts}\)

