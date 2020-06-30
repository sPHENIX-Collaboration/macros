#pragma once

#include "GlobalVariables.C"

#include "DisplayOn.C"
#include "G4Setup_fsPHENIX.C"
#include "G4_Bbc.C"
#include "G4_CaloTrigger.C"
#include "G4_DSTReader_fsPHENIX.C"
#include "G4_FwdJets.C"
#include "G4_Global.C"
#include "G4_Input.C"
#include "G4_Jets.C"

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)

// If using the default embedding file results in a error, try
// TFile *f1 = TFile::Open("http://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/fsPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root")
// if it returns a certificate error, something like
// Error in <DavixOpen>: can not open file with davix: Failure (Neon): Server certificate verification failed: bad certificate chain after 3 attempts (6)
// add the line
// Davix.GSI.CACheck: n
// to your .rootrc

int Fun4All_G4_fsPHENIX(
    const int nEvents = 2,
    const char *inputFile = "/sphenix/sim/sim01/production/2016-07-21/single_particle/spacal2d/fieldmap/G4Hits_sPHENIX_e-_eta0_8GeV-0002.root",
    const char *outputFile = "G4fsPHENIX.root",
    const char *embed_input_file = "https://www.phenix.bnl.gov/WWW/publish/phnxbld/sPHENIX/files/fsPHENIX_G4Hits_sHijing_9-11fm_00000_00010.root")
{
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
  Input::READHITS = false;
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
  INPUTSIMPLE::set_eta_range(-1, 3);
  INPUTSIMPLE::set_phi_range(-M_PI, M_PI);
  INPUTSIMPLE::set_pt_range(0.5, 50.);
  INPUTSIMPLE::set_vtx_mean(0., 0., 0.);
  INPUTSIMPLE::set_vtx_width(0., 0., 5.);

  //  Input::PYTHIA6 = true;

  //  Input::PYTHIA8 = true;

  //  Input::GUN = true;
  //Input::GUN_VERBOSITY = 0;
  INPUTGUN::AddParticle("pi-", 0, 1, 0);
  //INPUTGUN::set_vtx(0,0,0);

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

  //======================
  // What to run
  //======================
  // Global options (enabled for all enables subsystems - if implemented)
  //  Enable::ABSORBER = true;
  //  Enable::OVERLAPCHECK = true;
  //  Enable::VERBOSITY = 1;

  Enable::BBC = true;

  Enable::PIPE = true;
  Enable::PIPE_ABSORBER = true;
  //  Enable::PIPE_OVERLAPCHECK = false;

  // central tracking
  Enable::MVTX = true;
  Enable::MVTX_CELL = Enable::MVTX && true;
  Enable::MVTX_CLUSTER = Enable::MVTX_CELL && true;

  Enable::INTT = true;
  Enable::INTT_CELL = Enable::INTT && true;
  Enable::INTT_CLUSTER = Enable::INTT_CELL && true;

  Enable::TPC = true;
  //  Enable::TPC_ABSORBER = true;
  Enable::TPC_CELL = Enable::TPC && true;
  Enable::TPC_CLUSTER = Enable::TPC_CELL && true;

  Enable::TRACKING_TRACK = Enable::TPC_CELL && Enable::INTT_CELL && Enable::MVTX_CELL && true;
  Enable::TRACKING_EVAL = Enable::TRACKING_TRACK && true;

  // central calorimeters, which is a detailed simulation and slow to run
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
  //  Enable::MAGNET_ABSORBER = true;

  Enable::HCALOUT = true;
  //  Enable::HCALOUT_ABSORBER = true;
  Enable::HCALOUT_CELL = Enable::HCALOUT && true;
  Enable::HCALOUT_TOWER = Enable::HCALOUT_CELL && true;
  Enable::HCALOUT_CLUSTER = Enable::HCALOUT_TOWER && true;
  Enable::HCALOUT_EVAL = Enable::HCALOUT_CLUSTER && true;

  Enable::GLOBAL_RECO = true;
  Enable::GLOBAL_FASTSIM = true;

  Enable::CALOTRIGGER = Enable::CEMC_TOWER && Enable::HCALIN_TOWER && Enable::HCALOUT_TOWER && true;

  Enable::JETS = true;
  Enable::JETS_EVAL = Enable::JETS && true;

  Enable::FWDJETS = true;
  Enable::FWDJETS_EVAL = Enable::FWDJETS && true;

  // fsPHENIX geometry

  Enable::FGEM = true;
  Enable::FGEM_TRACK = Enable::FGEM && Enable::MVTX && true;
  Enable::FGEM_EVAL = Enable::FGEM_TRACK && true;

  Enable::FEMC = true;
  Enable::FEMC_ABSORBER = true;
  Enable::FEMC_CELL = Enable::FEMC && true;
  Enable::FEMC_TOWER = Enable::FEMC_CELL && true;
  Enable::FEMC_CLUSTER = Enable::FEMC_TOWER && true;

  Enable::FHCAL = true;
  Enable::FHCAL_ABSORBER = true;
  Enable::FHCAL_CELL = Enable::FHCAL && true;
  Enable::FHCAL_TOWER = Enable::FHCAL_CELL && true;
  Enable::FHCAL_CLUSTER = Enable::FHCAL_TOWER && true;
  Enable::FHCAL_EVAL = Enable::FHCAL_CLUSTER && true;

  //  Enable::PISTON = true;
  Enable::PISTON_ABSORBER = true;
  Enable::PISTON_OVERLAPCHECK = false;

  Enable::PLUGDOOR = true;
  //  Enable::PLUGDOOR_ABSORBER = true;
  Enable::PLUGDOOR_OVERLAPCHECK = false;

  // new settings using Enable namespace in GlobalVariables.C
  Enable::BLACKHOLE = true;
  //  BlackHoleGeometry::visible = true;

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

  if (!Input::READHITS)
  {
    //---------------------
    // Set up G4 only if we do not read hits
    //---------------------
    G4Setup();
  }

  //---------
  // BBC Reco, just smeared vertex
  //---------

  if (Enable::BBC)
  {
    BbcInit();
    Bbc_Reco();
  }

  //------------------
  // Detector Division
  //------------------

  if (Enable::MVTX_CELL) Mvtx_Cells();
  if (Enable::INTT_CELL) Intt_Cells();
  if (Enable::TPC_CELL) TPC_Cells();

  if (Enable::CEMC_CELL) CEMC_Cells();

  if (Enable::HCALIN_CELL) HCALInner_Cells();

  if (Enable::HCALOUT_CELL) HCALOuter_Cells();

  if (Enable::FEMC_CELL) FEMC_Cells();
  if (Enable::FHCAL_CELL) FHCAL_Cells();

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

  if (Enable::FEMC_TOWER) FEMC_Towers();
  if (Enable::FEMC_CLUSTER) FEMC_Clusters();

  if (Enable::FHCAL_TOWER) FHCAL_Towers();
  if (Enable::FHCAL_CLUSTER) FHCAL_Clusters();

  if (Enable::DSTOUT_COMPRESS) ShowerCompress();

  //--------------
  // SVTX tracking
  //--------------
  if (Enable::MVTX_CLUSTER) Mvtx_Clustering();
  if (Enable::INTT_CLUSTER) Intt_Clustering();
  if (Enable::TPC_CLUSTER) TPC_Clustering();

  if (Enable::TRACKING_TRACK) Tracking_Reco();

  //--------------
  // FGEM tracking
  //--------------

  if (Enable::FGEM_TRACK) FGEM_FastSim_Reco();

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

  if (Enable::CALOTRIGGER)
  {
    CaloTrigger_Sim();
  }

  //---------
  // Jet reco
  //---------

  if (Enable::JETS) Jet_Reco();

  if (Enable::FWDJETS) Jet_FwdReco();

  //----------------------
  // Simulation evaluation
  //----------------------

  if (Enable::TRACKING_EVAL) Tracking_Eval("g4tracking_eval.root");

  if (Enable::CEMC_EVAL) CEMC_Eval("g4cemc_eval.root");

  if (Enable::HCALIN_EVAL) HCALInner_Eval("g4hcalin_eval.root");

  if (Enable::HCALOUT_EVAL) HCALOuter_Eval("g4hcalout_eval.root");

  if (Enable::FHCAL_EVAL) FHCAL_Eval(string(outputFile) + "_g4fhcal_eval.root");

  if (Enable::JETS_EVAL) Jet_Eval("g4jet_eval.root");

  if (Enable::FWDJETS_EVAL) Jet_FwdEval("g4fwdjet_eval.root");

  if (Enable::FGEM_EVAL) FGEM_FastSim_Eval("g4tracking_fgem_eval.root");

  if (Enable::DSTREADER) G4DSTreader_fsPHENIX(outputFile);

  //--------------
  // Set up Input Managers
  //--------------

  InputManagers();

  //--------------
  // Set up Output Managers
  //--------------

  if (Enable::DSTOUT)
  {
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

  if (Enable::DISPLAY)
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
