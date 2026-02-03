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

for i in {0..23}
do
ebdc=$(printf "%02d" $i)
/bin/ls -1 /sphenix/lustre01/sphnxpro/physics/tpc/${type}/TPC_ebdc${ebdc}_${type}-${runnumber}-* > tpc${ebdc}.list
if [ ! -s tpc${ebdc}.list ]
then
  echo tpc${ebdc}.list empty, removing it
  rm  tpc${ebdc}.list
fi
done
