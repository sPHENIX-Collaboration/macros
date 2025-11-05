/*
 * Track + Calo matching for TPC Drift Velocity Calibration
 * using the production track cluster and track seed DSTs
 */

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Magnet.C>
#include <Trkr_Clustering.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>
#include <Trkr_Reco.C>
#include <G4_Global.C>

#include <caloreco/RawClusterBuilderTopo.h>


#include <tpcdvcalib/TrackToCalo.h>

#include <cdbobjects/CDBTTree.h>

#include <tpccalib/PHTpcResiduals.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <trackreco/AzimuthalSeeder.h>
#include <trackreco/PHActsTrackProjection.h>
#include <trackbase_historic/SvtxTrack.h>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

#include <iostream>
#include <filesystem>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libg4dst.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libTpcDVCalib.so)
R__LOAD_LIBRARY(libcalo_reco.so)

std::string GetFirstLine(const std::string& listname);
bool is_directory_empty(const std::filesystem::path& dir_path);

void Fun4All_TrackAnalysis(
    const int nEvents = 10,
    std::vector<std::string> myInputLists = {
        "run46730_0000_trkr_seed.txt",
        "run46730_0000_trkr_cluster.txt",
        "run46730_calo.list"}, 
    const std::string& outDir = "./",
    bool doTpcOnlyTracking = true,
    float initial_driftvelocity = 0.00710,
    bool doEMcalRadiusCorr = true,
    const bool convertSeeds = false)
{
  int verbosity = 0;

  G4TRACKING::convert_seeds_to_svtxtracks = convertSeeds;
  std::cout << "Converting to seeds : " << G4TRACKING::convert_seeds_to_svtxtracks << std::endl;
  std::string firstfile = GetFirstLine(myInputLists[0]);
  if (*firstfile.c_str() == '\0') return;
  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(firstfile);
  int runnumber = runseg.first;
  int segment = runseg.second;

  TpcReadoutInit(runnumber);
  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;

  // distortion calibration mode
  /*
   * set to true to enable residuals in the TPC with
   * TPC clusters not participating to the ACTS track fit
   */
  G4TRACKING::SC_CALIBMODE = false;

  ACTSGEOM::mvtxMisalignment = 100;
  ACTSGEOM::inttMisalignment = 100.;
  ACTSGEOM::tpotMisalignment = 100.;

  auto *se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto *rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4TPC::tpc_drift_velocity_reco = initial_driftvelocity; //cm/ns
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;
  // to turn on the default static corrections, enable the two lines below
  G4TPC::ENABLE_STATIC_CORRECTIONS = true;
  // G4TPC::DISTORTIONS_USE_PHI_AS_RADIANS = false;

  G4MAGNET::magfield_rescale = 1;
  TrackingInit();

  //Add all required input files
  for (unsigned int i = 0; i < myInputLists.size(); ++i)
  {
    Fun4AllInputManager *infile = new Fun4AllDstInputManager("DSTin_" + std::to_string(i));
    std::cout << "Including file " << myInputLists[i] << std::endl;
    infile->AddListFile(myInputLists[i]);
    se->registerInputManager(infile);
  }

  /*
   * Either converts seeds to tracks with a straight line/helix fit
   * or run the full Acts track kalman filter fit
   */
  if (G4TRACKING::convert_seeds_to_svtxtracks)
  {
    auto *converter = new TrackSeedTrackMapConverter;
    // Option to use TpcTrackSeedContainer or SvtxTrackSeeds
    // can be set to SiliconTrackSeedContainer for silicon-only track fit
    if (doTpcOnlyTracking)
    {
      converter->setTrackSeedName("TpcTrackSeedContainer");
    }
    else
    {
      converter->setTrackSeedName("SvtxTrackSeeds");
    }
    converter->setFieldMap(G4MAGNET::magfield_tracking);
    converter->Verbosity(0);
    se->registerSubsystem(converter);
  }
  else
  {
    auto *deltazcorr = new PHTpcDeltaZCorrection;
    deltazcorr->Verbosity(0);
    se->registerSubsystem(deltazcorr);

    // perform final track fit with ACTS
    auto *actsFit = new PHActsTrkFitter;
    actsFit->Verbosity(0);
    actsFit->commissioning(G4TRACKING::use_alignment);
    // in calibration mode, fit only Silicons and Micromegas hits
    if (!doTpcOnlyTracking)
    {
      actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
      actsFit->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
    }
    actsFit->set_pp_mode(TRACKING::pp_mode);
    actsFit->set_use_clustermover(true);  // default is true for now
    actsFit->useActsEvaluator(false);
    actsFit->useOutlierFinder(false);
    actsFit->setFieldMap(G4MAGNET::magfield_tracking);
    se->registerSubsystem(actsFit);
  }

  PHSimpleVertexFinder *finder = new PHSimpleVertexFinder;
  finder->Verbosity(verbosity);
  finder->setDcaCut(0.5);
  finder->setTrackPtCut(-99999.);
  finder->setBeamLineCut(1);
  finder->setTrackQualityCut(1000000000);
  if (!doTpcOnlyTracking)
  {
    finder->setRequireMVTX(true);
    finder->setNmvtxRequired(3);
  }
  else
  {
    finder->setRequireMVTX(false);
  }
  finder->setOutlierPairCut(0.1);
  se->registerSubsystem(finder);

  Global_Reco();

  auto *projection = new PHActsTrackProjection("CaloProjection");
  float new_cemc_rad = 100.70;//(1-(-0.077))*93.5 recommended cemc radius
  if (doEMcalRadiusCorr)
  {
    projection->setLayerRadius(SvtxTrack::CEMC, new_cemc_rad);
  }
  se->registerSubsystem(projection);

  RawClusterBuilderTopo* ClusterBuilder1 = new RawClusterBuilderTopo("EMcalRawClusterBuilderTopo1");
  ClusterBuilder1->Verbosity(verbosity);
  ClusterBuilder1->set_nodename("TOPOCLUSTER_EMCAL");
  ClusterBuilder1->set_enable_HCal(false);
  ClusterBuilder1->set_enable_EMCal(true);
  //ClusterBuilder1->set_noise(0.0025, 0.006, 0.03);
  ClusterBuilder1->set_noise(0.01, 0.03, 0.03);
  ClusterBuilder1->set_significance(4.0, 2.0, 1.0);
  ClusterBuilder1->allow_corner_neighbor(true);
  ClusterBuilder1->set_do_split(true);
  ClusterBuilder1->set_minE_local_max(1.0, 2.0, 0.5);
  ClusterBuilder1->set_R_shower(0.025);
  se->registerSubsystem(ClusterBuilder1);

  std::string outputAnaFileName = "TrackCalo_" + std::to_string(segment) + "_ana.root";
  std::string outputRecoDir = outDir + "inReconstruction/" + std::to_string(runnumber) + "/";
  std::string makeDirectory = "mkdir -p " + outputRecoDir;
  system(makeDirectory.c_str());
  std::string outputAnaFile = outputRecoDir + outputAnaFileName;
  std::cout << "Reco ANA file: " << outputAnaFile << std::endl;

  TrackToCalo *ttc = new TrackToCalo("Tracks_And_Calo", outputAnaFile);
  ttc->EMcalRadiusUser(true);
  ttc->setEMcalRadius(new_cemc_rad);
  se->registerSubsystem(ttc);

  std::ifstream file_ana(outputAnaFile.c_str(), std::ios::binary | std::ios::ate);
  if (file_ana.good() && (file_ana.tellg() > 100))
  {
    std::string outputRecoDirMove = outDir + "Reconstructed/" + std::to_string(runnumber) + "/";
    std::string makeDirectoryMove = "mkdir -p " + outputRecoDirMove;
    system(makeDirectoryMove.c_str());
    std::string moveOutput = "mv " + outputAnaFile + " " + outDir + "Reconstructed/" + std::to_string(runnumber);
    std::cout << "moveOutput: " << moveOutput << std::endl;
    system(moveOutput.c_str());
  }

  se->run(nEvents);
  se->End();
  se->PrintTimer();

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}

std::string GetFirstLine(const std::string& listname)
{
  std::ifstream file(listname);

  std::string firstLine;
  if (file.is_open()) {
      if (std::getline(file, firstLine)) {
          std::cout << "First Line: " << firstLine << std::endl;
      } else {
          std::cerr << "Unable to read first line of file" << std::endl;
      }
      file.close();
  } else {
      std::cerr << "Unable to open file" << std::endl;
  }
  return firstLine;
}

bool is_directory_empty(const std::filesystem::path& dir_path) {
    if (std::filesystem::exists(dir_path) && std::filesystem::is_directory(dir_path)) {
        return std::filesystem::directory_iterator(dir_path) == std::filesystem::directory_iterator();
    }
    return false;
}
