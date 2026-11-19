#!/usr/bin/env bash

for i in `cat runs.list`
do
  echo root.exe -q calib_t0mean.C\(\"results/$i/calmbdpass2.3_q_$i.root\"\)
  root.exe -b -q calib_t0mean.C\(\"results/$i/calmbdpass2.3_q_$i.root\"\)
  root.exe -b -q make_cdbtree.C\(\"results/$i/mbd_t0corr.calib\"\)
  mv results/$i/mbd_t0corr.root results/$i/mbd_t0corr-${i}.root
done

