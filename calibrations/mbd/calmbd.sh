#!/usr/bin/env bash
#
# usage:
#  calmbd.sh <prdf or dst or list> <nevts>

infile=$1
echo "input file " $infile

nevt=0
if [[ $# -gt 1 ]] 
then
  nevts=$2
  echo $nevts
fi

# get run number
run=""
if [[ "$infile" =~ prdf$ || "$infile" =~ root$ ]]
then
  run=${infile##*/}
  run=${run%-*}
  run=${run#*-}
  run=${run#000}
elif [[ "$infile" =~ list$ ]]
then
  run=${infile##*/}
  run=${run%.list}
  nevt=2000000
fi

echo processing run $run

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

# do the pass1 calibrations (only 20K events)
echo run_mbdcalpass.sh $infile 1
run_mbdcalpass.sh $infile 1

# process waveforms, output a DST_UNCALMBD
# here time is in ns and charge in adc
echo run_mbdcalpass.sh $infile 2 $nevts
run_mbdcalpass.sh $infile 2 $nevts

# do pass 2.0 (t0 offsets)
rootfname=""
if [[ "$infile" =~ prdf$ ]]
then
  rootfname=${infile%.prdf}.root
  rootfname=DST_UNCALMBD-${rootfname#*-}
elif [[ "$infile" =~ root$ ]]
then
  rootfname=$(echo $infile | sed 's/DST_TRIGGERED_EVENT_run2pp_new_2024p001/DST_UNCALMBD/' | sed 's:^.*/::')
elif [[ "$infile" =~ list$ ]]
then
  rootfname=$(printf "DST_UNCALMBD-%08d-0000.root" $run)
fi

echo run_tcal.sh $rootfname
run_tcal.sh $rootfname
ln -sf ${PASS0DIR}/mbd_tt_t0.calib ${caldir}/
ln -sf ${PASS0DIR}/mbd_tq_t0.calib ${caldir}/

# do pass 2.3 (mip fits)
echo run_qcal.sh $rootfname
run_qcal.sh $rootfname

