#!/usr/bin/bash

# Take the beam run list as argument, default is the run3oo good run list
input_file=${1:-/sphenix/user/anjsmenon/work_2026/RunList/Physics/Calo/goodRuns/grls/run3oo_pro001_pcdb001_v001_dst_calofitting_grl.list}
if [ ! -f "$input_file" ]; then
  echo "run list $input_file not found"
  exit 1
fi
# name of the calibration the spectra were made with (see run.sh): cosmics (default), cosmics_tsc, cosmics_tsc_temp, ...
# merges condorout_<name> into hadd_condor_<name> (condorout into hadd_condor for cosmics)
calibName=${2:-cosmics}
outSuffix="_$calibName"
[ "$calibName" = "cosmics" ] && outSuffix=""
if [ ! -d "condorout$outSuffix" ]; then
  echo "no folder condorout$outSuffix"
  exit 1
fi
echo "About to submit merge jobs for runs in: $input_file"
echo "merging condorout$outSuffix into hadd_condor$outSuffix"
read -p "Continue? [y/N]: " confirm
if [[ $confirm != [yY] ]]; then
  echo "Aborting."
  exit 1
fi

export TargetDir="$PWD"/hadd_condor$outSuffix




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

    mkdir ${TargetDir}/OutDir$i
    export WorkDir="${TargetDir}/OutDir$i"
    echo "WorkDir:" ${WorkDir}
    
    pushd ${WorkDir}

    cp -v "$PWD"/../../CondorHADD.sh CondorHaddJob$li.sh
    sed -i "s/RUN/$dir/g; s/CONDOROUT/condorout$outSuffix/" CondorHaddJob$li.sh

    chmod +x CondorHaddJob$li.sh
    
    cat >>ff.sub<< EOF
+JobFlavour                   = "workday"
transfer_input_files          = ${WorkDir}/CondorHaddJob$li.sh
Executable                    = CondorHaddJob$li.sh
Universe                      = vanilla
Notification                  = Never
Priority                      = +12
Output                        = condor.out
Error                         = condor.err
Log                           = /tmp/condor$li.log
Notify_user                   = bs3402@columbia.edu

Queue
EOF

    condor_submit ff.sub
    popd
  
    i=$((i+1))
done < "$input_file" #input file of runnumbers

