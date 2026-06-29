#!/usr/bin/env bash
#
# Input: runnumber
# Returns runtype (2024pp, 2024auau, 2025auau, 2025pp)
#

if [[ $# -lt 1 ]]
then
  echo "Usage: runtype.sh <runno>"
  exit -1
fi

runno=$1

if [[ $runno -le 42000 ]]
then
  # sim
  echo 5
elif [[ $runno -le 53880 ]]
then
  # 2024 Run2pp
  echo 0
elif [[ $runno -le 54962 ]]
then
  # 2024 Run2auau
  echo 1
elif [[ $runno -le 78954 ]]
then
  # 2025 Run3auau
  echo 2
elif [[ $runno -le 99999 ]]
then
  # 2025 Run3pp
  echo 3
fi


