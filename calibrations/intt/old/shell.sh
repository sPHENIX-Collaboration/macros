#!/bin/bash

MYINSTALL=""
# MYINSTALL="/sphenix/user/jbertaux/MYINSTALL"

# One-liner to get location of this shell script
# PWD=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)
PWD="."

INTT_FORMAT="/sphenix/lustre01/sphnxpro/physics/INTT/%s/%s_intt%d-%08d-*"

# Intermediate variable to hold file lists
# Given unique names to prevent conflicts if multiple jobs are run simultaneously
# Removed before the shell script exits to avoid unnecessary buildup of files
INTT_LIST="${PWD}/lst/run_%08d_intt%01d.list"

# Output--can be set relative to the $(pwd) or given absolute paths
# If any are unset, those outputs are not produced
HOTMAP_CDB_FORMAT="${PWD}/hotmap_cdb/hotmap_run_%08d.root"
HOTMAP_PNG_FORMAT="${PWD}/hotmap_png/hotmap_run_%08d.png"

BCOMAP_CDB_FORMAT="${PWD}/bcomap_cdb/bcomap_run_%08d.root"
BCOMAP_PNG_FORMAT="${PWD}/bcomap_png/bcomap_run_%08d.png"

RUN_NUM=""         # required
NUM_EVT="200000"   # default if argument 2 is empty
RUN_TYPE="physics" # default if argument 3 is empty

show_help()
{
cat << EOF

	usage:
		$0 [run number] [num events (optional)] [run type (optional)]
	The default for [num events] is ${NUM_EVT}
	The default for [run type] is ${RUN_TYPE}
	For older runs, you may need to set [run type] as "beam" (and specify [num events])

EOF
}

# Arguments
if [[ $# -lt 1 || $1 == "-h" || $1 == "--help" ]]; then
	show_help
	exit 0
fi

if [[ -n "$1" ]]; then
	RUN_NUM="$1"
fi

if [[ -n "$2" ]]; then
	NUM_EVT="$2"
fi

if [[ -n "$3" ]]; then
	RUN_TYPE="$3"
fi

# Custom MYINSTALL for developement/debugging purposes
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
if [ -n "${MYINSTALL}" ] && [ -d "${MYINSTALL}" ]; then
	source /opt/sphenix/core/bin/setup_local.sh ${MYINSTALL}
fi

for FLX_SVR in $(seq 0 7); do
	printf -v LIST ${INTT_LIST} ${RUN_NUM} ${FLX_SVR}
	printf -v FILE ${INTT_FORMAT} ${RUN_TYPE} ${RUN_TYPE} ${FLX_SVR} ${RUN_NUM}
	mkdir -p $(dirname ${LIST})
	ls -1 ${FILE} > ${LIST} 2>/dev/null
done

# Output
printf -v HOTMAP_CDB_FILE ${HOTMAP_CDB_FORMAT} ${RUN_NUM}
mkdir -p $(dirname ${HOTMAP_CDB_FILE})
printf -v HOTMAP_PNG_FILE ${HOTMAP_PNG_FORMAT} ${RUN_NUM}
mkdir -p $(dirname ${HOTMAP_PNG_FILE})

printf -v BCOMAP_CDB_FILE ${BCOMAP_CDB_FORMAT} ${RUN_NUM}
mkdir -p $(dirname ${BCOMAP_CDB_FILE})
printf -v BCOMAP_PNG_FILE ${BCOMAP_PNG_FORMAT} ${RUN_NUM}
mkdir -p $(dirname ${BCOMAP_PNG_FILE})

# Macro
root -q -b "${PWD}/macro.C(${RUN_NUM}, ${NUM_EVT}, \"${INTT_LIST}\", \"${HOTMAP_CDB_FILE}\", \"${HOTMAP_PNG_FILE}\", \"${BCOMAP_CDB_FILE}\", \"${BCOMAP_PNG_FILE}\")"
# gdb -ex run --args root.exe "${PWD}/macro.C(${RUN_NUM}, ${NUM_EVT}, \"${INTT_LIST}\", \"${HOTMAP_CDB_FILE}\", \"${HOTMAP_PNG_FILE}\", \"${BCOMAP_CDB_FILE}\", \"${BCOMAP_PNG_FILE}\")"
EXIT_VALUE="$?"

# Remove list files after finishing
for FLX_SVR in $(seq 0 7); do
	printf -v LIST ${INTT_LIST} ${RUN_NUM} ${FLX_SVR}
	rm -f ${LIST}
done

exit ${EXIT_VALUE}
