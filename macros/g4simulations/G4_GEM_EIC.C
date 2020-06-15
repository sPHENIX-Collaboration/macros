#pragma once

#include "GlobalVariables.C"

#include <g4detectors/PHG4SectorSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <TMath.h>

#include <string>

R__LOAD_LIBRARY(libg4detectors.so)

int make_GEM_station(string name, PHG4Reco *g4Reco, double zpos, double etamin,
                     double etamax, const int N_Sector = 8);
void AddLayers_MiniTPCDrift(PHG4SectorSubsystem *gem);
int make_LANL_FST_station(string name, PHG4Reco *g4Reco, double zpos, double Rmin,
                          double Rmax);

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

  /// LANL FST From Xuan Li
  // plane 1, z location: 1200mm, inner radius: 45mm, outer radius: 350mm
  // plane 2, z location: 1400mm, inner radius: 50mm, outer radius: 390mm
  // plane 3, z location: 1700mm, inner radius: 60mm, outer radius: 410mm
  // plane 4, z location: 1900mm, inner radius: 70mm, outer radius: 430mm
  // We also need two outer barrel layers which might need some adjustments by the space limitation
  // and needs integration with the central vertex detector.
  //
  // Note1:  last station need to be removed to avoid confliction with the gas RICH. GEM chamber at z=2.7m is used instead
  // Note2:  increase inner radius for beam pipe flange clearance

  make_LANL_FST_station("FST_0", g4Reco, 17, 3.2, 18);
  make_LANL_FST_station("FST_1", g4Reco, 62, 3.2, 18);

  ///////////////////////////////////////////////////////////////////////////

  make_LANL_FST_station("FST_2", g4Reco, 120, 10, 35);

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

  make_LANL_FST_station("FST_3", g4Reco, 140, 12, 41);

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

  make_LANL_FST_station("FST_4", g4Reco, 160, 12, 41);

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

  ///////////////////////////////////////////////////////////////////////////

  make_LANL_FST_station("FST_5", g4Reco, 280, 17, 41);

  ///////////////////////////////////////////////////////////////////////////
}

int make_LANL_FST_station(string name, PHG4Reco *g4Reco, double zpos, double Rmin,
                          double Rmax)
{
  //  cout
  //      << "make_GEM_station - GEM construction with PHG4SectorSubsystem - make_GEM_station_EdgeReadout  of "
  //      << name << endl;

  // always facing the interaction point
  double polar_angle = 0;
  if (zpos < 0)
  {
    zpos = -zpos;
    polar_angle = M_PI;
  }

  double min_polar_angle = TMath::ATan2(Rmin, zpos);
  double max_polar_angle = TMath::ATan2(Rmax, zpos);

  if (max_polar_angle < min_polar_angle)
  {
    double t = max_polar_angle;
    max_polar_angle = min_polar_angle;
    min_polar_angle = t;
  }

  PHG4SectorSubsystem *fst;
  fst = new PHG4SectorSubsystem(name);

  fst->SuperDetector(name);

  fst->get_geometry().set_normal_polar_angle(polar_angle);
  fst->get_geometry().set_normal_start(zpos * PHG4Sector::Sector_Geometry::Unit_cm());
  fst->get_geometry().set_min_polar_angle(min_polar_angle);
  fst->get_geometry().set_max_polar_angle(max_polar_angle);
  fst->get_geometry().set_max_polar_edge(PHG4Sector::Sector_Geometry::ConeEdge());
  fst->get_geometry().set_min_polar_edge(PHG4Sector::Sector_Geometry::ConeEdge());
  fst->get_geometry().set_N_Sector(1);
  fst->get_geometry().set_material("G4_AIR");
  fst->OverlapCheck(Enable::OVERLAPCHECK);

  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = 0.1 * cm;
  const double um = 1e-3 * mm;
  // build up layers
  fst->get_geometry().AddLayer("SliconSensor", "G4_Si", 18 * um, true, 100);
  fst->get_geometry().AddLayer("Metalconnection", "G4_Al", 15 * um, false, 100);
  fst->get_geometry().AddLayer("SliconSupport", "G4_Al", 285 * um, false, 100);
  fst->get_geometry().AddLayer("HDI", "G4_KAPTON", 20 * um, false, 100);
  fst->get_geometry().AddLayer("Cooling", "G4_WATER", 100 * um, false, 100);
  fst->get_geometry().AddLayer("Support", "G4_GRAPHITE", 50 * um, false, 100);
  fst->get_geometry().AddLayer("Support_Gap", "G4_AIR", 1 * cm, false, 100);
  fst->get_geometry().AddLayer("Support2", "G4_GRAPHITE", 50 * um, false, 100);

  g4Reco->registerSubsystem(fst);
  return 0;
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
    polar_angle = TMath::Pi();
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
