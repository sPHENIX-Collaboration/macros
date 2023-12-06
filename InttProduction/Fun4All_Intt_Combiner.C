#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/SingleInttPoolInput.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <phool/recoConsts.h>

#include <ffarawmodules/InttCheck.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

SingleInttPoolInput *sngl[9]{};

void Fun4All_Intt_Combiner(int nEvents = 0,
		      const string &input_file00 = "intt0.list",
		      const string &input_file01 = "intt1.list",
		      const string &input_file02 = "intt2.list",
		      const string &input_file03 = "intt3.list",
		      const string &input_file04 = "intt4.list",
		      const string &input_file05 = "intt5.list",
		      const string &input_file06 = "intt6.list",
		      const string &input_file07 = "intt7.list"
)
{
  vector<string> infile;
  infile.push_back(input_file00);
  infile.push_back(input_file01);
  infile.push_back(input_file02);
  infile.push_back(input_file03);
  infile.push_back(input_file04);
  infile.push_back(input_file05);
  infile.push_back(input_file06);
  infile.push_back(input_file07);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
//  rc->set_IntFlag("RUNNUMBER",20445);
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
//  in->Verbosity(10);
  int i=0;
  for (auto iter : infile)
  {
    SingleInttPoolInput *sngl= new SingleInttPoolInput("INTT_" + to_string(i));
//    sngl->Verbosity(3);
    sngl->AddListFile(iter);
    in->registerStreamingInput(sngl,Fun4AllStreamingInputManager::INTT);
    i++;
  }
  se->registerInputManager(in);

//   InttCheck *inttcheck = new InttCheck();
// //  inttcheck->Verbosity(3);
//   se->registerSubsystem(inttcheck);

  // Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out","intt-00020445.root");
  // se->registerOutputManager(out);

    se->run(nEvents);

    se->End();
    delete se;
    gSystem->Exit(0);
}
