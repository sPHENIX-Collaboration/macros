#!/bin/bash



output_file="out_RUN.root"

hadd $output_file /sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_y2/0mrad/condorout/Out*/OUT*RUN*.root

echo "Files have been merged into $output_file"

