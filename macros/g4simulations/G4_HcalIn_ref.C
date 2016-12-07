// Init is called by G4Setup.C
void HCalInnerInit() {}

double HCalInner(PHG4Reco* g4Reco,
		 double radius,
		 const int crossings,
		 const int absorberactive = 0,
		 int verbosity = 0) {
  // all sizes are in cm!  

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  PHG4InnerHcalSubsystem *hcal = new PHG4InnerHcalSubsystem("HCALIN");
  // these are the parameters you can change with their default settings
  // hcal->set_string_param("material","SS310");
  // hcal->set_int_param("ncross",4);
  // hcal->set_int_param("n_scinti_tiles",12);
  // hcal->set_int_param("light_scint_model",1);
  // hcal->set_double_param("inner_radius",116);
  // hcal->set_double_param("outer_radius",136);
  // hcal->set_double_param("scinti_inner_gap",0.85);
  // hcal->set_double_param("scinti_outer_gap",1.22);
  // hcal->set_double_param("scinti_tile_thickness",0.7);
  // hcal->set_double_param("scinti_gap_neighbor",0.1);
  // the SetLightCorrection is a convenience method, no
  // point in forcing users to set all 4 of them separately
  // and maybe forgetting one
  // hcal->SetLightCorrection(NAN,NAN,NAN,NAN);
  // hcal->set_double_param("place_x",0);
  // hcal->set_double_param("place_y",0);
  // hcal->set_double_param("place_z",0);
  // hcal->set_double_param("rot_x",0);
  // hcal->set_double_param("rot_y",0);
  // hcal->set_double_param("rot_z",0);
  // Flat plates with 4 scintillators per tower:
  hcal->set_int_param("n_scinti_plates_per_tower",4);
  hcal->set_double_param("scinti_outer_gap",1.22*(5.0/4.0));

  hcal->SetActive();
  hcal->SuperDetector("HCALIN");
  if (absorberactive)  
    {
      hcal->SetAbsorberActive();
    }
  hcal->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem( hcal );

  radius = hcal->get_double_param("outer_radius");

  HCalInner_SupportRing(g4Reco,absorberactive);
  
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
      cyl->set_double_param("place_z",z_rings[i]);
      cyl->SuperDetector("HCALIN_SPT");
      cyl->set_double_param("radius",innerradius);
      cyl->set_int_param("lengthviarapidity",0);
      cyl->set_double_param("length",dz);
      cyl->set_string_param("material","SS310");
      cyl->set_double_param("thickness",maxradius - innerradius);
      if (absorberactive)
	{
	  cyl->SetActive();
	}
      g4Reco->registerSubsystem(cyl);
    }
  
  return;
}


void HCALInner_Cells(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4HcalCellReco *hc = new PHG4HcalCellReco("HCALIN_CELLRECO");
  hc->Detector("HCALIN");
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

void HCALInner_Towers(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  
  HcalRawTowerBuilder *TowerBuilder = new HcalRawTowerBuilder("HcalInRawTowerBuilder");
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
