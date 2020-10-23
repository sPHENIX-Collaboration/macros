#ifndef MACRO_G4PLUGDOOR_C
#define MACRO_G4PLUGDOOR_C

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
  // sPHENIX forward flux return(s)
  // define via four corners in the engineering drawing
  double z_1 = 330.81;
  double z_2 = 360.81;
  double r_1 = 30;
  double r_2 = 263.5;

  double length = z_2 - z_1;
  double place_z = (z_1 + z_2) / 2.;
}  // namespace G4PLUGDOOR

void PlugDoorInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4PLUGDOOR::r_2);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4PLUGDOOR::place_z + G4PLUGDOOR::length / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -(G4PLUGDOOR::place_z + G4PLUGDOOR::length / 2.));
}
void PlugDoor(PHG4Reco *g4Reco)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::PLUGDOOR_OVERLAPCHECK;
  const bool flux_door_active = Enable::ABSORBER || Enable::PLUGDOOR_ABSORBER;

  const string material("Steel_1006");

  PHG4CylinderSubsystem *flux_return_plus = new PHG4CylinderSubsystem("FLUXRET_ETA_PLUS", 0);
  flux_return_plus->set_double_param("length", G4PLUGDOOR::length);
  flux_return_plus->set_double_param("radius", G4PLUGDOOR::r_1);
  flux_return_plus->set_double_param("place_z", G4PLUGDOOR::place_z);
  flux_return_plus->set_double_param("thickness", G4PLUGDOOR::r_2 - G4PLUGDOOR::r_1);
  flux_return_plus->set_string_param("material", material);
  flux_return_plus->SetActive(flux_door_active);
  //  flux_return_plus->SuperDetector("FLUXRET_ETA_PLUS");
  flux_return_plus->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(flux_return_plus);

  PHG4CylinderSubsystem *flux_return_minus = new PHG4CylinderSubsystem("FLUXRET_ETA_MINUS", 0);
  flux_return_minus->set_double_param("length", G4PLUGDOOR::length);
  flux_return_minus->set_double_param("radius", G4PLUGDOOR::r_1);
  flux_return_minus->set_double_param("place_z", -G4PLUGDOOR::place_z);
  flux_return_minus->set_double_param("thickness", G4PLUGDOOR::r_2 - G4PLUGDOOR::r_1);
  flux_return_minus->set_string_param("material", material);
  flux_return_minus->SetActive(flux_door_active);
  //  flux_return_minus->SuperDetector("FLUXRET_ETA_MINUS");
  flux_return_minus->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(flux_return_minus);

  return;
}
#endif
