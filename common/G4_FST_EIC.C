/*---------------------------------------------------------------------*
 * Barrel tracker designed by LANL EIC team                            *
 * See technical notes for details: arXiv:2009.02888                   *
 * Contact Ping and Xuan @LANL for questions:                          *
 *   Xuan: xuanli@lanl.gov                                             *
 *   Ping: cpwong@lanl.gov                                             *
 *---------------------------------------------------------------------*/

#ifndef MACRO_G4FSTEIC_C
#define MACRO_G4FSTEIC_C

#include "GlobalVariables.C"

#include <g4detectors/PHG4SectorSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <string>

R__LOAD_LIBRARY(libg4detectors.so)

int make_LANL_FST_station(const string &name, PHG4Reco *g4Reco, double zpos, double Rmin,
                          double Rmax, double tSilicon);
//-----------------------------------------------------------------------------------//
namespace Enable
{
  static bool FST = false;
}

namespace G4FST
{
  namespace SETTING
  {
    bool FSTV0 = false;
    bool FSTV1 = false;
    bool FSTV2 = false;
    bool FSTV3 = false;
    bool FSTV41 = false;
    bool FSTV42 = false;
    bool FSTV4 = false;
    bool FSTV5 = false;
    bool FST_TPC = false;
  }  // namespace SETTING
}  // namespace G4FST

//-----------------------------------------------------------------------------------//
void FST_Init()
{
  if ((G4FST::SETTING::FSTV0 ? 1 : 0) +
      (G4FST::SETTING::FSTV1 ? 1 : 0) +
      (G4FST::SETTING::FSTV2 ? 1 : 0) +
      (G4FST::SETTING::FSTV3 ? 1 : 0) +
      (G4FST::SETTING::FSTV4 ? 1 : 0) +
      (G4FST::SETTING::FSTV41 ? 1 : 0) +
      (G4FST::SETTING::FSTV42 ? 1 : 0) +
      (G4FST::SETTING::FSTV5 ? 1 : 0) +
      (G4FST::SETTING::FST_TPC ? 1 : 0) > 1)
  {
    cout << "use only G4FST::SETTING::FSTV0=true ";
    cout << "or G4FST::SETTING::FSTV1=true ";
    cout << "or G4FST::SETTING::FSTV2=true ";
    cout << "or G4FST::SETTING::FSTV3=true ";
    cout << "or G4FST::SETTING::FSTV4=true ";
    cout << "or G4FST::SETTING::FSTV41=true ";
    cout << "or G4FST::SETTING::FSTV42=true ";
    cout << "or G4FST::SETTING::FSTV5=true ";
    cout << "or G4FST::SETTING::FST_TPC=true "<< endl;
    gSystem->Exit(1);
  }

  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 45.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 282.);
}
//-----------------------------------------------------------------------------------//
void FSTSetup(PHG4Reco *g4Reco, const double min_eta = 1.245)
{
  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = .1 * cm;
  const double um = 1e-3 * mm;

  //Design from Xuan Li @LANL
  // Different FST version documented in arXiv:2009.0288
  if (G4FST::SETTING::FSTV1)
  {                                                              // version 1
    make_LANL_FST_station("FST_0", g4Reco, 35, 4, 25, 50 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 36, 50 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5, 36, 50 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101, 6, 38.5, 50 * um);
    make_LANL_FST_station("FST_4", g4Reco, 125, 6.5, 45, 50 * um);
  }
  else if (G4FST::SETTING::FSTV2)
  {                                                              // version 2
    make_LANL_FST_station("FST_0", g4Reco, 35, 4, 30, 50 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 35, 50 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5, 36, 50 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101, 6, 38.5, 50 * um);
    make_LANL_FST_station("FST_4", g4Reco, 270, 6.5, 45, 50 * um);
  }
  else if (G4FST::SETTING::FSTV3)
  {                                                              // version 3
    make_LANL_FST_station("FST_0", g4Reco, 35, 4, 25, 50 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 36, 50 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5, 36, 50 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101, 6, 38.5, 100 * um);
    make_LANL_FST_station("FST_4", g4Reco, 125, 6.5, 45, 100 * um);
  }
  else if (G4FST::SETTING::FSTV41)
  {                                                              // version 4.1
    make_LANL_FST_station("FST_0", g4Reco, 35, 4, 25, 50 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 36, 50 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5, 36, 50 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101, 6, 38.5, 50 * um);
    make_LANL_FST_station("FST_4", g4Reco, 125, 6.5, 45, 100 * um);
    make_LANL_FST_station("FST_5", g4Reco, 270, 15, 45, 100 * um);
  }
  else if (G4FST::SETTING::FSTV42)
  {                                                              // version 4.2
    make_LANL_FST_station("FST_0", g4Reco, 35, 4, 25, 50 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 36, 50 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5, 36, 50 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101, 6, 38.5, 100 * um);
    make_LANL_FST_station("FST_4", g4Reco, 125, 6.5, 45, 100 * um);
    make_LANL_FST_station("FST_5", g4Reco, 270, 15, 45, 100 * um);
  }
  else if (G4FST::SETTING::FSTV4)
  {                                                              // version 4
    make_LANL_FST_station("FST_0", g4Reco, 35, 4, 25, 50 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 36, 50 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5, 36, 50 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101, 6, 38.5, 50 * um);
    make_LANL_FST_station("FST_4", g4Reco, 125, 6.5, 45, 50 * um);
    make_LANL_FST_station("FST_5", g4Reco, 270, 15, 45, 50 * um);
  }
  else if (G4FST::SETTING::FSTV5)
  {
    make_LANL_FST_station("FST_0", g4Reco, 35,   4,   25, 35*um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 62.3, 4.5, 42, 35*um);
    make_LANL_FST_station("FST_2", g4Reco, 90,   6.5,   43, 35*um);
    make_LANL_FST_station("FST_3", g4Reco, 115,  8.9,   44, 85*um);
    make_LANL_FST_station("FST_4", g4Reco, 125,  9.5, 45, 85*um);
    make_LANL_FST_station("FST_5", g4Reco, 300,  16.8,  45, 85*um);  //optional disk at further location
  }
  else if (G4FST::SETTING::FST_TPC)
  {                                                                // tpc version (based on version 4)
    make_LANL_FST_station("FST_0", g4Reco, 35, 4,   17, 35 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 17, 35 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5,   17, 35 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101,7.5, 17, 85 * um);
    make_LANL_FST_station("FST_4", g4Reco, 125,9.5,   45, 85 * um);
    //make_LANL_FST_station("FST_5", g4Reco, 280, 16, 45, 50 * um);  //optional disk at further location
  }
  else
  {                                                               // Version 0
    make_LANL_FST_station("FST_0", g4Reco, 35, 4, 30, 100 * um);  //cm
    make_LANL_FST_station("FST_1", g4Reco, 53, 4.5, 35, 100 * um);
    make_LANL_FST_station("FST_2", g4Reco, 77, 5, 36, 100 * um);
    make_LANL_FST_station("FST_3", g4Reco, 101, 6, 38.5, 100 * um);
    make_LANL_FST_station("FST_4", g4Reco, 125, 6.5, 45, 100 * um);
  }
}
//-----------------------------------------------------------------------------------//
int make_LANL_FST_station(const string &name, PHG4Reco *g4Reco,
                          double zpos, double Rmin, double Rmax, double tSilicon)  //silicon thickness
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
  fst->OverlapCheck(true);

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

#endif

//-----------------------------------------------------------------------------------//
