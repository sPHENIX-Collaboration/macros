#!/usr/bin/bash

if [ $# -eq 0 ]
then
  echo "No type and runnumber supplied"
  exit 0
fi

if [ $# -eq 1 ]
then
  echo "No type or runnumber supplied"
  exit 0
fi

type=$1
runnumber=$(printf "%08d" $2)

/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/GL1/${type}/GL1_*-${runnumber}-* >gl1daq.list
if [ ! -s gl1daq.list ]
then
  echo gl1daq.list empty, removing it
  rm  gl1daq.list
fi
