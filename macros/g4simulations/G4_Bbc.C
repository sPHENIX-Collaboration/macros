#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <g4bbc/BbcVertexFastSimReco.h>
R__LOAD_LIBRARY(libg4bbc.so)
#endif

void BbcInit() {}

double Bbc(PHG4Reco* g4Reco,
	   double radius,
	   const int absorberactive = 0,
	   int verbosity = 0) {

  // the BBC is a fast sim only at the moment
  // this is a place holder for the G4 material setup
  
  return radius; 
}

void Bbc_Reco(int verbosity = 0) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4bbc.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  BbcVertexFastSimReco* bbcvertex = new BbcVertexFastSimReco();
  bbcvertex->set_z_smearing(0.0);   // 6 mm, temporarily perfect for TPC initial vertexing
  bbcvertex->set_t_smearing(0.002); // 20 ps
  se->registerSubsystem(bbcvertex);

  return;
}  
