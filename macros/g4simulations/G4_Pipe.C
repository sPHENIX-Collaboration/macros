#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void PipeInit() {}

double Pipe(PHG4Reco* g4Reco,
	    double radius,
	    const int absorberactive = 0,
	    int verbosity = 0) {

  double be_pipe_radius    = 2.0005; // 4.001 cm inner diameter from spec sheet
  double be_pipe_thickness = 0.0760; // 760 um based on spec sheet
  double be_pipe_length    = 80.0;   // +/- 40 cm

  double al_pipe_radius    = 2.0005; // same as Be pipe
  double al_pipe_thickness = 0.1600; // 1.6 mm based on spec
  double al_pipe_length    = 88.3;   // extension beyond +/- 40 cm
  
  if (radius > be_pipe_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than pipe inner radius: " << be_pipe_radius << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  // mid-rapidity beryillium pipe
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("VAC_BE_PIPE", 0);
  cyl->set_double_param("radius",0.0);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",be_pipe_length);
  cyl->set_string_param("material","G4_Galactic");
  cyl->set_double_param("thickness",be_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  
  cyl = new PHG4CylinderSubsystem("BE_PIPE", 1);
  cyl->set_double_param("radius",be_pipe_radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",be_pipe_length);
  cyl->set_string_param("material","G4_Be");
  cyl->set_double_param("thickness",be_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  // north aluminum pipe
  cyl = new PHG4CylinderSubsystem("VAC_N_AL_PIPE", 2);
  cyl->set_double_param("place_z",0.5*be_pipe_length+0.5*al_pipe_length+no_overlapp);
  cyl->set_double_param("radius",0.0);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",al_pipe_length);
  cyl->set_string_param("material","G4_Galactic");
  cyl->set_double_param("thickness",al_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  cyl = new PHG4CylinderSubsystem("N_AL_PIPE", 3);
  cyl->set_double_param("place_z",0.5*be_pipe_length+0.5*al_pipe_length+no_overlapp);
  cyl->set_double_param("radius",al_pipe_radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",al_pipe_length);
  cyl->set_string_param("material","G4_Al");
  cyl->set_double_param("thickness",al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  // south aluminum pipe
  cyl = new PHG4CylinderSubsystem("VAC_S_AL_PIPE", 4);
  cyl->set_double_param("place_z",-0.5*be_pipe_length-0.5*al_pipe_length-no_overlapp);
  cyl->set_double_param("radius",0.0);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",al_pipe_length);
  cyl->set_string_param("material","G4_Galactic");
  cyl->set_double_param("thickness",al_pipe_radius);
  cyl->SuperDetector("PIPE");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );

  cyl = new PHG4CylinderSubsystem("S_AL_PIPE", 5);
  cyl->set_double_param("place_z",-0.5*be_pipe_length-0.5*al_pipe_length-no_overlapp);
  cyl->set_double_param("radius",al_pipe_radius);
  cyl->set_int_param("lengthviarapidity",0);
  cyl->set_double_param("length",al_pipe_length);
  cyl->set_string_param("material","G4_Al");
  cyl->set_double_param("thickness",al_pipe_thickness);
  cyl->SuperDetector("PIPE");
  if (absorberactive)  cyl->SetActive();
  g4Reco->registerSubsystem( cyl );
  
  radius = be_pipe_radius + be_pipe_thickness;
  
  if (verbosity > 0) {
    cout << "=========================== G4_Pipe.C::Pipe() =============================" << endl;
    cout << " PIPE Material Description:" << endl;
    cout << "  inner radius = " << be_pipe_radius << " cm" << endl;
    cout << "  thickness = " << be_pipe_thickness << " cm" << endl;
    cout << "  outer radius = " << be_pipe_radius + be_pipe_thickness << " cm" << endl;
    cout << "  length = " << be_pipe_length << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  
  return radius; 
}
