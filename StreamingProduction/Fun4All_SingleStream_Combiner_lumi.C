#include <QA.C>

#include <rawbcolumi/Fun4AllStreamingLumiCountingInputManager.h>
#include <rawbcolumi/SingleGl1PoolInputv2.h>
#include <rawbcolumi/SingleStreamingInputv2.h>


#include <ffarawmodules/InttCheck.h>
#include <ffarawmodules/StreamingCheck.h>
#include <ffarawmodules/TpcCheck.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <TSystem.h>

#include <format>
#include <fstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(librawbcolumi.so)
R__LOAD_LIBRARY(libffarawmodules.so)

bool isGood(const std::string &infile);

void Fun4All_SingleStream_Combiner_lumi(int nEvents = 200,
                                        const int  /*runnumber*/ = 52050,
                                        const std::string &output_file = "output_streaming",
                                        //				   const std::string &outdir = "/sphenix/user/xuzhiwan/luminosity/streaming-macro/fun4alllumi/macro/output/",
                                        const std::string & /*type*/ = "streaming",
                                        const std::string &input_gl1file = "gl1daq.list")
{
  // GL1 which provides the beam clock reference (if we ran with GL1)

  std::vector<std::string> gl1_infile;
  gl1_infile.push_back(input_gl1file);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
//  recoConsts *rc = recoConsts::instance();
  // Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  Fun4AllStreamingLumiCountingInputManager *in = new Fun4AllStreamingLumiCountingInputManager("Comb");
  //  in->Verbosity(3);
  in->SetOutputFileName(output_file);

  // create and register input managers
  int i = 0;


  for (const auto& iter : gl1_infile)
  {
    if (isGood(iter))
    {
      SingleGl1PoolInputv2 *gl1_sngl = new SingleGl1PoolInputv2("GL1_" + std::to_string(i));
      //    gl1_sngl->Verbosity(3);
      gl1_sngl->AddListFile(iter);
      gl1_sngl->SetNegativeWindow(20);
      gl1_sngl->SetPositiveWindow(325);  // 325
      gl1_sngl->SetTotalEvent(nEvents);
      in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
      i++;
    }
  }
  se->registerInputManager(in);

  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  // std::string readoutNumber = "Events-" + std::to_string(nEvents) + "-";

  // std::string outfile = std::format("./{}-{}_lumi.root",type, readoutNumber);

  // Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out",outfile);
  // se->registerOutputManager(out);

  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);
  se->End();

  /*  char histoutfile[500];
    sprintf(histoutfile,"./Lumi_%s-%s-%08i-%05i-%s",type.c_str(),readoutNumber.c_str(),runnumber,0,output_file.c_str());
    QAHistManagerDef::saveQARootFile(histoutfile);
  */

  delete se;
  std::cout << "all done" << std::endl;
  gSystem->Exit(0);
}

bool isGood(const std::string &infile)
{
  std::ifstream intest;
  intest.open(infile);
  bool goodfile = false;
  if (intest.is_open())
  {
    if (intest.peek() != std::ifstream::traits_type::eof())  // is it non zero?
    {
      goodfile = true;
    }
    intest.close();
  }
  return goodfile;
}
