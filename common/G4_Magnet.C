#ifndef MACRO_G4MAGNET_C
#define MACRO_G4MAGNET_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool MAGNET = false;
  bool MAGNET_ABSORBER = false;
  bool MAGNET_OVERLAPCHECK = false;
  int MAGNET_VERBOSITY = 0;
}  // namespace Enable

namespace G4MAGNET
{
  double magnet_outer_cryostat_wall_radius = 174.5;
  double magnet_outer_cryostat_wall_thickness = 2.5;
  double magnet_length = 379.;
}  // namespace G4MAGNET

void MagnetFieldInit()
{
  if (!isfinite(G4MAGNET::magfield_rescale))
  {
    G4MAGNET::magfield_rescale = 1.;
  }
  if (G4MAGNET::magfield.empty())
  {
    G4MAGNET::magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sphenix3dbigmapxyz_gap_rebuild.root");
    G4MAGNET::magfield_OHCAL_steel = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sphenix3dbigmapxyz_steel_rebuild.root");
  }
}

void MagnetInit()
{
  MagnetFieldInit();
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4MAGNET::magnet_outer_cryostat_wall_radius + G4MAGNET::magnet_outer_cryostat_wall_thickness);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4MAGNET::magnet_length / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -G4MAGNET::magnet_length / 2.);
}

double Magnet(PHG4Reco* g4Reco, double radius)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::MAGNET_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::MAGNET_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::MAGNET_VERBOSITY);

  double magnet_inner_cryostat_wall_radius = 142;
  double magnet_inner_cryostat_wall_thickness = 1;
  double magnet_coil_radius = 150.8;
  double magnet_coil_thickness = 9.38;
  double magnet_length = 379.;
  double coil_length = 361.5;
  if (radius > magnet_inner_cryostat_wall_radius)
  {
    cout << "inconsistency: radius: " << radius
         << " larger than Magnet inner radius: " << magnet_inner_cryostat_wall_radius << endl;
    gSystem->Exit(-1);
  }

  radius = magnet_inner_cryostat_wall_radius;
  PHG4CylinderSubsystem* cyl = new PHG4CylinderSubsystem("MAGNET", 0);
  cyl->set_double_param("radius", magnet_inner_cryostat_wall_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4MAGNET::magnet_length);
  cyl->set_double_param("thickness", magnet_inner_cryostat_wall_thickness);
  cyl->set_string_param("material", "Al5083");  // use 1 radiation length Al for magnet thickness
  cyl->SuperDetector("MAGNET");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("MAGNET", 1);
  cyl->set_double_param("radius", magnet_coil_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", coil_length);
  cyl->set_double_param("thickness", magnet_coil_thickness);
  cyl->set_string_param("material", "Al5083");  // use 1 radiation length Al for magnet thickness
  cyl->SuperDetector("MAGNET");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  cyl = new PHG4CylinderSubsystem("MAGNET", 2);
  cyl->set_double_param("radius", G4MAGNET::magnet_outer_cryostat_wall_radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4MAGNET::magnet_length);
  cyl->set_double_param("thickness", G4MAGNET::magnet_outer_cryostat_wall_thickness);
  cyl->set_string_param("material", "Al5083");  // use 1 radiation length Al for magnet thickness
  cyl->SuperDetector("MAGNET");
  if (AbsorberActive) cyl->SetActive();
  cyl->OverlapCheck(OverlapCheck);
  g4Reco->registerSubsystem(cyl);

  radius = G4MAGNET::magnet_outer_cryostat_wall_radius + G4MAGNET::magnet_outer_cryostat_wall_thickness;  // outside of magnet

  if (verbosity > 0)
  {
    cout << "========================= G4_Magnet.C::Magnet() ===========================" << endl;
    cout << " MAGNET Material Description:" << endl;
    cout << "  inner radius = " << magnet_inner_cryostat_wall_radius << " cm" << endl;
    cout << "  outer radius = " << G4MAGNET::magnet_outer_cryostat_wall_radius + G4MAGNET::magnet_outer_cryostat_wall_thickness << " cm" << endl;
    cout << "  length = " << G4MAGNET::magnet_length << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;

  return radius;
}
#endif
