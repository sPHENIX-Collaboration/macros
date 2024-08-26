#ifndef MACRO_TRKRLASERCLUS_C
#define MACRO_TRKRLASERCLUS_C

#include <GlobalVariables.C>

#include <G4_TrkrVariables.C>

#include <tpc/LaserClusterizer.h>

#include <fun4all/Fun4AllServer.h>


R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)




void TPC_LaserClustering()
{

  // central membrane reconstruction
  if( G4TPC::ENABLE_CENTRAL_MEMBRANE_HITS )
    {
      
      int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
      ACTSGEOM::ActsGeomInit();
      Fun4AllServer* se = Fun4AllServer::instance();
      
      LaserClusterizer *laserClusterizer = new LaserClusterizer;
      laserClusterizer->Verbosity(verbosity);
      laserClusterizer->set_max_time_samples(TRACKING::reco_tpc_maxtime_sample);
      laserClusterizer->set_adc_threshold(G4TPC::laser_adc_threshold);
      se->registerSubsystem(laserClusterizer);

    }
}


#endif
