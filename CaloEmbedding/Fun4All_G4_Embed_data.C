#ifndef MACRO_FUN4ALLG4EMBED_C
#define MACRO_FUN4ALLG4EMBED_C

#include <GlobalVariables.C>

#include "G4Setup_sPHENIX.C"
#include <G4_Input.C>
#include <G4_Production.C>
#include <globalvertex/GlobalVertexReco.h>

#include <phpythia8/PHPy8JetTrigger.h>
#include <caloembedding/HepMCCollisionVertex.h>
#include <caloembedding/caloTowerEmbed.h>
#include <caloembedding/CopyIODataNodes.h>

#include <jetbase/JetReco.h>
#include <jetbase/TowerJetInput.h>
#include <jetbase/FastJetAlgo.h>

#include <jetbackground/FastJetAlgoSub.h>
#include <jetbackground/RetowerCEMC.h>

#include <g4jets/TruthJetInput.h>

#include <centrality/CentralityInfov2.h>
#include <calotrigger/MinimumBiasInfov1.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllSyncManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllUtils.h>

#include <phool/PHRandomSeed.h>
#include <phool/recoConsts.h>

#include <TRandom3.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libCaloEmbedding.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libjetbase.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libg4jets.so)
// R__LOAD_LIBRARY(libg4dst.so)
//  For HepMC Hijing
//  try inputFile = /sphenix/sim/sim01/sphnxpro/sHijing_HepMC/sHijing_0-12fm.dat

// namespace OUTPUTMANAGER
//{
// set<std::string> outfiles;
// }

void AddCommonNodes(Fun4AllOutputManager *out);

int Fun4All_G4_Embed_data(
    const int nEvents = 1000,
    const int segment = 00000,
    const std::string &embed_input_file0 = "/sphenix/u/bseidlitz/work/devMac/macros/CaloProduction/condor/test.root",
    const std::string &outdir = "./",
    const std::string &outnameEnd = "embed_test.root",
    const std::string &jettrigger = "Jet30",
    const std::string &cdbtag = "MDC2")
{

  std::cout << "segment: " << segment << std::endl;

  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

  int verbosity = 0;

  // Opt to print all random seed used for debugging reproducibility. Comment out to reduce stdout prints.
  PHRandomSeed::Verbosity(1);

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
  //  rc->set_IntFlag("RANDOMSEED", 12345);

  //===============
  // conditions DB flags
  //===============
  Enable::CDB = true;
  // tag
  rc->set_StringFlag("CDB_GLOBALTAG", cdbtag);
  // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP", CDB::timestamp);

  //  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(embed_input_file0);
  // int runnumber = runseg.first;
  // int segment = runseg.second;
  int runnumber = 54912;
  if (runnumber != 0)
  {
    rc->set_IntFlag("RUNNUMBER", runnumber);
    Fun4AllSyncManager *syncman = se->getSyncManager();
    syncman->SegmentNumber(segment);
  }

  // Sync Headers and Flags
  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  //  G4CEMC::useTowerInfoV2 = false;
  //  G4HCALOUT::useTowerInfoV2 = false;
  //  G4HCALIN::useTowerInfoV2 = false;

  //  G4CEMC::useTowerInfoV4 = true;
  // G4HCALOUT::useTowerInfoV4 = true;
  // G4HCALIN::useTowerInfoV4 = true;

  //===============
  // Input options
  //===============
  // verbosity setting (applies to all input managers)
  Input::VERBOSITY = 0;
  // First enable the input generators
  // Either:
  // read previously generated g4-hits files, in this case it opens a DST and skips
  // the simulations step completely. The G4Setup macro is only loaded to get information
  // about the number of layers used for the cell reco code
  //  Input::READHITS = true;
  // INPUTREADHITS::filename[0] = inputFile;
  // if you use a filelist
  // INPUTREADHITS::listfile[0] = inputFile;
  // Or:
  // Use particle generator
  // And
  // Further choose to embed newly simulated events to a previous simulation. Not compatible with `readhits = true`
  // In case embedding into a production output, please double check your G4Setup_sPHENIX.C and G4_*.C consistent with those in the production macro folder
  // E.g. /sphenix/sim//sim01/production/2016-07-21/single_particle/spacal2d/

  Input::EMBED = true;
  INPUTEMBED::filename[0] = embed_input_file0;
  // no repeating of embedding background, stop processing when end of file reached
  INPUTEMBED::REPEAT = false;

  // if you use a filelist
  // INPUTEMBED::listfile[0] = embed_input_file;

  // Input::SIMPLE = true;
  // Input::SIMPLE_NUMBER = 2; // if you need 2 of them
  // Input::SIMPLE_VERBOSITY = 1;

  //  Input::PYTHIA6 = true;
  // Enable this is emulating the nominal pp/pA/AA collision vertex distribution
  Input::BEAM_CONFIGURATION = Input::AA_COLLISION; // for 2023 sims we want the AA geometry for no pileup sims

  Input::PYTHIA8 = true;
  if (Input::PYTHIA8)
  {
    std::string pythia8_config_file = std::string(getenv("CALIBRATIONROOT")) + "/Generators/JetStructure_TG/";
    std::cout << "pythia config path: " << pythia8_config_file << std::endl;
    if (jettrigger == "Jet10")
    {
      pythia8_config_file += "phpythia8_15GeV_JS_MDC2.cfg";
    }
    else if (jettrigger == "Jet30")
    {
      pythia8_config_file += "phpythia8_30GeV_JS_MDC2.cfg";
    }
    else if (jettrigger == "Jet40")
    {
      pythia8_config_file += "phpythia8_40GeV_JS_MDC2.cfg";
    }
    else if (jettrigger == "PhotonJet")
    {
      pythia8_config_file += "phpythia8_JS_GJ_MDC2.cfg";
    }
    else
    {
      std::cout << "invalid jettrigger: " << jettrigger << std::endl;
      gSystem->Exit(1);
    }
    PYTHIA8::config_file = pythia8_config_file;
  }

  //-----------------
  // Initialize the selected Input/Event generation
  //-----------------
  InputInit();

  //--------------
  // Set generator specific options
  //--------------
  // can only be set after InputInit() is called

  if (Input::PYTHIA8)
  {
    //! apply sPHENIX nominal beam parameter with 2mrad crossing as defined in sPH-TRG-2020-001
    PHPy8JetTrigger *p8_js_signal_trigger = new PHPy8JetTrigger();
    p8_js_signal_trigger->SetEtaHighLow(1.5, -1.5); // Set eta acceptance for particles into the jet between +/- 1.5
    p8_js_signal_trigger->SetJetR(0.4);             // Set the radius for the trigger jet
    if (jettrigger == "Jet10")
    {
      p8_js_signal_trigger->SetMinJetPt(10); // require a 10 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet30")
    {
      p8_js_signal_trigger->SetMinJetPt(30); // require a 30 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet40")
    {
      p8_js_signal_trigger->SetMinJetPt(40); // require a 40 GeV minimum pT jet in the event
    }
    else if (jettrigger == "PhotonJet")
    {
      delete p8_js_signal_trigger;
      p8_js_signal_trigger = nullptr;
      std::cout << "no cut for PhotonJet" << std::endl;
    }
    else
    {
      std::cout << "invalid jettrigger: " << jettrigger << std::endl;
      gSystem->Exit(1);
    }
    if (p8_js_signal_trigger)
    {
      INPUTGENERATOR::Pythia8->register_trigger(p8_js_signal_trigger);
      INPUTGENERATOR::Pythia8->set_trigger_AND();
    }
    Input::ApplysPHENIXBeamParameter(INPUTGENERATOR::Pythia8);
  }

  //--------------
  // Set Input Manager specific options
  //--------------
  // can only be set after InputInit() is called

  // register all input generators with Fun4All

  std::string DataTopNode = "TOPData";
  /*
  EventSelector *EventSelectorHandler = new EventSelector();
  EventSelectorHandler->Verbosity( 1 );
  auto mbc = new MinBiasCut( );
  mbc->SetNodeName("MinimumBiasInfo");
  EventSelectorHandler->AddCut(mbc);

  auto tcc = new TowerChi2Cut( );
  std::vector<std::string> TowerChi2Nodes = {
    "TOWERINFO_CALIB_CEMC",
    "TOWERINFO_CALIB_HCALIN",
    "TOWERINFO_CALIB_HCALOUT"
  };
  tcc->SetNodeNames(TowerChi2Nodes);
  EventSelectorHandler->AddCut( tcc );

  auto zvc =  new ZVertexCut( 20, -20 );
  zvc->SetNodeName( "GlobalVertexMap" );
  EventSelectorHandler->AddCut( zvc );

  EventSelectorHandler->PrintCuts();
  se->registerSubsystem( EventSelectorHandler, DataTopNode);

  */

  HepMCCollisionVertex *collvtx = new HepMCCollisionVertex();
  collvtx->Verbosity(0);
  se->registerSubsystem(collvtx);

  InputRegister();

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  // set up production relatedstuff
  Enable::PRODUCTION = true;

  //======================
  // Write the DST
  //======================

  Enable::DSTOUT = true;
  Enable::DSTOUT_COMPRESS = false;
  DstOut::OutputDir = outdir;

  if (Enable::PRODUCTION)
  {
    PRODUCTION::SaveOutputDir = DstOut::OutputDir;
    //    Production_CreateOutputDir();
  }


  //-----------------
  // Copy over RunHeader, EventHeader, centrality, globalvertex, and MinimumBiasInfo nodes
  //-----------------
  CopyIODataNodes *cp = new CopyIODataNodes();
  cp->Verbosity(0);
  se->registerSubsystem(cp, DataTopNode);

  //======================
  // What to run
  //======================

  // Global options (enabled for all enables subsystems - if implemented)
  //  Enable::ABSORBER = true;
  //  Enable::OVERLAPCHECK = true;
  //  Enable::VERBOSITY = 1;

  Enable::MBD = true;

  // Enable::PIPE = true;

  // central tracking
  // Enable::MVTX = true;

  // Enable::INTT = true;

  // Enable::TPC = true;

  // Enable::MICROMEGAS = true;

  Enable::CEMC = true;
  // Enable::CEMC_ABSORBER = true;
  // Enable::CEMC_CELL = Enable::CEMC && true;
  // Enable::CEMC_TOWER = Enable::CEMC_CELL && true;

  Enable::HCALIN = true;
  // Enable::HCALIN_ABSORBER = true;
  // Enable::HCALIN_CELL = Enable::HCALIN && true;
  // Enable::HCALIN_TOWER = Enable::HCALIN_CELL && true;

  Enable::MAGNET = true;
  // Enable::MAGNET_ABSORBER = true;

  Enable::HCALOUT = true;
  // Enable::HCALOUT_ABSORBER = true;
  // Enable::HCALOUT_CELL = Enable::HCALOUT && true;
  // Enable::HCALOUT_TOWER = Enable::HCALOUT_CELL && true;

  Enable::EPD = true;

  // new settings using Enable namespace in GlobalVariables.C
  Enable::BLACKHOLE = true;
  Enable::BLACKHOLE_FORWARD_SAVEHITS = false; // disable forward/backward hits
  // Enable::BLACKHOLE_SAVEHITS = false; // turn off saving of bh hits
  // BlackHoleGeometry::visible = true;

  G4MAGNET::magfield = "FIELDMAP_GAP";
  G4MAGNET::magfield_OHCAL_steel = "FIELDMAP_STEEL";
  G4MAGNET::magfield_tracking = "FIELDMAP_TRACKING";

  // G4MAGNET::magfield = "/cvmfs/sphenix.sdcc.bnl.gov/calibrations/sphnxpro/cdb/FIELDMAP_GAP/65/a9/65a930ed6de9c0e049cd0f3ef226e6b4_sphenix3dbigmapxyz_gap_rebuild_v2.root";
  // G4MAGNET::magfield_OHCAL_steel = "/cvmfs/sphenix.sdcc.bnl.gov/calibrations/sphnxpro/cdb/FIELDMAP_STEEL/ea/f0/eaf07c2a2def280428b9aeea99d14fd1_sphenix3dbigmapxyz_steel_rebuild_v2.root";
  // G4MAGNET::magfield_tracking = "/cvmfs/sphenix.sdcc.bnl.gov/calibrations/sphnxpro/cdb/FIELDMAP_TRACKING/8e/4d/8e4d6c3b1660540a658da3a275af2bde_sphenix3dtrackingmapxyz.root";

  // Initialize the selected subsystems
  G4Init();

  //---------------------
  // GEANT4 Detector description
  //---------------------
  if (!Input::READHITS)
  {
    G4Setup();
  }

  //--------------------
  // GEANT4 Calorimeters
  //--------------------

  if (Enable::CEMC_CELL)
    CEMC_Cells();

  if (Enable::HCALIN_CELL)
    HCALInner_Cells();

  if (Enable::HCALOUT_CELL)
    HCALOuter_Cells();

  if (Enable::CEMC_TOWER)
    CEMC_Towers();

  if (Enable::HCALIN_TOWER)
    HCALInner_Towers();

  if (Enable::HCALOUT_TOWER)
    HCALOuter_Towers();

  //truth jets
  JetReco *truthjets2 = new JetReco("TRUTHJETRECO2");
  truthjets2->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjets2->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_Truth_r02");
  truthjets2->set_algo_node("ANTIKT");
  truthjets2->set_input_node("TRUTH");
  truthjets2->Verbosity(verbosity);
  se->registerSubsystem(truthjets2);

  JetReco *truthjets3 = new JetReco("TRUTHJETRECO3");
  truthjets3->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjets3->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_Truth_r03");
  truthjets3->set_algo_node("ANTIKT");
  truthjets3->set_input_node("TRUTH");
  truthjets3->Verbosity(verbosity);
  se->registerSubsystem(truthjets3);

  JetReco *truthjets4 = new JetReco("TRUTHJETRECO4");
  truthjets4->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjets4->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_Truth_r04");
  truthjets4->set_algo_node("ANTIKT");
  truthjets4->set_input_node("TRUTH");
  truthjets4->Verbosity(verbosity);
  se->registerSubsystem(truthjets4);

  JetReco *truthjets5 = new JetReco("TRUTHJETRECO5");
  truthjets5->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjets5->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.5), "AntiKt_Truth_r05");
  truthjets5->set_algo_node("ANTIKT");
  truthjets5->set_input_node("TRUTH");
  truthjets5->Verbosity(verbosity);
  se->registerSubsystem(truthjets5);

  JetReco *truthjets6 = new JetReco("TRUTHJETRECO6");
  truthjets6->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjets6->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.6), "AntiKt_Truth_r06");
  truthjets6->set_algo_node("ANTIKT");
  truthjets6->set_input_node("TRUTH");
  truthjets6->Verbosity(verbosity);
  se->registerSubsystem(truthjets6);

  JetReco *truthjets7 = new JetReco("TRUTHJETRECO7");
  truthjets7->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjets7->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.7), "AntiKt_Truth_r07");
  truthjets7->set_algo_node("ANTIKT");
  truthjets7->set_input_node("TRUTH");
  truthjets7->Verbosity(verbosity);
  se->registerSubsystem(truthjets7);

  JetReco *truthjets8 = new JetReco("TRUTHJETRECO8");
  truthjets8->add_input(new TruthJetInput(Jet::PARTICLE));
  truthjets8->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.8), "AntiKt_Truth_r08");
  truthjets8->set_algo_node("ANTIKT");
  truthjets8->set_input_node("TRUTH");
  truthjets8->Verbosity(verbosity);
  se->registerSubsystem(truthjets8);

  // waveform sim
  CaloWaveformSim *caloWaveformSim = new CaloWaveformSim();
  caloWaveformSim->set_detector_type(CaloTowerDefs::CEMC);
  caloWaveformSim->set_detector("CEMC");
  caloWaveformSim->set_nsamples(12);
  caloWaveformSim->set_pedestalsamples(12);
  caloWaveformSim->set_timewidth(0.2);
  caloWaveformSim->set_peakpos(6);
  caloWaveformSim->set_noise_type(CaloWaveformSim::NOISE_NONE);
  se->registerSubsystem(caloWaveformSim);

  caloWaveformSim = new CaloWaveformSim();
  caloWaveformSim->set_detector_type(CaloTowerDefs::HCALIN);
  caloWaveformSim->set_detector("HCALIN");
  caloWaveformSim->set_nsamples(12);
  caloWaveformSim->set_pedestalsamples(12);
  caloWaveformSim->set_timewidth(0.2);
  caloWaveformSim->set_peakpos(6);
  caloWaveformSim->set_noise_type(CaloWaveformSim::NOISE_NONE);
  se->registerSubsystem(caloWaveformSim);

  caloWaveformSim = new CaloWaveformSim();
  caloWaveformSim->set_detector_type(CaloTowerDefs::HCALOUT);
  caloWaveformSim->set_detector("HCALOUT");
  caloWaveformSim->set_nsamples(12);
  caloWaveformSim->set_pedestalsamples(12);
  caloWaveformSim->set_timewidth(0.2);
  caloWaveformSim->set_peakpos(6);
  caloWaveformSim->set_noise_type(CaloWaveformSim::NOISE_NONE);
  se->registerSubsystem(caloWaveformSim);

  se->Print("NODETREE");

  //-----------------
  // Calorimeter embedding
  //-----------------

  caloTowerEmbed *embedder_CEMC = new caloTowerEmbed("embedder_CEMC");
  embedder_CEMC->set_detector_type(CaloTowerDefs::CEMC);
  embedder_CEMC->set_removeBadTowers(false);
  embedder_CEMC->set_embedwaveform(true);
  embedder_CEMC->set_inputNodePrefix("TOWERS_");
  embedder_CEMC->Verbosity(verbosity);
  se->registerSubsystem(embedder_CEMC);

  caloTowerEmbed *embedder_IHCAL = new caloTowerEmbed("embedder_IHCAL");
  embedder_IHCAL->set_detector_type(CaloTowerDefs::HCALIN);
  embedder_IHCAL->set_removeBadTowers(false);
  embedder_IHCAL->set_embedwaveform(true);
  embedder_IHCAL->set_inputNodePrefix("TOWERS_");
  embedder_IHCAL->Verbosity(verbosity);
  se->registerSubsystem(embedder_IHCAL);

  caloTowerEmbed *embedder_OHCAL = new caloTowerEmbed("embedder_OHCal");
  embedder_OHCAL->set_detector_type(CaloTowerDefs::HCALOUT);
  embedder_OHCAL->set_removeBadTowers(false);
  embedder_OHCAL->set_embedwaveform(true);
  embedder_OHCAL->set_inputNodePrefix("TOWERS_");
  embedder_OHCAL->Verbosity(verbosity);
  se->registerSubsystem(embedder_OHCAL);

  // waveform fitting

  CaloTowerBuilder *ca2 = new CaloTowerBuilder();
  ca2->set_detector_type(CaloTowerDefs::CEMC);
  ca2->set_nsamples(12);
  ca2->set_dataflag(false);
  ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ca2->set_builder_type(CaloTowerDefs::kWaveformTowerSimv1);
  // match our current ZS threshold ~60ADC for emcal
  ca2->set_softwarezerosuppression(true, 60);
  se->registerSubsystem(ca2);

  ca2 = new CaloTowerBuilder();
  ca2->set_detector_type(CaloTowerDefs::HCALIN);
  ca2->set_nsamples(12);
  ca2->set_dataflag(false);
  ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ca2->set_builder_type(CaloTowerDefs::kWaveformTowerSimv1);
  ca2->set_softwarezerosuppression(true, 30);
  se->registerSubsystem(ca2);

  ca2 = new CaloTowerBuilder();
  ca2->set_detector_type(CaloTowerDefs::HCALOUT);
  ca2->set_nsamples(12);
  ca2->set_dataflag(false);
  ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ca2->set_builder_type(CaloTowerDefs::kWaveformTowerSimv1);
  ca2->set_softwarezerosuppression(true, 30);
  se->registerSubsystem(ca2);

  //--------------
  // Set up Input Managers
  //--------------

  for (auto & iter : INPUTEMBED::filename)
  {
    std::string mgrname = "DSTin" + std::to_string(iter.first);
    Fun4AllInputManager *hitsin = new Fun4AllDstInputManager(mgrname, "DST", DataTopNode);
    hitsin->fileopen(iter.second);
    hitsin->Verbosity(Input::VERBOSITY);
    if (INPUTEMBED::REPEAT)
    {
      hitsin->Repeat();
    }
    se->registerInputManager(hitsin);
  }
  // input for pedestals
  {
    TRandom3 randGen;
    // get seed
    unsigned int seed = PHRandomSeed();
    randGen.SetSeed(seed);
    // a int from 0 to 3259
    int sequence = randGen.Integer(3260);
    // pad the name
    std::ostringstream opedname;
    opedname << "pedestal-54256-0" << std::setw(4) << std::setfill('0') << sequence << ".root";

    std::string pedestalname = opedname.str();

    Fun4AllInputManager *hitsin = new Fun4AllNoSyncDstInputManager("DST2");
    hitsin->AddFile(pedestalname);
    hitsin->Repeat();
    se->registerInputManager(hitsin);
  }
  // input geom files
  {
    Fun4AllInputManager *ingeo = new Fun4AllRunNodeInputManager("DST_GEO");
    std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
    ingeo->AddFile(geoLocation);
    se->registerInputManager(ingeo);
  }

  //  InputManagers();

  std::string FullOutFile = outdir + "DST_TRUTH_G4HIT_" + outnameEnd;
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("TRUTHOUT", FullOutFile);
  AddCommonNodes(out);
  out->AddNode("G4TruthInfo");
  out->AddNode("G4HIT_BH_1");
  out->AddNode("PHHepMCGenEventMap");
  out->Verbosity(0);
  se->registerOutputManager(out);
  // OUTPUTMANAGER::outfiles.insert(FullOutFile);
  se->registerOutputManager(out);

  FullOutFile = outdir + "DST_TRUTH_JET_" + outnameEnd;
  out = new Fun4AllDstOutputManager("JETOUT", FullOutFile);
  AddCommonNodes(out);
  out->AddNode("AntiKt_Truth_r02");
  out->AddNode("AntiKt_Truth_r03");
  out->AddNode("AntiKt_Truth_r04");
  out->AddNode("AntiKt_Truth_r05");
  out->AddNode("AntiKt_Truth_r06");
  out->AddNode("AntiKt_Truth_r07");
  out->AddNode("AntiKt_Truth_r08");
  out->AddNode("AntiKt_Sim_r02");
  out->AddNode("AntiKt_Sim_r03");
  out->AddNode("AntiKt_Sim_r04");
  out->AddNode("AntiKt_Sim_r05");
  out->AddNode("AntiKt_Sim_r06");
  out->AddNode("AntiKt_Sim_r07");
  out->AddNode("AntiKt_Sim_r08");
  out->AddNode("AntiKt_Sim_Retower_r02");
  out->AddNode("AntiKt_Sim_Retower_r03");
  out->AddNode("AntiKt_Sim_Retower_r04");
  out->AddNode("AntiKt_Sim_Retower_r05");
  out->AddNode("AntiKt_Sim_Retower_r06");
  out->AddNode("AntiKt_Sim_Retower_r07");
  out->AddNode("AntiKt_Sim_Retower_r08");
  out->Verbosity(0);
  se->registerOutputManager(out);
  // OUTPUTMANAGER::outfiles.insert(FullOutFile);

  FullOutFile = outdir + "DST_CALO_" + outnameEnd;
  out = new Fun4AllDstOutputManager("CALOPROCESSED", FullOutFile);
  AddCommonNodes(out);
  out->AddNode("TOWERS_CEMC");
  out->AddNode("TOWERS_HCALIN");
  out->AddNode("TOWERS_HCALOUT");
  out->Verbosity(0);
  se->registerOutputManager(out);
  // OUTPUTMANAGER::outfiles.insert(FullOutFile);

  FullOutFile = outdir + "DST_GLOBAL_" + outnameEnd;
  out = new Fun4AllDstOutputManager("GLOBALOUT", FullOutFile);
  AddCommonNodes(out);
  out->AddNode("GlobalVertexMap");
  out->AddNode("MinimumBiasInfo");
  out->AddNode("CentralityInfo");
  out->AddNode("MbdOut");
  out->Verbosity(0);
  se->registerOutputManager(out);
  // OUTPUTMANAGER::outfiles.insert(FullOutFile);

  //-----------------
  // Event processing
  //-----------------
  // if we use a negative number of events we go back to the command line here
  if (nEvents < 0)
  {
    return 0;
  }
  // if we run the particle generator and use 0 it'll run forever
  if (nEvents == 0 && !Input::HEPMC && !Input::READHITS && INPUTEMBED::REPEAT)
  {
    std::cout << "using 0 for number of events is a bad idea when using particle generators" << std::endl;
    std::cout << "it will run forever, so I just return without running anything" << std::endl;
    return 0;
  }

  se->run(nEvents);

  //-----
  // Exit
  //-----

  CDBInterface::instance()->Print(); // print used DB files
  se->End();
  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
  return 0;
}

void AddCommonNodes(Fun4AllOutputManager *out)
{
  out->AddNode("Sync");
  out->AddNode("EventHeader");
  return;
}

#endif
