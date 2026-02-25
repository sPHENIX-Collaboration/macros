#!/usr/bin/bash


IDX="$1" # same as $(Process)
NEVENT=2

# Absolute path back to the submit host’s directory
BASEDIR="/sphenix/u/bseidlitz/work/macros/DataOverlayProduction/condor/condorout/"

# Make and enter OutDir${IDX} next to the submit directory

OUTDIR="${BASEDIR}/OutDir${IDX}"
mkdir -p "${OUTDIR}"
cd       "${OUTDIR}"

sh genList.sh $1
root -l -b -q "Fun4All_PrepDataFitting.C(${NEVENT})"

# get prepared data filename
FILE=$(cat prepData.txt)


# Run the Fun4All_G4_Embed_data.C script with the provided parameters
root -l -b -q "Fun4All_G4_Embed_data.C(${NEVENT},0,\"${FILE}\", \"./\" , \"embed_test.root\", \"Jet30\", \"MDC2\")"
