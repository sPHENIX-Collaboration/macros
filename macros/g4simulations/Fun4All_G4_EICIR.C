
int Fun4All_G4_EICIR(
		     const int nEvents = 10,
		     const char * inputFile = "/gpfs02/phenix/prod/sPHENIX/preCDR/pro.1-beta.5/single_particle/spacal1d/fieldmap/G4Hits_sPHENIX_e-_eta0_16GeV.root",
		     const char * outputFile = "G4EICIR.root"
		     )
{
  //===============
  // Input options
  //===============

  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  const bool readhits = false;
  // Or:
  // read files in HepMC format (typically output from event generators like hijing or pythia)
  const bool readhepmc = false; // read HepMC files
  // Or:
  // Use particle generator
  const bool runpythia8 = false;
  const bool runpythia6 = false;

  // USe particle gun
  const bool pgun = true;

  //======================
  // What to run
  //======================

  bool do_bbc = false;

  bool do_pipe = false;

  bool do_magnet = true;

  // Extended IR
  bool do_ExtendedIR = true;

  //================

  bool do_svtx = false;
  bool do_svtx_cell = false;
  bool do_svtx_track = false;
  bool do_svtx_eval = false;
  bool do_preshower = false;
  bool do_cemc = false;
  bool do_cemc_cell = false;
  bool do_cemc_twr = false;
  bool do_cemc_cluster = false;
  bool do_cemc_eval = false;
  bool do_hcalin = false;
  bool do_hcalin_cell = false;
  bool do_hcalin_twr = false;
  bool do_hcalin_cluster = false;
  bool do_hcalin_eval = false;
  bool do_hcalout = false;
  bool do_hcalout_cell = false;
  bool do_hcalout_twr = false;
  bool do_hcalout_cluster = false;
  bool do_hcalout_eval = false;
  bool do_global = false;
  bool do_global_fastsim = false;
  bool do_jet_reco = false;
  bool do_jet_eval = false;
  bool do_fwd_jet_reco = false;
  bool do_fwd_jet_eval = false;

  // EICDetector geometry - barrel

  bool do_DIRC = false;

  // EICDetector geometry - 'hadron' direction

  bool do_FGEM = false;

  bool do_RICH = false;
  bool do_Aerogel = false;

  bool do_FEMC = false;
  bool do_FEMC_cell = false;
  bool do_FEMC_twr = false;
  bool do_FEMC_cluster = false;

  bool do_FHCAL = false;
  bool do_FHCAL_cell = false;
  bool do_FHCAL_twr = false;
  bool do_FHCAL_cluster = false;


  // EICDetector geometry - 'hadron' direction

  bool do_EGEM = false;

  bool do_EEMC = false;
  bool do_EEMC_cell = false;
  bool do_EEMC_twr = false;
  bool do_EEMC_cluster = false;


  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libphhepmc.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4hough.so");
  gSystem->Load("libcemc.so");
  gSystem->Load("libg4eval.so");

  // establish the geometry and reconstruction setup
  gROOT->LoadMacro("G4Setup_EICIR.C");
  G4Init(do_svtx,do_preshower,do_cemc,do_hcalin,do_magnet,do_hcalout,do_pipe,do_FGEM,do_EGEM,do_FEMC,do_FHCAL,do_EEMC,do_DIRC,do_RICH,do_Aerogel,do_ExtendedIR);

  int absorberactive = 0; // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = "/phenix/upgrades/decadal/fieldmaps/sPHENIX.2d.root"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const float magfield_rescale = 1.4/1.5; // scale the map to a 1.4 T field

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
  //se->Verbosity(100); // uncomment for batch production running with minimal output messages
  se->Verbosity(Fun4AllServer::VERBOSITY_SOME); // uncomment for some info for interactive running
  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You can either set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  // rc->set_IntFlag("RANDOMSEED", 12345);

  //-----------------
  // Event generation
  //-----------------

  if (readhits)
    {
      // Get the hits from a file
      // The input manager is declared later
    }
  else if (readhepmc)
    {
      // this module is needed to read the HepMC records into our G4 sims
      // but only if you read HepMC input files
      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);
    }
  else if (runpythia8)
    {
      gSystem->Load("libPHPythia8.so");

      PHPythia8* pythia8 = new PHPythia8();
      // see coresoftware/generators/PHPythia8 for example config
      pythia8->set_config_file("phpythia8.cfg");
      se->registerSubsystem(pythia8);

      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);
    }
  else if (runpythia6)
    {
      gSystem->Load("libPHPythia6.so");

      PHPythia6 *pythia6 = new PHPythia6();
      pythia6->set_config_file("phpythia6.cfg");
      se->registerSubsystem(pythia6);

      HepMCNodeReader *hr = new HepMCNodeReader();
      se->registerSubsystem(hr);
    }
  else if (pgun)
    {
      /* angle of particle phi:
	 pz = p * cos(psi)
	 px = p * sin(psi) */
      double psi_mrad = 0;
      double ptot = 250*1;

      double px = ptot * sin(psi_mrad / 1000.);
      double py = 0;
      double pz = ptot * cos(psi_mrad / 1000.);

      PHG4ParticleGun*gun = new PHG4ParticleGun();
      //gun->set_name("gamma");
      gun->set_name("proton");
      gun->set_mom(px,py,pz);
      se->registerSubsystem(gun);
    }
  else
    {
      // toss low multiplicity dummy events
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      //gen->add_particles("e-",1); // mu+,e+,proton,pi+,Upsilon
      //gen->add_particles("e+",5); // mu-,e-,anti_proton,pi-
      gen->add_particles("proton",1); // mu-,e-,anti_proton,pi-
      if (readhepmc) {
        gen->set_reuse_existing_vertex(true);
        gen->set_existing_vertex_offset_vector(0.0,0.0,0.0);
      } else {
        gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
                                              PHG4SimpleEventGenerator::Uniform,
                                              PHG4SimpleEventGenerator::Uniform);
        gen->set_vertex_distribution_mean(0.0,0.0,0.0);
        gen->set_vertex_distribution_width(0.0,0.0,5.0);
      }
      gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
      gen->set_vertex_size_parameters(0.0,0.0);
      gen->set_eta_range(100.0, 100.0);
      //gen->set_eta_range(3.0, 3.0); //EICDetector FWD
      gen->set_phi_range(-1.0*TMath::Pi(), 1.0*TMath::Pi());
      //gen->set_phi_range(TMath::Pi()/2-0.1, TMath::Pi()/2-0.1);
      gen->set_p_range(250.0, 250.0);
      gen->Embed(1);
      gen->Verbosity(0);
      se->registerSubsystem(gen);
    }

  if (!readhits)
    {
      //---------------------
      // Detector description
      //---------------------

      G4Setup(absorberactive, magfield, TPythia6Decayer::kAll,do_svtx,do_preshower,do_cemc,do_hcalin,do_magnet,do_hcalout,do_pipe,do_FGEM,do_EGEM,do_FEMC,do_FHCAL,do_EEMC,do_DIRC,do_RICH,do_Aerogel,do_ExtendedIR,magfield_rescale);

    }

  //---------
  // BBC Reco
  //---------

  if (do_bbc)
    {
      gROOT->LoadMacro("G4_Bbc.C");
      BbcInit();
      Bbc_Reco();
    }

  //--------------
  // IO management
  //--------------

  if (readhits)
    {
      // Hits file
      Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
      hitsin->fileopen(inputFile);
      se->registerInputManager(hitsin);
    }
  if (readhepmc)
    {
      Fun4AllInputManager *in = new Fun4AllHepMCInputManager( "DSTIN");
      se->registerInputManager( in );
      se->fileopen( in->Name().c_str(), inputFile );
    }
  else
    {
      // for single particle generators we just need something which drives
      // the event loop, the Dummy Input Mgr does just that
      Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
      se->registerInputManager( in );
    }

  //Convert DST to human command readable TTree for quick poke around the outputs
  gROOT->LoadMacro("G4_DSTReader_EICIR.C");

  G4DSTreader_EICIR( outputFile, //
		     /*int*/ absorberactive );

  //Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
  //if (do_dst_compress) DstCompress(out);
  //se->registerOutputManager(out);

  if (nEvents == 0 && !readhits && !readhepmc)
    {
      cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
      cout << "it will run forever, so I just return without running anything" << endl;
      return;
    }

  if (nEvents < 0)
    {
      PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
      g4->ApplyCommand("/control/execute vis.mac");
      //g4->StartGui();
      se->run(1);

      se->End();
      std::cout << "All done" << std::endl;


      std::cout << "==== Useful display commands ==" << std::endl;
      cout << "draw axis: " << endl;
      cout << " G4Cmd(\"/vis/scene/add/axes 0 0 0 50 cm\")" << endl;
      cout << "zoom" << endl;
      cout << " G4Cmd(\"/vis/viewer/zoom 1\")" << endl;
      cout << "viewpoint:" << endl;
      cout << " G4Cmd(\"/vis/viewer/set/viewpointThetaPhi 0 0\")" << endl;
      cout << "panTo:" << endl;
      cout << " G4Cmd(\"/vis/viewer/panTo 0 0 cm\")" << endl;
      cout << "print to eps:" << endl;
      cout << " G4Cmd(\"/vis/ogl/printEPS\")" << endl;
      cout << "set background color:" << endl;
      cout << " G4Cmd(\"/vis/viewer/set/background white\")" << endl;
      std::cout << "===============================" << std::endl;
    }
  else
    {

      se->run(nEvents);

      se->End();
      std::cout << "All done" << std::endl;
      delete se;
      gSystem->Exit(0);
    }

}


void
G4Cmd(const char * cmd)
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  g4->ApplyCommand(cmd);
}
