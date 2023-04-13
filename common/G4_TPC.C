#ifndef MACRO_G4TPC_C
#define MACRO_G4TPC_C

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_Intt.C>
#include <G4_Mvtx.C>
#include <QA.C>

#include <g4tpc/PHG4TpcCentralMembrane.h>
#include <g4tpc/PHG4TpcDigitizer.h>
#include <g4tpc/PHG4TpcDirectLaser.h>
#include <g4tpc/PHG4TpcDistortion.h>
#include <g4tpc/PHG4TpcElectronDrift.h>
#include <g4tpc/PHG4TpcEndCapSubsystem.h>
#include <g4tpc/PHG4TpcPadPlane.h>
#include <g4tpc/PHG4TpcPadPlaneReadout.h>
#include <g4tpc/PHG4TpcSubsystem.h>

#include <g4main/PHG4Reco.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <tpc/TpcClusterizer.h>
#include <tpc/TpcSimpleClusterizer.h>
#include <tpc/TpcClusterZCrossingCorrection.h>
#pragma GCC diagnostic pop

#include <tpc/TpcClusterCleaner.h>

#include <tpccalib/PHTpcCentralMembraneClusterizer.h>
#include <tpccalib/PHTpcCentralMembraneMatcher.h>
#include <tpccalib/TpcDirectLaserReconstruction.h>

#include <qa_modules/QAG4SimulationTpc.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4tpc.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libqa_modules.so)

void TPCInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4TPC::tpc_outer_radius);

  if (Enable::TPC_ENDCAP)
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 130.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -130.);
  }
  else
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 211. / 2.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -211. / 2.);
  }

  // the mvtx is not called if disabled but the default number of layers is set to 3,
  // so we need to set it to zero
  if (!Enable::MVTX)
  {
    G4MVTX::n_maps_layer = 0;
  }
  // same for the INTT
  if (!Enable::INTT)
  {
    G4INTT::n_intt_layer = 0;
  }

  // Set the (static) drift velocity in the cluster Z crossing correction module
  TpcClusterZCrossingCorrection::_vdrift = G4TPC::tpc_drift_velocity_reco;
}

//! TPC end cap, wagon wheel, electronics
void TPC_Endcaps(PHG4Reco* g4Reco)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TPC_OVERLAPCHECK;
  bool AbsorberActive = Enable::ABSORBER || Enable::TPC_ABSORBER;

  PHG4TpcEndCapSubsystem* tpc_endcap = new PHG4TpcEndCapSubsystem("TPC_ENDCAP");
  tpc_endcap->SuperDetector("TPC_ENDCAP");

  if (AbsorberActive) tpc_endcap->SetActive();
  tpc_endcap->OverlapCheck(OverlapCheck);

  //  tpc_endcap->set_int_param("construction_verbosity", 2);

  g4Reco->registerSubsystem(tpc_endcap);

  return;
}

double TPC(PHG4Reco* g4Reco,
           double radius)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TPC_OVERLAPCHECK;
  bool AbsorberActive = Enable::ABSORBER || Enable::TPC_ABSORBER;

  PHG4TpcSubsystem* tpc = new PHG4TpcSubsystem("TPC");
  tpc->SetActive();
  tpc->SuperDetector("TPC");
  tpc->set_double_param("steplimits", 1);  // 1cm steps
  
  if (AbsorberActive)
    {
      tpc->SetAbsorberActive();
    }
  tpc->OverlapCheck(OverlapCheck);
  
  g4Reco->registerSubsystem(tpc);
  
  if (Enable::TPC_ENDCAP)
    {
      TPC_Endcaps(g4Reco);
    }
  
  radius = G4TPC::tpc_outer_radius;
  
  radius += no_overlapp;
  
  return radius;
}

void TPC_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  auto se = Fun4AllServer::instance();

  // central membrane G4Hit generation
  if( G4TPC::ENABLE_CENTRAL_MEMBRANE_HITS )
  {
    auto centralMembrane = new PHG4TpcCentralMembrane;
    centralMembrane->setCentralMembraneDelay(0);
    centralMembrane->setCentralMembraneEventModulo(1);
    se->registerSubsystem(centralMembrane);
  }

  // direct laser G4Hit generation
  if( G4TPC::ENABLE_DIRECT_LASER_HITS )
  {
    auto directLaser = new PHG4TpcDirectLaser;

    // setup phi and theta steps
    /* use 5deg steps */
    static constexpr double deg_to_rad = M_PI/180.;
    directLaser->SetPhiStepping( 72, 0*deg_to_rad, 360*deg_to_rad );
    directLaser->SetThetaStepping( 17, 5*deg_to_rad, 90*deg_to_rad );
    directLaser->SetDirectLaserAuto( true );
    directLaser->set_double_param("drift_velocity", G4TPC::tpc_drift_velocity_sim);
    se->registerSubsystem(directLaser);
  }

  //=========================
  // setup Tpc readout for filling cells
  // g4tpc/PHG4TpcElectronDrift uses
  // g4tpc/PHG4TpcPadPlaneReadout
  //=========================

  auto padplane = new PHG4TpcPadPlaneReadout;
  padplane->Verbosity(verbosity);

  auto edrift = new PHG4TpcElectronDrift;
  edrift->Detector("TPC");
  edrift->Verbosity(verbosity);
  if( G4TPC::ENABLE_STATIC_DISTORTIONS || G4TPC::ENABLE_TIME_ORDERED_DISTORTIONS )
  {
    auto distortionMap = new PHG4TpcDistortion;
    distortionMap->set_do_static_distortions( G4TPC::ENABLE_STATIC_DISTORTIONS );
    distortionMap->set_static_distortion_filename( G4TPC::static_distortion_filename );

    distortionMap->set_do_time_ordered_distortions( G4TPC::ENABLE_TIME_ORDERED_DISTORTIONS );
    distortionMap->set_time_ordered_distortion_filename( G4TPC::time_ordered_distortion_filename );

    distortionMap->Init();
    edrift->setTpcDistortion( distortionMap );
  }

  double tpc_readout_time = 105.5/ G4TPC::tpc_drift_velocity_sim;  // ns
  double extended_readout_time = 0.0;
  if(TRACKING::pp_mode) extended_readout_time = TRACKING::pp_extended_readout_time;
  edrift->set_double_param("max_time", tpc_readout_time + extended_readout_time);
  std::cout << "PHG4TpcElectronDrift readout window is from 0 to " <<  tpc_readout_time + extended_readout_time << std::endl;

  // override the default drift velocity parameter specification
  edrift->set_double_param("drift_velocity", G4TPC::tpc_drift_velocity_sim);
  padplane->SetDriftVelocity(G4TPC::tpc_drift_velocity_sim);

  // fudge factors to get drphi 150 microns (in mid and outer Tpc) and dz 500 microns cluster resolution
  // They represent effects not due to ideal gas properties and ideal readout plane behavior
  // defaults are 0.085 and 0.105, they can be changed here to get a different resolution
  edrift->registerPadPlane(padplane);
  se->registerSubsystem(edrift);

  // The pad plane readout default is set in PHG4TpcPadPlaneReadout

  // We may want to change the number of inner layers, and can do that here
  padplane->set_int_param("tpc_minlayer_inner", G4MVTX::n_maps_layer + G4INTT::n_intt_layer);  // sPHENIX layer number of first Tpc readout layer
  padplane->set_int_param("ntpc_layers_inner", G4TPC::n_tpc_layer_inner);
  padplane->set_int_param("ntpc_phibins_inner", G4TPC::tpc_layer_rphi_count_inner);

  // Tpc digitizer
  //=========
  PHG4TpcDigitizer* digitpc = new PHG4TpcDigitizer();
  digitpc->SetTpcMinLayer(G4MVTX::n_maps_layer + G4INTT::n_intt_layer);
  double ENC = 670.0;  // standard
  digitpc->SetENC(ENC);
  double ADC_threshold = 4.0 * ENC;
  digitpc->SetADCThreshold(ADC_threshold);  // 4 * ENC seems OK
  digitpc->Verbosity(verbosity);
  cout << " Tpc digitizer: Setting ENC to " << ENC << " ADC threshold to " << ADC_threshold
       << " maps+Intt layers set to " << G4MVTX::n_maps_layer + G4INTT::n_intt_layer << endl;
  digitpc ->set_skip_noise_flag(false);
  se->registerSubsystem(digitpc);

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
    auto centralMembraneClusterizer = new PHTpcCentralMembraneClusterizer;
    centralMembraneClusterizer->Verbosity(verbosity);
    centralMembraneClusterizer->set_histos_on(true);
    centralMembraneClusterizer->set_modulo_threshold(5);
    centralMembraneClusterizer->set_metaCluster_threshold(18);
    se->registerSubsystem(centralMembraneClusterizer);
    

    // match central membrane clusters to pads and generate distortion correction
    auto centralMembraneMatcher = new PHTpcCentralMembraneMatcher;
    centralMembraneMatcher->setSavehistograms( true );
    centralMembraneMatcher->Verbosity( verbosity );
    centralMembraneMatcher->setNMatchIter(2);
    se->registerSubsystem(centralMembraneMatcher);
  }
}


void TPC_QA()
{
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::TPC_VERBOSITY);

  Fun4AllServer* se = Fun4AllServer::instance();
  QAG4SimulationTpc * qa =  new QAG4SimulationTpc;
  qa->Verbosity(verbosity);
  qa->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(qa);
}


#endif
