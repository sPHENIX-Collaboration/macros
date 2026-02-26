#!/usr/bin/bash


IDX="$1" # same as $(Process)
NEVENT_GEN=100
DATARUNNUMBER=54404

NEVENTS_PREPARED_DATA=$(( NEVENT_GEN * 2 )) #padding for possible skipping of events

echo "=== run.sh inputs ==="
echo "  IDX (job index)           = ${IDX}"
echo "  NEVENT_GEN               = ${NEVENT_GEN}"
echo "  NEVENTS_PREPARED_DATA    = ${NEVENTS_PREPARED_DATA}"
echo "  DATARUNNUMBER            = ${DATARUNNUMBER}"

# check number of events in a setment
NEVENTS_SEGMENT=$(psql FileCatalog -t -A -c  "select events from datasets where segment=0 and filename like 'DST_TRIGGERED_EVENT_seb07%' and runnumber=${DATARUNNUMBER};")


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

# add the next segment to the list in case we go over the number of events in the segment
psql FileCatalog -t -A -c  "select filename from datasets where segment=$(( SEGMENT + 1 )) and filename like 'DST_TRIG%' and runnumber=${DATARUNNUMBER};" >> test.list

root -l -b -q "Fun4All_PrepDataFitting.C(${NEVENT_GEN},${NEVENTS_PREPARED_DATA},${NEVENTS_SKIP},\"test.list\")"

# get prepared data filename from prepData.txt which is created by Fun4All_PrepDataFitting.C
FILE=$(cat prepData.txt)


# Run the Fun4All_G4_Embed_data.C script with the provided parameters
root -l -b -q "Fun4All_G4_Embed_data.C(${NEVENT_GEN},0,\"${FILE}\", \"./\" , \"embed_test.root\", \"Jet30\", \"MDC2\")"
