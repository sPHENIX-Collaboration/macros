#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4main/PHG4Reco.h>
int make_GEM_station(string name, PHG4Reco* g4Reco, double zpos, double etamin,
                 double etamax,  const int N_Sector = 8);
void AddLayers_MiniTPCDrift(PHG4SectorSubsystem *gem);
R__LOAD_LIBRARY(libg4detectors.so)
#endif

void
EGEM_Init()
{

}

void
FGEM_Init()
{

}

void
EGEMSetup(PHG4Reco* g4Reco)
{
  /* Careful with dimensions! If GEM station volumes overlap, e.g. with TPC volume, they will be
   * drawn in event display but will NOT register any hits.
   *
   * Geometric constraints:
   * TPC length = 211 cm --> from z = -105.5 to z = +105.5
   */
  float thickness=3.;
  make_GEM_station("EGEM_0", g4Reco,  -20.5 + thickness, -0.94, -1.95);
  make_GEM_station("EGEM_1", g4Reco,  -69.5 + thickness, -2.07, -3.21);
  make_GEM_station("EGEM_2", g4Reco, -137.0 + thickness, -1.4, -3.9);
  make_GEM_station("EGEM_3", g4Reco, -160.0 + thickness, -1.5, -4.00);
}

void
FGEMSetup(PHG4Reco* g4Reco, const int N_Sector = 8, //
          const double min_eta = 1.245 //
          )
{
  const double tilt = .1;

  string name;
  double etamax;
  double etamin;
  double zpos;
  PHG4SectorSubsystem *gem;

  make_GEM_station("FGEM_0", g4Reco, 17.5, 0.94, 2.73, N_Sector);
  make_GEM_station("FGEM_1", g4Reco, 66.5, 2.07, 4.00, N_Sector);

  ///////////////////////////////////////////////////////////////////////////

  name = "FGEM_2";
  etamax = 4;
  etamin = min_eta;
  zpos = 134.0;

  gem = new PHG4SectorSubsystem(name.c_str());

  gem->get_geometry().set_normal_polar_angle(tilt);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm(), 0);
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamin));
  gem->get_geometry().set_max_polar_edge(
                                         PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->OverlapCheck(overlapcheck);
  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);

  ///////////////////////////////////////////////////////////////////////////

  name = "FGEM_3";
  etamax = 4;
  etamin = min_eta;
  zpos = 157.0;
  gem = new PHG4SectorSubsystem(name.c_str());

  gem->SuperDetector(name);
  gem->get_geometry().set_normal_polar_angle(tilt);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm(), 0);
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_max_polar_edge(
                                         PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->OverlapCheck(overlapcheck);
  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);

  gem = new PHG4SectorSubsystem(name + "_LowerEta");
  gem->SuperDetector(name);

  zpos = zpos
    - (zpos * sin(tilt)
       + zpos * cos(tilt)
       * tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(2) - tilt))
    * sin(tilt);

  gem->get_geometry().set_normal_polar_angle(
                                             (PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta)
                                              + PHG4Sector::Sector_Geometry::eta_to_polar_angle(2)) / 2);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm(),
                                       PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta));
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->get_geometry().set_min_polar_edge(
                                         PHG4Sector::Sector_Geometry::FlatEdge());

  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  gem->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(gem);

  ///////////////////////////////////////////////////////////////////////////

  name = "FGEM_4";
  etamax = 4;
  etamin = min_eta;
  zpos = 271.0;
  gem = new PHG4SectorSubsystem(name.c_str());

  gem->SuperDetector(name);
  gem->get_geometry().set_normal_polar_angle(tilt);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm(), 0);
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_max_polar_edge(
                                         PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->OverlapCheck(overlapcheck);
  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);

  zpos = zpos
    - (zpos * sin(tilt)
       + zpos * cos(tilt)
       * tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(2) - tilt))
    * sin(tilt);

  gem = new PHG4SectorSubsystem(name + "_LowerEta");
  gem->SuperDetector(name);

  gem->get_geometry().set_normal_polar_angle(
                                             (PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta)
                                              + PHG4Sector::Sector_Geometry::eta_to_polar_angle(2)) / 2);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm(),
                                       PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta));
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->get_geometry().set_min_polar_edge(
                                         PHG4Sector::Sector_Geometry::FlatEdge());

  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  gem->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(gem);

  ///////////////////////////////////////////////////////////////////////////

}

//! Add drift layers to mini TPC
void
AddLayers_MiniTPCDrift(PHG4SectorSubsystem *gem)
{
  assert(gem);

  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = .1 * cm;
  const double um = 1e-3 * mm;

  //  const int N_Layers = 70; // used for mini-drift TPC timing digitalization
  const int N_Layers = 1; // simplified setup
  const double thickness = 2 * cm;

  gem->get_geometry().AddLayer("EntranceWindow", "G4_MYLAR", 25 * um, false,
                               100);
  gem->get_geometry().AddLayer("Cathode", "G4_GRAPHITE", 10 * um, false, 100);

  for (int d = 1; d <= N_Layers; d++)
    {
      stringstream s;
      s << "DriftLayer_";
      s << d;

      gem->get_geometry().AddLayer(s.str(), "G4_METHANE", thickness / N_Layers,
                                   true);

    }
}

int
make_GEM_station(string name, PHG4Reco* g4Reco, double zpos, double etamin,
                 double etamax,  const int N_Sector = 8)
{

  //  cout
  //      << "make_GEM_station - GEM construction with PHG4SectorSubsystem - make_GEM_station_EdgeReadout  of "
  //      << name << endl;

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

  gem->SuperDetector(name);

  gem->get_geometry().set_normal_polar_angle(polar_angle);
  gem->get_geometry().set_normal_start(
                                       zpos * PHG4Sector::Sector_Geometry::Unit_cm());
  gem->get_geometry().set_min_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(
                                          PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamin));
  gem->get_geometry().set_max_polar_edge(
                                         PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_min_polar_edge(
                                         PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->get_geometry().set_material("G4_METHANE");
  gem->OverlapCheck(overlapcheck);

  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);
  return 0;
}
