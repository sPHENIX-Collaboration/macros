#!/bin/bash
# Pass 2 of CDBCaloQA for one running period and global tag: CDB status, plots (PDF), HTML pages.
# usage: run_pass2.sh <period: Run24pp, Run24AuAu, Run25OO, Run25pp, Run25AuAu> [global tag, default newcdbtag]
#   e.g. run_pass2.sh Run25OO newcdbtag
PERIOD=$1; GTAG=${2:-newcdbtag}
QABASE=${CDBQA_BASE:-/sphenix/user/$USER/cdbqa}
CODE=$(cd "$(dirname "$0")"/.. && pwd)
# other places for a test (default: the real ones)
WEB=${CDBQA_WEB:-/sphenix/WWW/subsystem/CDBCaloQA}
HISTS=${CDBQA_HISTS:-$QABASE/hists/$PERIOD/$GTAG}
WORK=${CDBQA_WORK:-$QABASE/pass2/$PERIOD/$GTAG}
RUNLIST=${CDBQA_RUNLIST:-$CODE/runlists/$PERIOD.txt}
JOBLOG=${CDBQA_JOBLOG:-$QABASE/condor/log/cdbcaloqa_${PERIOD}_${GTAG}}_{run}.out
GTJSON=${CDBQA_GTJSON:-$QABASE/cdb/gt_$GTAG.json}
source /opt/sphenix/core/bin/sphenix_setup.sh -n ${CDBQA_RELEASE:-new} > /dev/null 2>&1

# page title of the running period (the period is also the web folder)
case $PERIOD in
  Run24pp)   TITLE="Run 2024 p+p" ;;
  Run24AuAu) TITLE="Run 2024 Au+Au" ;;
  Run25OO)   TITLE="Run 2025 O+O" ;;
  Run25pp)   TITLE="Run 2025 p+p" ;;
  Run25AuAu) TITLE="Run 2025 Au+Au" ;;
  MC_pp)     TITLE="MC p+p PYTHIA8 Detroit" ;;
  MC_AuAu)   TITLE="MC Au+Au HIJING 0-20 fm" ;;
  MC_OO)     TITLE="MC O+O HIJING 0-15 fm" ;;
  *)         echo "period $PERIOD not set up (Run24pp, Run24AuAu, Run25OO, Run25pp, Run25AuAu, MC_pp, MC_AuAu, MC_OO)"; exit 1 ;;
esac
# the global tag must exist in the CDB
curl -s -m 60 http://sphenix-cdb.apps.sdcc.bnl.gov/api/cdb_rest/globalTags \
  | python3 -c "import json,sys; sys.exit(0 if sys.argv[1] in [t['name'] for t in json.load(sys.stdin)] else 1)" "$GTAG" \
  || { echo "global tag $GTAG not found in the CDB"; exit 1; }

# one directory per period and global tag: histograms, pass 2 files; web pages per period and tag
[ -s $RUNLIST ] || { echo "no run list $RUNLIST"; exit 1; }
# no histograms at all (jobs not run yet): stop before any file or page is written
NHIST=0
while read -r RUN; do
  ls $HISTS/HIST_CDBCALOQA_*-$(printf %08d $RUN).root > /dev/null 2>&1 && NHIST=$((NHIST + 1))
done < $RUNLIST
if [ $NHIST -eq 0 ]; then
  echo "no histograms in $HISTS: run make_qa_dag.sh $PERIOD $GTAG first"
  exit 1
fi

OUT=$WEB/$PERIOD/$GTAG
# global tag snapshot of today; stop before any page is touched if the download fails
mkdir -p $(dirname $GTJSON)
curl -sf -m 300 http://sphenix-cdb.apps.sdcc.bnl.gov/api/cdb_rest/globalTag/$GTAG -o $GTJSON.tmp \
  && python3 -c "import json,sys; json.load(open(sys.argv[1]))" $GTJSON.tmp \
  && mv $GTJSON.tmp $GTJSON \
  || { echo "cannot download global tag $GTAG"; rm -f $GTJSON.tmp; exit 1; }
# every pass 2 rewrites the whole tag page: nothing left over from an earlier run list
rm -rf "${OUT:?}"
mkdir -p $WORK $OUT/plots
# golden run list -> per-run histogram file, "-" for runs without output (job failed)
: > $WORK/runfiles.txt
while read -r RUN; do
  F=$(ls $HISTS/HIST_CDBCALOQA_*-$(printf %08d $RUN).root 2> /dev/null | head -1)
  echo "$RUN ${F:--}" >> $WORK/runfiles.txt
done < $RUNLIST
echo "$(grep -vc ' -$' $WORK/runfiles.txt) of $(wc -l < $WORK/runfiles.txt) runs with histograms"

# per-run plots in 30 condor jobs (one chunk of runs each), submitted now: they do not need the CDB status
# (only the summary does), which is made while they run; the summary vs run is drawn here after the jobs
rm -f "${WORK:?}"/runfiles_chunk_* "${WORK:?}"/draw_*
split -n l/30 -d $WORK/runfiles.txt $WORK/runfiles_chunk_
ls $WORK/runfiles_chunk_* > $WORK/draw_chunks.list
DRAWLOG=/tmp/$USER/draw_${PERIOD}_${GTAG}.log
mkdir -p /tmp/$USER
rm -f "$DRAWLOG"
cat > $WORK/draw_chunk.sub << SUB
Universe            = vanilla
Executable          = $CODE/scripts/draw_chunk.sh
transfer_executable = false
Arguments           = \$(chunk) $WORK/cdbstatus.txt $OUT/plots $PERIOD
Output              = $WORK/draw_\$(Process).log
Error               = $WORK/draw_\$(Process).err
Log                 = $DRAWLOG
environment         = "CDBQA_RELEASE=${CDBQA_RELEASE:-new}"
request_memory      = 2GB
Queue chunk from $WORK/draw_chunks.list
SUB
condor_submit $WORK/draw_chunk.sub | tail -1

# CDB status per run
python3 $CODE/scripts/cdbstatus.py $WORK/runfiles.txt $GTJSON $OUT/cdb $WORK/cdbstatus.txt "$JOBLOG"

# summary vs run and CDB map here while the per-run jobs run; then wait for them (the fingerprint needs their plots)
root.exe -l -b -q "$CODE/macros/drawCDBCaloQA.C(\"$WORK/runfiles.txt\", \"$WORK/cdbstatus.txt\", \"$OUT/plots\", \"$PERIOD\", false, true)" > $WORK/draw_summary.log 2>&1
condor_wait -wait 7200 $DRAWLOG || echo "draw jobs not all done after 2 h: the per-run plots of the missing chunks are not on the pages"
echo "draw: $(cat $WORK/draw_*.log $WORK/draw_*.err | grep -c 'has been created') PDFs"
# ZSCrossCalib maps vs run, from the per-ieta means in the per-run objects (needs the per-run plots)
root.exe -l -b -q "$CODE/macros/drawCDBCaloQA.C(\"$WORK/runfiles.txt\", \"$WORK/cdbstatus.txt\", \"$OUT/plots\", \"$PERIOD\", false, false, true)" > $WORK/draw_zsmap.log 2>&1

# fingerprint of the plots of this tag: any two tags of the period are compared live on compare.html
python3 $CODE/scripts/fingerprint_tag.py $WEB $PERIOD $GTAG
# payload list of the tag (file md5 and insert time per payload type and run) for the CDB map on compare.html
python3 $CODE/scripts/payload_list.py $GTJSON $RUNLIST $OUT/plots/payloads.json

# events and empty plots per run, for the flags on the pages (interpreted: no shared library to build when tags run at once)
root.exe -l -b -q "$CODE/macros/scan_empty.C(\"$WORK/runfiles.txt\", \"$OUT/plots/runs\", \"$WORK/empty_scan.txt\")" > $WORK/scan_empty.log 2>&1
CDBQA_JOBLOG_PATTERN="$JOBLOG" python3 $CODE/scripts/make_html.py $WEB $PERIOD "$TITLE" $WORK/runfiles.txt $WORK/cdbstatus.txt $OUT/cdb $GTAG

