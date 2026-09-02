/*
 * This macro shows a minimum working example of running the
 * TPC track reconstruction starting with hit unpackers
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
#include <Trkr_LaserClustering.C>
#include <Trkr_Reco.C>
#include <Trkr_RecoInit.C>
#include <Trkr_TpcReadoutInit.C>

#include <cdbobjects/CDBTTree.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>

#include <fun4all/Fun4AllReturnCodes.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <tpctrackreco/TpcCrossingFinder.h>
#include <tpctrackreco/TpcPolyClusterTrkrClusterConverter.h>
#include <tpctrackreco/TpcPolyTrackSeedConverter.h>
#include <tpctrackreco/Tpc_AssembledTrackReco.h>
#include <tpctrackreco/Tpc_ModuleTrackReco.h>
#include <tpctrackreco/Tpc_PolyClusterizer.h>
#include <tpctrackreco/Tpc_PolyTrackReco.h>
#include <tpctrackreco/Tpc_PolyTrackVertexer.h>

#include <trackingdiagnostics/Tpc_AssembledTrackDisplay.h>
#include <trackingdiagnostics/Tpc_ModuleTrackDisplay.h>
#include <trackingdiagnostics/Tpc_PolyClusterDisplay.h>
#include <trackingdiagnostics/Tpc_PolyClusterResiduals.h>
#include <trackingdiagnostics/TrackResiduals.h>

#include <format>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libcdbobjects.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libPHGarfield.so)
R__LOAD_LIBRARY(libtpctrackreco.so)
R__LOAD_LIBRARY(libTrackingDiagnostics.so)

class SkipFirstN : public SubsysReco
{
 public:
  explicit SkipFirstN(int n)
    : SubsysReco("SkipFirstN")
    , target_(n)
  {
  }
  int process_event(PHCompositeNode * /*unused*/) override
  {
    if (count_ < target_)
    {
      ++count_;
      return Fun4AllReturnCodes::ABORTEVENT;
    }
    return Fun4AllReturnCodes::EVENT_OK;
  }

 private:
  int target_ = 0;
  int count_ = 0;
};
// 'Golden' store of p+p
//.x Fun4All_raw_hit_TPC_reco.C(2, 79513, 0, ".", 0, "run3pp", "ana532_nocdbtag_v001","HITS_ppFieldOn")
//.x Fun4All_raw_hit_TPC_reco.C(2, 79516, 0, ".", 0, "run3pp", "ana532_nocdbtag_v001","HITS_ppFieldOn")

// 6x6       75570-75573 - 1mrad; 75574 - 0mrad
//.x Fun4All_raw_hit_TPC_reco.C(10, 75570, 0, ".", 0, "run3auau", "ana514_nocdbtag_v001","HITS_AuAu_ZeroField_1mrad")
//.x Fun4All_raw_hit_TPC_reco.C(10, 75574, 0, ".", 0, "run3auau", "ana514_nocdbtag_v001","HITS_AuAu_ZeroField_0mrad")

// 111x111 Au+Au Field On; 75391 - close to end of store, 75405 - beginning of the next store
//.x Fun4All_raw_hit_TPC_reco.C(2, 75391, 0, ".", 0, "run3auau", "ana514_nocdbtag_v001","HITS_AuAuFieldOn")
//.x Fun4All_raw_hit_TPC_reco.C(2, 75405, 0, ".", 0, "run3auau", "ana514_nocdbtag_v001","HITS_AuAuFieldOn")

// 111x111 O+O
//.x Fun4All_raw_hit_TPC_reco.C(2, 82626, 0, ".", 0, "run3oo", "ana537_nocdbtag_v001","HITS_OO")

void Fun4All_raw_hit_TPC_Matched_reco(
    const int nEvents = 10,
    const int runnumber = 79513,
    const int segment = 0,
    const std::string &outdir = ".",
    const int nSkip = 0,
    const std::string &collision = "run3pp",
    const std::string &production = "ana532_nocdbtag_v001",
    const std::string &outfilename = "ppFieldOn",
    const std::string &datatype = "physics")
{
  const bool convertSeeds = false;
  auto *se = Fun4AllServer::instance();
  se->Verbosity(1);
  auto *rc = recoConsts::instance();
  se->registerSubsystem(new SkipFirstN(nSkip));

  const std::string dsttype = "STREAMING_EVENT";
  const std::string dsttype_to_save = "TPC";

  G4TPC::sampa_tzero_bias = 0;

  // First order corrections will be applied from PHGarfield
  G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS = false;
  G4TPC::ENABLE_STATIC_CORRECTIONS = false;
  G4TPC::ENABLE_AVERAGE_CORRECTIONS = false;

  std::ostringstream runstr;
  runstr << std::setw(8) << std::setfill('0') << runnumber;

  std::ostringstream segstr;
  segstr << std::setw(5) << std::setfill('0') << segment;

  const int runbase = (runnumber / 100) * 100;
  const int runnext = runbase + 100;

  std::ostringstream rundir;
  rundir << "run_" << std::setw(8) << std::setfill('0') << runbase << "_" << std::setw(8) << std::setfill('0') << runnext;

  std::vector<std::string> streams;

  // TPC EBDC streams
  for (int ebdc = 0; ebdc < 24; ++ebdc)
  {
    for (int endpoint = 0; endpoint < 2; ++endpoint)
    {
      std::ostringstream s;
      s << "ebdc" << std::setw(2) << std::setfill('0') << ebdc << "_" << endpoint;
      streams.push_back(s.str());
    }
  }

  // TPOT
  streams.emplace_back("ebdc39");

  // INTT streams
  for (int server = 0; server < 8; ++server)
  {
    std::ostringstream s;
    s << "intt" << server;
    streams.push_back(s.str());
  }

  // MVTX streams
  for (int felix = 0; felix < 6; ++felix)
  {
    std::ostringstream s;
    s << "mvtx" << felix;
    streams.push_back(s.str());
  }

  int i = 0;
  std::stringstream nice_runnumber;
  nice_runnumber << std::setw(8) << std::setfill('0') << std::to_string(runnumber);

  int rounded_up = 100 * (std::ceil((float) runnumber / 100));
  std::stringstream nice_rounded_up;
  nice_rounded_up << std::setw(8) << std::setfill('0') << std::to_string(rounded_up);

  int rounded_down = 100 * (std::floor((float) runnumber / 100));
  std::stringstream nice_rounded_down;
  nice_rounded_down << std::setw(8) << std::setfill('0') << std::to_string(rounded_down);

  for (const auto& stream : streams)
  {
    std::string filename = std::format(
    "DST_{}_{}_{}_{}-{}-{}.root",
    dsttype, stream, collision, production, runstr.str(), segstr.str());

    std::string filepath = std::format(
    "/sphenix/lustre01/sphnxpro/production/{}/{}/{}/DST_{}_{}/run_{}_{}/{}",
    collision, datatype, production, dsttype, stream,
    nice_rounded_down.str(), nice_rounded_up.str(), filename);
    std::cout << "Adding DST: " << filepath << std::endl;
    if (i == 0)
    {
      rc->set_IntFlag("RUNNUMBER", runnumber);
      rc->set_uint64Flag("TIMESTAMP", runnumber);
    }

    std::string inputname = "InputManager" + std::to_string(i);
    auto *hitsin = new Fun4AllDstInputManager(inputname);
    hitsin->fileopen(filename);
    se->registerInputManager(hitsin);

    ++i;
  }

  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("RUNSEGMENT", segment);

  Enable::QA = false;
  Enable::CDB = true;
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  G4TRACKING::convert_seeds_to_svtxtracks = convertSeeds;

  G4MAGNET::magfield_rescale = 1;

  std::cout << " run: " << runnumber
            << " samples: " << TRACKING::reco_tpc_maxtime_sample
            << " pre: " << TRACKING::reco_tpc_time_presample
            << " vdrift: " << G4TPC::tpc_drift_velocity_reco
            << std::endl;

  TRACKING::streaming_mode = true;

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);

  TpcReadoutInit(runnumber);
  G4TPC::REJECT_LASER_EVENTS = true;
  // Flag for running the tpc hit unpacker with zero suppression on
  TRACKING::tpc_zero_supp = true;

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

  //==============================================================

  Mvtx_Clustering();
  Intt_Clustering();
  Micromegas_Clustering();

  //==============================================================

  Tpc_LaserEventIdentifying();
  Reject_Laser_Events();

  //==============================================================

  Tracking_Reco_SiliconSeed_run2pp();
  auto *converter = new TrackSeedTrackMapConverter("SiliconSeedToSvtxTrackMap");
  converter->setTrackSeedName("SiliconTrackSeedContainer");
  converter->setTrackMapName("SiliconSvtxTrackMap");
  converter->setClusterMapName("TRKR_CLUSTER");
  se->registerSubsystem(converter);

  auto *finder_svx = new PHSimpleVertexFinder("SiliconVertexFinder");
  finder_svx->Verbosity(0);
  finder_svx->setDcaCut(0.1);
  finder_svx->setTrackPtCut(0.2);
  finder_svx->setBeamLineCut(1);
  finder_svx->setTrackQualityCut(500);
  finder_svx->setNmvtxRequired(3);
  finder_svx->setOutlierPairCut(0.1);
  finder_svx->setTrackMapName("SiliconSvtxTrackMap");
  finder_svx->setVertexMapName("SiliconSvtxVertexMap");
  se->registerSubsystem(finder_svx);

  //==============================================================

  se->registerSubsystem(new Tpc_ModuleTrackReco());     // makes TPC_MODULETRACKS
  se->registerSubsystem(new Tpc_AssembledTrackReco());  // makes TPC_ASSEMBLEDTRACKS

  auto *crossingFinder = new TpcCrossingFinder();
  crossingFinder->Verbosity(0);
  crossingFinder->setInputNodeName("TPC_ASSEMBLEDTRACKS");
  crossingFinder->setOutputNodeName("TPC_CROSSING_DECISIONS");
  crossingFinder->setVertexMapNodeName("SiliconSvtxVertexMap");  // optional, configurable
  se->registerSubsystem(crossingFinder);

  auto *cluster = new Tpc_PolyClusterizer();  // makes TPC_POLYCLUSTERS
  cluster->setUseSurveyGeometry(false);
  cluster->setKEffSide0(1.00);  // OO 82626 - 4.5, AuAu 6x6 76905 -0, pp 79513 - 1.0, 75391 5.8 75405 4.8
  cluster->setKEffSide1(1.60);  // OO 82626 - 5.0, AuAu 6x6 76905 -0, pp 79513 - 1.6, 75391 5.6 75408 4.8
  cluster->setMaxAcceptedTier(2);
  se->registerSubsystem(cluster);

  se->registerSubsystem(new Tpc_PolyTrackReco());      // makes TPC_POLYTRACKS
  se->registerSubsystem(new Tpc_PolyTrackVertexer());  // makes TPC_POLYTRACKVERTICES

  se->registerSubsystem(new TpcPolyTrackSeedConverter());           // converts TPC_POLYTRACKS to TpcTrackSeed
  se->registerSubsystem(new TpcPolyClusterTrkrClusterConverter());  // converts TPC_POLYCLUSTERS to TRKR_CLUSTER

  auto *silicon_match = new PHSiliconTpcTrackMatching;
  silicon_match->Verbosity(0);
  silicon_match->set_pp_mode(TRACKING::streaming_mode);
  silicon_match->set_cluster_map_name("TRKR_CLUSTER");
  silicon_match->window_deta.set_posQoverpT_maxabs({-0.014, 0.0331, 0.48});
  silicon_match->window_deta.set_negQoverpT_maxabs({-0.014, 0.0331, 0.48});
  silicon_match->set_deltaeta_min(0.03);
  silicon_match->window_dphi.set_QoverpT_range({-0.15, 0, 0}, {0.15, 0, 0});
  silicon_match->window_dx.set_QoverpT_maxabs({3.0, 0, 0});
  silicon_match->window_dy.set_QoverpT_maxabs({3.0, 0, 0});
  silicon_match->window_dz.set_posQoverpT_maxabs({1.138, 0.3919, 0.84});
  silicon_match->window_dz.set_negQoverpT_maxabs({1.138, 0.3919, 0.84});
  silicon_match->set_crossing_deltaz_max(10);
  silicon_match->set_crossing_deltaz_min(0);
  silicon_match->set_test_windows_printout(false);
  silicon_match->set_max_crossing_diff(10); // good for poly seeding case
  // these are for testing. and default to false. The seed matcher will choose the crossing that works best
  //  silicon_match->set_use_tpc_crossing_only(false);  // use crossing information from TPC SA seed
  //  silicon_match->set_use_silicon_crossing_only(false);  // use crossing information from silicon seed
  se->registerSubsystem(silicon_match);

  // TPOT matching
  Tracking_Reco_TpcTpotTrackMatching_run2pp();

  auto *deltazcorr = new PHTpcDeltaZCorrection;
  deltazcorr->Verbosity(0);
  se->registerSubsystem(deltazcorr);

  auto *actsFit = new PHActsTrkFitter;
  actsFit->Verbosity(0);
  actsFit->commissioning(G4TRACKING::use_alignment);
  actsFit->setTrkrClusterContainerName("TRKR_CLUSTER");
  // in calibration mode, fit only Silicons and Micromegas hits
  actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
  actsFit->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
  actsFit->set_pp_mode(TRACKING::streaming_mode);
  actsFit->set_use_clustermover(false);
  actsFit->useActsEvaluator(false);
  actsFit->useOutlierFinder(false);
  actsFit->setFieldMap(G4MAGNET::magfield_tracking);
  se->registerSubsystem(actsFit);

  auto *cleaner = new PHTrackCleaner();
  cleaner->Verbosity(0);
  cleaner->set_quality_cut(1000);
  cleaner->set_pp_mode(TRACKING::streaming_mode);
  se->registerSubsystem(cleaner);

  auto *finder = new PHSimpleVertexFinder("SvtxVertexFinder");
  finder->Verbosity(0);
  finder->setDcaCut(0.1);
  finder->setTrackPtCut(0.2);
  finder->setBeamLineCut(1);
  finder->setTrackQualityCut(500);
  finder->setNmvtxRequired(3);
  finder->setOutlierPairCut(0.1);
  finder->setTrackMapName("SvtxTrackMap");
  finder->setVertexMapName("SvtxVertexMap");
  se->registerSubsystem(finder);

  auto *resid = new TrackResiduals("TrackResiduals");
  resid->outfileName(outdir + "/TrackResiduals_" + outfilename + "_" + std::to_string(runnumber) + "_" + std::to_string(segment) + ".root");
  resid->alignment(false);
  resid->clusterTree();
  resid->vertexTree();
  resid->hitTree();
  resid->convertSeeds(G4TRACKING::convert_seeds_to_svtxtracks);
  resid->Verbosity(0);
  se->registerSubsystem(resid);

  Fun4AllOutputManager *out = new Fun4AllDstOutputManager("out", std::format("{}/output_DST/DST_{}_{}_{}-{}-{}.root",
            outdir,
            dsttype_to_save,
            collision,
            production,
            runnumber,
            segment));

  out->AddNode("Sync");
  out->AddNode("EventHeader");
  out->AddRunNode("TPCGEOMCONTAINER");
  // out->AddNode("TRKR_HITSET");
  // out->AddNode("TPC_MODULETRACKS");
  // out->AddNode("TPC_ASSEMBLEDTRACKS");
  out->AddNode("TPC_CROSSING_DECISIONS");
  out->AddNode("TPC_POLYCLUSTERS");
  out->AddNode("TPC_POLYTRACKS");
  out->AddNode("TPC_POLYTRACKVERTICES");
  out->AddNode("TRKR_CLUSTER");

  // se->registerOutputManager(out);

  se->run(nEvents + nSkip);
  se->Print("NODETREE");
  se->End();
  se->PrintTimer();

  CDBInterface::instance()->Print();
  delete se;
  std::cout << "Finished" << std::endl;
  gSystem->Exit(0);
}
