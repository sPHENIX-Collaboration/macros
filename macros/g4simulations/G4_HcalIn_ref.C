// these are defined in the Fun4All macro, here we just override the values
// with what is used in this macro
Min_hcal_in_layer = 0;
Max_hcal_in_layer = 0;

void HCalInnerInit() {
  Min_hcal_in_layer = 0;
  Max_hcal_in_layer = 0;
}

double HCalInner(PHG4Reco* g4Reco,
		 double radius,
		 const int crossings,
		 const int absorberactive = 0,
		 int verbosity = 0) {
  // all sizes are in cm!  

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  int ilayer = Min_hcal_in_layer;
  PHG4InnerHcalSubsystem *hcal;
  hcal = new PHG4InnerHcalSubsystem("HCALIN");
  hcal->SetMaterial("SS310"); // SS310 stainless steel
  // these are all the defaults
  // hcal->SetGapWidth(0.85);
  // hcal->SetScintiThickness(0.7);
  // hcal->SetNumScintiPlates(5*64);
  hcal->SetTiltViaNcross(4); 
  hcal->SetActive();
  hcal->SuperDetector("HCALIN");
  if (absorberactive)  hcal->SetAbsorberActive();
  hcal->OverlapCheck(overlapcheck);
  //hcal->SetLightCorrection(116.0,0.85,135.0,1.0); 
  double innerradius = hcal->GetInnerRadius();
  if (radius > innerradius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than HCALIN inner radius: " << innerradius << endl;
    gSystem->Exit(-1);
  }

  g4Reco->registerSubsystem( hcal );

  radius = hcal->GetOuterRadius();

  HCalInner_SupportRing(g4Reco,absorberactive);
  
  if (verbosity > 0) {
    cout << "==================== G4_HcalIn_ref.C::HCalInner() =========================" << endl;
    cout << " HCALIN Material Description:" << endl;
    cout << "  inner radius = " << hcal->GetInnerRadius() << " cm" << endl;
    cout << "  outer radius = " << hcal->GetOuterRadius() << " cm" << endl;
    cout << "===========================================================================" << endl;
  }

  radius += no_overlapp;
  return radius;
}

//! A rough version of the inner HCal support ring, from Richie's CAD drawing. - Jin
void HCalInner_SupportRing(PHG4Reco* g4Reco,
			   const int absorberactive = 0) {
  
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  const double z_ring1 = (2025 + 2050) / 2. / 10.;
  const double z_ring2 = (2150 + 2175) / 2. / 10.;
  const double dz = 25. / 10.;
  const double innerradius = 116.;
  const double maxradius = 178.0 - 0.001; // avoid touching the outer HCal envelop volumne

  const double z_rings[] =
    { -z_ring2, -z_ring1, z_ring1, z_ring2, 0, 0, 0, 0 };

  PHG4CylinderSubsystem *cyl;

  for (int i = 0; i < 4; i++)
    {
      cyl = new PHG4CylinderSubsystem("HCALIN_SPT_N1", i);
      cyl->SetPosition(0, 0, z_rings[i]);
      cyl->SuperDetector("HCALIN_SPT");
      cyl->SetRadius(innerradius);
      cyl->SetLengthViaRapidityCoverage(false);
      cyl->SetLength(dz);
      cyl->SetMaterial("SS310");
      cyl->SetThickness(maxradius - innerradius);
      if (absorberactive)
        cyl->SetActive();
      g4Reco->registerSubsystem(cyl);
    }
  
  return;
}


void HCALInner_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4HcalCellReco *hc = new PHG4HcalCellReco();
  hc->Detector("HCALIN");
  hc->etasize_nslat(0, 0, 5);
  se->registerSubsystem(hc);
  
  return;  
}

void HCALInner_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawTowerBuilder *TowerBuilder = new RawTowerBuilder("HcalInRawTowerBuilder");
  TowerBuilder->Detector("HCALIN");
  TowerBuilder->set_sim_tower_node_prefix("SIM");
  TowerBuilder->Verbosity(verbosity);
  se->registerSubsystem( TowerBuilder );

  RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("HcalInRawTowerDigitizer");
  TowerDigitizer->Detector("HCALIN");
  TowerDigitizer->Verbosity(verbosity);
  TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kNo_digitalization);
  se->registerSubsystem( TowerDigitizer );

  RawTowerCalibration *TowerCalibration = new RawTowerCalibration("HcalInRawTowerCalibration");
  TowerCalibration->Detector("HCALIN");
  TowerCalibration->Verbosity(verbosity);
  TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
  TowerCalibration->set_calib_const_GeV_ADC(1./0.067);// muon sampling fraction from Abhisek Sen, 2015 SBU simulation workfest
  TowerCalibration->set_pedstal_ADC(0);
  se->registerSubsystem( TowerCalibration );
  return;
}

void HCALInner_Clusters(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  RawClusterBuilder* ClusterBuilder = new RawClusterBuilder("HcalInRawClusterBuilder");
  ClusterBuilder->Detector("HCALIN");
  ClusterBuilder->Verbosity(verbosity);
  se->registerSubsystem( ClusterBuilder );
  
  return;
}

void HCALInner_Eval(std::string outputfile, int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");
  Fun4AllServer *se = Fun4AllServer::instance();
    
  CaloEvaluator* eval = new CaloEvaluator("HCALINEVALUATOR", "HCALIN", outputfile.c_str());
  eval->Verbosity(verbosity);
  se->registerSubsystem( eval );
      
  return;
}
