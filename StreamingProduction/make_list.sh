#!/usr/bin/bash

# creates file lists from known locations in lustre
# run number is the input argument

if [ $# -eq 0 ]
  then
    echo "Creates needed lists of input files for a given run"
    echo "Usage: make_lists.sh <type> <run number> "
    exit 1
fi

if [ $# -eq 1 ]
then
  echo "No type or runnumber supplied"
  exit 0
fi

sh gl1_makelist.sh $1 $2
sh mvtx_makelist.sh $1 $2
sh intt_makelist.sh $1 $2
sh tpc_makelist.sh $1 $2
sh tpot_makelist.sh $1 $2
