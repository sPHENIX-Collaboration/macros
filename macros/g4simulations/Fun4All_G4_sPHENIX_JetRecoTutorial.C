int Min_cemc_layer = -1;
int Max_cemc_layer = -1;
int Min_hcal_in_layer = -1;
int Max_hcal_in_layer = -1;
int Min_hcal_out_layer = -1;
int Max_hcal_out_layer = -1;
int Min_preshower_layer = -1;
int Max_preshower_layer = -1;
int Min_si_layer = -1;
int Max_si_layer = -1;
int Cemc_slats_per_cell = 72; // make it 2*2*2*3*3 so we can try other combinations

int Fun4All_G4_sPHENIX_JetRecoTutorial(
		       int nEvents = 1,
		       const char * inputFile = "/direct/phenix+u/jdok/work/offline/analysis/sPHENIX_display/inputHEPMC_data/SingleEvent_PYTHIA_15.dat",
		       const char * outputFile = "G4sPHENIXCells.root"
		       )
{
  //===============
  // Input options
  //===============
  
  //PYTHIA dijets                                                               

  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  const bool readhits = false;
  // Or:
  // read files in HepMC format (typically output from event generators like hijing or pythia)
  const bool readhepmc = true; // read HepMC files
  // Or:
  // Use particle generator

  //======================
  // What to run
  //======================

  bool do_svtx = true;
  bool do_svtx_cell = true;
  bool do_svtx_track = true;
  bool do_svtx_eval = true;

  bool do_preshower = false;
  
  bool do_cemc = true;
  bool do_cemc_cell = true;
  bool do_cemc_twr = true;
  bool do_cemc_cluster = true;
  bool do_cemc_eval = true;

  bool do_hcalin = true;
  bool do_hcalin_cell = true;
  bool do_hcalin_twr = true;
  bool do_hcalin_cluster = true;
  bool do_hcalin_eval = true;

  bool do_magnet = true;
  
  bool do_hcalout = true;
  bool do_hcalout_cell = true;
  bool do_hcalout_twr = true;
  bool do_hcalout_cluster = true;
  bool do_hcalout_eval = true;

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
  gSystem->Load("libPHFlowJetMaker.so");

  // establish the geometry and reconstruction setup
  gROOT->LoadMacro("G4Setup_sPHENIX.C");
  G4Init(do_svtx,do_preshower,do_cemc,do_hcalin,do_magnet,do_hcalout);
  
  int absorberactive = 1; // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = "/phenix/upgrades/decadal/fieldmaps/bPHENIX.dp.root"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  // unique seed
  int uniqueseed = TRandom3(0).GetSeed();
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("RANDOMSEED", uniqueseed);

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
  else
    {
      // toss low multiplicity dummy events
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      gen->add_particles("e-",5); // mu+,e+,proton,pi+,Upsilon
      gen->add_particles("e+",5); // mu-,e-,anti_proton,pi-
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
      gen->set_eta_range(-0.5, 0.5);
      gen->set_phi_range(-1.0*TMath::Pi(), 1.0*TMath::Pi());
      gen->set_pt_range(0.1, 10.0);
      gen->set_embedflag(1);
      gen->set_seed(uniqueseed);
      gen->set_verbosity(0);
      se->registerSubsystem(gen);
    }

  if (!readhits)
    {
      //---------------------
      // Detector description
      //---------------------

      G4Setup(absorberactive, magfield, TPythia6Decayer::kAll,
	      do_svtx, do_preshower, do_cemc, do_hcalin, do_magnet, do_hcalout);
    }

  //------------------
  // Detector Division
  //------------------

  if (do_svtx_cell) Svtx_Cells();

  if (do_cemc_cell) CEMC_Cells();

  if (do_hcalin_cell) HCALInner_Cells();

  if (do_hcalout_cell) HCALOuter_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------

  if (do_cemc_twr) CEMC_Towers();
  if (do_cemc_cluster) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------
  
  if (do_hcalin_cell) HCALInner_Towers();
  if (do_hcalin_cell) HCALInner_Clusters();

  if (do_hcalout_cell) HCALOuter_Towers();
  if (do_hcalout_cell) HCALOuter_Clusters();

  //--------------
  // SVTX tracking
  //--------------

  if (do_svtx_track) Svtx_Reco();

  //----------------------
  // Simulation evaluation
  //----------------------

  if (do_svtx_eval) Svtx_Eval("g4svtx_eval.root");

  if (do_cemc_eval) CEMC_Eval("g4cemc_eval.root");

  if (do_hcalin_cell) HCALInner_Eval("g4hcalin_eval.root");

  if (do_hcalout_cell) HCALOuter_Eval("g4hcalout_eval.root");

  PHFlowJetMaker *ana = new PHFlowJetMaker(outputFile);
  se->registerSubsystem(ana);

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
      se->fileopen( in->Name(), inputFile );
    }
  else
    {
      // for single particle generators we just need something which drives
      // the event loop, the Dummy Input Mgr does just that
      Fun4AllInputManager *in = new Fun4AllDummyInputManager( "JADE");
      se->registerInputManager( in );
    }


  // Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
  // se->registerOutputManager(out);

  //-----------------
  // Event processing
  //-----------------
  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
}

int Get_Min_cemc_layer()
{
  return Min_cemc_layer;
}

int Get_Max_cemc_layer()
{
  return Max_cemc_layer;
}

int Get_Min_hcal_in_layer()
{
  return Min_hcal_in_layer;
}

int Get_Max_hcal_in_layer()
{
  return Max_hcal_in_layer;
}

int Get_Min_hcal_out_layer()
{
  return Min_hcal_out_layer;
}

int Get_Max_hcal_out_layer()
{
  return Max_hcal_out_layer;
}

int Get_Min_si_layer()
{
  return Min_si_layer;
}

int Get_Max_si_layer()
{
  return Max_si_layer;
}

int Get_Min_preshower_layer()
{
  return Min_preshower_layer;
}

int Get_Max_preshower_layer()
{
  return Max_preshower_layer;
}
