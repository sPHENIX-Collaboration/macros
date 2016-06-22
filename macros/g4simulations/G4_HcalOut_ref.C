
int Min_hcal_out_layer = 0;
int Max_hcal_out_layer = 0;

void HCalOuterInit()
{
  Min_hcal_out_layer = 1;
  Max_hcal_out_layer = 1;
}

double HCalOuter(PHG4Reco* g4Reco,
		 double radius,
		 const int crossings,
		 const int absorberactive = 0,
		 int verbosity = 0) {
  
  double outer_hcal_inner_radius = 178.0; // stay clear value
  double outer_hcal_outer_radius = 260.3;
   
  if (radius > outer_hcal_inner_radius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than HCALOUT inner radius: " << outer_hcal_inner_radius << endl;
    gSystem->Exit(-1);
  }
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4OuterHcalSubsystem *outerhcal = new PHG4OuterHcalSubsystem("HCALOUT");
  outerhcal->SetActive();
  outerhcal->set_int_param("ncross",-4);
  //outerhcal->SetLightCorrection(178.0,0.65,260.3,1.0);
  outerhcal->SuperDetector("HCALOUT");
  if (absorberactive)  outerhcal->SetAbsorberActive();
  outerhcal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem( outerhcal );

  radius = outer_hcal_outer_radius;
  
  if (verbosity > 0) {
    cout << "==================== G4_HcalOut_ref.C::HCalOuter() ========================" << endl;
    cout << " HCALOUT Material Description:" << endl;
    cout << "  outer radius = " << outer_hcal_outer_radius << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  
  return radius; 
}

void HCALOuter_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  PHG4HcalCellReco *hc = new PHG4HcalCellReco();
  hc->Detector("HCALOUT");
  hc->etasize_nslat(0, 0, 5);
  hc->set_timing_window_defaults(0.0,60.0);
  se->registerSubsystem(hc);

  return;
}

void HCALOuter_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawTowerBuilder* TowerBuilder = new RawTowerBuilder("HcalOutRawTowerBuilder");
  TowerBuilder->Detector("HCALOUT");
  TowerBuilder->set_sim_tower_node_prefix("SIM");
  TowerBuilder->Verbosity(verbosity);
  se->registerSubsystem( TowerBuilder );

  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("HcalOutRawTowerDigitizer");
  TowerDigitizer->Detector("HCALOUT");
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitalization);
  se->registerSubsystem( TowerDigitizer );

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("HcalOutRawTowerCalibration");
  TowerCalibration->Detector("HCALOUT");
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1./0.0305);// muon sampling fraction from Abhisek Sen, 2015 SBU simulation workfest
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem( TowerCalibration );
  return;
}

void HCALOuter_Clusters(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawClusterBuilder* ClusterBuilder = new RawClusterBuilder("HcalOutRawClusterBuilder");
  ClusterBuilder->Detector("HCALOUT");
  ClusterBuilder->Verbosity(verbosity);
  se->registerSubsystem( ClusterBuilder );
  
  return;
}

void HCALOuter_Eval(std::string outputfile, int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  CaloEvaluator* eval = new CaloEvaluator("HCALOUTEVALUATOR", "HCALOUT", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );
  
  return;
}
