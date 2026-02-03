#!/bin/bash

flist=$1
runlimit=0	# must have at least this many files in run

#sed 's/^.*0000//' $flist | sed 's/-.*$//' | sort | uniq > runs.list

awk -F - '{print $2}' $flist | sed 's/^00*//' | sort | uniq > runs.list

for run in `cat runs.list`
do
  #grep "0000$run" $flist | sort -t/ -k 5  > ${run}.list
  grep "00$run" $flist | sort > ${run}.list
  files=`wc -l ${run}.list | awk '{print $1}'`

  if [ $files -lt $runlimit ]
  then
    rm ${run}.list
  fi

done
