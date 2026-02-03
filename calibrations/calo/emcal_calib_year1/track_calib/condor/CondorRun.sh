#!/bin/bash

echo "------------------setting up environment--------------------"
export Cur_dir=$(pwd)
echo "running area:" ${Cur_dir}
echo "-------------------------running----------------------------"
cd ${Cur_dir}
ls
root '' > notes.log


# Run the ROOT macro with the file path as the second argument
root "Fun4All_EMCal.C(0,\"inputdata.txt\")"

echo "JOB COMPLETE!"
