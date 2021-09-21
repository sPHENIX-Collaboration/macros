#ifndef MACRO_G4CEMCALBEDO_C
#define MACRO_G4CEMCALBEDO_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

namespace Enable
{
  bool CEMCALBEDO = false;
  bool CEMCALBEDO_ABSORBER = false;
}  // namespace Enable

namespace G4CEMCALBEDO
{
  double teflon_cylinder_thickness = 1.5;
  double inner_radius = 95. - teflon_cylinder_thickness;  // inner radius emc, 1.5cm electronics subtracted
  double albedo_thickness = 2.;
}  // namespace G4CEMCALBEDO

void CEmcAlbedoInit()
{
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 149.47);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -149.47);
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4CEMCALBEDO::inner_radius + G4CEMCALBEDO::teflon_cylinder_thickness + G4CEMCALBEDO::albedo_thickness);
}

void CEmcAlbedo(PHG4Reco *g4Reco)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::CEMCALBEDO_ABSORBER;
  PHG4CylinderSubsystem *cyl = new PHG4CylinderSubsystem("CEMC_ELECTRONICS", 0);
  cyl->SuperDetector("CEMC_MOCKUP");
  cyl->set_double_param("radius", G4CEMCALBEDO::inner_radius);
  cyl->set_string_param("material", "G4_TEFLON");
  cyl->set_double_param("thickness", G4CEMCALBEDO::teflon_cylinder_thickness);
  if (AbsorberActive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);
  cyl = new PHG4CylinderSubsystem("CEMC_ALBEDO", 1);
  cyl->SuperDetector("CEMC_MOCKUP");
  cyl->set_double_param("radius", G4CEMCALBEDO::inner_radius + G4CEMCALBEDO::teflon_cylinder_thickness);
  cyl->set_string_param("material", "Spacal_W_Epoxy");
  cyl->set_double_param("thickness", G4CEMCALBEDO::albedo_thickness);
  if (AbsorberActive) cyl->SetActive();
  g4Reco->registerSubsystem(cyl);
}
#endif
