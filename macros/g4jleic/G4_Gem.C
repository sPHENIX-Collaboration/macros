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
  double gem_inner_radius = 0.; // cm
  double gem_outer_radius = 115.; // cm
  double size_z = 1.;
  PHG4CylinderSubsystem *cyl;
  for (int ilayer=0; ilayer<8; ilayer++)
  {
    double irad = gem_inner_radius + 1. + 0.5*ilayer;
    double orad = gem_outer_radius - 25. + 2.*ilayer;
    cyl = new PHG4CylinderSubsystem("GemHadron", ilayer);
    cyl->set_double_param("radius",irad);
    cyl->set_string_param("material","G4_CARBON_DIOXIDE");
    cyl->set_double_param("thickness",orad-irad);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length", size_z);
    double place_z = 340./2. + shift_z + 5. + 3.*ilayer;
    cyl->set_double_param("place_z",place_z);
    cyl->SetActive();
    cyl->SuperDetector("GEMHADRON");
    g4Reco->registerSubsystem( cyl );
  }
  for (int ilayer=0; ilayer<8; ilayer++)
  {
    double irad = gem_inner_radius + 1. + 0.5*ilayer;
    double orad = gem_outer_radius - 25. + 2.*ilayer;
    cyl = new PHG4CylinderSubsystem("GEMELECTRON", ilayer);
    cyl->set_double_param("radius",gem_inner_radius);
    cyl->set_string_param("material","G4_CARBON_DIOXIDE");
    cyl->set_double_param("thickness",orad - irad);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length", size_z);
    double place_z =  -340./2. + shift_z -5. - 3.*ilayer;
    cyl->set_double_param("place_z",place_z);
    cyl->SetActive();
    cyl->SuperDetector("GEMELECTRON");
    g4Reco->registerSubsystem( cyl );
  }

  return radius;
}
