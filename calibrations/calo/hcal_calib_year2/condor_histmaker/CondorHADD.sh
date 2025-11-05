#!/bin/bash



output_file="out_RUN.root"

hadd -k -j 8 $output_file ../../condorout/Out*/OUT*RUN*.root

echo "Files have been merged into $output_file"

