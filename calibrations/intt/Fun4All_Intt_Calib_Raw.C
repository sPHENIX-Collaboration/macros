#include <GlobalVariables.C>

#include <inttcalib/InttCalib.h>

#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>

#include <phool/recoConsts.h>

#include <Rtypes.h>  // resolves R__LOAD_LIBRARY for clang-tidy
#include <TSystem.h>

#include <fstream>

R__LOAD_LIBRARY(libinttcalib.so)
R__LOAD_LIBRARY(libintt.so)

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

bool isGood(const std::string &infile);

void Fun4All_Intt_Calib_Raw(
    int num_evt = 400000,
    int run_num = 0,
    const std::string &hotmap_cdb_file = "hotmap.root",
    const std::string &hotmap_png_file = "hotmap.png",
    const std::string &bcomap_cdb_file = "bcomap.root",
    const std::string &bcomap_png_file = "bcomap.png",
    const std::string &input_gl1file = "gl1daq.list",
    const std::string &input_inttfile00 = "intt0.list",
    const std::string &input_inttfile01 = "intt1.list",
    const std::string &input_inttfile02 = "intt2.list",
    const std::string &input_inttfile03 = "intt3.list",
    const std::string &input_inttfile04 = "intt4.list",
    const std::string &input_inttfile05 = "intt5.list",
    const std::string &input_inttfile06 = "intt6.list",
    const std::string &input_inttfile07 = "intt7.list",

    bool isStreaming = true)
{
  // GL1 which provides the beam clock reference (if we ran with GL1)
  std::vector<std::string> gl1_infile;
  gl1_infile.push_back(input_gl1file);

  // INTT
  std::vector<std::string> intt_infile;
  intt_infile.push_back(input_inttfile00);
  intt_infile.push_back(input_inttfile01);
  intt_infile.push_back(input_inttfile02);
  intt_infile.push_back(input_inttfile03);
  intt_infile.push_back(input_inttfile04);
  intt_infile.push_back(input_inttfile05);
  intt_infile.push_back(input_inttfile06);
  intt_infile.push_back(input_inttfile07);

  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(1);
  // Input Manager
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", run_num);

  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  int i = 0;
  for (const auto& iter : gl1_infile)
  {
    if (isGood(iter))
    {
      SingleGl1PoolInput *gl1_sngl = new SingleGl1PoolInput("GL1_" + std::to_string(i));
      //    gl1_sngl->Verbosity(3);
      gl1_sngl->AddListFile(iter);
      in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
      i++;
    }
  }
  i = 0;
  for (const auto& iter : intt_infile)
  {
    if (isGood(iter))
    {
      std::cout << "opening file " << iter << std::endl;
      SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + std::to_string(i));
      //    intt_sngl->Verbosity(3);
      intt_sngl->SetNegativeBco(120 - 23);
      intt_sngl->SetBcoRange(500);
      intt_sngl->AddListFile(iter);
      in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
      i++;
    }
  }
  se->registerInputManager(in);

  InttCalib *inttcalib = new InttCalib();
  inttcalib->Verbosity(0);

  // Construct file paths
  inttcalib->SetHotMapCdbFile(hotmap_cdb_file);
  inttcalib->SetHotMapPngFile(hotmap_png_file);
  inttcalib->SetBcoMapCdbFile(bcomap_cdb_file);
  inttcalib->SetBcoMapPngFile(bcomap_png_file);
  inttcalib->SetBcoMaximumEvent(num_evt);  // default = 50k
  inttcalib->SetStreamingMode(isStreaming);
  inttcalib->SetRunNumber(run_num);

  inttcalib->SetHotMapCdbFile(hotmap_cdb_file);
  // inttcalib->SetHotMapPngFile(hotmap_save_file);
  inttcalib->SetBcoMapCdbFile(bcomap_cdb_file);
  // inttcalib->SetBcoMapPngFile(bcomap_save_file);

  se->registerSubsystem(inttcalib);
  se->run(num_evt);

  se->End();
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
