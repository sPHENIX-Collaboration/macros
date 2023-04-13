#ifndef MACRO_TRKRCLUS_C
#define MACRO_TRKRCLUS_C

#include <GlobalVariables.C>

#include <G4_TrkrVariables.C>
//#include <G4_ActsGeom.C>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <tpc/TpcClusterizer.h>
#include <tpc/TpcSimpleClusterizer.h>
#pragma GCC diagnostic pop

#include <tpc/TpcClusterCleaner.h>

#include <tpccalib/PHTpcCentralMembraneClusterizer.h>
#include <tpccalib/PHTpcCentralMembraneMatcher.h>
#include <tpccalib/TpcDirectLaserReconstruction.h>
#include <micromegas/MicromegasClusterizer.h>

#include <fun4all/Fun4AllServer.h>


R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)




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
  if( G4TPC::USE_SIMPLE_CLUSTERIZER )
  {
    
    auto tpcclusterizer = new TpcSimpleClusterizer;
    tpcclusterizer->Verbosity(verbosity);
    se->registerSubsystem(tpcclusterizer);
    
  } else {

    auto tpcclusterizer = new TpcClusterizer;
    tpcclusterizer->Verbosity(verbosity);
    tpcclusterizer->set_cluster_version(G4TRACKING::cluster_version);
    tpcclusterizer->set_do_hit_association( G4TPC::DO_HIT_ASSOCIATION );
    se->registerSubsystem(tpcclusterizer);

  }
  
  if( !G4TPC::ENABLE_DIRECT_LASER_HITS )
  {
    auto tpcclustercleaner = new TpcClusterCleaner;
    tpcclustercleaner->Verbosity(verbosity);
    tpcclustercleaner->set_cluster_version(G4TRACKING::cluster_version);
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
    // central membrane clusterizer
    se->registerSubsystem(new PHTpcCentralMembraneClusterizer);
    
    // match central membrane clusters to pads and generate distortion correction
    auto centralMembraneMatcher = new PHTpcCentralMembraneMatcher;
    se->registerSubsystem(centralMembraneMatcher);
  }
}


#endif
