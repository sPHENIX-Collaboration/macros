#pragma once

#include "GlobalVariables.C"

#include "G4Setup_sPHENIX.C"
#include "G4_Bbc.C"
#include "G4_Global.C"
#include "G4_CaloTrigger.C"
#include "G4_Jets.C"
#include "G4_HIJetReco.C"
#include "G4_TopoClusterReco.C"
#include "G4_ParticleFlow.C"
#include "G4_DSTReader.C"
#include "G4_Input.C"
#include "DisplayOn.C"

#include <g4detectors/PHG4DetectorSubsystem.h>

#include <g4main/PHG4ParticleGeneratorBase.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4SimpleEventGenerator.h>
#include <g4main/PHG4ParticleGeneratorVectorMeson.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/HepMCNodeReader.h>

#include <phhepmc/Fun4AllHepMCPileupInputManager.h>
#include <phhepmc/Fun4AllHepMCInputManager.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libphhepmc.so)


int Fun4All_G4_sPHENIX(
    const int nEvents = 1,
    const char *inputFile = "/sphenix/data/data02/review_2017-08-02/single_particle/spacal2d/fieldmap/G4Hits_sPHENIX_e-_eta0_8GeV-0002.root",
    const char *outputFile = "G4sPHENIX.root",
    const char *embed_input_file = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/sPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  //Opt to print all random seed used for debugging reproducibility. Comment out to reduce stdout prints.
  PHRandomSeed::Verbosity(1);

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
    rc->set_IntFlag("RANDOMSEED", 12345);


  //===============
  // Input options
  //===============
  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
//  Input::READHITS = true;
  INPUTREADHITS::filename = inputFile;

  // Or:
  // Use particle generator
  // And
  // Further choose to embed newly simulated events to a previous simulation. Not compatible with `readhits = true`
  // In case embedding into a production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/

//  Input::EMBED = true;
  INPUTEMBED::filename = embed_input_file;

  Input::SIMPLE = true;
  //Input::SIMPLE_VERBOSITY = 1;
  INPUTSIMPLE::AddParticle("pi-", 5);
  //  INPUTSIMPLE::AddParticle("e-",0);
  //  INPUTSIMPLE::AddParticle("pi-",10);
  INPUTSIMPLE::set_eta_range(-1, 1);
  INPUTSIMPLE::set_phi_range(-M_PI, M_PI);
  INPUTSIMPLE::set_pt_range(0.1, 20.);
  INPUTSIMPLE::set_vtx_mean(0., 0., 0.);
  INPUTSIMPLE::set_vtx_width(0., 0., 5.);

  //  Input::PYTHIA6 = true;

  //  Input::PYTHIA8 = true;

  //  Input::GUN = true;
  //Input::GUN_VERBOSITY = 0;
  INPUTGUN::AddParticle("pi-", 0, 1, 0);
  //INPUTGUN::set_vtx(0,0,0);

  // Upsilon generator
  //Input::UPSILON = true;
  Input::UPSILON_VERBOSITY = 0;
  INPUTUPSILON::AddDecayParticles("e+", "e-", 0);

//  Input::HEPMC = true;
  Input::VERBOSITY = 0;
  INPUTHEPMC::filename = inputFile;

  //-----------------
  // Initialize the selected Input/Event generation
  //-----------------
  InputInit();

  // Event pile up simulation with collision rate in Hz MB collisions.
  // Note please follow up the macro to verify the settings for beam parameters
  const double pileup_collision_rate = 0;  // 100e3 for 100kHz nominal AuAu collision rate.
  const bool do_write_output = true;
  // To write cluster files set do_write_output = true and set 
  // do_tracking = true, do_tracking_cell = true, do_tracking_cluster = true and 
  // leave the tracking for later do_tracking_track =  false,  do_tracking_eval = false

  //======================
  // What to run
  //======================

  bool do_bbc = false;

  bool do_pipe = true;

  bool do_tracking = false;
  bool do_tracking_cell = do_tracking && false;
  bool do_tracking_cluster = do_tracking_cell && true;
  bool do_tracking_track = do_tracking_cluster && true;
  bool do_tracking_eval = do_tracking_track && true;

  bool do_pstof = false;

  Enable::CEMC = true;
  Enable::CEMC_ABSORBER = true;
  Enable::CEMC_CELL = Enable::CEMC && true;
  Enable::CEMC_TOWER = Enable::CEMC_CELL && true;
  Enable::CEMC_CLUSTER = Enable::CEMC_TOWER && true;
  Enable::CEMC_EVAL = Enable::CEMC_CLUSTER && true;
/*
  bool do_cemc = true;
  bool do_cemc_cell = do_cemc && true;
  bool do_cemc_twr = do_cemc_cell && true;
  bool do_cemc_cluster = do_cemc_twr && true;
  bool do_cemc_eval = do_cemc_cluster && true;
*/
  bool do_hcalin = false;
  bool do_hcalin_cell = do_hcalin && false;
  bool do_hcalin_twr = do_hcalin_cell && true;
  bool do_hcalin_cluster = do_hcalin_twr && true;
  bool do_hcalin_eval = do_hcalin_cluster && true;

  bool do_magnet = false;

  bool do_hcalout = false;
  bool do_hcalout_cell = do_hcalout && false;
  bool do_hcalout_twr = do_hcalout_cell && false;
  bool do_hcalout_cluster = do_hcalout_twr && true;
  bool do_hcalout_eval = do_hcalout_cluster && true;

  // forward EMC
  bool do_femc = false;
  bool do_femc_cell = do_femc && true;
  bool do_femc_twr = do_femc_cell && true;
  bool do_femc_cluster = do_femc_twr && true;
  bool do_femc_eval = do_femc_cluster && true;

  //! forward flux return plug door. Out of acceptance and off by default.
  bool do_plugdoor = false;

  bool do_global = false;
  bool do_global_fastsim = false;

  bool do_calotrigger = true && Enable::CEMC_TOWER && do_hcalin_twr && do_hcalout_twr;

  bool do_jet_reco = false;
  bool do_jet_eval = do_jet_reco && true;

  // HI Jet Reco for p+Au / Au+Au collisions (default is false for
  // single particle / p+p-only simulations, or for p+Au / Au+Au
  // simulations which don't particularly care about jets)
  bool do_HIjetreco = false && Enable::CEMC_TOWER && do_hcalin_twr && do_hcalout_twr;

  // 3-D topoCluster reconstruction, potentially in all calorimeter layers
  bool do_topoCluster = false && Enable::CEMC_TOWER && do_hcalin_twr && do_hcalout_twr;
  // particle flow jet reconstruction - needs topoClusters!
  bool do_particle_flow = false && do_topoCluster;

  bool do_dst_compress = false;

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  bool do_DSTReader = false;

  G4Init(do_tracking, do_pstof, do_hcalin, do_magnet, do_hcalout, do_pipe, do_plugdoor, do_femc);

  int absorberactive = 1;  // set to 1 to make all absorbers active volumes
  //  const string magfield = "1.5"; // alternatively to specify a constant magnetic field, give a float number, which will be translated to solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root"); // default map from the calibration database
  const float magfield_rescale = -1.4 / 1.5;                                     // scale the map to a 1.4 T field

  //---------------
  // Fun4All server
  //---------------

  bool display_on = false;

  if (!Input::READHITS)
  {
    //---------------------
    // Detector description
    //---------------------

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
    G4Setup(absorberactive, magfield, EDecayType::kAll,
            do_tracking, do_pstof, do_hcalin, do_magnet, do_hcalout, do_pipe,do_plugdoor, do_femc, magfield_rescale);
#else
    G4Setup(absorberactive, magfield, TPythia6Decayer::kAll,
            do_tracking, do_pstof, do_hcalin, do_magnet, do_hcalout, do_pipe,do_plugdoor, do_femc, magfield_rescale);
#endif
  }

  //---------
  // BBC Reco
  //---------

  if (do_bbc)
  {
    BbcInit();
    Bbc_Reco();
  }
  //------------------
  // Detector Division
  //------------------

  if (do_tracking_cell) Tracking_Cells();

  if (Enable::CEMC_CELL) CEMC_Cells();

  if (do_hcalin_cell) HCALInner_Cells();

  if (do_hcalout_cell) HCALOuter_Cells();

  if (do_femc_cell) FEMC_Cells();

  //-----------------------------
  // CEMC towering and clustering
  //-----------------------------

  if (Enable::CEMC_TOWER) CEMC_Towers();
  if (Enable::CEMC_CLUSTER) CEMC_Clusters();

  //-----------------------------
  // HCAL towering and clustering
  //-----------------------------

  if (do_hcalin_twr) HCALInner_Towers();
  if (do_hcalin_cluster) HCALInner_Clusters();

  if (do_hcalout_twr) HCALOuter_Towers();
  if (do_hcalout_cluster) HCALOuter_Clusters();

  // if enabled, do topoClustering early, upstream of any possible jet reconstruction
  if (do_topoCluster)
  {
    TopoClusterReco();
  }

  if (do_femc_twr) FEMC_Towers();
  if (do_femc_cluster) FEMC_Clusters();

  if (do_dst_compress) ShowerCompress();

  //--------------
  // SVTX tracking
  //--------------

  if (do_tracking_cluster) Tracking_Clus();

  if (do_tracking_track) Tracking_Reco();

  //-----------------
  // Global Vertexing
  //-----------------

  if (do_global)
  {
    Global_Reco();
  }

  else if (do_global_fastsim)
  {
    Global_FastSim();
  }

  //-----------------
  // Calo Trigger Simulation
  //-----------------

  if (do_calotrigger)
  {
    CaloTrigger_Sim();
  }

  //---------
  // Jet reco
  //---------

  if (do_jet_reco)
  {
    Jet_Reco();
  }

  if (do_HIjetreco)
  {
    HIJetReco();
  }

  if (do_particle_flow) {
    ParticleFlow();
  }

  //----------------------
  // Simulation evaluation
  //----------------------

  if (do_tracking_eval) Tracking_Eval(string(outputFile) + "_g4svtx_eval.root");

  if (Enable::CEMC_EVAL) CEMC_Eval(string(outputFile) + "_g4cemc_eval.root");

  if (do_hcalin_eval) HCALInner_Eval(string(outputFile) + "_g4hcalin_eval.root");

  if (do_hcalout_eval) HCALOuter_Eval(string(outputFile) + "_g4hcalout_eval.root");

  if (do_femc_eval) FEMC_Eval(string(outputFile) + "_g4femc_eval.root");

  if (do_jet_eval) Jet_Eval(string(outputFile) + "_g4jet_eval.root");

  //--------------
  // Set up Input Managers
  //--------------

  InputManagers();

  if (pileup_collision_rate > 0)
  {
    // pile up simulation.
    // add random beam collisions following a collision diamond and rate from a HepMC stream
    Fun4AllHepMCPileupInputManager *pileup = new Fun4AllHepMCPileupInputManager("HepMCPileupInput");
    se->registerInputManager(pileup);

    const string pileupfile("/sphenix/sim/sim01/sHijing/sHijing_0-12fm.dat");
    //background files for p+p pileup sim
    //const string pileupfile("/gpfs/mnt/gpfs04/sphenix/user/shlim/04.InnerTrackerTaskForce/01.PythiaGen/list_pythia8_mb.dat");
    pileup->AddFile(pileupfile);  // HepMC events used in pile up collisions. You can add multiple files, and the file list will be reused.
    //pileup->set_vertex_distribution_width(100e-4,100e-4,30,5);//override collision smear in space time
    //pileup->set_vertex_distribution_mean(0,0,0,0);//override collision central position shift in space time
    pileup->set_collision_rate(pileup_collision_rate);

    double time_window_minus = -35000;
    double time_window_plus = 35000;

    if (do_tracking)
    {
      // This gets the default drift velocity only! 
      PHG4TpcElectronDrift *dr = (PHG4TpcElectronDrift *)se->getSubsysReco("PHG4TpcElectronDrift");
      assert(dr);
      double TpcDriftVelocity = dr->get_double_param("drift_velocity");
      time_window_minus = -105.5 / TpcDriftVelocity;  // ns
      time_window_plus = 105.5 / TpcDriftVelocity;    // ns;
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
                /*bool*/ do_tracking,
                /*bool*/ do_pstof,
                /*bool*/ Enable::CEMC,
                /*bool*/ do_hcalin,
                /*bool*/ do_magnet,
                /*bool*/ do_hcalout,
                /*bool*/ Enable::CEMC_TOWER,
                /*bool*/ do_hcalin_twr,
                /*bool*/ do_hcalout_twr);
  }

  if(do_write_output) {
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
    if (do_dst_compress) DstCompress(out);
    se->registerOutputManager(out);
  }
  //-----------------
  // Event processing
  //-----------------
  if (nEvents < 0)
  {
    return 0;
  }
  // if we run the particle generator and use 0 it'll run forever
  if (nEvents == 0 && !Input::HEPMC && !Input::READHITS)
  {
    cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
    cout << "it will run forever, so I just return without running anything" << endl;
    return 0;
  }

  if(display_on)
    {
      DisplayOn();
      // prevent macro from finishing so can see display
      int i;
      cout << "***** Enter any integer to proceed" << endl;
      cin >> i;
    }

  se->run(nEvents);

  //-----
  // Exit
  //-----


  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
  return 0;
}
