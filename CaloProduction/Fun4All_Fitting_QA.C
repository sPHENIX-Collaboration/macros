#ifndef FUN4ALL_FITTING_QA_C
#define FUN4ALL_FITTING_QA_C

#include <calovalid/CaloFittingQA.h>

#include <mbd/MbdReco.h>

#include <qautils/QAHistManagerDef.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <phool/recoConsts.h>

#include <TSystem.h>

#include <fstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libcalovalid.so)

void Fun4All_Fitting_QA(int nEvents = 100,
                        const std::string &inlist = "/cvmfs/sphenix.sdcc.bnl.gov/files/calofitting/files.list",
                        const std::string &infilefits = "/cvmfs/sphenix.sdcc.bnl.gov/files/calofitting/DST_CALOFITTING_run3auau_pro001_pcdb001_v001-00068114-90000.root",
                        const std::string &outfile_hist = "HIST_CALOFITTINGQA_run3auau_pro001_pcdb001_v001-00068114-90000.root",
                        const std::string &dbtag = "newcdbtag")
{
  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  se->VerbosityDownscale(10000);

  recoConsts *rc = recoConsts::instance();

  // conditions DB flags and timestamp
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);

  CDBInterface::instance()->Verbosity(1);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  /////////////////////
  // MBD: full reco from the raw packets in the triggered DSTs (provides MbdOut t0)
  MbdReco *mbdreco = new MbdReco();
  se->registerSubsystem(mbdreco);

  ///////////////////////////////////
  // Validation
  CaloFittingQA *ca = new CaloFittingQA("CaloFittingQA");
  se->registerSubsystem(ca);

  Fun4AllInputManager *In = nullptr;
  std::ifstream infile;
  infile.open(inlist);
  int iman = 0;
  std::string line;
  if (infile.is_open())
  {
    bool first{true};
    while (std::getline(infile, line))
    {
      if (line[0] == '#')
      {
        std::cout << "found commented out line " << line << std::endl;
        continue;
      }
      if (first)
      {
        std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(line);
        int runnumber = runseg.first;
        rc->set_uint64Flag("TIMESTAMP", runnumber);
        first = false;
      }

      std::cout << line << std::endl;
      std::string magname = "DSTin_" + std::to_string(iman);
      In = new Fun4AllDstInputManager(magname);
      In->Verbosity(1);
      In->AddFile(line);
      se->registerInputManager(In);
      iman++;
    }
    infile.close();
  }

  In = new Fun4AllDstInputManager("inCALOFITTING");
  In->Verbosity(1);
  In->AddFile(infilefits);
  se->registerInputManager(In);

  if (nEvents < 0)
  {
    return;
  }
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
