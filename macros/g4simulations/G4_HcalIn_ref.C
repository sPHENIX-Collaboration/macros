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
  // these are the parameters you can change with their defaults
  // cemc->GetParameters()->set_material("SS310");
  // cemc->GetParameters()->set_ncross(4);
  // cemc->GetParameters()->set_n_scinti_plates(5 * 64);
  // cemc->GetParameters()->set_n_scinti_tiles(12);
  // cemc->GetParameters()->set_light_scint_model(1);
  // cemc->GetParameters()->set_inner_radius(116);
  // cemc->GetParameters()->set_outer_radius(136);
  // cemc->GetParameters()->set_scinti_gap(0.85);
  // cemc->GetParameters()->set_scinti_tile_thickness(0.7);
  // cemc->GetParameters()->set_scinti_gap_neighbor(0.1);
  // cemc->GetParameters()->SetLightCorrection(NAN,NAN,NAN,NAN);
  // cemc->GetParameters()->set_place(0, 0, 0);
  // cemc->GetParameters()->set_rot_x(0);
  // cemc->GetParameters()->set_rot_y(0);
  // cemc->GetParameters()->set_rot_z(0);

  hcal->SetActive();
  hcal->SuperDetector("HCALIN");
  if (absorberactive)  hcal->SetAbsorberActive();
  hcal->OverlapCheck(overlapcheck);
  double innerradius = hcal->GetParameters()->get_inner_radius()/10;
  if (radius > innerradius) {
    cout << "inconsistency: radius: " << radius 
	 << " larger than HCALIN inner radius: " << innerradius
	 << " cm" << endl;
    gSystem->Exit(-1);
  }

  g4Reco->registerSubsystem( hcal );

  radius = hcal->GetParameters()->get_outer_radius()/10;

  HCalInner_SupportRing(g4Reco,absorberactive);
  
  if (verbosity > 0) {
    cout << "==================== G4_HcalIn_ref.C::HCalInner() =========================" << endl;
    cout << " HCALIN Material Description:" << endl;
    cout << "  inner radius = " << hcal->get_inner_radius()/10 << " cm" << endl;
    cout << "  outer radius = " << hcal->get_outer_radius()/10 << " cm" << endl;
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
