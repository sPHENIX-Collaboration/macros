#!/usr/bin/bash

# "$(nevents) {outbase} {logbase} $(run) $(seg) {outdir} $(build) $(tag) $(inputs) $(ranges) {neventsper}"  
nevents=${1:-0}
outbase=${2}
logbase=${3}
runnumber=${4}
segment=${5}
outdir=${6}
build=${7}
dbtag=${8}
inputs=(`echo ${9} | tr "," " "`)  # array of input files 
ranges=(`echo ${10} | tr "," " "`)  # array of input files with ranges appended
neventsper=${11:-1000}
logdir=${12:-.}
pass0dir=${13}      # only set if using local private pass0 calibs, otherwise use CDB

echo $@ 

echo "XXX"

{

echo nevents: ${nevents}
echo outbase: ${outbase}
echo logbase: ${logbase}
echo run:     ${runnumber}
echo seg:     ${segment}
echo outdir:  ${outdir}
echo build:   ${build}
echo dbtag:   ${dbtag}
echo inputs:  ${inputs[@]}
echo nevper:  ${neventsper}
echo logdir:  ${logdir}
echo pass0dir: ${pass0dir}

if [[ $run -lt 44100 ]]
then
  runtype=1 # pp200
else
  runtype=0  # auau200
fi

echo runtype $runtype

export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${USER}

source /opt/sphenix/core/bin/sphenix_setup.sh -n ${7}

export ODBCINI=./odbc.ini
 
# There ought to be just one here... but ymmv...
echo ${inputs[@]}

if test -f cupstest.py
then
   mv cupstest.py cups.py
fi

# Flag as started
./cups.py -r ${runnumber} -s ${segment} -d ${outbase} started

################################################
# make calibration directory and fill it
caldir=${PWD}/results/${runnumber}
echo mkdir -p ${caldir}
mkdir -p ${caldir}


################################################
# If using local files, stage PASS0 calibrations
if [[ ! -z ${pass0dir} ]]
then
  ./cups.py -r ${runnumber} -s ${segment} -d ${outbase} message "Stage in pass0 from ${pass0dir}"
  for calib in mbd_shape.calib mbd_sherr.calib mbd_timecorr.calib mbd_slewcorr.calib mbd_tt_t0.calib mbd_tq_t0.calib
  do
    echo Stagein ${pass0dir}/${calib} to ${caldir}
    cp ${pass0dir}/${calib} ${caldir}/
  done
fi

# Flag as started
./cups.py -r ${runnumber} -s ${segment} -d ${outbase} running

################################################
# Pass 1 calibrations
echo "###############################################################################################################"
echo "Running pass1 calibration"
./cups.py -r ${runnumber} -s ${segment} -d ${outbase} message "Running PASS 1 calibration"
echo root.exe -q -b Fun4All_MBD_CalPass.C\(\"${inputs[0]}\",1,-1,0,\"${dbtag}\"\) 
root.exe -q -b Fun4All_MBD_CalPass.C\(\"${inputs[0]}\",1,-1,0,\"${dbtag}\"\) 

echo "Pass 1 calibration done"
ls -la *.root

# Flag as started
./cups.py -r ${runnumber} -s ${segment} -d ${outbase} message "Done"
./cups.py -r ${runnumber} -s ${segment} -d ${outbase} finished -e 0

for r in ${caldir}/*
do
    # skip linked files
    if [[ -h $r ]]
    then
      continue
    fi

    ./stageout.sh ${r} ${outdir} ${outbase}-$(printf "%08d" ${runnumber})-$(printf "%04d" ${segment}).root
done

################################################

# Flag run as finished.  Increment nevents by zero
echo ./cups.py -v -r ${runnumber} -s ${segment} -d ${outbase} finished -e 0 --nevents 0 --inc 
     ./cups.py -v -r ${runnumber} -s ${segment} -d ${outbase} finished -e 0 --nevents 0 --inc 


}  > ${logbase}.out 2>${logbase}.err 

[[ "${logdir%/}" != "." ]] && cp -p ${logbase}.out  ${logdir}
[[ "${logdir%/}" != "." ]] && cp -p ${logbase}.err  ${logdir}

