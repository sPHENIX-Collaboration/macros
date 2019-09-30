#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void EndCap_HadronInit() {}

double EndCap_Hadron(PHG4Reco* g4Reco,
		     double radius,
		     const int crossings = 0,
		     const int absorberactive = 0,
		     int verbosity = 0)
{
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // here is our silicon:
  double shift_z =  jleic_shiftz; // shift z from GlobalVariables.C
  double hadron_inner_radius = 80.; // cm
  double hadron_outer_radius = 243.; // cm
  PHG4CylinderSubsystem *mothervol = new PHG4CylinderSubsystem("EndCapHadronContainer",0);
  mothervol->set_color(0.3,0,3.,0.1);
  mothervol->set_double_param("radius",20.);
  mothervol->set_string_param("material","G4_AIR");
  mothervol->set_double_param("thickness",hadron_outer_radius - 20.);  
  mothervol->set_double_param("length",250.);
  mothervol->set_int_param("lengthviarapidity",0);
  mothervol->set_double_param("place_z", 400./2.-jleic_shiftz+250/2.+170.);
  g4Reco->registerSubsystem( mothervol );
  double size_z = 2.;
  double gap = 2.;
  double z_start = size_z/2.-250./2.;
  int nlayer=25;
  for (int i=0; i<nlayer; i++)
  {
    PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("ECHADRON", i);
    cyl->SetMotherSubsystem(mothervol);
    cyl->set_color(0.6, 0, 0.6, 1);
    cyl->set_double_param("radius",hadron_inner_radius);
    cyl->set_string_param("material","G4_Fe");
    cyl->set_double_param("thickness",hadron_outer_radius - hadron_inner_radius);
    cyl->set_int_param("lengthviarapidity",0);
    cyl->set_double_param("length", size_z);
    cyl->set_double_param("place_z",z_start);
    cyl->SetActive();
    cyl->SuperDetector("ECHADRON");
    z_start+=gap+size_z;
    g4Reco->registerSubsystem( cyl );
  }
  return radius;
}
