#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void Barrel_HcalInit() {}

double Barrel_Hcal(PHG4Reco* g4Reco,
	      double radius,
	      const int crossings = 0,
	      const int absorberactive = 0,
	      int verbosity = 0)
{

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // here is our silicon:
//  double shift_z =  jleic_shiftz; // shift z from GlobalVariables.C
  double inner_radius = 144.; // solenoid Rout
  double outer_radius = 144.+100.; // out barrel hcal thickness
  double size_z = 460.; // magnet (400) + endcap (60)
  double gap = 2.;
  double thickness = 2.; // cm
  if (radius > inner_radius)
 {
    cout << "inconsistency: radius: " << radius 
	 << " larger than Barrel Hcal inner radius: " << inner_radius << endl;
    gSystem->Exit(-1);
  }
  
  PHG4CylinderSubsystem *cyl;
  radius =  inner_radius;
  for (int ilayer = 0; ilayer < 25; ilayer++)
    {
      if (radius > outer_radius)
      {
	continue;
      }
      cyl = new PHG4CylinderSubsystem("BARRELHCAL", ilayer);
      cyl->set_double_param("radius",radius);
      cyl->set_string_param("material","G4_Fe");
      cyl->set_double_param("thickness",thickness);
      cyl->set_int_param("lengthviarapidity",0);
      cyl->set_double_param("length",size_z);
//      cyl->set_double_param("place_z",shift_z);
      cyl->SetActive();
      cyl->SuperDetector("BARRELHCAL");
      g4Reco->registerSubsystem( cyl );
      radius +=  gap + thickness;
    }
  return outer_radius;
}
