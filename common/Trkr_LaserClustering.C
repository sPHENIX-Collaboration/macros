#ifndef MACRO_TRKRLASERCLUS_C
#define MACRO_TRKRLASERCLUS_C

#include <GlobalVariables.C>

#include <G4_TrkrVariables.C>

#include <tpc/LaserClusterizer.h>
#include <tpccalib/TpcLaminationFitting.h>

#include <fun4all/Fun4AllServer.h>


R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)




void TPC_LaserClustering()
{

  // central membrane reconstruction
  if( G4TPC::ENABLE_CENTRAL_MEMBRANE_CLUSTERING )
    {
      
      int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
      ACTSGEOM::ActsGeomInit();
      Fun4AllServer* se = Fun4AllServer::instance();
      
      LaserClusterizer *laserClusterizer = new LaserClusterizer;
      laserClusterizer->Verbosity(verbosity);
      laserClusterizer->set_max_time_samples(TRACKING::reco_tpc_maxtime_sample);
      laserClusterizer->set_adc_threshold(G4TPC::laser_adc_threshold);
      if(G4TPC::laser_clusterizer_debug_filename != "")
      {
	laserClusterizer->set_debug(true);
	laserClusterizer->set_debug_name(G4TPC::laser_clusterizer_debug_filename);
      }
      se->registerSubsystem(laserClusterizer);

    }
}

void TPC_LaminationFitting()
{
  if( G4TPC::ENABLE_CENTRAL_MEMBRANE_CLUSTERING )
  {

    int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
    ACTSGEOM::ActsGeomInit();
    Fun4AllServer* se = Fun4AllServer::instance();
    
    TpcLaminationFitting *laminations = new TpcLaminationFitting;
    laminations->Verbosity(verbosity);
    laminations->set_nLayerCut(2);
    laminations->setOutputfile(G4TPC::LaminationOutputName);
    if(G4TPC::LaminationFitName != "")
    {
      laminations->set_fitFileName(G4TPC::LaminationFitName);
    }
    se->registerSubsystem(laminations);

  }
}


#endif
