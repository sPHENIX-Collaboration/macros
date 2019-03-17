#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <fun4all/Fun4AllServer.h>
#include <g4calo/RawTowerBuilderByHitIndex.h>
#include <g4calo/RawTowerDigitizer.h>
#include <caloreco/RawClusterBuilderFwd.h>
#include <caloreco/RawTowerCalibration.h>
#include <g4detectors/PHG4ForwardCalCellReco.h>
#include <g4detectors/PHG4ForwardHcalSubsystem.h>
#include <g4eval/CaloEvaluator.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)
#endif

using namespace std;

void
FHCALInit()
{
}

void FHCAL_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4ForwardCalCellReco *hc = new PHG4ForwardCalCellReco("FHCALCellReco");
  hc->Detector("FHCAL");
  se->registerSubsystem(hc);
  
  return;  
}

void
FHCALSetup(PHG4Reco* g4Reco, const int absorberactive = 0)
{

  gSystem->Load("libg4detectors.so");

  Fun4AllServer *se = Fun4AllServer::instance();

  /** Use dedicated FHCAL module */
  PHG4ForwardHcalSubsystem *hhcal = new PHG4ForwardHcalSubsystem("FHCAL");

  ostringstream mapping_hhcal;

  /* path to central copy of calibrations repositry */
  mapping_hhcal << getenv("CALIBRATIONROOT") ;
  mapping_hhcal << "/ForwardHcal/mapping/towerMap_FHCAL_v004.txt";
  cout << mapping_hhcal.str() << endl;
  //mapping_hhcal << "towerMap_FHCAL_latest.txt";

  hhcal->SetTowerMappingFile( mapping_hhcal.str() );
  hhcal->OverlapCheck(overlapcheck);

  if (absorberactive) hhcal->SetAbsorberActive();

  g4Reco->registerSubsystem( hhcal );

}

void FHCAL_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4calo.so");
  gSystem->Load("libcalo_reco.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_fhcal;
  mapping_fhcal << getenv("CALIBRATIONROOT") <<
  	"/ForwardHcal/mapping/towerMap_FHCAL_v004.txt";
  //mapping_fhcal << "towerMap_FHCAL_latest.txt";

  RawTowerBuilderByHitIndex* tower_FHCAL = new RawTowerBuilderByHitIndex("TowerBuilder_FHCAL");
  tower_FHCAL->Detector("FHCAL");
  tower_FHCAL->set_sim_tower_node_prefix("SIM");
  tower_FHCAL->GeometryTableFile( mapping_fhcal.str() );

  se->registerSubsystem(tower_FHCAL);

  // const double FHCAL_photoelectron_per_GeV = 500;

  // RawTowerDigitizer *TowerDigitizer_FHCAL = new RawTowerDigitizer("FHCALRawTowerDigitizer");

  // TowerDigitizer_FHCAL->Detector("FHCAL");
  // TowerDigitizer_FHCAL->Verbosity(verbosity);
  // TowerDigitizer_FHCAL->set_raw_tower_node_prefix("RAW");
  // TowerDigitizer_FHCAL->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitization);
  // TowerDigitizer_FHCAL->set_pedstal_central_ADC(0);
  // TowerDigitizer_FHCAL->set_pedstal_width_ADC(8);// eRD1 test beam setting
  // TowerDigitizer_FHCAL->set_photonelec_ADC(1);//not simulating ADC discretization error
  // TowerDigitizer_FHCAL->set_photonelec_yield_visible_GeV( FHCAL_photoelectron_per_GeV );
  // TowerDigitizer_FHCAL->set_zero_suppression_ADC(16); // eRD1 test beam setting

  // se->registerSubsystem( TowerDigitizer_FHCAL );

  // RawTowerCalibration *TowerCalibration_FHCAL = new RawTowerCalibration("FHCALRawTowerCalibration");
  // TowerCalibration_FHCAL->Detector("FHCAL");
  // TowerCalibration_FHCAL->Verbosity(verbosity);
  // TowerCalibration_FHCAL->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  // TowerCalibration_FHCAL->set_calib_const_GeV_ADC( 1. / FHCAL_photoelectron_per_GeV );
  // TowerCalibration_FHCAL->set_pedstal_ADC( 0 );

  // se->registerSubsystem( TowerCalibration_FHCAL );

  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("FHCALRawTowerDigitizer");
  TowerDigitizer->Detector("FHCAL");
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem( TowerDigitizer );

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("FHCALRawTowerCalibration");
  TowerCalibration->Detector("FHCAL");
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1./0.03898);  // calibrated with muons
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem( TowerCalibration );


}

void FHCAL_Clusters(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawClusterBuilderFwd* ClusterBuilder = new RawClusterBuilderFwd("FHCALRawClusterBuilderFwd");
  ClusterBuilder->Detector("FHCAL");
  ClusterBuilder->Verbosity(verbosity);
  ClusterBuilder->set_threshold_energy(0.100);  
  se->registerSubsystem( ClusterBuilder );
  
  return;
}

void FHCAL_Eval(std::string outputfile, int verbosity = 0)
{
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("FHCALEVALUATOR", "FHCAL", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
