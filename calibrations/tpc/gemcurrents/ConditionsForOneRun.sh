#! /usr/bin/bash

PROCESS=$1
LINE=$((PROCESS + 1))
RUNNUMBER=$(sed -n "${LINE}p" runlist.txt)

echo "Process: ${PROCESS}"
echo "Run: ${RUNNUMBER}"

python3 daqdbreader_tpc_GEM_charge_status_2.py ${RUNNUMBER}
root -l -b -q "FillGemCurrents.C(${RUNNUMBER})"
