/*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

// leave the GlobalVariables.C at the beginning, an empty line afterwards
// protects its position against reshuffling by clang-format
#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <G4_Mbd.C>
#include <QA.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <eventdisplay/TrackerEventDisplay.h>

#include <cdbobjects/CDBTTree.h>

#include <trackingqa/InttClusterQA.h>
#include <trackingqa/MicromegasClusterQA.h>
#include <trackingqa/MvtxClusterQA.h>
#include <trackingqa/TpcClusterQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>

#include <trackreco/WeightedFitter.h>

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
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrackingqa.so)
R__LOAD_LIBRARY(libEventDisplay.so)
void Fun4All_ZFAllTrackers(
    const int nEvents = 0,
    const std::string &filelist = "filelist.list",
    const std::string &outfilename = "clusters_seeds",
    const bool convertSeeds = true)
{
  G4TRACKING::convert_seeds_to_svtxtracks = convertSeeds;
  std::cout << "Converting to seeds : " << G4TRACKING::convert_seeds_to_svtxtracks << std::endl;

  auto *rc = recoConsts::instance();
  auto *se = Fun4AllServer::instance();

  // input manager for QM production raw hit DST file
  std::ifstream ifs(filelist);
  std::string filepath;

  int i = 0;
  int runnumber = std::numeric_limits<int>::quiet_NaN();
  int segment = std::numeric_limits<int>::quiet_NaN();

  while (std::getline(ifs, filepath))
  {
    std::cout << "Adding DST with filepath: " << filepath << std::endl;
    if (i == 0)
    {
      std::pair<int, int>
          runseg = Fun4AllUtils::GetRunSegment(filepath);
      runnumber = runseg.first;
      segment = runseg.second;
      rc->set_IntFlag("RUNNUMBER", runnumber);
      rc->set_uint64Flag("TIMESTAMP", runnumber);
    }

    std::string inputname = "InputManager" + std::to_string(i);
    auto *hitsin = new Fun4AllDstInputManager(inputname);
    hitsin->fileopen(filepath);
    se->registerInputManager(hitsin);
    i++;
  }

  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", segment);

  G4TRACKING::SC_CALIBMODE = false;
  Enable::MVTX_APPLYMISALIGNMENT = true;
  ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  TRACKING::pp_mode = true;

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

  std::string theOutfile = outfilename + "_" + std::to_string(runnumber) + "-" + std::to_string(segment) + ".root";
  se->Verbosity(1);

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4MAGNET::magfield = "0.01";
  G4MAGNET::magfield_tracking = G4MAGNET::magfield;
  G4MAGNET::magfield_rescale = 1;
  TrackingInit();

  for (int felix = 0; felix < 6; felix++)
  {
    Mvtx_HitUnpacking(std::to_string(felix));
  }
  for (int server = 0; server < 8; server++)
  {
    Intt_HitUnpacking(std::to_string(server));
  }
  std::ostringstream ebdcname;
  for (int ebdc = 0; ebdc < 24; ebdc++)
  {
    for (int endpoint = 0; endpoint < 2; endpoint++)
    {
      ebdcname.str("");
      if (ebdc < 10)
      {
        ebdcname << "0";
      }
      ebdcname << ebdc << "_" << endpoint;
      Tpc_HitUnpacking(ebdcname.str());
    }
  }

  Micromegas_HitUnpacking();

  Mvtx_Clustering();
  Intt_Clustering();
  TPC_Clustering_run2pp();
  Micromegas_Clustering();

  // this now does Si and TPC seeding only
  Tracking_Reco_TrackSeed_ZeroField();

  Tracking_Reco_SiTpcTrackMatching_run2pp();

  auto *mm_match = new PHMicromegasTpcTrackMatching;
  mm_match->Verbosity(0);
  mm_match->set_pp_mode(TRACKING::pp_mode);

  mm_match->set_rphi_search_window_lyr1(3.);
  mm_match->set_rphi_search_window_lyr2(15.0);
  mm_match->set_z_search_window_lyr1(30.0);
  mm_match->set_z_search_window_lyr2(3.);

  mm_match->set_min_tpc_layer(38);            // layer in TPC to start projection fit
  mm_match->set_test_windows_printout(true);  // used for tuning search windows only
  mm_match->zeroField(true);
  se->registerSubsystem(mm_match);

  if (G4TRACKING::convert_seeds_to_svtxtracks)
  {
    auto *converter = new TrackSeedTrackMapConverter;
    // Default set to full SvtxTrackSeeds. Can be set to
    // SiliconTrackSeedContainer or TpcTrackSeedContainer
    converter->setTrackSeedName("SvtxTrackSeedContainer");
    converter->setFieldMap(G4MAGNET::magfield_tracking);
    converter->Verbosity(0);
    converter->constField();
    se->registerSubsystem(converter);
  }
  else
  {
    auto *weightedFitter = new WeightedFitter;
    weightedFitter->set_track_map_node_name("SvtxTrackMap");

    if (!G4TRACKING::SC_USE_MICROMEGAS)
    {
      // exclude TPOT from fit
      weightedFitter->exclude_layer_in_fit(55);
      weightedFitter->exclude_layer_in_fit(56);
    }

    se->registerSubsystem(weightedFitter);
  }

  PHSimpleVertexFinder *finder = new PHSimpleVertexFinder;
  finder->zeroField(true);
  finder->Verbosity(0);
  finder->setDcaCut(0.5);
  finder->setTrackPtCut(-99999.);
  finder->setBeamLineCut(1);
  finder->setTrackQualityCut(1000000000);
  finder->setNmvtxRequired(3);
  finder->setOutlierPairCut(0.1);
  se->registerSubsystem(finder);

  std::string residstring = theOutfile + "_resid.root";

  auto *resid = new TrackResiduals("TrackResiduals");
  resid->outfileName(residstring);
  resid->alignment(false);
  resid->clusterTree();
  // resid->hitTree();
  resid->zeroField();
  resid->convertSeeds(G4TRACKING::convert_seeds_to_svtxtracks);
  resid->Verbosity(0);
  se->registerSubsystem(resid);

  // Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", "/sphenix/tg/tg01/hf/jdosbo/tracking_development/Run24/Beam/41626/hitsets.root");
  // se->registerOutputManager(out);
  if (Enable::QA)
  {
    se->registerSubsystem(new MvtxClusterQA);
    se->registerSubsystem(new InttClusterQA);
    se->registerSubsystem(new TpcClusterQA);
    se->registerSubsystem(new MicromegasClusterQA);
  }
  se->run(nEvents);
  se->End();
  se->PrintTimer();

  if (Enable::QA)
  {
    std::string qaOutputFileName = theOutfile + "_qa.root";
    QAHistManagerDef::saveQARootFile(qaOutputFileName);
  }

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
