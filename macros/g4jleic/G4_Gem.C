#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void GemInit() {}

double Gem(PHG4Reco* g4Reco,
	   double radius,
	   const int crossings = 0,
	   const int absorberactive = 0,
	   int verbosity = 0)
{

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // here is our silicon:
  double shift_z =  jleic_shiftz; // shift z from GlobalVariables.C
  double hadron_inner_radius = 0.; // cm
  double hadron_outer_radius = 115.; // cm
  double size_z = 30.;
  double place_z = 400./2.;
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("GemH", 0);
  cyl->set_double_param("radius",hadron_inner_radius);
  cyl->set_string_param("material","G4_Al");
  cyl->set_double_param("thickness",hadron_outer_radius - hadron_inner_radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length", size_z);
  cyl->set_double_param("place_z",place_z+shift_z);
  cyl->SetActive();
  cyl->SuperDetector("GEMH");
  g4Reco->registerSubsystem( cyl );

  cyl = new PHG4CylinderSubsystem("GemE", 0);
  cyl->set_double_param("radius",hadron_inner_radius);
  cyl->set_string_param("material","G4_Al");
  cyl->set_double_param("thickness",hadron_outer_radius - hadron_inner_radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length", size_z);
  cyl->set_double_param("place_z",shift_z-place_z);
  cyl->SetActive();
  cyl->SuperDetector("GEME");
  g4Reco->registerSubsystem( cyl );


  return radius;
}
