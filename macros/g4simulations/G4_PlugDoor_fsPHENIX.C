#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 00, 0)
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
#include "GlobalVariables.C"
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void PlugDoorInit()
{
  if (BlackHoleGeometry::max_radius < 263.5)
  {
    BlackHoleGeometry::max_radius = 263.5;  // eye balled, it can shrink a bit
  }
}

void PlugDoor(PHG4Reco *g4Reco,
              const int absorberactive = 0,
              int verbosity = 0)
{
  //----------------------------------------
  // fsPHENIX forward flux return(s)
  const string material("Steel_1006");
  const int flux_door_active = false;

  PHG4CylinderSubsystem *flux_return_plus = new PHG4CylinderSubsystem("FWDFLUXRET", 0);
  flux_return_plus->set_int_param("lengthviarapidity", 0);
  flux_return_plus->set_double_param("length", 10.2);
  flux_return_plus->set_double_param("radius", 2.1);
  flux_return_plus->set_double_param("place_z", 335.9);
  flux_return_plus->set_double_param("thickness", 263.5 - 5.0);
  flux_return_plus->set_string_param("material", material);
  flux_return_plus->SetActive(flux_door_active);
  flux_return_plus->SuperDetector("FLUXRET_ETA_PLUS");
  flux_return_plus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_plus);

  PHG4CylinderSubsystem *flux_return_minus = new PHG4CylinderSubsystem("FWDFLUXRET", 1);
  flux_return_minus->set_int_param("lengthviarapidity", 0);
  flux_return_minus->set_double_param("length", 10.2);
  flux_return_minus->set_double_param("radius", 2.1);
  flux_return_minus->set_double_param("place_z", -335.9);
  flux_return_minus->set_double_param("thickness", 263.5 - 5.0);
  flux_return_minus->set_string_param("material", material);
  flux_return_minus->SetActive(flux_door_active);
  flux_return_minus->SuperDetector("FLUXRET_ETA_MINUS");
  flux_return_minus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_minus);

  return;
}
