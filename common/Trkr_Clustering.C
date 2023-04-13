#ifndef MACRO_TRKRCLUS_C
#define MACRO_TRKRCLUS_C

#include <GlobalVariables.C>

#include <G4_TrkrVariables.C>
#include <G4_ActsGeom.C>

#include <mvtx/MvtxHitPruner.h>
#include <mvtx/MvtxClusterizer.h>
#include <intt/InttClusterizer.h>

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
  mvtxclusterizer->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(mvtxclusterizer);
}

void Intt_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  InttClusterizer* inttclusterizer = new InttClusterizer("InttClusterizer", G4MVTX::n_maps_layer, G4MVTX::n_maps_layer + G4INTT::n_intt_layer - 1);
  inttclusterizer->Verbosity(verbosity);
  inttclusterizer->set_cluster_version(G4TRACKING::cluster_version);
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
  tpcclusterizer->set_cluster_version(G4TRACKING::cluster_version);
  tpcclusterizer->set_do_hit_association( G4TPC::DO_HIT_ASSOCIATION );
  se->registerSubsystem(tpcclusterizer);
  
 
  auto tpcclustercleaner = new TpcClusterCleaner;
  tpcclustercleaner->Verbosity(verbosity);
  tpcclustercleaner->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(tpcclustercleaner);
  
}

void Micromegas_Clustering()
{
  auto se = Fun4AllServer::instance();
  auto mm_clus = new MicromegasClusterizer;
  mm_clus->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(mm_clus);
}


#endif
