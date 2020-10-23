#ifndef MACRO_G4MVTXEIC_C
#define MACRO_G4MVTXEIC_C

#include <GlobalVariables.C>

#include <g4mvtx/PHG4EICMvtxSubsystem.h>
#include <g4mvtx/PHG4MvtxDefs.h>

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4mvtx.so)

namespace Enable
{
  bool MVTX = false;
  bool MVTX_OVERLAPCHECK = false;
  int MVTX_VERBOSITY = 0;

}  // namespace Enable

namespace G4MVTX
{
  int n_maps_layer = 3;  // must be 0-3, setting it to zero removes Mvtx completely, n < 3 gives the first n layers
  vector<int> N_staves = {18, 24, 30};
  vector<double> nom_radius = {36.4, 48.1, 59.8};
}  // namespace G4MVTX

void MvtxInit(int verbosity = 0)
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4MVTX::nom_radius[G4MVTX::n_maps_layer - 1] / 10. + 0.7);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 16.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -17.);
}

double Mvtx(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0)
{
  bool maps_overlapcheck = Enable::OVERLAPCHECK || Enable::MVTX_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);

  // Update EIC MAPS layer structure based on inner two layers of U. Birmingham tracker

  PHG4EICMvtxSubsystem* mvtx = new PHG4EICMvtxSubsystem("MVTX");
  mvtx->Verbosity(verbosity);

  // H?kan Wennl?f <hwennlof@kth.se> :
  //    Without time-stamping layer:
  //    Stave type  Length  Overlap Radius [mm] Tilt  Radiation length  Number of staves
  //    ALICE inner 270 mm  2 mm  36.4  12.0 deg  0.3 % X0  18
  //    ALICE inner 270 mm  2 mm  59.8  12.0 deg  0.3 % X0  30
  //    ALICE outer 840 mm  4 mm  133.8 6.0 deg 0.8 % X0  16
  //    ALICE outer 840 mm  4 mm  180 6.0 deg 0.8 % X0  21

  //    int N_staves[G4MVTX::n_maps_layer] = {18, 24, 30};
  //    double nom_radius[G4MVTX::n_maps_layer] = {36.4, 48.1,  59.8};
  if (G4MVTX::N_staves.size() < G4MVTX::n_maps_layer)
  {
    cout << "vector<int> N_staves too small: " << G4MVTX::N_staves.size()
         << " needs to be at least of size " << G4MVTX::n_maps_layer << endl;
    gSystem->Exit(1);
  }
  if (G4MVTX::nom_radius.size() < G4MVTX::n_maps_layer)
  {
    cout << "vector<double> nom_radius too small: " << G4MVTX::nom_radius.size()
         << " needs to be at least of size " << G4MVTX::n_maps_layer << endl;
    gSystem->Exit(1);
  }
  for (int ilyr = 0; ilyr < G4MVTX::n_maps_layer; ilyr++)
  {
    mvtx->set_int_param(ilyr, "active", 1);  //non-automatic initialization in PHG4DetectorGroupSubsystem
    mvtx->set_int_param(ilyr, "layer", ilyr);
    mvtx->set_int_param(ilyr, "N_staves", G4MVTX::N_staves[ilyr]);
    mvtx->set_double_param(ilyr, "layer_nominal_radius", G4MVTX::nom_radius[ilyr]);  // mm
    mvtx->set_double_param(ilyr, "phitilt", 12.0 * 180. / M_PI + M_PI);
    mvtx->set_double_param(ilyr, "phi0", 0);
  }

  mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "stave_geometry_file", string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/mvtx_stave_v1.gdml"));
  mvtx->SetActive(1);
  mvtx->OverlapCheck(maps_overlapcheck);
  g4Reco->registerSubsystem(mvtx);
  return G4MVTX::nom_radius[G4MVTX::n_maps_layer - 1] / 10.;  // return cm
}
#endif
