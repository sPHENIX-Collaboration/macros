int Fun4All_G4_Prototype2(
			  int nEvents = -1
			  )
{

  gSystem->Load("libfun4all");
  gSystem->Load("libg4detectors");
  gSystem->Load("libg4testbench");
  gSystem->Load("libg4histos");

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("RANDOMSEED",12345);
  PHG4ParticleGenerator *gen = new PHG4ParticleGenerator();
  gen->set_name("geantino");
  //  gen->set_name("proton");
  gen->set_vtx(0, 0, 0);
  //  gen->set_eta_range(0.297, 0.303);
  gen->set_eta_range(-0.2, 0.2);
  gen->set_mom_range(1.0, 1.0);
  gen->set_z_range(0.,0.);
  gen->set_phi_range(18/180.*TMath::Pi(), -16/180.*TMath::Pi());
  //    gen->set_phi_range(-17./180.*TMath::Pi(),-7./180.*TMath::Pi());
  //  se->registerSubsystem(gen);

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
  se->registerSubsystem(gun);
  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_field(0);
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
  hcaltwr->Verbosity(2);
  se->registerSubsystem(hcaltwr);

  G4HitNtuple *hit = new G4HitNtuple("G4HitNtuple","/phenix/scratch/pinkenbu/g4hitntuple.root");
  hit->AddNode("INNERHCAL", 0);
  hit->AddNode("OUTERHCAL", 1);
  hit->AddNode("CRYO", 2);
  hit->AddNode("BlackHole", 3);
  hit->AddNode("ABSORBER_INNERHCAL", 10);
  hit->AddNode("ABSORBER_OUTERHCAL", 11);
  se->registerSubsystem(hit);

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

