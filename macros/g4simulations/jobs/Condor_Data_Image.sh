#!/bin/bash

# Author: Molly Taylor
# Date: 10/15/18
# Goal: iterate though different INTT configurations and generate Kalman extrapolation uncertainties.

# IMAGING DETECTOR WITH FIXED MOMENTUM
# eta = -1.2 to 1.2
# all phi
# z = -10 cm to 10 cm
# pt = 0.5 GeV, 1 GeV, 10 GeV

# layout 0 = four (eight) layers, laddertypes 0-1-1-1
# layout 1 = four (eight) layers, laddertypes 1-1-0-1
# layout 2 = three (six) outer layers, laddertypes 1-0-1
# layout 3 = three (six) outer layers, laddertypes 1-1-1
# layout 4 = two (four) outer layers, laddertypes 0-1
# layout 5 = two (four) outer layers, laddertypes 1-1
# layout 5 = one (two) outer layers, laddertype 1
# layout 6 = no outer layers

config=0
n_events=70
particle=pi+ # particle being thrown: pi+, mu+, e-
data=sphenix/u/mitay/Documents/latest/macros/macros/g4simulations/data_gen # must be absolute path, no slashes at beginning or end
parent=detector_image # parent directory

# verified correct layout by looking at truth particle data in ntp_hit

# define particle generation parameters
z_width=10 # number of cm on either side of zero, uniform distribution
eta_min=-1.2
eta_max=1.2
pt_min=0.5
pt_max=0.5
phi_min=-180
phi_max=180

# go through different momenta
for pt in 0.5 1 10
do
  # go through every INTT configuration
  for i in {0..7}
  do
    case $i in
      0) config=0111 ;;
      1) config=1101 ;;
      2) config=101  ;;
      3) config=111  ;;
      4) config=01   ;;
      5) config=11   ;;
      6) config=1    ;;
      7) config=n    ;;
      *) config=x    ;;
    esac

    pt_min="$pt"
    pt_max="$pt"

    # directory="$particle"_pt"$pt_min"-"$pt_max"GeV_phi"$phi_min"-"$phi_max"d_z-"$z_width"-"$z_width"cm_eta"$eta_min"-"$eta_max"
    directory="$particle"_pt"$pt_min"GeV_phi"$phi_min"-"$phi_max"d_z-"$z_width"-"$z_width"cm_eta"$eta_min"-"$eta_max"

    # check to see if there is a directory for this data set; create one if there isn't
    if [ ! -d /"$data"/"$parent" ]; then
      mkdir /"$data"/"$parent"
    fi

    if [ ! -d /"$data"/"$parent"/"$directory" ]; then
      mkdir /"$data"/"$parent"/"$directory"
    fi

    path="$data"/"$parent"/"$directory"

    condor_submit simple_job.job n_events=$n_events i=$i z_width=$z_width eta_min=$eta_min eta_max=$eta_max pt_min=$pt_min pt_max=$pt_max phi_min=$phi_min phi_max=$phi_max particle=$particle path=$path config=$config
  done
done

