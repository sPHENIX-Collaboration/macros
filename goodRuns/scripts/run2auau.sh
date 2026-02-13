#!/bin/bash

if [ -z "$RUNLISTGEN_EXE" ]; then
  echo "Error: RUNLISTGEN_EXE is not set. Please run setup.sh first."
  exit 1
fi

output_dir="$(pwd)/run2auau/"
mkdir -p "$output_dir"

"$RUNLISTGEN_EXE" --dataset run2auau \
  --tag ana509_2024p022_v001 \
  --dsttype DST_CALOFITTING \
  --subsystems hcal emcal mbd \
  --min_bias_triggers 10  \
  --runtype physics \
  --min_run_length 300 \
  --require_magnet_on True \
  --livetime_threshold 10:0.7 \
  --min_reco_events 100_000 \
  --offline_status \
    "emcal == GOLDEN" \
  --l0_report summary \
  --prefix "$output_dir"

