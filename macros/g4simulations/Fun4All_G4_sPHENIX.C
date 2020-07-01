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

  //======================
  // Write the DST
  //======================

  Enable::DSTOUT = true;
  Enable::DSTOUT_COMPRESS = false;
  //Option to convert DST to human command readable TTree for quick poke around the outputs
  //  Enable::DSTREADER = true;

  // turn the display on (default off)
  Enable::DISPLAY = false;

  // Event pile up simulation with collision rate in Hz MB collisions.
  // Note please follow up the macro to verify the settings for beam parameters
  const double pileup_collision_rate = 0;  // 100e3 for 100kHz nominal AuAu collision rate.
  // To write cluster files set do_write_output = true and set 
  // do_tracking = true, do_tracking_cell = true, do_tracking_cluster = true and 
  // leave the tracking for later do_tracking_track =  false,  do_tracking_eval = false

  //======================
  // What to run
  //======================

//  Enable::BBC = true;

  Enable::PIPE = true;
  Enable::PIPE_ABSORBER = true;

  bool do_tracking = false;
  bool do_tracking_cell = do_tracking && false;
  bool do_tracking_cluster = do_tracking_cell && true;
  bool do_tracking_track = do_tracking_cluster && true;
  bool do_tracking_eval = do_tracking_track && true;

  Enable::PSTOF = true;

  Enable::CEMC = true;
  Enable::CEMC_ABSORBER = true;
  Enable::CEMC_CELL = Enable::CEMC && true;
  Enable::CEMC_TOWER = Enable::CEMC_CELL && false;
  Enable::CEMC_CLUSTER = Enable::CEMC_TOWER && true;
  Enable::CEMC_EVAL = Enable::CEMC_CLUSTER && true;

  //Enable::HCALIN = true;
  //  Enable::HCALIN_ABSORBER = true;
  Enable::HCALIN_CELL = Enable::HCALIN && true;
  Enable::HCALIN_TOWER = Enable::HCALIN_CELL && true;
  Enable::HCALIN_CLUSTER = Enable::HCALIN_TOWER && true;
  Enable::HCALIN_EVAL = Enable::HCALIN_CLUSTER && true;

  Enable::MAGNET = true;
  Enable::MAGNET_ABSORBER = true;

//  Enable::HCALOUT = true;
  Enable::HCALOUT_ABSORBER = true;
  Enable::HCALOUT_CELL = Enable::HCALOUT && true;
  Enable::HCALOUT_TOWER = Enable::HCALOUT_CELL && true;
  Enable::HCALOUT_CLUSTER = Enable::HCALOUT_TOWER && true;
  Enable::HCALOUT_EVAL = Enable::HCALOUT_CLUSTER && true;

  // forward EMC
  Enable::FEMC = true;
  Enable::FEMC_ABSORBER = true;
  Enable::FEMC_CELL = Enable::FEMC && true;
  Enable::FEMC_TOWER = Enable::FEMC_CELL && true;
  Enable::FEMC_CLUSTER = Enable::FEMC_TOWER && true;
  Enable::FEMC_EVAL = Enable::FEMC_CLUSTER && true;

  //! forward flux return plug door. Out of acceptance and off by default.
  Enable::PLUGDOOR = true;
  Enable::PLUGDOOR_ABSORBER = true;

  bool do_global = false;
  bool do_global_fastsim = false;

  bool do_calotrigger = true && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER;

  bool do_jet_reco = false;
  bool do_jet_eval = do_jet_reco && true;

  // HI Jet Reco for p+Au / Au+Au collisions (default is false for
  // single particle / p+p-only simulations, or for p+Au / Au+Au
  // simulations which don't particularly care about jets)
  bool do_HIjetreco = false && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER;

  // 3-D topoCluster reconstruction, potentially in all calorimeter layers
  bool do_topoCluster = false && Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER;
  // particle flow jet reconstruction - needs topoClusters!
  bool do_particle_flow = false && do_topoCluster;

  G4Init(do_tracking);

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

    G4Setup(absorberactive, magfield, EDecayType::kAll,
            do_tracking, magfield_rescale);
  }

  //---------
  // BBC Reco
  //---------

  if (Enable::BBC)
  {
    BbcInit();
    Bbc_Reco();
  }
  //------------------
  // Detector Division
  //------------------

  if (do_tracking_cell) Tracking_Cells();

  if (Enable::CEMC_CELL) CEMC_Cells();

  if (Enable::HCALIN_CELL) HCALInner_Cells();

  if (Enable::HCALOUT_CELL) HCALOuter_Cells();

  if (Enable::FEMC_CELL) FEMC_Cells();

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

  // if enabled, do topoClustering early, upstream of any possible jet reconstruction
  if (do_topoCluster)
  {
    TopoClusterReco();
  }

  if (Enable::FEMC_TOWER) FEMC_Towers();
  if (Enable::FEMC_CLUSTER) FEMC_Clusters();

  if (Enable::DSTOUT_COMPRESS) ShowerCompress();

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
  string outputroot = outputFile;
  string remove_this = ".root";
  size_t pos = outputroot.find(remove_this);
  if (pos != string::npos)
  {
    outputroot.erase(pos, remove_this.length());
  }

  if (do_tracking_eval) Tracking_Eval(outputroot + "_g4svtx_eval.root");

  if (Enable::CEMC_EVAL) CEMC_Eval(outputroot + "_g4cemc_eval.root");

  if (Enable::HCALIN_EVAL) HCALInner_Eval(outputroot + "_g4hcalin_eval.root");

  if (Enable::HCALOUT_EVAL) HCALOuter_Eval(outputroot + "_g4hcalout_eval.root");

  if (Enable::FEMC_EVAL) FEMC_Eval(outputroot + "_g4femc_eval.root");

  if (do_jet_eval) Jet_Eval(outputroot + "_g4jet_eval.root");

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

  if (Enable::DSTREADER)
  {
    //Convert DST to human command readable TTree for quick poke around the outputs
    gROOT->LoadMacro("G4_DSTReader.C");

    G4DSTreader(outputFile,  //
                /*int*/ absorberactive,
                /*bool*/ do_tracking,
                /*bool*/ Enable::PSTOF,
                /*bool*/ Enable::CEMC,
                /*bool*/ Enable::HCALIN,
                /*bool*/ Enable::MAGNET,
                /*bool*/ Enable::HCALOUT,
                /*bool*/ Enable::CEMC_TOWER,
                /*bool*/ Enable::HCALIN_TOWER,
                /*bool*/ Enable::HCALOUT_TOWER);
  }

  if(Enable::DSTOUT) {
    Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outputFile);
    if (Enable::DSTOUT_COMPRESS) DstCompress(out);
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
