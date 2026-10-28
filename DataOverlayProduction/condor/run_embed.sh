#!/usr/bin/bash
# usage: run_embed.sh <IDX> <JETTRIGGER> <NEVENT_GEN> <BASEDIR> <DATARUNNUMBER>

# plain release, a local install with an old libCaloEmbedding breaks the embedding
source /opt/sphenix/core/bin/sphenix_setup.sh -n new

# PHJewel, its cards and tables are not in the build yet: local build against new
# and the calibrations branch. Remove once they are in the release.
source /opt/sphenix/core/bin/setup_local.sh /sphenix/u/bseidlitz/work/dataOverlayReboot/install_phjewel
export JEWEL_CARDDIR=/sphenix/u/bseidlitz/work/calibrations/Generators/JEWEL
export JEWEL_TABLEDIR=/sphenix/u/bseidlitz/work/phjewel/JEWEL_tables

IDX="$1" # same as $(Process)
JETTRIGGER="$2"
NEVENT_GEN="$3"
BASEDIR="$4"
DATARUNNUMBER="$5"

MACRODIR="/sphenix/u/bseidlitz/work/dataOverlayReboot/macros/DataOverlayProduction"

OUTDIR="${BASEDIR}/${JETTRIGGER}/OutDir${IDX}"
mkdir -p "${OUTDIR}"
cd "${OUTDIR}"
cp ${MACRODIR}/Fun4All_G4_Embed_data.C .
cp ${MACRODIR}/Fun4All_PrepDataFitting.C .
cp ${MACRODIR}/run.sh .

bash run.sh ${IDX} ${JETTRIGGER} ${NEVENT_GEN} ${DATARUNNUMBER}
