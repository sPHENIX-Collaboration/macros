#ifndef MACRO_G4FHCAL_C
#define MACRO_G4FHCAL_C

#include <GlobalVariables.C>

#include <g4calo/RawTowerBuilderByHitIndex.h>
#include <g4calo/RawTowerDigitizer.h>

#include <g4detectors/PHG4ForwardCalCellReco.h>
#include <g4detectors/PHG4ForwardHcalSubsystem.h>

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
  bool FHCAL = false;
  bool FHCAL_ABSORBER = false;
  bool FHCAL_CELL = false;
  bool FHCAL_TOWER = false;
  bool FHCAL_CLUSTER = false;
  bool FHCAL_EVAL = false;
  bool FHCAL_OVERLAPCHECK = false;
  int FHCAL_VERBOSITY = 0;
}  // namespace Enable

namespace G4FHCAL
{
  // from ForwardHcal/mapping/towerMap_FHCAL_v005.txt
  double Gz0 = 400.;
  double Gdz = 100.;
  double outer_radius = 262.;
  enum enu_FHcal_clusterizer
  {
    kFHcalGraphClusterizer,
    kFHcalTemplateClusterizer
  };
  //template clusterizer, as developed by Sasha Bazilevsky
  enu_FHcal_clusterizer FHcal_clusterizer = kFHcalTemplateClusterizer;
  // graph clusterizer
  //enu_FHcal_clusterizer FHcal_clusterizer = kFHcalGraphClusterizer;
  namespace SETTING
  {
    bool FullEtaAcc = false;
    bool HC2x = false;
    bool HC4x = false;
    bool towercalib1 = false;
    bool towercalibSiPM = false;
    bool towercalibHCALIN = false;
    bool towercalib3 = false;
  }  // namespace SETTING
}  // namespace G4FHCAL

void FHCALInit()
{
  // simple way to check if only 1 of the settings is true
  if ((G4FHCAL::SETTING::FullEtaAcc ? 1 : 0) + (G4FHCAL::SETTING::HC4x ? 1 : 0) + (G4FHCAL::SETTING::HC2x ? 1 : 0) > 1)
  {
    cout << "use only  G4FHCAL::SETTING::FullEtaAcc=true or G4FHCAL::SETTING::HC2x=true or G4FHCAL::SETTING::HC4x=true" << endl;
    gSystem->Exit(1);
  }
  if ((G4FHCAL::SETTING::towercalib1 ? 1 : 0) + (G4FHCAL::SETTING::towercalibSiPM ? 1 : 0) +
          (G4FHCAL::SETTING::towercalibHCALIN ? 1 : 0) + (G4FHCAL::SETTING::towercalib3 ? 1 : 0) >
      1)
  {
    cout << "use only G4FHCAL::SETTING::towercalib1 = true or G4FHCAL::SETTING::towercalibSiPM = true"
         << " or G4FHCAL::SETTING::towercalibHCALIN = true or G4FHCAL::SETTING::towercalib3 = true" << endl;
    gSystem->Exit(1);
  }

  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4FHCAL::outer_radius);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, G4FHCAL::Gz0 + G4FHCAL::Gdz / 2.);
}

void FHCALSetup(PHG4Reco *g4Reco)
{
  const bool AbsorberActive = Enable::ABSORBER || Enable::FHCAL_ABSORBER;
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::FHCAL_OVERLAPCHECK;
  Fun4AllServer *se = Fun4AllServer::instance();

  /** Use dedicated FHCAL module */
  PHG4ForwardHcalSubsystem *fhcal = new PHG4ForwardHcalSubsystem("FHCAL");

  ostringstream mapping_fhcal;

  // Switch to desired calo setup
  // HCal Fe-Scint with doubled granularity
  if (G4FHCAL::SETTING::HC2x )
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_2x.txt";
  }
  // full HCal Fe-Scint with nominal acceptance doubled granularity
  else if (G4FHCAL::SETTING::HC2x && G4FHCAL::SETTING::FullEtaAcc)
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_2x_fullEtaCov.txt";
  }
  // HCal Fe-Scint with four times granularity
  else if (G4FHCAL::SETTING::HC4x )
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_4x.txt";
  }
  // full HCal Fe-Scint with nominal acceptance four times granularity
  else if (G4FHCAL::SETTING::HC4x && G4FHCAL::SETTING::FullEtaAcc)
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_4x_fullEtaCov.txt";
  }
  // full HCal Fe-Scint with nominal acceptance
  else if (G4FHCAL::SETTING::FullEtaAcc)
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_default_fullEtaCov.txt";
  }
  // full HCal Fe-Scint with enlarged beam pipe opening for Mar 2020 beam pipe
  else
  {
    mapping_fhcal << getenv("CALIBRATIONROOT")
                  << "/ForwardHcal/mapping/towerMap_FHCAL_v005.txt";
  }

  fhcal->SetTowerMappingFile(mapping_fhcal.str());
  fhcal->OverlapCheck(OverlapCheck);
  fhcal->SetActive();
  fhcal->SuperDetector("FHCAL");
  if (AbsorberActive) fhcal->SetAbsorberActive();

  g4Reco->registerSubsystem(fhcal);
}

void FHCAL_Cells(int verbosity = 0)
{
  return;
}

void FHCAL_Towers()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FHCAL_VERBOSITY);

  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_fhcal;

  // Switch to desired calo setup
  // HCal Fe-Scint with doubled granularity
  if (G4FHCAL::SETTING::HC2x )
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_2x.txt";
  }
  // full HCal Fe-Scint with nominal acceptance doubled granularity
  else if (G4FHCAL::SETTING::HC2x && G4FHCAL::SETTING::FullEtaAcc)
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_2x_fullEtaCov.txt";
  }
  // HCal Fe-Scint with four times granularity
  else if (G4FHCAL::SETTING::HC4x )
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_4x.txt";
  }
  // full HCal Fe-Scint with nominal acceptance four times granularity
  else if (G4FHCAL::SETTING::HC4x && G4FHCAL::SETTING::FullEtaAcc)
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_4x_fullEtaCov.txt";
  }
  // full HCal Fe-Scint with nominal acceptance
  else if (G4FHCAL::SETTING::FullEtaAcc)
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_default_fullEtaCov.txt";
  }
  // full HCal Fe-Scint with enlarged beam pipe opening for Mar 2020 beam pipe
  else
  {
    mapping_fhcal << getenv("CALIBRATIONROOT") << "/ForwardHcal/mapping/towerMap_FHCAL_v005.txt";
  }

  RawTowerBuilderByHitIndex *tower_FHCAL = new RawTowerBuilderByHitIndex("TowerBuilder_FHCAL");
  tower_FHCAL->Detector("FHCAL");
  tower_FHCAL->set_sim_tower_node_prefix("SIM");
  tower_FHCAL->GeometryTableFile(mapping_fhcal.str());

  se->registerSubsystem(tower_FHCAL);

  // enable usage of different tower calibrations for systematic studies
  if (G4FHCAL::SETTING::towercalib1)
  {
    cout << "1: using towercalib1 for FHCAL towers" << endl;
    const double FHCAL_photoelectron_per_GeV = 500;
    RawTowerDigitizer *TowerDigitizer_FHCAL = new RawTowerDigitizer("FHCALRawTowerDigitizer");

    TowerDigitizer_FHCAL->Detector("FHCAL");
    TowerDigitizer_FHCAL->Verbosity(verbosity);
    TowerDigitizer_FHCAL->set_raw_tower_node_prefix("RAW");
    TowerDigitizer_FHCAL->set_digi_algorithm(RawTowerDigitizer::kSiPM_photon_digitization);
    TowerDigitizer_FHCAL->set_pedstal_central_ADC(0);
    TowerDigitizer_FHCAL->set_pedstal_width_ADC(8);  // eRD1 test beam setting
    TowerDigitizer_FHCAL->set_photonelec_ADC(1);     //not simulating ADC discretization error
    TowerDigitizer_FHCAL->set_photonelec_yield_visible_GeV(FHCAL_photoelectron_per_GeV);
    TowerDigitizer_FHCAL->set_zero_suppression_ADC(16);  // eRD1 test beam setting

    se->registerSubsystem(TowerDigitizer_FHCAL);

    RawTowerCalibration *TowerCalibration_FHCAL = new RawTowerCalibration("FHCALRawTowerCalibration");
    TowerCalibration_FHCAL->Detector("FHCAL");
    TowerCalibration_FHCAL->Verbosity(verbosity);
    TowerCalibration_FHCAL->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration_FHCAL->set_calib_const_GeV_ADC(1. / FHCAL_photoelectron_per_GeV);
    TowerCalibration_FHCAL->set_pedstal_ADC(0);

    se->registerSubsystem(TowerCalibration_FHCAL);
  }
  else if (G4FHCAL::SETTING::towercalibSiPM)
  {
    //from https://sphenix-collaboration.github.io/doxygen/d4/d58/Fun4All__G4__Prototype4_8C_source.html
    const double sampling_fraction = 0.019441;     //  +/-  0.019441 from 0 Degree indenting 12 GeV electron showers
    const double photoelectron_per_GeV = 500;      //500 photon per total GeV deposition
    const double ADC_per_photoelectron_HG = 3.8;   // From Sean Stoll, Mar 29
    const double ADC_per_photoelectron_LG = 0.24;  // From Sean Stoll, Mar 29

    cout << "2: using towercalibSiPM for FHCAL towers" << endl;
    RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("FHCALRawTowerDigitizer");
    TowerDigitizer->Detector("FHCAL");
    TowerDigitizer->set_raw_tower_node_prefix("RAW");
    TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSiPM_photon_digitization);
    TowerDigitizer->set_pedstal_central_ADC(0);
    TowerDigitizer->set_pedstal_width_ADC(1);
    TowerDigitizer->set_photonelec_ADC(1. / ADC_per_photoelectron_LG);
    TowerDigitizer->set_photonelec_yield_visible_GeV(photoelectron_per_GeV / sampling_fraction);
    TowerDigitizer->set_zero_suppression_ADC(-1000);  // no-zero suppression
    se->registerSubsystem(TowerDigitizer);

    RawTowerCalibration *TowerCalibration = new RawTowerCalibration("FHCALRawTowerCalibration");
    TowerCalibration->Detector("FHCAL");
    TowerCalibration->set_raw_tower_node_prefix("RAW");
    TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration->set_calib_const_GeV_ADC(1. / ADC_per_photoelectron_LG / photoelectron_per_GeV);
    TowerCalibration->set_pedstal_ADC(0);
    TowerCalibration->set_zero_suppression_GeV(-1);  // no-zero suppression
    se->registerSubsystem(TowerCalibration);
  }
  else if (G4FHCAL::SETTING::towercalibHCALIN)
  {
    const double visible_sample_fraction_HCALIN = 7.19505e-02;  // 1.34152e-02
    RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("FHCALRawTowerDigitizer");
    TowerDigitizer->Detector("FHCAL");
    TowerDigitizer->set_raw_tower_node_prefix("RAW");
    TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
    TowerDigitizer->set_pedstal_central_ADC(0);
    TowerDigitizer->set_pedstal_width_ADC(1);
    TowerDigitizer->set_photonelec_ADC(32. / 5.);
    TowerDigitizer->set_photonelec_yield_visible_GeV(32. / 5 / (0.4e-3));
    TowerDigitizer->set_zero_suppression_ADC(-1000);  // no-zero suppression
    se->registerSubsystem(TowerDigitizer);

    RawTowerCalibration *TowerCalibration = new RawTowerCalibration("FHCALRawTowerCalibration");
    TowerCalibration->Detector("FHCAL");
    TowerCalibration->set_raw_tower_node_prefix("RAW");
    TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration->set_calib_const_GeV_ADC(0.4e-3 / visible_sample_fraction_HCALIN);
    TowerCalibration->set_pedstal_ADC(0);
    TowerCalibration->set_zero_suppression_GeV(-1);  // no-zero suppression
    se->registerSubsystem(TowerCalibration);
  }
  else if (G4FHCAL::SETTING::towercalib3)
  {
    cout << "3: using towercalib3 for FHCAL towers" << endl;
    RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("FHCALRawTowerDigitizer");
    TowerDigitizer->Detector("FHCAL");
    TowerDigitizer->set_pedstal_central_ADC(0);
    TowerDigitizer->set_pedstal_width_ADC(8);  // eRD1 test beam setting
    TowerDigitizer->Verbosity(verbosity);
    TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
    se->registerSubsystem(TowerDigitizer);

    RawTowerCalibration *TowerCalibration = new RawTowerCalibration("FHCALRawTowerCalibration");
    TowerCalibration->Detector("FHCAL");
    TowerCalibration->Verbosity(verbosity);
    TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration->set_calib_const_GeV_ADC(1. / 0.03898);  // calibrated with muons
    TowerCalibration->set_pedstal_ADC(0);
    se->registerSubsystem(TowerCalibration);
  }
  else
  {
    cout << "def: using default for FHCAL towers" << endl;
    RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("FHCALRawTowerDigitizer");
    TowerDigitizer->Detector("FHCAL");
    TowerDigitizer->Verbosity(verbosity);
    TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
    se->registerSubsystem(TowerDigitizer);

    RawTowerCalibration *TowerCalibration = new RawTowerCalibration("FHCALRawTowerCalibration");
    TowerCalibration->Detector("FHCAL");
    TowerCalibration->Verbosity(verbosity);
    TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration->set_calib_const_GeV_ADC(1. / 0.03898);  // calibrated with muons
    TowerCalibration->set_pedstal_ADC(0);
    se->registerSubsystem(TowerCalibration);
  }
}

void FHCAL_Clusters()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FHCAL_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  if (G4FHCAL::FHcal_clusterizer == G4FHCAL::kFHcalTemplateClusterizer)
  {
    RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("FHCALRawClusterBuilderTemplate");
    ClusterBuilder->Detector("FHCAL");
    ClusterBuilder->SetPlanarGeometry();  // has to be called after Detector()
    ClusterBuilder->Verbosity(verbosity);
    ClusterBuilder->set_threshold_energy(0.100);
    se->registerSubsystem(ClusterBuilder);
  }
  else if (G4FHCAL::FHcal_clusterizer == G4FHCAL::kFHcalTemplateClusterizer)
  {
    RawClusterBuilderFwd *ClusterBuilder = new RawClusterBuilderFwd("FHCALRawClusterBuilderFwd");
    ClusterBuilder->Detector("FHCAL");
    ClusterBuilder->Verbosity(verbosity);
    ClusterBuilder->set_threshold_energy(0.100);
    se->registerSubsystem(ClusterBuilder);
  }
  else
  {
    cout << "FHCAL_Clusters - unknown clusterizer setting " << G4FHCAL::FHcal_clusterizer << endl;
    gSystem->Exit(1);
  }

  return;
}

void FHCAL_Eval(const std::string &outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::FHCAL_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("FHCALEVALUATOR", "FHCAL", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
#endif
