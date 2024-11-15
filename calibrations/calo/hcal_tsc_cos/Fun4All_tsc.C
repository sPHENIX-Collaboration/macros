#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <phool/recoConsts.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <calotrigger/MinimumBiasClassifier.h>
#include <centrality/CentralityInfo.h>

#include <caloreco/CaloTowerCalib.h>

#include <litecaloeval/LiteCaloEval.h>

#include <calovalid/CaloValid.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

#endif

void Fun4All_tsc(int nevents = 1000,const std::string &fname = "dst_calo-00023714.list")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = 0;

  se->Verbosity(verbosity);
  recoConsts *rc = recoConsts::instance();

  ifstream file(fname);
  string first_file;
  getline(file, first_file);

 //===============
  // conditions DB flags
  //===============
  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
  int runnumber = runseg.first;
   cout << "run number = " << runnumber << endl;

  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG","MDC2");
  // // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP",runnumber);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DST_TOWERS");
  in->AddListFile(fname);
  se->registerInputManager(in);

  std::string filename = first_file.substr(first_file.find_last_of("/\\") + 1);
  std::string OutFile = "CALOHIST_" + filename;

  ////////////////////
  // Calibrate towers
  CaloTowerCalib *calib = new CaloTowerCalib("CEMCCALIB");
  calib->set_detector_type(CaloTowerDefs::HCALOUT);
  calib->set_directURL("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_tsc_cos/cdbConv/ohcal_cdb_calib.root");
  se->registerSubsystem(calib);

  CaloTowerCalib *calib2 = new CaloTowerCalib("CEMCCALIB");
  calib2->set_detector_type(CaloTowerDefs::HCALIN);
  calib2->set_directURL("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_tsc_cos/cdbConv/ihcal_cdb_calib.root");
  se->registerSubsystem(calib2);

  LiteCaloEval *eval7e = new LiteCaloEval("EVALUATOR", "HCALOUT",Form("tsc_hcalout_%d.root",runnumber));
  eval7e->CaloType(LiteCaloEval::HCALOUT);
  eval7e->setInputTowerNodeName("TOWERINFO_CALIB_HCALOUT");
  se->registerSubsystem(eval7e);

  LiteCaloEval *eval7e2 = new LiteCaloEval("EVALUATOR2", "HCALIN",Form("tsc_hcalin_%d.root",runnumber));
  eval7e2->CaloType(LiteCaloEval::HCALIN);
  eval7e2->setInputTowerNodeName("TOWERINFO_CALIB_HCALIN");
  se->registerSubsystem(eval7e2);



  cout << "adding validation module" << endl;
  CaloValid *ca = new CaloValid("calomodulename",OutFile);
  ca->set_timing_cut_width(200);  //integers for timing width, > 1 : wider cut around max peak time
  ca->apply_vertex_cut(false);  
  ca->set_vertex_cut(20.);
  ca->set_debug(false);
  se->registerSubsystem(ca);

  //se->Verbosity(5);
  
  se->run(nevents); //update number of events as needed
  se->End();
  cout << "JOB COMPLETE :)" <<endl;

}
