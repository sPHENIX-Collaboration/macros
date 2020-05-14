#pragma once
static bool overlapcheck = false;
static double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes

namespace Enable
{
  static bool OVERLAPCHECK = false;
}

// every G4 subsystem needs to implement this
// rather than forcing another include file, 
// let's put this into the GlobalVariables.C
namespace BlackHoleGeometry
{
  static double max_radius = 0.; // this is needed for the overall dimension of the black hole
  static double min_z = 0.;
  static double max_z = 0.;
  static double gap = no_overlapp;
  static bool visible = false;
};
