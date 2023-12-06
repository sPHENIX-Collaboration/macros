#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>
#include <fun4allraw/SingleMicromegasPoolInput.h>
#include <fun4allraw/SingleMvtxPoolInput.h>
#include <fun4allraw/SingleTpcPoolInput.h>

#include <phool/recoConsts.h>

#include <ffarawmodules/InttCheck.h>
#include <ffarawmodules/StreamingCheck.h>
#include <ffarawmodules/TpcCheck.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_Stream_Combiner(int nEvents = 0,
                             const string &input_gl1file = "gl1.list",
                             const string &input_inttfile00 = "intt0.list",
                             const string &input_inttfile01 = "intt1.list",
                             const string &input_inttfile02 = "intt2.list",
                             const string &input_inttfile03 = "intt3.list",
                             const string &input_inttfile04 = "intt4.list",
                             const string &input_inttfile05 = "intt5.list",
                             const string &input_inttfile06 = "intt6.list",
                             const string &input_inttfile07 = "intt7.list",
                             const string &input_mvtxfile00 = "mvtx-flx0.list",
                             const string &input_mvtxfile01 = "mvtx-flx1.list",
                             const string &input_mvtxfile02 = "mvtx-flx2.list",
                             const string &input_mvtxfile03 = "mvtx-flx3.list",
                             const string &input_mvtxfile04 = "mvtx-flx4.list",
                             const string &input_mvtxfile05 = "mvtx-flx5.list",
                             const string &input_tpcfile00 = "tpc00.list",
                             const string &input_tpcfile01 = "tpc01.list",
                             const string &input_tpcfile02 = "tpc02.list",
                             const string &input_tpcfile03 = "tpc03.list",
                             const string &input_tpcfile04 = "tpc04.list",
                             const string &input_tpcfile05 = "tpc05.list",
                             const string &input_tpcfile06 = "tpc06.list",
                             const string &input_tpcfile07 = "tpc07.list",
                             const string &input_tpcfile08 = "tpc08.list",
                             const string &input_tpcfile09 = "tpc09.list",
                             const string &input_tpcfile10 = "tpc10.list",
                             const string &input_tpcfile11 = "tpc11.list",
                             const string &input_tpcfile12 = "tpc12.list",
                             const string &input_tpcfile13 = "tpc13.list",
                             const string &input_tpcfile14 = "tpc14.list",
                             const string &input_tpcfile15 = "tpc15.list",
                             const string &input_tpcfile16 = "tpc16.list",
                             const string &input_tpcfile17 = "tpc17.list",
                             const string &input_tpcfile18 = "tpc18.list",
                             const string &input_tpcfile19 = "tpc19.list",
                             const string &input_tpcfile20 = "tpc20.list",
                             const string &input_tpcfile21 = "tpc21.list",
                             const string &input_tpcfile22 = "tpc22.list",
                             const string &input_tpcfile23 = "tpc23.list",
                             const string &input_tpotfile = "tpot.list"
)
{
  vector<string> tpc_infile;
  tpc_infile.push_back(input_tpcfile00);
  tpc_infile.push_back(input_tpcfile01);
  tpc_infile.push_back(input_tpcfile02);
  tpc_infile.push_back(input_tpcfile03);
  tpc_infile.push_back(input_tpcfile04);
  tpc_infile.push_back(input_tpcfile05);
  tpc_infile.push_back(input_tpcfile06);
  tpc_infile.push_back(input_tpcfile07);
  tpc_infile.push_back(input_tpcfile08);
  tpc_infile.push_back(input_tpcfile09);
  tpc_infile.push_back(input_tpcfile10);
  tpc_infile.push_back(input_tpcfile11);
  tpc_infile.push_back(input_tpcfile12);
  tpc_infile.push_back(input_tpcfile13);
  tpc_infile.push_back(input_tpcfile14);
  tpc_infile.push_back(input_tpcfile15);
  tpc_infile.push_back(input_tpcfile16);
  tpc_infile.push_back(input_tpcfile17);
  tpc_infile.push_back(input_tpcfile18);
  tpc_infile.push_back(input_tpcfile19);
  tpc_infile.push_back(input_tpcfile20);
  tpc_infile.push_back(input_tpcfile21);
  tpc_infile.push_back(input_tpcfile22);
  tpc_infile.push_back(input_tpcfile23);

  vector<string> intt_infile;
  intt_infile.push_back(input_inttfile00);
  intt_infile.push_back(input_inttfile01);
  intt_infile.push_back(input_inttfile02);
  intt_infile.push_back(input_inttfile03);
  intt_infile.push_back(input_inttfile04);
  intt_infile.push_back(input_inttfile05);
  intt_infile.push_back(input_inttfile06);
  intt_infile.push_back(input_inttfile07);

  vector<string> mvtx_infile;
  mvtx_infile.push_back(input_mvtxfile00);
  mvtx_infile.push_back(input_mvtxfile01);
  mvtx_infile.push_back(input_mvtxfile02);
  mvtx_infile.push_back(input_mvtxfile03);
  mvtx_infile.push_back(input_mvtxfile04);
  mvtx_infile.push_back(input_mvtxfile05);

  vector<string> gl1_infile;
  gl1_infile.push_back(input_gl1file);

  vector<string> tpot_infile;
 tpot_infile.push_back(input_tpotfile);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
  // rc->set_IntFlag("RUNNUMBER",20445);
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  //  in->Verbosity(2);
  int i = 0;
  for (auto iter : tpc_infile)
  {
    SingleTpcPoolInput *tpc_sngl = new SingleTpcPoolInput("TPC_" + to_string(i));
    //    tpc_sngl->Verbosity(3);
    tpc_sngl->SetBcoRange(130);
    tpc_sngl->AddListFile(iter);
    in->registerStreamingInput(tpc_sngl, Fun4AllStreamingInputManager::TPC);
    i++;
  }
  i = 0;
  for (auto iter : intt_infile)
  {
    SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + to_string(i));
    //    intt_sngl->Verbosity(3);
    intt_sngl->AddListFile(iter);
    in->registerStreamingInput(intt_sngl, Fun4AllStreamingInputManager::INTT);
    i++;
  }
  i = 0;
  for (auto iter : mvtx_infile)
  {
    SingleMvtxPoolInput *mvtx_sngl = new SingleMvtxPoolInput("MVTX_" + to_string(i));
    //    mvtx_sngl->Verbosity(3);
    mvtx_sngl->SetBcoRange(1000);
    mvtx_sngl->SetNegativeBco(1000);
    mvtx_sngl->AddListFile(iter);
    in->registerStreamingInput(mvtx_sngl, Fun4AllStreamingInputManager::MVTX);
    i++;
  }
  i = 0;
  for (auto iter : gl1_infile)
  {
    SingleGl1PoolInput *sngl = new SingleGl1PoolInput("GL1_" + to_string(i));
    //    sngl->Verbosity(3);
    sngl->AddListFile(iter);
    in->registerStreamingInput(sngl, Fun4AllStreamingInputManager::GL1);
    i++;
  }
  i = 0;
  for (auto iter : tpot_infile)
  {
    SingleMicromegasPoolInput *sngl = new SingleMicromegasPoolInput("MICROMEGAS_" + to_string(i));
    //   sngl->Verbosity(3);
    sngl->SetBcoRange(100);
    sngl->SetNegativeBco(2);
    sngl->AddListFile(iter);
    in->registerStreamingInput(sngl, Fun4AllStreamingInputManager::MICROMEGAS);
    i++;
  }

  se->registerInputManager(in);
  StreamingCheck *scheck = new StreamingCheck();
  scheck->SetTpcBcoRange(130);
  // se->registerSubsystem(scheck);
  // TpcCheck *tpccheck = new TpcCheck();
  // tpccheck->Verbosity(3);
  // tpccheck->SetBcoRange(130);
  // se->registerSubsystem(tpccheck);

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "/sphenix/user/pinkenbu/streaming.root");
  se->registerOutputManager(out);

  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);

  se->End();
  delete se;
  cout << "all done" << endl;
  gSystem->Exit(0);
}
