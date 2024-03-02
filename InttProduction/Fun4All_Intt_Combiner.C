#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleInttPoolInput.h>

#include <phool/recoConsts.h>

#include <ffarawmodules/InttCheck.h>

#include <intt/InttCombinedRawDataDecoder.h>
#include <Trkr_Clustering.C>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libintt.so)

SingleInttPoolInput *sngl[9]{};

void Fun4All_Intt_Combiner(int nEvents = 0,
                           const string &input_file00 = "intt0.list",
                           const string &input_file01 = "intt1.list",
                           const string &input_file02 = "intt2.list",
                           const string &input_file03 = "intt3.list",
                           const string &input_file04 = "intt4.list",
                           const string &input_file05 = "intt5.list",
                           const string &input_file06 = "intt6.list",
                           const string &input_file07 = "intt7.list")
{
  bool runTrkrHits = false;
  bool runTkrkClus = false;
  bool stripRawHit = false;

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
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  //  in->Verbosity(10);
  int i = 0;
  for (auto iter : infile)
  {
    SingleInttPoolInput *sngl = new SingleInttPoolInput("INTT_" + to_string(i));
    //    sngl->Verbosity(3);
    sngl->AddListFile(iter);
    int nBcoVal = runTrkrHits ? 0 : 2;
    sngl->SetNegativeBco(nBcoVal);
    sngl->SetBcoRange(2);
    in->registerStreamingInput(sngl, InputManagerType::INTT);
    i++;
  }

  se->registerInputManager(in);

  //  InttCheck *inttcheck = new InttCheck();
  //  inttcheck->Verbosity(3);
  //  se->registerSubsystem(inttcheck);

  if (runTrkrHits)
  {
    InttCombinedRawDataDecoder *myDecoder = new InttCombinedRawDataDecoder("myUnpacker");
    myDecoder->runInttStandalone(true);
    myDecoder->writeInttEventHeader(true);
    se->registerSubsystem(myDecoder);
  }

  if (runTkrkClus)
  {
    Intt_Clustering(); //Be careful!!! INTT z-clustering may be off which is not what you want!
  }

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "intt-00020869.root");
  if (stripRawHit)
  {
    out->StripNode("INTTRAWHIT");
  }
  se->registerOutputManager(out);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
