#!/usr/bin/bash

# creates file lists for the INTT from known locations in lustre
# run number is the input argument

if [ $# -eq 0 ]
  then
    echo "Creates needed lists of input files for the Intt for a given run"
    echo "Usage: intt_makelist.sh <run number>"
    exit 1
fi

runnumber=$(printf "%08d" $1)

for i in {0..7}
do
/bin/ls -1 /sphenix/lustre01/sphnxpro/commissioning/INTT/cosmics/cosmics_intt${i}-${runnumber}-* > intt${i}.list
if [ ! -s intt${i}.list ]
then
  echo intt${i}.list empty, removing it
  rm intt${i}.list
fi

done
