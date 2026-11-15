#!/bin/bash
# Submits the condor jobs that make the tower spectra of a dataset with Fun4All_HCal.C.
#   sh run.sh [dataset] [run list] [OHCal calibration] [IHCal calibration] [name]
# dataset    : run2pp, run2auau, run3oo, ... (see ../HcalCalibDatasetinputs.h), asked for if not given
# run list   : one run per line, default: the run list of the dataset in HcalCalibDatasetinputs.h
# calibration: cosmics          (default) the cosmics only file of the dataset
#              cosmics_tsc      the cosmics + towerslope file of the dataset (genTSCalib)
#              cosmics_tsc_temp the final files of the dataset, cosmics + towerslope + temperature (genFinalCalib)
#              cdb, a CDB file, or a folder with final calibration files (see Fun4All_HCal.C)
#              The IHCal calibration is the same kind as the OHCal one if it is not given.
# name       : name of the calibration for the output folder, default: the calibration (e.g. cosmics_tsc).
#              The jobs write to condorout (cosmics) or condorout_<name>; give the same name to
#              hadd_condor.sh and copy_outfiles.sh.
#   sh run.sh run3oo cosmics_tsc        -> condorout_cosmics_tsc
#   sh run.sh run3oo cosmics_tsc_temp   -> condorout_cosmics_tsc_temp
# (the run list can be left out when the calibration is cosmics, cosmics_tsc, cosmics_tsc_temp or cdb)

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

dataset=$1
# the run list can be left out: sh run.sh run3oo cosmics_tsc is sh run.sh run3oo "" cosmics_tsc
case "$2" in
  cosmics|cosmics_tsc|cosmics_tsc_temp|cdb) [ ! -f "$2" ] && set -- "$1" "" "${@:2}" ;;
esac
if [ -z "$dataset" ]; then
  datasets=$(root -l -b -q ../PrintHcalCalibDatasetinputs.C | grep '^HCALCALIB_DATASETS=' | cut -d= -f2-)
  read -p "dataset ($datasets): " dataset
fi

# settings of the dataset from HcalCalibDatasetinputs.h
info=$(root -l -b -q "../PrintHcalCalibDatasetinputs.C(\"$dataset\")")
if ! echo "$info" | grep -q '^HCALCALIB_NAME='; then
  echo "$info" | tail -1
  exit 1
fi
getinfo() { echo "$info" | grep "^HCALCALIB_$1=" | cut -d= -f2-; }
dstTag=$(getinfo DSTTAG)

input_file=${2:-$(getinfo RUNLIST)}
if [ -z "$input_file" ]; then
  read -p "no run list for $dataset in HcalCalibDatasetinputs.h, run list: " input_file
fi
if [ ! -s "$input_file" ]; then
  echo "run list \"$input_file\" not found or empty"
  exit 1
fi
if [ -z "$dstTag" ]; then
  read -p "no DST tag for $dataset in HcalCalibDatasetinputs.h, tag for CreateDstList.pl (e.g. pro001_pcdb001_v001): " dstTag
fi
ohcalCalib=${3:-cosmics}
ihcalCalib=$4
if [ -z "$ihcalCalib" ]; then
  # same kind of calibration as the OHCal; a file or folder given for the OHCal is not used for the IHCal
  case "$ohcalCalib" in
    cosmics|cosmics_tsc|cosmics_tsc_temp|cdb) ihcalCalib=$ohcalCalib ;;
    *) ihcalCalib=cosmics ;;
  esac
fi
# name of the calibration, for the output folder
calibName=$5
if [ -z "$calibName" ]; then
  if [ "$ohcalCalib" = "$ihcalCalib" ] && [ ! -e "$ohcalCalib" ]; then
    calibName=$ohcalCalib
  else
    read -p "name of this calibration for the output folder (condorout_<name>): " calibName
  fi
fi
if [ -z "$calibName" ]; then
  echo "no name for the calibration"
  exit 1
fi
outSuffix="_$calibName"
[ "$calibName" = "cosmics" ] && outSuffix=""
# a file or folder is given to the jobs with its full path
[ -e "$ohcalCalib" ] && ohcalCalib=$(readlink -f "$ohcalCalib")
[ -e "$ihcalCalib" ] && ihcalCalib=$(readlink -f "$ihcalCalib")

echo "dataset           : $dataset"
echo "run list          : $input_file ($(grep -c . $input_file) runs)"
echo "DST files         : CreateDstList.pl -dataset $dataset --tag $dstTag DST_CALOFITTING"
showCalib() {  # detector, calibration, cosmics only file, final folder
  case "$2" in
    cosmics) echo "$1 calibration : cosmics only, $3" ;;
    cosmics_tsc) echo "$1 calibration : cosmics + towerslope, files in $(getinfo TSCDIR)" ;;
    cosmics_tsc_temp) echo "$1 calibration : cosmics + towerslope + temperature, files in $4" ;;
    *) echo "$1 calibration : $2" ;;
  esac
}
showCalib OHCal "$ohcalCalib" "$(getinfo OHCOSMIC)" "$(getinfo OHFINALDIR)"
showCalib IHCal "$ihcalCalib" "$(getinfo IHCOSMIC)" "$(getinfo IHFINALDIR)"
# the folders of the dataset have to be noted and there
for c in "$ohcalCalib:$(getinfo OHFINALDIR)" "$ihcalCalib:$(getinfo IHFINALDIR)"; do
  kind=${c%%:*}; dir=""
  [ "$kind" = "cosmics_tsc" ] && dir=$(getinfo TSCDIR)
  [ "$kind" = "cosmics_tsc_temp" ] && dir=${c#*:}
  if [ "$kind" = "cosmics_tsc" ] || [ "$kind" = "cosmics_tsc_temp" ]; then
    if [ -z "$dir" ] || [ ! -d "$dir" ]; then
      echo "folder of the $kind files of $dataset not noted in HcalCalibDatasetinputs.h or not there: \"$dir\""
      exit 1
    fi
  fi
done
echo "output folder     : condorout$outSuffix"
echo "About to submit jobs for runs in: $input_file"
read -p "Continue? [y/N]: " confirm
if [[ $confirm != [yY] ]]; then
  echo "Aborting."
  exit 1
fi
mkdir -p dstLists  #Ensure directory exists

export TargetDir="$PWD"/condorout$outSuffix




if [ -d ${TargetDir} ]; then
  if [ -n "$(ls -A ${TargetDir}/OutDir*)" ]; then
    rm -rf ${TargetDir}/OutDir*
  fi
else
  mkdir ${TargetDir}
fi

i=0
while read dir; do
  li=$(printf "%04d" $i)

  rm inputdata.txt
  rm dst_cal*
  dstName="dstLists/dst_calofitting-000${dir}.list"

  # creates a list of all files for a particular run
  if [ -f ${dstName} ]; then 
    cp ${dstName} inputdata.txt
  else
    echo "did not find ${dstName} getting from DB"
    CreateDstList.pl -dataset ${dataset} --tag ${dstTag} --run ${dir} DST_CALOFITTING
    cp dst_calo* inputdata.txt
    cp dst_calo* dstLists/.
  fi

  if [ ! -s inputdata.txt ]; then
    echo "inputdata.txt is empty, skipping to next iteration."
    continue
  fi

  j=50

  tot_files=$( cat inputdata.txt | wc -l )
  echo "total files: $tot_files"
  rem=$(( $tot_files%$j ))
  files_per_job=$(( $tot_files/$j ))
  njob=$j
  if [ $rem -ne 0 ]; then
    files_per_job=$(( $files_per_job+1 ))
  fi
  rem2=$(( $tot_files%$files_per_job ))
  njob=$(( $tot_files/$files_per_job ))
  if [ $rem2 -ne 0 ]; then
    njob=$(( ($tot_files/$files_per_job)+1 ))
  fi
  #echo "files per job: $files_per_job"
  #echo "njob: $njob"

  mkdir -p ${TargetDir}/OutDir$i
  export WorkDir="${TargetDir}/OutDir$i"
  echo "WorkDir:" ${WorkDir}

  cat > ${WorkDir}/ff.sub <<EOF
+JobFlavour                   = "workday"
Universe                      = vanilla
Notification                  = Never
Priority                      = +12
concurrency_limits            =CONCURRENCY_LIMIT_DEFAULT:20

EOF

  pushd ${WorkDir}

  cp "$PWD"/../../../Fun4All_HCal.C .
  cp "$PWD"/../../../HcalCalibDatasetinputs.h .
  cp "$PWD"/../../inputdata.txt .
  touch DONE.txt

  for((q=0;q<$njob;q++)); do
    start_file=$(( $q*$files_per_job+1 ))
    end_file=$(( $start_file+$files_per_job-1 ))
    #echo "start file: $start_file   end file: $end_file"

    sed -n $start_file\,${end_file}p inputdata.txt > ${WorkDir}/inputdata_$q.txt

    cp "$PWD"/../../CondorRun.sh CondorRunJob${li}_$q.sh
    sed -i "10 a cp ${WorkDir}/inputdata_$q.txt inputdata.txt" CondorRunJob${li}_$q.sh
    sed -i "s/inputdata.txt/inputdata_$q.txt/g" CondorRunJob${li}_$q.sh
    sed -i "s|DATASET|${dataset}|; s|OHCALCALIB|${ohcalCalib}|; s|IHCALCALIB|${ihcalCalib}|" CondorRunJob${li}_$q.sh
    #echo "rm CondorRunJob${li}_$q.sh" >> CondorRunJob${li}_$q.sh
   #transfer_input_files          = ${WorkDir}/CondorRunJob${li}_$q.sh, ${WorkDir}/inputdata_$q.txt,${WorkDir}/Fun4All_HCal.C

    cat >> ${WorkDir}/ff.sub <<EOF
Executable                    = ${WorkDir}/CondorRunJob${li}_$q.sh
Output                        =   ${WorkDir}/condor_${li}_$q.out
Error                         =  ${WorkDir}/condor_${li}_$q.errr
Log                           = /tmp/condor_${li}_$q.log
Queue

EOF

    i=$((i+1))
  done

  echo "submitting some condor jobs"
  condor_submit ff.sub
  popd

done < "$input_file"  # redirect the input of the


# Set the directory where the files are located
file_directory="${TargetDir}/OutDir*/DONE.root"

# while [ $(find condorout/OutDir* -name "DONE.txt" -print0 | xargs -0 cat | wc -l) -lt $((i)) ]; do
#    current_file_count=$(find condorout/OutDir* -name "DONE.txt" -print0 | xargs -0 cat | wc -l)
#    echo "Waiting for $((i)) files, currently $current_file_count"
#    sleep 30  # Adjust the sleep duration as needed
# done


