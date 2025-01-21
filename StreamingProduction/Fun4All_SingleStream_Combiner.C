#include <QA.C>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>
#include <fun4allraw/SingleMicromegasPoolInput.h>
#include <fun4allraw/SingleMvtxPoolInput.h>
#include <fun4allraw/SingleTpcPoolInput.h>

#include <intt/InttOdbcQuery.h>

#include <phool/recoConsts.h>

#include <ffarawmodules/InttCheck.h>
#include <ffarawmodules/StreamingCheck.h>
#include <ffarawmodules/TpcCheck.h>

#include <ffamodules/HeadReco.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/SyncReco.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)
R__LOAD_LIBRARY(libintt.so)
bool isGood(const string &infile);

void Fun4All_SingleStream_Combiner(int nEvents = 0,
				   const int runnumber = 30117,
				   const string &outdir = "/sphenix/lustre01/sphnxpro/commissioning/slurp/tpccosmics/",
				   const string &type = "streaming",
		        const string &input_gl1file = "gl1daq.list",
		        const string &input_tpcfile00 = "tpc00.list",
		        const string &input_inttfile00 = "intt0.list",
                        const string &input_mvtxfile00 = "mvtx0.list",
                        const string &input_tpotfile = "tpot.list")
{
// GL1 which provides the beam clock reference (if we ran with GL1)
  vector<string> gl1_infile;
  gl1_infile.push_back(input_gl1file);

// MVTX
  vector<string> mvtx_infile;
  mvtx_infile.push_back(input_mvtxfile00);

// INTT
  vector<string> intt_infile;
  intt_infile.push_back(input_inttfile00);

  vector<string> tpc_infile;
  tpc_infile.push_back(input_tpcfile00);

// TPOT
  vector<string> tpot_infile;
  tpot_infile.push_back(input_tpotfile);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
//  in->Verbosity(3);

// create and register input managers
  int i = 0;

  std::string readoutNumber = "";

  for (auto iter : gl1_infile)
  {
    if (isGood(iter))
    {
      SingleGl1PoolInput *gl1_sngl = new SingleGl1PoolInput("GL1_" + to_string(i));
      //    gl1_sngl->Verbosity(3);
      gl1_sngl->AddListFile(iter);
      in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
      i++;
    }
  }
  i = 0;


  for (auto iter : intt_infile)
  {
    if (isGood(iter))
    {
    SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + to_string(i));
    //intt_sngl->Verbosity(3);
    InttOdbcQuery query;
    bool isStreaming = false;
    if(RunNumber != 0)
      {
	query.Query(RunNumber);
	isStreaming = query.IsStreaming();
      }
    intt_sngl->streamingMode(isStreaming);
    auto pos = iter.find("intt");
    std::string num = iter.substr(pos+4, 1);
    readoutNumber = "INTT"+num;
    intt_sngl->setHitContainerName("INTTRAWHIT_" + num);

    intt_sngl->AddListFile(iter);
    in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
    i++;
    }
  }
  i = 0;
  for (auto iter : mvtx_infile)
  {
    if (isGood(iter))
    {

      /// find the ebdc number from the filename
      std::string filepath, felix;
      std::ifstream ifs(iter);
      while(std::getline(ifs, filepath))
	{
	  auto pos = filepath.find("mvtx");
	  felix = filepath.substr(pos+4, 1);
	  break;
	}
      readoutNumber = "MVTX"+felix;
    SingleMvtxPoolInput *mvtx_sngl = new SingleMvtxPoolInput("MVTX_" + to_string(i));
//    mvtx_sngl->Verbosity(5);

    mvtx_sngl->setHitContainerName("MVTXRAWHIT_" + felix);
    mvtx_sngl->setRawEventHeaderName("MVTXRAWEVTHEADER_" + felix);
    mvtx_sngl->AddListFile(iter);
    in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
    i++;
    }
  }
  i = 0;
  for (auto iter : tpc_infile)
  {
    if (isGood(iter))
    {

      /// find the ebdc number from the filename
      std::string filepath, ebdc;
      std::ifstream ifs(iter);
      while(std::getline(ifs, filepath))
	{
	  auto pos = filepath.find("ebdc");
	  ebdc = filepath.substr(pos+4, 2);
	  break;
	}

    SingleTpcPoolInput *tpc_sngl = new SingleTpcPoolInput("TPC_" + to_string(i));
//    tpc_sngl->Verbosity(2);
    //   tpc_sngl->DryRun();
    readoutNumber = "TPC"+ebdc;
    tpc_sngl->SetBcoRange(5);
    tpc_sngl->setHitContainerName("TPCRAWHIT_" + ebdc);
    tpc_sngl->AddListFile(iter);
    in->registerStreamingInput(tpc_sngl, InputManagerType::TPC);
    i++;
    }
  }
  i = 0;

  for (auto iter : tpot_infile)
  {
    if (isGood(iter))
    {
    SingleMicromegasPoolInput *mm_sngl = new SingleMicromegasPoolInput("MICROMEGAS_" + to_string(i));
    //   sngl->Verbosity(3);
    mm_sngl->SetBcoRange(10);
    mm_sngl->SetNegativeBco(2);
    mm_sngl->SetBcoRange(10);
    mm_sngl->SetBcoPoolSize(50);
    mm_sngl->AddListFile(iter);
    in->registerStreamingInput(mm_sngl, InputManagerType::MICROMEGAS);
    readoutNumber = "TPOT";
    i++;
    }
  }

  se->registerInputManager(in);
  // StreamingCheck *scheck = new StreamingCheck();
  // scheck->SetTpcBcoRange(130);
  // se->registerSubsystem(scheck);
  // TpcCheck *tpccheck = new TpcCheck();
  // tpccheck->Verbosity(3);
  // tpccheck->SetBcoRange(130);
  // se->registerSubsystem(tpccheck);
  SyncReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  HeadReco *head = new HeadReco();
  se->registerSubsystem(head);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);


  char outfile[500];
  sprintf(outfile,"./%s-%s.root",type.c_str(),readoutNumber.c_str());

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out",outfile);
  se->registerOutputManager(out);

  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);

  se->End();

  char histoutfile[500];
  sprintf(histoutfile,"./HIST_%s-%s-%08i-%05i.root",type.c_str(),readoutNumber.c_str(),runnumber,0);
  QAHistManagerDef::saveQARootFile(histoutfile);

  delete se;
  cout << "all done" << endl;
  gSystem->Exit(0);
}

bool isGood(const string &infile)
{
  ifstream intest;
  intest.open(infile);
  bool goodfile = false;
  if (intest.is_open())
  {
    if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
    {
      goodfile = true;
    }
      intest.close();
  }
  return goodfile;
}
