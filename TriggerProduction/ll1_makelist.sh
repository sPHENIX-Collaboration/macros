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

/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/LL1/${type}/${type}_*-${runnumber}-* >seb19.list
if [ ! -s seb19.list ]
then
  echo seb19.list empty, removing it
  rm  seb19.list
fi
