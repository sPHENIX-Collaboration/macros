#!/bin/bash

source /opt/sphenix/core/bin/sphenix_setup.sh -n new

echo "------------------setting up environment--------------------"
export Cur_dir=$(pwd)
echo "running area:" ${Cur_dir}
echo "-------------------------running----------------------------"
cd ${Cur_dir}
ls



root -b "Fun4All_EMCal.C(1e3,\"inputdata.txt\",iteration,\"local_calib_copy.root\")"

echo "JOB COMPLETE!"

# if things are running well add the following lines for speed
#rm Fun4All_EMCal.C
#rm ff.sub
#rm inputdata.txt
