#!/usr/bin/bash

# creates file lists for the INTT from known locations in lustre
# run number is the input argument

if [ $# -eq 0 ]
  then
    echo "Creates needed lists of input files for the Intt for a given run"
    echo "Usage: intt_makelist.sh <run number>"
    exit 1
fi

if [ $# -eq 1 ]
then
  echo "No type or runnumber supplied"
  exit 0
fi

type=$1
runnumber=$(printf "%08d" $2)
for i in {16..17}
do
seb=$(printf "seb%02d" $i)
listname=$(printf "seb%02d.list" $i)
/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/HCal/${type}/${type}_${seb}-${runnumber}-* > ${listname}
if [ ! -s ${listname} ]
then
  echo ${listname} empty, removing it
  rm ${listname}
fi
done
