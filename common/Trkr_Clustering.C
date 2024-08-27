#ifndef MACRO_TRKRCLUS_C
#define MACRO_TRKRCLUS_C

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_TrkrVariables.C>

#include <intt/InttCombinedRawDataDecoder.h>
#include <micromegas/MicromegasCombinedDataDecoder.h>
#include <mvtx/MvtxCombinedRawDataDecoder.h>
#include <tpc/TpcCombinedRawDataUnpacker.h>

#include <intt/InttClusterizer.h>
#include <mvtx/MvtxClusterizer.h>
#include <mvtx/MvtxHitPruner.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <tpc/TpcClusterizer.h>
#pragma GCC diagnostic pop

#include <tpc/TpcClusterCleaner.h>

#include <micromegas/MicromegasClusterizer.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libmicromegas.so)
R__LOAD_LIBRARY(libtrack_reco.so)

void ClusteringInit()
{
  ACTSGEOM::ActsGeomInit();
}

void Mvtx_HitUnpacking(const std::string& felix="")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  auto mvtxunpacker = new MvtxCombinedRawDataDecoder;
  mvtxunpacker->Verbosity(verbosity);
  if(felix.length() > 0)
    {
      mvtxunpacker->useRawHitNodeName("MVTXRAWHIT_" + felix);
      mvtxunpacker->useRawEvtHeaderNodeName("MVTXRAWEVTHEADER_" + felix);
    }
  se->registerSubsystem(mvtxunpacker);
}
void Mvtx_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  // prune the extra MVTX hits due to multiple strobes per hit
  MvtxHitPruner* mvtxhitpruner = new MvtxHitPruner();
  mvtxhitpruner->Verbosity(verbosity);
  se->registerSubsystem(mvtxhitpruner);

  // For the Mvtx layers
  //================
  MvtxClusterizer* mvtxclusterizer = new MvtxClusterizer("MvtxClusterizer");
  mvtxclusterizer->Verbosity(verbosity);
  se->registerSubsystem(mvtxclusterizer);
}
void Intt_HitUnpacking(const std::string& server="")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  auto inttunpacker = new InttCombinedRawDataDecoder;
  inttunpacker->Verbosity(verbosity);
  inttunpacker->LoadHotChannelMapRemote("INTT_HotMap");
   if(server.length() > 0)
    {
      inttunpacker->useRawHitNodeName("INTTRAWHIT_" + server);
    }
  se->registerSubsystem(inttunpacker);
}
void Intt_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  InttClusterizer* inttclusterizer = new InttClusterizer("InttClusterizer", G4MVTX::n_maps_layer, G4MVTX::n_maps_layer + G4INTT::n_intt_layer - 1);
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

void Tpc_HitUnpacking(const std::string& ebdc="")
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  auto tpcunpacker = new TpcCombinedRawDataUnpacker;
  tpcunpacker->set_presampleShift(TRACKING::reco_tpc_time_presample);
  if(ebdc.length() > 0)
    {
      tpcunpacker->useRawHitNodeName("TPCRAWHIT_" + ebdc);
    }
  if(TRACKING::tpc_zero_supp)
    {
      tpcunpacker->ReadZeroSuppressedData();
    }
  tpcunpacker->Verbosity(verbosity);
  se->registerSubsystem(tpcunpacker);
}

void TPC_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  ACTSGEOM::ActsGeomInit();
  Fun4AllServer* se = Fun4AllServer::instance();

  //-------------
  // Cluster Hits
  //-------------

  // For the Tpc
  //==========

  auto tpcclusterizer = new TpcClusterizer;
  tpcclusterizer->Verbosity(verbosity);
  tpcclusterizer->set_do_hit_association(G4TPC::DO_HIT_ASSOCIATION);
  se->registerSubsystem(tpcclusterizer);

  auto tpcclustercleaner = new TpcClusterCleaner;
  tpcclustercleaner->Verbosity(verbosity);
  se->registerSubsystem(tpcclustercleaner);
}

void Micromegas_HitUnpacking()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MICROMEGAS_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  auto tpotunpacker = new MicromegasCombinedDataDecoder;
  std::string calibrationFile = CDBInterface::instance()->getUrl("TPOT_Pedestal");
  tpotunpacker->set_calibration_file(calibrationFile);
  se->registerSubsystem(tpotunpacker);
}

void Micromegas_Clustering()
{
  auto se = Fun4AllServer::instance();
  auto mm_clus = new MicromegasClusterizer;
  se->registerSubsystem(mm_clus);
}

#endif
