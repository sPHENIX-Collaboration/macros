#!/bin/bash
# Pass 1 and pass 2 of CDBCaloQA in one condor DAG: one node per run (pass 1), then pass 2 (CDB status, plots, HTML)
# once every run is finished. DAGMan follows the jobs in their log file (no condor_q).
#   a crashed or held job is run once more; a run that fails twice goes to failed_pass1.txt and pass 2 still runs
# usage: make_qa_dag.sh <period> <global tag> [--runs <run list>] [--test <dir>]
#   --runs: runs to do (default runlists/<period>.txt; Run25pp, Run25AuAu: all runs with DST_CALOFITTING, from CreateDstList.pl)
#   --test: histograms, logs, pass 2 files and pages in <dir> (the real ones are not touched)
# environment: CDBQA_BASE (work area), CDBQA_WEB (pages), CDBQA_RELEASE, CDBQA_INSTALL (local build, optional)
# progress: <dag dir>/qa.dag.dagman.out, failed runs: <dag dir>/failed_pass1.txt
PERIOD=$1; GTAG=$2; shift 2
QABASE=${CDBQA_BASE:?set CDBQA_BASE, the work area (lists, hists, pass2, condor logs)}
CODE=$(cd "$(dirname "$0")"/.. && pwd)
RUNLIST=$CODE/runlists/$PERIOD.txt; TEST=""
while [ $# -gt 0 ]; do
  case $1 in
    --runs) RUNLIST=$(readlink -f $2); shift 2 ;;
    --test) TEST=$(readlink -f $2); shift 2 ;;
    *) echo "unknown option $1"; exit 1 ;;
  esac
done
[ -n "$PERIOD" ] && [ -n "$GTAG" ] || { echo "usage: make_qa_dag.sh <period> <global tag> [--runs <run list>] [--test <dir>]"; exit 1; }
if [ -n "$TEST" ]; then
  HISTS=$TEST/hists/$PERIOD/$GTAG; LOGDIR=$TEST/log; WORK=$TEST/pass2/$PERIOD/$GTAG
  WEB=$TEST/web; GTJSON=$TEST/gt_$GTAG.json; D=$TEST/dag/${PERIOD}_$GTAG
else
  HISTS=$QABASE/hists/$PERIOD/$GTAG; LOGDIR=$QABASE/condor/log; WORK=$QABASE/pass2/$PERIOD/$GTAG
  WEB=${CDBQA_WEB:-/sphenix/WWW/subsystem/CDBCaloQA}; GTJSON=$QABASE/cdb/gt_$GTAG.json; D=$QABASE/condor/dag/${PERIOD}_$GTAG
fi
NAME=cdbqa_${PERIOD}_$GTAG
JOBLOG=/tmp/$USER/$NAME.log
rm -rf "${D:?}"
mkdir -p $D $HISTS $LOGDIR $WEB $(dirname $GTJSON) /tmp/$USER
if [ ! -s "$RUNLIST" ]; then
  case $PERIOD in
    Run25pp) DATASET=run3pp ;;
    Run25AuAu) DATASET=run3auau ;;
    *) echo "no run list $RUNLIST"; exit 1 ;;
  esac
  source /opt/sphenix/core/bin/sphenix_setup.sh -n ${CDBQA_RELEASE:-new} > /dev/null 2>&1
  RUNLIST=$D/runs.txt
  CreateDstList.pl --printruns --dataset $DATASET --tag pro001_pcdb001_v001 DST_CALOFITTING > $RUNLIST
  [ -s $RUNLIST ] || { echo "no runs from CreateDstList.pl for $DATASET"; exit 1; }
fi

# pass 1: one run per node; a held job is removed, so DAGMan sees a failure and runs it again
cat > $D/pass1.sub << SUB
Universe        = vanilla
Executable      = $CODE/scripts/run_cdbcaloqa.sh
transfer_executable = false
Arguments       = $PERIOD \$(run) $GTAG $HISTS
Initialdir      = $QABASE
Output          = $LOGDIR/cdbcaloqa_${PERIOD}_${GTAG}_\$(run).out
Error           = $LOGDIR/cdbcaloqa_${PERIOD}_${GTAG}_\$(run).err
Log             = $JOBLOG
environment     = "CDBQA_BASE=$QABASE CDBQA_LISTS=$CODE/runlists CDBQA_RELEASE=${CDBQA_RELEASE:-new} CDBQA_INSTALL=$CDBQA_INSTALL"
request_memory  = 2GB
periodic_remove = (JobStatus == 5)
Queue
SUB

# after each pass-1 job: failed on the first try -> node fails, DAGMan runs it again;
# failed on the last try -> run listed in failed_pass1.txt, node counted as done so pass 2 still runs
cat > $D/post.sh << SH
#!/bin/bash
NODE=\$1; RET=\$2; RETRY=\$3; MAXRETRIES=\$4
[[ \$RET == 0 ]] && exit 0
if [[ \$RETRY -lt \$MAXRETRIES ]]; then exit "\$RET"; fi
echo "\${NODE#P1_} exit \$RET after \$((RETRY + 1)) tries" >> $D/failed_pass1.txt
exit 0
SH
chmod +x $D/post.sh

# pass 2 on the submit machine (local universe): it submits its own drawing jobs
cat > $D/pass2.sub << SUB
Universe       = local
Executable     = /bin/bash
Arguments      = $CODE/scripts/run_pass2.sh $PERIOD $GTAG
Initialdir     = $QABASE
Output         = $D/pass2.out
Error          = $D/pass2.err
Log            = $JOBLOG
environment    = "PATH=/usr/bin:/bin HOME=$HOME USER=$USER CDBQA_BASE=$QABASE CDBQA_RELEASE=${CDBQA_RELEASE:-new} CDBQA_WEB=$WEB CDBQA_HISTS=$HISTS CDBQA_WORK=$WORK CDBQA_RUNLIST=$RUNLIST CDBQA_JOBLOG=$LOGDIR/cdbcaloqa_${PERIOD}_$GTAG CDBQA_GTJSON=$GTJSON"
request_memory = 2GB
Queue
SUB

# DAGMan reads the job log every minute, no limit on submitted or idle jobs
cat > $D/dagman.config << CFG
DAGMAN_USER_LOG_SCAN_INTERVAL = 60
DAGMAN_MAX_SUBMITS_PER_INTERVAL = 5000
DAGMAN_MAX_JOBS_IDLE = 0
CFG

{
  for r in $(cat $RUNLIST); do
    echo "JOB P1_$r $D/pass1.sub"
    echo "VARS P1_$r run=\"$r\""
    echo "RETRY P1_$r 1"
    echo "SCRIPT POST P1_$r $D/post.sh \$JOB \$RETURN \$RETRY \$MAX_RETRIES"
  done
  echo "JOB P2 $D/pass2.sub"
  echo "PARENT $(sed 's/^/P1_/' $RUNLIST | tr '\n' ' ')CHILD P2"
  echo "CONFIG $D/dagman.config"
} > $D/qa.dag
echo "$NAME: $(wc -l < $RUNLIST) runs, DAG $D/qa.dag, pages $WEB/$PERIOD/$GTAG"

# the DAGMan log must be in /tmp (site rule)
cd $D
rm -f "/tmp/$USER/$NAME.dagman.log" "$JOBLOG"
condor_submit_dag -f -no_submit -batch-name $NAME qa.dag > /dev/null || exit 1
condor_submit -append "log = /tmp/$USER/$NAME.dagman.log" qa.dag.condor.sub | tail -1
