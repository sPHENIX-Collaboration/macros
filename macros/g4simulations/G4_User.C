#pragma once

namespace Enable
{
  bool USER = false;
}

namespace G4USER
{
  int myparam = 0;
}

void UserInit(int verbosity = 0)
{
  // set the black hole dimensions surrounding the detector
  // XXX: maximum radius of your detector
  // YYY: maximum extension in z
  // ZZZ: maximum extension in -z (use -ZZZ)
  //BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, XXX);
  //BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, YYY);
  //BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, ZZZ);
}

void UserDetector(PHG4Reco *g4Reco)
{
  return;
}
