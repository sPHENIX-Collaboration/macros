#!/bin/bash
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
export RUNLISTGEN_SRC="$(pwd)"
export RUNLISTGEN_EXE="$RUNLISTGEN_SRC/runListGen.py"