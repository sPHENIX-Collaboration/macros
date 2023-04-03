#ifndef MACRO_TRKRRECO_C
#define MACRO_TRKRRECO_C

#include <GlobalVariables.C>

#include <G4_TrkrVariables.C>
//#include <G4_ActsGeom.C>


#include <g4eval/SvtxTruthRecoTableEval.h>
#include <g4eval/TrackSeedTrackMapConverter.h>

#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHActsTrackProjection.h>
#include <trackreco/PHActsTrkFitter.h>
#include <trackreco/PHActsVertexPropagator.h>
#include <trackreco/PHCASeeding.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHSiliconSeedMerger.h>
#include <trackreco/PHSiliconTpcTrackMatching.h>
#include <trackreco/PHSimpleKFProp.h>
#include <trackreco/PHSimpleVertexFinder.h>
#include <trackreco/PHTpcDeltaZCorrection.h>
#include <trackreco/PHTrackCleaner.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/SecondaryVertexFinder.h>

#include <tpc/TpcLoadDistortionCorrection.h>

#include <tpccalib/PHTpcResiduals.h>

#include <trackermillepedealignment/MakeMilleFiles.h>
#include <trackermillepedealignment/HelicalFitter.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtrackeralign.so)

void convert_seeds()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  TrackSeedTrackMapConverter *converter = new TrackSeedTrackMapConverter();
  // Default set to full SvtxTrackSeeds. Can be set to 
  // SiliconTrackSeedContainer or TpcTrackSeedContainer 
  converter->setTrackSeedName("SvtxTrackSeedContainer");
  converter->Verbosity(verbosity);
  se->registerSubsystem(converter);
}

void Tracking_Reco_TrackSeed()
{
  // set up verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  
  // get fun4all server instance
  auto se = Fun4AllServer::instance();

  // Assemble silicon clusters into track stubs 
  
  auto silicon_Seeding = new PHActsSiliconSeeding;
  silicon_Seeding->Verbosity(verbosity);
  std::cout << "SETTING SI SEED CV" << std::endl;
  silicon_Seeding->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(silicon_Seeding);

  std::cout << " cluster version: " << G4TRACKING::cluster_version << std::endl; 

  auto merger = new PHSiliconSeedMerger;
  merger->Verbosity(verbosity);
  se->registerSubsystem(merger);
  
  
  auto seeder = new PHCASeeding("PHCASeeding");
  seeder->set_field_dir(G4MAGNET::magfield_rescale);  // to get charge sign right
  if (G4MAGNET::magfield.find("3d") != std::string::npos)
    {
      seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
    }
  seeder->Verbosity(verbosity);
  seeder->SetLayerRange(7, 55);
  seeder->SetSearchWindow(1.5, 0.05);  // (z width, phi width)
  seeder->SetMinHitsPerCluster(0);
  seeder->SetMinClustersPerTrack(3);
  seeder->useConstBField(false);
  seeder->useFixedClusterError(true);
  se->registerSubsystem(seeder);
  
  // expand stubs in the TPC using simple kalman filter
  auto cprop = new PHSimpleKFProp("PHSimpleKFProp");
  cprop->set_field_dir(G4MAGNET::magfield_rescale);
  if (G4MAGNET::magfield.find("3d") != std::string::npos)
    {
      cprop->set_field_dir(-1 * G4MAGNET::magfield_rescale);
    }
  cprop->useConstBField(false);
  cprop->useFixedClusterError(true);
  cprop->set_max_window(5.);
  cprop->set_cluster_version(G4TRACKING::cluster_version);
  cprop->Verbosity(verbosity);
  se->registerSubsystem(cprop);
  
  std::cout << "Tracking_Reco_TrackSeed - Using stub matching for Si matching " << std::endl;
  // The normal silicon association methods
  // Match the TPC track stubs from the CA seeder to silicon track stubs from PHSiliconTruthTrackSeeding
  auto silicon_match = new PHSiliconTpcTrackMatching;
  silicon_match->Verbosity(verbosity);
  silicon_match->set_pp_mode(TRACKING::pp_mode);
  std::cout << "PHSiliconTpcTrackMatching pp_mode set to " << TRACKING::pp_mode << std::endl;
  if (G4TRACKING::SC_CALIBMODE)
    {
      // search windows for initial matching with distortions
      // tuned values are 0.04 and 0.008 in distorted events
      silicon_match->set_phi_search_window(0.04);
      silicon_match->set_eta_search_window(0.008);
    } else {
    // after distortion corrections and rerunning clustering, default tuned values are 0.02 and 0.004 in low occupancy events
    silicon_match->set_phi_search_window(0.03);
    silicon_match->set_eta_search_window(0.005);
  }
  silicon_match->set_test_windows_printout(false);  // used for tuning search windows
  se->registerSubsystem(silicon_match);
 
  // Associate Micromegas clusters with the tracks
  if( Enable::MICROMEGAS ){
    std::cout << "Tracking_Reco_TrackSeed - Using Micromegas matching " << std::endl;
    
    // Match TPC track stubs from CA seeder to clusters in the micromegas layers
    auto mm_match = new PHMicromegasTpcTrackMatching;
    mm_match->Verbosity(verbosity);
    if (G4TRACKING::SC_CALIBMODE)
      {
	// configuration is potentially with different search windows
	mm_match->set_rphi_search_window_lyr1(0.2);
	mm_match->set_rphi_search_window_lyr2(13.0);
	mm_match->set_z_search_window_lyr1(26.0);
	mm_match->set_z_search_window_lyr2(0.2);
      } else {
      // baseline configuration is (0.2, 13.0, 26, 0.2) and is the default
      mm_match->set_rphi_search_window_lyr1(0.2);
      mm_match->set_rphi_search_window_lyr2(13.0);
      mm_match->set_z_search_window_lyr1(26.0);
      mm_match->set_z_search_window_lyr2(0.2);
    }
    mm_match->set_min_tpc_layer(38);             // layer in TPC to start projection fit
    mm_match->set_test_windows_printout(false);  // used for tuning search windows only
    se->registerSubsystem(mm_match);
  }

   /*
   * all done
   * at this stage tracks are fully assembled. They contain clusters spaning Silicon detectors, TPC and Micromegas
   * they are ready to be fit.
   */ 
  if(G4TRACKING::convert_seeds_to_svtxtracks)
    {
      convert_seeds();
    }
}


void vertexing()
{
  Fun4AllServer* se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  auto vtxfinder = new PHSimpleVertexFinder;
  vtxfinder->Verbosity(verbosity);
  se->registerSubsystem(vtxfinder);
  
}

void Tracking_Reco_TrackFit()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  auto se = Fun4AllServer::instance();

  // correct clusters for particle propagation in TPC
  auto deltazcorr = new PHTpcDeltaZCorrection;
  deltazcorr->Verbosity(verbosity);
  se->registerSubsystem(deltazcorr);
  

  // perform final track fit with ACTS
  auto actsFit = new PHActsTrkFitter;
  actsFit->Verbosity(verbosity);
  //actsFit->commissioning(G4TRACKING::use_alignment);
  actsFit->set_cluster_version(G4TRACKING::cluster_version);
  // in calibration mode, fit only Silicons and Micromegas hits
  actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
  actsFit->set_pp_mode(TRACKING::pp_mode);
  se->registerSubsystem(actsFit);
  
  if (G4TRACKING::SC_CALIBMODE)
  {
    /*
    * in calibration mode, calculate residuals between TPC and fitted tracks, 
    * store in dedicated structure for distortion correction
    */
    auto residuals = new PHTpcResiduals;
    residuals->setOutputfile(G4TRACKING::SC_ROOTOUTPUT_FILENAME);
    residuals->setSavehistograms( G4TRACKING::SC_SAVEHISTOGRAMS );
    residuals->setHistogramOutputfile( G4TRACKING::SC_HISTOGRAMOUTPUT_FILENAME );
    residuals->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
    residuals->Verbosity(verbosity);
    se->registerSubsystem(residuals);
  } else {
    
    /* 
     * in full tracking mode, run track cleaner, vertex finder, 
     * propagete tracks to vertex 
     * propagate tracks to EMCAL
     */ 
    
    if( !G4TRACKING::use_full_truth_track_seeding )
    {
      // Choose the best silicon matched track for each TPC track seed
      /* this breaks in truth_track seeding mode because there is no TpcSeed */
      auto cleaner = new PHTrackCleaner;
      cleaner->Verbosity(verbosity);
      se->registerSubsystem(cleaner);
    }
    
    vertexing();
    
    // Propagate track positions to the vertex position
    auto vtxProp = new PHActsVertexPropagator;
    vtxProp->Verbosity(verbosity);
    se->registerSubsystem(vtxProp);

    // project tracks to EMCAL
    auto projection = new PHActsTrackProjection;
    projection->Verbosity(verbosity);
    se->registerSubsystem(projection);
  }
  
}

void Tracking_Reco_CommissioningTrackSeed()
{
   // set up verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  
  // get fun4all server instance
  auto se = Fun4AllServer::instance();

  auto silicon_Seeding = new PHActsSiliconSeeding;
  silicon_Seeding->Verbosity(verbosity);
  silicon_Seeding->set_cluster_version(G4TRACKING::cluster_version);
  silicon_Seeding->sigmaScattering(50.);
  silicon_Seeding->setRPhiSearchWindow(0.4);
  se->registerSubsystem(silicon_Seeding);

  auto merger = new PHSiliconSeedMerger;
  merger->Verbosity(verbosity);
  se->registerSubsystem(merger);
  
  // Assemble TPC clusters into track stubs 
  auto seeder = new PHCASeeding("PHCASeeding");
  seeder->set_field_dir(G4MAGNET::magfield_rescale);  // to get charge sign right
  if (G4MAGNET::magfield.find("3d") != std::string::npos)
    {
      seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
    }
  seeder->Verbosity(verbosity);
  seeder->SetLayerRange(7, 55);
  seeder->SetSearchWindow(1.5, 0.05);  // (z width, phi width)
  seeder->SetMinHitsPerCluster(0);
  seeder->SetMinClustersPerTrack(3);
  seeder->useConstBField(false);
  seeder->useFixedClusterError(true);
  se->registerSubsystem(seeder);
  
  // expand stubs in the TPC using simple kalman filter
  auto cprop = new PHSimpleKFProp("PHSimpleKFProp");
  cprop->set_field_dir(G4MAGNET::magfield_rescale);
  if (G4MAGNET::magfield.find("3d") != std::string::npos)
    {
      cprop->set_field_dir(-1 * G4MAGNET::magfield_rescale);
    }
  cprop->useConstBField(false);
  cprop->useFixedClusterError(true);
  cprop->set_max_window(5.);
  cprop->set_cluster_version(G4TRACKING::cluster_version);
  cprop->Verbosity(verbosity);
  se->registerSubsystem(cprop);
  
  
  // match silicon track seeds to TPC track seeds
  
  // The normal silicon association methods
  // Match the TPC track stubs from the CA seeder to silicon track stubs from PHSiliconTruthTrackSeeding
  auto silicon_match = new PHSiliconTpcTrackMatching;
  silicon_match->Verbosity(verbosity);
  silicon_match->set_pp_mode(TRACKING::pp_mode);
  
  silicon_match->set_phi_search_window(0.2);
  silicon_match->set_eta_search_window(0.015);
  silicon_match->set_x_search_window(std::numeric_limits<double>::max());  
  silicon_match->set_y_search_window(std::numeric_limits<double>::max());  
  silicon_match->set_z_search_window(std::numeric_limits<double>::max());  
  
  silicon_match->set_test_windows_printout(false);  // used for tuning search windows
  se->registerSubsystem(silicon_match);
  
  
  // Associate Micromegas clusters with the tracks
  if( Enable::MICROMEGAS )
    {
      // Match TPC track stubs from CA seeder to clusters in the micromegas layers
      auto mm_match = new PHMicromegasTpcTrackMatching;
      mm_match->Verbosity(verbosity);
      
      mm_match->set_rphi_search_window_lyr1(0.4);
      mm_match->set_rphi_search_window_lyr2(13.0);
      mm_match->set_z_search_window_lyr1(26.0);
      mm_match->set_z_search_window_lyr2(0.2);
      
      mm_match->set_min_tpc_layer(38);             // layer in TPC to start projection fit
      mm_match->set_test_windows_printout(false);  // used for tuning search windows only
      se->registerSubsystem(mm_match);
      
      
    } 
  if(G4TRACKING::convert_seeds_to_svtxtracks)
    {
      convert_seeds();
    }
}

void alignment(std::string datafilename = "mille_output_data_file", 
	       std::string steeringfilename = "mille_steer")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  auto mille = new MakeMilleFiles;
  mille->Verbosity(verbosity);
  mille->set_datafile_name(datafilename + ".bin");
  mille->set_steeringfile_name(steeringfilename + ".txt");
  mille->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(mille);

  auto helical = new HelicalFitter;
  helical->Verbosity(0);
  helical->set_datafile_name(datafilename + "_helical.bin");
  helical->set_steeringfile_name(steeringfilename + "_helical.txt");
  helical->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(helical);

}
 
void Tracking_Reco()
{
  /*
   * just a wrapper around track seeding and track fitting methods, 
   * to minimize disruption to existing steering macros
   */
  if(G4TRACKING::use_alignment)
    {
      Tracking_Reco_CommissioningTrackSeed();
    }
  else
    {
      Tracking_Reco_TrackSeed();
    }

  if(G4TRACKING::convert_seeds_to_svtxtracks)
    {
      vertexing();
    }
  else
    {
      Tracking_Reco_TrackFit();
    }

  if(G4TRACKING::use_alignment)
    {
      alignment();
    }

}

void  Filter_Conversion_Electrons(std::string ntuple_outfile)
{
  Fun4AllServer* se = Fun4AllServer::instance();
  SecondaryVertexFinder* secvert = new SecondaryVertexFinder;
  secvert->Verbosity(0);
  //  secvert->set_write_electrons_node(true);  // writes copy of filtered electron tracks to node tree
  //  secvert->set_write_ntuple(false);  // writes ntuple for tuning cuts
  secvert->setDecayParticleMass( 0.000511);  // for electrons
  secvert->setOutfileName(ntuple_outfile);
  se->registerSubsystem(secvert);
}


#endif
