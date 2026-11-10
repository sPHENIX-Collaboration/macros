#!/bin/bash
# one condor job of pass 2: per-run plots of a chunk of runs
# usage: draw_chunk.sh <run file chunk> <cdbstatus.txt> <plot dir> <period>
CHUNK=$1; STATUS=$2; PLOTS=$3; PERIOD=$4
MACROS=$(cd "$(dirname "$0")"/../macros && pwd)
source /opt/sphenix/core/bin/sphenix_setup.sh -n ${CDBQA_RELEASE:-new} > /dev/null 2>&1
cd ${_CONDOR_SCRATCH_DIR:-/tmp}
root.exe -l -b -q "$MACROS/drawCDBCaloQA.C(\"$CHUNK\", \"$STATUS\", \"$PLOTS\", \"$PERIOD\", true, false)"
