// $Id: G4_RICH.C,v 1.2 2013/10/09 01:08:17 jinhuang Exp $

/*!
 * \file G4_RICH.C
 * \brief Setup the gas RICH detector as designed in ePHENIX LOI
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.2 $
 * \date $Date: 2013/10/09 01:08:17 $
 */

using namespace std;

// global macro parameters

void
RICHInit()
{
}

//! ePHENIX Gas RICH. Ref to Geometry parameter defined in ePHENIXRICH::RICH_Geometry
//! \param[in] N_RICH_Sector number of RICH sectors
//! \param[in] min_eta minimal eta coverage
//! \param[in] R_mirror_ref Radius for the reflection layer of the mirror
void
RICHSetup(PHG4Reco* g4Reco, //
	  const int N_RICH_Sector = 8, //
	  const double min_eta = 1.45, //
	  const double R_mirror_ref = 195, //cm
	  const double z_shift = 75, // cm
	  const double R_shift = 18.5 // cm
	  )
{

  /* Use dedicated RICH subsystem */
  PHG4RICHSubsystem *rich = new PHG4RICHSubsystem("RICH");
  rich->get_RICH_geometry().set_N_RICH_Sector(N_RICH_Sector);
  rich->get_RICH_geometry().set_min_eta(min_eta);

  //  rich->get_RICH_geometry().set_R_shift(10 * ePHENIXRICH::RICH_Geometry::Unit_cm()); // For compact RICH of 2<Eta<4

  rich->get_RICH_geometry().set_R_mirror_ref(R_mirror_ref * ePHENIXRICH::RICH_Geometry::Unit_cm());

  rich->get_RICH_geometry().set_z_shift(z_shift * ePHENIXRICH::RICH_Geometry::Unit_cm());
  rich->get_RICH_geometry().set_R_shift(R_shift * ePHENIXRICH::RICH_Geometry::Unit_cm());

  /* Register RICH module */
  rich->OverlapCheck( overlapcheck );

  g4Reco->registerSubsystem( rich );

}
