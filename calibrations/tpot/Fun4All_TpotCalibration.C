#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>

#include <fun4allraw/Fun4AllStreamingInputManager.h>
#include <fun4allraw/SingleMicromegasPoolInput.h>

#include <phool/recoConsts.h>

#include <micromegas/MicromegasCombinedDataCalibration.h>
#include <micromegas/MicromegasClusterizer.h>

#include <GlobalVariables.C>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)

//____________________________________________________________________
int Fun4All_TpotCalibration(
  const int nEvents = 1000,
  const char* inputFile = "/sphenix/lustre01/sphnxpro/physics/TPOT/calib/TPOT_ebdc39_calib-00074871-0000.evt",
  const char* calibrationFile = "TPOT_Pedestal-00074871-0000.root",
  const char* evaluationFile = "MicromegasCombinedDataCalibration-00074871-0000.root"
  )
{
  // print inputs
  std::cout << "Fun4All_TpotCalibration - nEvents: " << nEvents << std::endl;
  std::cout << "Fun4All_TpotCalibration - inputFile: " << inputFile << std::endl;
  std::cout << "Fun4All_TpotCalibration - calibrationFile: " << calibrationFile << std::endl;
  std::cout << "Fun4All_TpotCalibration - evaluationFile: " << evaluationFile << std::endl;

  // server
  auto se = Fun4AllServer::instance();
  se->Verbosity(1);

  // reco const
  auto rc = recoConsts::instance();

  // condition database
  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG",CDB::global_tag);
  rc->set_uint64Flag("TIMESTAMP",CDB::timestamp);

  //input
  auto in = new Fun4AllStreamingInputManager;
  se->registerInputManager(in);
  if( true )
  {
    // Micromegas single input
    auto single = new SingleMicromegasPoolInput;
    single->AddFile(inputFile);
    in->registerStreamingInput(single, InputManagerType::MICROMEGAS);
  }

  {
    // micromegas raw data calibration
    auto micromegasCombinedDataCalibration = new MicromegasCombinedDataCalibration;
    micromegasCombinedDataCalibration->set_sample_min(0);
    micromegasCombinedDataCalibration->set_sample_max(20);

    // calibration output
    micromegasCombinedDataCalibration->set_calibration_file(calibrationFile);

    // evaluation outpue
    micromegasCombinedDataCalibration->set_do_evaluation(true);
    micromegasCombinedDataCalibration->set_evaluation_file(evaluationFile);

    se->registerSubsystem( micromegasCombinedDataCalibration );
  }

  // process events
  se->run(nEvents);

  // terminate
  se->End();
  se->PrintTimer();

  std::cout << "All done" << std::endl;
  delete se;
  gSystem->Exit(0);
  return 0;
}

// This function is only used to test if we can load this as root6 macro
// without running into unresolved libraries and include files
void RunFFALoadTest() {}
