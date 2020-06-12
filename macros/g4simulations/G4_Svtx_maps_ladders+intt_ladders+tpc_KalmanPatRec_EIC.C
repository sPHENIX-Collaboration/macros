#pragma once

#include "GlobalVariables.C"

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4mvtx/PHG4MvtxDefs.h>
#include <g4mvtx/PHG4MvtxSubsystem.h>

#include <g4tpc/PHG4TpcSpaceChargeDistortion.h>

#include <g4eval/SvtxEvaluator.h>

#include <g4main/PHG4Reco.h>

#include <fun4all/Fun4AllServer.h>

//#include <TMath.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libg4mvtx.so)


namespace TrackingEicMvtxMacro
{
const int n_maps_layer = 3;  // must be 0-3, setting it to zero removes Mvtx completely, n < 3 gives the first n layers
}

namespace TrackingEicTpcMacro
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
}

double inner_cage_radius = 20.;
// double inner_readout_radius = 30.; - deprecated for EIC simulation

// TPC gas parameters
// These are set for a variety of gas choices...
//==============================================
enum TPC_Gas
{
  Ne2K_100,
  Ne2K_400,
  NeCF4_100,
  NeCF4_300,
  NeCF4_400,
  ByHand
};
TPC_Gas ether = TPC_Gas::NeCF4_400;
//TPC_Gas ether = TPC_Gas::ByHand;

// Data on gasses @20 C and 760 Torr from the following source:
// http://www.slac.stanford.edu/pubs/icfa/summer98/paper3/paper3.pdf
double Ne_dEdx = 1.56;    // keV/cm
double CF4_dEdx = 7.00;   // keV/cm
double iBut_dEdx = 5.93;  // keV/cm

double Ne_NPrimary = 12;    // Number/cm
double CF4_NPrimary = 51;   // Number/cm
double iBut_NPrimary = 84;  // Number/cm

double Ne_NTotal = 43;     // Number/cm
double CF4_NTotal = 100;   // Number/cm
double iBut_NTotal = 195;  // Number/cm

// TPC Performance Parameter (applies extra smear to mimic the avalanche):
double TPC_SigmaT = 0.03;  // 0.03 means 300 microns...Prakhar Garg Simulation...desire measurement...

// to be overwritten...
double TPCDriftVelocity;
double TPC_Trans_Diffusion;
double TPC_Long_Diffusion;
double TPC_dEdx;
double TPC_NPri;
double TPC_NTot;
double TPC_ElectronsPerKeV;

// TPC readout shaping time and ADC clock parameters
// these set the Z size of the TPC cells
// These need to be set in the init since some of them require the drift velocity...
//=======================================
double TPCADCClock;
double TPCShapingRMSLead;
double TPCShapingRMSTail;
double tpc_cell_z;
double TPC_SmearRPhi;
double TPC_SmearZ;

//}

void SvtxInit(int verbosity = 0)
{

  switch (ether)
  {
  // https://www.phenix.bnl.gov/WWW/p/draft/prakhar/tpc/HTML_Gas_Linear/Ne_CF4_IC4H10_95_3_2.html
  case TPC_Gas::Ne2K_100:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::Ne2K_100" << endl;
    TPCDriftVelocity = 3.2 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0065;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0300;      // cm/SQRT(cm)
    TPC_dEdx = 0.95 * Ne_dEdx + 0.03 * CF4_dEdx + 0.02 * iBut_dEdx;
    TPC_NPri = 0.95 * Ne_NPrimary + 0.03 * CF4_NPrimary + 0.02 * iBut_NPrimary;
    TPC_NTot = 0.95 * Ne_NTotal + 0.03 * CF4_NTotal + 0.02 * iBut_NTotal;
    break;
  }
  case TPC_Gas::Ne2K_400:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::Ne2K_400" << endl;
    TPCDriftVelocity = 5.5 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0120;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0175;      // cm/SQRT(cm)
    TPC_dEdx = 0.95 * Ne_dEdx + 0.03 * CF4_dEdx + 0.02 * iBut_dEdx;
    TPC_NPri = 0.95 * Ne_NPrimary + 0.03 * CF4_NPrimary + 0.02 * iBut_NPrimary;
    TPC_NTot = 0.95 * Ne_NTotal + 0.03 * CF4_NTotal + 0.02 * iBut_NTotal;
    break;
  }
  // https://www.phenix.bnl.gov/WWW/p/draft/prakhar/tpc/HTML_Gas_Linear/Ne_CF4_90_10.html
  case TPC_Gas::NeCF4_100:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::NeCF4_100" << endl;
    TPCDriftVelocity = 4.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0045;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0270;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  case TPC_Gas::NeCF4_300:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::NeCF4_300" << endl;
    TPCDriftVelocity = 7.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0052;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0170;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  case TPC_Gas::NeCF4_400:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::NeCF4_400" << endl;
    TPCDriftVelocity = 8.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0060;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0150;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  case TPC_Gas::ByHand:
  {
    if (verbosity)
      cout << "Gas Choice:  TPC_Gas::ByHand" << endl;
    TPCDriftVelocity = 6.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0130;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0130;      // cm/SQRT(cm)
    TPC_ElectronsPerKeV = 28.0;
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = TPC_ElectronsPerKeV * TPC_dEdx;
    break;
  }
  default:  // defaults to NeCF4_400
  {
    if (verbosity)
      cout << "Gas Choice Undefined...using TPC_Gas::NeCF4_400" << endl;
    TPCDriftVelocity = 8.0 / 1000.0;  // cm/ns
    TPC_Trans_Diffusion = 0.0060;     // cm/SQRT(cm)
    TPC_Long_Diffusion = 0.0150;      // cm/SQRT(cm)
    TPC_dEdx = 0.90 * Ne_dEdx + 0.10 * CF4_dEdx;
    TPC_NPri = 0.90 * Ne_NPrimary + 0.10 * CF4_NPrimary;
    TPC_NTot = 0.90 * Ne_NTotal + 0.10 * CF4_NTotal;
    break;
  }
  }

  TPC_ElectronsPerKeV = TPC_NTot / TPC_dEdx;

  // TPC readout shaping time and ADC clock parameters
  // these set the Z size of the TPC cells
  //=======================================
  TPCShapingRMSLead = 32.0;                     // ns, rising RMS equivalent of shaping amplifier for 80 ns SAMPA
  TPCShapingRMSTail = 48.0;                     // ns, falling RMS equivalent of shaping amplifier for 80 ns SAMPA
  TPCADCClock = 53.0;                           // ns, corresponds to an ADC clock rate of 18.8 MHz
  tpc_cell_z = TPCADCClock * TPCDriftVelocity;  // cm

  //  these are fudge parameters, tuned to give average of 150 microns r-phi and 500 microns Z resolution in the outer TPC layers with the TPC setup used here and 80 ns SAMPA peaking time
  TPC_SmearRPhi = 0.25;
  TPC_SmearZ = 0.15;
}

double Svtx(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0,
            int verbosity = 0)
{
  gSystem->Load("libg4mvtx.so");
  int n_gas_layer = TrackingEicTpcMacro::n_tpc_layer_inner + TrackingEicTpcMacro::n_tpc_layer_mid + TrackingEicTpcMacro::n_tpc_layer_outer;
  if (TrackingEicMvtxMacro::n_maps_layer > 0)
  {
    bool maps_overlapcheck = false;  // set to true if you want to check for overlaps

    // Update EIC MAPS layer structure based on inner two layers of U. Birmingham tracker

    PHG4MvtxSubsystem* mvtx = new PHG4MvtxSubsystem("MVTX");
    mvtx->Verbosity(verbosity);

    // H?kan Wennl?f <hwennlof@kth.se> :
    //    Without time-stamping layer:
    //    Stave type  Length  Overlap Radius [mm] Tilt  Radiation length  Number of staves
    //    ALICE inner 270 mm  2 mm  36.4  12.0 deg  0.3 % X0  18
    //    ALICE inner 270 mm  2 mm  59.8  12.0 deg  0.3 % X0  30
    //    ALICE outer 840 mm  4 mm  133.8 6.0 deg 0.8 % X0  16
    //    ALICE outer 840 mm  4 mm  180 6.0 deg 0.8 % X0  21


    static const double Degree2Rad = 180. /M_PI;
    int N_staves[3] = {18, 24, 30};
    double nom_radius[3] = {36.4, 48.1,  59.8};
    for (int ilyr = 0; ilyr<3; ilyr++)
    {
      mvtx->set_int_param(ilyr, "active", 1);  //non-automatic initialization in PHG4DetectorGroupSubsystem
      mvtx->set_int_param(ilyr, "layer", ilyr);
      mvtx->set_int_param(ilyr, "N_staves", N_staves[ilyr]);
      mvtx->set_double_param(ilyr, "layer_nominal_radius", nom_radius[ilyr]); // mm
      mvtx->set_double_param(ilyr, "phitilt", 12.0 * Degree2Rad +M_PI);
      mvtx->set_double_param(ilyr, "phi0", 0);
    }

    mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "stave_geometry_file", string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/mvtx_stave_v1.gdml"));
    mvtx->SetActive(1);
    mvtx->OverlapCheck(maps_overlapcheck);
    g4Reco->registerSubsystem(mvtx);
  }  //   if (TrackingEicMvtxMacro::n_maps_layer > 0)

  //  int verbosity = 1;

  // time projection chamber layers --------------------------------------------

  PHG4CylinderSubsystem* cyl;

  radius = inner_cage_radius;

  double cage_length = 211.0;  // From TPC group, gives eta = 1.1 at 78 cm
  double n_rad_length_cage = 1.13e-02;
  double cage_thickness = 28.6 * n_rad_length_cage;  // Kapton X_0 = 28.6 cm  // mocks up Kapton + carbon fiber structure

  // inner field cage
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", TrackingEicMvtxMacro::n_maps_layer);
  cyl->set_double_param("radius", radius);
  cyl->set_double_param("length", cage_length);
  cyl->set_string_param("material", "G4_KAPTON");
  cyl->set_double_param("thickness", cage_thickness);
  cyl->SuperDetector("SVTXSUPPORT");
  cyl->Verbosity(0);
  g4Reco->registerSubsystem(cyl);

  radius += cage_thickness;

  // double inner_readout_radius = radius; 
  //  if (inner_readout_radius < radius) inner_readout_radius = radius;
  //
  string tpcgas = "sPHENIX_TPC_Gas";  //  Ne(90%) CF4(10%) - defined in g4main/PHG4Reco.cc
                                      //
  double outer_radius = 78.;

  int n_tpc_layers[3] = {16,16,16};
  int  tpc_layer_rphi_count[3] = {1152, 1536, 2304};

  double tpc_region_thickness[3] = { 20.,  20., 18.};
  // Active layers of the TPC (inner layers)
int nlayer = TrackingEicMvtxMacro::n_maps_layer;
for (int irange=0; irange<3; irange++)
{
  double tpc_layer_thickness = tpc_region_thickness[irange]/n_tpc_layers[irange]; // thickness per layer
  for (int ilayer = 0; ilayer < n_tpc_layers[irange]; ilayer++)
  {
    cyl = new PHG4CylinderSubsystem("SVTX", nlayer);
    cyl->set_double_param("radius", radius);
    cyl->set_double_param("length", cage_length);
    cyl->set_string_param("material", tpcgas.c_str());
    cyl->set_double_param("thickness", tpc_layer_thickness - 0.01);
    cyl->SetActive();
    cyl->SuperDetector("SVTX");
    g4Reco->registerSubsystem(cyl);

    radius += tpc_layer_thickness;
    nlayer++;
  }
}

  // outer field cage
  cyl = new PHG4CylinderSubsystem("SVTXSUPPORT", nlayer);
  cyl->set_double_param("radius", radius);
  cyl->set_int_param("lengthviarapidity", 0);
  cyl->set_double_param("length", cage_length);
  cyl->set_string_param("material", "G4_KAPTON");
  cyl->set_double_param("thickness", cage_thickness);  // Kapton X_0 = 28.6 cm
  cyl->SuperDetector("SVTXSUPPORT");
  g4Reco->registerSubsystem(cyl);

  radius += cage_thickness;

  return radius;
}

// Central detector cell reco is disabled as EIC setup use the fast tracking sim for now
void Svtx_Cells(int verbosity = 0)
{
  // runs the cellularization of the energy deposits (g4hits)
  // into detector hits (g4cells)

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  //-----------
  // SVTX cells
  //-----------

  return;
}

// Central detector  reco is disabled as EIC setup use the fast tracking sim for now
void Svtx_Reco(int verbosity = 0)
{
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  return;
}
