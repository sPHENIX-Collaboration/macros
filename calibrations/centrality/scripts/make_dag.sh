#!/bin/bash
# Writes the condor DAG that makes a period's centrality calibration from the DSTs, in $CENT_WORKDIR/dag:
#   T<run>    tree jobs, SEGMENTS_PER_JOB DST segments each      H<run>    per-run tree and its t0 offset fit
#   TABLE     t0 offset table, after every H                     C<ref>    the reference runs, then
#   C<run>    every calibrated run against its reference         PAYLOADS  CDB payload files of every run
#
#   CENT_TREEDIR=<bulk disk> CENT_WORKDIR=<dir> [CENT_INSTALL=<local install>] make_dag.sh <period>
# and submit it with the command printed at the end.
HERE=$(cd "$(dirname "$(readlink -f "$0")")"/.. && pwd)
PERIOD=$1
source ${HERE}/periods/${PERIOD}.sh || exit 1
: "${CENT_TREEDIR:?}" "${CENT_WORKDIR:?}"
D=${CENT_WORKDIR}/dag
mkdir -p ${D}/dstlists ${CENT_WORKDIR}/log /tmp/${USER}

# DST_CALOFITTING + DST_ZDC_RAW pairs of every segment with their sizes (for the disk requests)
RUNS=$(awk '{ print $1 }' ${HERE}/${RUNLIST} | paste -sd,)
psql -h sphnxdbmaster -d FileCatalog -t -A -F' ' -c "select c.runnumber, c.filename, z.filename,
    (select max(size) from files where lfn = c.filename), (select max(size) from files where lfn = z.filename)
  from datasets c
  join datasets z on z.runnumber = c.runnumber and z.segment = c.segment and z.dsttype = 'DST_ZDC_RAW' and z.tag = '${DSTTAG}' and z.dataset = '${DATASET}'
  where c.dsttype = 'DST_CALOFITTING' and c.tag = '${DSTTAG}' and c.dataset = '${DATASET}' and c.runnumber in (${RUNS})
  order by c.runnumber, c.segment" > ${D}/segments.txt || exit 1
# per run: dstlists/<run>/<tag>.list with SEGMENTS_PER_JOB pairs, and dstlists/<run>.queue with one
# "list tag disk" line per job; disk (kB) = the job's DSTs + 10% + 1 GB for the tree
find ${D}/dstlists -mindepth 1 -delete
awk -v d=${D}/dstlists -v n=${SEGMENTS_PER_JOB} '
  {
    if ($1 != run) { run = $1; k = 0; system("mkdir -p " d "/" run) }
    tag = sprintf("%04d", int(k / n)); list = d "/" run "/" tag ".list"
    print $2, $3 >> list; close(list)
    disk[run, tag] += $4 + $5
    if (k % n == 0) order[++njob] = run SUBSEP tag
    k++
  }
  END {
    for (j = 1; j <= njob; j++) {
      split(order[j], a, SUBSEP)
      q = d "/" a[1] ".queue"
      printf "%s/%s/%s.list %s %d\n", d, a[1], a[2], a[2], disk[a[1], a[2]] * 1.10 / 1024 + 1048576 >> q; close(q)
    }
  }' ${D}/segments.txt

# every reference run needs its tree: without DST segments it would get no tree job, and its calibration would fail or
# read an old tree
missing=""
for ref in $(awk '{ print $2 }' ${HERE}/${RUNLIST} | sort -u); do
  [[ -s ${D}/dstlists/${ref}.queue ]] || missing="${missing} ${ref}"
done
if [[ -n ${missing} ]]; then
  echo "reference run(s) without DST segments:${missing}; no DAG written" >&2
  exit 1
fi

# submit files
header()
{
  cat << EOF
Universe           = vanilla
Executable         = ${HERE}/scripts/centrality_job.sh
Environment        = "CENT_TREEDIR=${CENT_TREEDIR} CENT_WORKDIR=${CENT_WORKDIR} CENT_INSTALL=${CENT_INSTALL}"
notification       = Never
Log                = /tmp/${USER}/centrality_${PERIOD}.log
PeriodicHold       = (NumJobStarts >= 1 && JobStatus == 1 && !(ON_EVICT_CHECK_RequestMemory_REQUIREMENTS))
job_lease_duration = 3600
EOF
}
{ header; cat << EOF; } > ${D}/tree.sub
Arguments          = ${PERIOD} tree \$(run) \$(list) \$(tag)
Output             = ${CENT_WORKDIR}/log/tree_\$(run)_\$(tag).out
Error              = ${CENT_WORKDIR}/log/tree_\$(run)_\$(tag).err
request_memory     = ${TREE_MEMORY}MB
request_disk       = \$(disk)
Queue list, tag, disk from ${D}/dstlists/\$(run).queue
EOF
{ header; cat << EOF; } > ${D}/hadd.sub
Arguments          = ${PERIOD} hadd \$(run)
Output             = ${CENT_WORKDIR}/log/hadd_\$(run).out
Error              = ${CENT_WORKDIR}/log/hadd_\$(run).err
request_memory     = 2000MB
request_disk       = 1GB
Queue
EOF
{ header; cat << EOF; } > ${D}/calib.sub
Arguments          = ${PERIOD} calib \$(run) \$(ref)
Output             = ${CENT_WORKDIR}/log/calib_\$(run).out
Error              = ${CENT_WORKDIR}/log/calib_\$(run).err
request_memory     = ${CALIB_MEMORY}MB
request_disk       = 1GB
Queue
EOF
for step in table payloads; do
  { header; cat << EOF; } > ${D}/${step}.sub
Arguments          = ${PERIOD} ${step}
Output             = ${CENT_WORKDIR}/log/${step}.out
Error              = ${CENT_WORKDIR}/log/${step}.err
request_memory     = 1000MB
request_disk       = 1GB
Queue
EOF
done

# the DAG
DAG=${D}/centrality_${PERIOD}.dag
{
  echo "# ${PERIOD} centrality calibration, written by make_dag.sh"
  hadds=""
  while read -r run ref calibrate; do
    if [[ ! -s ${D}/dstlists/${run}.queue ]]; then
      echo "# ${run}: no DST segments"
      continue
    fi
    echo "JOB T${run} ${D}/tree.sub"
    echo "VARS T${run} run=\"${run}\""
    echo "RETRY T${run} 2"
    echo "JOB H${run} ${D}/hadd.sub"
    echo "VARS H${run} run=\"${run}\""
    echo "RETRY H${run} 1"
    echo "PARENT T${run} CHILD H${run}"
    hadds="${hadds} H${run}"
  done < ${HERE}/${RUNLIST}
  echo "JOB TABLE ${D}/table.sub"
  echo "PARENT${hadds} CHILD TABLE"
  calibs=""
  for ref in $(awk '{ print $2 }' ${HERE}/${RUNLIST} | sort -u); do
    echo "JOB C${ref} ${D}/calib.sub"
    echo "VARS C${ref} run=\"${ref}\" ref=\"${ref}\""
    echo "RETRY C${ref} 1"
    echo "PARENT TABLE CHILD C${ref}"
    calibs="${calibs} C${ref}"
  done
  while read -r run ref calibrate; do
    [[ ${calibrate} == 1 && ${run} != "${ref}" && -s ${D}/dstlists/${run}.queue ]] || continue
    echo "JOB C${run} ${D}/calib.sub"
    echo "VARS C${run} run=\"${run}\" ref=\"${ref}\""
    echo "RETRY C${run} 1"
    echo "PARENT C${ref} CHILD C${run}"
    calibs="${calibs} C${run}"
  done < ${HERE}/${RUNLIST}
  echo "JOB PAYLOADS ${D}/payloads.sub"
  echo "PARENT${calibs} CHILD PAYLOADS"
} > ${DAG}

echo "${DAG}: $(grep -c '^JOB T[0-9]' ${DAG}) runs, $(cat ${D}/dstlists/*.queue | wc -l) tree jobs, $(grep -c '^JOB C' ${DAG}) calibrations"
echo "submit (DAGMan's own log must be under /tmp):"
echo "  condor_submit_dag -no_submit -batch-name centrality_${PERIOD} ${DAG} && condor_submit -append \"log = /tmp/${USER}/centrality_${PERIOD}.dagman.log\" ${DAG}.condor.sub"
