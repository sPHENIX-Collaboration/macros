#ifndef MACRO_TRKRCLUS_C
#define MACRO_TRKRCLUS_C

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_TrkrVariables.C>

#include <intt/InttClusterizer.h>
#include <intt/InttCombinedRawDataDecoder.h>
#include <intt/InttOdbcQuery.h>

#include <micromegas/MicromegasCombinedDataDecoder.h>

#include <mvtx/MvtxClusterPruner.h>
#include <mvtx/MvtxClusterizer.h>
#include <mvtx/MvtxCombinedRawDataDecoder.h>

#include <tpc/LaserEventIdentifier.h>
#include <tpc/TpcClusterCleaner.h>
#include <tpc/TpcCombinedRawDataUnpacker.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <tpc/TpcClusterizer.h>
#pragma GCC diagnostic pop

#include <micromegas/MicromegasClusterizer.h>

#include <fun4all/Fun4AllServer.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libphool.so)

void ClusteringInit()
{
  ACTSGEOM::ActsGeomInit();
}

void Mvtx_HitUnpacking(const std::string &felix = "")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  auto *mvtxunpacker = new MvtxCombinedRawDataDecoder("MvtxCombinedRawDataDecoder" + felix);
  mvtxunpacker->Verbosity(verbosity);
  if (!felix.empty())
  {
    mvtxunpacker->useRawHitNodeName("MVTXRAWHIT_" + felix);
    mvtxunpacker->useRawEvtHeaderNodeName("MVTXRAWEVTHEADER_" + felix);
  }
  se->registerSubsystem(mvtxunpacker);
}

void Mvtx_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  auto *se = Fun4AllServer::instance();

  // clusterizer
  auto *mvtxclusterizer = new MvtxClusterizer;
  mvtxclusterizer->Verbosity(verbosity);
  se->registerSubsystem(mvtxclusterizer);

  // cluster pruner
  auto *mvtxClusterPruner = new MvtxClusterPruner;
  mvtxClusterPruner->set_use_strict_matching(true);
  se->registerSubsystem(mvtxClusterPruner);
}

void Intt_HitUnpacking(const std::string &server = "")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();
  auto *rc = recoConsts::instance();
  int runnumber = rc->get_IntFlag("RUNNUMBER");
  InttOdbcQuery query;
  bool isStreaming = true;
  if (runnumber != 0)
  {
    query.Query(runnumber);
    isStreaming = query.IsStreaming();
  }
  auto *inttunpacker = new InttCombinedRawDataDecoder("InttCombinedRawDataDecoder" + server);
  inttunpacker->Verbosity(verbosity);
  /// Only necessary to call the following method if using a non-default calibration
  inttunpacker->LoadBadChannelMap("INTT_HotMap");
  inttunpacker->set_triggeredMode(!isStreaming);
  if (!server.empty())
  {
    inttunpacker->useRawHitNodeName("INTTRAWHIT_" + server);
  }
  se->registerSubsystem(inttunpacker);
}
void Intt_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  InttClusterizer *inttclusterizer = new InttClusterizer("InttClusterizer", G4MVTX::n_maps_layer, G4MVTX::n_maps_layer + G4INTT::n_intt_layer - 1);
  inttclusterizer->Verbosity(verbosity);
  // no Z clustering for Intt type 1 layers (we DO want Z clustering for type 0 layers)
  // turning off phi clustering for type 0 layers is not necessary, there is only one strip
  // per sensor in phi
  for (int i = G4MVTX::n_maps_layer; i < G4MVTX::n_maps_layer + G4INTT::n_intt_layer; i++)
  {
    if (G4INTT::laddertype[i - G4MVTX::n_maps_layer] == PHG4InttDefs::SEGMENTATION_PHI)
    {
      inttclusterizer->set_z_clustering(i, false);
    }
  }
  se->registerSubsystem(inttclusterizer);
}

void Tpc_HitUnpacking(const std::string &ebdc = "")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();
  // std::string name = "TpcCombinedRawDataUnpacker"+ebdc;
  auto *tpcunpacker = new TpcCombinedRawDataUnpacker("TpcCombinedRawDataUnpacker" + ebdc);
  tpcunpacker->set_presampleShift(TRACKING::reco_tpc_time_presample);
  tpcunpacker->set_t0(TRACKING::reco_t0);
  if (!ebdc.empty())
  {
    tpcunpacker->useRawHitNodeName("TPCRAWHIT_" + ebdc);
  }
  if (TRACKING::tpc_zero_supp)
  {
    tpcunpacker->ReadZeroSuppressedData();
  }
  tpcunpacker->doBaselineCorr(TRACKING::tpc_baseline_corr);
  tpcunpacker->Verbosity(verbosity);
  se->registerSubsystem(tpcunpacker);
}

void Tpc_LaserEventIdentifying()
{
  //  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();
  auto *rc = recoConsts::instance();
  int runnumber = rc->get_IntFlag("RUNNUMBER");
  auto *laserEventIdentifier = new LaserEventIdentifier;
  if (!G4TPC::laser_event_debug_filename.empty())
  {
    laserEventIdentifier->set_debug(true);
    laserEventIdentifier->set_debug_name(G4TPC::laser_event_debug_filename);
  }
  laserEventIdentifier->set_max_time_samples(TRACKING::reco_tpc_maxtime_sample);
  laserEventIdentifier->set_runnumber(runnumber);
  se->registerSubsystem(laserEventIdentifier);
}

void TPC_Clustering_run2pp()
{
  //  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  ACTSGEOM::ActsGeomInit();
  Fun4AllServer *se = Fun4AllServer::instance();

  auto *tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(0);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  tpcclusterizer->set_rawdata_reco();
  tpcclusterizer->set_reject_event(G4TPC::REJECT_LASER_EVENTS);
  se->registerSubsystem(tpcclusterizer);
}
void TPC_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  ACTSGEOM::ActsGeomInit();
  Fun4AllServer *se = Fun4AllServer::instance();

  //-------------
  // Cluster Hits
  //-------------

  // For the Tpc
  //==========

  auto *tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(verbosity);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  tpcclusterizer->set_min_err_squared(0.000001);
  se->registerSubsystem(tpcclusterizer);

  auto *tpcclustercleaner = new TpcClusterCleaner;
  tpcclustercleaner->Verbosity(verbosity);
  tpcclustercleaner->set_rphi_error_low_cut(0.001);
  se->registerSubsystem(tpcclustercleaner);
}

void Micromegas_HitUnpacking()
{
  //  int verbosity = std::max(Enable::VERBOSITY, Enable::MICROMEGAS_VERBOSITY);
  Fun4AllServer *se = Fun4AllServer::instance();

  auto *tpotunpacker = new MicromegasCombinedDataDecoder;
  std::string calibrationFile = CDBInterface::instance()->getUrl("TPOT_Pedestal");
  tpotunpacker->set_calibration_file(calibrationFile);
  tpotunpacker->set_sample_max(1024);
  se->registerSubsystem(tpotunpacker);
}

void Micromegas_Clustering()
{
  auto *se = Fun4AllServer::instance();
  auto *mm_clus = new MicromegasClusterizer;
  const auto calibrationFile = CDBInterface::instance()->getUrl("TPOT_Pedestal");
  mm_clus->set_calibration_file(calibrationFile);
  se->registerSubsystem(mm_clus);
}

#endif
