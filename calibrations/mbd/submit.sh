#!/bin/bash
#
# submit jobs to condor
# 
# $executable the program to execute
# $arg is what is passed to the run.cmd script
#

# get_ndst_needed <arg>
#   arg: path of the form dir/<run>.list  (<run> is a zero-padded run number)
#   reads runs_mbd.txt (columns: runnumber  mbd_nscaled  eventsinrun)
#   returns the number of DST files needed to accumulate ~3M MBD triggers
get_ndst_needed() {
  local arg=$1

  # Extract run number (strip directory and .list; 10# forces base-10)
  local runfile=${arg##*/}
  local runno=$((10#${runfile%.list}))

  # Find run in runs_mbd.list and extract mbd_nscaled and eventsinrun
  local mbdlist=runlists/runs_mbd.txt
  local fields
  fields=$(awk -v run="$runno" 'int($1)==run {print $2, $3; exit}' "$mbdlist")
  if [[ -z "$fields" ]]; then
    echo "WARNING: run $runno not found in $mbdlist" >&2
    wc -l < "$arg"
    return 1
  fi
  local mbd_nscaled eventsinrun
  read mbd_nscaled eventsinrun <<< "$fields"

  # Count DST files listed in arg
  local num_dst_files
  num_dst_files=$(wc -l < "$arg")

  local min_mbd=3000000

  # Floating-point arithmetic in awk:
  #   frac_made    = 100000 * num_dst_files / eventsinrun
  #   mbd_expected = mbd_nscaled * frac_made
  #   if mbd_expected > min_mbd: ndst_needed = int((min_mbd / mbd_expected) * num_dst_files)
  #   else:                       ndst_needed = num_dst_files
  #   if ndst < 30 and num_dst_files >= 30: ndst = 30
  awk -v nf="$num_dst_files" -v evts="$eventsinrun" -v mbd="$mbd_nscaled" \
      -v min_mbd="$min_mbd" 'BEGIN {
    frac_made    = 100000.0 * nf / evts
    mbd_expected = mbd * frac_made
    if (mbd_expected > min_mbd) {
      ndst = int((min_mbd / mbd_expected) * nf)
      if (ndst < 1) ndst = 1
    } else {
      ndst = nf
    }
    if (ndst < 30 && nf >= 30) ndst = 30
    print ndst
  }'
}

executable=$1
listfile=$2
arg="${@:2}"  # everything starting from 2nd arg
logfname=${executable}

# Trim the input list to only the files needed for ~3M MBD triggers
num_dst_files=$(wc -l < "$listfile")
final_ndst=$num_dst_files
ndst_needed=$(get_ndst_needed "$listfile")
if [[ $? -eq 0 && -n "$ndst_needed" && $ndst_needed -lt $num_dst_files ]]; then
  trimfile="${listfile%.list}_trim.list"
  trimfile=${trimfile##*/}
  shuf -n "$ndst_needed" "$listfile" | sort > "$trimfile"
  arg="$trimfile ${@:3}"
  arg=${arg% }
  final_ndst=$ndst_needed
  echo "Trimmed $listfile to $ndst_needed/$num_dst_files files -> $trimfile"
fi

request_disk=$(( final_ndst * 4 ))GB

echo $executable $arg

# make sure log directory exists
TMPLOG=/tmp/${USER}/${executable##*/}/log
echo mkdir -p ${TMPLOG}
mkdir -p ${TMPLOG}
logdir=log/${executable##*/}
mkdir -p $logdir

echo "Submitting Run ${ijob}"

log=${arg##*/}           # log file names
log=${log%% *}           # log file names

#prepend pwd to executable if needed
if [[ ! $executable =~ ^/ ]]
then
  executable=${PWD}/${executable}
fi

echo "Queue" | condor_submit \
    -a "Executable=${executable}" \
    -a "Arguments=\"${arg}\"" \
    -a "Log=${TMPLOG}/${log}.log" \
    -a "Output=$PWD/${logdir}/${log}.out" \
    -a "Error=$PWD/${logdir}/${log}.err" \
    -a "Initialdir=$PWD" \
    -a "PeriodicHold=(NumJobStarts>=1 && JobStatus == 1 && !(ON_EVICT_CHECK_RequestMemory_REQUIREMENTS))" \
    -a "request_memory=4096MB" \
    -a "retry_request_memory_increase=2048MB" \
    -a "retry_request_memory_max=16192MB" \
    -a "request_disk=${request_disk}" \
    -a "GetEnv=True"


echo Condor submit logs are in $TMPLOG and output logs are in $logdir

