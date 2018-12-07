#!/bin/bash

# Author: Molly Taylor
# Date: 10/29/18
# Goal: iterate though different INTT configurations and generate Kalman extrapolation uncertainties.

# SCAN THROUGH MOMENTA
# eta = -1.2 to 1.2
# all phi
# z = 0 cm
# pt = 500 MeV, 600 MeV, 700 MeV, 800 MeV, 900 MeV, 1 GeV, 2 GeV, 3 GeV

# layout 0 = four (eight) layers, laddertypes z-p-p-p
# layout 1 = four (eight) layers, laddertypes p-p-z-p
# layout 2 = three (six) outer layers, laddertypes p-z-p
# layout 3 = three (six) outer layers, laddertypes p-p-p
# layout 4 = two (four) outer layers, laddertypes z-p
# layout 5 = two (four) outer layers, laddertypes p-p
# layout 6 = one (two) outer layers, laddertype p
# layout 7 = no outer layers
# layout 8 = two (four) outer layers at 13 cm and 15 cm, laddertypes p-p

config=0
n_events=500
particle=pi+ # particle being thrown: pi+, mu+, e-
data=direct/star+u/rcorliss/sphenix/macros/macros/g4simulations/data_gen #ross's path
#data=sphenix/u/mitay/Documents/latest/macros/macros/g4simulations/data_gen # must be absolute path, no slashes at beginning or end
parent=mom_scan # parent directory

# verified correct layout by looking at truth particle data in ntp_hit

# define particle generation parameters
z_width=0 # number of cm on either side of zero, uniform distribution
eta_min=-1.2
eta_max=1.2
pt_min=0.5
pt_max=0.5
phi_min=-180
phi_max=180

# go through different momenta

for set in 1 2 3 4 5 6 7 8 9

do
  # go through every INTT configuration
  for i in 0 1 2 3 4 5 6 7 8
  do
    case $i in
      0) config=zppp ;;
      1) config=ppzp ;;
      2) config=0pzp ;;
      3) config=0ppp ;;
      4) config=00zp ;;
      5) config=00pp ;;
      6) config=000p ;;
      7) config=0000 ;;
      8) config=00pp_out ;;
      *) config=xxxx ;;
    esac

    pt_min=0.5
    pt_max=3.0

    # directory="$particle"_pt"$pt_min"-"$pt_max"GeV_phi"$phi_min"-"$phi_max"d_z-"$z_width"-"$z_width"cm_eta"$eta_min"-"$eta_max"
    directory="$particle"_pt"$pt_min"-"$pt_max"GeV_z"$z_width"cm_eta"$eta_min"-"$eta_max"_set"$set"
    # directory="$particle"_pt"$pt_min"-"$pt_max"GeV_phi"$phi_min"-"$phi_max"d_z"$z_width"cm_eta"$eta_min"-"$eta_max"


    # check to see if there is a directory for this data set; create one if there isn't
    if [ ! -d "$data"/"$parent" ]; then
      mkdir "$data"/"$parent"
    fi

    if [ ! -d "$data"/"$parent"/"$directory" ]; then
      mkdir "$data"/"$parent"/"$directory"
    fi

    path="$data"/"$parent"/"$directory"

    condor_submit simple_job.job n_events=$n_events i=$i z_width=$z_width eta_min=$eta_min eta_max=$eta_max pt_min=$pt_min pt_max=$pt_max phi_min=$phi_min phi_max=$phi_max particle=$particle path=$path config=$config

    #echo condor_submit simple_job.job n_events=$n_events i=$i z_width=$z_width eta_min=$eta_min eta_max=$eta_max pt_min=$pt_min pt_max=$pt_max phi_min=$phi_min phi_max=$phi_max particle=$particle path=$path config=$config
  done
done

