#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void BlackHoleInit() {}

void BlackHole(PHG4Reco *g4Reco, double radius)
{
  // swallow all particles coming out of our detector
  if (radius < BlackHoleGeometry::max_radius)
  {
    radius = BlackHoleGeometry::max_radius;
  }
  double blackhole_length = (BlackHoleGeometry::max_z - BlackHoleGeometry::min_z)+2*BlackHoleGeometry::gap;
  double blackhole_zpos = BlackHoleGeometry::min_z - BlackHoleGeometry::gap + blackhole_length/2.;
  double blackhole_radius = radius + BlackHoleGeometry::gap+10; // make the black hole slightly larger than the radius
  cout << "blackhole_length: " << blackhole_length << endl;
  cout << "blackhole_zpos: " << blackhole_zpos << endl;
  cout << "blackhole_radius: " << blackhole_radius << endl;
  cout << "BlackHoleGeometry::min_z: " << BlackHoleGeometry::min_z << endl;
  cout << "BlackHoleGeometry::max_z: " << BlackHoleGeometry::max_z << endl;
  cout << "BlackHoleGeometry::gap: " << BlackHoleGeometry::gap << endl;
  PHG4CylinderSubsystem *blackhole = new PHG4CylinderSubsystem("BH", 1);
  blackhole->set_double_param("radius",blackhole_radius); 
  blackhole->set_double_param("length", blackhole_length); // make it cover the world in length
  blackhole->set_double_param("place_z",blackhole_zpos);
  blackhole->set_double_param("thickness",BlackHoleGeometry::gap/2.); // it needs some thickness
  if (BlackHoleGeometry::visible)
  {
    blackhole->set_color(1,0,0,0.7);
  }
  blackhole->BlackHole();
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(true);
  g4Reco->registerSubsystem(blackhole);

  //----------------------------------------
  // FORWARD/BACKWARD BLACKHOLEs (thin disks, thickness is radius, length is thickness)
  // +Z
  double blackhole_forward_zpos = blackhole_zpos + blackhole_length/2.+BlackHoleGeometry::gap/4.+BlackHoleGeometry::gap;
  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_PLUS", 1);
  blackhole->SuperDetector("BH_FORWARD_PLUS");
  blackhole->set_double_param("radius",0.);
  blackhole->set_double_param("thickness",blackhole_radius + BlackHoleGeometry::gap); // add a bit so we cover the microscopic gap (BlackHoleGeometry::gap) between barrel and endplate
  blackhole->set_double_param("length",BlackHoleGeometry::gap/2.); // it needs some thickness but not go outside world
  blackhole->set_double_param("place_z",blackhole_forward_zpos); // put at the end of the world
  if (BlackHoleGeometry::visible)
  {
    blackhole->set_color(1,0,0,0.7);
  }
  blackhole->BlackHole();
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(true);
  g4Reco->registerSubsystem(blackhole);

  // -Z
  double blackhole_backward_zpos = blackhole_zpos - (blackhole_length/2.+BlackHoleGeometry::gap/4.+BlackHoleGeometry::gap);

  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_NEG", 1);
  blackhole->SuperDetector("BH_FORWARD_NEG");
  blackhole->set_double_param("radius",0); // add 10 cm
  blackhole->set_double_param("thickness",blackhole_radius + BlackHoleGeometry::gap); // add a bit so we cover the microscopic gap (BlackHoleGeometry::gap) between barrel and endplate
  blackhole->set_double_param("length",BlackHoleGeometry::gap/2.); // it needs some thickness but not go outside world
  blackhole->set_double_param("place_z", blackhole_backward_zpos);
  if (BlackHoleGeometry::visible)
  {
    blackhole->set_color(1,0,0,0.7);
  }
  blackhole->BlackHole();
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(true);
  g4Reco->registerSubsystem(blackhole);
  return;
}
