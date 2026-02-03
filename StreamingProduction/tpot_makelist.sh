#!/usr/bin/bash

if [ $# -eq 0 ]
then
  echo "No runnumber supplied"
  exit 0
fi
if [ $# -eq 1 ]
then
  echo "No type or runnumber supplied"
  exit 0
fi

type=$1
runnumber=$(printf "%08d" $2)

/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/TPOT/${type}/TPOT_ebdc39_${type}-${runnumber}-* >tpot.list
if [ ! -s tpot.list ]
then
  echo tpot.list empty, removing it
  rm  tpot.list
fi
