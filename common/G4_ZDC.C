#ifndef MACRO_G4ZDC_C
#define MACRO_G4ZDC_C

#include <GlobalVariables.C>

#include <g4calo/RawTowerBuilderByHitIndex.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4detectors/PHG4ZDCSubsystem.h>

#include <g4detectors/PHG4DetectorSubsystem.h>

#include <g4eval/CaloEvaluator.h>

#include <g4main/PHG4Reco.h>

#include <caloreco/RawClusterBuilderFwd.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawTowerCalibration.h>
#include <calobase/RawTowerDefs.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)

namespace Enable
{
  bool ZDC = false;
  bool ZDC_ABSORBER = false;
  bool ZDC_CELL = false;
  bool ZDC_TOWER = false;
  bool ZDC_CLUSTER = false;
  bool ZDC_EVAL = false;
  bool ZDC_OVERLAPCHECK = false;
  int ZDC_VERBOSITY = 0;
}  

namespace G4ZDC
{
 
  double Gz0 = 1900.;
  double outer_radius = 18.;
  string calibfile = "/ZDC/mapping/towerMap_ZDC.txt";
}
void ZDCInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4ZDC::outer_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4ZDC::Gz0);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z,  - G4ZDC::Gz0);

}

void ZDCSetup(PHG4Reco *g4Reco, const int absorberactive = 0)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::ZDC_ABSORBER || (absorberactive > 0);
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::ZDC_OVERLAPCHECK;

  Fun4AllServer *se = Fun4AllServer::instance();

 

  PHG4ZDCSubsystem *zdc = new PHG4ZDCSubsystem("ZDC");
  
  zdc->OverlapCheck(OverlapCheck);
  zdc->SetActive();
  zdc->SuperDetector("ZDC");
  if (AbsorberActive) zdc->SetAbsorberActive(AbsorberActive);
  g4Reco->registerSubsystem(zdc);
  //cout << mapping_zdc.str() << endl;
}

void ZDC_Cells()
{
  return;
}

void ZDC_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::ZDC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_zdc;
  mapping_zdc << getenv("CALIBRATIONROOT") << G4ZDC::calibfile;
  //mapping_zdc  << G4ZDC::calibfile;

  RawTowerBuilderByHitIndex *tower_ZDC = new RawTowerBuilderByHitIndex("TowerBuilder_ZDC");
  tower_ZDC->Detector("ZDC");
  tower_ZDC->set_sim_tower_node_prefix("SIM");
  tower_ZDC->GeometryTableFile(mapping_zdc.str());

  se->registerSubsystem(tower_ZDC);


  
  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("ZDCRawTowerDigitizer");
  TowerDigitizer->Detector("ZDC");
  TowerDigitizer->TowerType(0);
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer);

 

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("ZDCRawTowerCalibration");
  TowerCalibration->Detector("ZDC");
  TowerCalibration->TowerType(0);
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1.0); 
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration);

}

void ZDC_Clusters()
{

  return;
}

void ZDC_Eval(std::string outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::ZDC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("ZDCEVALUATOR", "ZDC", outputfile.c_str());
  eval->set_do_cluster_eval(false); 
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
#endif
