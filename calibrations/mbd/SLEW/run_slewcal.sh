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

fname=$1    # DST_UNCALMBD filename

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

run=${mbd_uncalrootf##*/}
run=${run#*-}
run=${run%.root}
run=${run%-*}
run=$((10#${run}))
echo Processing $run

runtype=1  # 0=auau, 1=pp

# we should run_tcal.sh first to get updated tt_t0, etc.

#subpass=1  # generate slew curves (slew calibrations may or may not be applied in dst_uncal)
#echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events},${runtype}\)
#root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events},${runtype}\)
#
## after this, run recal_mbd_slew.C to generate the slewcorr calib file,
#
#slewrootf=results/${run}/calmbdpass2.${subpass}_slew-${run}.root
#echo root.exe ${BATCH} -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 
#root.exe ${BATCH} -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 

# cp the pass1 slewcorr file so that it is used in the subpass 2
# this applies the slew correction to whatever was already in the dst_uncal
# (you should not run the addslew.c here)
#echo cp -p results/${run}/pass${subpass}_mbd_slewcorr.calib results/${run}/mbd_slewcorr.calib
#cp -p results/${run}/pass${subpass}_mbd_slewcorr.calib results/${run}/mbd_slewcorr.calib
#echo Updating mbd_slewcorr.calib using pass${subpass}_mbd_slewcorr.calib

## then run addt0.C to get combined t0 calibs and cp it
#echo root.exe -b -q addt0.C\(\"results/${run}/mbd_tt_t0.calib\",\"results/${run}/pass${subpass}_mbd_tt_t0.calib\"\)
#root.exe -b -q addt0.C\(\"results/${run}/mbd_tt_t0.calib\",\"results/${run}/pass${subpass}_mbd_tt_t0.calib\"\)
#rm -f results/${run}/mbd_tt_t0.calib results/${run}/mbd_tq_t0.calib
#cp -p results/${run}/add_mbd_tt_t0.calib results/${run}/mbd_tt_t0.calib
#cp -p results/${run}/add_mbd_tq_t0.calib results/${run}/mbd_tq_t0.calib

subpass=2  # do slew calibrations, applying any that exist
echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events},${runtype}\)
root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events},${runtype}\) 

slewrootf=results/${run}/calmbdpass2.${subpass}_slew-${run}.root
echo root.exe ${BATCH} -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 
root.exe ${BATCH} -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 

# then run addslew.C to get combined slewcorr.calib. need to add in original, since that was applied in dst_uncalmbd
# then run addt0 to get combined t0 calibs
