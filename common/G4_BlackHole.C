#ifndef MACRO_G4BLACKHOLE_C
#define MACRO_G4BLACKHOLE_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool BLACKHOLE = false;
  bool BLACKHOLE_SAVEHITS = true;
  bool BLACKHOLE_FORWARD_SAVEHITS = true;
}  // namespace Enable

void BlackHoleInit() {}

void BlackHole(PHG4Reco *g4Reco, double radius)
{
  // swallow all particles coming out of our detector
  if (radius < BlackHoleGeometry::max_radius)
  {
    radius = BlackHoleGeometry::max_radius;
  }
  double blackhole_length = (BlackHoleGeometry::max_z - BlackHoleGeometry::min_z) + 2 * BlackHoleGeometry::gap;
  double blackhole_zpos = BlackHoleGeometry::min_z - BlackHoleGeometry::gap + blackhole_length / 2.;
  double blackhole_radius = radius + BlackHoleGeometry::gap;  // make the black hole slightly larger than the radius
  PHG4CylinderSubsystem *blackhole = new PHG4CylinderSubsystem("BH", 1);
  blackhole->set_double_param("radius", blackhole_radius);
  blackhole->set_double_param("length", blackhole_length);  // make it cover the world in length
  blackhole->set_double_param("place_z", blackhole_zpos);
  blackhole->set_double_param("thickness", BlackHoleGeometry::gap / 2.);  // it needs some thickness
  if (BlackHoleGeometry::visible)
  {
    blackhole->set_color(1, 0, 0, 0.7);
  }
  blackhole->BlackHole();
  if (Enable::BLACKHOLE_SAVEHITS)
  {
    blackhole->SetActive();  // see what leaks out
  }
  blackhole->OverlapCheck(true);
  g4Reco->registerSubsystem(blackhole);

  //----------------------------------------
  // FORWARD/BACKWARD BLACKHOLEs (thin disks, thickness is radius, length is thickness)
  // +Z
  // if min/max z is not symmetric, the cylinder is not centered around z=0
  // to find the offset we start with the middle of the barrel at blackhole_zpos, then add its length/2. which
  // gets us to the end of the cylinder, then we add another gap so the endcap does not overlap
  // the endcap itself is BlackHoleGeometry::gap/2 thick, leaving BlackHoleGeometry::gap/4 on each side of the
  // center. Basically we have a gap of BlackHoleGeometry::gap/4 between the barrel and the endplates
  // therefore we add BlackHoleGeometry::gap to the radius of the endcap so particles trying to go through
  // the tiny gap between barren and endplate will hit the endplate
  double blackhole_forward_zpos = blackhole_zpos + blackhole_length / 2. + BlackHoleGeometry::gap / 2.;
  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_PLUS", 1);
  blackhole->SuperDetector("BH_FORWARD_PLUS");
  blackhole->set_double_param("radius", 0.);
  blackhole->set_double_param("thickness", blackhole_radius + BlackHoleGeometry::gap);  // add a bit so we cover the microscopic gap (BlackHoleGeometry::gap) between barrel and endplate
  blackhole->set_double_param("length", BlackHoleGeometry::gap / 2.);                   // it needs some thickness but not go outside world
  blackhole->set_double_param("place_z", blackhole_forward_zpos);                       // put at the end of the world
  if (BlackHoleGeometry::visible)
  {
    blackhole->set_color(1, 0, 0, 0.7);
  }
  blackhole->BlackHole();
  if (Enable::BLACKHOLE_SAVEHITS && Enable::BLACKHOLE_FORWARD_SAVEHITS)
  {
    blackhole->SetActive();  // see what leaks out
  }
  blackhole->OverlapCheck(true);
  g4Reco->registerSubsystem(blackhole);

  // -Z
  double blackhole_backward_zpos = blackhole_zpos - (blackhole_length / 2. + BlackHoleGeometry::gap / 2.);

  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_NEG", 1);
  blackhole->SuperDetector("BH_FORWARD_NEG");
  blackhole->set_double_param("radius", 0);                                             // add 10 cm
  blackhole->set_double_param("thickness", blackhole_radius + BlackHoleGeometry::gap);  // add a bit so we cover the microscopic gap (BlackHoleGeometry::gap) between barrel and endplate
  blackhole->set_double_param("length", BlackHoleGeometry::gap / 2.);                   // it needs some thickness but not go outside world
  blackhole->set_double_param("place_z", blackhole_backward_zpos);
  if (BlackHoleGeometry::visible)
  {
    blackhole->set_color(1, 0, 0, 0.7);
  }
  blackhole->BlackHole();
  if (Enable::BLACKHOLE_SAVEHITS && Enable::BLACKHOLE_FORWARD_SAVEHITS)
  {
    blackhole->SetActive();  // always see what leaks out
  }
  blackhole->OverlapCheck(true);
  g4Reco->registerSubsystem(blackhole);
  return;
}
#endif
