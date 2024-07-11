#!/bin/bash

source /sphenix/u/bseidlitz/setup.sh

echo "------------------setting up environment--------------------"
export Cur_dir=$(pwd)
echo "running area:" ${Cur_dir}
echo "-------------------------running----------------------------"
cd ${Cur_dir}
ls
root '' > notes.log

# Read the file path from prdf_file_path.txt into a variable
prdf_path=$(cat prdf_file.txt)

# Run the ROOT macro with the combined string as an argument
root  "Fun4All_HCalCalib.C(0,\"inputdata.txt\")"

echo "JOB COMPLETE!"
