#ifndef MACRO_FUN4ALLG4EICDETECTOR_C
#define MACRO_FUN4ALLG4EICDETECTOR_C

#include <GlobalVariables.C>

#include <DisplayOn.C>
#include <G4Setup_EICDetector.C>
#include <G4_Bbc.C>
#include <G4_CaloTrigger.C>
#include <G4_DSTReader_EICDetector.C>
#include <G4_FwdJets.C>
#include <G4_Global.C>
#include <G4_HIJetReco.C>
#include <G4_Input.C>
#include <G4_Jets.C>
#include <G4_Production.C>
#include <G4_User.C>

#include <TROOT.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)

int Fun4All_G4_EICDetector(
    const int nEvents = 1,
    const string &inputFile = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
    const string &outputFile = "G4EICDetector.root",
    const string &embed_input_file = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root",
    const int skip = 0,
    const string &outdir = ".")
{
  //---------------
  // Fun4All server
  //---------------
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  //Opt to print all random seed used for debugging reproducibility. Comment out to reduce stdout prints.
  //PHRandomSeed::Verbosity(1);

  // just if we set some flags somewhere in this macro
  recoConsts *rc = recoConsts::instance();
  // By default every random number generator uses
  // PHRandomSeed() which reads /dev/urandom to get its seed
  // if the RANDOMSEED flag is set its value is taken as initial seed
  // which will produce identical results so you can debug your code
  // rc->set_IntFlag("RANDOMSEED", 12345);

  //===============
  // Input options
  //===============

  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  //
  //Input::READHITS = true;
  INPUTREADHITS::filename = inputFile;

  // Or:
  // Use one or more particle generators
  // It is run if Input::<generator> is set to true
  // all other options only play a role if it is active
  // In case embedding into a production output, please double check your G4Setup_EICDetector.C and G4_*.C consistent with those in the production macro folder
  //  Input::EMBED = true;
  INPUTEMBED::filename = embed_input_file;
  // Use Pythia 8
  //  Input::PYTHIA8 = true;

  // Use Pythia 6
  //   Input::PYTHIA6 = true;

  // Use Sartre
  //   Input::SARTRE = true;

  // Simple multi particle generator in eta/phi/pt ranges
  Input::SIMPLE = true;
  // Input::SIMPLE_NUMBER = 2; // if you need 2 of them
  // Input::SIMPLE_VERBOSITY = 1;

  // Particle gun (same particles in always the same direction)
  // Input::GUN = true;
  // Input::GUN_NUMBER = 3; // if you need 3 of them
  // Input::GUN_VERBOSITY = 0;

  // Upsilon generator
  // Input::UPSILON = true;
  // Input::UPSILON_NUMBER = 3; // if you need 3 of them
  // Input::UPSILON_VERBOSITY = 0;

  // And/Or read generated particles from file

  // eic-smear output
  //  Input::READEIC = true;
  INPUTREADEIC::filename = inputFile;

  // HepMC2 files
  //  Input::HEPMC = true;
  Input::VERBOSITY = 0;
  INPUTHEPMC::filename = inputFile;

  //-----------------
  // Initialize the selected Input/Event generation
  //-----------------
  InputInit();
  //--------------
  // Set generator specific options
  //--------------
  // can only be set after InputInit() is called

  // Simple Input generator:
  // if you run more than one of these Input::SIMPLE_NUMBER > 1
  // add the settings for other with [1], next with [2]...
  if (Input::SIMPLE)
  {
    INPUTGENERATOR::SimpleEventGenerator[0]->add_particles("pi-", 5);
    if (Input::HEPMC || Input::EMBED)
    {
      INPUTGENERATOR::SimpleEventGenerator[0]->set_reuse_existing_vertex(true);
      INPUTGENERATOR::SimpleEventGenerator[0]->set_existing_vertex_offset_vector(0.0, 0.0, 0.0);
    }
    else
    {
      INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
                                                                                PHG4SimpleEventGenerator::Uniform,
                                                                                PHG4SimpleEventGenerator::Uniform);
      INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_mean(0., 0., 0.);
      INPUTGENERATOR::SimpleEventGenerator[0]->set_vertex_distribution_width(0., 0., 5.);
    }
    INPUTGENERATOR::SimpleEventGenerator[0]->set_eta_range(-3, 3);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_phi_range(-M_PI, M_PI);
    INPUTGENERATOR::SimpleEventGenerator[0]->set_pt_range(0.1, 20.);
  }
  // Upsilons
  // if you run more than one of these Input::UPSILON_NUMBER > 1
  // add the settings for other with [1], next with [2]...
  if (Input::UPSILON)
  {
    INPUTGENERATOR::VectorMesonGenerator[0]->add_decay_particles("mu", 0);
    INPUTGENERATOR::VectorMesonGenerator[0]->set_rapidity_range(-1, 1);
    INPUTGENERATOR::VectorMesonGenerator[0]->set_pt_range(0., 10.);
    // Y species - select only one, last one wins
    INPUTGENERATOR::VectorMesonGenerator[0]->set_upsilon_1s();
  }
  // particle gun
  // if you run more than one of these Input::GUN_NUMBER > 1
  // add the settings for other with [1], next with [2]...
  if (Input::GUN)
  {
    INPUTGENERATOR::Gun[0]->AddParticle("pi-", 0, 1, 0);
    INPUTGENERATOR::Gun[0]->set_vtx(0, 0, 0);
  }
  // pythia6
  if (Input::PYTHIA6)
  {
    INPUTGENERATOR::Pythia6->set_config_file(string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6_ep.cfg");
  }

  //--------------
  // Set Input Manager specific options
  //--------------
  // can only be set after InputInit() is called

  if (Input::HEPMC)
  {
    INPUTMANAGER::HepMCInputManager->set_vertex_distribution_width(100e-4, 100e-4, 30, 0);  //optional collision smear in space, time
                                                                                            //    INPUTMANAGER::HepMCInputManager->set_vertex_distribution_mean(0,0,0,0);//optional collision central position shift in space, time
    // //optional choice of vertex distribution function in space, time
    INPUTMANAGER::HepMCInputManager->set_vertex_distribution_function(PHHepMCGenHelper::Gaus, PHHepMCGenHelper::Gaus, PHHepMCGenHelper::Gaus, PHHepMCGenHelper::Gaus);
    //! embedding ID for the event
    //! positive ID is the embedded event of interest, e.g. jetty event from pythia
    //! negative IDs are backgrounds, .e.g out of time pile up collisions
    //! Usually, ID = 0 means the primary Au+Au collision background
    //INPUTMANAGER::HepMCInputManager->set_embedding_id(2);
  }

  // register all input generators with Fun4All
  InputRegister();

  // set up production relatedstuff
  //   Enable::PRODUCTION = true;

  //======================
  // Write the DST
  //======================

  //  Enable::DSTOUT = true;
  DstOut::OutputDir = outdir;
  DstOut::OutputFile = outputFile;
  Enable::DSTOUT_COMPRESS = false;  // Compress DST files

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  //Enable::DSTREADER = true;

  // turn the display on (default off)
  Enable::DISPLAY = false;

  //======================
  // What to run
  //======================
  // Global options (enabled for all subsystems - if implemented)
  //  Enable::ABSORBER = true;
  //  Enable::OVERLAPCHECK = true;
  //  Enable::VERBOSITY = 1;

  //  Enable::BBC = true;
  Enable::BBCFAKE = true; // Smeared vtx and t0, use if you don't want real BBC in simulation

  // whether to simulate the Be section of the beam pipe
  Enable::PIPE = true;
  // EIC beam pipe extension beyond the Be-section:
  //G4PIPE::use_forward_pipes = true;

  Enable::EGEM = true;
  Enable::FGEM = true;
  // barrel tracker
  Enable::BARREL = false;
  Enable::FST = false;
  // mvtx/tpc tracker
  Enable::MVTX = true;
  Enable::TPC = true;
  //  Enable::TPC_ENDCAP = true;

  Enable::TRACKING = true;
  Enable::TRACKING_EVAL = Enable::TRACKING && true;
  G4TRACKING::DISPLACED_VERTEX = false;  // this option exclude vertex in the track fitting and use RAVE to reconstruct primary and 2ndary vertexes
                                         // projections to calorimeters
  G4TRACKING::PROJECTION_CEMC = false;
  G4TRACKING::PROJECTION_FEMC = false;
  G4TRACKING::PROJECTION_FHCAL = false;

  Enable::CEMC = true;
  //  Enable::CEMC_ABSORBER = true;
  Enable::CEMC_CELL = Enable::CEMC && true;
  Enable::CEMC_TOWER = Enable::CEMC_CELL && true;
  Enable::CEMC_CLUSTER = Enable::CEMC_TOWER && true;
  Enable::CEMC_EVAL = Enable::CEMC_CLUSTER && true;

  Enable::HCALIN = true;
  //  Enable::HCALIN_ABSORBER = true;
  Enable::HCALIN_CELL = Enable::HCALIN && true;
  Enable::HCALIN_TOWER = Enable::HCALIN_CELL && true;
  Enable::HCALIN_CLUSTER = Enable::HCALIN_TOWER && true;
  Enable::HCALIN_EVAL = Enable::HCALIN_CLUSTER && true;

  Enable::MAGNET = true;

  Enable::HCALOUT = true;
  //  Enable::HCALOUT_ABSORBER = true;
  Enable::HCALOUT_CELL = Enable::HCALOUT && true;
  Enable::HCALOUT_TOWER = Enable::HCALOUT_CELL && true;
  Enable::HCALOUT_CLUSTER = Enable::HCALOUT_TOWER && true;
  Enable::HCALOUT_EVAL = Enable::HCALOUT_CLUSTER && true;

  // EICDetector geometry - barrel
  Enable::DIRC = true;

  // EICDetector geometry - 'hadron' direction
  Enable::RICH = true;
  Enable::AEROGEL = true;

  Enable::FEMC = true;
  //  Enable::FEMC_ABSORBER = true;
  Enable::FEMC_TOWER = Enable::FEMC && true;
  Enable::FEMC_CLUSTER = Enable::FEMC_TOWER && true;
  Enable::FEMC_EVAL = Enable::FEMC_CLUSTER && true;

  Enable::FHCAL = true;
  //  Enable::FHCAL_ABSORBER = true;
  Enable::FHCAL_TOWER = Enable::FHCAL && true;
  Enable::FHCAL_CLUSTER = Enable::FHCAL_TOWER && true;
  Enable::FHCAL_EVAL = Enable::FHCAL_CLUSTER && true;

  // EICDetector geometry - 'electron' direction
  Enable::EEMC = true;
  Enable::EEMC_TOWER = Enable::EEMC && true;
  Enable::EEMC_CLUSTER = Enable::EEMC_TOWER && true;
  Enable::EEMC_EVAL = Enable::EEMC_CLUSTER && true;

  Enable::PLUGDOOR = true;

  // Other options
  Enable::GLOBAL_RECO = true;
  Enable::GLOBAL_FASTSIM = true;

  Enable::CALOTRIGGER = true && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER;

  // Select only one jet reconstruction- they currently use the same
  // output collections on the node tree!
  Enable::JETS = true;
  Enable::JETS_EVAL = Enable::JETS && true;

  Enable::FWDJETS = true;
  Enable::FWDJETS_EVAL = Enable::FWDJETS && true;

  // HI Jet Reco for jet simulations in Au+Au (default is false for
  // single particle / p+p simulations, or for Au+Au simulations which
  // don't care about jets)
  Enable::HIJETS = false && Enable::JETS && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER;

  // new settings using Enable namespace in GlobalVariables.C
  Enable::BLACKHOLE = true;
  //Enable::BLACKHOLE_SAVEHITS = false; // turn off saving of bh hits
  //BlackHoleGeometry::visible = true;

  //Enable::USER = true;

  //---------------
  // World Settings
  //---------------
  //  G4WORLD::PhysicsList = "QGSP_BERT"; //FTFP_BERT_HP best for calo
  //  G4WORLD::WorldMaterial = "G4_AIR"; // set to G4_GALACTIC for material scans

  //---------------
  // Magnet Settings
  //---------------

  //  const string magfield = "1.5"; // alternatively to specify a constant magnetic field, give a float number, which will be translated to solenoidal field in T, if string use as fieldmap name (including path)
  //  G4MAGNET::magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root");  // default map from the calibration database
  G4MAGNET::magfield_rescale = -1.4 / 1.5;  // make consistent with expected Babar field strength of 1.4T

  //---------------
  // Pythia Decayer
  //---------------
  // list of decay types in
  // $OFFLINE_MAIN/include/g4decayer/EDecayType.hh
  // default is All:
  // G4P6DECAYER::decayType = EDecayType::kAll;

  // Initialize the selected subsystems
  G4Init();

  //---------------------
  // GEANT4 Detector description
  //---------------------

  // If "readhepMC" is also set, the Upsilons will be embedded in Hijing events, if 'particles" is set, the Upsilons will be embedded in whatever particles are thrown
  if (!Input::READHITS)
  {
    G4Setup();
  }

  //------------------
  // Detector Division
  //------------------

  if (Enable::BBC || Enable::BBCFAKE) Bbc_Reco();

  if (Enable::CEMC_CELL) CEMC_Cells();

  if (Enable::HCALIN_CELL) HCALInner_Cells();

  if (Enable::HCALOUT_CELL) HCALOuter_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------

  if (Enable::CEMC_TOWER) CEMC_Towers();
  if (Enable::CEMC_CLUSTER) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------

  if (Enable::HCALIN_TOWER) HCALInner_Towers();
  if (Enable::HCALIN_CLUSTER) HCALInner_Clusters();

  if (Enable::HCALOUT_TOWER) HCALOuter_Towers();
  if (Enable::HCALOUT_CLUSTER) HCALOuter_Clusters();

  //-----------------------------
  // e, h direction Calorimeter  towering and clustering
  //-----------------------------

  if (Enable::FEMC_TOWER) FEMC_Towers();
  if (Enable::FEMC_CLUSTER) FEMC_Clusters();

  if (Enable::FHCAL_TOWER) FHCAL_Towers();
  if (Enable::FHCAL_CLUSTER) FHCAL_Clusters();

  if (Enable::EEMC_TOWER) EEMC_Towers();
  if (Enable::EEMC_CLUSTER) EEMC_Clusters();

  if (Enable::DSTOUT_COMPRESS) ShowerCompress();

  //--------------
  // SVTX tracking
  //--------------

  if (Enable::TRACKING) Tracking_Reco();

  //-----------------
  // Global Vertexing
  //-----------------

  if (Enable::GLOBAL_RECO)
  {
    Global_Reco();
  }
  else if (Enable::GLOBAL_FASTSIM)
  {
    Global_FastSim();
  }

  //-----------------
  // Calo Trigger Simulation
  //-----------------

  if (Enable::CALOTRIGGER) CaloTrigger_Sim();

  //---------
  // Jet reco
  //---------

  if (Enable::JETS) Jet_Reco();

  if (Enable::HIJETS) HIJetReco();

  if (Enable::FWDJETS) Jet_FwdReco();

  string outputroot = outputFile;
  string remove_this = ".root";
  size_t pos = outputroot.find(remove_this);
  if (pos != string::npos)
  {
    outputroot.erase(pos, remove_this.length());
  }

  if (Enable::DSTREADER) G4DSTreader_EICDetector(outputroot + "_DSTReader.root");

  //----------------------
  // Simulation evaluation
  //----------------------
  if (Enable::TRACKING_EVAL) Tracking_Eval(outputroot + "_g4tracking_eval.root");

  if (Enable::CEMC_EVAL) CEMC_Eval(outputroot + "_g4cemc_eval.root");

  if (Enable::HCALIN_EVAL) HCALInner_Eval(outputroot + "_g4hcalin_eval.root");

  if (Enable::HCALOUT_EVAL) HCALOuter_Eval(outputroot + "_g4hcalout_eval.root");

  if (Enable::FEMC_EVAL) FEMC_Eval(outputroot + "_g4femc_eval.root");

  if (Enable::FHCAL_EVAL) FHCAL_Eval(outputroot + "_g4fhcal_eval.root");

  if (Enable::EEMC_EVAL) EEMC_Eval(outputroot + "_g4eemc_eval.root");

  if (Enable::JETS_EVAL) Jet_Eval(outputroot + "_g4jet_eval.root");

  if (Enable::FWDJETS_EVAL) Jet_FwdEval(outputroot + "_g4fwdjet_eval.root");

  if (Enable::USER) UserAnalysisInit();

  //--------------
  // Set up Input Managers
  //--------------

  InputManagers();

  //--------------
  // Set up Output Manager
  //--------------
  if (Enable::PRODUCTION)
  {
    Production_CreateOutputDir();
  }

  if (Enable::DSTOUT)
  {
    string FullOutFile = DstOut::OutputDir + "/" + DstOut::OutputFile;
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", FullOutFile);
    if (Enable::DSTOUT_COMPRESS) DstCompress(out);
    se->registerOutputManager(out);
  }

  //-----------------
  // Event processing
  //-----------------
  if (Enable::DISPLAY)
  {
    DisplayOn();

    gROOT->ProcessLine("Fun4AllServer *se = Fun4AllServer::instance();");
    gROOT->ProcessLine("PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco(\"PHG4RECO\");");

    cout << "-------------------------------------------------" << endl;
    cout << "You are in event display mode. Run one event with" << endl;
    cout << "se->run(1)" << endl;
    cout << "Run Geant4 command with following examples" << endl;
    gROOT->ProcessLine("displaycmd()");

    return 0;
  }
  // if we use a negative number of events we go back to the command line here
  if (nEvents < 0)
  {
    return 0;
  }
  // if we run any of the particle generators and use 0 it'll run forever
  if (nEvents == 0 && !Input::READHITS && !Input::HEPMC && !Input::READEIC)
  {
    cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
    cout << "it will run forever, so I just return without running anything" << endl;
    return 0;
  }

  se->skip(skip);
  se->run(nEvents);

  //-----
  // Exit
  //-----

  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  if (Enable::PRODUCTION)
  {
    Production_MoveOutput();
  }
  gSystem->Exit(0);
  return 0;
}
#endif
