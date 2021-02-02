#ifndef MACRO_G4EEMC_C
#define MACRO_G4EEMC_C

#include <GlobalVariables.C>

#include <g4calo/RawTowerBuilderByHitIndex.h>
#include <g4calo/RawTowerDigitizer.h>
#include <g4detectors/PHG4CrystalCalorimeterSubsystem.h>
#include <g4detectors/PHG4ForwardCalCellReco.h>

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
  bool EEMC = false;
  bool EEMC_ABSORBER = false;
  bool EEMC_CELL = false;
  bool EEMC_TOWER = false;
  bool EEMC_CLUSTER = false;
  bool EEMC_EVAL = false;
  bool EEMC_OVERLAPCHECK = false;
  int EEMC_VERBOSITY = 0;
}  // namespace Enable

namespace G4EEMC
{
  int use_projective_geometry = 0;
  double Gdz = 18. + 0.0001;
  double Gz0 = -170.;
  enum enu_Eemc_clusterizer
  {
    kEemcGraphClusterizer,
    kEemcTemplateClusterizer
  };
  //default template clusterizer, as developed by Sasha Bazilevsky
  enu_Eemc_clusterizer Eemc_clusterizer = kEemcTemplateClusterizer;
  // graph clusterizer
  //enu_Eemc_clusterizer Eemc_clusterizer = kEemcGraphClusterizer;

}  // namespace G4EEMC

void EEMCInit()
{
  if (G4EEMC::use_projective_geometry)
  {
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 81.);
  }
  else
  {
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 65.6);
  }
  // from towerMap_EEMC_v006.txt
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, G4EEMC::Gz0 - G4EEMC::Gdz / 2.);
}

void EEMCSetup(PHG4Reco *g4Reco)
{
  bool AbsorberActive = Enable::ABSORBER || Enable::EEMC_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::EEMC_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::EEMC_VERBOSITY);

  /** Use dedicated EEMC module */
  PHG4CrystalCalorimeterSubsystem *eemc = new PHG4CrystalCalorimeterSubsystem("EEMC");
  eemc->SuperDetector("EEMC");
  eemc->SetActive();
  if (AbsorberActive)
  {
    eemc->SetAbsorberActive();
  }

  /* path to central copy of calibrations repository */
  ostringstream mapping_eemc;

  /* Use non-projective geometry */
  if (!G4EEMC::use_projective_geometry)
  {
    mapping_eemc << getenv("CALIBRATIONROOT") << "/CrystalCalorimeter/mapping/towerMap_EEMC_v006.txt";
    eemc->set_string_param("mappingtower", mapping_eemc.str());
  }

  /* use projective geometry */
  else
  {
    cout << "The projective version has serious problems with overlaps" << endl;
    cout << "Do Not Use!" << endl;
    cout << "If you insist, copy G4_EEMC.C locally and comment out this exit" << endl;
    gSystem->Exit(1);
    ostringstream mapping_eemc_4x4construct;

    mapping_eemc << getenv("CALIBRATIONROOT") << "/CrystalCalorimeter/mapping/crystals_v005.txt";
    mapping_eemc_4x4construct << getenv("CALIBRATIONROOT") << "/CrystalCalorimeter/mapping/4_by_4_construction_v005.txt";
    eemc->SetProjectiveGeometry(mapping_eemc.str(), mapping_eemc_4x4construct.str());
  }

  eemc->OverlapCheck(OverlapCheck);

  /* register Ecal module */
  g4Reco->registerSubsystem(eemc);
}

void EEMC_Cells()
{
}

void EEMC_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::EEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_eemc;
  mapping_eemc << getenv("CALIBRATIONROOT") << "/CrystalCalorimeter/mapping/towerMap_EEMC_v006.txt";

  RawTowerBuilderByHitIndex *tower_EEMC = new RawTowerBuilderByHitIndex("TowerBuilder_EEMC");
  tower_EEMC->Detector("EEMC");
  tower_EEMC->set_sim_tower_node_prefix("SIM");
  tower_EEMC->GeometryTableFile(mapping_eemc.str());

  se->registerSubsystem(tower_EEMC);

  /* Calorimeter digitization */

  // CMS lead tungstate barrel ECAL at 18 degree centrigrade: 4.5 photoelectrons per MeV
  const double EEMC_photoelectron_per_GeV = 4500;

  RawTowerDigitizer *TowerDigitizer_EEMC = new RawTowerDigitizer("EEMCRawTowerDigitizer");
  TowerDigitizer_EEMC->Detector("EEMC");
  TowerDigitizer_EEMC->Verbosity(verbosity);
  TowerDigitizer_EEMC->set_raw_tower_node_prefix("RAW");
  TowerDigitizer_EEMC->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitization);
  TowerDigitizer_EEMC->set_pedstal_central_ADC(0);
  TowerDigitizer_EEMC->set_pedstal_width_ADC(8);  // eRD1 test beam setting
  TowerDigitizer_EEMC->set_photonelec_ADC(1);     //not simulating ADC discretization error
  TowerDigitizer_EEMC->set_photonelec_yield_visible_GeV(EEMC_photoelectron_per_GeV);
  TowerDigitizer_EEMC->set_zero_suppression_ADC(16);  // eRD1 test beam setting

  se->registerSubsystem(TowerDigitizer_EEMC);

  /* Calorimeter calibration */

  RawTowerCalibration *TowerCalibration_EEMC = new RawTowerCalibration("EEMCRawTowerCalibration");
  TowerCalibration_EEMC->Detector("EEMC");
  TowerCalibration_EEMC->Verbosity(verbosity);
  TowerCalibration_EEMC->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration_EEMC->set_calib_const_GeV_ADC(1. / EEMC_photoelectron_per_GeV);
  TowerCalibration_EEMC->set_pedstal_ADC(0);

  se->registerSubsystem(TowerCalibration_EEMC);
}

void EEMC_Clusters()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::EEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  if (G4EEMC::Eemc_clusterizer == G4EEMC::kEemcTemplateClusterizer)
  {
    RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EEMCRawClusterBuilderTemplate");

    ClusterBuilder->Detector("EEMC");
    ClusterBuilder->Verbosity(verbosity);
    se->registerSubsystem(ClusterBuilder);
  }
  else if (G4EEMC::Eemc_clusterizer == G4EEMC::kEemcGraphClusterizer)
  {
    RawClusterBuilderFwd *ClusterBuilder = new RawClusterBuilderFwd("EEMCRawClusterBuilderFwd");

    ClusterBuilder->Detector("EEMC");
    ClusterBuilder->Verbosity(verbosity);
    se->registerSubsystem(ClusterBuilder);
  }
  else
  {
    cout << "EEMC_Clusters - unknown clusterizer setting " << G4EEMC::Eemc_clusterizer << endl;
    gSystem->Exit(1);
  }
  return;
}

void EEMC_Eval(const std::string &outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::EEMC_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("EEMCEVALUATOR", "EEMC", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
#endif
