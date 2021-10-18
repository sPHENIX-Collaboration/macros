#ifndef MACRO_G4ZDC_C
#define MACRO_G4ZDC_C

#include <GlobalVariables.C>

#include <G4_BeamLine.C>

#include <g4calo/RawTowerBuilderByHitIndex.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4detectors/PHG4DetectorSubsystem.h>
#include <g4detectors/PHG4ZDCDefs.h>
#include <g4detectors/PHG4ZDCSubsystem.h>

#include <g4eval/CaloEvaluator.h>

#include <g4main/PHG4Reco.h>

#include <calobase/RawTowerDefs.h>

#include <caloreco/RawClusterBuilderFwd.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawTowerCalibration.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libg4calo.so)
R__LOAD_LIBRARY(libg4detectors.so)
R__LOAD_LIBRARY(libg4eval.so)

namespace Enable
{
  bool ZDC = false;
  bool ZDC_ABSORBER = false;
  bool ZDC_SUPPORT = false;
  bool ZDC_TOWER = false;
  bool ZDC_EVAL = false;
  bool ZDC_OVERLAPCHECK = false;
  int ZDC_VERBOSITY = 0;
}  // namespace Enable

namespace G4ZDC
{
  double Gz0 = 1900.;
  double outer_radius = 180.;
  string calibfile = "ZDC/mapping/towerMap_ZDC.txt";
  double ZDCPlaceZ = 1843.0;

}  // namespace G4ZDC

void ZDCInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4ZDC::outer_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4ZDC::Gz0);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -G4ZDC::Gz0);
}

void ZDCSetup(PHG4Reco *g4Reco, const int absorberactive = 0)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::ZDC_ABSORBER || (absorberactive > 0);
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::ZDC_OVERLAPCHECK;

  bool SupportActive = Enable::SUPPORT || Enable::ZDC_SUPPORT;

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4ZDCSubsystem *zdc = new PHG4ZDCSubsystem("ZDC",PHG4ZDCDefs::NORTH);
// place zdc in beam enclosure
  zdc->set_double_param("place_z", G4ZDC::ZDCPlaceZ - G4BEAMLINE::enclosure_center);
  zdc->OverlapCheck(OverlapCheck);
  zdc->SetActive();
  zdc->SuperDetector("ZDC");
  if (AbsorberActive) zdc->SetAbsorberActive(AbsorberActive);
  if (SupportActive) zdc->SetSupportActive(SupportActive);
  zdc->SetMotherSubsystem(G4BEAMLINE::ForwardBeamLineEnclosure);
  g4Reco->registerSubsystem(zdc);

  zdc = new PHG4ZDCSubsystem("ZDC", PHG4ZDCDefs::SOUTH);
// place zdc in beam enclosure
  zdc->set_double_param("place_z", G4ZDC::ZDCPlaceZ - G4BEAMLINE::enclosure_center);
  zdc->OverlapCheck(OverlapCheck);
  zdc->SetActive();
  zdc->SuperDetector("ZDC");
  if (AbsorberActive) zdc->SetAbsorberActive(AbsorberActive);
  if (SupportActive) zdc->SetSupportActive(SupportActive);
  zdc->SetMotherSubsystem(G4BEAMLINE::BackwardBeamLineEnclosure);
  g4Reco->registerSubsystem(zdc);
}

void ZDC_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::ZDC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  string mapping_zdc = string(getenv("CALIBRATIONROOT")) + "/" + G4ZDC::calibfile;

  RawTowerBuilderByHitIndex *tower_ZDC = new RawTowerBuilderByHitIndex("TowerBuilder_ZDC");
  tower_ZDC->Detector("ZDC");
  tower_ZDC->set_sim_tower_node_prefix("SIM");
  tower_ZDC->GeometryTableFile(mapping_zdc);
  se->registerSubsystem(tower_ZDC);

  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("ZDCRawTowerDigitizer");
  TowerDigitizer->Detector("ZDC");
  TowerDigitizer->TowerType(0);
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer);
  //SMD
  RawTowerDigitizer *TowerDigitizer1 = new RawTowerDigitizer("ZDCRawTowerDigitizer1");
  TowerDigitizer1->Detector("ZDC");
  TowerDigitizer1->TowerType(1);
  TowerDigitizer1->Verbosity(verbosity);
  TowerDigitizer1->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer1);

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("ZDCRawTowerCalibration");
  TowerCalibration->Detector("ZDC");
  TowerCalibration->TowerType(0);
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1.0);  // sampling fraction = 0.010
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration);

  RawTowerCalibration *TowerCalibration1 = new RawTowerCalibration("ZDCRawTowerCalibration1");
  TowerCalibration1->Detector("ZDC");
  TowerCalibration1->TowerType(1);
  TowerCalibration1->Verbosity(verbosity);
  TowerCalibration1->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration1->set_calib_const_GeV_ADC(1.0);
  TowerCalibration1->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration1);
}

void ZDC_Eval(std::string outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::ZDC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("ZDCEVALUATOR", "ZDC", outputfile);
  eval->set_do_cluster_eval(false);
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
#endif
