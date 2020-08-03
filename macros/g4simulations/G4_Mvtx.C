#pragma once

#include "GlobalVariables.C"

#include <g4mvtx/PHG4MvtxDefs.h>
#include <g4mvtx/PHG4MvtxDigitizer.h>
#include <g4mvtx/PHG4MvtxHitReco.h>
#include <g4mvtx/PHG4MvtxSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <mvtx/MvtxClusterizer.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4mvtx.so)
R__LOAD_LIBRARY(libmvtx.so)

namespace Enable
{
  bool MVTX = false;
  bool MVTX_OVERLAPCHECK = false;
  bool MVTX_CELL = false;
  bool MVTX_CLUSTER = false;
  int MVTX_VERBOSITY = 0;

}  // namespace Enable

namespace G4MVTX
{
  int n_maps_layer = 3;        // must be 0-3, setting it to zero removes Mvtx completely, n < 3 gives the first n layers
  double radius_offset = 0.7;  // clearance around radius
}  // namespace G4MVTX

void MvtxInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, (PHG4MvtxDefs::mvtxdat[G4MVTX::n_maps_layer - 1][PHG4MvtxDefs::kRmd]) / 10. + G4MVTX::radius_offset);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 16.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -17.);
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
    if (verbosity)
    {
      cout << "Create Maps layer " << ilayer << " with radius " << radius_lyr << " mm." << endl;
    }
    radius = radius_lyr / 10.;
  }
  mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "stave_geometry_file", string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/mvtx_stave_v1.gdml"));
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
  for (int ilayer = 0; ilayer < G4MVTX::n_maps_layer; ilayer++)
  {
    // override the default timing window for this layer - default is +/- 5000 ns
    maps_hits->set_timing_window(ilayer, -5000, 5000);
  }
  se->registerSubsystem(maps_hits);
  return;
}

void Mvtx_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();
  PHG4MvtxDigitizer* digimvtx = new PHG4MvtxDigitizer();
  digimvtx->Verbosity(verbosity);
  // energy deposit in 25 microns = 9.6 KeV = 1000 electrons collected after recombination
  //digimvtx->set_adc_scale(0.95e-6);  // default set in code is 0.95e-06, which is 99 electrons
  se->registerSubsystem(digimvtx);
  // For the Mvtx layers
  //================
  MvtxClusterizer* mvtxclusterizer = new MvtxClusterizer("MvtxClusterizer");
  mvtxclusterizer->Verbosity(verbosity);
  se->registerSubsystem(mvtxclusterizer);
}
