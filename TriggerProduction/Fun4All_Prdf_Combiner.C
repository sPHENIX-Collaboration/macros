#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <ffarawmodules/ClockDiffCheck.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <fun4allraw/SingleGl1TriggeredInput.h>
#include <fun4allraw/Fun4AllTriggeredInputManager.h>
#include <fun4allraw/InputManagerType.h>


#include <Rtypes.h>
#include <TSystem.h>

#include <format>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_New_Prdf_Combiner(int nEvents = 0,
			       const std::string &daqhost_out = "seb15",
			       const std::string &outdir = "/sphenix/user/pinkenbu")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
//  se->VerbosityDownscale(100000);
  Fun4AllTriggeredInputManager *in = new Fun4AllTriggeredInputManager("Tin");
  SingleTriggeredInput *gl1 = new SingleGl1TriggeredInput("Gl1in");
  gl1->KeepPackets();
  gl1->AddListFile("gl1daq.list");
//  gl1->Verbosity(10);
  in->registerGl1TriggeredInput(gl1);
  std::ifstream infile;
  SingleTriggeredInput *input = nullptr;
  for (int i=0; i<21; i++)
  {
    std::string daqhost = std::format("seb{:02}",i);
    std::string daqlist = std::format("{}.list",daqhost);
    infile.open(daqlist);
    if (infile.is_open())
    {
      infile.close();
      input = new SingleTriggeredInput(daqhost);
      if (daqhost == "seb18")
      {
	input->KeepPackets();
      }
//  input->Verbosity(10);
//      input->FakeProblemEvent(10);
      input->AddListFile(daqlist);
      in->registerTriggeredInput(input);
    }
  }
  se->registerInputManager(in);

  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);
  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

// not functional yet
  ClockDiffCheck *clkchk = new ClockDiffCheck();
//   clkchk->Verbosity(3);
 clkchk->set_delBadPkts(true);
  se->registerSubsystem(clkchk);
  std::string outfile = outdir + "/DST_TRIGGERED_EVENT_" + daqhost_out + "_run2auau_new_nocdbtag_v004.root";
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("dstout",outfile);
  out->SplitLevel(0);
  out->UseFileRule();
  out->SetNEvents(100000);
//  out->SetClosingScript("copyscript.pl");
//  out->SetClosingScriptArgs(" -mv -outdir " + outdir);
//  se->registerOutputManager(out);
  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);
  se->End();
  delete se;
  std::cout << "all done" << std::endl;
  gSystem->Exit(0);
}
