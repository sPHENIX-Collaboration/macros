#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4allraw/SingleGl1TriggeredInput.h>
#include <fun4allraw/Fun4AllTriggeredInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <ffarawmodules/ClockDiffCheck.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_New_Prdf_Combiner(int nEvents = 0,
			       const std::string &daqhost = "seb15",
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
  ifstream infile;
  SingleTriggeredInput *input = nullptr;
  for (int i=0; i<21; i++)
  {
    char daqhost[200];
    char daqlist[200];
    sprintf(daqhost,"seb%02d",i);
    sprintf(daqlist,"%s.list",daqhost);
    infile.open(daqlist);
    if (infile.is_open())
    {
      infile.close();
      input = new SingleTriggeredInput(daqhost);
      if (strcmp(daqhost,"seb18") == 0)
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
  std::string outfile = "DST_TRIGGERED_EVENT_" + daqhost + "_run2auau_new_nocdbtag_v004.root";
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
