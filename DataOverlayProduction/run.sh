#!/usr/bin/bash


IDX="$1" # same as $(Process)
JETTRIGGER="${2:-Jet10}" # Jet5, Jet10, Jet12, Jet20, Jet30, Jet40, PhotonJet12, PhotonJet20
NEVENT_GEN="${3:-500}"
DATARUNNUMBER="${4:-68145}"

# padding for events failing the MB trigger
NEVENTS_PREPARED_DATA=$(( NEVENT_GEN * 5 / 2 ))

echo "=== run.sh inputs ==="
echo "  IDX (job index)           = ${IDX}"
echo "  JETTRIGGER               = ${JETTRIGGER}"
echo "  NEVENT_GEN               = ${NEVENT_GEN}"
echo "  NEVENTS_PREPARED_DATA    = ${NEVENTS_PREPARED_DATA}"
echo "  DATARUNNUMBER            = ${DATARUNNUMBER}"

# check number of events in a setment
NEVENTS_SEGMENT=$(psql FileCatalog -t -A -c  "select events from datasets where segment=0 and filename like 'DST_TRIGGERED_EVENT_seb%' and runnumber=${DATARUNNUMBER} limit 1;")
if [ -z "${NEVENTS_SEGMENT}" ] || [ "${NEVENTS_SEGMENT}" -le 0 ]; then
  echo "no DST_TRIGGERED_EVENT files found in FileCatalog for run ${DATARUNNUMBER}"
  exit 1
fi


#calculate which segment to run
SEGMENT=$(( ( IDX * NEVENTS_PREPARED_DATA ) / NEVENTS_SEGMENT))  

# calculate how many events to skip
NEVENTS_SKIP=$(( ( IDX * NEVENTS_PREPARED_DATA ) % NEVENTS_SEGMENT))  

START_INDEX=$(( IDX * NEVENTS_PREPARED_DATA ))
echo "=== segment/skip math ==="
echo "  NEVENTS_SEGMENT (from DB)= ${NEVENTS_SEGMENT}"
echo "  start index (IDX*NEVENTS_PREPARED_DATA) = ${START_INDEX}"
echo "  SEGMENT (which file)     = ${SEGMENT}  (= start / NEVENTS_SEGMENT)"
echo "  NEVENTS_SKIP             = ${NEVENTS_SKIP}  (= start % NEVENTS_SEGMENT)"
echo "========================="  

# create list of files to run
psql FileCatalog -t -A -c  "select filename from datasets where segment=${SEGMENT} and filename like 'DST_TRIG%' and runnumber=${DATARUNNUMBER};" > test.list
# raw DSTs get purged seb by seb, a partial list means part of the detector is missing
echo "  $(wc -l < test.list) DST_TRIGGERED_EVENT files for segment ${SEGMENT} (expect 20 for run3)"

# add the next segment to the list in case we go over the number of events in the segment
psql FileCatalog -t -A -c  "select filename from datasets where segment=$(( SEGMENT + 1 )) and filename like 'DST_TRIG%' and runnumber=${DATARUNNUMBER};" >> test.list

root -l -b -q "Fun4All_PrepDataFitting.C(${NEVENT_GEN},${NEVENTS_PREPARED_DATA},${NEVENTS_SKIP},\"test.list\")"

# get prepared data filename from prepData.txt which is created by Fun4All_PrepDataFitting.C
FILE=$(cat prepData.txt)
if [ ! -f "${FILE}" ]; then
  echo "prepared data file not found, Fun4All_PrepDataFitting.C failed"
  exit 1
fi


# Run the Fun4All_G4_Embed_data.C script with the provided parameters
root -l -b -q "Fun4All_G4_Embed_data.C(${NEVENT_GEN},0,\"${FILE}\", \"./\" , \"embed_test.root\", \"${JETTRIGGER}\", \"MDC2\")"
