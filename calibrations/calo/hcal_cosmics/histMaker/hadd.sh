#!/usr/bin/bash

export TargetDir="$PWD"/combine_out

if [ ! -d ${TargetDir} ]; then
  mkdir ${TargetDir}
fi

rm $TargetDir/ohcal_sum.root
rm $TargetDir/ihcal_sum.root

#while read dir; do 
#  rm $TargetDir/ihcal_hist_"$dir".root
#  hadd -k $TargetDir/ihcal_hist_"$dir".root condorout/OutDir*/ihcal_h*"$dir"*.root
#  rm $TargetDir/ohcal_hist_"$dir".root
#  hadd -k $TargetDir/ohcal_hist_"$dir".root condorout/OutDir*/ohcal_h*"$dir"*.root
#done < runlist_clock.txt 

while read dir; do 
  cp $TargetDir/ihcal_hist_"$dir".root $TargetDir/ihcal_sum.root
  cp $TargetDir/ohcal_hist_"$dir".root $TargetDir/ohcal_sum.root
  break
done < runlist_clock.txt 

while read dir; do 
  hadd -a -k $TargetDir/ihcal_sum.root $TargetDir/ihcal_hist_"$dir".root
  hadd -a -k $TargetDir/ohcal_sum.root $TargetDir/ohcal_hist_"$dir".root
done < runlist_clock.txt 

