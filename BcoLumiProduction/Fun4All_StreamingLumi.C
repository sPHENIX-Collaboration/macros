
#include <bcolumicount/StreamingLumiReco.h>
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

#include <Trkr_BcoCounting.C>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libbcolumicount.so)
R__LOAD_LIBRARY(libffamodules.so)

void Fun4All_StreamingLumi(const int nEvents = 0,
		      const std::string &infile = "DST_BCOINFO-00079516.root"
          //const std::string &outdir = "/sphenix/tg/tg01/coldqcd/dillfitz/singletrackan/lumi/"
        )
{

  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  //se->Verbosity(1);

  Fun4AllInputManager *in_bco_dst = new Fun4AllDstInputManager("BCO_DSTin");
  in_bco_dst->AddFile(infile);
  se->registerInputManager(in_bco_dst);

  // This is coming from ../common/Trkr_BcoCounting.C 
  StreamingBco();
  StreamingLumi();

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
