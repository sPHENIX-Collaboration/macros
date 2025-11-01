#include <ffarawmodules/Gl1BcoDump.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TSystem.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_Gl1QA(int nEvents = 0,
		   const std::string &input_list = "gl1daq.list",
		   const std::string &outfile = "gl1daq.root")
{
  Fun4AllServer *se = Fun4AllServer::instance();
//  se->Verbosity(1);
  Gl1BcoDump *gl1dmp = new Gl1BcoDump();
  gl1dmp->OutFileName(outfile);
  se->registerSubsystem(gl1dmp);
  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->AddListFile(input_list);
  se->registerInputManager(in);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
