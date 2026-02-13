#!/bin/bash

if [ -z "$RUNLISTGEN_EXE" ]; then
  echo "Error: RUNLISTGEN_EXE is not set. Please run setup.sh first."
  exit 1
fi

output_dir="$(pwd)/run3auau/"
mkdir -p "$output_dir"

"$RUNLISTGEN_EXE" --dataset run3auau \
  --tag new_newcdbtag_v008 \
  --dsttype DST_CALOFITTING \
  --subsystems hcal emcal mbd \
  --min_bias_triggers 12  \
  --runtype physics \
  --min_run_length 300 \
  --require_magnet_on True \
  --livetime_threshold 12:0.7 \
  --min_reco_events 100_000 \
  --offline_status \
    "emcal <> BAD" \
    "ohcal <> BAD" \
    "ihcal <> BAD" \
  --l0_report summary \
  --prefix "$output_dir"
