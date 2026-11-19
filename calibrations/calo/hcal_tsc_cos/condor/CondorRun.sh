#!/bin/bash

source /sphenix/u/bseidlitz/setup.sh

echo "------------------setting up environment--------------------"
export Cur_dir=$(pwd)
echo "running area:" ${Cur_dir}
echo "-------------------------running----------------------------"
cd ${Cur_dir}
ls
root '' > notes.log


#if [[ -f input.txt]]; then
#  file_path=$(cat input.txt)
#else
#  echo "input.txt does not exist in the current directory. Exiting."
#  exit 1
#fi

# Run the ROOT macro with the file path as the second argument
root "Fun4All_tsc.C(0,\"inputdata.txt\")"

echo "JOB COMPLETE!"
