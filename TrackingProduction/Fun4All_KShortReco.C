/*
 * This macro shows a working example of running KFParticle over a Track DST
 * This has certain KFParticle features like Particle ID and trigger Info off
 * Because those features require other DSTs like the cluster or hit dst
 */

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Magnet.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <globalvertex/GlobalVertexReco.h>

#include <cdbobjects/CDBTTree.h>

#include <tpccalib/PHTpcResiduals.h>

#include <trackingqa/SiliconSeedsQA.h>
#include <trackingqa/TpcSeedsQA.h>
#include <trackingqa/TpcSiliconQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <kfparticle_sphenix/KFParticle_sPHENIX.h>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libkfparticle_sphenix.so)
R__LOAD_LIBRARY(libcalotrigger.so)

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)

void Fun4All_KShortReco(
    const int nEvents = 10000,
    const std::string &inputDST = "DST_TRKR_TRACKS_run2pp_ana475_2024p018_v001-00053877-00000.root",
    const std::string &inputDir = "/sphenix/lustre01/sphnxpro/production/run2pp/physics/ana475_2024p018_v001/DST_TRKR_TRACKS/run_00053800_00053900/dst/",
    const int nSkip = 0)
{
  std::string inputSeedFile = inputDir + inputDST;

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(inputDST);
  int runnumber = runseg.first;
  std::stringstream nice_runnumber;
  nice_runnumber << std::setw(8) << std::setfill('0') << std::to_string(runnumber);

  int rounded_up = 100 * (std::ceil((float) runnumber / 100));
  std::stringstream nice_rounded_up;
  nice_rounded_up << std::setw(8) << std::setfill('0') << std::to_string(rounded_up);

  int rounded_down = 100 * (std::floor((float) runnumber / 100));
  std::stringstream nice_rounded_down;
  nice_rounded_down << std::setw(8) << std::setfill('0') << std::to_string(rounded_down);

  int segment = runseg.second;
  std::stringstream nice_segment;
  nice_segment << std::setw(5) << std::setfill('0') << std::to_string(segment);

  std::stringstream nice_skip;
  nice_skip << std::setw(5) << std::setfill('0') << std::to_string(nSkip);

  auto *rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  TpcReadoutInit(runnumber);
  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;

  std::string output_dir = "./";  // Top dir of where the output nTuples will be written
  std::string kfp_header = "outputKFParticle_";
  std::string processing_folder = "inReconstruction/";
  std::string trailer = "_" + nice_runnumber.str() + "_" + nice_segment.str() + "_" + nice_skip.str() + ".root";
  std::string pipi_reconstruction_name = "pipi_reco";  // Used for naming output folder, file and node

  std::string output_file_name = kfp_header + pipi_reconstruction_name + trailer;
  std::string pipi_output_dir = output_dir + pipi_reconstruction_name + "/";
  std::string output_reco_dir = pipi_output_dir + processing_folder;
  std::string pipi_output_reco_file = output_reco_dir + output_file_name;

  std::string makeDirectory = "mkdir -p " + output_reco_dir;
  system(makeDirectory.c_str());

  // distortion calibration mode
  /*
   * set to true to enable residuals in the TPC with
   * TPC clusters not participating to the ACTS track fit
   */

  G4TRACKING::SC_CALIBMODE = false;
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  TRACKING::pp_mode = true;

  auto *se = Fun4AllServer::instance();
  se->Verbosity(1);

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  TrackingInit();

  auto *tracks = new Fun4AllDstInputManager("TrackInputManager");
  tracks->fileopen(inputSeedFile);
  se->registerInputManager(tracks);

  GlobalVertexReco *gblvertex = new GlobalVertexReco();
  gblvertex->Verbosity(0);
  se->registerSubsystem(gblvertex);

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("pipi_reco");
  kfparticle->Verbosity(0);

  kfparticle->setDecayDescriptor("K_S0 -> pi^+ pi^-");  // See twiki on how to set this

  kfparticle->usePID(false);
  // kfparticle->setPIDacceptFraction(pid_frac);
  kfparticle->dontUseGlobalVertex(true);
  kfparticle->requireTrackVertexBunchCrossingMatch(true);
  kfparticle->getAllPVInfo(true);
  kfparticle->allowZeroMassTracks();
  kfparticle->use2Dmatching(false);
  kfparticle->getTriggerInfo(false);
  kfparticle->getDetectorInfo(false);
  kfparticle->saveDST(false);
  kfparticle->setContainerName("pipi_reco");
  kfparticle->magFieldFile("FIELDMAP_TRACKING");

  // PV to SV cuts
  kfparticle->constrainToPrimaryVertex(true);
  kfparticle->setMotherIPchi2(100);
  kfparticle->setFlightDistancechi2(-1.);
  kfparticle->setMinDIRA(0.999);
  kfparticle->setDecayLengthRange(0.1, FLT_MAX);

  // Track parameters
  kfparticle->setMinimumTrackPT(0.0);
  kfparticle->setMinimumTrackIPchi2(-1.);
  kfparticle->setMinimumTrackIP(-1.);
  kfparticle->setMaximumTrackchi2nDOF(100.);
  kfparticle->setMinTPChits(25);

  // Vertex parameters
  kfparticle->setMaximumVertexchi2nDOF(20);
  kfparticle->setMaximumDaughterDCA(0.5);  // 5 mm

  // Set Tracker State Requirements
  kfparticle->setMinMVTXhits(0);
  kfparticle->setMinINTThits(0);
  kfparticle->setMinTPChits(20);

  // Parent parameters
  kfparticle->setMotherPT(0);
  kfparticle->setMinimumMass(0.40);  // Check mass ranges
  kfparticle->setMaximumMass(0.60);
  kfparticle->setMaximumMotherVertexVolume(0.1);

  kfparticle->setOutputName(pipi_output_reco_file);

  se->registerSubsystem(kfparticle);

  se->skip(nSkip);
  se->run(nEvents);
  se->End();
  se->PrintTimer();

  std::ifstream file(pipi_output_reco_file);
  if (file.good())
  {
    std::string moveOutput = "mv " + pipi_output_reco_file + " " + pipi_output_dir;
    system(moveOutput.c_str());
  }

  delete se;

  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
