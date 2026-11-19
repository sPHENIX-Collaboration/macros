#!/usr/bin/env bash

infile=$1

nevts=0
nskip=0
if [[ $# -gt 1 ]] 
then
  nevts=$2
fi
if [[ $# -gt 2 ]] 
then
  nskip=$3
fi

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

if [[ "$infile" =~ prdf$ ]]
then

  echo root.exe ${BATCH} -q Fun4All_MBD_Prdf.C\(\"${infile}\",${nevts},${nskip}\)
  root.exe ${BATCH} -q Fun4All_MBD_Prdf.C\(\"${infile}\",${nevts},${nskip}\)
  #root.exe ${BATCH} -q Fun4All_MBD_Prdf.C\(\"${infile}\",${nevts},${nskip},\"newcdbtag\"\)

elif [[ "$infile" =~ root$ ]]
then

  echo root.exe ${BATCH} -q Fun4All_MBD_Year2.C\(\"${infile}\",${nevts},${nskip}\)
  root.exe ${BATCH} -q Fun4All_MBD_Year2.C\(\"${infile}\",${nevts},${nskip}\)

fi


