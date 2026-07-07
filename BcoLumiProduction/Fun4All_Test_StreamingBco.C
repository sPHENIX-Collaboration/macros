
#include <bcolumicount/StreamingBcoCheck.h>

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

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libbcolumicount.so)
R__LOAD_LIBRARY(libffamodules.so)

void Fun4All_Test_StreamingBco(const int nEvents = 0,
          const std::string &bcodst = "DST_STREAMING_BCOINFO-00052050.root")
{
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  //se->Verbosity(1);

  StreamingBcoCheck *streaming_bco_check = new StreamingBcoCheck();
  //streaming_bco_check->Verbosity(10);
  se->registerSubsystem(streaming_bco_check);


  Fun4AllInputManager *in_dst = new Fun4AllDstInputManager("in");
  in_dst->AddFile(bcodst);
  se->registerInputManager(in_dst);


  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
