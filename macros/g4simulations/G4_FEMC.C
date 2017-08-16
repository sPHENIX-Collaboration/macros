using namespace std;

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

  // fsPHENIX ECAL
  femc->SetfsPHENIXDetector(); 
  mapping_femc<< getenv("CALIBRATIONROOT") << "/ForwardEcal/mapping/towerMap_FEMC_fsPHENIX_v002.txt";

  cout << mapping_femc.str() << endl;

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

  // fsPHENIX ECAL
  mapping_femc << getenv("CALIBRATIONROOT") <<
   	"/ForwardEcal/mapping/towerMap_FEMC_fsPHENIX_v002.txt";

  RawTowerBuilderByHitIndex* tower_FEMC = new RawTowerBuilderByHitIndex("TowerBuilder_FEMC");
  tower_FEMC->Detector("FEMC");
  tower_FEMC->set_sim_tower_node_prefix("SIM");
  tower_FEMC->GeometryTableFile( mapping_femc.str() );

  se->registerSubsystem(tower_FEMC);

  // PbW crystals
  RawTowerDigitizer *TowerDigitizer1 = new RawTowerDigitizer("FEMCRawTowerDigitizer1");
  TowerDigitizer1->Detector("FEMC");
  TowerDigitizer1->TowerType(1); 
  TowerDigitizer1->Verbosity(verbosity);
  TowerDigitizer1->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem( TowerDigitizer1 );

  // PbSc towers
  RawTowerDigitizer *TowerDigitizer2 = new RawTowerDigitizer("FEMCRawTowerDigitizer2");
  TowerDigitizer2->Detector("FEMC");
  TowerDigitizer2->TowerType(2); 
  TowerDigitizer2->Verbosity(verbosity);
  TowerDigitizer2->set_digi_algorithm(RawTowerDigitizer::kNo_digitization);
  se->registerSubsystem( TowerDigitizer2 );

  // PbW crystals
  RawTowerCalibration *TowerCalibration1 = new RawTowerCalibration("FEMCRawTowerCalibration1");
  TowerCalibration1->Detector("FEMC");
  TowerCalibration1->TowerType(1);
  TowerCalibration1->Verbosity(verbosity);
  TowerCalibration1->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration1->set_calib_const_GeV_ADC(1.0);  // sampling fraction = 1.0
  TowerCalibration1->set_pedstal_ADC(0);
  se->registerSubsystem( TowerCalibration1 );

  // PbSc towers
  RawTowerCalibration *TowerCalibration2 = new RawTowerCalibration("FEMCRawTowerCalibration2");
  TowerCalibration2->Detector("FEMC");
  TowerCalibration2->TowerType(2);
  TowerCalibration2->Verbosity(verbosity);
  TowerCalibration2->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration2->set_calib_const_GeV_ADC(1.0/0.249);  // sampling fraction = 0.249 for e-
  TowerCalibration2->set_pedstal_ADC(0);
  se->registerSubsystem( TowerCalibration2 );

}

void FEMC_Clusters(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawClusterBuilderFwd* ClusterBuilder = new RawClusterBuilderFwd("FEMCRawClusterBuilderFwd");
  ClusterBuilder->Detector("FEMC");
  ClusterBuilder->Verbosity(verbosity);
  ClusterBuilder->set_threshold_energy(0.100);  
  se->registerSubsystem( ClusterBuilder );
  
  return;
}

void FEMC_Eval(std::string outputfile, int verbosity = 0)
{
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator *eval = new CaloEvaluator("FEMCEVALUATOR", "FEMC", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}
