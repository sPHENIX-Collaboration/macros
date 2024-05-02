#ifndef FUN4ALL_YEAR1_C
#define FUN4ALL_YEAR1_C

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/CaloWaveformProcessing.h>

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

#include <calovalid/CaloValid.h>

#include <litecaloeval/HCalCosmics.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

void Fun4All_HCalCosmics(int nEvents = 5e2, const std::string &fname = "dst_triggered_raw_cosmics-00040174.list") 
{
  bool useDSTRAW = true;
  // v1 uncomment:
  // CaloTowerDefs::BuilderType buildertype = CaloTowerDefs:::kPRDFTowerv1;
  // v2 uncomment:
  CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kWaveformTowerv2;
  // v3 uncomment:
  // CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFWaveform;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();

  ifstream file(fname);
  std::string first_file;
  getline(file, first_file);
  

  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
  int runnumber = runseg.first;
  int segment = runseg.second;
  char outfile[100];
  char outfile_hist1[100];
  char outfile_hist2[100];
  sprintf(outfile, "DST_CALOR-%08d-%04d.root", runnumber, segment);
  sprintf(outfile_hist1, "HIST_HCALOUT-%08d-%04d.root", runnumber, segment);
  sprintf(outfile_hist2, "HIST_HCALIN-%08d-%04d.root", runnumber, segment);
  std::string fulloutfile = std::string("./") + outfile;
  std::string fulloutfile_hist1 = std::string("./") + outfile_hist1;
  std::string fulloutfile_hist2 = std::string("./") + outfile_hist2;

  //===============
  // conditions DB flags
  //===============
  // ENABLE::CDB = true;
  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  // // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  CDBInterface::instance()->Verbosity(1);

  // Sync Headers and Flags
  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);


  /////////////////
  // build towers
  CaloTowerBuilder *ctbIHCal = new CaloTowerBuilder("HCALINBUILDER");
  ctbIHCal->set_detector_type(CaloTowerDefs::HCALIN);
  ctbIHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbIHCal->set_builder_type(buildertype);
  //ctbIHCal->set_nsamples(31);
  if (useDSTRAW) ctbIHCal->set_offlineflag();
  ctbIHCal->set_softwarezerosuppression(true, 200);
  se->registerSubsystem(ctbIHCal);

  CaloTowerBuilder *ctbOHCal = new CaloTowerBuilder("HCALOUTBUILDER");
  ctbOHCal->set_detector_type(CaloTowerDefs::HCALOUT);
  ctbOHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbOHCal->set_builder_type(buildertype);
  //ctbOHCal->set_nsamples(31);
  if (useDSTRAW) ctbOHCal->set_offlineflag();
  ctbOHCal->set_softwarezerosuppression(true, 200);
  se->registerSubsystem(ctbOHCal);

  ////////////////////
  // Calibrate towers
  std::cout << "Calibrating OHcal" << std::endl;
  CaloTowerCalib *calibOHCal = new CaloTowerCalib("HCALOUT");
  calibOHCal->set_detector_type(CaloTowerDefs::HCALOUT);
  se->registerSubsystem(calibOHCal);

  std::cout << "Calibrating IHcal" << std::endl;
  CaloTowerCalib *calibIHCal = new CaloTowerCalib("HCALIN");
  calibIHCal->set_detector_type(CaloTowerDefs::HCALIN);
  se->registerSubsystem(calibIHCal);

  std::cout << "Adding Geometry file" << std::endl;
  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2);

  ///////////////////////////////////////////
  // Cosmics histMaker 
  HCalCosmics *wt2 = new HCalCosmics("HCalCalib_HCALIN",fulloutfile_hist2);
  wt2->Detector("HCALIN");
  se->registerSubsystem(wt2);

  HCalCosmics *wt3 = new HCalCosmics("HCalCosmics_HCALOUT",fulloutfile_hist1);
  wt3->Detector("HCALOUT");
  se->registerSubsystem(wt3);

/*
  ///////////////////////////////////
  // Validation 
  CaloValid *ca = new CaloValid("calomodulename",fulloutfile_hist);
  ca->set_timing_cut_width(200);  //integers for timing width, > 1 : wider cut around max peak time
  ca->apply_vertex_cut(false);
  ca->set_vertex_cut(20.);
  se->registerSubsystem(ca);
*/

  Fun4AllInputManager *In;
  if(!useDSTRAW) In = new Fun4AllPrdfInputManager("in");
  if(useDSTRAW)  In = new Fun4AllDstInputManager("in");
  In->AddListFile(fname);
  se->registerInputManager(In);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", fulloutfile);
  out->StripNode("CEMCPackets");
  out->StripNode("HCALPackets");
  out->StripNode("ZDCPackets");
  out->StripNode("SEPDPackets");
  out->StripNode("MBDPackets");
  se->registerOutputManager(out);

  se->run(nEvents);
  se->End();
  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;

  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

#endif
