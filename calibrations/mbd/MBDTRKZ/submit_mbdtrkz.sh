#!/usr/bin/env bash

trkdst="clus"

# Parse --trkdst; remaining positional args are ${listdir}/${run}.list paths
runlists=()
while [[ $# -gt 0 ]]; do
  case "$1" in
    --trkdst|--trkdst=*)
      if [[ "$1" == *=* ]]; then trkdst="${1#*=}"; shift
      else [[ $# -lt 2 ]] && { echo "ERROR: --trkdst requires an argument" >&2; exit 1; }
           trkdst=$2; shift 2; fi
      [[ "$trkdst" == "clus" || "$trkdst" == "tracks" ]] || \
        { echo "ERROR: --trkdst must be clus or tracks" >&2; exit 1; } ;;
    *) runlists+=("$1"); shift ;;
  esac
done

NEVT_PER_JOB=100000      # nevt to process per list file
EVT_PER_DST=1000         # nevt per dst_trkr_cluster or track
EVT_PER_FIT=100000       # nevt per dst_calofit

NFILES_JOB=$(( NEVT_PER_JOB / EVT_PER_DST ))

for runlist in "${runlists[@]}"
do
  # runlist is ${base_listdir}/${dst_subdir}/${run}.list; go up two levels for base_listdir
  base_listdir=$(dirname "$(dirname "$runlist")")

  for dst in $(shuf -n ${NFILES_JOB} "$runlist")
  do
    echo dst $dst 
    seg=${dst%.root}
    seg=${seg##*-}

    run=${dst%-*}
    run=${run##*-}

    log_base=${run}_${seg}

    seg=$((10#$seg))  # convert to base 10
    run=$((10#$run))

    echo ./submit.sh --log ${log_base} ./prorun_mbdtrkz.sh $run --trkseg $seg --trkdst $trkdst --listdir "$base_listdir"
    ./submit.sh --log ${log_base} ./prorun_mbdtrkz.sh $run --trkseg $seg --trkdst $trkdst --listdir "$base_listdir"
  done
done

