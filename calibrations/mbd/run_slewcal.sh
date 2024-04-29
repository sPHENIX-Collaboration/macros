#!/usr/bin/env bash
#
# run_slewcal.sh <fname>
# do slew calibrations
# <fname> is the name of the DSTUNCALMBD root file with the waveforms

# Check that user specifies a file
if [[ $# -lt 1 ]]
then
  echo "Usage: run_slewcal.sh <rootfname>"
  exit -1
fi

fname=$1    # DigSig ROOT filename

nevt=0
if [[ $# -gt 1 ]]
then
  nevt=$2
  echo Processing $2 events
fi

rootf=$fname
echo PATH=$PATH
echo LD_LIBRARY_PATH=$LD_LIBRARY_PATH

# first process up to 250K uncalibrated events
mbd_uncalrootf=${fname%.root}.root

# now run calibrations
tcalib_events=0
if [[ $nevt -ne 0 ]] && [[ $nevt -lt 100000 ]]
then
  tcalib_events=${nevt}
fi

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

pass=1  # do slew calibrations, don't apply
echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${tcalib_events}\)
root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${tcalib_events}\) 

# cp the slewcorr file so thaat it is used

#pass=2  # do slew calibrations, applying any that exist
#echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${tcalib_events}\)
#root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${pass},${tcalib_events}\) 
