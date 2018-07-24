
int Fun4All_G4_fsPHENIX(
		       const int nEvents = 2,
           const char * inputFile = "/sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/fieldmap/G4Hits_sPHENIX_e-_eta0_8GeV-0002.root",
		       const char * outputFile = "G4fsPHENIX.root",
           const char * embed_input_file = "/sphenix/sim/sim01/production/2016-07-12/sHijing/spacal2d/G4Hits_sPHENIX_sHijing-0-4.4fm.list"
		       )
{
  // Set the number of TPC layer
  const int n_TPC_layers = 40;  // use 60 for backward compatibility only

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
  // And
  // Further choose to embed newly simulated events to a previous simulation. Not compatible with `readhits = true`
  // In case embedding into a production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/
  const bool do_embedding = false;

  //======================
  // What to run
  //======================

  bool do_bbc = true;
  
  bool do_pipe = true;
  
  bool do_svtx = true;
  bool do_svtx_cell = do_svtx && true;
  bool do_svtx_track = do_svtx_cell && true;
  bool do_svtx_eval = do_svtx_track && false;

  bool do_cemc = true;
  bool do_cemc_cell = do_cemc && true;
  bool do_cemc_twr = do_cemc_cell && true;
  bool do_cemc_cluster = do_cemc_twr && true;
  bool do_cemc_eval = do_cemc_cluster && false;

  bool do_hcalin = true;
  bool do_hcalin_cell = do_hcalin && true;
  bool do_hcalin_twr = do_hcalin_cell && true;
  bool do_hcalin_cluster = do_hcalin_twr && true;
  bool do_hcalin_eval = do_hcalin_cluster && false;

  bool do_magnet = true;

  bool do_hcalout = true;
  bool do_hcalout_cell = do_hcalout && true;
  bool do_hcalout_twr = do_hcalout_cell && true;
  bool do_hcalout_cluster = do_hcalout_twr && true;
  bool do_hcalout_eval = do_hcalout_cluster && false;

  bool do_global = true;
  bool do_global_fastsim = false;

  bool do_jet_reco = false;
  bool do_jet_eval = do_jet_reco && true;

  bool do_fwd_jet_reco = true;
  bool do_fwd_jet_eval = do_fwd_jet_reco && true;

  // fsPHENIX geometry

  bool do_FGEM = true;
  bool do_FGEM_track = do_FGEM &&  true;

  bool do_FEMC = true;
  bool do_FEMC_cell = do_FEMC && true;
  bool do_FEMC_twr = do_FEMC_cell && true;
  bool do_FEMC_cluster = do_FEMC_twr && true;

  bool do_FHCAL = true;
  bool do_FHCAL_cell = do_FHCAL && true;
  bool do_FHCAL_twr = do_FHCAL_cell && true;
  bool do_FHCAL_cluster = do_FHCAL_twr && true;

  bool do_dst_compress = false;
  
  //Option to convert DST to human command readable TTree for quick poke around the outputs
  bool do_DSTReader = true;
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libphhepmc.so");
  gSystem->Load("libg4testbench.so");
  gSystem->Load("libg4hough.so");
  gSystem->Load("libg4eval.so");

  // establish the geometry and reconstruction setup
  gROOT->LoadMacro("G4Setup_fsPHENIX.C");
  G4Init(do_svtx,do_cemc,do_hcalin,do_magnet,do_hcalout,do_pipe,do_FGEM,do_FEMC,do_FHCAL,n_TPC_layers);

  int absorberactive = 0; // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5"; // alternatively to specify a constant magnetic field, give a float number, which will be translated to solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root"); // default map from the calibration database
  const float magfield_rescale = 1.0; // already adjusted to 1.4T central field

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
//  se->Verbosity(0); // uncomment for batch production running with minimal output messages
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
      if (do_embedding)
       {
         cout <<"Do not support read hits and embed background at the same time."<<endl;
         exit(1);
       }
    }
  else if (readhepmc)
    {
    // action is performed in later stage at the input manager level
    }
  else if (runpythia8)
    {
      gSystem->Load("libPHPythia8.so");
      
      PHPythia8* pythia8 = new PHPythia8();
      // see coresoftware/generators/PHPythia8 for example config
      pythia8->set_config_file("phpythia8.cfg"); 
      se->registerSubsystem(pythia8);
    }
  else if (runpythia6)
    {
      gSystem->Load("libPHPythia6.so");

      PHPythia6 *pythia6 = new PHPythia6();
      pythia6->set_config_file("phpythia6.cfg");
      se->registerSubsystem(pythia6);
    }
  else
    {
      // toss low multiplicity dummy events
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      //gen->add_particles("e-",5); // mu+,e+,proton,pi+,Upsilon
      //gen->add_particles("e+",5); // mu-,e-,anti_proton,pi-
      gen->add_particles("pi-",1); // mu-,e-,anti_proton,pi-
      if (readhepmc || do_embedding) {
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
      gen->set_eta_range(1.4, 3.0);
      //gen->set_eta_range(3.0, 3.0); //fsPHENIX FWD
      gen->set_phi_range(-1.0*TMath::Pi(), 1.0*TMath::Pi());
      //gen->set_phi_range(TMath::Pi()/2-0.1, TMath::Pi()/2-0.1);
      gen->set_p_range(30.0, 30.0);
      gen->Embed(1);
      gen->Verbosity(0);
      se->registerSubsystem(gen);
    }

  if (!readhits)
    {
      //---------------------
      // Detector description
      //---------------------

      G4Setup(absorberactive, magfield, TPythia6Decayer::kAll,
	      do_svtx, do_cemc, do_hcalin, do_magnet, do_hcalout, do_pipe,
	      do_FGEM, do_FEMC, do_FHCAL,
	      magfield_rescale);
      
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
  
  //------------------
  // Detector Division
  //------------------

  if (do_svtx_cell) Svtx_Cells();

  if (do_cemc_cell) CEMC_Cells();

  if (do_hcalin_cell) HCALInner_Cells();

  if (do_hcalout_cell) HCALOuter_Cells();

  if (do_FEMC_cell) FEMC_Cells();
  if (do_FHCAL_cell) FHCAL_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------

  if (do_cemc_twr) CEMC_Towers();
  if (do_cemc_cluster) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------
  
  if (do_hcalin_twr) HCALInner_Towers();
  if (do_hcalin_cluster) HCALInner_Clusters();

  if (do_hcalout_twr) HCALOuter_Towers();
  if (do_hcalout_cluster) HCALOuter_Clusters();

  if (do_FEMC_twr) FEMC_Towers();
  if (do_FEMC_cluster) FEMC_Clusters();

  if (do_FHCAL_twr) FHCAL_Towers();
  if (do_FHCAL_cluster) FHCAL_Clusters();

  if (do_dst_compress) ShowerCompress();
  
  //--------------
  // SVTX tracking
  //--------------

  if (do_svtx_track) Svtx_Reco();

  //--------------
  // FGEM tracking
  //--------------

  if(do_FGEM_track) FGEM_FastSim_Reco();

  //-----------------
  // Global Vertexing
  //-----------------

  if (do_global) 
    {
      gROOT->LoadMacro("G4_Global.C");
      Global_Reco();
    }

  else if (do_global_fastsim) 
    {
      gROOT->LoadMacro("G4_Global.C");
      Global_FastSim();
    }  
  
  //---------
  // Jet reco
  //---------

  if (do_jet_reco) 
    {
      gROOT->LoadMacro("G4_Jets.C");
      Jet_Reco();
    }
 
  if (do_fwd_jet_reco)
    {
      gROOT->LoadMacro("G4_FwdJets.C");
      Jet_FwdReco();
    }
  //----------------------
  // Simulation evaluation
  //----------------------

  if (do_svtx_eval) Svtx_Eval("g4svtx_eval.root");

  if (do_cemc_eval) CEMC_Eval("g4cemc_eval.root");

  if (do_hcalin_eval) HCALInner_Eval("g4hcalin_eval.root");

  if (do_hcalout_eval) HCALOuter_Eval("g4hcalout_eval.root");

  if (do_jet_eval) Jet_Eval("g4jet_eval.root");

  if (do_fwd_jet_eval) Jet_FwdEval("g4fwdjet_eval.root");

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
  if (do_embedding)
    {
      if (embed_input_file == NULL)
        {
          cout << "Missing embed_input_file! Exit";
          exit(3);
        }

      Fun4AllDstInputManager *in1 = new Fun4AllNoSyncDstInputManager("DSTinEmbed");
      //      in1->AddFile(embed_input_file); // if one use a single input file
      in1->AddListFile(embed_input_file); // RecommendedL: if one use a text list of many input files
      se->registerInputManager(in1);
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

  if (do_DSTReader)
    {
      //Convert DST to human command readable TTree for quick poke around the outputs
      gROOT->LoadMacro("G4_DSTReader_fsPHENIX.C");

      G4DSTreader_fsPHENIX( outputFile, //
          /*int*/ absorberactive ,
          /*bool*/ do_svtx ,
          /*bool*/ do_cemc ,
          /*bool*/ do_hcalin ,
          /*bool*/ do_magnet ,
          /*bool*/ do_hcalout ,
          /*bool*/ do_cemc_twr ,
          /*bool*/ do_hcalin_twr ,
          /*bool*/ do_magnet  ,
	  /*bool*/ do_hcalout_twr,
    /*bool*/ do_FGEM,
	  /*bool*/ do_FHCAL,
	  /*bool*/ do_FHCAL_twr,
	  /*bool*/ do_FEMC,
	  /*bool*/ do_FEMC_twr
          );
    }

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
