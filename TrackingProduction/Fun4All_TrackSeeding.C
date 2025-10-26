/*
 * This macro shows a working example of running TrackSeeding over the cluster DST
 * This has track residuals as default output but has KFParticle set up with a togglable flag
 * with the default set up for K Short reconstruction
 */

// GlobalVariables.C has to be first, an empty line afterwards
// protects its position against reshuffling by clang-format
#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>


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



R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)
R__LOAD_LIBRARY(libkfparticle_sphenix.so)

void Fun4All_TrackSeeding(
    const int nEvents = 5,
    const std::string &clusterfilename = "DST_TRKR_CLUSTER_run2pp_ana517_2024p024_v001-00053877-02611.root",
    const std::string &dir = "/sphenix/lustre01/sphnxpro/production/run2pp/physics/ana517_2024p024_v001/DST_TRKR_CLUSTER/run_00053800_00053900/dst/",
    const std::string &outfilename = "clusters_seeds",
    const bool convertSeeds = false,
    const bool doKFParticle = false)
{
  //std::string inputseedRawHitFile = dir + seedfilename;
  std::string inputclusterRawHitFile = dir + clusterfilename;

  G4TRACKING::convert_seeds_to_svtxtracks = convertSeeds;
  std::cout << "Converting to seeds : " << G4TRACKING::convert_seeds_to_svtxtracks << std::endl;
  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(clusterfilename);
  int runnumber = runseg.first;
  int segment = runseg.second;

  auto *rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);

  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  TpcReadoutInit(runnumber);
 // these lines show how to override the drift velocity and time offset values set in TpcReadoutInit
  // G4TPC::tpc_drift_velocity_reco = 0.0073844; // cm/ns
  // TpcClusterZCrossingCorrection::_vdrift = G4TPC::tpc_drift_velocity_reco;
  // G4TPC::tpc_tzero_reco = -5*50;  // ns
  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;

  std::string outDir = "myKShortReco/";
  std::string outputFileName = "outputKFParticle_" + std::to_string(runnumber) + "_" + std::to_string(segment) + ".root";
  std::string outputRecoDir = outDir + "inReconstruction/";
  std::string outputRecoFile = outputRecoDir + outputFileName;

  if(doKFParticle){
    std::string makeMainDirectory = "mkdir -p " + outDir;
    system(makeMainDirectory.c_str());
    std::string makeDirectory = "mkdir -p " + outputRecoDir;
    system(makeDirectory.c_str());
  }

  // distortion calibration mode
  /*
   * set to true to enable residuals in the TPC with
   * TPC clusters not participating to the ACTS track fit
   */
  G4TRACKING::SC_CALIBMODE = false;
  TRACKING::pp_mode = true;
  
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  
  std::string theOutfile = outfilename + "_" + std::to_string(runnumber) + "-" + std::to_string(segment) + ".root";
  auto *se = Fun4AllServer::instance();
  se->Verbosity(1);


  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4TPC::REJECT_LASER_EVENTS=true;
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = true;

  // to turn on the default static corrections, enable the two lines below
  G4TPC::ENABLE_STATIC_CORRECTIONS = true;
  G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS = false;

  //to turn on the average corrections, enable the three lines below
  //note: these are designed to be used only if static corrections are also applied
  G4TPC::ENABLE_AVERAGE_CORRECTIONS = true;
  G4TPC::USE_PHI_AS_RAD_AVERAGE_CORRECTIONS = false;
   // to use a custom file instead of the database file:
  G4TPC::average_correction_filename = CDBInterface::instance()->getUrl("TPC_LAMINATION_FIT_CORRECTION");
   
  G4MAGNET::magfield_rescale = 1;
  TrackingInit();

  auto *hitsinclus = new Fun4AllDstInputManager("ClusterInputManager");
  hitsinclus->fileopen(inputclusterRawHitFile);
  se->registerInputManager(hitsinclus);

  // reject laser events if G4TPC::REJECT_LASER_EVENTS is true
  Reject_Laser_Events();

  /*
   * Begin Track Seeding
   */
  
  Tracking_Reco_TrackSeed_run2pp();
  Tracking_Reco_TrackMatching_run2pp();
  
  /*
   * Either converts seeds to tracks with a straight line/helix fit
   * or run the full Acts track kalman filter fit
   */
  if (G4TRACKING::convert_seeds_to_svtxtracks)
  {
    auto *converter = new TrackSeedTrackMapConverter;
    // Default set to full SvtxTrackSeeds. Can be set to
    // SiliconTrackSeedContainer or TpcTrackSeedContainer
    converter->setTrackSeedName("SvtxTrackSeedContainer");
    converter->setFieldMap(G4MAGNET::magfield_tracking);
    converter->Verbosity(0);
    se->registerSubsystem(converter);
  }
  else
  {
    Tracking_Reco_TrackFit_run2pp(theOutfile);
  }

  //vertexing and propagation to vertex
  Tracking_Reco_Vertex_run2pp();

  //run KFParticle
  if(doKFParticle){
     Global_Reco();

  //KFParticle setup

  KFParticle_sPHENIX *kfparticle = new KFParticle_sPHENIX("myKShortReco");
  kfparticle->Verbosity(1);
  kfparticle->setDecayDescriptor("K_S0 -> pi^+ pi^-");

  //Basic node selection and configuration
  kfparticle->magFieldFile("FIELDMAP_TRACKING");
  kfparticle->getAllPVInfo(false);
  kfparticle->allowZeroMassTracks(true);
  kfparticle->useFakePrimaryVertex(false);
  kfparticle->getDetectorInfo(true);

  kfparticle->constrainToPrimaryVertex(true);
  kfparticle->setMotherIPchi2(FLT_MAX);
  kfparticle->setFlightDistancechi2(-1.);
  kfparticle->setMinDIRA(-1.1);
  kfparticle->setDecayLengthRange(0., FLT_MAX);
  kfparticle->setDecayTimeRange(-1*FLT_MAX, FLT_MAX);

  //Track parameters
  kfparticle->setMinMVTXhits(0);
  //kfparticle->setMinINTThits(0);
  kfparticle->setMinTPChits(20);
  kfparticle->setMinimumTrackPT(-1.);
  kfparticle->setMaximumTrackPTchi2(FLT_MAX);
  kfparticle->setMinimumTrackIPchi2(-1.);
  kfparticle->setMinimumTrackIP(-1.);
  //kfparticle->setMaximumTrackchi2nDOF(20.);
  kfparticle->setMaximumTrackchi2nDOF(300.);

  //Vertex parameters
  kfparticle->setMaximumVertexchi2nDOF(50);
  kfparticle->setMaximumDaughterDCA(1.);

  //Parent parameters
  kfparticle->setMotherPT(0);
  kfparticle->setMinimumMass(0.200);
  kfparticle->setMaximumMass(1.000);
  kfparticle->setMaximumMotherVertexVolume(0.1);

  kfparticle->setOutputName(outputRecoFile);

  se->registerSubsystem(kfparticle);
  }

  std::string residstring = theOutfile + "_resid.root";

  auto *resid = new TrackResiduals("TrackResiduals");
  resid->outfileName(residstring);
  resid->clusterTree();
  resid->alignment(false);

  // adjust track map name
  if (G4TRACKING::SC_CALIBMODE && !G4TRACKING::convert_seeds_to_svtxtracks)
  {
    resid->trackmapName("SvtxSiliconMMTrackMap");
    if (G4TRACKING::SC_USE_MICROMEGAS)
    {
      resid->set_doMicromegasOnly(true);
    }
  }

  resid->clusterTree();
  resid->convertSeeds(G4TRACKING::convert_seeds_to_svtxtracks);
  resid->Verbosity(0);
  se->registerSubsystem(resid);

  if (Enable::QA)
  {
    se->registerSubsystem(new SiliconSeedsQA);
    se->registerSubsystem(new TpcSeedsQA);
    se->registerSubsystem(new TpcSiliconQA);
  }
  se->run(nEvents);
  se->End();
  se->PrintTimer();
  CDBInterface::instance()->Print();
  if (Enable::QA)
  {
    std::string qaOutputFileName = theOutfile + "_qa.root";
    QAHistManagerDef::saveQARootFile(qaOutputFileName);
  }

  if(doKFParticle){
    std::ifstream file(outputRecoFile.c_str());
    if (file.good())
    {
      std::string moveOutput = "mv " + outputRecoFile + " " + outDir;
      system(moveOutput.c_str());
    }
  }

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
