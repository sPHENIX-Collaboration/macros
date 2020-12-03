#ifndef MACRO_G4TPCEIC_C
#define MACRO_G4TPCEIC_C

#include <GlobalVariables.C>

#include <G4_Mvtx_EIC.C>

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4tpc/PHG4TpcEndCapSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libg4mvtx.so)
R__LOAD_LIBRARY(libg4tpc.so)

namespace Enable
{
  bool TPC = false;
  bool TPC_ENDCAP = false;
  bool TPC_ABSORBER = false;
  bool TPC_OVERLAPCHECK = false;
}  // namespace Enable

namespace G4TPC
{
  int n_tpc_layer_inner = 16;
  double tpc_layer_thick_inner = 1.25;  // EIC- recover default inner radius of TPC vol.
  int tpc_layer_rphi_count_inner = 1152;

  int n_tpc_layer_mid = 16;
  double tpc_layer_thick_mid = 1.25;
  int tpc_layer_rphi_count_mid = 1536;

  int n_tpc_layer_outer = 16;
  double tpc_layer_thick_outer = 1.125;  // outer later reach from 60-78 cm (instead of 80 cm), that leads to radial thickness of 1.125 cm
  int tpc_layer_rphi_count_outer = 2304;

  double outer_radius = 78.;
  double inner_cage_radius = 20.;
  double cage_length = 211.0;  // From TPC group, gives eta = 1.1 at 78 cm
  double n_rad_length_cage = 1.13e-02;
  double cage_thickness = 28.6 * n_rad_length_cage;  // Kapton X_0 = 28.6 cm  // mocks up Kapton + carbon fiber structure

  string tpcgas = "sPHENIX_TPC_Gas";  //  Ne(90%) CF4(10%) - defined in g4main/PHG4Reco.cc

}  // namespace G4TPC

void TPCInit()
{
  //  BlackHoleGeometry::max_radius set at the end of the TPC function
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4TPC::cage_length / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -G4TPC::cage_length / 2.);
}

//! TPC end cap, wagon wheel, electronics
void TPC_Endcaps(PHG4Reco* g4Reco)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TPC_OVERLAPCHECK;
  bool AbsorberActive = Enable::ABSORBER || Enable::TPC_ABSORBER;

  PHG4TpcEndCapSubsystem* tpc_endcap = new PHG4TpcEndCapSubsystem("TPC_ENDCAP");
  tpc_endcap->SuperDetector("TPC_ENDCAP");

  if (AbsorberActive) tpc_endcap->SetActive();
  tpc_endcap->OverlapCheck(OverlapCheck);

  //  tpc_endcap->set_int_param("construction_verbosity", 2);

  g4Reco->registerSubsystem(tpc_endcap);

  return;
}

double TPC(PHG4Reco* g4Reco, double radius,
           const int absorberactive = 0,
           int verbosity = 0)
{
  // time projection chamber layers --------------------------------------------

  PHG4CylinderSubsystem* cyl;

  radius = G4TPC::inner_cage_radius;

  // inner field cage
  cyl = new PHG4CylinderSubsystem("FIELDCAGE", G4MVTX::n_maps_layer);
  cyl->set_double_param("radius", radius);
  cyl->set_double_param("length", G4TPC::cage_length);
  cyl->set_string_param("material", "G4_KAPTON");
  cyl->set_double_param("thickness", G4TPC::cage_thickness);
  cyl->SuperDetector("FIELDCAGE");
  cyl->Verbosity(0);
  g4Reco->registerSubsystem(cyl);

  radius += G4TPC::cage_thickness;

  // double inner_readout_radius = radius;
  //  if (inner_readout_radius < radius) inner_readout_radius = radius;
  //
  //

  int n_tpc_layers[3] = {16, 16, 16};
  int tpc_layer_rphi_count[3] = {1152, 1536, 2304};

  double tpc_region_thickness[3] = {20., 20., 18.};
  // Active layers of the TPC (inner layers)
  int nlayer = G4MVTX::n_maps_layer;
  for (int irange = 0; irange < 3; irange++)
  {
    double tpc_layer_thickness = tpc_region_thickness[irange] / n_tpc_layers[irange];  // thickness per layer
    for (int ilayer = 0; ilayer < n_tpc_layers[irange]; ilayer++)
    {
      cyl = new PHG4CylinderSubsystem("TPC", nlayer);
      cyl->set_double_param("radius", radius);
      cyl->set_double_param("length", G4TPC::cage_length);
      cyl->set_string_param("material", G4TPC::tpcgas);
      cyl->set_double_param("thickness", tpc_layer_thickness - 0.01);
      cyl->SetActive();
      cyl->SuperDetector("TPC");
      g4Reco->registerSubsystem(cyl);

      radius += tpc_layer_thickness;
      nlayer++;
    }
  }

  // outer field cage
  cyl = new PHG4CylinderSubsystem("FIELDCAGE", nlayer);
  cyl->set_double_param("radius", radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", G4TPC::cage_length);
  cyl->set_string_param("material", "G4_KAPTON");
  cyl->set_double_param("thickness", G4TPC::cage_thickness);  // Kapton X_0 = 28.6 cm
  cyl->SuperDetector("FIELDCAGE");
  g4Reco->registerSubsystem(cyl);

  radius += G4TPC::cage_thickness;

  if (Enable::TPC_ENDCAP)
  {
    TPC_Endcaps(g4Reco);
  }
  // update now that we know the outer radius
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, radius);
  return radius;
}

#endif
