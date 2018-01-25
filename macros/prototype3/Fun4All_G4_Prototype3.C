int Fun4All_G4_Prototype3(int nEvents = 1)
{

  gSystem->Load("libfun4all");
  gSystem->Load("libg4detectors");
  gSystem->Load("libg4testbench");
  gSystem->Load("libg4histos");
  gSystem->Load("libg4eval.so");
  gSystem->Load("libqa_modules");
  gSystem->Load("libg4calo");
  gSystem->Load("libcalo_reco");

  bool cemc_on = true;
  bool cemc_cell = cemc_on && true;
  bool cemc_twr = cemc_cell && true;
  bool cemc_digi = cemc_twr && true;
  bool cemc_twrcal = cemc_digi && true;
  bool ihcal_on = true;
  bool ihcal_cell = ihcal_on && false;
  bool ihcal_twr = ihcal_cell && false;
  bool ihcal_digi = ihcal_twr && false;
  bool ihcal_twrcal = ihcal_digi && false;
  bool ohcal_on = true;
  bool ohcal_cell = ohcal_on && false;
  bool ohcal_twr = ohcal_cell && false;
  bool ohcal_digi = ohcal_twr && false;
  bool ohcal_twrcal =  ohcal_digi && false;
  bool cryo_on = true;
  bool bh_on = false; // the surrounding boxes need some further thinking
  bool dstreader = true;
  bool hit_ntuple = false;
  bool dstoutput = false;

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
  // only set this if you want a fixed random seed to make
  // results reproducible for testing
//    rc->set_IntFlag("RANDOMSEED",12345678);

  // simulated setup sits at eta=1, theta=40.395 degrees
  double theta = 90-46.4;
  // shift in x with respect to midrapidity setup
  double add_place_x = 183.-173.93+2.54/2.;
  // Test beam generator
  PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
  gen->add_particles("e-", 1); // mu-,e-,anti_proton,pi-
  gen->set_vertex_distribution_mean(0.0, 0.0, 0);
  gen->set_vertex_distribution_width(0.0, .7, .7); // Rough beam profile size @ 16 GeV measured by Abhisek
  gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Gaus,
					PHG4SimpleEventGenerator::Gaus, 
                                        PHG4SimpleEventGenerator::Gaus); // Gauss beam profile
  double angle = theta*TMath::Pi()/180.;
  double eta = -1.*TMath::Log(TMath::Tan(angle/2.));
  gen->set_eta_range(eta-0.001,eta+0.001); // 1mrad angular divergence
  gen->set_phi_range(-0.001, 0.001); // 1mrad angular divergence
  const double momentum = 32;
  gen->set_p_range(momentum,momentum, momentum*2e-2); // 2% momentum smearing
  se->registerSubsystem(gen);

  PHG4ParticleGenerator *pgen = new PHG4ParticleGenerator();
  pgen->set_name("geantino");
  //pgen->set_name(particle);
  pgen->set_vtx(0, 0, 0);
  //pgen->set_vtx(0, ypos, 0);
  double angle = theta*TMath::Pi()/180.;
  double eta = -1.*TMath::Log(TMath::Tan(angle/2.));
  pgen->set_eta_range(0.2*eta, 1.8*eta);
  //pgen->set_phi_range(-0.001, 0.001); // 1mrad angular diverpgence
  //pgen->set_phi_range(-0.5/180.*TMath::Pi(), 0.5/180.*TMath::Pi());
  //pgen->set_eta_range(-1., 1.);
  //pgen->set_phi_range(-0./180.*TMath::Pi(), 0./180.*TMath::Pi());
  pgen->set_phi_range(-20/180.*TMath::Pi(), 20/180.*TMath::Pi());
  pgen->set_mom_range(1, 1);
  //  se->registerSubsystem(pgen);

  // Simple single particle generator
  PHG4ParticleGun *gun = new PHG4ParticleGun();
  gun->set_name("geantino");
  //  gun->set_name("proton");
  gun->set_vtx(0, 0, 0);
  double angle = theta*TMath::Pi()/180.;
  gun->set_mom(sin(angle),0.,cos(angle));
//  se->registerSubsystem(gun);


  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_field(0);
  //  g4Reco->SetPhysicsList("QGSP_BERT_HP"); // uncomment this line to enable the high-precision neutron simulation physics list, QGSP_BERT_HP

  //----------------------------------------
  // EMCal G4
  //----------------------------------------
  if (cemc_on)
    {
      PHG4SpacalPrototypeSubsystem *cemc;
      cemc = new PHG4SpacalPrototypeSubsystem("CEMC");
      cemc->SetActive();
      cemc->SuperDetector("CEMC");
      cemc->SetAbsorberActive();
      cemc->OverlapCheck(true);
//      cemc->Verbosity(2);
//      cemc->set_int_param("construction_verbose",2);
      cemc->UseCalibFiles(PHG4DetectorSubsystem::xml);
      cemc->SetCalibrationFileDir(string(getenv("CALIBRATIONROOT")) + string("/Prototype3/Geometry/") );
//      cemc->SetCalibrationFileDir("./test_geom/" );
      //  cemc->set_double_param("z_rotation_degree", 15); // rotation around CG
//      cemc->set_double_param("xpos", (116.77 + 137.0)*.5 - 26.5 - 10.2); // location in cm of EMCal CG. Updated with final positioning of EMCal
//      cemc->set_double_param("ypos", 4); // put it some where in UIUC blocks
//      cemc->set_double_param("zpos", 4); // put it some where in UIUC blocks
      g4Reco->registerSubsystem(cemc);
    }
  //----------------------------------------
  // HCal G4
  //----------------------------------------
  if (ihcal_on)
    {
      PHG4Prototype2InnerHcalSubsystem *innerhcal = new PHG4Prototype2InnerHcalSubsystem("HCalIn");
      innerhcal->set_int_param("hi_eta",1);
      innerhcal->set_double_param("place_x",add_place_x);
      innerhcal->set_double_param("place_z",144);
      innerhcal->SetActive();
      innerhcal->SetAbsorberActive();
      innerhcal->SetAbsorberTruth(1);
      innerhcal->OverlapCheck(true);
      innerhcal->SuperDetector("HCALIN");
      g4Reco->registerSubsystem(innerhcal);
    }
  if (ohcal_on)
    {
      PHG4Prototype2OuterHcalSubsystem *outerhcal = new PHG4Prototype2OuterHcalSubsystem("HCalOut");
      outerhcal->set_int_param("hi_eta",1);
      outerhcal->set_double_param("place_x",add_place_x);
      outerhcal->set_double_param("place_z",229.5);
      outerhcal->SetActive();
      outerhcal->SetAbsorberActive();
      outerhcal->SetAbsorberTruth(1);
      outerhcal->OverlapCheck(true);
      outerhcal->SuperDetector("HCALOUT");
      g4Reco->registerSubsystem(outerhcal);
    }
  if (cryo_on)
    {
      double place_z = 175.;
      // Cryostat from engineering drawing
      PHG4BlockSubsystem *cryo1 = new PHG4BlockSubsystem("cryo1",1);
      cryo1->set_double_param("size_x",0.95);
      cryo1->set_double_param("size_y",60.96);
      cryo1->set_double_param("size_z",60.96);
      cryo1->set_double_param("place_x",141.96+0.95/2.+add_place_x);
      cryo1->set_double_param("place_z",place_z);
      cryo1->set_string_param("material","G4_Al");
      cryo1->SetActive(); // it is an active volume - save G4Hits
      cryo1->SuperDetector("CRYO");
      g4Reco->registerSubsystem(cryo1);

      PHG4BlockSubsystem *cryo2 = new PHG4BlockSubsystem("cryo2",2);
      cryo2->set_double_param("size_x",8.89);
      cryo2->set_double_param("size_y",60.96);
      cryo2->set_double_param("size_z",60.96);
      cryo2->set_double_param("place_x",150.72+8.89/2.+add_place_x);
      cryo2->set_double_param("place_z",place_z);
      cryo2->set_string_param("material","G4_Al");
      cryo2->SetActive(); // it is an active volume - save G4Hits
      cryo2->SuperDetector("CRYO");
      g4Reco->registerSubsystem(cryo2);

      PHG4BlockSubsystem *cryo3 = new PHG4BlockSubsystem("cryo3",3);
      cryo3->set_double_param("size_x",2.54);
      cryo3->set_double_param("size_y",60.96);
      cryo3->set_double_param("size_z",60.96);
      cryo3->set_double_param("place_x",173.93+2.54/2.+add_place_x);
      cryo3->set_double_param("place_z",place_z);
      cryo3->set_string_param("material","G4_Al");
      cryo3->SetActive(); // it is an active volume - save G4Hits
      cryo3->SuperDetector("CRYO");
      g4Reco->registerSubsystem(cryo3);
    }
  if (bh_on)
    { 
      // BLACKHOLE, box surrounding the prototype to check for leakage
      PHG4BlockSubsystem *bh[5];
      // surrounding outer hcal
      // top
      bh[0] = new PHG4BlockSubsystem("bh1",1);
      bh[0]->set_double_param("size_x",270.);
      bh[0]->set_double_param("size_y",0.01);
      bh[0]->set_double_param("size_z",165.);
      bh[0]->set_double_param("place_x",270./2.);
      bh[0]->set_double_param("place_y",125./2.);
      // bottom
      bh[1] = new PHG4BlockSubsystem("bh2",2);
      bh[1]->set_double_param("size_x",270.);
      bh[1]->set_double_param("size_y",0.01);
      bh[1]->set_double_param("size_z",165.);
      bh[1]->set_double_param("place_x",270./2.);
      bh[1]->set_double_param("place_y",-125./2.);
      // right side
      bh[2] = new PHG4BlockSubsystem("bh3",3);
      bh[2]->set_double_param("size_x",200.);
      bh[2]->set_double_param("size_y",125.);
      bh[2]->set_double_param("size_z",0.01);
      bh[2]->set_double_param("place_x",200./2.);
      bh[2]->set_double_param("place_z",165./2.);
      // left side
      bh[3] = new PHG4BlockSubsystem("bh4",4);
      bh[3]->set_double_param("size_x",270.);
      bh[3]->set_double_param("size_y",125.);
      bh[3]->set_double_param("size_z",0.01);
      bh[3]->set_double_param("place_x",270./2.);
      bh[3]->set_double_param("place_z",-165./2.);
      // back
      bh[4] = new PHG4BlockSubsystem("bh5",5);
      bh[4]->set_double_param("size_x",0.01);
      bh[4]->set_double_param("size_y",125.);
      bh[4]->set_double_param("size_z",165.);
      bh[4]->set_double_param("place_x",270.);
      for (int i=0; i<5; i++)
	{
	  bh[i]->BlackHole();
	  bh[i]->SetActive();
	  bh[i]->SuperDetector("BlackHole");
	  bh[i]->OverlapCheck(true);
	  g4Reco->registerSubsystem(bh[i]);
	}
    }
  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem( g4Reco );
  //----------------------------------------
  // EMCal digitization
  //----------------------------------------
  if (cemc_cell)
    {
      PHG4FullProjSpacalCellReco *cemc_cells = new PHG4FullProjSpacalCellReco("CEMCCYLCELLRECO");
      cemc_cells->Detector("CEMC");
      cemc_cells->set_timing_window(0.,60.);
      cemc_cells->get_light_collection_model().load_data_file(string(getenv("CALIBRATIONROOT")) + string("/CEMC/LightCollection/Prototype2Module.xml"),"data_grid_light_guide_efficiency","data_grid_fiber_trans");

      se->registerSubsystem(cemc_cells);
    }
  if (cemc_twr)
    {
      RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
      TowerBuilder->Detector("CEMC");
      TowerBuilder->set_sim_tower_node_prefix("SIM");
      se->registerSubsystem(TowerBuilder);
    }
  const double sampling_fraction = 0.0190134; //  +/-   0.000224984  from 0 Degree indenting 32 GeV electron showers
  const double photoelectron_per_GeV = 500; //500 photon per total GeV deposition
  const double ADC_per_photoelectron_HG = 3.8; // From Sean Stoll, Mar 29
  const double ADC_per_photoelectron_LG = 0.24; // From Sean Stoll, Mar 29

  // low gains
  if (cemc_digi)
    {
      RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer("EmcRawTowerDigitizerLG");
      TowerDigitizer->Detector("CEMC");
      TowerDigitizer->set_raw_tower_node_prefix("RAW_LG");
      TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer->set_pedstal_central_ADC(0);
      TowerDigitizer->set_pedstal_width_ADC(1); // From Jin's guess. No EMCal High Gain data yet! TODO: update
      TowerDigitizer->set_photonelec_ADC(1. / ADC_per_photoelectron_LG);
      TowerDigitizer->set_photonelec_yield_visible_GeV(photoelectron_per_GeV / sampling_fraction);
      TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
      se->registerSubsystem(TowerDigitizer);
      // high gains
      TowerDigitizer = new RawTowerDigitizer("EmcRawTowerDigitizerHG");
      TowerDigitizer->Detector("CEMC");
      TowerDigitizer->set_raw_tower_node_prefix("RAW_HG");
      TowerDigitizer->set_digi_algorithm(
					 RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer->set_pedstal_central_ADC(0);
      TowerDigitizer->set_pedstal_width_ADC(15); // From John Haggerty, Mar 29
      TowerDigitizer->set_photonelec_ADC(1. / ADC_per_photoelectron_HG);
      TowerDigitizer->set_photonelec_yield_visible_GeV(photoelectron_per_GeV / sampling_fraction);
      TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
      se->registerSubsystem(TowerDigitizer);
    }
  if (cemc_twrcal)
    {
      RawTowerCalibration *TowerCalibration = new RawTowerCalibration("EmcRawTowerCalibrationLG");
      TowerCalibration->Detector("CEMC");
      TowerCalibration->set_raw_tower_node_prefix("RAW_LG");
      TowerCalibration->set_calib_tower_node_prefix("CALIB_LG");
      TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration->set_calib_const_GeV_ADC(1. / ADC_per_photoelectron_LG / photoelectron_per_GeV);
      TowerCalibration->set_pedstal_ADC(0);
      TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
      se->registerSubsystem(TowerCalibration);


      TowerCalibration = new RawTowerCalibration("EmcRawTowerCalibrationHG");
      TowerCalibration->Detector("CEMC");
      TowerCalibration->set_raw_tower_node_prefix("RAW_HG");
      TowerCalibration->set_calib_tower_node_prefix("CALIB_HG");
      TowerCalibration->set_calib_algorithm(
					    RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration->set_calib_const_GeV_ADC(1. / ADC_per_photoelectron_HG / photoelectron_per_GeV);
      TowerCalibration->set_pedstal_ADC(0);
      TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
      se->registerSubsystem(TowerCalibration);
    }

  //----------------------------------------
  // HCal towering
  //----------------------------------------
  if (ihcal_cell)
    {
      PHG4Prototype2HcalCellReco *hccell = new PHG4Prototype2HcalCellReco("HCALinCellReco");
      hccell->Detector("HCALIN");
      se->registerSubsystem(hccell);
    }
  if (ihcal_twr)
    {
      Prototype2RawTowerBuilder *hcaltwr = new Prototype2RawTowerBuilder("HCALinRawTowerBuilder");
      hcaltwr->Detector("HCALIN");
      hcaltwr->set_sim_tower_node_prefix("SIM");
      se->registerSubsystem(hcaltwr);
    }


  if (ohcal_cell)
    {
      hccell = new PHG4Prototype2HcalCellReco("HCALoutCellReco");
      hccell->Detector("HCALOUT");
      se->registerSubsystem(hccell);
    }
  if (ohcal_twr)
    {
      hcaltwr = new Prototype2RawTowerBuilder("HCALoutRawTowerBuilder");
      hcaltwr->Detector("HCALOUT");
      hcaltwr->set_sim_tower_node_prefix("SIM");
      se->registerSubsystem(hcaltwr);
    }

  //----------------------------------------
  // HCal digitization
  //----------------------------------------
  //  From: Abhisek Sen [mailto:sen.abhisek@gmail.com]
  //  Sent: Tuesday, April 19, 2016 10:55 PM
  //  To: Huang, Jin <jhuang@bnl.gov>; Haggerty, John <haggerty@bnl.gov>

  //  HCALIN:
  //     1/5 pixel / HG ADC channel
  //     32/5 pixel / LG ADC channel
  //     0.4 MeV/ LG ADC
  //     0.4/32 MeV/ HG ADC

  //  HCALOUT:
  //     1/5 pixel / HG ADC channel
  //     16/5 pixel / LG ADC channel
  //     0.2 MeV/ LG ADC
  //     0.2/16 MeV/ HG ADC
  RawTowerDigitizer *TowerDigitizer = NULL;
  if (ihcal_digi)
    {
      TowerDigitizer = new RawTowerDigitizer("HCALinTowerDigitizerLG");
      TowerDigitizer->Detector("HCALIN");
      TowerDigitizer->set_raw_tower_node_prefix("RAW_LG");
      TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer->set_pedstal_central_ADC(0);
      TowerDigitizer->set_pedstal_width_ADC(1); // From Jin's guess. No EMCal High Gain data yet! TODO: update
      TowerDigitizer->set_photonelec_ADC(32. / 5.);
      TowerDigitizer->set_photonelec_yield_visible_GeV(32. / 5 / (0.4e-3));
      TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
      se->registerSubsystem(TowerDigitizer);

      TowerDigitizer = new RawTowerDigitizer("HCALinTowerDigitizerHG");
      TowerDigitizer->Detector("HCALIN");
      TowerDigitizer->set_raw_tower_node_prefix("RAW_HG");
      TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer->set_pedstal_central_ADC(0);
      TowerDigitizer->set_pedstal_width_ADC(1); // From Jin's guess. No EMCal High Gain data yet! TODO: update
      TowerDigitizer->set_photonelec_ADC(1. / 5.);
      TowerDigitizer->set_photonelec_yield_visible_GeV(1. / 5 / (0.4e-3 / 32));
      TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
      se->registerSubsystem(TowerDigitizer);
    }
  if (ohcal_digi)
    {
      TowerDigitizer = new RawTowerDigitizer("HCALoutTowerDigitizerLG");
      TowerDigitizer->Detector("HCALOUT");
      TowerDigitizer->set_raw_tower_node_prefix("RAW_LG");
      TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer->set_pedstal_central_ADC(0);
      TowerDigitizer->set_pedstal_width_ADC(1); // From Jin's guess. No EMCal High Gain data yet! TODO: update
      TowerDigitizer->set_photonelec_ADC(16. / 5.);
      TowerDigitizer->set_photonelec_yield_visible_GeV(16. / 5 / (0.2e-3));
      TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
      se->registerSubsystem(TowerDigitizer);

      TowerDigitizer = new RawTowerDigitizer("HCALoutTowerDigitizerHG");
      TowerDigitizer->Detector("HCALOUT");
      TowerDigitizer->set_raw_tower_node_prefix("RAW_HG");
      TowerDigitizer->set_digi_algorithm(RawTowerDigitizer::kSimple_photon_digitalization);
      TowerDigitizer->set_pedstal_central_ADC(0);
      TowerDigitizer->set_pedstal_width_ADC(1); // From Jin's guess. No EMCal High Gain data yet! TODO: update
      TowerDigitizer->set_photonelec_ADC(1. / 5.);
      TowerDigitizer->set_photonelec_yield_visible_GeV(1. / 5 / (0.2e-3 / 16));
      TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
      se->registerSubsystem(TowerDigitizer);
    }
  //----------------------------------------
  // HCal calibration
  //----------------------------------------
  // 32 GeV Pi+ scan
  const double visible_sample_fraction_HCALIN = 7.19505e-02 ; // 1.34152e-02
  const double visible_sample_fraction_HCALOUT = 0.0313466 ; //  +/-   0.0067744
  RawTowerCalibration *TowerCalibration = NULL;
  if (ihcal_twrcal)
    {
      TowerCalibration = new RawTowerCalibration("HCALinRawTowerCalibrationLG");
      TowerCalibration->Detector("HCALIN");
      TowerCalibration->set_raw_tower_node_prefix("RAW_LG");
      TowerCalibration->set_calib_tower_node_prefix("CALIB_LG");
      TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration->set_calib_const_GeV_ADC(0.4e-3 / visible_sample_fraction_HCALIN);
      TowerCalibration->set_pedstal_ADC(0);
      TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
      se->registerSubsystem(TowerCalibration);

      TowerCalibration = new RawTowerCalibration("HCALinRawTowerCalibrationHG");
      TowerCalibration->Detector("HCALIN");
      TowerCalibration->set_raw_tower_node_prefix("RAW_HG");
      TowerCalibration->set_calib_tower_node_prefix("CALIB_HG");
      TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration->set_calib_const_GeV_ADC(0.4e-3 / 32 / visible_sample_fraction_HCALIN);
      TowerCalibration->set_pedstal_ADC(0);
      TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
      se->registerSubsystem(TowerCalibration);
    }
  if (ohcal_twrcal)
    {
      TowerCalibration = new RawTowerCalibration("HCALoutRawTowerCalibrationLG");
      TowerCalibration->Detector("HCALOUT");
      TowerCalibration->set_raw_tower_node_prefix("RAW_LG");
      TowerCalibration->set_calib_tower_node_prefix("CALIB_LG");
      TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration->set_calib_const_GeV_ADC(0.2e-3 / visible_sample_fraction_HCALOUT);
      TowerCalibration->set_pedstal_ADC(0);
      TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
      se->registerSubsystem(TowerCalibration);

      TowerCalibration = new RawTowerCalibration("HCALoutRawTowerCalibrationHG");
      TowerCalibration->Detector("HCALOUT");
      TowerCalibration->set_raw_tower_node_prefix("RAW_HG");
      TowerCalibration->set_calib_tower_node_prefix("CALIB_HG");
      TowerCalibration->set_calib_algorithm(RawTowerCalibration::kSimple_linear_calibration);
      TowerCalibration->set_calib_const_GeV_ADC(0.2e-3 / 16 / visible_sample_fraction_HCALOUT);
      TowerCalibration->set_pedstal_ADC(0);
      TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
      se->registerSubsystem(TowerCalibration);
    }
  //----------------------
  // QA Histograms
  //----------------------
  if (cemc_on)
    {
      se->registerSubsystem(new QAG4SimulationCalorimeter("CEMC",QAG4SimulationCalorimeter::kProcessG4Hit));
    }
  if (ihcal_on)
    {
      // TODO: disable QA for HCal right now as there is a hit->particle truth association error at the moment
      // se->registerSubsystem(new QAG4SimulationCalorimeter("HCALIN",QAG4SimulationCalorimeter::kProcessG4Hit));
    }
  if (ohcal_on)
    {
      // se->registerSubsystem(new QAG4SimulationCalorimeter("HCALOUT",QAG4SimulationCalorimeter::kProcessG4Hit));
    }
  //----------------------
  // G4HitNtuple
  //----------------------
  if (hit_ntuple)
    {
      G4HitNtuple *hit = new G4HitNtuple("G4HitNtuple","g4hitntuple.root");
      hit->AddNode("HCALIN", 0);
      hit->AddNode("HCALOUT", 1);
      hit->AddNode("CRYO", 2);
      hit->AddNode("BlackHole", 3);
      hit->AddNode("ABSORBER_HCALIN", 10);
      hit->AddNode("ABSORBER_HCALOUT", 11);
      se->registerSubsystem(hit);
    }
  // G4ScintillatorSlatTTree *scintcell = new G4ScintillatorSlatTTree("inslat");
  // scintcell->Detector("HCALIN");
  // se->registerSubsystem(scintcell);

  // scintcell = new G4ScintillatorSlatTTree("outslat");
  // scintcell->Detector("HCALOUT");
  // se->registerSubsystem(scintcell);


  //----------------------
  // save a comprehensive  evaluation file
  //----------------------
  if (dstreader)
    {
      PHG4DSTReader* ana = new PHG4DSTReader(string("DSTReader.root"));
      ana->set_save_particle(true);
      ana->set_load_all_particle(false);
      ana->set_load_active_particle(false);
      ana->set_save_vertex(true);
      ana->set_tower_zero_sup(-1000); // no zero suppression

      //  ana->AddNode("CEMC");
      //  if (absorberactive)
      //    {
      //      ana->AddNode("ABSORBER_CEMC");
      //    }

      if (cemc_twr)
        ana->AddTower("SIM_CEMC");
      if (cemc_digi)
        ana->AddTower("RAW_LG_CEMC");
      if (cemc_twrcal)
        ana->AddTower("CALIB_LG_CEMC"); // Low gain CEMC
      if (cemc_digi)
        ana->AddTower("RAW_HG_CEMC");
      if (cemc_twrcal)
        ana->AddTower("CALIB_HG_CEMC"); // High gain CEMC

      if (ohcal_twr)
        ana->AddTower("SIM_HCALOUT");
      if (ihcal_twr)
        ana->AddTower("SIM_HCALIN");

      if (ihcal_digi)
        ana->AddTower("RAW_LG_HCALIN");
      if (ihcal_digi)
        ana->AddTower("RAW_HG_HCALIN");
      if (ohcal_digi)
        ana->AddTower("RAW_LG_HCALOUT");
      if (ohcal_digi)
        ana->AddTower("RAW_HG_HCALOUT");

      if (ihcal_twrcal)
        ana->AddTower("CALIB_LG_HCALIN");
      if (ihcal_twrcal)
        ana->AddTower("CALIB_HG_HCALIN");
      if (ohcal_twrcal)
        ana->AddTower("CALIB_LG_HCALOUT");
      if (ohcal_twrcal)
        ana->AddTower("CALIB_HG_HCALOUT");

      if (bh_on)
        ana->AddNode("BlackHole"); // add a G4Hit node

      se->registerSubsystem(ana);
    }

  // Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT","/phenix/scratch/pinkenbu/G4Prototype2Hcalin.root");
  // out->AddNode("G4RootScintillatorSlat_HCALIN");
  // se->registerOutputManager(out);

  // out = new Fun4AllDstOutputManager("DSTHCOUT","/phenix/scratch/pinkenbu/G4Prototype2Hcalout.root");
  // out->AddNode("G4RootScintillatorSlat_HCALOUT");
  // se->registerOutputManager(out);

  if (dstoutput)
    {
      Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT","G4Prototype3New.root");
      se->registerOutputManager(out);
    }

  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
  se->registerInputManager( in );
  if (nEvents <= 0)
    {
      return 0;
    }
  se->run(nEvents);

  se->End();

  QAHistManagerDef::saveQARootFile("G4Prototype2_qa.root");


  //   std::cout << "All done" << std::endl;
  delete se;
  //   return 0;
  gSystem->Exit(0);

}
