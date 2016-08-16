// $Id: G4_FGEM_ePHENIX.C,v 1.4 2013/10/13 21:45:27 jinhuang Exp $

/*!
 * \file G4_FGEM_ePHENIX.C
 * \brief
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.4 $
 * \date $Date: 2013/10/13 21:45:27 $
 */

void
EGEM_Init()
{

}

void
EGEMSetup(PHG4Reco* g4Reco)
{
  make_GEM_station("EGEM_0", g4Reco, -32., -1.6, -3.4);
  make_GEM_station("EGEM_1", g4Reco, -58., -2.1, -3.98); // reduce max eta from -4.0 to -3.98 to avoid overlap with volume S_AL_PIPE_5 (aluminum beam pipe)
  make_GEM_station("EGEM_2", g4Reco, -101., -1.2, -4.5);
}

int
make_GEM_station(string name, PHG4Reco* g4Reco, double zpos, double etamin,
                 double etamax)
{

  //  cout
  //    << "make_GEM_station - GEM construction with PHG4SectorSubsystem - make_GEM_station_EdgeReadout  of "
  //    << name << endl;

  double polar_angle = 0;

  if (zpos < 0)
    {
      zpos = -zpos;
      polar_angle = TMath::Pi();

    }
  if (etamax < etamin)
    {
      double t = etamax;
      etamax = etamin;
      etamin = t;
    }

  PHG4SectorSubsystem *gem;
  gem = new PHG4SectorSubsystem(name.c_str());

  gem->get_geometry().set_normal_polar_angle(polar_angle);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm());
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamin));
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(1);
  gem->OverlapCheck(overlapcheck);

  gem->get_geometry().AddLayers_DriftVol_COMPASS();
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);

}

