#ifndef MACRO_G4GEMEIC_C
#define MACRO_G4GEMEIC_C

#include <GlobalVariables.C>

#include <g4detectors/PHG4SectorSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <string>

R__LOAD_LIBRARY(libg4detectors.so)

int make_GEM_station(string name, PHG4Reco *g4Reco, double zpos, double etamin, double etamax, const int N_Sector = 8);
void AddLayers_MiniTPCDrift(PHG4SectorSubsystem *gem);

namespace Enable
{
  bool EGEM = false;
  bool FGEM = false;
}  // namespace Enable

void EGEM_Init()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 80.);
  // extends only to -z
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -160.);
}

void FGEM_Init()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 150.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 282.);
}

void EGEMSetup(PHG4Reco *g4Reco)
{
  /* Careful with dimensions! If GEM station volumes overlap, e.g. with TPC volume, they will be
   * drawn in event display but will NOT register any hits.
   *
   * Geometric constraints:
   * TPC length = 211 cm --> from z = -105.5 to z = +105.5
   */
  float thickness = 3.;
  make_GEM_station("EGEM_0", g4Reco, -20.5 + thickness, -0.94, -1.95);
  make_GEM_station("EGEM_1", g4Reco, -69.5 + thickness, -2.07, -3.21);
  make_GEM_station("EGEM_2", g4Reco, -137.0 + thickness, -1.4, -3.5);
  make_GEM_station("EGEM_3", g4Reco, -160.0 + thickness, -1.5, -3.6);
}

void FGEMSetup(PHG4Reco *g4Reco, const int N_Sector = 8,  //
               const double min_eta = 1.245               //
)
{
  const double tilt = .1;

  string name;
  double etamax;
  double etamin;
  double zpos;
  PHG4SectorSubsystem *gem;

  ///////////////////////////////////////////////////////////////////////////

  name = "FGEM_2";
  etamax = 2;
  etamin = min_eta;
  zpos = 134.0;

  gem = new PHG4SectorSubsystem(name);

  gem->get_geometry().set_normal_polar_angle(tilt);
  gem->get_geometry().set_normal_start(zpos * PHG4Sector::Sector_Geometry::Unit_cm(), 0);
  gem->get_geometry().set_min_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamin));
  gem->get_geometry().set_max_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->OverlapCheck(Enable::OVERLAPCHECK);
  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);

  ///////////////////////////////////////////////////////////////////////////

  name = "FGEM_3";
  etamax = 2;
  etamin = min_eta;
  zpos = 157.0;

  gem = new PHG4SectorSubsystem(name + "_LowerEta");
  gem->SuperDetector(name);

  zpos = zpos - (zpos * sin(tilt) + zpos * cos(tilt) * tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax) - tilt)) * sin(tilt);

  gem->get_geometry().set_normal_polar_angle((PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta) + PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax)) / 2);
  gem->get_geometry().set_normal_start(
      zpos * PHG4Sector::Sector_Geometry::Unit_cm(),
      PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_min_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta));
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->get_geometry().set_min_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());

  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  gem->OverlapCheck(Enable::OVERLAPCHECK);
  g4Reco->registerSubsystem(gem);

  ///////////////////////////////////////////////////////////////////////////

  name = "FGEM_4";
  etamax = 3.5;
  etamin = min_eta;
  zpos = 271.0;
  gem = new PHG4SectorSubsystem(name);

  gem->SuperDetector(name);
  gem->get_geometry().set_normal_polar_angle(tilt);
  gem->get_geometry().set_normal_start(zpos * PHG4Sector::Sector_Geometry::Unit_cm(), 0);
  gem->get_geometry().set_min_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_max_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->OverlapCheck(Enable::OVERLAPCHECK);
  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);

  ///////////////////////////////////////////////////////////////////////////

  zpos = zpos - (zpos * sin(tilt) + zpos * cos(tilt) * tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(2) - tilt)) * sin(tilt);

  gem = new PHG4SectorSubsystem(name + "_LowerEta");
  gem->SuperDetector(name);

  gem->get_geometry().set_normal_polar_angle((PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta) + PHG4Sector::Sector_Geometry::eta_to_polar_angle(2)) / 2);
  gem->get_geometry().set_normal_start(
      zpos * PHG4Sector::Sector_Geometry::Unit_cm(),
      PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_min_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(2));
  gem->get_geometry().set_max_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta));
  gem->get_geometry().set_material("G4_METHANE");
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->get_geometry().set_min_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());

  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  gem->OverlapCheck(Enable::OVERLAPCHECK);
  g4Reco->registerSubsystem(gem);
}

//! Add drift layers to mini TPC
void AddLayers_MiniTPCDrift(PHG4SectorSubsystem *gem)
{
  assert(gem);

  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = 0.1 * cm;
  const double um = 1e-3 * mm;

  //  const int N_Layers = 70; // used for mini-drift TPC timing digitalization
  const int N_Layers = 1;  // simplified setup
  const double thickness = 2 * cm;

  gem->get_geometry().AddLayer("EntranceWindow", "G4_MYLAR", 25 * um, false, 100);
  gem->get_geometry().AddLayer("Cathode", "G4_GRAPHITE", 10 * um, false, 100);

  for (int d = 1; d <= N_Layers; d++)
  {
    ostringstream s;
    s << "DriftLayer_";
    s << d;

    gem->get_geometry().AddLayer(s.str(), "G4_METHANE", thickness / N_Layers, true);
  }
}

int make_GEM_station(string name, PHG4Reco *g4Reco, double zpos, double etamin,
                     double etamax, const int N_Sector = 8)
{
  //  cout
  //      << "make_GEM_station - GEM construction with PHG4SectorSubsystem - make_GEM_station_EdgeReadout  of "
  //      << name << endl;

  double polar_angle = 0;

  if (zpos < 0)
  {
    zpos = -zpos;
    polar_angle = M_PI;
  }
  if (etamax < etamin)
  {
    double t = etamax;
    etamax = etamin;
    etamin = t;
  }

  PHG4SectorSubsystem *gem;
  gem = new PHG4SectorSubsystem(name);

  gem->SuperDetector(name);

  gem->get_geometry().set_normal_polar_angle(polar_angle);
  gem->get_geometry().set_normal_start(zpos * PHG4Sector::Sector_Geometry::Unit_cm());
  gem->get_geometry().set_min_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamax));
  gem->get_geometry().set_max_polar_angle(PHG4Sector::Sector_Geometry::eta_to_polar_angle(etamin));
  gem->get_geometry().set_max_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_min_polar_edge(PHG4Sector::Sector_Geometry::FlatEdge());
  gem->get_geometry().set_N_Sector(N_Sector);
  gem->get_geometry().set_material("G4_METHANE");
  gem->OverlapCheck(Enable::OVERLAPCHECK);

  AddLayers_MiniTPCDrift(gem);
  gem->get_geometry().AddLayers_HBD_GEM();
  g4Reco->registerSubsystem(gem);
  return 0;
}
#endif
