#!/usr/bin/bash

export TargetDir="$PWD"/combine_out

if [ ! -d ${TargetDir} ]; then
  mkdir ${TargetDir}
fi

while read dir; do # read each line of dirs.txt as dir
  rm $TargetDir/out_"$dir".root
  hadd -k $TargetDir/out_"$dir".root condorout/OutDir*/CALOHIST_*"$dir"*.root
  rm $TargetDir/tsc_hcalout_"$dir".root
  hadd -k $TargetDir/tsc_hcalout_"$dir".root condorout/OutDir*/tsc_hcalout_"$dir".root
  rm $TargetDir/tsc_hcalin_"$dir".root
  hadd -k $TargetDir/tsc_hcalin_"$dir".root condorout/OutDir*/tsc_hcalin_"$dir".root
done < runList.txt




rm $TargetDir/tsc_hcalout_"$dir".root
rm $TargetDir/tsc_hcalin_"$dir".root

i=0

while read dir; do 
  if [ "$i" -le 0 ]; then
    cp  $TargetDir/tsc_hcalout_"$dir".root $TargetDir/tsc_hcalout_comb.root
    cp  $TargetDir/tsc_hcalin_"$dir".root  $TargetDir/tsc_hcalin_comb.root
  else
    hadd -a $TargetDir/tsc_hcalout_comb.root $TargetDir/tsc_hcalout_"$dir".root 
    hadd -a $TargetDir/tsc_hcalin_comb.root $TargetDir/tsc_hcalin_"$dir".root  

  fi
  i=$((i+1))
done < runList.txt

