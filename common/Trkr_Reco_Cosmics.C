#ifndef MACRO_TRKRRECO_COSMICS_C
#define MACRO_TRKRRECO_COSMICS_C

#include <G4_TrkrVariables.C>

#include <g4eval/SvtxTruthRecoTableEval.h>
#include <g4eval/TrackSeedTrackMapConverter.h>

#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHActsTrackProjection.h>
#include <trackreco/PHActsTrkFitter.h>
#include <trackreco/PHActsVertexPropagator.h>
#include <trackreco/PHCASeeding.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHSiliconHelicalPropagator.h>
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

#include <trackermillepedealignment/HelicalFitter.h>
#include <trackermillepedealignment/MakeMilleFiles.h>

#include <fun4all/Fun4AllServer.h>

#include <string>

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
  converter->setFieldMap(G4MAGNET::magfield);
  se->registerSubsystem(converter);
}

void Tracking_Reco_TrackSeed()
{
  // set up verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  // get fun4all server instance
  auto se = Fun4AllServer::instance();

  auto seeder = new PHCASeeding("PHCASeeding");
  seeder->set_field_dir(G4MAGNET::magfield_rescale);  // to get charge sign right
  if (G4MAGNET::magfield.find("3d") != std::string::npos)
  {
    seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
  }
  if (G4MAGNET::magfield.find(".root") == std::string::npos)
  {
    //! constant field
    seeder->useConstBField(true);
    seeder->constBField(std::stod(G4MAGNET::magfield));
  }
  seeder->Verbosity(verbosity);
  seeder->SetLayerRange(7, 55);
  seeder->SetSearchWindow(50., 1.);  // (z width, phi width)
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

  if (G4MAGNET::magfield.find(".root") == std::string::npos)
  {
    //! constant field
    cprop->useConstBField(true);
    cprop->setConstBField(std::stod(G4MAGNET::magfield));
  }
  cprop->useFixedClusterError(true);
  cprop->set_max_window(5.);
  cprop->Verbosity(verbosity);
  se->registerSubsystem(cprop);

  PHSiliconHelicalPropagator *hprop = new PHSiliconHelicalPropagator("HelicalPropagator");
  hprop->Verbosity(verbosity);
  se->registerSubsystem(hprop);

  // Associate Micromegas clusters with the tracks
  if (Enable::MICROMEGAS)
  {
    // Match TPC track stubs from CA seeder to clusters in the micromegas layers
    auto mm_match = new PHMicromegasTpcTrackMatching;
    mm_match->Verbosity(verbosity);

    // baseline configuration is (0.2, 13.0, 26, 0.2) and is the default
    mm_match->set_rphi_search_window_lyr1(0.2);
    mm_match->set_rphi_search_window_lyr2(13.0);
    mm_match->set_z_search_window_lyr1(26.0);
    mm_match->set_z_search_window_lyr2(0.2);
    mm_match->set_min_tpc_layer(38);             // layer in TPC to start projection fit
    mm_match->set_test_windows_printout(false);  // used for tuning search windows only
    se->registerSubsystem(mm_match);
  }

  convert_seeds();
}

void vertexing()
{
  Fun4AllServer *se = Fun4AllServer::instance();
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
  // actsFit->commissioning(G4TRACKING::use_alignment);
  //  in calibration mode, fit only Silicons and Micromegas hits
  actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
  actsFit->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
  actsFit->set_pp_mode(TRACKING::pp_mode);
  actsFit->useActsEvaluator(false);
  actsFit->useOutlierFinder(false);
  actsFit->setFieldMap(G4MAGNET::magfield);
  se->registerSubsystem(actsFit);

  if (G4TRACKING::SC_CALIBMODE)
  {
    /*
     * in calibration mode, calculate residuals between TPC and fitted tracks,
     * store in dedicated structure for distortion correction
     */
    auto residuals = new PHTpcResiduals;
    residuals->setOutputfile(G4TRACKING::SC_ROOTOUTPUT_FILENAME);
    residuals->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
    residuals->Verbosity(verbosity);
    se->registerSubsystem(residuals);
  }
  else
  {
    /*
     * in full tracking mode, run track cleaner, vertex finder,
     * propagete tracks to vertex
     * propagate tracks to EMCAL
     */

    if (!G4TRACKING::use_full_truth_track_seeding)
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

void alignment(std::string datafilename = "mille_output_data_file",
               std::string steeringfilename = "mille_steer")
{
  Fun4AllServer *se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  auto mille = new MakeMilleFiles;
  mille->Verbosity(verbosity);
  mille->set_datafile_name(datafilename + ".bin");
  mille->set_steeringfile_name(steeringfilename + ".txt");
  // mille->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(mille);

  auto helical = new HelicalFitter;
  helical->Verbosity(verbosity);
  helical->set_datafile_name(datafilename + "_helical.bin");
  helical->set_steeringfile_name(steeringfilename + "_helical.txt");
  // helical->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(helical);
}

void Tracking_Reco()
{
  Tracking_Reco_TrackSeed();

  vertexing();

  if (G4TRACKING::use_alignment)
  {
    alignment();
  }
}

#endif
