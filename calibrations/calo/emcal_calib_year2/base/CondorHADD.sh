#!/bin/bash



output_file="out_RUN.root"

hadd $output_file BASEDIR/condorout/Out*/OUT*RUN*.root

echo "Files have been merged into $output_file"

