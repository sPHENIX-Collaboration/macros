#ifndef FUN4ALL_JETSKIMMEDPRODUCTIONYEAR2_C
#define FUN4ALL_JETSKIMMEDPRODUCTIONYEAR2_C

#include <QA.C>
#include <Calo_Calib.C>
#include <HIJetReco.C>
#include <Jet_QA.C>
#include <G4_Global.C>
#include <G4_Centrality.C>

#include <mbd/MbdReco.h>

#include <caloreco/CaloTowerBuilder.h>

#include <epd/EpdReco.h>

#include <zdcinfo/ZdcReco.h>

#include <globalvertex/GlobalVertexReco.h>

#include <centrality/CentralityReco.h>

#include <globalqa/GlobalQA.h>

#include <calotrigger/MinimumBiasClassifier.h>

#include <calovalid/CaloValid.h>

#include <jetdstskimmer/JetDSTSkimmer.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libglobalQA.so)
R__LOAD_LIBRARY(libJetDSTSkimmer.so)
R__LOAD_LIBRARY(libepd.so)
R__LOAD_LIBRARY(libzdcinfo.so)

void Fun4All_JetSkimmedProductionYear2(int nEvents = 1000,
                        const std::string &fname_calo = "DST_CALOFITTING_run3oo_pro001_pcdb001_v001-00082703-00000.root",
                        const std::string &fname_zdc  = "DST_ZDC_RAW_run3oo_pro001_pcdb001_v001-00082703-00000.root",
                        const std::string &fname_sepd  = "DST_SEPD_RAW_run3oo_pro001_pcdb001_v001-00082703-00000.root",
                        const std::string &outfile_low  = "DST_JETCALO_run3oo_pro001_pcdb001_v001-00082703-00000.root",
                        const std::string &outfile_high = "DST_Jet_run3oo_pro001_pcdb001_v001-00082703-00000.root",
                        const std::string &outfile_hist = "HIST_JETQA_run3oo_pro001_pcdb001_v001-00082703-00000.root",
                        const std::string &dbtag = "newcdbtag"
  )
{
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  se->VerbosityDownscale(10000);
  recoConsts *rc = recoConsts::instance();

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(fname_calo);
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

  // sEPD Reconstruction--Calib Info
  CaloTowerBuilder *caEPD = new CaloTowerBuilder("SEPDBUILDER");
  caEPD->set_detector_type(CaloTowerDefs::SEPD);
  caEPD->set_builder_type(CaloTowerDefs::kPRDFTowerv4);
  caEPD->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  caEPD->set_nsamples(12);
  caEPD->set_offlineflag();
  se->registerSubsystem(caEPD);

  EpdReco *epdreco = new EpdReco();
  se->registerSubsystem(epdreco);

  //we need to do the ZDC fitting here...
  CaloTowerBuilder *caZDC = new CaloTowerBuilder("ZDCBUILDER");
  caZDC->set_detector_type(CaloTowerDefs::ZDC);
  caZDC->set_builder_type(CaloTowerDefs::kPRDFTowerv4);
  if( (runnumber > RunnumberRange::RUN2PP_FIRST && runnumber < RunnumberRange::RUN2PP_LAST)
     || (runnumber > RunnumberRange::RUN3PP_FIRST && runnumber < RunnumberRange::RUN3PP_LAST) ){
    caZDC->set_processing_type(CaloWaveformProcessing::FAST);
  }
  else {
    caZDC->set_processing_type(CaloWaveformProcessing::FUNCFIT);
    caZDC->set_funcfit_type(2);
  }
  caZDC->set_nsamples(16);
  caZDC->set_offlineflag();
  se->registerSubsystem(caZDC);


  // ZDC Reconstruction--Calib Info
  ZdcReco *zdcreco = new ZdcReco();
  zdcreco->set_zdc1_cut(0.0);
  zdcreco->set_zdc2_cut(0.0);
  se->registerSubsystem(zdcreco);

  // Official vertex storage
  GlobalVertexReco *gvertex = new GlobalVertexReco();
  se->registerSubsystem(gvertex);

  GlobalQA *gqa = new GlobalQA("GlobalQA");
  se->registerSubsystem(gqa);

  /////////////////////////////////////////////////////
  // Set status of CALO towers, calibrate, cluster
  std::cout << "Processing Calo Calibration" << std::endl;
  Process_Calo_Calib();

  CaloValid *ca = new CaloValid("CaloValid");
  ca->set_timing_cut_width(200);
  se->registerSubsystem(ca);

  // Centrality required for HI
  Centrality();


  // Jet reco related flags
  Enable::QA = true;
  
  // Au+Au mode
  HIJETS::is_pp = false;
  
  // QA options
  JetQA::HasTracks        = false;
  JetQA::DoInclusive      = true;
  JetQA::DoTriggered      = false;
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
  
  jetNodePts["AntiKt_Tower_r03_Sub1"] = 10.0F;
  jetDSTSkimmer->SetJetNodeThresholds(jetNodePts);
  
  // Cluster thresholds: map from cluster node [set min cluster pT]
  std::map<std::string, float> clusterNodePts;
  clusterNodePts["CLUSTERINFO_CEMC"] = 8.0F;
  jetDSTSkimmer->SetClusterNodeThresholds(clusterNodePts);
  
  jetDSTSkimmer->Verbosity(0);
  se->registerSubsystem(jetDSTSkimmer);
  std::cout << ">>> JetDSTSkimmer registered" << std::endl;
  
  // Filter out beam-background events
  /*
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
  */
  // Register modules necessary for QA
  if (Enable::QA)
    {
      std::cout << ">>> Enabling QA: DoRhoCalculation + Jet_QA" << std::endl;
      DoRhoCalculation();
      Jet_QA();
    }

  Fun4AllInputManager *InCalo = new Fun4AllDstInputManager("in");
  InCalo->AddFile(fname_calo);
  se->registerInputManager(InCalo);

  Fun4AllInputManager *InZDC = new Fun4AllDstInputManager("inzdc");
  InZDC->AddFile(fname_zdc);
  se->registerInputManager(InZDC);

  Fun4AllInputManager *InSEPD = new Fun4AllDstInputManager("insepd");
  InSEPD->AddFile(fname_sepd);
  se->registerInputManager(InSEPD);

  // --- Output managers ---
  // DST_JETCALO: low-threshold output — keeps raw tower and ZDC data
  Fun4AllDstOutputManager *outlower = new Fun4AllDstOutputManager("DSTOUTLOW", outfile_low);
  outlower->AddNode("Sync");
  outlower->AddNode("EventHeader");
  outlower->AddNode("12001");
  outlower->AddNode("14001");
  outlower->AddNode("TOWERS_HCALIN");
  outlower->AddNode("TOWERS_HCALOUT");
  outlower->AddNode("TOWERS_CEMC");
  outlower->AddNode("TOWERS_SEPD");
  outlower->AddNode("MbdRawContainer");
  outlower->AddNode("TriggerRunInfo");
  outlower->StripCompositeNode("Packets");
  se->registerOutputManager(outlower);

  // DST_Jet: high-threshold output — strips raw tower data, keeps jets
  Fun4AllDstOutputManager *outhigher = new Fun4AllDstOutputManager("DSTOUTHIGH", outfile_high);
  outhigher->StripNode("TOWERINFO_CALIB_HCALIN");
  outhigher->StripNode("TOWERS_HCALIN");
  outhigher->StripNode("TOWERINFO_CALIB_HCALOUT");
  outhigher->StripNode("TOWERS_HCALOUT");
  outhigher->StripNode("TOWERINFO_CALIB_CEMC");
  outhigher->StripNode("TOWERS_CEMC");
  outhigher->StripNode("TOWERS_SEPD");
  outhigher->StripNode("TOWERINFO_CALIB_ZDC");
  outhigher->StripNode("12001");
  outhigher->StripCompositeNode("Packets");
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
