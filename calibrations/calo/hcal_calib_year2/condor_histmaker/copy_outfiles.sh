#!/bin/bash

# Define source and destination
hadd_dir="hadd_condor"
dest_dir="out_run"

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

