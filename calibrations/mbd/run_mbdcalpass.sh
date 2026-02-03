#!/usr/bin/env bash
#
# run the mbd calibration passes
# usage:
# runmbd_calpass.sh <input> <calpass> <nevts> <nskip>
#
# <input> can be prdf, dst_trig, or dst_fitted, or list files of these
#

echo PWD=${PWD}
echo LD_LIBRARY_PATH=${LD_LIBRARY_PATH}
echo HOST=`hostname`
ORIG_DIR=${PWD}

ulimit -c 0     # no core files

prdf=$1
calpass=$2

nevts=0
nskip=0
if [[ $# -gt 2 ]]
then
  nevts=$3
fi
if [[ $# -gt 3 ]]
then
  nskip=$4
fi

if [[ $2 -eq 1 ]]
then
  # for calpass 1, we set nevts to 20K by default
  nevts=20000
fi


# get run number from prdf file
run=${prdf##*/}
run=${run%-*}
run=${run#*-}
run=$((10#$run))  # convert string to decimal

# if PASS0DIR set, use that, else use default cdbtag
if [ -z "${PASS0DIR}" ]
then
  # set cdb tag by run
  cdbtag="newcdbtag"
  #export PASS0DIR=/sphenix/user/chiu/sphenix_bbc/CDB/2024_PASS0_AUAU/
  #echo "Setting PASS0DIR to ${PASS0DIR}"
else
  # using local files
  # make calibration directory and fill it
  caldir=${PWD}/results/${run}
  echo mkdir -p ${caldir}
  mkdir -p ${caldir}

  echo "PASS0DIR = ${PASS0DIR}"
  ln -sf ${PASS0DIR}/mbd_shape.calib ${caldir}/
  ln -sf ${PASS0DIR}/mbd_sherr.calib ${caldir}/
  ln -sf ${PASS0DIR}/mbd_timecorr.calib ${caldir}/
  [[ -f ${PASS0DIR}/mbd_slewcorr.calib ]] && ln -sf ${PASS0DIR}/mbd_slewcorr.calib ${caldir}/

  #if [[ ! -f ${caldir}/mbd_slewcorr.calib ]]
  #then
  #  ln -s ${PASS0DIR}/mbd_slewcorr.calib ${caldir}/
  #fi
fi

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

if [[ ! -z "${_CONDOR_SCRATCH_DIR}" ]]
then
  mkdir -p ${_CONDOR_SCRATCH_DIR}
  cp -p * ${_CONDOR_SCRATCH_DIR}
  cd ${_CONDOR_SCRATCH_DIR}
  ln -s ${ORIG_DIR}/results/ ${_CONDOR_SCRATCH_DIR}
  if [[ "$prdf" =~ prdf$ || "$prdf" =~ root$ ]]
  then
    getinputfiles.pl $prdf
  fi
fi


echo root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${calpass},${nevts},${nskip},\"${cdbtag}\"\)
root.exe ${BATCH} -q Fun4All_MBD_CalPass.C\(\"${prdf}\",${calpass},${nevts},${nskip},\"${cdbtag}\"\)

if [[ ! -z "${_CONDOR_SCRATCH_DIR}" ]]
then
  cp -p DST_UNCAL*.root ${ORIG_DIR}
  cd ${ORIG_DIR}
fi

