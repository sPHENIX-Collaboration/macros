#!/bin/bash

# the job does not get the environment of the submit shell (GetEnv is not allowed), so set it up here
source /opt/sphenix/core/bin/sphenix_setup.sh -n new


output_file="out_RUN.root"

# CONDOROUT is filled in by hadd_condor.sh: condorout, or condorout_<name of the calibration>
hadd -k -j 8 $output_file ../../CONDOROUT/Out*/OUT*RUN*.root

echo "Files have been merged into $output_file"

