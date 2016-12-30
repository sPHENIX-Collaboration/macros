// Init is called by G4Setup.C
void HCalOuterInit(){}

double HCalOuter(PHG4Reco* g4Reco,
		 double radius,
		 const int crossings,
		 const int absorberactive = 0,
		 int verbosity = 0) {

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4OuterHcalSubsystem *hcal = new PHG4OuterHcalSubsystem("HCALOUT");
  // integer params:
  //  hcal->set_int_param("light_scint_model",1);
  //  hcal->set_int_param("magnet_cutout_first_scint",8);
  //  hcal->set_int_param("n_scinti_plates_per_tower",5);
  //  hcal->set_int_param("n_scinti_tiles",12);
  //  hcal->set_int_param("n_towers",64);
  //  hcal->set_int_param("ncross",-4);
  // double params:
  //  hcal->set_double_param("inner_radius", 178);
  //  hcal->set_double_param("magnet_cutout", 12);
  //  hcal->set_double_param("outer_radius", 260);
  //  hcal->set_double_param("place_x", 0);
  //  hcal->set_double_param("place_y", 0);
  //  hcal->set_double_param("place_z", 0);
  //  hcal->set_double_param("rot_x", 0);
  //  hcal->set_double_param("rot_y", 0);
  //  hcal->set_double_param("rot_z", 0);
  //  hcal->set_double_param("scinti_eta_coverage", 1.1);
  //  hcal->set_double_param("scinti_gap", 0.85);
  //  hcal->set_double_param("scinti_gap_neighbor", 0.1);
  //  hcal->set_double_param("scinti_tile_thickness", 0.7);
  //  hcal->set_double_param("size_z", 609.82);
  //  hcal->set_double_param("steplimits", nan);
  // angle is calculated from crossings
  //  hcal->set_double_param("tilt_angle",NAN);
  // convenience method so we do not forget to set all of them
  //  hcal->SetLightCorrection(NAN,NAN,NAN,NAN);
  // string params:
  //  hcal->set_string_param("material","Steel_1006");

  hcal->SetActive();
  hcal->SuperDetector("HCALOUT");
  if (absorberactive)
    {
      hcal->SetAbsorberActive();
    }
  hcal->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(hcal);

  radius = hcal->get_double_param("outer_radius");
  
  radius += no_overlapp;
  
  return radius; 
}

void HCALOuter_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  PHG4HcalCellReco *hc = new PHG4HcalCellReco("HCALOUT_CELLRECO");
  hc->Detector("HCALOUT");
  //  hc->Verbosity(2);
  // check for energy conservation - needs modified "infinite" timing cuts
  // 0-999999999 
  //  hc->checkenergy();
  // timing cuts with their default settings
  // hc->set_double_param("tmin",0.); 
  // hc->set_double_param("tmax",60.0); 
  // or all at once:
  // hc->set_timing_window(0.0,60.0);
  se->registerSubsystem(hc);

  return;
}

void HCALOuter_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  HcalRawTowerBuilder* TowerBuilder = new HcalRawTowerBuilder("HcalOutRawTowerBuilder");
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
