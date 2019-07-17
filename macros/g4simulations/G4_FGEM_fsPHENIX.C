#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 00, 0)
#include <fun4all/Fun4AllServer.h>
#include <g4detectors/PHG4SectorSubsystem.h>
#include <g4eval/SvtxEvaluator.h>
#include <g4main/PHG4Reco.h>
#include <g4trackfastsim/PHG4TrackFastSim.h>
#include <g4trackfastsim/PHG4TrackFastSimEval.h>
#include "GlobalVariables.C"
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libg4trackfastsim.so)
int make_GEM_station(string name, PHG4Reco *g4Reco, double zpos, double etamin,
                     double etamax, const int N_Sector = 8);
void AddLayers_MiniTPCDrift(PHG4SectorSubsystem *gem);
#endif
// $Id: G4_FGEM_fsPHENIX.C,v 1.2 2014/01/22 01:44:13 jinhuang Exp $

/*!
 * \file G4_FGEM_fsPHENIX.C
 * \brief 
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.2 $
 * \date $Date: 2014/01/22 01:44:13 $
 */

using namespace std;

void FGEM_Init()
{
}

void FGEMSetup(PHG4Reco *g4Reco, const int N_Sector = 8,  //
               const double min_eta = 1.45                //
)
{
  const double tilt = .1;

  string name;
  double etamax;
  double etamin;
  double zpos;
  PHG4SectorSubsystem *gem;

  make_GEM_station("FGEM_0", g4Reco, 17, 1.01, 2.7, N_Sector);
  make_GEM_station("FGEM_1", g4Reco, 62, 2.15, 4.0, N_Sector);

  ///////////////////////////////////////////////////////////////////////////

  name = "FGEM_2";
  etamax = 4;
  etamin = min_eta;
  zpos = 1.2e2;

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
  zpos = 1.6e2;
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

  zpos = zpos - (zpos * sin(tilt) + zpos * cos(tilt) * tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(2) - tilt)) * sin(tilt);

  gem->get_geometry().set_normal_polar_angle(
      (PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta) + PHG4Sector::Sector_Geometry::eta_to_polar_angle(2)) / 2);
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
  zpos = 2.75e2;
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

  zpos = zpos - (zpos * sin(tilt) + zpos * cos(tilt) * tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(2) - tilt)) * sin(tilt);

  gem = new PHG4SectorSubsystem(name + "_LowerEta");
  gem->SuperDetector(name);

  gem->get_geometry().set_normal_polar_angle(
      (PHG4Sector::Sector_Geometry::eta_to_polar_angle(min_eta) + PHG4Sector::Sector_Geometry::eta_to_polar_angle(2)) / 2);
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
void AddLayers_MiniTPCDrift(PHG4SectorSubsystem *gem)
{
  assert(gem);

  const double cm = PHG4Sector::Sector_Geometry::Unit_cm();
  const double mm = .1 * cm;
  const double um = 1e-3 * mm;

  //  const int N_Layers = 70; // used for mini-drift TPC timing digitalization
  const int N_Layers = 1;  // simplified setup
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

void FGEM_FastSim_Reco(int verbosity = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4trackfastsim.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4TrackFastSim *kalman = new PHG4TrackFastSim("PHG4TrackFastSim");
  kalman->Verbosity(verbosity);

  kalman->set_use_vertex_in_fitting(true);
  kalman->set_vertex_xy_resolution(50E-4);
  kalman->set_vertex_z_resolution(50E-4);

  kalman->set_sub_top_node_name("SVTX");
  kalman->set_trackmap_out_name("SvtxTrackMap");

  //   MAPS in MVTX detector
  kalman->add_phg4hits(
      "G4HIT_MVTX",                //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
      5e-4,                        //      const float radres,
      5e-4,                        //      const float phires,
      5e-4,                        //      const float lonres,
      1,                           //      const float eff,
      0                            //      const float noise
  );

  // GEM0, 70um azimuthal resolution, 1cm radial strips
  kalman->add_phg4hits(
      "G4HIT_FGEM_0",                    //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
      1. / sqrt(12),                     //      const float radres,
      70e-4,                             //      const float phires,
      100e-4,                            //      const float lonres,
      1,                                 //      const float eff,
      0                                  //      const float noise
  );
  // GEM1, 70um azimuthal resolution, 1cm radial strips
  kalman->add_phg4hits(
      "G4HIT_FGEM_1",                    //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
      1. / sqrt(12),                     //      const float radres,
      70e-4,                            //      const float phires,
      100e-4,                            //      const float lonres,
      1,                                 //      const float eff,
      0                                  //      const float noise
  );

  // TPC
  kalman->add_phg4hits(
      "G4HIT_TPC",                 //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Cylinder,  //      const DETECTOR_TYPE phg4dettype,
      1,                           //      const float radres,
      200e-4,                      //      const float phires,
      500e-4,                      //      const float lonres,
      1,                           //      const float eff,
      0                            //      const float noise
  );

  // GEM2, 70um azimuthal resolution, 1cm radial strips
  kalman->add_phg4hits(
      "G4HIT_FGEM_2",                    //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
      1. / sqrt(12),                     //      const float radres,
      70e-4,                             //      const float phires,
      100e-4,                            //      const float lonres,
      1,                                 //      const float eff,
      0                                  //      const float noise
  );
  // GEM3, 70um azimuthal resolution, 1cm radial strips
  kalman->add_phg4hits(
      "G4HIT_FGEM_3",                    //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
      1. / sqrt(12),                     //      const float radres,
      70e-4,                             //      const float phires,
      100e-4,                            //      const float lonres,
      1,                                 //      const float eff,
      0                                  //      const float noise
  );
  // GEM4, 70um azimuthal resolution, 1cm radial strips
  kalman->add_phg4hits(
      "G4HIT_FGEM_4",                    //      const std::string& phg4hitsNames,
      PHG4TrackFastSim::Vertical_Plane,  //      const DETECTOR_TYPE phg4dettype,
      1. / sqrt(12),                     //      const float radres,
      70e-4,                             //      const float phires,
      100e-4,                            //      const float lonres,
      1,                                 //      const float eff,
      0                                  //      const float noise
  );

  // Saved track states (projections)
  kalman->add_state_name("FEMC");
  kalman->add_state_name("FHCAL");

  se->registerSubsystem(kalman);
}

void FGEM_FastSim_Eval(std::string outputfile, int verbosity = 0)
{
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4trackfastsim.so");

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4TrackFastSimEval *fast_sim_eval = new PHG4TrackFastSimEval("FastTrackingEval");
  fast_sim_eval->set_filename(outputfile.c_str());
  se->registerSubsystem(fast_sim_eval);

  return;
}
