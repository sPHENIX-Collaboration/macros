#!/usr/bin/bash

export TargetDir="$PWD"/combine_out

if [ ! -d ${TargetDir} ]; then
  mkdir ${TargetDir}
fi

while read dir; do # read each line of dirs.txt as dir
  rm $TargetDir/out_"$dir".root
  hadd -k $TargetDir/out_"$dir".root condorout/OutDir*/OUTHIST_*"$dir"*.root
done < runList.txt
