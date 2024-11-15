#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/SinglePrdfInput.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputPoolManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4allraw/Fun4AllEventOutputManager.h>
#include <ffarawmodules/EventNumberCheck.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_Combiner(int nEvents = 0,
		      const string &input_file10 = "hcaleast.list",
		      const string &input_file11 = "hcalwest.list"
) {
  vector<string> infile;
  infile.push_back(input_file11);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  Fun4AllPrdfInputPoolManager *in = new Fun4AllPrdfInputPoolManager("Comb");
  in->Verbosity(1);
  in->AddPrdfInputList(input_file10)->MakeReference(true);

  for (auto iter : infile) {
    in->AddPrdfInputList(iter);
  }
  se->registerInputManager(in);

  Fun4AllEventOutputManager *out = new Fun4AllEventOutputManager("EvtOut","/sphenix/tg/tg01/commissioning/CaloCalibWG/bseidlitz/cosmics-%08d-%04d.prdf",30000);
  se->registerOutputManager(out);

  if (nEvents < 0) return;
  se->run(nEvents);
//  in->Print();
  se->End();
  delete se;
  gSystem->Exit(0);
}
