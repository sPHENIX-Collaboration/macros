#ifndef FUN4ALL_JETSKIMMEDPRODUCTIONYEAR3_C
#define FUN4ALL_JETSKIMMEDPRODUCTIONYEAR3_C

#include <QA.C>
#include <Calo_Calib.C>
#include <HIJetReco.C>
#include <Jet_QA.C>
//copying from the JetProductionYear2.C, but isn't it wierd to use macro with G4 prefix here?
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

#include <jetbackground/BeamBackgroundFilterAndQA.h>
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

void Fun4All_JetSkimmedProductionYear3(int nEvents=1000,
                        const std::string &fname = "DST_CALOFITTING_run3auau_new_newcdbtag_v007-00068490-00000.root",
                        const std::string& outfile_low= "DST_JETCALO-00000000-000000.root",
                        const std::string& outfile_high= "DST_Jet-00000000-000000.root",
                        const std::string& outfile_hist= "HIST_JETQA-00000000-000000.root",
                        const std::string& dbtag= "ProdA_2024"
  )
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();

  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(fname);
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
  Process_Calo_Calib();

  // Jet reco related flags
  Enable::QA = true;

  // turn on pp mode
  HIJETS::is_pp = false;

  // qa options
  JetQA::HasTracks = false;
  JetQA::DoInclusive = true;
  JetQA::DoTriggered = true;
  JetQA::RestrictPtToTrig = false;
  JetQA::RestrictEtaByR = true;
  JetQA::DoPP = HIJETS::is_pp;
  JetQA::UseBkgdSub = true;
  JetQA::HasCalos = true;
  
  if (!HIJETS::is_pp)
  {
    Centrality();
  }

  // filter out beam-background events (use default parameters for
  // streak-sideband filter)
  BeamBackgroundFilterAndQA* filter = new BeamBackgroundFilterAndQA("BeamBackgroundFilterAndQA");
  filter->Verbosity(std::max(Enable::QA_VERBOSITY, Enable::JETQA_VERBOSITY));
  filter->SetConfig(
    {
      .debug          = false,
      .doQA           = Enable::QA,
      .doEvtAbort     = false,
      .filtersToApply = {"StreakSideband"}
    }
  );
  se->registerSubsystem(filter);

  // do jet reconstruction & rho calculation
  HIJetReco();

  JetDSTSkimmer *jetDSTSkimmer = new JetDSTSkimmer();
  //these are all default values
  jetDSTSkimmer->SetMinJetPt(7);
  jetDSTSkimmer->SetMinClusterPt(5);
  jetDSTSkimmer->SetJetNodeName("AntiKt_Tower_r04_Sub1");
  jetDSTSkimmer->SetClusterNodeName("CLUSTERINFO_CEMC");
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
  outlower->AddNode("GL1Packet");
  //outlower->AddNode("TOWERINFO_CALIB_HCALIN");
  outlower->AddNode("TOWERS_HCALIN");
  //outlower->AddNode("TOWERINFO_CALIB_HCALOUT");
  outlower->AddNode("TOWERS_HCALOUT");
  //outlower->AddNode("TOWERINFO_CALIB_CEMC");
  outlower->AddNode("TOWERS_CEMC");
  outlower->AddNode("TOWERS_SEPD");
  //outlower->AddNode("TOWERINFO_CALIB_ZDC");
  outlower->AddNode("TOWERS_ZDC");
  outlower->AddNode("MbdOut");
  outlower->AddNode("MbdPmtContainer");
  outlower->AddNode("MBDPackets");
  outlower->AddNode("TriggerRunInfo");
  se->registerOutputManager(outlower);

  Fun4AllDstOutputManager *outhigher = new Fun4AllDstOutputManager("DSTOUTHIGH", outfile_high);
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
    QAHistManagerDef::saveQARootFile(outfile_hist);
  }

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

#endif
