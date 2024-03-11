#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>

#include <phool/recoConsts.h>

//#include <ffarawmodules/Gl1Check.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

SingleGl1PoolInput *sngl[9]{};

void Fun4All_Gl1_Combiner(int nEvents = 0,
		      const string &input_file00 = "gl1.list"
)
{
  vector<string> infile;
  infile.push_back(input_file00);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
//  rc->set_IntFlag("RUNNUMBER",20445);
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
//  in->Verbosity(10);
  int i=0;
  for (auto iter : infile)
  {
    SingleGl1PoolInput *sngl= new SingleGl1PoolInput("GL1_" + to_string(i));
//    sngl->Verbosity(3);
    sngl->AddListFile(iter);
    in->registerStreamingInput(sngl,InputManagerType::GL1);
    i++;
  }
  se->registerInputManager(in);

//   Gl1Check *gl1check = new Gl1Check();
// //  gl1check->Verbosity(3);
//   se->registerSubsystem(gl1check);

   Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out","gl1.root");
out->UseFileRule();
out->SetNEvents(5);
   se->registerOutputManager(out);

    se->run(nEvents);

    se->End();
    delete se;
    gSystem->Exit(0);
}
