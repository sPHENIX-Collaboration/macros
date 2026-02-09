#ifndef FUN4ALL_PREPDATAFITTING_C
#define FUN4ALL_PREPDATAFITTING_C

#include <Calo_Fitting.C>
#include <QA.C>

#include <calotrigger/TriggerRunInfoReco.h>

#include <calovalid/CaloFittingQA.h>

#include <mbd/MbdReco.h>

#include <epd/EpdReco.h>

#include <globalvertex/GlobalVertexReco.h>

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

#include <zdcinfo/ZdcReco.h>

#include <Calo_Calib.C>

#include <Rtypes.h> // for R__LOAD_LIBRARY
#include <TSystem.h>

#include <format>
#include <fstream>

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloWaveformProcessing.h>

#include <centrality/CentralityReco.h>
#include <calotrigger/MinimumBiasClassifier.h>
#include <generaldstslimmer/TriggerDSTSkimmer.h>

R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libepd.so)
R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libTriggerDSTSkimmer.so)

// this pass containis the reco process that's stable wrt time stamps(raw tower building)
void Fun4All_PrepDataFitting(int nEvents = 1e2,
			   const std::string& inlist = "test.list",
                           const std::string &outfile = "DST_CALOFITTING",
                           const std::string &outfile_hist = "HIST_CALOFITTINGQA",
                           const std::string &dbtag = "ProdA_2024")
{

  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  se->VerbosityDownscale(1000);

  recoConsts *rc = recoConsts::instance();

  // conditions DB global tag
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  CDBInterface::instance()->Verbosity(1);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);


  // loop over all files in file list and create an input manager for each one  
  Fun4AllInputManager *In = nullptr;
  std::ifstream infile;
  infile.open(inlist);
  int iman = 0;
  std::string line;
  bool first {true};
  int runnumber = 0;
  int segment = 99999;
  if (infile.is_open())
  {
    while (std::getline(infile, line))
    {
      if (line[0] == '#')
      {
	std::cout << "found commented out line " << line << std::endl;
	continue;
      }
      // extract run number from first not commented out file in list
      if (first)
      {
	std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(line);
	runnumber = runseg.first;
	segment = runseg.second;
	rc->set_uint64Flag("TIMESTAMP", runnumber);
	first = false;
      }
      std::string magname = "DSTin_" + std::to_string(iman);
      In = new Fun4AllDstInputManager(magname);
      In->Verbosity(1);
      In->AddFile(line);
      se->registerInputManager(In);
      iman++;
    }
    infile.close();
  }

  // Get info from DB and store in DSTs
  TriggerRunInfoReco *triggerinfo = new TriggerRunInfoReco();
  se->registerSubsystem(triggerinfo);

  // MBD/BBC Reconstruction
  MbdReco *mbdreco = new MbdReco();
  se->registerSubsystem(mbdreco);

  // Official vertex storage
  GlobalVertexReco *gvertex = new GlobalVertexReco();
  se->registerSubsystem(gvertex);

  //Process_Calo_Fitting();

  //////////////////////////////////
  // make towerinfov3 nodes 
  CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFWaveform;

  CaloTowerBuilder *caZDC = new CaloTowerBuilder("ZDCBUILDER");
  caZDC->set_detector_type(CaloTowerDefs::ZDC);
  caZDC->set_builder_type(buildertype);
  caZDC->set_processing_type(CaloWaveformProcessing::FAST);
  caZDC->set_nsamples(16);
  caZDC->set_offlineflag();
  se->registerSubsystem(caZDC);

  CaloTowerBuilder *ctbEMCal = new CaloTowerBuilder("EMCalBUILDER");
  ctbEMCal->set_detector_type(CaloTowerDefs::CEMC);
  ctbEMCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbEMCal->set_builder_type(buildertype);
  ctbEMCal->set_offlineflag(true);
  ctbEMCal->set_nsamples(12);
  ctbEMCal->set_bitFlipRecovery(true);
  //60 ADC SZS
  ctbEMCal->set_softwarezerosuppression(true, 60);
  se->registerSubsystem(ctbEMCal);

  CaloTowerBuilder *ctbIHCal = new CaloTowerBuilder("HCALINBUILDER");
  ctbIHCal->set_detector_type(CaloTowerDefs::HCALIN);
  ctbIHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbIHCal->set_builder_type(buildertype);
  ctbIHCal->set_offlineflag();
  ctbIHCal->set_nsamples(12);
  ctbIHCal->set_bitFlipRecovery(true);
  //30 ADC SZS
  ctbIHCal->set_softwarezerosuppression(true, 30);
  se->registerSubsystem(ctbIHCal);

  CaloTowerBuilder *ctbOHCal = new CaloTowerBuilder("HCALOUTBUILDER");
  ctbOHCal->set_detector_type(CaloTowerDefs::HCALOUT);
  ctbOHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbOHCal->set_builder_type(buildertype);
  ctbOHCal->set_offlineflag();
  ctbOHCal->set_nsamples(12);
  ctbOHCal->set_bitFlipRecovery(true);
  //30 ADC SZS
  ctbOHCal->set_softwarezerosuppression(true, 30);
  se->registerSubsystem(ctbOHCal);

  CaloTowerBuilder *caEPD = new CaloTowerBuilder("SEPDBUILDER");
  caEPD->set_detector_type(CaloTowerDefs::SEPD);
  caEPD->set_builder_type(buildertype);
  caEPD->set_processing_type(CaloWaveformProcessing::FAST);
  caEPD->set_nsamples(12);
  caEPD->set_offlineflag();
  se->registerSubsystem(caEPD);



  ///////////////////////////
  // other detectors
  // sEPD Reconstruction--Calib Info
  EpdReco *epdreco = new EpdReco();
  se->registerSubsystem(epdreco);

  // ZDC Reconstruction--Calib Info
  ZdcReco *zdcreco = new ZdcReco();
  zdcreco->set_zdc1_cut(0.0);
  zdcreco->set_zdc2_cut(0.0);
  se->registerSubsystem(zdcreco);

  //////////////////////////////
  // Centrality
  MinimumBiasClassifier *mb = new MinimumBiasClassifier();
  se->registerSubsystem(mb);

  CentralityReco* cent = new CentralityReco();
  se->registerSubsystem( cent );

  //Process_Calo_Calib(); 


  //////////////////
  // skim out non-MB events
  TriggerDSTSkimmer *tskim = new TriggerDSTSkimmer();
  vector<int> triggers;
  triggers.push_back(10);
  tskim->SetTrigger(triggers);
  tskim->set_accept_max(nEvents);
  se->registerSubsystem(tskim);

  ///////////////////////////////////
  // Validation
  CaloFittingQA *ca = new CaloFittingQA("CaloFittingQA");
  se->registerSubsystem(ca);

  
// this strips all nodes under the Packets PHCompositeNode
// (means removes all offline packets)
  std::string dstoutfile = std::format("{}-{:08}-{:05}.root",outfile, runnumber,segment);

  std::ofstream outList("prepData.txt");
  outList << dstoutfile;
  outList.close();
  
  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", dstoutfile);
  out->StripCompositeNode("Packets");
  se->registerOutputManager(out);
  // se->Print();
  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents*2);
  se->End();
  dstoutfile = std::format("{}-{:08}-{:05}.root",outfile_hist, runnumber,segment);
  QAHistManagerDef::saveQARootFile(dstoutfile);

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
#endif
