#ifndef MACRO_G4AEROGEL_C
#define MACRO_G4AEROGEL_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4SectorSubsystem.h>

#include <g4main/PHG4Reco.h>

/*!
 * \file G4_Aerogel.C
 * \brief Aerogel RICH for EIC detector
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.2 $
 * \date $Date: 2013/10/09 01:08:17 $
 */

namespace Enable
{
  bool AEROGEL = false;
  bool AEROGEL_OVERLAPCHECK = false;
}  // namespace Enable

void AerogelInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 164.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 287.);
}

void AerogelSetup(PHG4Reco* g4Reco, const int N_Sector = 8,  //
                  const double min_eta = 1.242)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::AEROGEL_OVERLAPCHECK;

  PHG4SectorSubsystem* ag = new PHG4SectorSubsystem("Aerogel");

  ag->get_geometry().set_normal_polar_angle((PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta) +
                                             PHG4Sector::Sector_Geometry::eta_to_polar_angle(2)) /
                                            2);
  //  ag->get_geometry().set_normal_polar_angle(0);
  ag->get_geometry().set_normal_start(280 * PHG4Sector::Sector_Geometry::Unit_cm());  // 307
  ag->get_geometry().set_min_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(1.85));
  ag->get_geometry().set_max_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta));
  ag->get_geometry().set_min_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());
  ag->get_geometry().set_material("G4_AIR");
  ag->get_geometry().set_N_Sector(N_Sector);
  ag->OverlapCheck(OverlapCheck);

  // Aerogel dimensions ins cm
  double radiator_length = 2.;
  double expansion_length = 18.;  // 10.;

  ag->get_geometry().AddLayers_AeroGel_ePHENIX(radiator_length * PHG4Sector::Sector_Geometry::Unit_cm(),
                                               expansion_length * PHG4Sector::Sector_Geometry::Unit_cm());
  g4Reco->registerSubsystem(ag);
}
#endif
