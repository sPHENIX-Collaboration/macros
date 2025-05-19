#!/usr/bin/bash
#
# Run the mbd calibrations
#
#

# Default values
runno=
outbase="DST_MBD_CALIBRATION_run3auau"
outdir="/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/TEST"
logdir="/sphenix/user/chiu/sphenix_bbc/run2025/CALIBPRODUCTION/TEST/log"
logbase="DST_MBD_CALIBRATION_run3auau"
build="new"
dbtag="ProdA_2024"
pass0dir=""      # only set if using local private pass0 calibs, otherwise use CDB
nevents=0        # by default process all events

#echo $@ 

# Use getopt to parse options
PARSED=$(getopt -o "r:b:d:p:n:h" -l "outbase:,outdir:,logbase:,logdir:,run:,build:,dbtag:,pass0dir:,nevents:,help" -- "$@")
if [[ $? -ne 0 ]]; then
    echo "Failed to parse arguments" >&2
    exit 1
fi

# Apply the parsed arguments
eval set -- "$PARSED"

# Extract options and their arguments
while true; do
    case "$1" in
        --outbase)
            outbase="$2"
            shift 2
            ;;
        --outdir)
            outdir="$2"
            shift 2
            ;;
        --logbase)
            logbase="$2"
            shift 2
            ;;
        --logdir)
            logdir="$2"
            shift 2
            ;;
        -r|--run)
            runno="$2"
            shift 2
            ;;
        -b|--build)
            build="$2"
            shift 2
            ;;
        -d|--dbtag)
            dbtag="$2"
            shift 2
            ;;
        -p|--pass0dir)
            pass0dir="$2"
            shift 2
            ;;
        -n|--nevents)
            nevents="$2"
            shift 2
            ;;
        -h|--help)
            echo "Usage: $0 [--OPTIONS] inputfile"
            echo
            echo " The inputfile can be a list file, DST, or PRDF"
            echo
            echo " The options are"
            echo
            echo "          [--outbase BASENAME] [--outdir DIR]"
            echo "          [--logbase LOGBASE] [--logdir LOGDIR]"
            echo "          [--run RUNNUMBER]"
            echo "          [--build BUILD] [--dbtag CDBTAG]"
            echo "          [--nevents NEVENTS]"
            echo "          [--pass0dir PASS0DIR]  # only for using local (text-based) pass0 calib"
            exit 0
            ;;
        --)
            shift
            break
            ;;
        *)
            echo "Unknown option: $1"
            exit 1
            ;;
    esac
done


# Validate required option(s)
if [[ -z "$runno" ]]; then
    echo "Error: --run is required" >&2
    exit 1
fi


# Get input files, concatentate into comma-separated string
inputs=$1
shift
for arg in "$@"; do
  inputs=${inputs},${arg}
done
echo $inputs 

# update the base names with the build, tag, and run number
outbase=${outbase}_${build}_${dbtag}_$(printf "%08d" ${runno})
logbase=${logbase}_${build}_${dbtag}_$(printf "%08d" ${runno})


{

echo run:     ${runno}
echo inputs:  ${inputs}
echo outbase: ${outbase}
echo outdir:  ${outdir}
echo logbase: ${logbase}
echo logdir:  ${logdir}
echo build:   ${build}
echo dbtag:   ${dbtag}
echo pass0dir: ${pass0dir}
echo nevents: ${nevents}


if [[ $runno -lt 44100 ]]
then
  runtype=1  # pp200
else
  runtype=0  # auau200
fi

echo runtype $runtype

export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${USER}

if [[ $build != "none" ]]   # use none to keep existing env
then
  echo source /opt/sphenix/core/bin/sphenix_setup.sh -n ${build}
  source /opt/sphenix/core/bin/sphenix_setup.sh -n ${build}
fi

export ODBCINI=./odbc.ini
 
# There ought to be just one here... but ymmv...
echo "Input files: " ${inputs}

#if test -f cupstest.py
#then
#   mv cupstest.py cups.py
#fi

# Flag as started
#./cups.py -r ${runno} -s ${segment} -d ${outbase} started

################################################
# make local calibration directory and fill it
caldir=${PWD}/results/${runno}
echo mkdir -p ${caldir}
mkdir -p ${caldir}


################################################
# If using local files, stage PASS0 calibrations
if [[ ! -z ${pass0dir} ]]
then
#  ./cups.py -r ${runno} -s ${segment} -d ${outbase} message "Stage in pass0 from ${pass0dir}"
  for calib in mbd_shape.calib mbd_sherr.calib mbd_timecorr.calib mbd_slewcorr.calib mbd_tt_t0.calib mbd_tq_t0.calib
  do
    echo Stagein ${pass0dir}/${calib} to ${caldir}
    cp -p ${pass0dir}/${calib} ${caldir}/
  done
fi

# Flag as started
#./cups.py -r ${runno} -s ${segment} -d ${outbase} running

################################################
# Pass 1 calibrations
echo "###############################################################################################################"
echo "Running pass1 calibration"
#./cups.py -r ${runno} -s ${segment} -d ${outbase} message "Running PASS 1 calibration"
echo root.exe -q -b Fun4All_MBD_CalPass.C\(\"${inputs}\",1,-1,0,\"${dbtag}\"\) 
root.exe -q -b Fun4All_MBD_CalPass.C\(\"${inputs}\",1,-1,0,\"${dbtag}\"\) 

echo "Pass 1 calibration done"
ls -la *.root

################################################
# Pass 2 calibrations waveforms
#./cups.py -r ${runnumber} -s ${segment} -d ${outbase} message "Running PASS 2 calibration, process waveforms"
echo root.exe -q -b Fun4All_MBD_CalPass.C\(\"${inputs}\",2,${nevents},0,\"${dbtag}\"\)
root.exe -q -b Fun4All_MBD_CalPass.C\(\"${inputs}\",2,${nevents},0,\"${dbtag}\"\)

echo "Pass 2 calibration done (waveforms processed)"
ls -la *.root

# Flag as started
#./cups.py -r ${runnumber} -s ${segment} -d ${outbase} running

################################################
# Pass 2 calibrations (t0 offsets)
# Pass 2 calibrations mip fits
fname=$(ls -tr DST_UNCALMBD*.root | tail -1)

echo "###############################################################################################################"
echo "Running pass2.0 calibration"
#./cups.py -r ${runnumber} -s ${segment} -d ${outbase} message "Running PASS 2.0 calibration"
pass=0
echo root.exe -q -b cal_mbd.C\(\"${fname}\",${pass},${nevents}\)
root.exe -q -b cal_mbd.C\(\"${fname}\",${pass},${nevents}\)

echo "###############################################################################################################"
echo "Running pass2.3 calibration"
#./cups.py -r ${runnumber} -s ${segment} -d ${outbase} message "Running PASS 2.2 calibration"
pass=3
echo root.exe -q -b cal_mbd.C\(\"${fname}\",${pass},${nevents},${runtype}\)
root.exe -q -b cal_mbd.C\(\"${fname}\",${pass},${nevents},${runtype},\"${dbtag}\"\)

# Flag as done
#./cups.py -r ${runno} -s ${segment} -d ${outbase} message "Done"
#./cups.py -r ${runno} -s ${segment} -d ${outbase} finished -e 0

# Copy out files
mkdir -p ${outdir}/${runno}
cp -p ${fname} ${outdir}/${runno}/
for r in ${caldir}/*
do
    # skip linked files
    if [[ -h $r ]]
    then
      continue
    fi

    cp -p ${r} ${outdir}/${runno}/

done

################################################

# Flag run as finished.  Increment nevents by zero
#echo ./cups.py -v -r ${runno} -s ${segment} -d ${outbase} finished -e 0 --nevents 0 --inc 
#     ./cups.py -v -r ${runno} -s ${segment} -d ${outbase} finished -e 0 --nevents 0 --inc 


}  > ${logbase}.out 2>${logbase}.err 

[[ "${logdir%/}" != "." ]] && cp -p ${logbase}.out  ${logdir}
[[ "${logdir%/}" != "." ]] && cp -p ${logbase}.err  ${logdir}

