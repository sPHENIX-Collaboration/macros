#!/usr/bin/bash

export TargetDir="$PWD"/condorout

if [ -d ${TargetDir} ]; then
  if [ -d ${TargetDir}/OutDir0 ]; then
    rm -rf ${TargetDir}/OutDir*
  fi
else
  mkdir ${TargetDir}
fi

i=0
while read dir; do 
  li=$(printf "%04d" $i)

  rm inputdata.txt
  
# creates a list of all files for a particular run
for file in /sphenix/lustre01/sphnxpro/commissioning/DST_ana395_2023p007/DST_CALO*-000"$dir"-*.root
do
cat >>inputdata.txt<< EOF
$file
EOF
done

  j=10 #jobs per a run
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
  echo "files per job: $files_per_job"
  echo "njob: $njob"


  for((q=0;q<$njob;q++));
  do

    mkdir ${TargetDir}/OutDir$i
    export WorkDir="${TargetDir}/OutDir$i"
    echo "WorkDir:" ${WorkDir}
    start_file=$(( $q*$files_per_job+1 ))
    end_file=$(( $start_file+$files_per_job-1 ))
    echo "start file: $start_file   end file: $end_file"

    sed -n $start_file\,${end_file}p inputdata.txt > tmp.txt
    mv tmp.txt ${WorkDir}/inputdata.txt
    
    pushd ${WorkDir}

      
    
    cp -v "$PWD"/../../CondorRun.sh CondorRunJob$li.sh
    cp -v "$PWD"/../../../Fun4All_EMCal.C .
    
    chmod +x CondorRunJob$li.sh
    
    
    cat >>ff.sub<< EOF
+JobFlavour                   = "workday"
transfer_input_files          = ${WorkDir}/CondorRunJob$li.sh, ${WorkDir}/input.txt
Executable                    = CondorRunJob$li.sh
request_memory                = 10GB
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
  done
done < runList.txt # redirect the input of the
