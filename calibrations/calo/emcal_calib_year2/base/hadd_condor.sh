#!/usr/bin/bash


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

    cp -v "$PWD"/../../CondorHADD.sh CondorRunJob$li.sh
    sed -i "s/RUN/$dir/g" CondorRunJob$li.sh

    chmod +x CondorRunJob$li.sh
    
    cat >>ff.sub<< EOF
+JobFlavour                   = "workday"
transfer_input_files          = ${WorkDir}/CondorRunJob$li.sh
Executable                    = CondorRunJob$li.sh
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
done < runList.txt # redirect the input of the

