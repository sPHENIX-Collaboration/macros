#!/bin/bash

if [ -z "$RUNLISTGEN_EXE" ]; then
  echo "Error: RUNLISTGEN_EXE is not set. Please run setup.sh first."
  exit 1
fi

output_dir="$(pwd)/run3auau/"
mkdir -p "$output_dir"

MB_TRIG_BIT=12

"$RUNLISTGEN_EXE" --dataset run3auau \
  --tag new_newcdbtag_v008 \
  --dsttype DST_CALOFITTING \
  --subsystems hcal emcal mbd sepd \
  --min_bias_triggers ${MB_TRIG_BIT}  \
  --runtype physics \
  --min_run_length 300 \
  --require_magnet_on True \
  --livetime_threshold ${MB_TRIG_BIT}:0.7 \
  --min_reco_events 1_000_000 \
  --offline_status \
    "emcal == GOLDEN" \
    "ohcal == GOLDEN" \
    "ihcal == GOLDEN" \
  --l0_report summary \
  --prefix "$output_dir"
