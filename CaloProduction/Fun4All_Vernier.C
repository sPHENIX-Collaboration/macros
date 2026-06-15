#ifndef FUN4ALL_VERNIER_C
#define FUN4ALL_VERNIER_C

#include <mbd/MbdReco.h>

#include <caloreco/CaloTowerBuilder.h>

#include <calotrigger/TriggerRunInfoReco.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <TSystem.h>

#include <fstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalotrigger.so)

// this pass containis the reco process that's stable wrt time stamps(raw tower building)
void Fun4All_Vernier(int nEvents = 100,
                     const std::string &inlist = "files.list",
                     const std::string &outfile1 = "DST_Vernier_run3auau_pro001_pcdb001_v001-00054733-00000.root",
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

  MbdReco *mbd = new MbdReco();
  mbd->DoOnlyFits();
  se->registerSubsystem(mbd);

  CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFTowerv4;

  // build ZDC towers
  CaloTowerBuilder *caZDC = new CaloTowerBuilder("ZDCBUILDER");
  caZDC->set_detector_type(CaloTowerDefs::ZDC);
  caZDC->set_builder_type(buildertype);
  caZDC->set_processing_type(CaloWaveformProcessing::FUNCFIT);
  caZDC->set_funcfit_type(2);
  caZDC->set_nsamples(16);
  caZDC->set_offlineflag();
  se->registerSubsystem(caZDC);

  Fun4AllInputManager *In{nullptr};
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
  out->AddNode("EventHeader");
  out->AddNode("Sync");
  out->AddNode("14001");
  out->AddNode("MbdRawContainer");
  out->AddNode("TOWERS_ZDC");
  se->registerOutputManager(out);

  // se->Print();
  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);
  se->End();

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
#endif
