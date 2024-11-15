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
/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/mbd/${type}/${type}_seb18-${runnumber}-* >seb18.list
if [ ! -s seb18.list ]
then
  echo seb18.list empty, removing it
  rm  seb18.list
fi
