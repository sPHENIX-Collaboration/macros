#!/usr/bin/env bash
#
# make my own dst_uncal, but broken up into smaller segments to make
# processing faster
#
# probably won't break apart if processing DST_TRIGGERED_EVENT files,
# since those are already broken up
#

prdflist=$1
tmpfile=/tmp/make_dst_uncal_noslew.out

nevts=20000 # number to process in each group

export PASS0DIR=${PWD}/PASS0/66737
echo $PASS0DIR

for fullprdf in $(cat $prdflist)
do
  if [[ "$fullprdf" =~ prdf$ ]]
  then
    prdf=${fullprdf##*/}
    echo  psql -h sphnxdaqdbreplica.sdcc.bnl.gov -Uphnxrc -w daq -c "select filename,firstevent,lastevent from filelist where filename ~ '$prdf';" 
    exec psql -h sphnxdaqdbreplica.sdcc.bnl.gov -Uphnxrc -w daq -c "select filename,firstevent,lastevent from filelist where filename ~ '$prdf';" | grep $prdf > $tmpfile
    firstevt=$(awk '{print $3}' $tmpfile)
    lastevt=$(awk '{print $NF}' $tmpfile)
    let nevt=${lastevt}-${firstevt}+1
    let nsegments=nevt/20000
    let nsegments+=1
    echo $firstevt $lastevt $nevt $nsegments
  else
    prdf=${fullprdf##*/}
    let nsegments=1
  fi

  let seg=0
  while [[ $seg -lt $nsegments ]]
  do
    let nskip=$seg*20000
    dir=${prdf%.prdf}
    dir=${dir%.root}
    dir=${dir}-$(printf "%04d" $seg)
    mkdir -p ${dir}
    cd ${dir}
    ln -sf ../make_dst_uncal_noslew.cmd .
    ln -sf ../run_mbdcalpass.sh .
    ln -sf ../Fun4All_MBD_CalPass.C .
    [[ -f $fullprdf ]] && ln -sf $fullprdf .
    ../submit.sh make_dst_uncal_noslew.cmd $prdf $nevts $nskip
    let seg+=1
    cd ..
  done
done

