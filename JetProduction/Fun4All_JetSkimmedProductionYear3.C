#ifndef FUN4ALL_JETSKIMMEDPRODUCTIONYEAR3_C
#define FUN4ALL_JETSKIMMEDPRODUCTIONYEAR3_C

#include <QA.C>
#include <Calo_Calib.C>
#include <HIJetReco.C>
#include <Jet_QA.C>
#include <G4_Global.C>
#include <G4_Centrality.C>

#include <mbd/MbdReco.h>
#include <globalvertex/GlobalVertexReco.h>
#include <jetbackground/BeamBackgroundFilterAndQA.h>
#include <centrality/CentralityReco.h>
#include <calotrigger/MinimumBiasClassifier.h>
#include <calovalid/CaloValid.h>
#include <jetdstskimmer/JetDSTSkimmer.h>

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

#include <iostream>
#include <map>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libjetbackground.so)
R__LOAD_LIBRARY(libJetDSTSkimmer.so)

void Fun4All_JetSkimmedProductionYear3(
				       int nEvents = 1000,
				       const std::string &fname =
				       "DST_CALOFITTING_run3auau_new_newcdbtag_v008-00075142-00000.root",
				       const std::string &outfile_low  = "DST_JETCALO-00000000-000000.root",
				       const std::string &outfile_high = "DST_Jet-00000000-000000.root",
				       const std::string &outfile_hist = "HIST_JETQA-00000000-000000.root",
				       const std::string &dbtag        = "newcdbtag")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  recoConsts::instance()->set_IntFlag("PHOOL_VERBOSITY", 0);
  se->Verbosity(0);
  
  recoConsts *rc = recoConsts::instance();
  
  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(fname);
  int runnumber = runseg.first;
  
  // conditions DB flags and timestamp
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  CDBInterface::instance()->Verbosity(0);
  
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
  std::cout << ">>> Processing Calo Calibration" << std::endl;
  Process_Calo_Calib();
  
  // Jet reco related flags
  Enable::QA = true;
  
  // Au+Au mode
  HIJETS::is_pp = false;
  
  // QA options
  JetQA::HasTracks        = false;
  JetQA::DoInclusive      = true;
  JetQA::DoTriggered      = true;
  JetQA::RestrictPtToTrig = false;
  JetQA::RestrictEtaByR   = true;
  JetQA::DoPP             = HIJETS::is_pp;
  JetQA::UseBkgdSub       = true;
  JetQA::HasCalos         = true;
  
  if (!HIJETS::is_pp)
    {
      std::cout << ">>> Running Centrality()" << std::endl;
      Centrality();
    }
  
  // HIJetReco macro 
  std::cout << ">>> Running HIJetReco()" << std::endl;
  HIJetReco();
  
  // Jet DST skimmer with map-based threshold
  std::cout << ">>> Configuring JetDSTSkimmer" << std::endl;
  JetDSTSkimmer *jetDSTSkimmer = new JetDSTSkimmer();
  
  // Jet thresholds: map from node name [set min jet pT]
  std::map<std::string, float> jetNodePts;
  
  jetNodePts["AntiKt_Tower_r04_Sub1"] = 7.0F;
  jetDSTSkimmer->SetJetNodeThresholds(jetNodePts);
  
  // Cluster thresholds: map from cluster node [set min cluster pT]
  std::map<std::string, float> clusterNodePts;
  clusterNodePts["CLUSTERINFO_CEMC"] = 5.0F;
  jetDSTSkimmer->SetClusterNodeThresholds(clusterNodePts);
  
  jetDSTSkimmer->Verbosity(0);
  se->registerSubsystem(jetDSTSkimmer);
  std::cout << ">>> JetDSTSkimmer registered" << std::endl;
  
  // Filter out beam-background events
  BeamBackgroundFilterAndQA *filter =
    new BeamBackgroundFilterAndQA("BeamBackgroundFilterAndQA");
  filter->Verbosity(std::max(Enable::QA_VERBOSITY, Enable::JETQA_VERBOSITY));
  filter->SetConfig(
		    {
		      .debug          = false,
		      .doQA           = Enable::QA,
		      .doEvtAbort     = false,
		      .filtersToApply = {"StreakSideband"}
		    });
  se->registerSubsystem(filter);
  
  // Register modules necessary for QA
  if (Enable::QA)
    {
      std::cout << ">>> Enabling QA: DoRhoCalculation + Jet_QA" << std::endl;
      DoRhoCalculation();
      Jet_QA();
    }
  
  std::cout << ">>> Setting up input and output managers" << std::endl;
  
  Fun4AllInputManager *In = new Fun4AllDstInputManager("in");
  In->Verbosity(1);
  In->AddFile(fname);
  se->registerInputManager(In);
  
  Fun4AllDstOutputManager *outlower =
    new Fun4AllDstOutputManager("DSTOUTLOW", outfile_low);
  outlower->AddNode("Sync");
  outlower->AddNode("EventHeader");
  outlower->AddNode("GL1Packet");
  outlower->AddNode("TOWERS_HCALIN");
  outlower->AddNode("TOWERS_HCALOUT");
  outlower->AddNode("TOWERS_SEPD");
  outlower->AddNode("TOWERS_ZDC");
  outlower->AddNode("MbdOut");
  outlower->AddNode("MbdPmtContainer");
  outlower->AddNode("MBDPackets");
  outlower->AddNode("TriggerRunInfo");
  se->registerOutputManager(outlower);
  
  Fun4AllDstOutputManager *outhigher =
    new Fun4AllDstOutputManager("DSTOUTHIGH", outfile_high);
  outhigher->StripNode("TOWERINFO_CALIB_HCALIN");
  outhigher->StripNode("TOWERS_HCALIN");
  outhigher->StripNode("TOWERINFO_CALIB_HCALOUT");
  outhigher->StripNode("TOWERS_HCALOUT");
  outhigher->StripNode("TOWERINFO_CALIB_CEMC");
  outhigher->StripNode("TOWERS_CEMC");
  outhigher->StripNode("TOWERS_SEPD");
  outhigher->StripNode("TOWERINFO_CALIB_ZDC");
  outhigher->StripNode("TOWERS_ZDC");
  outhigher->StripNode("MBDPackets");
  outhigher->StripNode("MbdOut");
  outhigher->StripNode("MbdPmtContainer");
  se->registerOutputManager(outhigher);

  se->run(nEvents);
  se->End();
  
  if (Enable::QA)
    {
      std::cout << ">>> Saving QA histograms to " << outfile_hist << std::endl;
      QAHistManagerDef::saveQARootFile(outfile_hist);
    }
  
  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

#endif
