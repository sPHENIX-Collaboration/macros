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
                             const string &input_tpotfile = "tpot.list")
{
  vector<string> infile_tmp;
// GL1 which provides the beam clock reference (if we ran with GL1)
  vector<string> gl1_infile;
  ifstream intest(input_gl1file);
  if (intest.is_open()) // does it exist?
  {
    if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
    {
      gl1_infile.push_back(input_gl1file);
    }
    intest.close();
  }
// TPOT
  vector<string> tpot_infile;
  intest.open(input_tpotfile);
  if (intest.is_open()) // does it exist?
  {
    if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
    {
      tpot_infile.push_back(input_tpotfile);
    }
    intest.close();
  }
// INTT
  vector<string> intt_infile;
  infile_tmp.push_back(input_inttfile00);
  infile_tmp.push_back(input_inttfile01);
  infile_tmp.push_back(input_inttfile02);
  infile_tmp.push_back(input_inttfile03);
  infile_tmp.push_back(input_inttfile04);
  infile_tmp.push_back(input_inttfile05);
  infile_tmp.push_back(input_inttfile06);
  infile_tmp.push_back(input_inttfile07);
  for (int i=0; i<infile_tmp.size(); i++)
  {
// C++ std17 filesystem::exists does not work in our CLING version
//    std::cout << infile_tmp[i] << std::endl;
    intest.open(infile_tmp[i]);
    if (intest.is_open()) // does it exist?
    {
      if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
      {
	intt_infile.push_back(infile_tmp[i]);
      }
      intest.close();
    }
  }
  infile_tmp.clear();

// MVTX
  vector<string> mvtx_infile;
  infile_tmp.push_back(input_mvtxfile00);
  infile_tmp.push_back(input_mvtxfile01);
  infile_tmp.push_back(input_mvtxfile02);
  infile_tmp.push_back(input_mvtxfile03);
  infile_tmp.push_back(input_mvtxfile04);
  infile_tmp.push_back(input_mvtxfile05);
  for (int i=0; i<infile_tmp.size(); i++)
  {
// C++ std17 filesystem::exists does not work in our CLING version
//    std::cout << infile_tmp[i] << std::endl;
    intest.open(infile_tmp[i]);
    if (intest.is_open()) // does it exist?
    {
      if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
      {
	mvtx_infile.push_back(infile_tmp[i]);
      }
      intest.close();
    }
  }
  infile_tmp.clear();


  vector<string> tpc_infile;
  infile_tmp.push_back(input_tpcfile00);
  infile_tmp.push_back(input_tpcfile01);
  infile_tmp.push_back(input_tpcfile02);
  infile_tmp.push_back(input_tpcfile03);
  infile_tmp.push_back(input_tpcfile04);
  infile_tmp.push_back(input_tpcfile05);
  infile_tmp.push_back(input_tpcfile06);
  infile_tmp.push_back(input_tpcfile07);
  infile_tmp.push_back(input_tpcfile08);
  infile_tmp.push_back(input_tpcfile09);
  infile_tmp.push_back(input_tpcfile10);
  infile_tmp.push_back(input_tpcfile11);
  infile_tmp.push_back(input_tpcfile12);
  infile_tmp.push_back(input_tpcfile13);
  infile_tmp.push_back(input_tpcfile14);
  infile_tmp.push_back(input_tpcfile15);
  infile_tmp.push_back(input_tpcfile16);
  infile_tmp.push_back(input_tpcfile17);
  infile_tmp.push_back(input_tpcfile18);
  infile_tmp.push_back(input_tpcfile19);
  infile_tmp.push_back(input_tpcfile20);
  infile_tmp.push_back(input_tpcfile21);
  infile_tmp.push_back(input_tpcfile22);
  infile_tmp.push_back(input_tpcfile23);

  for (int i=0; i<infile_tmp.size(); i++)
  {
// C++ std17 filesystem::exists does not work in our CLING version
//    std::cout << infile_tmp[i] << std::endl;
    intest.open(infile_tmp[i]);
    if (intest.is_open()) // does it exist?
    {
      if (intest.peek() != std::ifstream::traits_type::eof()) // is it non zero?
      {
	tpc_infile.push_back(infile_tmp[i]);
      }
      intest.close();
    }
  }
  infile_tmp.clear();

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  recoConsts *rc = recoConsts::instance();
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  //  in->Verbosity(2);
  int i = 0;
  for (auto iter : gl1_infile)
  {
    SingleGl1PoolInput *gl1_sngl = new SingleGl1PoolInput("GL1_" + to_string(i));
    //    gl1_sngl->Verbosity(3);
    gl1_sngl->AddListFile(iter);
    in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
    i++;
  }
  i = 0;
  for (auto iter : intt_infile)
  {
    SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + to_string(i));
    //    intt_sngl->Verbosity(3);
    intt_sngl->SetNegativeBco(1);
    intt_sngl->SetBcoRange(2);
    intt_sngl->AddListFile(iter);
    in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
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
    in->registerStreamingInput(mvtx_sngl, InputManagerType::MVTX);
    i++;
  }
  i = 0;
  for (auto iter : tpc_infile)
  {
    SingleTpcPoolInput *tpc_sngl = new SingleTpcPoolInput("TPC_" + to_string(i));
    //    tpc_sngl->Verbosity(3);
    tpc_sngl->SetBcoRange(130);
    tpc_sngl->AddListFile(iter);
    in->registerStreamingInput(tpc_sngl, InputManagerType::TPC);
    i++;
  }
  i = 0;
  for (auto iter : tpot_infile)
  {
    SingleMicromegasPoolInput *mm_sngl = new SingleMicromegasPoolInput("MICROMEGAS_" + to_string(i));
    //   sngl->Verbosity(3);
    mm_sngl->SetBcoRange(100);
    mm_sngl->SetNegativeBco(2);
    mm_sngl->AddListFile(iter);
    in->registerStreamingInput(mm_sngl, InputManagerType::MICROMEGAS);
    i++;
  }

  se->registerInputManager(in);
  // StreamingCheck *scheck = new StreamingCheck();
  // scheck->SetTpcBcoRange(130);
  // se->registerSubsystem(scheck);
  // TpcCheck *tpccheck = new TpcCheck();
  // tpccheck->Verbosity(3);
  // tpccheck->SetBcoRange(130);
  // se->registerSubsystem(tpccheck);

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "streaming.root");
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
