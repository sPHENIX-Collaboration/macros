#ifndef MACRO_G4MVTX_C
#define MACRO_G4MVTX_C

#include <GlobalVariables.C>
//#include <Trkr_TruthTables.C>

#include <QA.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4mvtx/PHG4MvtxDefs.h>
#include <g4mvtx/PHG4MvtxDigitizer.h>
#include <g4mvtx/PHG4MvtxHitReco.h>
#include <g4mvtx/PHG4MvtxSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <mvtx/MvtxHitPruner.h>
#include <mvtx/MvtxClusterizer.h>
#include <qa_modules/QAG4SimulationMvtx.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4mvtx.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libqa_modules.so)


void MvtxInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 12.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -165.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 24.);
}

double Mvtx(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0)
{
  bool maps_overlapcheck = Enable::OVERLAPCHECK || Enable::MVTX_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);

  PHG4MvtxSubsystem* mvtx = new PHG4MvtxSubsystem("MVTX");
  mvtx->Verbosity(verbosity);

  for (int ilayer = 0; ilayer < G4MVTX::n_maps_layer; ilayer++)
  {
    double radius_lyr = PHG4MvtxDefs::mvtxdat[ilayer][PHG4MvtxDefs::kRmd];
//    mvtx->set_double_param(ilayer, "layer_z_offset", G4MVTXAlignment::z_offset[ilayer]);
    if (verbosity)
    {
      cout << "Create Maps layer " << ilayer << " with radius " << radius_lyr << " mm." << endl;
    }
    radius = radius_lyr / 10.;
  }
//  mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "alignment_path",  G4MVTXAlignment::alignment_path);
  mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "stave_geometry_file", string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/mvtx_stave.gdml"));

  mvtx->SetActive();
  mvtx->OverlapCheck(maps_overlapcheck);
  g4Reco->registerSubsystem(mvtx);
  radius += G4MVTX::radius_offset;
  return radius;
}

// Central detector cell reco is disabled as EIC setup use the fast tracking sim for now
void Mvtx_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();
  // new storage containers
  PHG4MvtxHitReco* maps_hits = new PHG4MvtxHitReco("MVTX");
  maps_hits->Verbosity(verbosity);

  double maps_readout_window = 5000.0;  // ns
  double extended_readout_time = 0.0;
  if(TRACKING::pp_mode) extended_readout_time = TRACKING::pp_extended_readout_time;
  // override the default timing window - default is +/- 5000 ns
  maps_hits->set_double_param("mvtx_tmin",  -maps_readout_window);
  maps_hits->set_double_param("mvtx_tmax",  maps_readout_window + extended_readout_time);

  std::cout << "PHG4MvtxHitReco: readout window is from " << -maps_readout_window << " to " <<  maps_readout_window + extended_readout_time << std::endl;
  se->registerSubsystem(maps_hits);

  PHG4MvtxDigitizer* digimvtx = new PHG4MvtxDigitizer();
  digimvtx->Verbosity(verbosity);
  // energy deposit in 25 microns = 9.6 KeV = 1000 electrons collected after recombination
  //digimvtx->set_adc_scale(0.95e-6);  // default set in code is 0.95e-06, which is 99 electrons
  se->registerSubsystem(digimvtx);

  return;
}

void Mvtx_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

 // prune the extra MVTX hits due to multiple strobes per hit
  MvtxHitPruner* mvtxhitpruner = new MvtxHitPruner();
  mvtxhitpruner->Verbosity(verbosity);
  se->registerSubsystem(mvtxhitpruner);

  // For the Mvtx layers
  //================
  MvtxClusterizer* mvtxclusterizer = new MvtxClusterizer("MvtxClusterizer");
  mvtxclusterizer->Verbosity(verbosity);
  mvtxclusterizer->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(mvtxclusterizer);
}

void Mvtx_QA()
{
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::MVTX_VERBOSITY);

  Fun4AllServer* se = Fun4AllServer::instance();
  QAG4SimulationMvtx* qa = new QAG4SimulationMvtx;
  qa->Verbosity(verbosity);
  qa->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(qa);
}

#endif
