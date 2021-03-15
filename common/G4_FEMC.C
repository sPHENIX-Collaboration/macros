#ifndef MACRO_G4FEMC_C
#define MACRO_G4FEMC_C

#include <GlobalVariables.C>

#include <g4calo/RawTowerBuilderByHitIndex.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4detectors/PHG4ForwardCalCellReco.h>
#include <g4detectors/PHG4ForwardEcalSubsystem.h>

#include <g4eval/CaloEvaluator.h>

#include <g4main/PHG4Reco.h>

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
  bool FEMC = false;
  bool FEMC_ABSORBER = false;
  bool FEMC_CELL = false;
  bool FEMC_TOWER = false;
  bool FEMC_CLUSTER = false;
  bool FEMC_EVAL = false;
  bool FEMC_OVERLAPCHECK = false;
  int FEMC_VERBOSITY = 0;
}  // namespace Enable

namespace G4FEMC
{
  // from ForwardEcal/mapping/towerMap_FEMC_fsPHENIX_v004.txt
  double Gz0 = 305.;
  double Gdz = 40.;
  double outer_radius = 180.;
  string calibfile = "/ForwardEcal/mapping/towerMap_FEMC_fsPHENIX_v004.txt";
  enum enu_Femc_clusterizer
  {
    kFemcGraphClusterizer,
    kFemcTemplateClusterizer
  };

  //template clusterizer, as developed by Sasha Bazilevsky
  enu_Femc_clusterizer Femc_clusterizer = kFemcTemplateClusterizer;
  // graph clusterizer
  //enu_Femc_clusterizer Femc_clusterizer = kFemcGraphClusterizer;
}  // namespace G4FEMC

void FEMCInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4FEMC::outer_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4FEMC::Gz0 + G4FEMC::Gdz / 2.);
}

void FEMCSetup(PHG4Reco *g4Reco, const int absorberactive = 0)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::FEMC_ABSORBER || (absorberactive > 0);
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::FEMC_OVERLAPCHECK;

  Fun4AllServer *se = Fun4AllServer::instance();

  /** Use dedicated FEMC module */
  PHG4ForwardEcalSubsystem *femc = new PHG4ForwardEcalSubsystem("FEMC");

  ostringstream mapping_femc;

  // fsPHENIX ECAL
  femc->SetfsPHENIXDetector();
  mapping_femc << getenv("CALIBRATIONROOT") << G4FEMC::calibfile;

  //  cout << mapping_femc.str() << endl;
  femc->SetTowerMappingFile(mapping_femc.str());
  femc->OverlapCheck(OverlapCheck);
  femc->SetActive();
  femc->SuperDetector("FEMC");
  if (AbsorberActive) femc->SetAbsorberActive(AbsorberActive);

  g4Reco->registerSubsystem(femc);
}

void FEMC_Cells()
{
  return;
}

void FEMC_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FEMC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_femc;

  // fsPHENIX ECAL
  mapping_femc << getenv("CALIBRATIONROOT") << G4FEMC::calibfile;

  RawTowerBuilderByHitIndex *tower_FEMC = new RawTowerBuilderByHitIndex("TowerBuilder_FEMC");
  tower_FEMC->Detector("FEMC");
  tower_FEMC->set_sim_tower_node_prefix("SIM");
  tower_FEMC->GeometryTableFile(mapping_femc.str());

  se->registerSubsystem(tower_FEMC);

  // PbW crystals
  //RawTowerDigitizer *TowerDigitizer1 = new RawTowerDigitizer("FEMCRawTowerDigitizer1");
  //TowerDigitizer1->Detector("FEMC");
  //TowerDigitizer1->TowerType(1);
  //TowerDigitizer1->Verbosity(verbosity);
  //TowerDigitizer1->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  //se->registerSubsystem( TowerDigitizer1 );

  // PbSc towers
  //RawTowerDigitizer *TowerDigitizer2 = new RawTowerDigitizer("FEMCRawTowerDigitizer2");
  //TowerDigitizer2->Detector("FEMC");
  //TowerDigitizer2->TowerType(2);
  //TowerDigitizer2->Verbosity(verbosity);
  //TowerDigitizer2->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  //se->registerSubsystem( TowerDigitizer2 );

  // E864 towers (three types for three sizes)
  RawTowerDigitizer *TowerDigitizer3 = new RawTowerDigitizer("FEMCRawTowerDigitizer3");
  TowerDigitizer3->Detector("FEMC");
  TowerDigitizer3->TowerType(3);
  TowerDigitizer3->Verbosity(verbosity);
  TowerDigitizer3->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer3);

  RawTowerDigitizer *TowerDigitizer4 = new RawTowerDigitizer("FEMCRawTowerDigitizer4");
  TowerDigitizer4->Detector("FEMC");
  TowerDigitizer4->TowerType(4);
  TowerDigitizer4->Verbosity(verbosity);
  TowerDigitizer4->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer4);

  RawTowerDigitizer *TowerDigitizer5 = new RawTowerDigitizer("FEMCRawTowerDigitizer5");
  TowerDigitizer5->Detector("FEMC");
  TowerDigitizer5->TowerType(5);
  TowerDigitizer5->Verbosity(verbosity);
  TowerDigitizer5->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer5);

  RawTowerDigitizer *TowerDigitizer6 = new RawTowerDigitizer("FEMCRawTowerDigitizer6");
  TowerDigitizer6->Detector("FEMC");
  TowerDigitizer6->TowerType(6);
  TowerDigitizer6->Verbosity(verbosity);
  TowerDigitizer6->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem(TowerDigitizer6);

  // PbW crystals
  //RawTowerCalibration *TowerCalibration1 = new RawTowerCalibration("FEMCRawTowerCalibration1");
  //TowerCalibration1->Detector("FEMC");
  //TowerCalibration1->TowerType(1);
  //TowerCalibration1->Verbosity(verbosity);
  //TowerCalibration1->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  //TowerCalibration1->set_calib_const_GeV_ADC(1.0);  // sampling fraction = 1.0
  //TowerCalibration1->set_pedstal_ADC(0);
  //se->registerSubsystem( TowerCalibration1 );

  // PbSc towers
  //RawTowerCalibration *TowerCalibration2 = new RawTowerCalibration("FEMCRawTowerCalibration2");
  //TowerCalibration2->Detector("FEMC");
  //TowerCalibration2->TowerType(2);
  //TowerCalibration2->Verbosity(verbosity);
  //TowerCalibration2->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  //TowerCalibration2->set_calib_const_GeV_ADC(1.0/0.249);  // sampling fraction = 0.249 for e-
  //TowerCalibration2->set_pedstal_ADC(0);
  //se->registerSubsystem( TowerCalibration2 );

  // E864 towers (three types for three sizes)
  RawTowerCalibration *TowerCalibration3 = new RawTowerCalibration("FEMCRawTowerCalibration3");
  TowerCalibration3->Detector("FEMC");
  TowerCalibration3->TowerType(3);
  TowerCalibration3->Verbosity(verbosity);
  TowerCalibration3->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration3->set_calib_const_GeV_ADC(1.0 / 0.030);  // sampling fraction = 0.030
  TowerCalibration3->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration3);

  RawTowerCalibration *TowerCalibration4 = new RawTowerCalibration("FEMCRawTowerCalibration4");
  TowerCalibration4->Detector("FEMC");
  TowerCalibration4->TowerType(4);
  TowerCalibration4->Verbosity(verbosity);
  TowerCalibration4->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration4->set_calib_const_GeV_ADC(1.0 / 0.030);  // sampling fraction = 0.030
  TowerCalibration4->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration4);

  RawTowerCalibration *TowerCalibration5 = new RawTowerCalibration("FEMCRawTowerCalibration5");
  TowerCalibration5->Detector("FEMC");
  TowerCalibration5->TowerType(5);
  TowerCalibration5->Verbosity(verbosity);
  TowerCalibration5->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration5->set_calib_const_GeV_ADC(1.0 / 0.030);  // sampling fraction = 0.030
  TowerCalibration5->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration5);

  RawTowerCalibration *TowerCalibration6 = new RawTowerCalibration("FEMCRawTowerCalibration6");
  TowerCalibration6->Detector("FEMC");
  TowerCalibration6->TowerType(6);
  TowerCalibration6->Verbosity(verbosity);
  TowerCalibration6->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration6->set_calib_const_GeV_ADC(1.0 / 0.030);  // sampling fraction = 0.030
  TowerCalibration6->set_pedstal_ADC(0);
  se->registerSubsystem(TowerCalibration6);
}

void FEMC_Clusters()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  if (G4FEMC::Femc_clusterizer == G4FEMC::kFemcTemplateClusterizer)
  {
    RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplateFEMC");
    ClusterBuilder->Detector("FEMC");
    ClusterBuilder->Verbosity(verbosity);
    ClusterBuilder->set_threshold_energy(0.020);  // This threshold should be the same as in FEMCprof_Thresh**.root file below
    std::string femc_prof = getenv("CALIBRATIONROOT");
    femc_prof += "/EmcProfile/FEMCprof_Thresh20MeV.root";
    ClusterBuilder->LoadProfile(femc_prof.c_str());
    se->registerSubsystem(ClusterBuilder);
  }
  else if (G4FEMC::Femc_clusterizer == G4FEMC::kFemcGraphClusterizer)
  {
    RawClusterBuilderFwd *ClusterBuilder = new RawClusterBuilderFwd("FEMCRawClusterBuilderFwd");

    ClusterBuilder->Detector("FEMC");
    ClusterBuilder->Verbosity(verbosity);
    ClusterBuilder->set_threshold_energy(0.010);
    se->registerSubsystem(ClusterBuilder);
  }
  else
  {
    cout << "FEMC_Clusters - unknown clusterizer setting!" << endl;
    exit(1);
  }

  return;
}

void FEMC_Eval(std::string outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FEMC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("FEMCEVALUATOR", "FEMC", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
#endif
