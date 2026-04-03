
#include <bcolumicount/BcoLumiReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <phool/recoConsts.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TSystem.h>

R__LOAD_LIBRARY(libbcolumicount.so)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4allraw.so)

void Fun4All_BcoLumi(const int nEvents = 0,
		     const std::string &inlist = "gl1daq.list",
		     const std::string &outfile = "DST_BCOINFO-00081000-00000.root")
{
  Fun4AllServer *se = Fun4AllServer::instance();
//  se->Verbosity(1);

  SyncReco *sync = new SyncReco();
//  sync->Verbosity(10);
  se->registerSubsystem(sync);

  BcoLumiReco *lumi = new BcoLumiReco();
//  lumi->Verbosity(10);
  se->registerSubsystem(lumi);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->AddListFile(inlist);
  se->registerInputManager(in);
  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out",outfile);
  se->registerOutputManager(out);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
