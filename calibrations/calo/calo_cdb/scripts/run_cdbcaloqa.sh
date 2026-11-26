#!/bin/bash
# one condor job: CDB QA of one run
# usage: run_cdbcaloqa.sh <period> <run> <global tag> <histogram dir>
PERIOD=$1; RUN=$2; GTAG=$3; H=$4
MACROS=$(cd "$(dirname "$0")"/../macros && pwd)
source /opt/sphenix/core/bin/sphenix_setup.sh -n ${CDBQA_RELEASE:-new}
[ -n "$CDBQA_INSTALL" ] && source /opt/sphenix/core/bin/setup_local.sh $CDBQA_INSTALL
mkdir -p $H
rm -f $H/HIST_CDBCALOQA_*-$(printf %08d $RUN).root
cd ${_CONDOR_SCRATCH_DIR:-/tmp}
root.exe -l -b -q "$MACROS/Fun4All_CDBCaloQA.C(\"$PERIOD\", $RUN, \"$GTAG\")"
F=$(ls HIST_CDBCALOQA_*-$(printf %08d $RUN).root 2> /dev/null)
[ -n "$F" ] && cp $F $H/
