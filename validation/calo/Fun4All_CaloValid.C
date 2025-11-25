#ifndef MACRO_FUN4ALL_CALOVALID_C
#define MACRO_FUN4ALL_CALOVALID_C

#include <QA.C>

#include <calotrigger/MinimumBiasClassifier.h>

#include <caloreco/CaloTowerCalib.h>

#include <calovalid/CaloValid.h>

#include <centrality/CentralityInfo.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <phool/recoConsts.h>

#include <format>
#include <fstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)

void Fun4All_CaloValid(int nevents = 1e5,const std::string &fname = "example.list")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = 0;

  se->Verbosity(verbosity);
  recoConsts *rc = recoConsts::instance();

  std::ifstream file(fname);
  std::string first_file;
  getline(file, first_file);

 //===============
  // conditions DB flags
  //===============
  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
  int runnumber = runseg.first;
  int segment = runseg.second;
   std::cout << "run number = " << runnumber << std::endl;

  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG","MDC2");
  // // 64 bit timestamp
  rc->set_uint64Flag("TIMESTAMP",runnumber);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DST_TOWERS");
  in->AddListFile(fname);
  se->registerInputManager(in);

  std::string fulloutfile_hist = std::format("./HIST_CALOR_QA-{:08}-{:04}.root", runnumber, segment);



  CaloValid *ca = new CaloValid("CaloValid");
  ca->set_timing_cut_width(200);  //integers for timing width, > 1 : wider cut around max peak time
  //ca->set_debug(false);
  se->registerSubsystem(ca);


  
  se->run(nevents); //update number of events as needed
  se->End();

  QAHistManagerDef::saveQARootFile(fulloutfile_hist);

  std::cout << "JOB COMPLETE :)" << std::endl;

}

#endif
