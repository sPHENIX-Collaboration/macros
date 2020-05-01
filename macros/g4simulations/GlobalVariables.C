#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
static bool overlapcheck = false;
static double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes
namespace BlackHoleGeometry
{
  static double max_radius = 0.; // this is needed for the overall dimension of the black hole
};
#endif
