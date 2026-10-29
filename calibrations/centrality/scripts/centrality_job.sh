#!/bin/bash
# One step of the Au+Au centrality calibration (condor job of make_dag.sh, or by hand). Every step reads the period
# configuration (periods/<period>.sh) and the work directories:
#   CENT_TREEDIR  per-run trees (large: use a bulk disk)
#   CENT_WORKDIR  t0 offsets, calibrations, payloads
#   CENT_INSTALL  optional local install of calibrations/centrality (setup_local.sh), until it is in the release
#
#   centrality_job.sh <period> tree <run> <dst list> <tag>   trees of the DST segments of <dst list> (condor scratch)
#   centrality_job.sh <period> hadd <run>                     per-run tree, then its t0 offset fit
#   centrality_job.sh <period> table                          t0 offset table of every run of the run list
#   centrality_job.sh <period> calib <run> <refrun>           calibration of a run (refrun = run: the reference run)
#   centrality_job.sh <period> payloads                       CDB payload files of every run of the run list
export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${USER}

HERE=$(cd "$(dirname "$(readlink -f "$0")")"/.. && pwd)
PERIOD=$1
STEP=$2
shift 2
source ${HERE}/periods/${PERIOD}.sh || exit 1
: "${CENT_TREEDIR:?}" "${CENT_WORKDIR:?}"
source /opt/sphenix/core/bin/sphenix_setup.sh -n ${BUILD}
[[ -n ${CENT_INSTALL} ]] && source /opt/sphenix/core/bin/setup_local.sh ${CENT_INSTALL}
M=${HERE}/macros
TABLE=${CENT_WORKDIR}/t0_offsets.csv
CALIBDIR=${CENT_WORKDIR}/calib
echo "$(hostname): ${PERIOD} ${STEP} $* (${BUILD}, ${CENT_INSTALL:-release})"

case ${STEP} in
  tree)
    RUN=$1
    LIST=$2
    TAG=$3
    cd "${_CONDOR_SCRATCH_DIR:?not a condor job}" || exit 1
    # stage the segments; one that is no longer retrievable is skipped
    : > segments.list
    while read -r calo zdc; do
      if getinputfiles.pl ${calo} && getinputfiles.pl ${zdc}; then
        echo "${calo} ${zdc}" >> segments.list
      else
        echo "skipping segment ${calo} ${zdc}: not retrievable"
      fi
    done < ${LIST}
    [[ -s segments.list ]] || { echo "no retrievable segments in ${LIST}"; exit 0; }
    root -b -q "${M}/Fun4All_CentralityTree.C(\"segments.list\", \"tree.root\", 0, \"${CDBTAG}\")" || exit 1
    mkdir -p ${CENT_TREEDIR}/segments/${RUN}
    cp tree.root ${CENT_TREEDIR}/segments/${RUN}/tree_${TAG}.root.part &&
      mv ${CENT_TREEDIR}/segments/${RUN}/tree_${TAG}.root.part ${CENT_TREEDIR}/segments/${RUN}/tree_${TAG}.root || exit 1
    ;;
  hadd)
    RUN=$1
    SEGS=$(ls ${CENT_TREEDIR}/segments/${RUN}/tree_*.root 2> /dev/null)
    if [[ -n ${SEGS} ]]; then
      hadd -f -k ${CENT_TREEDIR}/run${RUN}.root ${SEGS} || exit 1
      rm -r ${CENT_TREEDIR}/segments/${RUN}   # the per-run tree replaces the segments
    fi
    [[ -s ${CENT_TREEDIR}/run${RUN}.root ]] || { echo "no tree for run ${RUN}"; exit 1; }
    root -b -q "${M}/fit_t0offset.C(${RUN}, \"${CENT_TREEDIR}/run${RUN}.root\", \"${CENT_WORKDIR}/t0offset\", ${TRIGGER_BIT}, ${T0_SEED_WINDOW})" || exit 1
    ;;
  table)
    # the header of CentralityCalibrator::T0TableHeader(); a run without a fit gets status 9 (no usable offset)
    echo "run,nsel,offset,offset_err,sigma,chi2ndf,m5_rel,m5_rel_err,p5_rel,p5_rel_err,status" > ${TABLE}.part
    while read -r run ref calibrate; do
      f=${CENT_WORKDIR}/t0offset/t0offset_${run}.csv
      if [[ -s ${f} ]]; then cat ${f}; else echo "${run},0,0,0,0,-1,-99,0,-99,0,9"; fi
    done < ${HERE}/${RUNLIST} >> ${TABLE}.part
    mv ${TABLE}.part ${TABLE}
    awk -F, 'NR > 1 { n[$11]++ } END { for (s in n) print "  t0 status " s ": " n[s] " runs" }' ${TABLE}
    ;;
  calib)
    RUN=$1
    REF=$2
    REFFILE=""
    [[ ${RUN} != "${REF}" ]] && REFFILE=${CALIBDIR}/centrality_calib_${REF}.root
    root -b -q "${M}/calibrate_run.C(${RUN}, \"${CENT_TREEDIR}/run${RUN}.root\", \"${TABLE}\", \"${REFFILE}\", \"${CALIBDIR}/centrality_calib_${RUN}.root\", ${TRIGGER_BIT}, \"${HERE}/${GLAUBER}\", ${NDIVISIONS})" || exit 1
    ;;
  payloads)
    root -b -q "${M}/make_payloads.C(\"${HERE}/${RUNLIST}\", \"${CALIBDIR}\", \"${TABLE}\", \"${CENT_WORKDIR}/payloads\")" || exit 1
    ;;
  *)
    echo "unknown step ${STEP}"
    exit 1
    ;;
esac
