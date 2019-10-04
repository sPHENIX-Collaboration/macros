#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4jleic/G4JLeicVTXSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4jleic.so)
#endif
void VTXInit() {}

double VTX(PHG4Reco* g4Reco,
	   double radius,
	   const int crossings = 0,
	   const int absorberactive = 0,
	   int verbosity = 0)
{
  gSystem->Load("libg4jleic.so");
  gSystem->Load("libg4testbench.so");
  if (radius > 3.5)
  {
    cout << "radius too large to fit VTX" << endl;
    exit(1);
  }
  G4JLeicVTXSubsystem *jlvtx = new G4JLeicVTXSubsystem();
  jlvtx->SetActive();
  jlvtx->SuperDetector("JLVTX");
  g4Reco->registerSubsystem(jlvtx);
  radius = 15.5;
  return radius;
}
