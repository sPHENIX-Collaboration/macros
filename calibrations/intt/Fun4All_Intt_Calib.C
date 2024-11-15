#include <GlobalVariables.C>

#include <intt/InttOdbcQuery.h>  // To get data taking mode

#include <inttcalib/InttCalib.h>

#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libinttcalib.so)
R__LOAD_LIBRARY(libintt.so)

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libffarawmodules.so)

void Fun4All_Intt_Calib(
    int num_evt = 400000,
    int run_num,
    const string &input_dst_list = "intt.list",
    const std::string &hotmap_cdb_file = "hotmap.root",
    const std::string &hotmap_png_file = "hotmap.png",
    const std::string &bcomap_cdb_file = "bcomap.root",
    const std::string &bcomap_png_file = "bcomap.png",
    bool isStreaming = true
  )
{

  Fun4AllServer *se = Fun4AllServer::instance();
  //  se->Verbosity(1);
  // Input Manager
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", run_num);

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

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  in->AddListFile(input_dst_list);
  se->registerInputManager(in);

  se->run(num_evt);

  se->End();
  delete se;
  cout << "all done" << endl;
  gSystem->Exit(0);
}
