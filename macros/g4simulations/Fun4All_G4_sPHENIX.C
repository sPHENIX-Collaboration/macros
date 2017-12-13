#include <iostream>
using namespace std;

int Fun4All_G4_sPHENIX(
    const int nEvents = 1,
    const char *inputFile = "/sphenix/data/data02/review_2017-08-02/single_particle/spacal2d/fieldmap/G4Hits_sPHENIX_e-_eta0_8GeV-0002.root",
    const char *outputFile = "G4sPHENIX.root",
    const char *embed_input_file = "/sphenix/data/data02/review_2017-08-02/sHijing/fm_0-4.list")
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
  //
  // In case reading production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/
  const bool readhits = false;
  // Or:
  // read files in HepMC format (typically output from event generators like hijing or pythia)
  const bool readhepmc = false;  // read HepMC files
  // Or:
  // Use pythia
  const bool runpythia8 = false;
  const bool runpythia6 = false;
  //
  // **** And ****
  // Further choose to embed newly simulated events to a previous simulation. Not compatible with `readhits = true`
  // In case embedding into a production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/data/data02/review_2017-08-02/
  const bool do_embedding = false;

  // Besides the above flags. One can further choose to further put in following particles in Geant4 simulation
  // Use multi-particle generator (PHG4SimpleEventGenerator), see the code block below to choose particle species and kinematics
  const bool particles = true && !readhits;
  // or gun/ very simple single particle gun generator
  const bool usegun = false && !readhits;
  // Throw single Upsilons, may be embedded in Hijing by setting readhepmc flag also  (note, careful to set Z vertex equal to Hijing events)
  const bool upsilons = false && !readhits;
  // Event pile up simulation with collision rate in Hz MB collisions.
  // Note please follow up the macro to verify the settings for beam parameters
  const double pileup_collision_rate = 0;  // 100e3 for 100kHz nominal AuAu collision rate.

  //======================
  // What to run
  //======================

  bool do_bbc = true;

  bool do_pipe = true;

  bool do_svtx = true;
  bool do_svtx_cell = do_svtx && true;
  bool do_svtx_track = do_svtx_cell && true;
  bool do_svtx_eval = do_svtx_track && true;

  bool do_pstof = false;

  bool do_cemc = true;
  bool do_cemc_cell = do_cemc && true;
  bool do_cemc_twr = do_cemc_cell && true;
  bool do_cemc_cluster = do_cemc_twr && true;
  bool do_cemc_eval = do_cemc_cluster && true;

  bool do_hcalin = true;
  bool do_hcalin_cell = do_hcalin && true;
  bool do_hcalin_twr = do_hcalin_cell && true;
  bool do_hcalin_cluster = do_hcalin_twr && true;
  bool do_hcalin_eval = do_hcalin_cluster && true;

  bool do_magnet = true;

  bool do_hcalout = true;
  bool do_hcalout_cell = do_hcalout && true;
  bool do_hcalout_twr = do_hcalout_cell && true;
  bool do_hcalout_cluster = do_hcalout_twr && true;
  bool do_hcalout_eval = do_hcalout_cluster && true;

  bool do_global = true;
  bool do_global_fastsim = true;

  bool do_calotrigger = true && do_cemc_twr && do_hcalin_twr && do_hcalout_twr;

  bool do_jet_reco = true;
  bool do_jet_eval = do_jet_reco && true;

  // HI Jet Reco for p+Au / Au+Au collisions (default is false for
  // single particle / p+p-only simulations, or for p+Au / Au+Au
  // simulations which don't particularly care about jets)
  bool do_HIjetreco = false && do_cemc_twr && do_hcalin_twr && do_hcalout_twr;

  bool do_dst_compress = false;

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  bool do_DSTReader = false;
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
  gROOT->LoadMacro("G4Setup_sPHENIX.C");
  G4Init(do_svtx, do_pstof, do_cemc, do_hcalin, do_magnet, do_hcalout, do_pipe, n_TPC_layers);

  int absorberactive = 1;  // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5"; // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = "/phenix/upgrades/decadal/fieldmaps/sPHENIX.2d.root";  // if like float -> solenoidal field in T, if string use as fieldmap name (including path)
  const float magfield_rescale = -1.4 / 1.5;                                     // scale the map to a 1.4 T field

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as seed
  // You ca neither set this to a random value using PHRandomSeed()
  // which will make all seeds identical (not sure what the point of
  // this would be:
  //  rc->set_IntFlag("RANDOMSEED",PHRandomSeed());
  // or set it to a fixed value so you can debug your code
  //  rc->set_IntFlag("RANDOMSEED", 12345);

  //-----------------
  // Event generation
  //-----------------

  if (readhits)
  {
    // Get the hits from a file
    // The input manager is declared later

    if (do_embedding)
    {
      cout << "Do not support read hits and embed background at the same time." << endl;
      exit(1);
    }
  }
  else
  {
    // running Geant4 stage. First load event generators.

    if (readhepmc)
    {
      // place holder. Additional action is performed in later stage at the input manager level
    }

    if (runpythia8)
    {
      gSystem->Load("libPHPythia8.so");

      PHPythia8 *pythia8 = new PHPythia8();
      // see coresoftware/generators/PHPythia8 for example config
      pythia8->set_config_file("phpythia8.cfg");
      if (readhepmc)
        pythia8->set_reuse_vertex(0);  // reuse vertex of subevent with embedding ID of 0
      // pythia8->set_vertex_distribution_width(0,0,10,0); // additional vertex smearing if needed, more vertex options available
      se->registerSubsystem(pythia8);
    }

    if (runpythia6)
    {
      gSystem->Load("libPHPythia6.so");

      PHPythia6 *pythia6 = new PHPythia6();
      pythia6->set_config_file("phpythia6.cfg");
      if (readhepmc)
        pythia6->set_reuse_vertex(0);  // reuse vertex of subevent with embedding ID of 0
      // pythia6->set_vertex_distribution_width(0,0,10,0); // additional vertex smearing if needed, more vertex options available
      se->registerSubsystem(pythia6);
    }

    // If "readhepMC" is also set, the particles will be embedded in Hijing events
    if (particles)
    {
      // toss low multiplicity dummy events
      PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
      gen->add_particles("pi-", 2);  // mu+,e+,proton,pi+,Upsilon
      //gen->add_particles("pi+",100); // 100 pion option
      if (readhepmc || do_embedding || runpythia8 || runpythia6)
      {
        gen->set_reuse_existing_vertex(true);
        gen->set_existing_vertex_offset_vector(0.0, 0.0, 0.0);
      }
      else
      {
        gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
                                              PHG4SimpleEventGenerator::Uniform,
                                              PHG4SimpleEventGenerator::Uniform);
        gen->set_vertex_distribution_mean(0.0, 0.0, 0.0);
        gen->set_vertex_distribution_width(0.0, 0.0, 5.0);
      }
      gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
      gen->set_vertex_size_parameters(0.0, 0.0);
      gen->set_eta_range(-1.0, 1.0);
      gen->set_phi_range(-1.0 * TMath::Pi(), 1.0 * TMath::Pi());
      //gen->set_pt_range(0.1, 50.0);
      gen->set_pt_range(0.1, 20.0);
      gen->Embed(2);
      gen->Verbosity(0);

      se->registerSubsystem(gen);
    }

    if (usegun)
    {
      PHG4ParticleGun *gun = new PHG4ParticleGun();
      //  gun->set_name("anti_proton");
      gun->set_name("geantino");
      gun->set_vtx(0, 0, 0);
      gun->set_mom(10, 0, 0.01);
      // gun->AddParticle("geantino",1.7776,-0.4335,0.);
      // gun->AddParticle("geantino",1.7709,-0.4598,0.);
      // gun->AddParticle("geantino",2.5621,0.60964,0.);
      // gun->AddParticle("geantino",1.8121,0.253,0.);
      //	  se->registerSubsystem(gun);
      PHG4ParticleGenerator *pgen = new PHG4ParticleGenerator();
      pgen->set_name("geantino");
      pgen->set_z_range(0, 0);
      pgen->set_eta_range(0.01, 0.01);
      pgen->set_mom_range(10, 10);
      pgen->set_phi_range(5.3 / 180. * TMath::Pi(), 5.7 / 180. * TMath::Pi());
      se->registerSubsystem(pgen);
    }

    // If "readhepMC" is also set, the Upsilons will be embedded in Hijing events, if 'particles" is set, the Upsilons will be embedded in whatever particles are thrown
    if (upsilons)
    {
      // run upsilons for momentum, dca performance, alone or embedded in Hijing

      PHG4ParticleGeneratorVectorMeson *vgen = new PHG4ParticleGeneratorVectorMeson();
      vgen->add_decay_particles("e+", "e-", 0);  // i = decay id
      // event vertex
      if (readhepmc || do_embedding || particles || runpythia8 || runpythia6)
      {
        vgen->set_reuse_existing_vertex(true);
      }
      else
      {
        vgen->set_vtx_zrange(-10.0, +10.0);
      }

      // Note: this rapidity range completely fills the acceptance of eta = +/- 1 unit
      vgen->set_rapidity_range(-1.0, +1.0);
      vgen->set_pt_range(0.0, 10.0);

      int istate = 1;

      if (istate == 1)
      {
        // Upsilon(1S)
        vgen->set_mass(9.46);
        vgen->set_width(54.02e-6);
      }
      else if (istate == 2)
      {
        // Upsilon(2S)
        vgen->set_mass(10.0233);
        vgen->set_width(31.98e-6);
      }
      else
      {
        // Upsilon(3S)
        vgen->set_mass(10.3552);
        vgen->set_width(20.32e-6);
      }

      vgen->Verbosity(0);
      vgen->Embed(3);
      se->registerSubsystem(vgen);

      cout << "Upsilon generator for istate = " << istate << " created and registered " << endl;
    }
  }

  if (!readhits)
  {
    //---------------------
    // Detector description
    //---------------------

    G4Setup(absorberactive, magfield, TPythia6Decayer::kAll,
            do_svtx, do_pstof, do_cemc, do_hcalin, do_magnet, do_hcalout, do_pipe, magfield_rescale);
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

  if (do_dst_compress) ShowerCompress();

  //--------------
  // SVTX tracking
  //--------------

  if (do_svtx_track) Svtx_Reco();

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

  //-----------------
  // Calo Trigger Simulation
  //-----------------

  if (do_calotrigger)
  {
    gROOT->LoadMacro("G4_CaloTrigger.C");
    CaloTrigger_Sim();
  }

  //---------
  // Jet reco
  //---------

  if (do_jet_reco)
  {
    gROOT->LoadMacro("G4_Jets.C");
    Jet_Reco();
  }

  if (do_HIjetreco)
  {
    gROOT->LoadMacro("G4_HIJetReco.C");
    HIJetReco();
  }

  //----------------------
  // Simulation evaluation
  //----------------------

  if (do_svtx_eval) Svtx_Eval(string(outputFile) + "_g4svtx_eval.root");

  if (do_cemc_eval) CEMC_Eval(string(outputFile) + "_g4cemc_eval.root");

  if (do_hcalin_eval) HCALInner_Eval(string(outputFile) + "_g4hcalin_eval.root");

  if (do_hcalout_eval) HCALOuter_Eval(string(outputFile) + "_g4hcalout_eval.root");

  if (do_jet_eval) Jet_Eval(string(outputFile) + "_g4jet_eval.root");

  //--------------
  // IO management
  //--------------

  if (readhits)
  {
    //meta-lib for DST objects used in simulation outputs
    gSystem->Load("libg4dst.so");

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

    //meta-lib for DST objects used in simulation outputs
    gSystem->Load("libg4dst.so");

    Fun4AllDstInputManager *in1 = new Fun4AllNoSyncDstInputManager("DSTinEmbed");
    //      in1->AddFile(embed_input_file); // if one use a single input file
    in1->AddListFile(embed_input_file);  // RecommendedL: if one use a text list of many input files
    se->registerInputManager(in1);
  }

  if (readhepmc)
  {
    //meta-lib for DST objects used in simulation outputs
    gSystem->Load("libg4dst.so");

    Fun4AllHepMCInputManager *in = new Fun4AllHepMCInputManager("HepMCInput_1");
    se->registerInputManager(in);
    se->fileopen(in->Name().c_str(), inputFile);
    //in->set_vertex_distribution_width(100e-4,100e-4,30,0);//optional collision smear in space, time
    //in->set_vertex_distribution_mean(0,0,1,0);//optional collision central position shift in space, time
    // //optional choice of vertex distribution function in space, time
    //in->set_vertex_distribution_function(PHHepMCGenHelper::Gaus,PHHepMCGenHelper::Gaus,PHHepMCGenHelper::Uniform,PHHepMCGenHelper::Gaus);
    //! embedding ID for the event
    //! positive ID is the embedded event of interest, e.g. jetty event from pythia
    //! negative IDs are backgrounds, .e.g out of time pile up collisions
    //! Usually, ID = 0 means the primary Au+Au collision background
    //in->set_embedding_id(2);
  }
  else
  {
    // for single particle generators we just need something which drives
    // the event loop, the Dummy Input Mgr does just that
    Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
    se->registerInputManager(in);
  }

  if (pileup_collision_rate > 0)
  {
    // pile up simulation.
    // add random beam collisions following a collision diamond and rate from a HepMC stream
    Fun4AllHepMCPileupInputManager *pileup = new Fun4AllHepMCPileupInputManager("HepMCPileupInput");
    se->registerInputManager(pileup);

    const string pileupfile("/sphenix/sim/sim01/sHijing/sHijing_0-12fm.dat");
    pileup->AddFile(pileupfile);  // HepMC events used in pile up collisions. You can add multiple files, and the file list will be reused.
    //pileup->set_vertex_distribution_width(100e-4,100e-4,30,5);//override collision smear in space time
    //pileup->set_vertex_distribution_mean(0,0,0,0);//override collision central position shift in space time
    pileup->set_collision_rate(pileup_collision_rate);

    double time_window_minus = -35000;
    double time_window_plus = 35000;

    if (do_svtx)
    {
      // double TPCDriftVelocity = 6.0 / 1000.0; // cm/ns, which is loaded from G4_SVTX*.C macros
      time_window_minus = -105.5 / TPCDriftVelocity;  // ns
      time_window_plus = 105.5 / TPCDriftVelocity;    // ns;
    }
    pileup->set_time_window(time_window_minus, time_window_plus);  // override timing window in ns
    cout << "Collision pileup enabled using file " << pileupfile << " with collision rate " << pileup_collision_rate
         << " and time window " << time_window_minus << " to " << time_window_plus << endl;
  }

  if (do_DSTReader)
  {
    //Convert DST to human command readable TTree for quick poke around the outputs
    gROOT->LoadMacro("G4_DSTReader.C");

    G4DSTreader(outputFile,  //
                /*int*/ absorberactive,
                /*bool*/ do_svtx,
                /*bool*/ do_pstof,
                /*bool*/ do_cemc,
                /*bool*/ do_hcalin,
                /*bool*/ do_magnet,
                /*bool*/ do_hcalout,
                /*bool*/ do_cemc_twr,
                /*bool*/ do_hcalin_twr,
                /*bool*/ do_magnet,
                /*bool*/ do_hcalout_twr);
  }

  //  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
  // if (do_dst_compress) DstCompress(out);
  //  se->registerOutputManager(out);

  //-----------------
  // Event processing
  //-----------------
  if (nEvents < 0)
  {
    return;
  }
  // if we run the particle generator and use 0 it'll run forever
  if (nEvents == 0 && !readhits && !readhepmc)
  {
    cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
    cout << "it will run forever, so I just return without running anything" << endl;
    return;
  }

  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
}
