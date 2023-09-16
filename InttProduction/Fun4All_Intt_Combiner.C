#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/SingleEvtInput.h>
#include <fun4allraw/Fun4AllEvtInputPoolManager.h>
#include <fun4all/Fun4AllOutputManager.h>

#include <intt/InttCombinedRawDataDecoder.h>

#include <ffarawmodules/EvtCheck.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libintt.so)

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

  Fun4AllEvtInputPoolManager *in = new Fun4AllEvtInputPoolManager("Comb");
//  in->Verbosity(10);
  for (auto iter : infile)
  {
    in->AddEvtInputList(iter);
  }
  se->registerInputManager(in);
  InttCombinedRawDataDecoder *inttdec = new InttCombinedRawDataDecoder();
  se->registerSubsystem(inttdec);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
