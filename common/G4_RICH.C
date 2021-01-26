/*!
 * \file G4_RICH.C
 * \brief Setup the gas RICH detector as designed in ePHENIX LOI
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.2 $
 * \date $Date: 2013/10/09 01:08:17 $
 */
#ifndef MACRO_G4RICH_C
#define MACRO_G4RICH_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4RICHSubsystem.h>

#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

namespace Enable
{
  bool RICH = false;
}

void RICHInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 135.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 268.);
}

//! ePHENIX Gas RICH. Ref to Geometry parameter defined in ePHENIXRICH::RICH_Geometry
//! \param[in] N_RICH_Sector number of RICH sectors
//! \param[in] min_eta minimal eta coverage
//! \param[in] R_mirror_ref Radius for the reflection layer of the mirror
void RICHSetup(PHG4Reco* g4Reco,                   //
               const int N_RICH_Sector = 8,        //
               const double min_eta = 1.3,         //
               const double R_mirror_ref = 190,    //cm // Reduced from 195 (2014 LOI) -> 190 to avoid overlap with FGEM4 (it seems to fit fine in the AutoCAD drawing- is the RICH longer in Geant4 than in the AutoCAD drawing?)
               const double z_shift = 75,          // cm
               const double R_shift = 18.5,        // cm
               const double R_beampipe_front = 8,  // clearance for EIC beam pipe flange
               const double R_beampipe_back = 27   // clearance for EIC beam pipe flange
)
{
  /* Use dedicated RICH subsystem */
  PHG4RICHSubsystem* rich = new PHG4RICHSubsystem("RICH");
  rich->get_RICH_geometry().set_N_RICH_Sector(N_RICH_Sector);
  rich->get_RICH_geometry().set_min_eta(min_eta);

  //  rich->get_RICH_geometry().set_R_shift(10 * ePHENIXRICH::RICH_Geometry::Unit_cm()); // For compact RICH of 2<Eta<4

  rich->get_RICH_geometry().set_R_mirror_ref(R_mirror_ref * ePHENIXRICH::RICH_Geometry::Unit_cm());

  rich->get_RICH_geometry().set_z_shift(z_shift * ePHENIXRICH::RICH_Geometry::Unit_cm());
  rich->get_RICH_geometry().set_R_shift(R_shift * ePHENIXRICH::RICH_Geometry::Unit_cm());

  rich->get_RICH_geometry().set_R_beam_pipe_front(R_beampipe_front * ePHENIXRICH::RICH_Geometry::Unit_cm());
  rich->get_RICH_geometry().set_R_beam_pipe_back(R_beampipe_back * ePHENIXRICH::RICH_Geometry::Unit_cm());

  rich->OverlapCheck(Enable::OVERLAPCHECK);

  /* Register RICH module */
  g4Reco->registerSubsystem(rich);
}
#endif
