#!/bin/bash

MYINSTALL=""
#MYINSTALL="/sphenix/user/jbertaux/MYINSTALL"
#MYINSTALL="/sphenix/user/jaein213/modules/install"

RUN_NUM=""         # required
NUM_EVT="400000"   # default if argument 2 is empty

show_help()
{
cat << EOF

	usage:
		$0 [run number] [num events (optional)]
	The default for [num events] is ${NUM_EVT}

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

# Custom MYINSTALL for developement/debugging purposes
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
if [ -n "${MYINSTALL}" ] && [ -d "${MYINSTALL}" ]; then
	source /opt/sphenix/core/bin/setup_local.sh ${MYINSTALL}
fi

# Macro
root -q -b "${PWD}/macro_Calib.C(${RUN_NUM}, ${NUM_EVT})"
# gdb -ex run --args root.exe "${PWD}/macro.C(${RUN_NUM}, ${NUM_EVT})"
EXIT_VALUE="$?"

exit ${EXIT_VALUE}

