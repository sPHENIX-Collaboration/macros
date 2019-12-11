#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <caloreco/RawClusterBuilderTopo.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalo_reco.so)
#endif

void TopoClusterRecoInit() {}

void TopoClusterReco(int verbosity = 0) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libcalo_reco.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  RawClusterBuilderTopo* ClusterBuilder = new RawClusterBuilderTopo("HcalRawClusterBuilderTopo");
  ClusterBuilder->Verbosity( verbosity );
  ClusterBuilder->set_noise( 0.0025 , 0.006 );
  ClusterBuilder->set_significance( 4.0 , 2.0 , 0.0 );
  ClusterBuilder->allow_corner_neighbor( true );
  se->registerSubsystem( ClusterBuilder );
  
  return;

}  
