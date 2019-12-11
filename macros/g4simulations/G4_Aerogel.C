#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif
// $Id: G4_Aerogel.C,v 1.2 2013/10/09 01:08:17 jinhuang Exp $

/*!
 * \file G4_Aerogel.C
 * \brief Aerogel RICH for EIC detector
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.2 $
 * \date $Date: 2013/10/09 01:08:17 $
 */

void
AerogelInit()
{

}

void
AerogelSetup(PHG4Reco* g4Reco, const int N_Sector = 8, //
    const double min_eta = 1.242
    )
{

  PHG4SectorSubsystem *ag;
  ag = new PHG4SectorSubsystem("Aerogel");

  ag->get_geometry().set_normal_polar_angle(
      (PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta)
          + PHG4Sector::Sector_Geometry::eta_to_polar_angle(2)) / 2);
//  ag->get_geometry().set_normal_polar_angle(0);
  ag->get_geometry().set_normal_start(
      280 * PHG4Sector::Sector_Geometry::Unit_cm()); // 307
  ag->get_geometry().set_min_polar_angle(
      PHG4Sector::Sector_Geometry::eta_to_polar_angle(1.85));
  ag->get_geometry().set_max_polar_angle(
      PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta));
  ag->get_geometry().set_min_polar_edge(
      PHG4Sector::Sector_Geometry::FlatEdge());
  ag->get_geometry().set_material("G4_AIR");
  ag->get_geometry().set_N_Sector(N_Sector);
  ag->OverlapCheck(overlapcheck);

  // Aerogel dimensions ins cm
  double radiator_length = 2.;
  double expansion_length = 18.;// 10.;

  ag->get_geometry().AddLayers_AeroGel_ePHENIX( radiator_length * PHG4Sector::Sector_Geometry::Unit_cm(),
						expansion_length * PHG4Sector::Sector_Geometry::Unit_cm() );
  g4Reco->registerSubsystem(ag);

}

