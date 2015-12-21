using namespace std;

// global macro parameters
bool overlapcheck = false; // set to true if you want to check for overlaps

void
FEMCInit()
{
}

void FEMC_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4ForwardCalCellReco *hc = new PHG4ForwardCalCellReco("FEMCCellReco");
  hc->Detector("FEMC");
  se->registerSubsystem(hc);
  
  return;  
}

void
FEMCSetup(PHG4Reco* g4Reco, const int absorberactive = 0)
{

  gSystem->Load("libg4detectors.so");

  Fun4AllServer *se = Fun4AllServer::instance();

  /** Use dedicated FEMC module */
  PHG4ForwardEcalSubsystem *femc = new PHG4ForwardEcalSubsystem("FEMC");

  ostringstream mapping_femc;

  /* path to central copy of calibrations repositry */
  mapping_femc << getenv("OFFLINE_MAIN") << "/share/calibrations";
  mapping_femc << "/ForwardEcal/mapping/towerMap_FEMC_v004.txt";
  cout << mapping_femc.str() << endl;
  //mapping_femc << "towerMap_FEMC_latest.txt";

  femc->SetTowerMappingFile( mapping_femc.str() );
  femc->OverlapCheck(overlapcheck);

  if (absorberactive)  femc->SetAbsorberActive();

  g4Reco->registerSubsystem( femc );

}

void FEMC_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  ostringstream mapping_femc;
  mapping_femc << getenv("OFFLINE_MAIN") <<
   	"/share/calibrations/ForwardEcal/mapping/towerMap_FEMC_v004.txt";
  //mapping_femc << "towerMap_FEMC_latest.txt";

  RawTowerBuilderByHitIndex* tower_FEMC = new RawTowerBuilderByHitIndex("TowerBuilder_FEMC");
  tower_FEMC->Detector("FEMC");
  tower_FEMC->set_sim_tower_node_prefix("SIM");
  tower_FEMC->GeometryTableFile( mapping_femc.str() );

  se->registerSubsystem(tower_FEMC);

  // const double FEMC_photoelectron_per_GeV = 500; //500 photon per total GeV deposition

  // RawTowerDigitizer *TowerDigitizer_FEMC = new RawTowerDigitizer("FEMCRawTowerDigitizer");
  // TowerDigitizer_FEMC->Detector("FEMC");
  // TowerDigitizer_FEMC->Verbosity(verbosity);
  // TowerDigitizer_FEMC->set_raw_tower_node_prefix("RAW");
  // TowerDigitizer_FEMC->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
  // TowerDigitizer_FEMC->set_pedstal_central_ADC(0);
  // TowerDigitizer_FEMC->set_pedstal_width_ADC(8);// eRD1 test beam setting
  // TowerDigitizer_FEMC->set_photonelec_ADC(1);//not simulating ADC discretization error
  // TowerDigitizer_FEMC->set_photonelec_yield_visible_GeV( FEMC_photoelectron_per_GeV );
  // TowerDigitizer_FEMC->set_zero_suppression_ADC(16); // eRD1 test beam setting

  // se->registerSubsystem( TowerDigitizer_FEMC );

  // RawTowerCalibration *TowerCalibration_FEMC = new RawTowerCalibration("FEMCRawTowerCalibration");
  // TowerCalibration_FEMC->Detector("FEMC");
  // TowerCalibration_FEMC->Verbosity(verbosity);
  // TowerCalibration_FEMC->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  // TowerCalibration_FEMC->set_calib_const_GeV_ADC( 1. / FEMC_photoelectron_per_GeV );
  // TowerCalibration_FEMC->set_pedstal_ADC( 0 );

  // se->registerSubsystem( TowerCalibration_FEMC );

  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("FEMCRawTowerDigitizer");
  TowerDigitizer->Detector("FEMC");
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitalization);
  se->registerSubsystem( TowerDigitizer );

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("FEMCRawTowerCalibration");
  TowerCalibration->Detector("FEMC");
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1.0/0.06376);  // calibrated with muons
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem( TowerCalibration );

}

void FEMC_Clusters(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawClusterBuilderFwd* ClusterBuilder = new RawClusterBuilderFwd("FEMCRawClusterBuilderFwd");
  ClusterBuilder->Detector("FEMC");
  ClusterBuilder->Verbosity(verbosity);
  se->registerSubsystem( ClusterBuilder );
  
  return;
}
