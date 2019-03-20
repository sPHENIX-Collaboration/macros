#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void PlugDoorInit() {}
void PlugDoor(PHG4Reco *g4Reco,
              const int absorberactive = 0,
              int verbosity = 0)
{
  //----------------------------------------
  // sPHENIX forward flux return(s)
  // define via four cornors in the engineering drawing
  const double z_1  = 330.81;
  const double z_2  = 360.81;
  const double r_1  = 30;
  const double r_2  = 263.5;
  const string material("Steel_1006");
  const int flux_door_active = false;


  PHG4CylinderSubsystem *flux_return_plus = new PHG4CylinderSubsystem("FLUXRET_ETA_PLUS", 0);
  flux_return_plus->set_int_param("lengthviarapidity", 0);
  flux_return_plus->set_double_param("length", z_2 - z_1);
  flux_return_plus->set_double_param("radius", r_1);
  flux_return_plus->set_double_param("place_z", (z_1 + z_2) / 2.);
  flux_return_plus->set_double_param("thickness", r_2 - r_1);
  flux_return_plus->set_string_param("material", material);
  flux_return_plus->SetActive(flux_door_active);
//  flux_return_plus->SuperDetector("FLUXRET_ETA_PLUS");
  flux_return_plus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_plus);

  PHG4CylinderSubsystem *flux_return_minus = new PHG4CylinderSubsystem("FLUXRET_ETA_MINUS", 0);
  flux_return_minus->set_int_param("lengthviarapidity", 0);
  flux_return_minus->set_double_param("length", z_2 - z_1);
  flux_return_minus->set_double_param("radius", r_1);
  flux_return_minus->set_double_param("place_z", -(z_1 + z_2) / 2.);
  flux_return_minus->set_double_param("thickness", r_2 - r_1);
  flux_return_minus->set_string_param("material", material);
  flux_return_minus->SetActive(flux_door_active);
//  flux_return_minus->SuperDetector("FLUXRET_ETA_MINUS");
  flux_return_minus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_minus);

  return;
}
