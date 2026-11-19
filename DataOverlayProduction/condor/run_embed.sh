#!/usr/bin/bash
# usage: run_embed.sh <IDX> <JETTRIGGER> <NEVENT_GEN> <BASEDIR> <DATARUNNUMBER> [CENTMAX]

# plain release, a local install with an old libCaloEmbedding breaks the embedding
source /opt/sphenix/core/bin/sphenix_setup.sh -n new

IDX="$1" # job index, selects the data events
JETTRIGGER="$2"
NEVENT_GEN="$3"
BASEDIR="$4"
DATARUNNUMBER="$5"
CENTMAX="${6:-100}" # data (and JEWEL medium) centrality 0-CENTMAX %

# macros are copied from here at run time; point it at a frozen copy for a production
MACRODIR="${MACRODIR:-/sphenix/u/bseidlitz/work/dataOverlayReboot/macros/DataOverlayProduction}"

SAMPLE="${JETTRIGGER}"
if [ "${CENTMAX}" -lt 100 ]; then
  SAMPLE="${JETTRIGGER}_cent0to${CENTMAX}"
fi
OUTDIR="${BASEDIR}/${SAMPLE}/OutDir${IDX}"
mkdir -p "${OUTDIR}"
cd "${OUTDIR}"
cp ${MACRODIR}/Fun4All_G4_Embed_data.C .
cp ${MACRODIR}/Fun4All_PrepDataFitting.C .
cp ${MACRODIR}/run.sh .

bash run.sh ${IDX} ${JETTRIGGER} ${NEVENT_GEN} ${DATARUNNUMBER} ${CENTMAX}
