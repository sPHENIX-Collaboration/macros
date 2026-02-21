#include <sepd_eventplanecalib/QVecCalib.h>

#include <fun4all/Fun4AllBase.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>

// root includes --
#include <TSystem.h>

// c++ includes --
#include <iostream>
#include <string>


R__LOAD_LIBRARY(libsepd_eventplanecalib.so)

void Fun4All_QVecCalib(const std::string& flist,
                       const std::string& input_hist,
                       const std::string& input_Q_calib = "",
                       int pass = 0,
                       int nEvents = 0,
                       const std::string& output = "test.root",
                       const std::string& dst_tag = "new_newcdbtag_v008",
                       const std::string& cdb_output_dir = ".")
{
  std::cout << "########################" << std::endl;
  std::cout << "Run Parameters" << std::endl;
  std::cout << "input list: " << flist << std::endl;
  std::cout << "input hist: " << input_hist << std::endl;
  std::cout << "input Q Vec Calib: " << input_Q_calib << std::endl;
  std::cout << "nEvents: " << nEvents << std::endl;
  std::cout << "Pass: " << pass << std::endl;
  std::cout << "Output: " << output << std::endl;
  std::cout << "dst_tag: " << dst_tag << std::endl;
  std::cout << "CDB Output Dir: " << cdb_output_dir << std::endl;
  std::cout << "########################" << std::endl;

  Fun4AllServer* se = Fun4AllServer::instance();
  se->Verbosity(10);

  // sEPD Tree Gen
  QVecCalib* calib = new QVecCalib();
  calib->set_pass(pass);
  calib->set_input_hist(input_hist);
  calib->set_input_Q_calib(input_Q_calib);
  calib->set_dst_tag(dst_tag);
  calib->set_cdb_output_dir(cdb_output_dir);
  calib->Verbosity(1);
  se->registerSubsystem(calib);

  Fun4AllInputManager* In = new Fun4AllDstInputManager("in");
  In->AddListFile(flist);
  se->registerInputManager(In);

  se->Verbosity(Fun4AllBase::VERBOSITY_QUIET);
  se->run(nEvents);
  se->End();

  se->dumpHistos(output);

  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
