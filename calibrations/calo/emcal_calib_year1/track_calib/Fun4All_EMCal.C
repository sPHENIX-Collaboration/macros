#ifndef FUN4ALL_EMCAL_C
#define FUN4ALL_EMCAL_C

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>

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

#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <ffamodules/CDBInterface.h>
#include <GlobalVariables.C>

#include <litecaloeval/LiteCaloEval.h>
#include <calib_emc_pi0/pi0EtaByEta.h>

R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)

void createLocalEMCalCalibFile(const string fname, int runNumber);




void Fun4All_EMCal(int nevents = 100, const std::string &fname = "inputdata.txt")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  // se->Verbosity(verbosity);
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
  rc->set_StringFlag("CDB_GLOBALTAG", "MDC2");
  // // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DST_TOWERS");
  in->AddListFile(fname);
  se->registerInputManager(in);

  std::string filename = first_file.substr(first_file.find_last_of("/\\") + 1);
  std::string OutFile = Form("OUTHIST_%s",filename.c_str());


    LiteCaloEval *eval7e = new LiteCaloEval("CEMCEVALUATOR2", "CEMC",OutFile);
    eval7e->CaloType(LiteCaloEval::CEMC);
    eval7e->setInputTowerNodeName("TOWERINFO_CALIB_CEMC");
    se->registerSubsystem(eval7e);


  se->run(nevents);
  se->End();
  se->PrintTimer();
  delete se;

}




#endif
