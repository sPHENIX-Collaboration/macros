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

subpass=1  # do slew calibrations, don't apply them
echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events}\)
root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events}\) 

# after this, run recal_mbd_slew.C to generate the slewcorr calib file,
run=${mbd_uncalrootf##*/}
run=${run##*-}
run=${run%.root}
run=$(echo ${run} | sed 's/^00*//')

slewrootf=results/${run}/calmbdpass2.${subpass}_slew-${run}.root
echo root.exe -b -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 
root.exe -b -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 

exit

# cp the slewcorr file so that it is used in the subpass 2
echo cp -p results/${run}/pass${subpass}_mbd_slewcorr.calib results/${run}/mbd_slewcorr.calib
cp -p results/${run}/pass${subpass}_mbd_slewcorr.calib results/${run}/mbd_slewcorr.calib
echo Updating mbd_slewcorr.calib using pass${subpass}_mbd_slewcorr.calib

# then run addt0.C to get combined t0 calibs and cp it
echo root.exe -b -q addt0.C\(\"results/${run}/mbd_tt_t0.calib\",\"results/${run}/pass${subpass}_mbd_tt_t0.calib\"\)
root.exe -b -q addt0.C\(\"results/${run}/mbd_tt_t0.calib\",\"results/${run}/pass${subpass}_mbd_tt_t0.calib\"\)
rm -f results/${run}/mbd_tt_t0.calib results/${run}/mbd_tq_t0.calib
cp -p add_mbd_tt_t0.calib results/${run}/mbd_tt_t0.calib
cp -p add_mbd_tq_t0.calib results/${run}/mbd_tq_t0.calib

subpass=2  # do slew calibrations, applying any that exist
echo root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events}\)
root.exe $BATCH -q cal_mbd.C\(\"${mbd_uncalrootf}\",${subpass},${tcalib_events}\) 

slewrootf=results/${run}/calmbdpass2.${subpass}_slew-${run}.root
echo root.exe -b -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 
root.exe -b -q recal_mbd_slew.C\(\"${slewrootf}\",${subpass}\) 

# then run addslew.C to get combined slewcorr.calib. need to add in original, since that was applied in dst_uncalmbd
# then run addt0 to get combined t0 calibs
