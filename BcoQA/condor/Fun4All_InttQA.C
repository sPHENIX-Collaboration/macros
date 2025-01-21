#include <QA.C>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <ffarawmodules/InttBcoDump.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_InttQA(int nEvents = 0,
		   const std::string &input_list = "prdf.list",
		   const std::string outfile = "inttbcodiff.root")
{
  Fun4AllServer *se = Fun4AllServer::instance();
//  se->Verbosity(1);

  InttBcoDump *inttdmp = new InttBcoDump;
  inttdmp->OutFileName(outfile);
  se->registerSubsystem(inttdmp);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->AddListFile(input_list);
  se->registerInputManager(in);

  se->run(nEvents);

  se->End();

  delete se;
  std::cout <<"Finished"<<std::endl;
  gSystem->Exit(0);
}
