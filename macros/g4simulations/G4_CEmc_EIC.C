#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <caloreco/RawClusterBuilderGraph.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawTowerCalibration.h>
#include <fun4all/Fun4AllServer.h>
#include <g4calo/RawTowerBuilder.h>
#include <g4calo/RawTowerDigitizer.h>
#include <g4detectors/PHG4CylinderCellReco.h>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4eval/CaloEvaluator.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)
#endif

double cemcdepth = 9;
// tungs to scint width ratio of ~10:1
// corresponds to approx 2% sampling fraction

// 18 radiation lengths for 40 layers
double scint_width = 0.05;
double tungs_width = 0.245;

int min_cemc_layer = 1;
int max_cemc_layer = 41;

double topradius = 106.8; // cm
double bottomradius = 95; // cm
double negrapidity = -1.5;
double posrapidity = 1.24;
// this is default set to -1.5<eta<1.24 for 2018 Letter of Intent
// if the user changes these, the z position of the
// calorimeter must be changed in the function CEmc(...)

enum enu_Cemc_clusterizer {
  kCemcGraphClusterizer,

  kCemcTemplateClusterizer
};

//! template clusterizer, RawClusterBuilderTemplate, as developed by Sasha
//! Bazilevsky
enu_Cemc_clusterizer Cemc_clusterizer = kCemcTemplateClusterizer;
//! graph clusterizer, RawClusterBuilderGraph
// enu_Cemc_clusterizer Cemc_clusterizer = kCemcGraphClusterizer;

void CEmcInit(const int nslats = 1) {}

double CEmc(PHG4Reco *g4Reco, double radius, const int crossings,
            const int absorberactive = 0) {

  if (radius > 95) {
    cout << "inconsistency, radius: " << radius
         << " larger than allowed inner radius for CEMC = 95 cm" << endl;
    gSystem->Exit(-1);
  }

  radius = 95;

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4CylinderSubsystem *cemc;

  // determine the length of the calorimeter
  // can adjust length coverage by just adjusting these values
  // rapidity coverage will be determined by z shift of EMCAl
  // as indicated in the loop below

  // eta = -ln(tan(theta/2))
  double theta1 = 2. * TMath::ATan(TMath::Exp(-1 * posrapidity));
  double theta2 = 2. * TMath::ATan(TMath::Exp(-1 * negrapidity));
  // get the angle between the beam pipe and negative pseudorapidity axis
  theta2 = TMath::Pi() - theta2;

  double z1 = topradius / TMath::Tan(theta1);
  double z2 = topradius / TMath::Tan(theta2);

  double z3 = bottomradius / TMath::Tan(theta1);
  double z4 = bottomradius / TMath::Tan(theta2);

  // this is the top layer length
  double totaltoplength = z1 + z2;
  // this is the bottom layer length
  double totalbottomlength = z3 + z4;

  double height = 0;
  for (int thislayer = min_cemc_layer; thislayer <= max_cemc_layer;
       thislayer++) {

    // the length for a particular layer is determined from the bottom length
    double thislength = totalbottomlength + height / TMath::Tan(theta1) +
                        height / TMath::Tan(theta2);

    cemc = new PHG4CylinderSubsystem("ABSORBER_CEMC", thislayer);
    cemc->set_double_param("radius", radius);
    cemc->set_string_param("material", "Spacal_W_Epoxy");
    cemc->set_double_param("thickness", tungs_width);
    cemc->set_double_param("length", thislength);
    cemc->set_int_param("lengthviarapidity", 0);

    // starts centered around IP
    // shift backwards 30 cm for total 370 cm length to cover -1.5<eta<1.24
    cemc->set_double_param("place_z", -30);
    cemc->SuperDetector("ABSORBER_CEMC");
    if (absorberactive)
      cemc->SetActive();
    cemc->OverlapCheck(overlapcheck);

    g4Reco->registerSubsystem(cemc);

    radius += tungs_width;
    radius += no_overlapp;

    height += tungs_width;

    cemc = new PHG4CylinderSubsystem("CEMC", thislayer);
    cemc->set_double_param("radius", radius);
    cemc->set_string_param("material", "PMMA");
    cemc->set_double_param("thickness", scint_width);
    cemc->set_int_param("lightyield", 1);
    cemc->set_int_param("lengthviarapidity", 0);
    cemc->set_double_param("length", thislength);

    // shift back -30 cm to cover -1.4<eta<1.1
    cemc->set_double_param("place_z", -30);
    cemc->SuperDetector("CEMC");

    cemc->SetActive();
    cemc->OverlapCheck(overlapcheck);
    g4Reco->registerSubsystem(cemc);

    radius += scint_width;
    radius += no_overlapp;

    height += scint_width;
  }

  PHG4CylinderSubsystem *cemc_cyl =
      new PHG4CylinderSubsystem("CEMC_ELECTRONICS", 0);
  cemc_cyl->set_double_param("radius", radius);
  cemc_cyl->set_string_param("material", "G4_TEFLON");
  cemc_cyl->set_double_param("thickness", 0.5);

  double l1 = (radius + 0.5) / TMath::Tan(theta1);
  double l2 = (radius + 0.5) / TMath::Tan(theta2);
  cemc_cyl->set_int_param("lengthviarapidity", 0);
  cemc_cyl->set_double_param("length", l1 + l2);
  // shift back -30 cm to cover -1.4<eta<1.1
  cemc_cyl->set_double_param("place_z", -30);
  if (absorberactive)
    cemc_cyl->SetActive();
  cemc_cyl->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(cemc_cyl);
  return radius;
}

void CEMC_Cells(int verbosity = 0) {
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4CylinderCellReco *cemc_cells =
      new PHG4CylinderCellReco("CEMCCYLCELLRECO");
  cemc_cells->Detector("CEMC");
  cemc_cells->Verbosity(verbosity);
  double radius = 95;
  for (int i = min_cemc_layer; i <= max_cemc_layer; i++) {
    cemc_cells->cellsize(i, 2. * TMath::Pi() / 256. * radius,
                         2. * TMath::Pi() / 256. * radius);
    radius += (scint_width + tungs_width);
  }
  se->registerSubsystem(cemc_cells);

  return;
}

void CEMC_Towers(int verbosity = 0) {
  gSystem->Load("libg4calo.so");
  gSystem->Load("libcalo_reco.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  RawTowerBuilder *CemcTowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
  CemcTowerBuilder->Detector("CEMC");
  CemcTowerBuilder->set_sim_tower_node_prefix("SIM");
  CemcTowerBuilder->Verbosity(verbosity);
  se->registerSubsystem(CemcTowerBuilder);

  const double photoelectron_per_GeV =
      500; // 500 photon per total GeV deposition
  // just set a 4% sampling fraction - already tuned by tungs/scint width ratio
  double sampling_fraction = 4e-02;
  RawTowerDigitizer *CemcTowerDigitizer =
      new RawTowerDigitizer("EmcRawTowerDigitizer");
  CemcTowerDigitizer->Detector("CEMC");
  CemcTowerDigitizer->Verbosity(verbosity);
  CemcTowerDigitizer->set_digi_algorithm(
      RawTowerDigitizer::kSimple_photon_digitization);
  CemcTowerDigitizer->set_pedstal_central_ADC(0);
  CemcTowerDigitizer->set_pedstal_width_ADC(8); // eRD1 test beam setting
  CemcTowerDigitizer->set_photonelec_ADC(
      1); // not simulating ADC discretization error
  CemcTowerDigitizer->set_photonelec_yield_visible_GeV(photoelectron_per_GeV /
                                                       sampling_fraction);
  CemcTowerDigitizer->set_zero_suppression_ADC(16); // eRD1 test beam setting
  se->registerSubsystem(CemcTowerDigitizer);

  RawTowerCalibration *CemcTowerCalibration =
      new RawTowerCalibration("EmcRawTowerCalibration");
  CemcTowerCalibration->Detector("CEMC");
  CemcTowerCalibration->Verbosity(verbosity);
  CemcTowerCalibration->set_calib_algorithm(
      RawTowerCalibration::kSimple_linear_calibration);
  CemcTowerCalibration->set_calib_const_GeV_ADC(1. / photoelectron_per_GeV /
                                                0.9715);
  CemcTowerCalibration->set_pedstal_ADC(0);

  se->registerSubsystem(CemcTowerCalibration);

  return;
}

void CEMC_Clusters(int verbosity = 0) {

  gSystem->Load("libcalo_reco.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  if (Cemc_clusterizer == kCemcTemplateClusterizer) {
    RawClusterBuilderTemplate *cemc_clusterbuilder =
        new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate");
    cemc_clusterbuilder->Detector("CEMC");
    cemc_clusterbuilder->Verbosity(verbosity);

    se->registerSubsystem(cemc_clusterbuilder);
  } else if (Cemc_clusterizer == kCemcGraphClusterizer) {
    RawClusterBuilderGraph *cemc_clusterbuilder =
        new RawClusterBuilderGraph("EmcRawClusterBuilderGraph");
    cemc_clusterbuilder->Detector("CEMC");
    cemc_clusterbuilder->Verbosity(verbosity);

    se->registerSubsystem(cemc_clusterbuilder);
  } else {
    cout << "CEMC_Clusters - unknown clusterizer setting!! " << endl;
    exit(1);
  }
  return;
}
void CEMC_Eval(std::string outputfile, int verbosity = 0) {
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval =
      new CaloEvaluator("CEMCEVALUATOR", "CEMC", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
