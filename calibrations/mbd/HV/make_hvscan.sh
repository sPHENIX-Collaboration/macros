#!/usr/bin/env bash
#
# usage: make_hvscan.sh <hvscan.list>
#

hvscanlist=$1

dir=${hvscanlist#hvscan.}
mkdir -p ${dir}

cat $hvscanlist | while read runnum hv
do
  echo ./run_mbdlaser.sh laser/*${runnum}*
  ./run_mbdlaser.sh laser/*${runnum}*
  mv DST_UNCALMBD-*${runnum}*.root ${dir}
done

