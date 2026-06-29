#!/usr/bin/env bash


if [[ $USER == "sphnxpro" ]]
then
  BUILD=pro.001
  source /opt/sphenix/core/bin/sphenix_setup.sh -n $BUILD
  TOPDIR=${PWD}
  # work area for this sub-production
  tobedonedir=${TOPDIR}/SUBMITTED.${BUILD}.$(date +"%Y%m%d%H%M")
  # directory containing the calib macros and scripts
  SUBMITDIR=${tobedonedir}
elif [[ $USER == "chiu" ]]
then
  TOPDIR=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/lists
  tobedonedir=${TOPDIR}/SUBMITTED.$(date +"%Y%m%d%H%M")
  SUBMITDIR=/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/
fi

mkdir ${tobedonedir}
cd ${tobedonedir}

# copy macros and scripts
cd ${SUBMITDIR}
if [[ $USER == "sphnxpro" ]]
then
  cp -p ${HOME}/chiu/offline/macros/calibrations/mbd/* .
elif [[ $USER == "chiu" ]]
then
  cp -p /sphenix/user/chiu/sphenix_bbc/offline/macros/calibrations/mbd/* .
fi

exit

# here we should copy the runlists over, and then check that all is okay

# submit jobs to condor
for fname in ${SUBMITDIR}/runlists/?????.list
do
  echo ./submit.sh ./prorun.sh ${fname}
  ./submit.sh ./prorun.sh ${fname}

  let njobs=$(condor_q | tail -3 | awk '/sphnxpro/ { print $4 }')
  while [[ $njobs -gt 900 ]]
  do
    sleep 300
    let njobs=$(condor_q | tail -3 | awk '/sphnxpro/ { print $4 }')
  done
done

