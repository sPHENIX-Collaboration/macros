#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void CTDInit() {}

double CTD(PHG4Reco* g4Reco,
	      double radius,
	      const int crossings = 0,
	      const int absorberactive = 0,
	      int verbosity = 0)
{

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // here is our silicon:
  double shift_z =  jleic_shiftz; // shift z from GlobalVariables.C
  double inner_radius = 21.; // cm
  double outer_radius = 80.; // cm
  double size_z = 340.;
  double si_layer_gap = 5.;
  double si_thickness = 0.001; // cm
  if (radius > inner_radius)
 {
    cout << "inconsistency: radius: " << radius 
	 << " larger than CTD inner radius: " << inner_radius << endl;
    gSystem->Exit(-1);
  }
  
  PHG4CylinderSubsystem *cyl;
  for (int ilayer = 0; ilayer < 15; ilayer++)
    {
      radius =  inner_radius + ilayer*si_layer_gap;
      if (radius+si_thickness > outer_radius)
      {
	continue;
      }
      cyl = new PHG4CylinderSubsystem("JLCTD", ilayer);
      cyl->set_color(0.1, 0, 1., 0.1);
      cyl->set_double_param("radius",radius);
      cyl->set_string_param("material","G4_Si");
      cyl->set_double_param("thickness",si_thickness);
      cyl->set_int_param("lengthviarapidity",0);
      cyl->set_double_param("place_z",shift_z);
      cyl->SetActive();
      cyl->SuperDetector("JLCTD");
      cyl->set_double_param("length",size_z);
      g4Reco->registerSubsystem( cyl );
    }
  radius += si_thickness + no_overlapp;
  return outer_radius;
}
