#!/usr/bin/bash
# quick interactive test of the full chain (prep + embed) with a few events
# usage: run_local_test.sh [JETTRIGGER] [NEVENT_GEN] [IDX] [DATARUNNUMBER]
# output in localtest/<JETTRIGGER>/OutDir<IDX>

JETTRIGGER="${1:-Jet10}"
NEVENT_GEN="${2:-3}"
IDX="${3:-0}"
DATARUNNUMBER="${4:-68145}"

BASEDIR="$(cd "$(dirname "$0")" && pwd)/localtest"
mkdir -p "${BASEDIR}"
LOG="${BASEDIR}/${JETTRIGGER}_${IDX}.log"

bash "$(dirname "$0")/run_embed.sh" ${IDX} ${JETTRIGGER} ${NEVENT_GEN} "${BASEDIR}" ${DATARUNNUMBER} > "${LOG}" 2>&1
echo "exit code $? ; log in ${LOG}"
