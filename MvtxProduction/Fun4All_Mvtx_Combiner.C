#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleMvtxPoolInput.h>

#include <phool/recoConsts.h>

#include <array>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)


void Fun4All_Mvtx_Combiner(int nEvents = 0,
		      const string &input_file00 = "mvtx-flx0.list",
		      const string &input_file01 = "mvtx-flx1.list",
		      const string &input_file02 = "mvtx-flx2.list",
		      const string &input_file03 = "mvtx-flx3.list",
		      const string &input_file04 = "mvtx-flx4.list",
		      const string &input_file05 = "mvtx-flx5.list")
{
  vector<string> infiles;
  infiles.push_back(input_file00);
  infiles.push_back(input_file01);
  infiles.push_back(input_file02);
  infiles.push_back(input_file03);
  infiles.push_back(input_file04);
  infiles.push_back(input_file05);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
  //rc->set_IntFlag("RUNNUMBER",20445);
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");

//  in->Verbosity(10);
  int i = 0;
  for (auto& infile : infiles)
  {
    auto* sngl= new SingleMvtxPoolInput("MVTX_FLX" + to_string(i));
//    sngl->Verbosity(3);
    sngl->SetBcoRange(10);
    sngl->AddListFile(infile);
    in->registerStreamingInput(sngl, InputManagerType::MVTX);
    i++;
  }
  se->registerInputManager(in);

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out","mvtx_raw_hits.root");
  se->registerOutputManager(out);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
