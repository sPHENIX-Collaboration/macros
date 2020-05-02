#pragma once
static bool overlapcheck = false;
static double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes

namespace Enable
{
  static bool BlackHole = false;
  static bool OverLapCheck = false;
}

namespace BlackHoleGeometry
{
  static double max_radius = 0.; // this is needed for the overall dimension of the black hole
  static double min_z = 0.;
  static double max_z = 0.;
  static double gap = no_overlapp;
  static bool visible = false;
};
