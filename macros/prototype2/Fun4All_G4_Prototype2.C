int Fun4All_G4_Prototype2(
			  int nEvents = 1
			  )
{

  gSystem->Load("libfun4all");
  gSystem->Load("libg4detectors");
  gSystem->Load("libg4testbench");
  gSystem->Load("libg4histos");
  gSystem->Load("libg4eval.so");

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("RANDOMSEED",12345);

  // Test beam generator
  PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
  gen->add_particles("pi-", 1); // mu-,e-,anti_proton,pi-
  gen->set_vertex_distribution_mean(0.0, 0.0, 0);
  gen->set_vertex_distribution_width(0.0, .3/2, .3/2); // 3x3 mm beam spot as measured in Si-telescope
  gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Gaus,
      PHG4SimpleEventGenerator::Gaus, PHG4SimpleEventGenerator::Gaus); // Gauss beam profile
  gen->set_eta_range(-.001, .001); // 1mrad angular divergence
  gen->set_phi_range(-.001, .001); // 1mrad angular divergence
  const double momentum = 32;
  gen->set_p_range(momentum,momentum, momentum*2e-2); // 2% momentum smearing
  se->registerSubsystem(gen);

  // Simple single particle generator
  PHG4ParticleGun *gun = new PHG4ParticleGun();
  //  gun->set_name("anti_proton");
  //  gun->set_name("geantino");
    gun->set_name("proton");
  gun->set_vtx(0, 0, 0);
  gun->set_mom(120, 0, 0);
  // gun->AddParticle("geantino",1.7776,-0.4335,0.);
  // gun->AddParticle("geantino",1.7709,-0.4598,0.);
  // gun->AddParticle("geantino",2.5621,0.60964,0.);
  // gun->AddParticle("geantino",1.8121,0.253,0.);
//  se->registerSubsystem(gun);

  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_field(0);


  //----------------------------------------
  // EMCal G4
  //----------------------------------------
  PHG4SpacalPrototypeSubsystem *cemc;
  cemc = new PHG4SpacalPrototypeSubsystem("CEMC");
  cemc->SetActive();
  cemc->SuperDetector("CEMC");
  cemc->SetAbsorberActive();
  cemc->OverlapCheck(true);
  cemc->GetParameters().ReadFromFile("xml", string(getenv("OFFLINE_MAIN")) + string("/share/calibrations/Prototype2/Geometry/") ); // geometry database
//  cemc->GetParameters().set_double_param("z_rotation_degree", 15); // rotation around CG
  cemc->GetParameters().set_double_param("xpos", 105); // location in cm of EMCal CG. Update with final positioning of EMCal
//  cemc->GetParameters().set_double_param("ypos", 0); // vertical shift
//  cemc->GetParameters().set_double_param("zpos", 0); // horizontal shift
  g4Reco->registerSubsystem(cemc);

  //----------------------------------------
  // HCal G4
  //----------------------------------------

  PHG4Prototype2InnerHcalSubsystem *innerhcal = new PHG4Prototype2InnerHcalSubsystem("HCalIn");
  innerhcal->SetActive();
  innerhcal->SetAbsorberActive();
  innerhcal->OverlapCheck(true);
  innerhcal->SuperDetector("INNERHCAL");
  g4Reco->registerSubsystem(innerhcal);
  PHG4Prototype2OuterHcalSubsystem *outerhcal = new PHG4Prototype2OuterHcalSubsystem("HCalOut");
  outerhcal->SetActive();
  outerhcal->SetAbsorberActive();
  outerhcal->OverlapCheck(true);
  outerhcal->SuperDetector("OUTERHCAL");
  g4Reco->registerSubsystem(outerhcal);

  // Cryostat from engineering drawing
  PHG4BlockSubsystem *cryo1 = new PHG4BlockSubsystem("cryo1",1);
  cryo1->SetSize(0.95,60.96,60.96);
  cryo1->SetCenter(141.96+0.95/2.,0,0); // shift cryo1 so we do not create particles in its center
  cryo1->SetMaterial("G4_Al");
  cryo1->SetActive(); // it is an active volume - save G4Hits
  cryo1->SuperDetector("CRYO");
  g4Reco->registerSubsystem(cryo1);

  PHG4BlockSubsystem *cryo2 = new PHG4BlockSubsystem("cryo2",2);
  cryo2->SetSize(8.89,60.96,60.96);
  cryo2->SetCenter(150.72+8.89/2.,0,0); // shift cryo2 so we do not create particles in its center
  cryo2->SetMaterial("G4_Al");
  cryo2->SetActive(); // it is an active volume - save G4Hits
  cryo2->SuperDetector("CRYO");
  g4Reco->registerSubsystem(cryo2);

  PHG4BlockSubsystem *cryo3 = new PHG4BlockSubsystem("cryo3",3);
  cryo3->SetSize(2.54,60.96,60.96);
  cryo3->SetCenter(173.93+2.54/2.,0,0); // shift cryo3 so we do not create particles in its center
  cryo3->SetMaterial("G4_Al");
  cryo3->SetActive(); // it is an active volume - save G4Hits
  cryo3->SuperDetector("CRYO");
  g4Reco->registerSubsystem(cryo3);
  // BLACKHOLE, box surrounding the prototype to check for leakage
  PHG4BlockSubsystem *bh[5];
  // surrounding outer hcal
  // top
  bh[0] = new PHG4BlockSubsystem("bh1",1);
  bh[0]->SetSize(270.,0.01,165.);
  bh[0]->SetCenter(270./2.,125./2.,0);
  // bottom
  bh[1] = new PHG4BlockSubsystem("bh2",2);
  bh[1]->SetSize(270.,0.01,165.);
  bh[1]->SetCenter(270./2.,-125./2.,0);
  // right side
  bh[2] = new PHG4BlockSubsystem("bh3",3);
  bh[2]->SetSize(270.,125,0.01);
  bh[2]->SetCenter(270./2.,0.,165./2.);
  // left side
  bh[3] = new PHG4BlockSubsystem("bh4",4);
  bh[3]->SetSize(270.,125,0.01);
  bh[3]->SetCenter(270./2.,0.,-165./2.);
  // back
  bh[4] = new PHG4BlockSubsystem("bh5",5);
  bh[4]->SetSize(0.01,125,165);
  bh[4]->SetCenter(270.,0.,0.);
  for (int i=0; i<5; i++)
    {
      bh[i]->BlackHole();
      bh[i]->SetActive();
      bh[i]->SuperDetector("BlackHole");
      bh[i]->OverlapCheck(true);
      g4Reco->registerSubsystem(bh[i]);
    }

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  se->registerSubsystem( g4Reco );

  //----------------------------------------
  // EMCal digitization
  //----------------------------------------

  {
    PHG4FullProjSpacalCellReco *cemc_cells = new PHG4FullProjSpacalCellReco(
        "CEMCCYLCELLRECO");
    cemc_cells->Detector("CEMC");
    cemc_cells->set_timing_window_size(60);
    se->registerSubsystem(cemc_cells);

    RawTowerBuilder *TowerBuilder = new RawTowerBuilder("EmcRawTowerBuilder");
    TowerBuilder->Detector("CEMC");
    TowerBuilder->set_sim_tower_node_prefix("SIM");
    se->registerSubsystem(TowerBuilder);

    const double sampling_fraction = 0.0233369; //  +/-   8.22211e-05  from 15 Degree indenting 8 GeV electron showers
    const double photoelectron_per_GeV = 500; //500 photon per total GeV deposition
    const double ADC_per_photoelectron_HG = 3.8; // From Sean Stoll, Mar 29
    const double ADC_per_photoelectron_LG = 0.24; // From Sean Stoll, Mar 29

    // low gains
    RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer(
        "EmcRawTowerDigitizer");
    TowerDigitizer->Detector("CEMC");
    TowerDigitizer->set_raw_tower_node_prefix("RAW_LG");
    TowerDigitizer->set_digi_algorithm(
        RawTowerDigitizer::kSimple_photon_digitalization);
    TowerDigitizer->set_pedstal_central_ADC(0);
    TowerDigitizer->set_pedstal_width_ADC(1); // From Jin's guess. No EMCal High Gain data yet! TODO: update
    TowerDigitizer->set_photonelec_ADC(1. / ADC_per_photoelectron_LG);
    TowerDigitizer->set_photonelec_yield_visible_GeV(
        photoelectron_per_GeV / sampling_fraction);
    TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
    se->registerSubsystem(TowerDigitizer);

    RawTowerCalibration *TowerCalibration = new RawTowerCalibration(
        "EmcRawTowerCalibration");
    TowerCalibration->Detector("CEMC");
    TowerCalibration->set_raw_tower_node_prefix("RAW_LG");
    TowerCalibration->set_calib_tower_node_prefix("CALIB_LG");
    TowerCalibration->set_calib_algorithm(
        RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration->set_calib_const_GeV_ADC(
        1. / ADC_per_photoelectron_LG / photoelectron_per_GeV);
    TowerCalibration->set_pedstal_ADC(0);
    TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
    se->registerSubsystem(TowerCalibration);

    // high gains
    RawTowerDigitizer *TowerDigitizer = new RawTowerDigitizer(
        "EmcRawTowerDigitizer");
    TowerDigitizer->Detector("CEMC");
    TowerDigitizer->set_raw_tower_node_prefix("RAW_HG");
    TowerDigitizer->set_digi_algorithm(
        RawTowerDigitizer::kSimple_photon_digitalization);
    TowerDigitizer->set_pedstal_central_ADC(0);
    TowerDigitizer->set_pedstal_width_ADC(15); // From John Haggerty, Mar 29
    TowerDigitizer->set_photonelec_ADC(1. / ADC_per_photoelectron_HG);
    TowerDigitizer->set_photonelec_yield_visible_GeV(
        photoelectron_per_GeV / sampling_fraction);
    TowerDigitizer->set_zero_suppression_ADC(-1000); // no-zero suppression
    se->registerSubsystem(TowerDigitizer);

    RawTowerCalibration *TowerCalibration = new RawTowerCalibration(
        "EmcRawTowerCalibration");
    TowerCalibration->Detector("CEMC");
    TowerCalibration->set_raw_tower_node_prefix("RAW_HG");
    TowerCalibration->set_calib_tower_node_prefix("CALIB_HG");
    TowerCalibration->set_calib_algorithm(
        RawTowerCalibration::kSimple_linear_calibration);
    TowerCalibration->set_calib_const_GeV_ADC(
        1. / ADC_per_photoelectron_HG / photoelectron_per_GeV);
    TowerCalibration->set_pedstal_ADC(0);
    TowerCalibration->set_zero_suppression_GeV(-1); // no-zero suppression
    se->registerSubsystem(TowerCalibration);
  }

  //----------------------------------------
  // HCal digitization
  //----------------------------------------
  PHG4Prototype2HcalCellReco *hccell = new PHG4Prototype2HcalCellReco();
  hccell->Detector("INNERHCAL");
  se->registerSubsystem(hccell);

  hccell = new PHG4Prototype2HcalCellReco();
  hccell->Detector("OUTERHCAL");
  se->registerSubsystem(hccell);

  Prototype2RawTowerBuilder *hcaltwr = new Prototype2RawTowerBuilder();
  hcaltwr->Detector("INNERHCAL");
  se->registerSubsystem(hcaltwr);

  hcaltwr = new Prototype2RawTowerBuilder();
  hcaltwr->Detector("OUTERHCAL");
//  hcaltwr->Verbosity(2);
  se->registerSubsystem(hcaltwr);

  //----------------------
  // G4HitNtuple
  //----------------------
  G4HitNtuple *hit = new G4HitNtuple("G4HitNtuple","g4hitntuple.root");
  hit->AddNode("INNERHCAL", 0);
  hit->AddNode("OUTERHCAL", 1);
  hit->AddNode("CRYO", 2);
  hit->AddNode("BlackHole", 3);
  hit->AddNode("ABSORBER_INNERHCAL", 10);
  hit->AddNode("ABSORBER_OUTERHCAL", 11);
  se->registerSubsystem(hit);

  //----------------------
  // save a comprehensive  evaluation file
  //----------------------
  PHG4DSTReader* ana = new PHG4DSTReader(
      string("DSTReader.root"));
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
  ana->AddTower("SIM_CEMC");
  ana->AddTower("RAW_LG_CEMC");
  ana->AddTower("CALIB_LG_CEMC");// Low gain CEMC
  ana->AddTower("RAW_HG_CEMC");
  ana->AddTower("CALIB_HG_CEMC");// High gain CEMC
  ana->AddTower("OUTERHCAL");
  ana->AddTower("INNERHCAL");

  ana->AddNode("BlackHole");// add a G4Hit node

  se->registerSubsystem(ana);

  // Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT","G4Prototype2.root");
  // se->registerOutputManager(out);

  Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
  se->registerInputManager( in );
  if (nEvents <= 0)
    {
      return 0;
    }
  se->run(nEvents);

  se->End();
  //   std::cout << "All done" << std::endl;
    delete se;
//   return 0;
    gSystem->Exit(0);

}

