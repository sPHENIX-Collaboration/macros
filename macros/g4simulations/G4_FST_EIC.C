#pragma once

#include "GlobalVariables.C"

#include <g4detectors/PHG4SectorSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <string>

R__LOAD_LIBRARY(libg4detectors.so)

int make_LANL_FST_station(string name, PHG4Reco *g4Reco, double zpos, double Rmin,
                          double Rmax,double tSilicon);
//-----------------------------------------------------------------------------------//
namespace Enable
{
  static bool FST = false;
}
//-----------------------------------------------------------------------------------//
void FST_Init()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 44.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 282.);
}
//-----------------------------------------------------------------------------------//
void FSTSetup(PHG4Reco *g4Reco, const double min_eta = 1.245)
{
  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = .1 * cm;
  const double um = 1e-3 * mm;

  //Design from Xuan Li @LANL
  make_LANL_FST_station("FST_0", g4Reco, 35,   4,   25, 35*um);  //cm
  make_LANL_FST_station("FST_1", g4Reco, 62.3, 4.5, 42, 35*um);
  make_LANL_FST_station("FST_2", g4Reco, 90,   5.2, 43, 35*um);
  make_LANL_FST_station("FST_3", g4Reco, 115,  6,   44, 85*um);
  make_LANL_FST_station("FST_4", g4Reco, 125,  6.5, 45, 85*um);
  make_LANL_FST_station("FST_5", g4Reco, 300,  15,  45, 85*um);
}
//-----------------------------------------------------------------------------------//
int make_LANL_FST_station(string name, PHG4Reco *g4Reco,
			  double zpos, double Rmin, double Rmax,double tSilicon) //silicon thickness
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

  double min_polar_angle = atan2(Rmin, zpos);
  double max_polar_angle = atan2(Rmax, zpos);

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
  fst->OverlapCheck(overlapcheck);

  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = .1 * cm;
  const double um = 1e-3 * mm;
  // build up layers

  fst->get_geometry().AddLayer("SiliconSensor", "G4_Si", tSilicon, true, 100);
  fst->get_geometry().AddLayer("Metalconnection", "G4_Al", 15 * um, false, 100);
  fst->get_geometry().AddLayer("HDI", "G4_KAPTON", 20 * um, false, 100);
  fst->get_geometry().AddLayer("Cooling", "G4_WATER", 100 * um, false, 100);
  fst->get_geometry().AddLayer("Support", "G4_GRAPHITE", 50 * um, false, 100);
  fst->get_geometry().AddLayer("Support_Gap", "G4_AIR", 1 * cm, false, 100);
  fst->get_geometry().AddLayer("Support2", "G4_GRAPHITE", 50 * um, false, 100);

  g4Reco->registerSubsystem(fst);
  return 0;
}
//-----------------------------------------------------------------------------------//
