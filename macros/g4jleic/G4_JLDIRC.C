#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4jleic/G4JLeicDIRCSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4jleic.so)
#endif
void JLDIRCInit() {}

double JLDIRC(PHG4Reco* g4Reco,
	   double radius,
	   const int crossings = 0,
	   const int absorberactive = 0,
	   int verbosity = 0)
{
  gSystem->Load("libg4jleic.so");
  gSystem->Load("libg4testbench.so");
  if (radius > 81)
  {
    cout << "radius " << radius << " too large (>81) to fit DIRC" << endl;
    exit(1);
  }
  G4JLeicDIRCSubsystem *jldirc = new G4JLeicDIRCSubsystem();
  jldirc->SetActive();
  jldirc->SuperDetector("JLDIRC");
  g4Reco->registerSubsystem(jldirc);
  radius = 85.;
  return radius;
}
