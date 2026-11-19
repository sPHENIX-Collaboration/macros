#!/bin/bash
#
# prorun_mbdtrkz.sh <runnumber> [--nevents N] [-n fun4all_nevents]
#                               [--trkdst clus|tracks] [--listdir DIR]
#
# Creates MBDTRKZ/<runnumber>/<zero-padded trkseg>/, generates seed/clus/fit lists there,
# and runs Fun4All_MBD_TrackVertex.C (tracks mode) or Fun4All_MBD_TrackFitting.C
# once per clus file (clus mode).
#
#   --trkseg S           segment of track dst (default 0)
#   --nevents N          events per block passed to make_mbdtrkz_lists.py (default 100000)
#   -n fun4all_n         nEvents passed to Fun4All (default 0 = all)
#   --trkdst clus|tracks DST type (default: tracks)
#   --listdir DIR        base list directory passed to make_mbdtrkz_lists.py
#
# Under condor: stages files to _CONDOR_SCRATCH_DIR, runs there, copies
# output back to the MBDTRKZ subdir.
#

usage() {
  #echo "Usage: $0 <runnumber> [--skip S] [--nevents N] [-n fun4all_nevents] [--trkdst clus|tracks] [--listdir DIR]" >&2
  echo "Usage: $0 <runnumber> [--nevents N] [-n fun4all_nevents] [--trkdst clus|tracks] [--listdir DIR]" >&2
  exit 1
}

echo "PWD=${PWD}"
echo "HOST=$(hostname)"
ORIG_DIR="${PWD}"

ulimit -c 0   # no core files

SCRIPTDIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

runnumber=""
#skip=0
trkseg=0
list_nevents=1000
nevents=0
trkdst="tracks"
listdir=""

while [[ $# -gt 0 ]]; do
  case "$1" in
#    --skip|--skip=*)
#      if [[ "$1" == *=* ]]; then skip="${1#*=}"; shift
#      else [[ $# -lt 2 ]] && usage; skip=$2; shift 2; fi ;;
    --trkseg|--trkseg=*)
      if [[ "$1" == *=* ]]; then trkseg="${1#*=}"; shift
      else [[ $# -lt 2 ]] && usage; trkseg=$2; shift 2; fi ;;
    --nevents|--nevents=*)
      if [[ "$1" == *=* ]]; then nevents="${1#*=}"; shift
      else [[ $# -lt 2 ]] && usage; nevents=$2; shift 2; fi ;;
    --trkdst|--trkdst=*)
      if [[ "$1" == *=* ]]; then trkdst="${1#*=}"; shift
      else [[ $# -lt 2 ]] && usage; trkdst=$2; shift 2; fi
      [[ "$trkdst" == "clus" || "$trkdst" == "tracks" ]] || { echo "ERROR: --trkdst must be clus or tracks" >&2; exit 1; } ;;
    --listdir|--listdir=*)
      if [[ "$1" == *=* ]]; then listdir="${1#*=}"; shift
      else [[ $# -lt 2 ]] && usage; listdir=$2; shift 2; fi ;;
    --nevtperjob|--nevtperjob=*)
      if [[ "$1" == *=* ]]; then list_nevents="${1#*=}"; shift
      else [[ $# -lt 2 ]] && usage; list_nevents=$2; shift 2; fi ;;
    -*)    usage ;;
    *)     [[ -n "$runnumber" ]] && usage; runnumber=$1; shift ;;
  esac
done

[[ -z "$runnumber" ]] && usage

subdir="${ORIG_DIR}/MBDTRKZ/${runnumber}/$(printf '%04d' ${trkseg})"
echo $subdir
mkdir -p "${subdir}"

cd "${subdir}"

ln -sf "${SCRIPTDIR}/Fun4All_MBD_TrackVertex.C" .
ln -sf "${SCRIPTDIR}/Fun4All_MBD_TrackSeeding.C" .
ln -sf "${SCRIPTDIR}/Fun4All_MBD_TrackFitting.C" .
ln -sf /pdata/chiu/25_CALIBPRODUCTION/results/ .

python3 "${SCRIPTDIR}/make_mbdtrkz_lists.py" "$runnumber" --trkseg "$trkseg" --nevents "$list_nevents" --trkdst "$trkdst" \
  ${listdir:+--listdir "$listdir"} || exit 1

# tracks mode: stage files and run Fun4All_MBD_TrackVertex.C as before
if [[ -n "${_CONDOR_SCRATCH_DIR}" ]]; then
  mkdir -p "${_CONDOR_SCRATCH_DIR}"
  cp -p *.C *.list "${_CONDOR_SCRATCH_DIR}/"
  cd "${_CONDOR_SCRATCH_DIR}"
  ln -sf /pdata/chiu/25_CALIBPRODUCTION/results/ .
  time getinputfiles.pl --filelist fit.list
  [[ -f clus.list ]] && time getinputfiles.pl --filelist clus.list
  [[ -f seed.list ]] && time getinputfiles.pl --filelist seed.list  # never used
  [[ -f tracks.list ]] && time getinputfiles.pl --filelist tracks.list
fi

if [[ "$trkdst" == "clus" ]]; then
  # Run Fun4All_MBD_TrackFitting.C once per file in clus.list
  while IFS= read -r clusfile; do
    [[ -z "$clusfile" ]] && continue
    echo root.exe -b -q "Fun4All_MBD_TrackSeeding.C(${nevents},\"${clusfile}\")"
    root.exe -b -q "Fun4All_MBD_TrackSeeding.C(${nevents},\"${clusfile}\")" || exit 1
    seedfile=DST_TRKR_SEED${clusfile#DST_TRKR_CLUSTER}
    echo root.exe -b -q "Fun4All_MBD_TrackFitting.C(${nevents},\"${seedfile}\")"
    root.exe -b -q "Fun4All_MBD_TrackFitting.C(${nevents},\"${seedfile}\")" || exit 1
  done < clus.list

  ls DST_TRKR_TRACK*.root > tracks.list
else
  if [[ -n "${_CONDOR_SCRATCH_DIR}" ]]; then
    time getinputfiles.pl --filelist tracks.list
  fi
fi

[[ -s tracks.list ]] || { echo "ERROR: tracks.list is empty" >&2; exit 1; }
[[ -s fit.list ]] || { echo "ERROR: fit.list is empty" >&2; exit 1; }

tracks_dst_file=$(head -1 tracks.list)
mbd_dst_file=$(head -1 fit.list)
echo root.exe -b -q Fun4All_MBD_TrackVertex.C\(${nevents},\"${tracks_dst_file}\",\"${mbd_dst_file}\"\)
root.exe -b -q Fun4All_MBD_TrackVertex.C\(${nevents},\"${tracks_dst_file}\",\"${mbd_dst_file}\"\) || exit 1

if [[ -n "${_CONDOR_SCRATCH_DIR}" ]]; then
  pwd
  df -h
  ls -l
  cp -p mbdtrk_*.root "${subdir}/"
  cd "${ORIG_DIR}"
fi
