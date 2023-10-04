#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/SingleTpcInput.h>
#include <fun4allraw/Fun4AllEvtInputPoolManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

SingleTpcInput *sngl[24]{};

void Fun4All_Tpc_Combiner(int nEvents = 0,
		      const string &input_file00 = "tpc00.list",
		      const string &input_file01 = "tpc01.list",
		      const string &input_file02 = "tpc02.list",
		      const string &input_file03 = "tpc03.list",
		      const string &input_file04 = "tpc04.list",
		      const string &input_file05 = "tpc05.list",
		      const string &input_file06 = "tpc06.list",
		      const string &input_file07 = "tpc07.list",
		      const string &input_file08 = "tpc08.list",
		      const string &input_file09 = "tpc09.list",
		      const string &input_file10 = "tpc10.list",
		      const string &input_file11 = "tpc11.list",
		      const string &input_file12 = "tpc12.list",
		      const string &input_file13 = "tpc13.list",
		      const string &input_file14 = "tpc14.list",
		      const string &input_file15 = "tpc15.list",
		      const string &input_file16 = "tpc16.list",
		      const string &input_file17 = "tpc17.list",
		      const string &input_file18 = "tpc18.list",
		      const string &input_file19 = "tpc19.list",
		      const string &input_file20 = "tpc20.list",
		      const string &input_file21 = "tpc21.list",
		      const string &input_file22 = "tpc22.list",
		      const string &input_file23 = "tpc23.list"
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
  infile.push_back(input_file08);
  infile.push_back(input_file09);
  infile.push_back(input_file10);
  infile.push_back(input_file11);
  infile.push_back(input_file12);
  infile.push_back(input_file13);
  infile.push_back(input_file14);
  infile.push_back(input_file15);
  infile.push_back(input_file16);
  infile.push_back(input_file17);
  infile.push_back(input_file18);
  infile.push_back(input_file19);
  infile.push_back(input_file20);
  infile.push_back(input_file21);
  infile.push_back(input_file22);
  infile.push_back(input_file23);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
recoConsts *rc = recoConsts::instance();
//rc->set_IntFlag("RUNNUMBER",20445);
  Fun4AllEvtInputPoolManager *in = new Fun4AllEvtInputPoolManager("Comb");
//  in->Verbosity(10);
  int i=0;
  for (auto iter : infile)
  {
    SingleTpcInput *sngl= new SingleTpcInput("TPC_" + to_string(i));
//    sngl->Verbosity(3);
    sngl->AddListFile(iter);
    in->registerStreamingInput(sngl,Fun4AllEvtInputPoolManager::TPC);
    i++;
  }
  se->registerInputManager(in);

  // Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out","test.root");
  // se->registerOutputManager(out);

   se->run(nEvents);

   se->End();
   delete se;
   gSystem->Exit(0);
}
