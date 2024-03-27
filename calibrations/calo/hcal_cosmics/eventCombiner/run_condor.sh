#!/bin/bash


# Directory to look in
export macropath="$PWD"
export TargetDir="$PWD"/condorout

if [ -d ${TargetDir} ]; then
  rm -rf ${TargetDir}/Run*
else
  mkdir ${TargetDir}
fi

# Loop through each number in the array
while read number; do

    # Create a directory for each run number, then create OutDir within that
    export WorkDir="${TargetDir}/Run$number"
    mkdir -p ${WorkDir}
    echo "WorkDir:" ${WorkDir}

    pushd ${WorkDir}

    ls -1 /sphenix/lustre01/sphnxpro/commissioning/HCal/cosmics/cosmics_seb16-000${number}-* > hcaleast.list
    ls -1 /sphenix/lustre01/sphnxpro/commissioning/HCal/cosmics/cosmics_seb17-000${number}-* > hcalwest.list

    cp -v "$macropath"/CondorRun.sh CondorRunTC$number.sh
    cp -v "$macropath"/Fun4All_Combiner.C Fun4All_Combiner.C

    chmod +x CondorRunTC$number.sh

    cat >>ff.sub<< EOF
+JobFlavour                   = "workday"
transfer_input_files          = ${WorkDir}/CondorRunTC$number.sh,${WorkDir}/Fun4All_Combiner.C
Executable                    = CondorRunTC$number.sh
Universe                      = vanilla
Notification                  = Never
GetEnv                        = True
Priority                      = +40
Output                        = test.out
Error                         = test.err
Log                           = test.log
Notify_user                   = hj2552@columbia.edu

Queue
EOF

    condor_submit ff.sub
    popd
done < runlist.txt
