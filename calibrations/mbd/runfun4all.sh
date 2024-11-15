#!/usr/bin/env bash

prdf=$1

nevts=0
if [[ $# -gt 1 ]] 
then
  nevts=$2
fi

# if not interactive, run ROOT in batch mode
if [ ! -z $PS1 ]
then
  BATCH=-b
fi

echo root.exe ${BATCH} -q Fun4All_MBD_Prdf.C\(\"${prdf}\",${nevts}\)
#root.exe ${BATCH} -q Fun4All_MBD_Prdf.C\(\"${prdf}\",${nevts},\"ProdA_2024\"\)
root.exe ${BATCH} -q Fun4All_MBD_Prdf.C\(\"${prdf}\",${nevts}\)


