#!/usr/bin/bash

# creates file lists for the MVTX from known locations in lustre
# run number is the input argument

if [ $# -eq 0 ]
  then
    echo "Creates needed lists of input files for the MVTX for a given run"
    echo "Usage: mvtx_makelist.sh <type> <run number>"
    exit 1
fi

if [ $# -eq 1 ]
then
  echo "No type or runnumber supplied"
  exit 0
fi

type=$1
runnumber=$(printf "%08d" $2)

for i in {0..5}
do
ls -1 /sphenix/lustre01/sphnxpro/physics/MVTX/${type}/${type}_mvtx${i}-${runnumber}-* > mvtx${i}.list
if [ ! -s mvtx${i}.list ]
then
  echo mvtx${i}.list empty, removing it
  rm mvtx${i}.list
fi

done
