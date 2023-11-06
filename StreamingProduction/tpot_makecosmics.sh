#!/usr/bin/bash

if [ $# -eq 0 ]
then
  echo "No runnumber supplied"
  exit 0
fi

runnumber=$(printf "%08d" $1)
/bin/ls -1 /sphenix/lustre01/sphnxpro/commissioning/TPOT/cosmics/TPOT_ebdc39_cosmics-${runnumber}-* >tpot.list
if [ ! -s tpot.list ]
then
  echo tpot.list empty, removing it
  rm  tpot.list
fi
