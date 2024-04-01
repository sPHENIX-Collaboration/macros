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
/bin/ls -1 /sphenix/lustre01/sphnxpro/commissioning/mbd/${type}/${type}_seb18-${runnumber}-* >mbd.list
if [ ! -s mbd.list ]
then
  echo mbd.list empty, removing it
  rm  mbd.list
fi
