#pragma once

#include "GlobalVariables.C"

#include "DisplayOn.C"
#include "G4Setup_fsPHENIX.C"
#include "G4_Bbc.C"
#include "G4_CaloTrigger.C"
#include "G4_DSTReader_fsPHENIX.C"
#include "G4_FwdJets.C"
#include "G4_Global.C"
#include "G4_Jets.C"
#include "G4_Input.C"

#include <g4detectors/PHG4DetectorSubsystem.h>

#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4ParticleGeneratorBase.h>
#include <g4main/PHG4ParticleGeneratorVectorMeson.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4SimpleEventGenerator.h>

#include <phhepmc/Fun4AllHepMCInputManager.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)

int Fun4All_G4_fsPHENIX(
    const int nEvents = 2,
    const char *inputFile = "/sphenix/sim/sim01/production/2016-07-21/single_particle/spacal2d/fieldmap/G4Hits_sPHENIX_e-_eta0_8GeV-0002.root",
    const char *outputFile = "G4fsPHENIX.root",
    const char *embed_input_file = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/fsPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root")
{
  //===============
  // Input options
  //===============
//  Input::SIMPLE = true;
  Input::SIMPLE_VERBOSITY = true;
  INPUTSIMPLE::AddParticle("pi-",1);
  INPUTSIMPLE::AddParticle("e-",0);
  INPUTSIMPLE::AddParticle("pi-",10);

//  Input::PYTHIA6 = true;
//  Input::PYTHIA8 = true;

//  Input::GUN = true;
  Input::GUN_VERBOSITY = 5;
  INPUTGUN::AddParticle("pi-",0,1,0);

  Input::HEPMC = true;
  Input::HEPMC_VERBOSITY = 1;
  INPUTHEPMC::filename=inputFile;

  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  const bool readhits = false;
  // Or:
  // Or:
  // Use particle generator
  // And
  // Further choose to embed newly simulated events to a previous simulation. Not compatible with `readhits = true`
  // In case embedding into a production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/
  const bool do_embedding = false;

// Initialize the selected Input
  InputInit();
  // Write the DST
  const bool do_write_output = true;
  const bool do_dst_compress = false;

  //Option to convert DST to human command readable TTree for quick poke around the outputs
  const bool do_DSTReader = false;

// turn the display on
  bool display_on = false;

  //======================
  // What to run
  //======================
// Global options (enabled for all enables subsystems - if implemented)
//  Enable::ABSORBER = true;
//  Enable::OVERLAPCHECK = true;
//  Enable::VERBOSITY = 1;
  int absorberactive = 0;  // set to 1 to make all absorbers active volumes

  bool do_bbc = false;

  Enable::PIPE = false;
  Enable::PIPE_ABSORBER = true;
//  Enable::PIPE_OVERLAPCHECK = false;
  // central tracking
  Enable::MVTX = false;
  bool do_mvtx_cell = Enable::MVTX && true;
  bool do_mvtx_cluster = do_mvtx_cell && true;

  Enable::INTT = false;
  bool do_intt_cell = Enable::INTT && true;
  bool do_intt_cluster = do_intt_cell && true;

  Enable::TPC = false;
//  Enable::TPC_ABSORBER = true;
  bool do_tpc_cell = Enable::TPC && true;
  bool do_tpc_cluster = do_tpc_cell && true;

  bool do_tracking_track = do_tpc_cell && do_intt_cell && do_mvtx_cell && true;
  bool do_tracking_eval = do_tracking_track && true;

  // central calorimeters, which is a detailed simulation and slow to run
  Enable::CEMC = false;
//  Enable::CEMC_ABSORBER = true;
  bool do_cemc_cell = Enable::CEMC && true;
  bool do_cemc_twr = do_cemc_cell && true;
  bool do_cemc_cluster = do_cemc_twr && true;
  bool do_cemc_eval = do_cemc_cluster && true;

  Enable::HCALIN = false;
//  Enable::HCALIN_ABSORBER = true;
  bool do_hcalin_cell = Enable::HCALIN && true;
  bool do_hcalin_twr = do_hcalin_cell && true;
  bool do_hcalin_cluster = do_hcalin_twr && true;
  bool do_hcalin_eval = do_hcalin_cluster && true;

  Enable::MAGNET = false;
//  Enable::MAGNET_ABSORBER = true;

  Enable::HCALOUT = false;
//  Enable::HCALOUT_ABSORBER = true;
  bool do_hcalout_cell = Enable::HCALOUT && true;
  bool do_hcalout_twr = do_hcalout_cell && true;
  bool do_hcalout_cluster = do_hcalout_twr && true;
  bool do_hcalout_eval = do_hcalout_cluster && true;

  bool do_global = false;
  bool do_global_fastsim = false;

  bool do_jet_reco = false;
  bool do_jet_eval = do_jet_reco && true;

  bool do_fwd_jet_reco = false;
  bool do_fwd_jet_eval = do_fwd_jet_reco && true;

  // fsPHENIX geometry

  Enable::FGEM = false;
  bool do_FGEM_track = Enable::FGEM && true;
  bool do_FGEM_eval = do_FGEM_track && true;

  Enable::FEMC = false;
//  Enable::FEMC_ABSORBER = true;
  bool do_FEMC_cell = Enable::FEMC && true;
  bool do_FEMC_twr = do_FEMC_cell && true;
  bool do_FEMC_cluster = do_FEMC_twr && true;

  Enable::FHCAL = false;
//  Enable::FHCAL_ABSORBER = true;
  bool do_FHCAL_cell = Enable::FHCAL && true;
  bool do_FHCAL_twr = do_FHCAL_cell && true;
  bool do_FHCAL_cluster = do_FHCAL_twr && true;

  Enable::PISTON = false;
//  Enable::PISTON_ABSORBER = true;
  Enable::PISTON_OVERLAPCHECK = false;

  Enable::PLUGDOOR = false;
//  Enable::PLUGDOOR_ABSORBER = true;
  Enable::PLUGDOOR_OVERLAPCHECK = false;

  // new settings using Enable namespace in GlobalVariables.C
  //  Enable::BLACKHOLE = true;
  BlackHoleGeometry::visible = true;

  // Initialize the selected subsystems
  G4Init();

  //  const string magfield = "1.5"; // alternatively to specify a constant magnetic field, give a float number, which will be translated to solenoidal field in T, if string use as fieldmap name (including path)
  const string magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sPHENIX.2d.root");  // default map from the calibration database
  const float magfield_rescale = -1.4 / 1.5;                                                         // make consistent with Fun4All_G4_sPHENIX()


  //---------------
  // Fun4All server
  //---------------


  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(0); // uncomment for batch production running with minimal output messages
  //  se->Verbosity(Fun4AllServer::VERBOSITY_SOME); // uncomment for some info for interactive running
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
  rc->set_IntFlag("RANDOMSEED", 12345);

  //-----------------
  // Event generation
  //-----------------
//  InputInit();

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
  }

  if (!readhits)
  {
    //---------------------
    // Detector description
    //---------------------

    G4Setup(magfield, EDecayType::kAll,
            magfield_rescale);
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

  if (do_mvtx_cell) Mvtx_Cells();
  if (do_intt_cell) Intt_Cells();
  if (do_tpc_cell) TPC_Cells();

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
  if (do_mvtx_cluster) Mvtx_Clustering();
  if (do_intt_cluster) Intt_Clustering();
  if (do_tpc_cluster) TPC_Clustering();

  if (do_tracking_track) Tracking_Reco();

  //--------------
  // FGEM tracking
  //--------------

  if (do_FGEM_track) FGEM_FastSim_Reco();

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

  //---------
  // Jet reco
  //---------

  if (do_jet_reco)
  {
    Jet_Reco();
  }

  if (do_fwd_jet_reco)
  {
    Jet_FwdReco();
  }
  //----------------------
  // Simulation evaluation
  //----------------------

  if (do_tracking_eval) Tracking_Eval("g4tracking_eval.root");

  if (do_cemc_eval) CEMC_Eval("g4cemc_eval.root");

  if (do_hcalin_eval) HCALInner_Eval("g4hcalin_eval.root");

  if (do_hcalout_eval) HCALOuter_Eval("g4hcalout_eval.root");

  if (do_jet_eval) Jet_Eval("g4jet_eval.root");

  if (do_fwd_jet_eval) Jet_FwdEval("g4fwdjet_eval.root");

  if (do_FGEM_eval) FGEM_FastSim_Eval("g4tracking_fgem_eval.root");

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
    //meta-lib for DST objects used in simulation outputs
    gSystem->Load("libg4dst.so");

    Fun4AllDstInputManager *in1 = new Fun4AllNoSyncDstInputManager("DSTinEmbed");
    in1->AddFile(embed_input_file);  // if one use a single input file
                                     //    in1->AddListFile(embed_input_file);  // Recommended: if one use a text list of many input files
    in1->Repeat();                   // if file(or filelist) is exhausted, start from beginning
    se->registerInputManager(in1);
  }
    // for single particle generators we just need something which drives
    // the event loop, the Dummy Input Mgr does just that
//      Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
//    se->registerInputManager(in);

  if (do_DSTReader)
  {
    //Convert DST to human command readable TTree for quick poke around the outputs
    G4DSTreader_fsPHENIX(outputFile,  //
                         /*int*/ absorberactive,
                         /*bool*/ Enable::CEMC,
                         /*bool*/ Enable::HCALIN,
                         /*bool*/ Enable::MAGNET,
                         /*bool*/ Enable::HCALOUT,
                         /*bool*/ do_cemc_twr,
                         /*bool*/ do_hcalin_twr,
                         /*bool*/ do_hcalout_twr,
                         /*bool*/ Enable::FGEM,
                         /*bool*/ Enable::FHCAL,
                         /*bool*/ do_FHCAL_twr,
                         /*bool*/ Enable::FEMC,
                         /*bool*/ do_FEMC_twr);
  }

  InputManagers();

  if (do_write_output)
  {
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
  if (nEvents == 0 && !readhits && !Input::HEPMC)
  {
    cout << "using 0 for number of events is a bad idea when using particle generators" << endl;
    cout << "it will run forever, so I just return without running anything" << endl;
    return 0;
  }

  if (display_on)
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

void G4Cmd(const char *cmd)
{
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4Reco *g4 = (PHG4Reco *) se->getSubsysReco("PHG4RECO");
  g4->ApplyCommand(cmd);
}

void RunFFALoadTest() {}
