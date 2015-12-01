
double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes
bool overlapcheck = false; // set to true if you want to check for overlaps

void G4Init(bool do_svtx = true,
	    bool do_preshower = false,
	    bool do_cemc = true,
	    bool do_hcalin = true,
	    bool do_magnet = true,
	    bool do_hcalout = true,
	    bool do_pipe = true,
	    bool do_bbc = true,
	    bool do_global = true) {

  gROOT->LoadMacro("G4_Bbc.C");
  if (do_bbc) BbcInit();
  
  gROOT->LoadMacro("G4_Pipe.C");
  if (do_pipe) PipeInit();
  
  // load detector macros and execute Init() function
  gROOT->LoadMacro("G4_Svtx.C");                 // default MIE projections
  //gROOT->LoadMacro("G4_Svtx_pixels+strips.C"); // testing
  //gROOT->LoadMacro("G4_Svtx_pixels+tpc.C");    // testing
  //gROOT->LoadMacro("G4_Svtx_maps+strips.C");   // testing
  //gROOT->LoadMacro("G4_Svtx_maps+tpc.C");      // testing
  //gROOT->LoadMacro("G4_Svtx_ladders.C");       // testing
  //gROOT->LoadMacro("G4_Svtx_ITS.C");           // testing
  if (do_svtx) SvtxInit();

  if (do_preshower) {
    gROOT->LoadMacro("G4_PreShower.C");
    PreShowerInit();
  }

  gROOT->LoadMacro("G4_CEmc_Spacal.C");
  if (do_cemc) CEmcInit(72); // make it 2*2*2*3*3 so we can try other combinations

  gROOT->LoadMacro("G4_HcalIn_ref.C");
  if (do_hcalin) HCalInnerInit();

  gROOT->LoadMacro("G4_Magnet.C");
  if (do_magnet) MagnetInit();

  gROOT->LoadMacro("G4_HcalOut_ref.C");
  if (do_hcalout) HCalOuterInit();

  gROOT->LoadMacro("G4_Global.C");
  
  gROOT->LoadMacro("G4_Jets.C");
}


int G4Setup(const int absorberactive = 0,
	    const string &field ="1.5",
	    const EDecayType decayType = TPythia6Decayer::kAll,
	    const bool do_svtx = true,
	    const bool do_preshower = false,
	    const bool do_cemc = true,
	    const bool do_hcalin = true,
	    const bool do_magnet = true,
	    const bool do_hcalout = true,
	    const bool do_pipe = true,
	    const bool do_bbc = true,
	    const float magfield_rescale = 1.0) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->set_rapidity_coverage(1.1); // according to drawings
  if (decayType != TPythia6Decayer::kAll) {
    g4Reco->set_force_decay(decayType);
  }
  
  double fieldstrength;
  istringstream stringline(field);
  stringline >> fieldstrength;
  if (stringline.fail()) { // conversion to double fails -> we have a string

    if (field.find("sPHENIX.root") != string::npos) {
      g4Reco->set_field_map(field, 1);
    } else {
      g4Reco->set_field_map(field, 2);
    }
  } else {
    g4Reco->set_field(fieldstrength); // use const soleniodal field
  }
  g4Reco->set_field_rescale(magfield_rescale);
  
  double radius = 0.;

  //----------------------------------------
  // PIPE
  if (do_pipe) radius = Pipe(g4Reco, radius, absorberactive);
  
  //----------------------------------------
  // SVTX
  if (do_svtx) radius = Svtx(g4Reco, radius, absorberactive);

  //----------------------------------------
  // PRESHOWER
  
  if (do_preshower) radius = PreShower(g4Reco, radius, absorberactive);

  //----------------------------------------
  // CEMC
//
  if (do_cemc) radius = CEmc(g4Reco, radius, 8, absorberactive);
//  if (do_cemc) radius = CEmc_Vis(g4Reco, radius, 8, absorberactive);// for visualization substructure of SPACAL, slow to render
  
  //----------------------------------------
  // HCALIN
  
  if (do_hcalin) radius = HCalInner(g4Reco, radius, 4, absorberactive);

  //----------------------------------------
  // MAGNET
  
  if (do_magnet) radius = Magnet(g4Reco, radius, 0, absorberactive);

  //----------------------------------------
  // HCALOUT
  
  if (do_hcalout) radius = HCalOuter(g4Reco, radius, 4, absorberactive);

  //----------------------------------------
  // BLACKHOLE
  
  // swallow all particles coming out of the backend of sPHENIX
  PHG4CylinderSubsystem *blackhole = new PHG4CylinderSubsystem("BH", 1);
  blackhole->SetRadius(radius + 10); // add 10 cm
  blackhole->SetLengthViaRapidityCoverage(false);
  blackhole->SetLength(g4Reco->GetWorldSizeZ() - no_overlapp); // make it cover the world in length
  blackhole->BlackHole();
  blackhole->SetThickness(0.1); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  //----------------------------------------
  // FORWARD BLACKHOLEs
  // +Z
  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_PLUS", 1);
  blackhole->SuperDetector("BH_FORWARD_PLUS");
  blackhole->SetRadius(0); // add 10 cm
  blackhole->SetLengthViaRapidityCoverage(false);
  blackhole->SetLength(0.1); // make it cover the world in length
  blackhole->SetPosition(0,0, g4Reco->GetWorldSizeZ()/2. - 0.1  - no_overlapp);
  blackhole->BlackHole();
  blackhole->SetThickness(radius - no_overlapp); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_NEG", 1);
  blackhole->SuperDetector("BH_FORWARD_NEG");
  blackhole->SetRadius(0); // add 10 cm
  blackhole->SetLengthViaRapidityCoverage(false);
  blackhole->SetLength(0.1); // make it cover the world in length
  blackhole->SetPosition(0,0, - g4Reco->GetWorldSizeZ()/2. +0.1  + no_overlapp);
  blackhole->BlackHole();
  blackhole->SetThickness(radius - no_overlapp); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);
  se->registerSubsystem( g4Reco );
}
