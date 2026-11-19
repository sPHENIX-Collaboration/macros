#!/usr/bin/bash

# creates file lists for the INTT from known locations in lustre
# run number is the input argument

if [ $# -eq 0 ]
  then
    echo "Creates needed lists of input files for the Intt for a given run"
    echo "Usage: intt_makelist.sh <type> <run number>"
    exit 1
fi

if [ $# -eq 1 ]
then
  echo "No type or runnumber supplied"
  exit 0
fi

type=$1
runnumber=$(printf "%08d" $2)

for i in {0..7}
do
/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/INTT/${type}/${type}_intt${i}-${runnumber}-* > intt${i}.list
if [ ! -s intt${i}.list ]
then
  echo intt${i}.list empty, removing it
  rm intt${i}.list
fi

done
