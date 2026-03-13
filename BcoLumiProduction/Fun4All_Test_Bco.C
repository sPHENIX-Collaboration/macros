
#include <bcolumicount/BcoLumiCheck.h>

#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <phool/recoConsts.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TSystem.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libbcolumicount.so)
R__LOAD_LIBRARY(libffamodules.so)

void Fun4All_Test_Bco(const int nEvents = 0,
		      const std::string &inlist = "streaming.list",
		      const std::string &bcodst = "DST_BCOINFO-00081000-00000.root")
{
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

  BcoLumiCheck *lumi = new BcoLumiCheck();
  lumi->Verbosity(10);
  se->registerSubsystem(lumi);
  Fun4AllInputManager *in = new Fun4AllDstInputManager("in1");
  in->Verbosity(10);
  in->AddListFile(inlist);
  se->registerInputManager(in);
in = new Fun4AllDstInputManager("in2");
in->AddFile(bcodst);
  se->registerInputManager(in);

  // Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out","combined.root");
  // se->registerOutputManager(out);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
