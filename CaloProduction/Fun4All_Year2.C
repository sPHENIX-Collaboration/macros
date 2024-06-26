#ifndef FUN4ALL_YEAR1_C
#define FUN4ALL_YEAR1_C

#include <QA.C>
#include<Calo_Calib.C>

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/CaloWaveformProcessing.h>
#include <caloreco/DeadHotMapLoader.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterDeadHotMask.h>
#include <caloreco/RawClusterPositionCorrection.h>
#include <caloreco/TowerInfoDeadHotMask.h>

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

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libcalovalid.so)

void Fun4All_Year2(const std::string &fname = "/sphenix/lustre01/sphnxpro/commissioning/slurp/calobeam/run_00040700_00040800/DST_TRIGGERED_RAW_beam_new_2023p015-00040797-0001.root", int nEvents = 10)
{
  bool addZeroSupCaloNodes = 0;
  // v1 uncomment:
  // CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFTowerv1;
  // v2 uncomment:
  CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kWaveformTowerv2;
  // v3 uncomment:
  //CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFWaveform;
  // v4 uncomment:
  // CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFTowerv4;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  recoConsts *rc = recoConsts::instance();

  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(fname);
  int runnumber = runseg.first;
  int segment = runseg.second;
  char outfile[100];
  char outfile_hist[100];
  sprintf(outfile, "DST_CALOR-%08d-%04d.root", runnumber, segment);
  sprintf(outfile_hist, "HIST_CALOR-%08d-%04d.root", runnumber, segment);
  string fulloutfile = string("./") + outfile;
  string fulloutfile_hist = string("./") + outfile_hist;

  // conditions DB flags
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  // 64 bit timestamp
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

  /////////////////
  // build towers
  CaloTowerBuilder *ctbEMCal = new CaloTowerBuilder("EMCalBUILDER");
  ctbEMCal->set_detector_type(CaloTowerDefs::CEMC);
  ctbEMCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbEMCal->set_builder_type(buildertype);
  ctbEMCal->set_offlineflag(true);
  ctbEMCal->set_nsamples(12);
  se->registerSubsystem(ctbEMCal);

  CaloTowerBuilder *ctbIHCal = new CaloTowerBuilder("HCALINBUILDER");
  ctbIHCal->set_detector_type(CaloTowerDefs::HCALIN);
  ctbIHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbIHCal->set_builder_type(buildertype);
  ctbIHCal->set_offlineflag();
  ctbIHCal->set_nsamples(12);
  se->registerSubsystem(ctbIHCal);

  CaloTowerBuilder *ctbOHCal = new CaloTowerBuilder("HCALOUTBUILDER");
  ctbOHCal->set_detector_type(CaloTowerDefs::HCALOUT);
  ctbOHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbOHCal->set_builder_type(buildertype);
  ctbOHCal->set_offlineflag();
  ctbOHCal->set_nsamples(12);
  se->registerSubsystem(ctbOHCal);

  CaloTowerBuilder *caZDC = new CaloTowerBuilder("ZDCBUILDER");
  caZDC->set_detector_type(CaloTowerDefs::ZDC);
  caZDC->set_builder_type(buildertype);
  caZDC->set_processing_type(CaloWaveformProcessing::FAST);
  caZDC->set_nsamples(16);
  caZDC->set_offlineflag();
  se->registerSubsystem(caZDC);

  CaloTowerBuilder *caEPD = new CaloTowerBuilder("SEPDBUILDER");
  caEPD->set_detector_type(CaloTowerDefs::SEPD);
  caEPD->set_builder_type(buildertype);
  caEPD->set_processing_type(CaloWaveformProcessing::FAST);
  caEPD->set_nsamples(12);
  caEPD->set_offlineflag();
  se->registerSubsystem(caEPD);


  /////////////////////
  // Geometry 
  std::cout << "Adding Geometry file" << std::endl;
  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2);

  Process_Calo_Calib();

  
  /////////////////
  // Centrality 
  //MinimumBiasClassifier *minimumbiasclassifier = new MinimumBiasClassifier();
  //se->registerSubsystem(minimumbiasclassifier);
  //CentralityReco *centralityreco = new CentralityReco();
  //se->registerSubsystem(centralityreco);

  ///////////////////////////////////
  // Validation 
  CaloValid *ca = new CaloValid("CaloValid");
  ca->set_timing_cut_width(200);
  se->registerSubsystem(ca);

  
  Fun4AllInputManager *In = new Fun4AllDstInputManager("in");
  In->AddFile(fname);
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

  TString qaname = fulloutfile_hist + "_qa.root";
  std::string qaOutputFileName(qaname.Data());
  QAHistManagerDef::saveQARootFile(qaOutputFileName);

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

#endif
