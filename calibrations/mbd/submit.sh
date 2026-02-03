#!/bin/bash
#
# submit jobs to condor
# 
# $executable the program to execute (usually a shell script)
# $arg is what is passed to the run.cmd script
# $log is what the log file names are
# $user is the email to send to
#

executable=$1
arg=$2
arg="${@:2}"
logfname=${executable}

echo $executable $arg

# make sure log directory exists
TMPLOG=/tmp/${USER}/${executable##*/}/log
echo mkdir -p ${TMPLOG}
mkdir -p ${TMPLOG}
logdir=log/${executable##*/}
mkdir -p $logdir

echo "Submitting Run ${ijob}"

log=${arg##*/}           # log file names

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
    -a "request_memory=4096MB" \
    -a "PeriodicHold=(NumJobStarts>=1 && JobStatus == 1)" \
    -a "GetEnv=True"


echo Condor submit logs are in $TMPLOG and output logs are in $logdir

