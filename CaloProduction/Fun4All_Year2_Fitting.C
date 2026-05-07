#ifndef FUN4ALL_YEAR2_FITTING_C
#define FUN4ALL_YEAR2_FITTING_C

#include <Calo_Fitting.C>
#include <QA.C>

#include <calotrigger/TriggerRunInfoReco.h>

#include <calovalid/CaloFittingQA.h>

#include <calopacketskimmer/CaloPacketSkimmer.h>

#include <mbd/MbdReco.h>

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

#include <TSystem.h>

#include <fstream>

R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libCaloPacketSkimmer.so)

// this pass containis the reco process that's stable wrt time stamps(raw tower building)
void Fun4All_Year2_Fitting(int nEvents = 100,
                           const std::string &inlist = "files.list",
                           const std::string &outfile1 = "DST_CALOFITTING_run3oo_pro1_newcdbtag_v008-00082703-00000.root",
                           const std::string &outfile2 = "DST_ZDC_RAW_run3oo_pro1_newcdbtag_v008-00082703-00000.root",
                           const std::string &outfile_hist = "HIST_CALOFITTINGQA_run3auau_new_newcdbtag_v008-00082703-00000.root",
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

  // Get info from DB and store in DSTs
  TriggerRunInfoReco *triggerinfo = new TriggerRunInfoReco();
  se->registerSubsystem(triggerinfo);

  CaloPacketSkimmer *calopacket = new CaloPacketSkimmer();
  se->registerSubsystem(calopacket);

  Process_Calo_Fitting();

  ///////////////////////////////////
  // Validation
  CaloFittingQA *ca = new CaloFittingQA("CaloFittingQA");
  se->registerSubsystem(ca);

  // Fun4AllInputManager *In = new Fun4AllDstInputManager("in");
  // In->AddFile(fname);
  // se->registerInputManager(In);
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
  if (iman == 0)
  {
    std::cout << "No files in filelist" << std::endl;
    gSystem->Exit(1);
  }
  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile1);
  out->StripCompositeNode("Packets");
  out->StripNode("12001");
  se->registerOutputManager(out);
  out = new Fun4AllDstOutputManager("DSTOUTzdc", outfile2);
  out->AddNode("EventHeader");
  out->AddNode("Sync");
  out->AddNode("12001");
  se->registerOutputManager(out);
  // se->Print();
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
