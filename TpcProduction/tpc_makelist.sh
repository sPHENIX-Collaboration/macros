#!/usr/bin/bash

if [ $# -eq 0 ]
then
  echo "No runnumber supplied"
  exit 0
fi

runnumber=$(printf "%08d" $1)

for i in {0..23}
do
ebdc=$(printf "%02d" $i)
ls -1 /sphenix/lustre01/sphnxpro/commissioning/tpc/beam/TPC_ebdc${ebdc}_beam-${runnumber}-* > tpc${ebdc}.list
if [ ! -s tpc${ebdc}.list ]
then
  echo tpc${ebdc}.list empty, removing it
  rm  tpc${ebdc}.list
fi
done
