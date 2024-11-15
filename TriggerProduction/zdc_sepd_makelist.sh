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
/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/ZDC/${type}/${type}_seb20-${runnumber}-* >seb20.list
if [ ! -s seb20.list ]
then
  echo seb20.list empty, removing it
  rm  seb20.list
fi
