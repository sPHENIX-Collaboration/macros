 /*
 * This macro shows a minimum working example of running the tracking
 * hit unpackers with some basic seeding algorithms to try to put together
 * tracks. There are some analysis modules run at the end which package
 * hits, clusters, and clusters on tracks into trees for analysis.
 */

#include <fun4all/Fun4AllUtils.h>
#include <G4_ActsGeom.C>
#include <G4_Global.C>
#include <G4_Magnet.C>
#include <G4_Mbd.C>
#include <GlobalVariables.C>
#include <QA.C>
#include <Trkr_RecoInit.C>
#include <Trkr_Clustering.C>
#include <Trkr_Reco.C>
#include <Trkr_TpcReadoutInit.C>

#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>

#include <eventdisplay/TrackerEventDisplay.h>
#include <phool/recoConsts.h>
#include <trackingqa/InttClusterQA.h>

#include <cdbobjects/CDBTTree.h>

#include <trackingqa/MicromegasClusterQA.h>

#include <trackingqa/MvtxClusterQA.h>

#include <trackingdiagnostics/TrackResiduals.h>
#include <trackingdiagnostics/TrkrNtuplizer.h>
#include <trackingqa/TpcClusterQA.h>
#include <trackreco/AzimuthalSeeder.h>

#include <stdio.h>

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
    const std::string tpcfilename = "DST_STREAMING_EVENT_run2pp_new_2024p002-00052077-00015.root",
    const std::string tpcdir = "/sphenix/lustre01/sphnxpro/physics/slurp/streaming/physics/new_2024p002/run_00052000_00052100/",
    const std::string outfilename = "clusters_seeds",
    const bool convertSeeds = true)
{
  std::string inputtpcRawHitFile = tpcdir + tpcfilename;

  G4TRACKING::convert_seeds_to_svtxtracks = convertSeeds;
  std::cout << "Converting to seeds : " << G4TRACKING::convert_seeds_to_svtxtracks << std::endl;
  std::pair<int, int>
      runseg = Fun4AllUtils::GetRunSegment(tpcfilename);
  int runnumber = runseg.first;
  int segment = runseg.second;

  auto rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", runnumber);
  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "2024p008");
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  TpcReadoutInit( runnumber );
  std::cout<< " run: " << runnumber
	   << " samples: " << TRACKING::reco_tpc_maxtime_sample
	   << " pre: " << TRACKING::reco_tpc_time_presample
	   << " vdrift: " << G4TPC::tpc_drift_velocity_reco
	   << std::endl;

  ACTSGEOM::mvtxMisalignment = 100;
  ACTSGEOM::inttMisalignment = 1000.;
  ACTSGEOM::tpotMisalignment = 100.;
  TString outfile = outfilename + "_" + runnumber + "-" + segment + ".root";
  std::string theOutfile = outfile.Data();
  auto se = Fun4AllServer::instance();
  se->Verbosity(1);

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  G4MAGNET::magfield = "0.01";
  G4MAGNET::magfield_tracking = G4MAGNET::magfield;
  G4MAGNET::magfield_rescale = 1;
  TrackingInit();

  auto hitsin = new Fun4AllDstInputManager("InputManager");
  hitsin->fileopen(inputtpcRawHitFile);
  // hitsin->AddFile(inputMbd);
  se->registerInputManager(hitsin);

  TRACKING::tpc_zero_supp = true;
  Mvtx_HitUnpacking();
  Intt_HitUnpacking();
  Tpc_HitUnpacking();
  Micromegas_HitUnpacking();

  Mvtx_Clustering();
  Intt_Clustering();

  auto tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  tpcclusterizer->set_rawdata_reco();
  se->registerSubsystem(tpcclusterizer);

  Micromegas_Clustering();

  // this now does Si and TPC seeding only
  Tracking_Reco_TrackSeed_ZeroField();

  auto silicon_match = new PHSiliconTpcTrackMatching;
  silicon_match->Verbosity(0);
  silicon_match->set_use_legacy_windowing(false);
  // set search windows matching Silicon to TPC seeds
  // Selected for tracks with ntpc>34,|z_Si-z_TPC|<30,crossing==0
  // see https://indico.bnl.gov/event/26202/attachments/59703/102575/2025_01_31_ZeroField.pdf
  // Will probably be narrowed from improved alignment soon.
  silicon_match->window_dx.set_QoverpT_maxabs({2.6,0,0});
  silicon_match->window_dy.set_QoverpT_maxabs({2.3,0,0});
  silicon_match->window_dz.set_QoverpT_range({-2.9,0,0},{4.2,0,0});
  silicon_match->window_deta.set_QoverpT_maxabs({0.06,0,0});
  silicon_match->window_dphi.set_QoverpT_maxabs({0.11,0,0});
  silicon_match->set_test_windows_printout(false);
  silicon_match->set_pp_mode(TRACKING::pp_mode);
  silicon_match->zeroField(true);
  se->registerSubsystem(silicon_match);

  auto mm_match = new PHMicromegasTpcTrackMatching;
  mm_match->Verbosity(0);

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

    auto converter = new TrackSeedTrackMapConverter;
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
    auto deltazcorr = new PHTpcDeltaZCorrection;
    deltazcorr->Verbosity(0);
    se->registerSubsystem(deltazcorr);

    // perform final track fit with ACTS
    auto actsFit = new PHActsTrkFitter;
    actsFit->Verbosity(0);
    actsFit->commissioning(G4TRACKING::use_alignment);
    // in calibration mode, fit only Silicons and Micromegas hits
    actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
    actsFit->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
    actsFit->set_pp_mode(TRACKING::pp_mode);
    actsFit->set_use_clustermover(true);  // default is true for now
    actsFit->useActsEvaluator(false);
    actsFit->useOutlierFinder(false);
    actsFit->setFieldMap(G4MAGNET::magfield_tracking);
    se->registerSubsystem(actsFit);
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

  TString residoutfile = theOutfile + "_resid.root";
  std::string residstring(residoutfile.Data());

  auto resid = new TrackResiduals("TrackResiduals");
  resid->outfileName(residstring);
  resid->alignment(false);
  resid->clusterTree();
  resid->hitTree();
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
    TString qaname = theOutfile + "_qa.root";
    std::string qaOutputFileName(qaname.Data());
    QAHistManagerDef::saveQARootFile(qaOutputFileName);
  }

  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
