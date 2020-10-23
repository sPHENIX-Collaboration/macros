#ifndef COMMON_G4BBC_C
#define COMMON_G4BBC_C

#include <g4bbc/BbcVertexFastSimReco.h>
#include <fun4all/Fun4AllServer.h>
#include "g4detectors/PHG4BbcSubsystem.h"


R__LOAD_LIBRARY(libg4bbc.so)

namespace Enable
{
  bool BBC = false;
  bool BBCFAKE = false;
}

namespace G4BBC
{
  double z_smearing = 0.;     // should be 6 mm, temporarily perfect for TPC initial vertexing
  double t_smearing = 0.02;   // 20ps timing resolution
}  // namespace G4BBC

void BbcInit()
{
  //cout << "In BbcInit()" << endl;

  // Set boundary of tracked particles to include BBC (values in cm)
  if ( Enable::BBC == true )
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 300.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -300.);
    BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 15.0);
  }

}


double Bbc(PHG4Reco* g4Reco, double radius)
{
  //cout << "XXXX CREATING BBC" << endl;

  gSystem->Load("libg4detectors.so");

  if ( Enable::BBC == true )
  {
    PHG4BbcSubsystem *bbc = new PHG4BbcSubsystem( "BBC" );
    bbc->SuperDetector( "BBC" );
    g4Reco->registerSubsystem( bbc );

    radius = 15.;  // outer shell is 15 cm
  }

  return radius; 
}


void Bbc_Reco(int verbosity = 0)
{
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  // If BBC is enabled, it overrides the BBCFAKE
  if ( Enable::BBCFAKE == true && Enable::BBC == false )
  {
    cout << "BBCFAKE: Using smeared vtx and t0 resolutions of "
      << G4BBC::z_smearing << " cm and " << G4BBC::t_smearing*1000 << " ps" << endl;
    BbcVertexFastSimReco* bbcvertex = new BbcVertexFastSimReco();
    bbcvertex->set_z_smearing(G4BBC::z_smearing);  
    bbcvertex->set_t_smearing(G4BBC::t_smearing);
    se->registerSubsystem(bbcvertex);
  }

  return;
}
#endif
