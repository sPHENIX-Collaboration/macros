
#include <bcolumicount/StreamingLumiReco.h>

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

void Fun4All_StreamingLumi(const int nEvents = 0,
		      const std::string &inlist = "gl1daq.list")
{

  std::string streaming_bcodst = "DST_STREAMING_BCOINFO-";
  std::string outfile = "DST_STREAMING_LUMIINFO-";
  std::ifstream file("gl1daq.list");  // open the file
  if (!file.is_open()) {
    std::cerr << "Failed to open file\n";
  }

  // There is probably a better way to do this... revisit!
  std::string infilename;
  if (std::getline(file, infilename)) {
      std::string run_segments = infilename.substr(infilename.size() - 17, 8);
      outfile += run_segments;
      outfile += ".root";
      streaming_bcodst += run_segments;
      streaming_bcodst += ".root";
  } else {
      std::cout << "File is empty or read failed\n";
  }
  gSystem->Load("libg4dst.so");
  Fun4AllServer *se = Fun4AllServer::instance();
  //se->Verbosity(1);

  StreamingLumiReco *streaming_lumi = new StreamingLumiReco();
  //streaming_lumi->Verbosity(2);

  se->registerSubsystem(streaming_lumi);
  Fun4AllPrdfInputManager *in_prdf = new Fun4AllPrdfInputManager("PRDFin");
  //in->Verbosity(10);
  in_prdf->AddListFile(inlist);
  se->registerInputManager(in_prdf);

  Fun4AllInputManager *in_dst = new Fun4AllDstInputManager("DSTin");
  in_dst->AddFile(streaming_bcodst);
  se->registerInputManager(in_dst);

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out",outfile);
  se->registerOutputManager(out);

  se->run(nEvents);

  se->End();
  delete se;
  gSystem->Exit(0);
}
