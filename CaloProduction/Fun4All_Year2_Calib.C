#ifndef FUN4ALL_YEAR2_C
#define FUN4ALL_YEAR2_C

#include <QA.C>
#include <Calo_Calib.C>

#include <mbd/MbdReco.h>

#include <zdcinfo/ZdcReco.h>

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
#include <globalqa/GlobalQA.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libglobalvertex.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libglobalQA.so)

void Fun4All_Year2_Calib(int nEvents=100,
                   const std::string &fname = "DST_CALOFITTING-00000000-000000.root",
                   const std::string& outfile= "DST_CALO-00000000-000000.root",
                   const std::string& outfile_hist= "HIST_CALOQA-00000000-000000.root",
                   const std::string& dbtag= "ProdA_2024"
  )
{

  // towerinfov1=kPRDFTowerv1, v2=:kWaveformTowerv2, v3=kPRDFWaveform, v4=kPRDFTowerv4
  CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFTowerv4;

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

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

  //ZDC Reconstruction--Calib Info
  ZdcReco *zdcreco = new ZdcReco();
  se->registerSubsystem(zdcreco);

  // Official vertex storage
  GlobalVertexReco *gvertex = new GlobalVertexReco();
  se->registerSubsystem(gvertex);

  /////////////////////
  // Geometry 
  std::cout << "Adding Geometry file" << std::endl;
  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2);


  /////////////////////////////////////////////////////
  // Set status of towers, Calibrate towers,  Cluster
  Process_Calo_Calib();

  ///////////////////////////////////
  // Validation 
  CaloValid *ca = new CaloValid("CaloValid");
  ca->set_timing_cut_width(200);
  se->registerSubsystem(ca);

  GlobalQA *gqa = new GlobalQA("GlobalQA");
  se->registerSubsystem(gqa);
  
  Fun4AllInputManager *In = new Fun4AllDstInputManager("in");
  In->AddFile(fname);
  se->registerInputManager(In);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile);
  out->StripNode("TOWERS_CEMC");
  out->StripNode("TOWERS_HCALIN");
  out->StripNode("TOWERS_HCALOUT");
  out->StripNode("TOWERS_ZDC");
  out->StripNode("TOWERS_SEPD");
  out->StripNode("MBDPackets");
  se->registerOutputManager(out);

  se->run(nEvents);
  se->End();

  QAHistManagerDef::saveQARootFile(outfile_hist);

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

#endif
