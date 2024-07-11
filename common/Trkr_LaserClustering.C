#ifndef MACRO_TRKRLASERCLUS_C
#define MACRO_TRKRLASERCLUS_C

#include <GlobalVariables.C>

#include <G4_TrkrVariables.C>
//#include <G4_ActsGeom.C>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <tpc/TpcClusterizer.h>
#include <tpc/TpcSimpleClusterizer.h>
#include <tpc/LaserClusterizer.h>
#pragma GCC diagnostic pop

#include <tpc/TpcClusterCleaner.h>

#include <tpccalib/TpcCentralMembraneMatching.h>
#include <tpccalib/TpcDirectLaserReconstruction.h>
#include <micromegas/MicromegasClusterizer.h>

#include <fun4all/Fun4AllServer.h>


R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)




void TPC_LaserClustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  ACTSGEOM::ActsGeomInit();
  Fun4AllServer* se = Fun4AllServer::instance();

  //-------------
  // Cluster Hits
  //-------------

  // For the Tpc
  //==========
  if( G4TPC::USE_SIMPLE_CLUSTERIZER )
  {
    
    auto tpcclusterizer = new TpcSimpleClusterizer;
    tpcclusterizer->Verbosity(verbosity);
    se->registerSubsystem(tpcclusterizer);
    
  } else {

    auto tpcclusterizer = new TpcClusterizer;
    tpcclusterizer->Verbosity(verbosity);
    tpcclusterizer->set_do_hit_association( G4TPC::DO_HIT_ASSOCIATION );
    se->registerSubsystem(tpcclusterizer);

  }
  
  if( !G4TPC::ENABLE_DIRECT_LASER_HITS )
  {
    auto tpcclustercleaner = new TpcClusterCleaner;
    tpcclustercleaner->Verbosity(verbosity);
    se->registerSubsystem(tpcclustercleaner);
  }

  // direct laser reconstruction
  if( G4TPC::ENABLE_DIRECT_LASER_HITS )
  { 
    auto directLaserReconstruction = new TpcDirectLaserReconstruction;
    directLaserReconstruction->set_outputfile( G4TPC::DIRECT_LASER_ROOTOUTPUT_FILENAME );
    directLaserReconstruction->set_savehistograms( G4TPC::DIRECT_LASER_SAVEHISTOGRAMS );
    directLaserReconstruction->set_histogram_outputfile( G4TPC::DIRECT_LASER_HISTOGRAMOUTPUT_FILENAME );
    se->registerSubsystem(directLaserReconstruction); 
  }
  
  // central membrane reconstruction
  if( G4TPC::ENABLE_CENTRAL_MEMBRANE_HITS )
    {

      auto laserClusterizer = new LaserClusterizer;
      laserClusterizer->Verbosity(verbosity);
      se->registerSubsystem(laserClusterizer);

      // match central membrane clusters to pads and generate distortion correction
      auto centralMembraneMatcher = new TpcCentralMembraneMatching;
      centralMembraneMatcher->Verbosity( verbosity );
      se->registerSubsystem(centralMembraneMatcher);
    }
}


#endif
