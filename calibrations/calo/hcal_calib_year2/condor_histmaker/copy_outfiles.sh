#!/bin/bash

# name of the calibration the spectra were made with (see run.sh): cosmics (default), cosmics_tsc, cosmics_tsc_temp, ...
#   sh copy_outfiles.sh cosmics_tsc   copies hadd_condor_cosmics_tsc to out_run_cosmics_tsc
calibName=${1:-cosmics}
outSuffix="_$calibName"
[ "$calibName" = "cosmics" ] && outSuffix=""

# Define source and destination
hadd_dir="hadd_condor$outSuffix"
dest_dir="out_run$outSuffix"
if [ ! -d "$hadd_dir" ]; then
  echo "no folder $hadd_dir"
  exit 1
fi

# Create destination directory if it doesn't exist
mkdir -p "$dest_dir"

# Loop through all OutDir* folders inside hadd_condor
for dir in "$hadd_dir"/OutDir*; do
  if [ -d "$dir" ]; then
    for file in "$dir"/out_*.root; do
      if [ -f "$file" ]; then
        echo "Copying $file to $dest_dir/"
        cp "$file" "$dest_dir/"
      fi
    done
  fi
done

echo "All out_*.root files have been copied to $dest_dir/"
