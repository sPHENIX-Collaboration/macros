#!/usr/bin/env bash
#
# run_qcal.sh <fname>
# do charge fits to get timing offset calibrations
# <fname> is the name of the uncalibrated mbd dst file
#

# Check that user specifies a file
if [[ $# -lt 1 ]]
then
  echo "Usage: run_qcal.sh <fname>"
  exit -1
fi

mbd_uncalrootf=$1    # DigSig ROOT filename

nevt=0
if [[ $# -gt 1 ]]
then
  nevt=$2
  echo Processing $2 events
fi

echo PATH=$PATH
echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH

# now run calibrations
qcalib_events=0
if [[ $nevt -ne 0 ]]
then
  qcalib_events=${nevt}
fi

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

pass=3
runtype=1 # pp200
echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${qcalib_events},${runtype}\)
root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${qcalib_events},${runtype}\)  # time calibrations

