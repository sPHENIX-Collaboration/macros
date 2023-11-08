#!/usr/bin/bash

# creates file lists from known locations in lustre
# run number is the input argument

if [ $# -eq 0 ]
  then
    echo "Creates needed lists of input files for a given run"
    echo "Usage: make_cosmics.sh <run number>"
    exit 1
fi

sh intt_makecosmics.sh $1
sh tpc_makecosmics.sh $1
sh gl1_makecosmics.sh $1
sh tpot_makecosmics.sh $1
sh mvtx_makecosmics.sh $1
