#pragma once
static bool overlapcheck = false;
static double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes

namespace Enable
{
  static bool AEROGEL = false;
  static bool BLACKHOLE = false;
  static bool CEMC = false;
  static bool DIRC = false;
  static bool EEMC = false;
  static bool FEMC = false;
  static bool FHCAL = false;
  static bool HCALIN = false;
  static bool HCALOUT = false;
  static bool OVERLAPCHECK = false;
  static bool PIPE = false;
  static bool PSTOF = false;
  static bool RICH = false;
  static bool PLUGDOOR = false;
  static bool TRACKING = false;
}

namespace BlackHoleGeometry
{
  static double max_radius = 0.; // this is needed for the overall dimension of the black hole
  static double min_z = 0.;
  static double max_z = 0.;
  static double gap = no_overlapp;
  static bool visible = false;
};
