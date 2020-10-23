#ifndef MACRO_G4PLUGDOORFSPHENIX_C
#define MACRO_G4PLUGDOORFSPHENIX_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool PLUGDOOR = false;
  bool PLUGDOOR_ABSORBER = false;
  bool PLUGDOOR_OVERLAPCHECK = false;
}  // namespace Enable

namespace G4PLUGDOOR
{
  double place_z = 335.9;
  double length = 10.2;
  double inner_radius = 2.1;
  double thickness = 258.5;
}  // namespace G4PLUGDOOR

void PlugDoorInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4PLUGDOOR::inner_radius + G4PLUGDOOR::thickness);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4PLUGDOOR::place_z + G4PLUGDOOR::length / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -(G4PLUGDOOR::place_z + G4PLUGDOOR::length / 2.));
}

void PlugDoor(PHG4Reco *g4Reco)
{
  //----------------------------------------
  // fsPHENIX forward flux return(s)
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::PLUGDOOR_OVERLAPCHECK;
  bool flux_door_active = Enable::ABSORBER || Enable::PLUGDOOR_ABSORBER;

  const string material("Steel_1006");

  PHG4CylinderSubsystem *flux_return_plus = new PHG4CylinderSubsystem("FWDFLUXRET", 0);
  flux_return_plus->set_double_param("length", G4PLUGDOOR::length);
  flux_return_plus->set_double_param("radius", G4PLUGDOOR::inner_radius);
  flux_return_plus->set_double_param("place_z", G4PLUGDOOR::place_z);
  flux_return_plus->set_double_param("thickness", G4PLUGDOOR::thickness);
  flux_return_plus->set_string_param("material", material);
  flux_return_plus->SetActive(flux_door_active);
  flux_return_plus->SuperDetector("FLUXRET_ETA_PLUS");
  flux_return_plus->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(flux_return_plus);

  PHG4CylinderSubsystem *flux_return_minus = new PHG4CylinderSubsystem("FWDFLUXRET", 0);
  flux_return_minus->set_double_param("length", G4PLUGDOOR::length);
  flux_return_minus->set_double_param("radius", G4PLUGDOOR::inner_radius);
  flux_return_minus->set_double_param("place_z", -G4PLUGDOOR::place_z);
  flux_return_minus->set_double_param("thickness", G4PLUGDOOR::thickness);
  flux_return_minus->set_string_param("material", material);
  flux_return_minus->SetActive(flux_door_active);
  flux_return_minus->SuperDetector("FLUXRET_ETA_MINUS");
  flux_return_minus->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(flux_return_minus);

  return;
}
#endif
