#!/bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

echo "------------------setting up environment--------------------"
export Cur_dir=$(pwd)
echo "running area:" ${Cur_dir}
echo "-------------------------running----------------------------"
cd ${Cur_dir}



ls
root '' > notes.log

# the dataset and the calibrations are filled in by run.sh
root -b "Fun4All_HCal.C(0,\"inputdata.txt\",\"DATASET\",\"OHCALCALIB\",\"IHCALCALIB\")"

rm inputdata.txt
echo "DONE" >> DONE.txt

echo "JOB COMPLETE!"
