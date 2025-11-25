#ifndef FUN4ALL_JETSKIMMEDPRODUCTIONYEAR2_C
#define FUN4ALL_JETSKIMMEDPRODUCTIONYEAR2_C

#include <QA.C>
#include <Calo_Calib.C>
#include <HIJetReco.C>
#include <Jet_QA.C>
#include <G4_Global.C>
#include <G4_Centrality.C>

#include <mbd/MbdReco.h>

#include <globalvertex/GlobalVertexReco.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <centrality/CentralityReco.h>
#include <calotrigger/MinimumBiasClassifier.h>

#include <calovalid/CaloValid.h>

#include <jetdstskimmer/JetDSTSkimmer.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libJetDSTSkimmer.so)

void Fun4All_JetSkimmedProductionYear2(int nEvents=1000,
                        const std::string &fname = "DST_CALOFITTING_run2pp_ana509_2024p022_v001-00047289-00000.root",
                        const std::string& outfile_low= "DST_JETCALO_run2pp_ana509_2024p022_v001-00047289-00000.root",
                        const std::string& outfile_high= "DST_Jet_run2pp_ana509_2024p022_v001-00047289-00000.root",
                        const std::string& outfile_hist= "HIST_JETQA_run2pp_ana509_2024p022_v001-00047289-00000.root",
                        const std::string& dbtag= "ProdA_2024"
  )
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

  recoConsts *rc = recoConsts::instance();

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(fname);
  int runnumber = runseg.first;

  // conditions DB flags and timestamp
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  CDBInterface::instance()->Verbosity(1);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  // MBD/BBC Reconstruction
  MbdReco *mbdreco = new MbdReco();
  se->registerSubsystem(mbdreco);

  // Official vertex storage
  GlobalVertexReco *gvertex = new GlobalVertexReco();
  se->registerSubsystem(gvertex);

  /////////////////////////////////////////////////////
  // Set status of CALO towers, Calibrate towers,  Cluster
  std::cout << "Processing Calo Calibration" << std::endl;
  Process_Calo_Calib();

  ///////////////////////////////////
  // Validation maybe no need to run for the skimmed production?
  /*
  CaloValid *ca = new CaloValid("CaloValid");
  ca->set_timing_cut_width(200);
  se->registerSubsystem(ca);
  */

  // Jet reco related flags
  Enable::QA = true;

  // turn on pp mode
  HIJETS::is_pp = true;

  // qa options
  JetQA::HasTracks = false;
  JetQA::DoInclusive = true;
  JetQA::DoTriggered = true;
  JetQA::RestrictPtToTrig = false;
  JetQA::RestrictEtaByR = true;

  if (!HIJETS::is_pp)
  {
    Centrality();
  }

  GlobalVertex::VTXTYPE vertex_type = GlobalVertex::MBD;

  std::string tower_prefix = "TOWERINFO_CALIB";

  RetowerCEMC *rcemc = new RetowerCEMC();
  rcemc->Verbosity(0);
  rcemc->set_towerinfo(true);
  rcemc->set_frac_cut(0.5);  // fraction of retower that must be masked to mask the full retower
  rcemc->set_towerNodePrefix(tower_prefix);
  se->registerSubsystem(rcemc);


  // do unsubtracted jet reconstruction
  TowerJetInput *incemc = new TowerJetInput(Jet::CEMC_TOWERINFO_RETOWER,tower_prefix);
  TowerJetInput *inihcal = new TowerJetInput(Jet::HCALIN_TOWERINFO, tower_prefix);
  TowerJetInput *inohcal = new TowerJetInput(Jet::HCALOUT_TOWERINFO,tower_prefix);
  incemc->set_GlobalVertexType(vertex_type);
  inihcal->set_GlobalVertexType(vertex_type);
  inohcal->set_GlobalVertexType(vertex_type);
  
  JetReco *_jetRecoUnsub = new JetReco();
  _jetRecoUnsub->add_input(incemc);
  _jetRecoUnsub->add_input(inihcal);
  _jetRecoUnsub->add_input(inohcal);
  _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.2), "AntiKt_unsubtracted_r02");
  _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.3), "AntiKt_unsubtracted_r03");
  _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.4), "AntiKt_unsubtracted_r04");
  _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.5), "AntiKt_unsubtracted_r05");
  _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.6), "AntiKt_unsubtracted_r06");
  _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.7), "AntiKt_unsubtracted_r07");
  _jetRecoUnsub->add_algo(new FastJetAlgoSub(Jet::ANTIKT, 0.8), "AntiKt_unsubtracted_r08");

  
  _jetRecoUnsub->set_algo_node("ANTIKT");
  _jetRecoUnsub->set_input_node("TOWER");
  se->registerSubsystem(_jetRecoUnsub);

  JetDSTSkimmer *jetDSTSkimmer = new JetDSTSkimmer();
  std::map<std::string, float> jetNodePts;
  jetNodePts["AntiKt_unsubtracted_r02"] = 6.6;
  jetNodePts["AntiKt_unsubtracted_r03"] = 7.3;
  jetNodePts["AntiKt_unsubtracted_r04"] = 7.6;
  jetNodePts["AntiKt_unsubtracted_r05"] = 7.9;
  jetNodePts["AntiKt_unsubtracted_r06"] = 8.0;
  jetNodePts["AntiKt_unsubtracted_r07"] = 11.0;
  jetNodePts["AntiKt_unsubtracted_r08"] = 12.0;
  jetDSTSkimmer->SetJetNodeThresholds(jetNodePts);
  std::map<std::string, float> clusterNodePts;
  clusterNodePts["CLUSTERINFO_CEMC"] = 5;
  jetDSTSkimmer->SetClusterNodeThresholds(clusterNodePts);
  se->registerSubsystem(jetDSTSkimmer);

  // register modules necessary for QA
  if (Enable::QA)
  {
    DoRhoCalculation();
    Jet_QA();
  }

  Fun4AllInputManager *In = new Fun4AllDstInputManager("in");
  In->AddFile(fname);
  se->registerInputManager(In);

  Fun4AllDstOutputManager *outlower = new Fun4AllDstOutputManager("DSTOUTLOW", outfile_low);
  outlower->AddNode("Sync");
  outlower->AddNode("EventHeader");
  //outlower->AddNode("PacketsKeep");
  outlower->AddNode("14001");
  outlower->AddNode("1001");
  outlower->AddNode("1002");
  outlower->AddNode("TOWERS_HCALIN");
  outlower->AddNode("TOWERS_HCALOUT");
  outlower->AddNode("TOWERS_CEMC");
  outlower->AddNode("TOWERS_SEPD");
  outlower->AddNode("TOWERS_ZDC");
  //outlower->AddNode("MbdOut");
  //outlower->AddNode("MbdPmtContainer");
  //outlower->AddNode("MBDPackets");
  outlower->AddNode("TriggerRunInfo");
  se->registerOutputManager(outlower);

  Fun4AllDstOutputManager *outhigher = new Fun4AllDstOutputManager("DSTOUTHIGH", outfile_high);
  outhigher->StripNode("1001");
  outhigher->StripNode("1002");
  outhigher->StripNode("TOWERINFO_CALIB_HCALIN");
  outhigher->StripNode("TOWERS_HCALIN");
  outhigher->StripNode("TOWERINFO_CALIB_HCALOUT");
  outhigher->StripNode("TOWERS_HCALOUT");
  outhigher->StripNode("TOWERINFO_CALIB_CEMC");
  outhigher->StripNode("TOWERS_CEMC");
  outhigher->StripNode("TOWERS_SEPD");
  outhigher->StripNode("TOWERINFO_CALIB_ZDC");
  outhigher->StripNode("TOWERS_ZDC");
  se->registerOutputManager(outhigher);

  se->run(nEvents);
  se->End();

  if (Enable::QA)
  {
    QAHistManagerDef::saveQARootFile(outfile_hist);
  }

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

#endif
