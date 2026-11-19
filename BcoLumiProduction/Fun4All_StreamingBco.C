
#include <bcolumicount/StreamingBcoReco.h>

#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>


#include <phool/recoConsts.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TSystem.h>
#include <fstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libbcolumicount.so)
R__LOAD_LIBRARY(libffamodules.so)

void Fun4All_StreamingBco(const int nEvents = 0,
		      const std::string &bcodst = "DST_BCOINFO-00081100.root")
{

  std::string outfile = "DST_STREAMING_BCOINFO-";
  std::string histfile = "bco_histos-";

  // There is probably a better way to do this... revisit!
  std::string run_segments = bcodst.substr(bcodst.size() - 13, 8);
  outfile += run_segments;
  outfile += ".root";

  histfile += run_segments;
  histfile += ".root";

  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  //se->Verbosity(1);

  StreamingBcoReco *streaming_bco = new StreamingBcoReco();
  //streaming_bco->Verbosity(2);

  se->registerSubsystem(streaming_bco);
  //in->Verbosity(10);

  Fun4AllInputManager *in_dst = new Fun4AllDstInputManager("DSTin");
  in_dst->AddFile(bcodst);
  se->registerInputManager(in_dst);

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out",outfile);
  se->registerOutputManager(out);

  se->run(nEvents);

  se->dumpHistos(histfile);
  se->End();
  delete se;
  gSystem->Exit(0);
}
