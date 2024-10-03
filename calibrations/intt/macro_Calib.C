#ifndef INTT_CALIBRATION_MACRO_C
#define INTT_CALIBRATION_MACRO_C

#include <GlobalVariables.C>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllServer.h>
R__LOAD_LIBRARY(libfun4all.so)

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/InputManagerType.h>
#include <fun4allraw/SingleGl1PoolInput.h>
#include <fun4allraw/SingleInttPoolInput.h>
R__LOAD_LIBRARY(libfun4allraw.so)

#include <phool/phool.h> // PHWHERE
#include <phool/recoConsts.h>
R__LOAD_LIBRARY(libphool.so)

#include <ffamodules/FlagHandler.h>
R__LOAD_LIBRARY(libffamodules.so)

#include <inttcalib/InttCalib.h>
R__LOAD_LIBRARY(libinttcalib.so)

#include <intt/InttOdbcQuery.h>
R__LOAD_LIBRARY(libintt.so)

#include <filesystem>
#include <boost/format.hpp>

void macro_Calib(
    int run_num,
    int num_evt
) {
  // TODO: Please prepend file locations with the fullpath to a directory appropriate for transient calibrations data
  // before including this macro in the standard production
  std::string hotmap_cdb_file = (boost::format("hotmap_cdb_%08d.root") % run_num).str();
  std::string bcomap_cdb_file = (boost::format("bcomap_cdb_%08d.root") % run_num).str();
  std::string hotmap_png_file = (boost::format("hotmap_png_%08d.png") % run_num).str();
  std::string bcomap_png_file = (boost::format("bcomap_png_%08d.png") % run_num).str();

  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2023");
  rc->set_uint64Flag("TIMESTAMP", run_num);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

  InttOdbcQuery query;
  query.Verbosity(1);
  if(query.Query(run_num))
  {
    std::cerr << PHWHERE << "\n"
              << "\tDatabase query unsuccessful\n"
              << "\tExiting" << std::endl;
	delete se;
    gSystem->Exit(1);
  }

  // Input Manager
  Fun4AllStreamingInputManager *in = new Fun4AllStreamingInputManager("Comb");

  // Append GL1 input if streaming
  if (query.IsStreaming()) // run 45235 -> GL1p offically established 
  {
    SingleGl1PoolInput *gl1_sngl = new SingleGl1PoolInput("GL1");
    for(auto itr = query.Gl1FileListBegin(); itr != query.Gl1FileListEnd(); ++itr)
    {
      gl1_sngl->AddFile(*itr);
    }
    in->registerStreamingInput(gl1_sngl, InputManagerType::GL1);
  }

  // Append INTT input
  for (int i = 0; i < 8; ++i)
  {
    SingleInttPoolInput *intt_sngl = new SingleInttPoolInput("INTT_" + std::to_string(i));
    intt_sngl->SetNegativeBco(120 - 23);
    intt_sngl->SetBcoRange(120);
    for(auto itr = query.InttFileListBegin(i); itr != query.InttFileListEnd(i); ++itr)
    {
      intt_sngl->AddFile(*itr);
    }
    in->registerStreamingInput(intt_sngl, InputManagerType::INTT);
  }
  se->registerInputManager(in);

  // Subsys reco modules
  InttCalib *inttcalib = new InttCalib();
  inttcalib->Verbosity(0);
  inttcalib->SetBcoMaximumEvent(num_evt);
  inttcalib->SetStreamingMode(query.IsStreaming());
  inttcalib->SetRunNumber(run_num);

  inttcalib->SetHotMapCdbFile(hotmap_cdb_file);
  inttcalib->SetBcoMapCdbFile(bcomap_cdb_file);
  // inttcalib->SetHotMapPngFile(hotmap_png_file); // probably register this to the html page directly, but that's a separate project
  // inttcalib->SetBcoMapPngFile(bcomap_png_file); // probably register this to the html page directly, but that's a separate project

  se->registerSubsystem(inttcalib);
  if(inttcalib->Verbosity())
  {
    std::cout << "Run: " << run_num << " is " << (query.IsStreaming() ? "streaming" : "triggered") << std::endl;
  }

  // Run
  se->run(num_evt);
  se->End();
  delete se;

  // Move the CDB files hotmap_cdb_file and bcomap_cdb_file to the database...
  // ...

  cout << "all done" << endl;
  gSystem->Exit(0);
}

#endif//INTT_CALIBRATION_MACRO_C

