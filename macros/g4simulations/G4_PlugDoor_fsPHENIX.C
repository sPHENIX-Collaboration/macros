#pragma once

#include "GlobalVariables.C"

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

const double place_z = 335.9;
const double length = 10.2;
const double inner_radius = 2.1;
const double thickness = 258.5;

void PlugDoorInit()
{
  if (BlackHoleGeometry::max_radius < (inner_radius+thickness))
  {
    BlackHoleGeometry::max_radius = inner_radius+thickness;
  }
  if (BlackHoleGeometry::max_z < (place_z + length/2.))
  {
    BlackHoleGeometry::max_z = place_z + length/2.;
  }
  if (BlackHoleGeometry::min_z > -(place_z + length/2.))
  {
    BlackHoleGeometry::min_z = -(place_z + length/2.);
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
  flux_return_plus->set_double_param("length", length);
  flux_return_plus->set_double_param("radius", inner_radius);
  flux_return_plus->set_double_param("place_z", place_z);
  flux_return_plus->set_double_param("thickness", thickness);
  flux_return_plus->set_string_param("material", material);
  flux_return_plus->SetActive(flux_door_active);
  flux_return_plus->SuperDetector("FLUXRET_ETA_PLUS");
  flux_return_plus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_plus);

  PHG4CylinderSubsystem *flux_return_minus = new PHG4CylinderSubsystem("FWDFLUXRET", 1);
  flux_return_minus->set_double_param("length", length);
  flux_return_minus->set_double_param("radius", inner_radius);
  flux_return_minus->set_double_param("place_z", -place_z);
  flux_return_minus->set_double_param("thickness", thickness);
  flux_return_minus->set_string_param("material", material);
  flux_return_minus->SetActive(flux_door_active);
  flux_return_minus->SuperDetector("FLUXRET_ETA_MINUS");
  flux_return_minus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_minus);

  return;
}
