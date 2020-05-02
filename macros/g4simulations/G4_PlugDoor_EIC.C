#pragma once

#include "GlobalVariables.C"

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)


  // sPHENIX forward flux return(s)
  // define via four corners in the engineering drawing
const double z_1  = 330.81;
const double z_2  = 360.81;
const double r_1  = 30;
const double r_2  = 263.5;

const double length = z_2 - z_1;
const double place_z =  -(z_1 + z_2) / 2.;

void PlugDoorInit() 
{
  if (BlackHoleGeometry::max_radius < r_2)
  {
    BlackHoleGeometry::max_radius = r_2;
  }
  if (BlackHoleGeometry::min_z > (place_z - length/2.))
  {
    BlackHoleGeometry::min_z = (place_z - length/2.);
  }
}

void PlugDoor(PHG4Reco *g4Reco,
              const int absorberactive = 0,
              int verbosity = 0)
{
  //----------------------------------------
  const string material("Steel_1006");
  const int flux_door_active = false;

  PHG4CylinderSubsystem *flux_return_minus = new PHG4CylinderSubsystem("FLUXRET_ETA_MINUS", 0);
  flux_return_minus->set_int_param("lengthviarapidity", 0);
  flux_return_minus->set_double_param("length", length);
  flux_return_minus->set_double_param("radius", r_1);
  flux_return_minus->set_double_param("place_z", place_z);
  flux_return_minus->set_double_param("thickness", r_2 - r_1);
  flux_return_minus->set_string_param("material", material);
  flux_return_minus->SetActive(flux_door_active);
//  flux_return_minus->SuperDetector("FLUXRET_ETA_MINUS");
  flux_return_minus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_minus);

  return;
}
