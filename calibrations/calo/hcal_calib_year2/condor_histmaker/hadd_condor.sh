#!/usr/bin/bash

# Take input file as argument, default to runList.txt
input_file=${1:-runList.txt}
echo "About to submit merge jobs for runs in: $input_file"
read -p "Continue? [y/N]: " confirm
if [[ $confirm != [yY] ]]; then
  echo "Aborting."
  exit 1
fi

export TargetDir="$PWD"/hadd_condor




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
    sed -i "s/RUN/$dir/g" CondorHaddJob$li.sh

    chmod +x CondorHaddJob$li.sh
    
    cat >>ff.sub<< EOF
+JobFlavour                   = "workday"
transfer_input_files          = ${WorkDir}/CondorHaddJob$li.sh
Executable                    = CondorHaddJob$li.sh
Universe                      = vanilla
Notification                  = Never
GetEnv                        = True
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

