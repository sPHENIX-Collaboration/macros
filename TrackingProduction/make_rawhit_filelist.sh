#!/usr/bin/sh

# runs with a production file path, run number, and segment number you want to look at
# for example, sh make_rawhit_filelist.sh /sphenix/lustre01/sphnxpro/production/run2pp/physics/ana464_nocdbtag_v001/ 53877 0

productionpath=$1
run=$2
seg=$3

runnumber=$(printf "%08d" $run)
segment=$(printf "%05d" $seg)

lrun=$(( $run / 100 * 100 ))
hrun=$(( $run / 100 * 100 + 100 ))
lowrun=$(printf "%08d" $lrun)
hirun=$(printf "%08d" $hrun)
ls $productionpath/DST_STREAMING_EVENT*/run_$lowrun\_$hirun/dst/*-$runnumber-$segment.root > filelist.list
