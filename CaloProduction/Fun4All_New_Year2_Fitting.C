#ifndef FUN4ALL_YEAR2_FITTING_C
#define FUN4ALL_YEAR2_FITTING_C

#include <Calo_Fitting.C>
#include <QA.C>

#include <calotrigger/TriggerRunInfoReco.h>

#include <calovalid/CaloFittingQA.h>

#include <calopacketskimmer/CaloPacketSkimmer.h>

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

#include <format>
#include <fstream>

R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libCaloPacketSkimmer.so);

// this pass containis the reco process that's stable wrt time stamps(raw tower building)
void Fun4All_New_Year2_Fitting(int nEvents = 100,
			   const std::string &inlist = "files.list",
                           const std::string &outfile = "DST_CALOFITTING_run2auau_ana487_2024p018_v001",
                           const std::string &outfile_hist = "HIST_CALOFITTINGQA_run2auau_ana487_2024p018_v001",
                           const std::string &dbtag = "ProdA_2024")
{
  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  se->VerbosityDownscale(1000);

  recoConsts *rc = recoConsts::instance();

  // conditions DB global tag
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

// loop over all files in file list and create an input manager for each one  
  Fun4AllInputManager *In = nullptr;
  std::ifstream infile;
  infile.open(inlist);
  int iman = 0;
  std::string line;
  bool first {true};
  int runnumber = 0;
  int segment = 99999;
  if (infile.is_open())
  {
    while (std::getline(infile, line))
    {
      if (line[0] == '#')
      {
	std::cout << "found commented out line " << line << std::endl;
	continue;
      }
      // extract run number from first not commented out file in list
      if (first)
      {
	std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(line);
	runnumber = runseg.first;
	segment = runseg.second;
	rc->set_uint64Flag("TIMESTAMP", runnumber);
	first = false;
      }
      std::string magname = "DSTin_" + std::to_string(iman);
      In = new Fun4AllDstInputManager(magname);
      In->Verbosity(1);
      In->AddFile(line);
      se->registerInputManager(In);
      iman++;
    }
    infile.close();
  }
  
// this strips all nodes under the Packets PHCompositeNode
// (means removes all offline packets)
  std::string dstoutfile = std::format("{}-{:08}-{:05}.root",outfile, runnumber,segment);
  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", dstoutfile);
  out->StripCompositeNode("Packets");
  se->registerOutputManager(out);
  // se->Print();
  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);
  se->End();
  
  dstoutfile = std::format("{}-{:08}-{:05}.root",outfile_hist, runnumber,segment);
  QAHistManagerDef::saveQARootFile(dstoutfile);

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
#endif
