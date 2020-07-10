#ifndef MACRO_G4ALLSILICON_C
#define MACRO_G4ALLSILICON_C

#include "GlobalVariables.C"

#include <g4lblvtx/AllSiliconTrackerSubsystem.h>

R__LOAD_LIBRARY(libg4lblvtx.so)

namespace Enable
{
  bool ALLSILICON = false;
  bool ALLSILICON_ABSORBER = false;
}

void AllSiliconInit(){}

void AllSiliconSetup(PHG4Reco *g4Reco)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::ALLSILICON_ABSORBER;

  AllSiliconTrackerSubsystem *allsili = new AllSiliconTrackerSubsystem();
  allsili->set_string_param("GDMPath", string(getenv("CALIBRATIONROOT")) + "/AllSiliconTracker/FAIRGeom.gdml");

  allsili->AddAssemblyVolume("VST");	// Barrel
  allsili->AddAssemblyVolume("FST");	// Forward disks
  allsili->AddAssemblyVolume("BST");	// Backward disks
  allsili->AddAssemblyVolume("BEAMPIPE");	// Beampipe
  allsili->SuperDetector("LBLVTX");
  allsili->SetActive();          // this saves hits in the MimosaCore volumes
  if (AbsorberActive) allsili->SetAbsorberActive();  // this saves hits in all volumes (in the absorber node)
  g4Reco->registerSubsystem(allsili);
}
#endif
