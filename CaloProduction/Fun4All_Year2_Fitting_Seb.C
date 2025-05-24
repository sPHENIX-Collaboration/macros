#ifndef FUN4ALL_YEAR2_FITTING_SEB_C
#define FUN4ALL_YEAR2_FITTING_SEB_C

#include <Calo_Fitting.C>
#include <QA.C>

#include <calotrigger/TriggerRunInfoReco.h>

#include <calovalid/CaloFittingQA.h>

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

R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libcalotrigger.so)


// This will be the new fitting macro and the old one will be made obselete
// once we switch to the new event combiner
// this pass containis the reco process that's stable wrt time stamps(raw tower building)
void Fun4All_Year2_Fitting_Seb(int nEvents = 100,
                               const std::string &filelist = "DST_TRIGGERED_EVENT_run2pp_ana451_2024p009-00047748-00000.root",
                               const std::string &outfile = "DST_CALOFITTING_run2pp_ana451_2024p009-00047748-00000.root",
                               const std::string &outfile_hist = "HIST_CALOFITTINGQA_run2pp_ana451_2024p009-00047748-00000.root",
                               const std::string &dbtag = "ProdA_2024")
{
  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

  recoConsts *rc = recoConsts::instance();

  //copy from the tracking production code, as we are combining each seb with gl1 we need one input manager per seb input
  std::ifstream ifs(filelist);
  std::string filepath;
  int i = 0;
  while (std::getline(ifs, filepath))
  {
    std::cout << "Opening file: " << filepath << std::endl;
    if (i == 0)
    {
      std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(filepath);
      int runNumber = runseg.first;
      int segment = runseg.second;
      rc->set_IntFlag("RUNNUMBER", runNumber);
      rc->set_uint64Flag("TIMESTAMP", runNumber);
    }
    std::string inputname = "InputManager" + std::to_string(i);
    auto hitsin = new Fun4AllDstInputManager(inputname);
    hitsin->fileopen(filepath);
    se->registerInputManager(hitsin);
    i++;
  }

  // conditions DB flags and timestamp
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  CDBInterface::instance()->Verbosity(1);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  // Get info from DB and store in DSTs
  TriggerRunInfoReco *triggerinfo = new TriggerRunInfoReco();
  se->registerSubsystem(triggerinfo);

  Process_Calo_Fitting();

  ///////////////////////////////////
  // Validation
  CaloFittingQA *ca = new CaloFittingQA("CaloFittingQA");
  se->registerSubsystem(ca);


  Fun4AllDstOutputManager *outlower = new Fun4AllDstOutputManager("DSTOUT", outfile);
  out->AddNode("Sync");
  out->AddNode("EventHeader");
  out->AddNode("14001");
  out->AddNode("TOWERS_HCALIN");
  out->AddNode("TOWERS_HCALOUT");
  out->AddNode("TOWERS_CEMC");
  out->AddNode("TOWERS_SEPD");
  out->AddNode("TOWERS_ZDC");
  out->AddNode("1001");
  out->AddNode("1002");
  out->AddNode("TriggerRunInfo");
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
