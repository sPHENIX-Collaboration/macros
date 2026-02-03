#!/bin/bash
input_file=${1:-runList.txt}
echo "About to submit jobs for runs in: $input_file"
read -p "Continue? [y/N]: " confirm
if [[ $confirm != [yY] ]]; then
  echo "Aborting."
  exit 1
fi
mkdir -p dstLists  #Ensure directory exists

export TargetDir="$PWD"/condorout




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
  dstName="dstLists/dst_calofitting_run2pp-000${dir}.list" 

  # creates a list of all files for a particular run
  if [ -f ${dstName} ]; then 
    cp ${dstName} inputdata.txt
  else
    echo "did not find ${dstName} getting from DB"
    CreateDstList.pl --build ana468 --cdb 2024p012_v001 --run ${dir}  DST_CALOFITTING_run2pp
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


