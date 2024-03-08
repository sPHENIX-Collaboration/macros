#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleMicromegasPoolInput.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_Tpot_Combiner(int nEvents = 0,
		      const string &input_file00 = "tpot.list"
)
{
  vector<string> infile;
  infile.push_back(input_file00);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
recoConsts *rc = recoConsts::instance();
//rc->set_IntFlag("RUNNUMBER",20445);
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  in->Verbosity(10);
  int i=0;
  for (auto iter : infile)
  {
    SingleMicromegasPoolInput *sngl= new SingleMicromegasPoolInput("MICROMEGAS_" + to_string(i));
    sngl->Verbosity(3);
    sngl->SetBcoRange(130);
    sngl->AddListFile(iter);
    in->registerStreamingInput(sngl,InputManagerType::MICROMEGAS);
    i++;
  }
  se->registerInputManager(in);

   // MicromegasCheck *micromegascheck = new MicromegasCheck();
   // micromegascheck->Verbosity(3);
   // micromegascheck->SetBcoRange(130);
   // se->registerSubsystem(micromegascheck);

   Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out","micromegas.root");
   se->registerOutputManager(out);

   se->run(nEvents);

   se->End();
   delete se;
   gSystem->Exit(0);
}
