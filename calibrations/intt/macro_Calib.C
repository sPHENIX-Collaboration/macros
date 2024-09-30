#include <GlobalVariables.C>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>
#include <phool/recoConsts.h>
#include <ffamodules/FlagHandler.h>

#include <inttcalib/InttCalib.h>
#include <intt/InttOdbcQuery.h> // To get data taking mode


R__LOAD_LIBRARY(libinttcalib.so)
R__LOAD_LIBRARY(libintt.so)

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

#include <filesystem>
namespace fs = std::filesystem;

void macro_Calib(
    int run_num,
    int num_evt,
    std::string const &gl1_format,
    std::string const &intt_format,
    std::string const &hotmap_cdb_file,
    std::string const &hotmap_png_file,
    std::string const &bcomap_cdb_file,
    std::string const &bcomap_png_file, 
    bool isStreaming = true)
{
  char buff_gl1[256];
  char buff[256];
  // GL1 which provides the beam clock reference (if we ran with GL1)
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  // Input Manager
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  rc->set_uint64Flag("TIMESTAMP", run_num);

  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");
  std::vector<std::string> missing_list_files = {};
  InttOdbcQuery query;
  query.Query(run_num);
  if (query.IsStreaming()) // run 45235 -> GL1p offically established 
  {
    SingleGl1PoolInput *gl1_sngl = new SingleGl1PoolInput("GL1");
    snprintf(buff_gl1, sizeof(buff_gl1), gl1_format.c_str(), run_num);
    std::cout << buff_gl1 << std::endl;
    gl1_sngl->AddListFile(buff_gl1);
    in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
  } // If run is not streaming, doing combining without GL1 is also fine for INTT standalone calibration
  for (int i = 0; i < 8; ++i)
  {
    snprintf(buff, sizeof(buff), intt_format.c_str(), run_num, i);
    if (!std::filesystem::exists(buff))
    {
      missing_list_files.push_back(buff);
      continue;
    }

    SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + std::to_string(i));
    intt_sngl->SetNegativeBco(120 - 23);
    intt_sngl->SetBcoRange(120);
    intt_sngl->AddListFile(buff);
    in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
  }
  se->registerInputManager(in);

  InttCalib *inttcalib = new InttCalib();
  inttcalib->Verbosity(0);
  fs::path currentPath = fs::current_path();
  std::string RunNumberStr = std::to_string(run_num);
  std::string EventNumberStr = std::to_string(num_evt);

  // Construct file paths
  inttcalib->SetHotMapCdbFile(hotmap_cdb_file);
  inttcalib->SetHotMapPngFile(hotmap_png_file);
  inttcalib->SetBcoMapCdbFile(bcomap_cdb_file);
  inttcalib->SetBcoMapPngFile(bcomap_png_file);
  inttcalib->SetBcoMaximumEvent(num_evt); // default = 50k
  inttcalib->SetStreamingMode(query.IsStreaming());
  inttcalib->SetRunNumber(run_num);
  std::cout << "Run: " << run_num << " is \n"
            << "\t" << (query.IsStreaming() ? "streaming" : "triggered") << std::endl;
  // std::string hotmap_cdb_file = (currentPath / "hotmap_cdb" / ("hotmap_cdb_" + RunNumberStr + "_" + EventNumberStr + ".root")).string();
  // std::string hotmap_save_file = (currentPath / "hotmap_png" / ("hotmap_png_" + RunNumberStr + "_" + EventNumberStr + ".png")).string(); // .root .png .pdf
  // std::string bcomap_cdb_file = (currentPath / "bcomap_cdb" / ("bcomap_cdb_" + RunNumberStr + "_" + EventNumberStr + ".root")).string();
  // std::string bcomap_save_file = (currentPath / "bcomap_png" / ("bcomap_png_" + RunNumberStr + "_" + EventNumberStr + ".png")).string(); //.root .png .pdf

  // inttcalib->SetHotMapCdbFile(hotmap_cdb_file);
  // inttcalib->SetHotMapPngFile(hotmap_save_file);
  // inttcalib->SetBcoMapCdbFile(bcomap_cdb_file);
  // inttcalib->SetBcoMapPngFile(bcomap_save_file);

  se->registerSubsystem(inttcalib);

  se->run(num_evt);

  se->End();
  delete se;
  cout << "all done" << endl;
  gSystem->Exit(0);
}