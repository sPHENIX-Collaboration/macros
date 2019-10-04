#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void MagnetInit() {}

double Magnet(PHG4Reco* g4Reco,
	      double radius,
	      const int crossings = 0,
	      const int absorberactive = 0,
	      int verbosity = 0) {

  double magnet_inner_radius = 130;
  double magnet_outer_radius = 143;
  double magnet_length = 400.;
  if (radius > magnet_inner_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than Magnet inner radius: " << magnet_inner_radius << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  radius = magnet_inner_radius;
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("MAGNET", 0);
  cyl->set_double_param("radius",magnet_inner_radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",magnet_length);
  cyl->set_double_param("thickness",magnet_outer_radius - magnet_inner_radius);
  cyl->set_string_param("material","Al5083"); // use 1 radiation length Al for magnet thickness
  cyl->set_double_param("place_z",-40.);
  cyl->SuperDetector("MAGNET");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  radius = magnet_outer_radius; // outside of magnet
  
  if (verbosity > 0) {
    cout << "========================= G4_Magnet.C::Magnet() ===========================" << endl;
    cout << " MAGNET Material Description:" << endl;
    cout << "  inner radius = " << magnet_inner_radius << " cm" << endl;
    cout << "  outer radius = " << magnet_outer_radius << " cm" << endl;
    cout << "  length = " << magnet_length << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  
  return radius; 
}
