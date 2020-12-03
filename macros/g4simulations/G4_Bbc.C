#pragma once

#include <g4bbc/BbcVertexFastSimReco.h>
#include <fun4all/Fun4AllServer.h>
#include <g4detectors/PHG4BbcSubsystem.h>

R__LOAD_LIBRARY(libg4bbc.so)

namespace Enable
{
  bool BBC = false;
}

namespace G4BBC
{
  double z_smearing = 0.; // should be 6mm, temporary to 0 for TPC
  double t_smearing = 0.002; // 20ps timing resolution
}

void BbcInit()
{
  cout << "BbcInit()" << endl;
  // Nothing to Init for now
}

double Bbc(PHG4Reco* g4Reco,
	   double radius,
	   const int absorberactive = 0,
	   int verbosity = 0)
{

  gSystem->Load("libg4detectors.so");

  cout << "XXXX CREATING BBC" << endl;
  PHG4BbcSubsystem *bbc = new PHG4BbcSubsystem( "BBC" );
  bbc->SuperDetector( "BBC" );
  g4Reco->registerSubsystem( bbc );

  radius = 15.;  // outer shell is 15 cm
  return radius; 
}


void Bbc_Reco(int verbosity = 0)
{

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  // disable fast sim for now
  int do_bbcfast = 0;
  if ( do_bbcfast )
  {
    BbcVertexFastSimReco* bbcvertex = new BbcVertexFastSimReco();
    bbcvertex->set_z_smearing(G4BBC::z_smearing);
    bbcvertex->set_t_smearing(G4BBC::t_smearing);
    se->registerSubsystem(bbcvertex);
  }

  return;
}  
