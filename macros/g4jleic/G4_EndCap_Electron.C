#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void EndCap_ElectronInit() {}

double EndCap_Electron(PHG4Reco* g4Reco,
	   double radius,
	   const int crossings = 0,
	   const int absorberactive = 0,
	   int verbosity = 0)
{

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // here is our silicon:
  double shift_z =  jleic_shiftz; // shift z from GlobalVariables.C
  double electron_inner_radius = 20.; // cm
  double electron_outer_radius = 244.; // cm
  double size_z = 60.;
  double place_z = -400./2.+jleic_shiftz-size_z/2.;
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("ECELECTRON", 0);
  cyl->set_double_param("radius",electron_inner_radius);
  cyl->set_string_param("material","G4_Fe");
  cyl->set_double_param("thickness",electron_outer_radius - electron_inner_radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length", size_z);
  cyl->set_double_param("place_z",place_z);
  cyl->SetActive();
  cyl->SuperDetector("ECELECTRON");
  g4Reco->registerSubsystem( cyl );


  return radius;
}
