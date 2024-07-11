#!/usr/bin/bash

if [ $# -eq 0 ]
then
  echo "No runnumber supplied"
  exit 0
fi

runnumber=$(printf "%08d" $1)
/bin/ls -1 /sphenix/lustre01/sphnxpro/commissioning/GL1/cosmics/GL1_cosmic*-${runnumber}-* >gl1.list
if [ ! -s gl1.list ]
then
  echo gl1.list empty, removing it
  rm  gl1.list
fi
