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
ls -1 /sphenix/lustre01/sphnxpro/commissioning/HCal/${type}/${type}_East-${runnumber}-* > hcaleast.list
ls -1 /sphenix/lustre01/sphnxpro/commissioning/HCal/${type}/${type}_West-${runnumber}-* > hcalwest.list
if [ ! -s hcaleast.list ]
then
  ls -1 /sphenix/lustre01/sphnxpro/commissioning/HCal/${type}/${type}_seb16-${runnumber}-* > hcaleast.list
  if [ ! -s hcaleast.list ]
  then
    echo hcaleast.list empty, removing it
    rm hcaleast.list
  fi
  ls -1 /sphenix/lustre01/sphnxpro/commissioning/HCal/${type}/${type}_seb17-${runnumber}-* > hcalwest.list
  if [ ! -s hcalwest.list ]
  then
    echo hcalwest.list empty, removing it
    rm hcalwest.list
  fi
fi
