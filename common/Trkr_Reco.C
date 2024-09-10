#ifndef MACRO_TRKRRECO_C
#define MACRO_TRKRRECO_C

#include <G4_TrkrVariables.C>

#include <trackingdiagnostics/TrackSeedTrackMapConverter.h>

#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHActsTrackProjection.h>
#include <trackreco/PHActsTrkFitter.h>
#include <trackreco/PHActsVertexPropagator.h>
#include <trackreco/PHCASeeding.h>
#include <trackreco/PHGenFitTrkFitter.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHSiliconHelicalPropagator.h>
#include <trackreco/PHSiliconSeedMerger.h>
#include <trackreco/PHSiliconTpcTrackMatching.h>
#include <trackreco/PHSimpleKFProp.h>
#include <trackreco/PHSimpleVertexFinder.h>
#include <trackreco/PHTpcDeltaZCorrection.h>
#include <trackreco/PHTrackCleaner.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PrelimDistortionCorrection.h>
#include <trackreco/SecondaryVertexFinder.h>
#include <trackreco/TrackingIterationCounter.h>

#include <tpc/TpcLoadDistortionCorrection.h>

#include <tpccalib/PHTpcResiduals.h>
#include <tpccalib/TpcSpaceChargeReconstruction.h>

#include <trackermillepedealignment/HelicalFitter.h>
#include <trackermillepedealignment/MakeMilleFiles.h>

#include <fun4all/Fun4AllServer.h>

#include <trackingdiagnostics/TrackContainerCombiner.h>

#include <string>

R__LOAD_LIBRARY(libTrackingDiagnostics.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtrackeralign.so)

void convert_seeds()
{
  auto se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  auto converter = new TrackSeedTrackMapConverter;
  // Default set to full SvtxTrackSeeds. Can be set to
  // SiliconTrackSeedContainer or TpcTrackSeedContainer
  converter->setTrackSeedName("SvtxTrackSeedContainer");
  converter->setFieldMap(G4MAGNET::magfield_tracking);
  converter->Verbosity(verbosity);
  se->registerSubsystem(converter);
}
void Tracking_Reco_TrackSeed_ZeroField()
{
  // set up verbosity
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  // get fun4all server instance
  auto se = Fun4AllServer::instance();

  auto seeder = new PHCASeeding("PHCASeeding");
  double fieldstrength = std::numeric_limits<double>::quiet_NaN();  // set by isConstantField if constant
  bool ConstField = isConstantField(G4MAGNET::magfield_tracking, fieldstrength);
  if (ConstField)
  {
    seeder->useConstBField(true);
    seeder->constBField(fieldstrength);
  }
  else
  {
    seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
    seeder->useConstBField(false);
    seeder->magFieldFile(G4MAGNET::magfield_tracking);  // to get charge sign right
  }
  seeder->Verbosity(verbosity);
  seeder->SetLayerRange(7, 55);
  seeder->SetSearchWindow(1.5, 0.05);  // (z width, phi width)
  seeder->SetMinHitsPerCluster(0);
  seeder->SetMinClustersPerTrack(3);
  seeder->useFixedClusterError(true);

  if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
  {
    seeder->setNeonFraction(G4TPC::NeCF4_Ne_frac);
    seeder->setArgonFraction(G4TPC::NeCF4_Ar_frac);
    seeder->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
  {
    seeder->setNeonFraction(G4TPC::ArCF4_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
  {
    seeder->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
  {
    seeder->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
  }
  else
  {
  }

  seeder->set_pp_mode(TRACKING::pp_mode);
  se->registerSubsystem(seeder);

  // expand stubs in the TPC using simple kalman filter
  auto cprop = new PHSimpleKFProp("PHSimpleKFProp");
  cprop->set_field_dir(G4MAGNET::magfield_rescale);
  if (ConstField)
  {
    cprop->useConstBField(true);
    cprop->setConstBField(fieldstrength);
  }
  else
  {
    cprop->magFieldFile(G4MAGNET::magfield_tracking);
    cprop->set_field_dir(-1 * G4MAGNET::magfield_rescale);
  }
  cprop->useFixedClusterError(true);
  cprop->set_max_window(5.);
  cprop->Verbosity(verbosity);

  if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
  {
    cprop->setNeonFraction(G4TPC::NeCF4_Ne_frac);
    cprop->setArgonFraction(G4TPC::NeCF4_Ar_frac);
    cprop->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
  {
    cprop->setNeonFraction(G4TPC::ArCF4_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
  {
    cprop->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
  {
    cprop->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
  }
  else
  {
  }

  cprop->set_pp_mode(TRACKING::pp_mode);
  se->registerSubsystem(cprop);

  PHSiliconHelicalPropagator* hprop = new PHSiliconHelicalPropagator("PHSiliconHelicalPropagator");
  hprop->dca_xy_cut(0.3);
  hprop->dca_z_cut(1.);
  hprop->Verbosity(verbosity);
  hprop->zeroField();
  se->registerSubsystem(hprop);

  auto mm_match = new PHMicromegasTpcTrackMatching;
  mm_match->Verbosity(verbosity);

  // baseline configuration is (0.2, 13.0, 26, 0.2) and is the default
  mm_match->set_rphi_search_window_lyr1(0.4);
  mm_match->set_rphi_search_window_lyr2(13.0);
  mm_match->set_z_search_window_lyr1(26.0);
  mm_match->set_z_search_window_lyr2(0.4);

  mm_match->set_min_tpc_layer(38);            // layer in TPC to start projection fit
  mm_match->set_test_windows_printout(true);  // used for tuning search windows only
  se->registerSubsystem(mm_match);
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
  se->registerSubsystem(silicon_Seeding);

  auto merger = new PHSiliconSeedMerger;
  merger->Verbosity(verbosity);
  se->registerSubsystem(merger);

  auto seeder = new PHCASeeding("PHCASeeding");
  double fieldstrength = std::numeric_limits<double>::quiet_NaN();  // set by isConstantField if constant
  bool ConstField = isConstantField(G4MAGNET::magfield_tracking, fieldstrength);
  if (ConstField)
  {
    seeder->useConstBField(true);
    seeder->constBField(fieldstrength);
  }
  else
  {
    seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
    seeder->useConstBField(false);
    seeder->magFieldFile(G4MAGNET::magfield_tracking);  // to get charge sign right
  }
  seeder->Verbosity(verbosity);
  seeder->SetLayerRange(7, 55);
  seeder->SetSearchWindow(1.5, 0.05);  // (z width, phi width)
  seeder->SetMinHitsPerCluster(0);
  seeder->SetMinClustersPerTrack(3);
  seeder->useFixedClusterError(true);

  if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
  {
    seeder->setNeonFraction(G4TPC::NeCF4_Ne_frac);
    seeder->setArgonFraction(G4TPC::NeCF4_Ar_frac);
    seeder->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
  {
    seeder->setNeonFraction(G4TPC::ArCF4_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
  {
    seeder->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
  {
    seeder->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
  }
  else
  {
  }

  seeder->set_pp_mode(TRACKING::pp_mode);
  se->registerSubsystem(seeder);

  // expand stubs in the TPC using simple kalman filter
  auto cprop = new PHSimpleKFProp("PHSimpleKFProp");
  cprop->set_field_dir(G4MAGNET::magfield_rescale);
  if (ConstField)
  {
    cprop->useConstBField(true);
    cprop->setConstBField(fieldstrength);
  }
  else
  {
    cprop->magFieldFile(G4MAGNET::magfield_tracking);
    cprop->set_field_dir(-1 * G4MAGNET::magfield_rescale);
  }
  cprop->useFixedClusterError(true);
  cprop->set_max_window(5.);
  cprop->Verbosity(verbosity);

  if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
  {
    cprop->setNeonFraction(G4TPC::NeCF4_Ne_frac);
    cprop->setArgonFraction(G4TPC::NeCF4_Ar_frac);
    cprop->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
  {
    cprop->setNeonFraction(G4TPC::ArCF4_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
  {
    cprop->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
  {
    cprop->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
  }
  else
  {
  }

  cprop->set_pp_mode(TRACKING::pp_mode);
  se->registerSubsystem(cprop);

  if (TRACKING::pp_mode)
  {
    // for pp mode, apply preliminary distortion corrections to TPC clusters before crossing is known
    // and refit the trackseeds. Replace KFProp fits with the new fit parameters in the TPC seeds.
    auto prelim_distcorr = new PrelimDistortionCorrection;
    prelim_distcorr->set_pp_mode(TRACKING::pp_mode);
    prelim_distcorr->Verbosity(verbosity);

    if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
    {
      prelim_distcorr->setNeonFraction(G4TPC::NeCF4_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::NeCF4_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
    }
    else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
    {
      prelim_distcorr->setNeonFraction(G4TPC::ArCF4_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::ArCF4_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
    }
    else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
    {
      prelim_distcorr->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
    }
    else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
    {
      prelim_distcorr->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
    }
    else
    {
    }

    se->registerSubsystem(prelim_distcorr);
  }

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
  }
  else
  {
    // after distortion corrections and rerunning clustering, default tuned values are 0.02 and 0.004 in low occupancy events
    silicon_match->set_phi_search_window(0.03);
    silicon_match->set_eta_search_window(0.005);
  }
  silicon_match->set_test_windows_printout(false);  // used for tuning search windows
  se->registerSubsystem(silicon_match);

  // Associate Micromegas clusters with the tracks
  if (Enable::MICROMEGAS)
  {
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
    }
    else
    {
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
}

void Tracking_Reco_TrackSeed_pass1()
{
  Fun4AllServer* se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  TrackingIterationCounter* counter = new TrackingIterationCounter("TrkrIter1");
  /// Clusters already used are in the 0th iteration
  counter->iteration(0);
  se->registerSubsystem(counter);

  PHActsSiliconSeeding* silseed = new PHActsSiliconSeeding("PHActsSiliconSeedingIt1");
  silseed->Verbosity(verbosity);
  silseed->searchInIntt();
  silseed->iteration(1);
  silseed->set_track_map_name("SiliconTrackSeedContainerIt1");
  se->registerSubsystem(silseed);

  PHSiliconSeedMerger* merger = new PHSiliconSeedMerger("SiliconSeedMargerIt1");
  merger->Verbosity(verbosity);
  merger->clusterOverlap(2);
  merger->searchIntt();
  merger->trackMapName("SiliconTrackSeedContainerIt1");
  se->registerSubsystem(merger);

  TrackContainerCombiner* combiner = new TrackContainerCombiner;
  combiner->newContainerName("SiliconTrackSeedContainer");
  combiner->oldContainerName("SiliconTrackSeedContainerIt1");
  combiner->Verbosity(verbosity);
  se->registerSubsystem(combiner);
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

  if (G4TRACKING::use_genfit_track_fitter)
  {
    // perform final track fit with GENFIT
    auto genfitFit = new PHGenFitTrkFitter;
    genfitFit->Verbosity(verbosity);
    genfitFit->set_fit_silicon_mms(G4TRACKING::SC_CALIBMODE);
    se->registerSubsystem(genfitFit);

    if (G4TRACKING::SC_CALIBMODE)
    {
      // Genfit based Tpc space charge Reconstruction
      auto tpcSpaceChargeReconstruction = new TpcSpaceChargeReconstruction;
      tpcSpaceChargeReconstruction->set_use_micromegas(G4TRACKING::SC_USE_MICROMEGAS);
      tpcSpaceChargeReconstruction->set_outputfile(G4TRACKING::SC_ROOTOUTPUT_FILENAME);
      // reconstructed distortion grid size (phi, r, z)
      tpcSpaceChargeReconstruction->set_grid_dimensions(36, 48, 80);
      se->registerSubsystem(tpcSpaceChargeReconstruction);
    }
  }
  else
  {
    // perform final track fit with ACTS
    auto actsFit = new PHActsTrkFitter;
    actsFit->Verbosity(verbosity);
    actsFit->commissioning(G4TRACKING::use_alignment);
    // in calibration mode, fit only Silicons and Micromegas hits
    actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
    actsFit->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
    actsFit->set_pp_mode(TRACKING::pp_mode);
    actsFit->set_use_clustermover(true);  // default is true for now
    actsFit->useActsEvaluator(false);
    actsFit->useOutlierFinder(false);
    actsFit->setFieldMap(G4MAGNET::magfield_tracking);
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
      // reconstructed distortion grid size (phi, r, z)
      residuals->setGridDimensions(36, 48, 80);
      residuals->Verbosity(verbosity);
      se->registerSubsystem(residuals);
    }
  }

  if (!G4TRACKING::SC_CALIBMODE)
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
      // cleaner->set_quality_cut(30.0);
      se->registerSubsystem(cleaner);
    }

    vertexing();

    if (!G4TRACKING::use_genfit_track_fitter)
    {
      // Propagate track positions to the vertex position
      auto vtxProp = new PHActsVertexPropagator;
      vtxProp->Verbosity(verbosity);
      vtxProp->fieldMap(G4MAGNET::magfield_tracking);
      se->registerSubsystem(vtxProp);

      // project tracks to EMCAL
      auto projection = new PHActsTrackProjection;
      projection->Verbosity(verbosity);
      double fieldstrength = std::numeric_limits<double>::quiet_NaN();
      if (isConstantField(G4MAGNET::magfield_tracking, fieldstrength))
      {
        projection->setConstFieldVal(fieldstrength);
      }
      se->registerSubsystem(projection);
    }
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
  silicon_Seeding->sigmaScattering(50.);
  silicon_Seeding->setinttRPhiSearchWindow(2.);
  silicon_Seeding->helixcut(0.01);
  se->registerSubsystem(silicon_Seeding);

  auto merger = new PHSiliconSeedMerger;
  merger->Verbosity(verbosity);
  se->registerSubsystem(merger);

  // Assemble TPC clusters into track stubs
  auto seeder = new PHCASeeding("PHCASeeding");
  seeder->set_field_dir(G4MAGNET::magfield_rescale);                // to get charge sign right
  double fieldstrength = std::numeric_limits<double>::quiet_NaN();  // set by isConstantField if constant
  bool ConstField = isConstantField(G4MAGNET::magfield_tracking, fieldstrength);
  if (!ConstField)
  {
    seeder->magFieldFile(G4MAGNET::magfield_tracking);
    seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
  }
  seeder->Verbosity(verbosity);
  seeder->SetLayerRange(7, 55);
  seeder->SetSearchWindow(1.5, 0.05);  // (z width, phi width)
  seeder->SetMinHitsPerCluster(0);
  seeder->SetMinClustersPerTrack(3);
  seeder->useConstBField(false);
  seeder->useFixedClusterError(true);

  if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
  {
    seeder->setNeonFraction(G4TPC::NeCF4_Ne_frac);
    seeder->setArgonFraction(G4TPC::NeCF4_Ar_frac);
    seeder->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
  {
    seeder->setNeonFraction(G4TPC::ArCF4_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
  {
    seeder->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
  {
    seeder->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
    seeder->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
    seeder->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
    seeder->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
    seeder->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
  }
  else
  {
  }

  seeder->set_pp_mode(TRACKING::pp_mode);
  se->registerSubsystem(seeder);

  // expand stubs in the TPC using simple kalman filter
  auto cprop = new PHSimpleKFProp("PHSimpleKFProp");
  cprop->set_field_dir(G4MAGNET::magfield_rescale);
  if (!ConstField)
  {
    cprop->set_field_dir(-1 * G4MAGNET::magfield_rescale);
  }
  cprop->useConstBField(false);
  cprop->useFixedClusterError(true);
  cprop->set_max_window(5.);
  cprop->Verbosity(verbosity);

  if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
  {
    cprop->setNeonFraction(G4TPC::NeCF4_Ne_frac);
    cprop->setArgonFraction(G4TPC::NeCF4_Ar_frac);
    cprop->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
  {
    cprop->setNeonFraction(G4TPC::ArCF4_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
  {
    cprop->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
  }
  else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
  {
    cprop->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
    cprop->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
    cprop->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
    cprop->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
    cprop->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
  }
  else
  {
  }

  cprop->set_pp_mode(TRACKING::pp_mode);
  se->registerSubsystem(cprop);

  if (TRACKING::pp_mode)
  {
    // for pp mode, apply preliminary distortion corrections to TPC clusters before crossing is known
    // and refit the trackseeds. Replace KFProp fits with the new fit parameters in the TPC seeds.
    auto prelim_distcorr = new PrelimDistortionCorrection;
    prelim_distcorr->set_pp_mode(TRACKING::pp_mode);
    prelim_distcorr->Verbosity(verbosity);

    if (G4TPC::TPC_GAS_MIXTURE == "NeCF4")
    {
      prelim_distcorr->setNeonFraction(G4TPC::NeCF4_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::NeCF4_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::NeCF4_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::NeCF4_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::NeCF4_isobutane_frac);
    }
    else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4")
    {
      prelim_distcorr->setNeonFraction(G4TPC::ArCF4_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::ArCF4_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::ArCF4_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::ArCF4_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::ArCF4_isobutane_frac);
    }
    else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4N2")
    {
      prelim_distcorr->setNeonFraction(G4TPC::ArCF4N2_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::ArCF4N2_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::ArCF4N2_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::ArCF4N2_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::ArCF4N2_isobutane_frac);
    }
    else if (G4TPC::TPC_GAS_MIXTURE == "ArCF4Isobutane")
    {
      prelim_distcorr->setNeonFraction(G4TPC::ArCF4Isobutane_Ne_frac);
      prelim_distcorr->setArgonFraction(G4TPC::ArCF4Isobutane_Ar_frac);
      prelim_distcorr->setCF4Fraction(G4TPC::ArCF4Isobutane_CF4_frac);
      prelim_distcorr->setNitrogenFraction(G4TPC::ArCF4Isobutane_N2_frac);
      prelim_distcorr->setIsobutaneFraction(G4TPC::ArCF4Isobutane_isobutane_frac);
    }
    else
    {
    }

    se->registerSubsystem(prelim_distcorr);
  }

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
  if (Enable::MICROMEGAS)
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
}

void alignment(std::string datafilename = "mille_output_data_file",
               std::string steeringfilename = "mille_steer")
{
  Fun4AllServer* se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  auto mille = new MakeMilleFiles;
  mille->Verbosity(verbosity);
  mille->set_datafile_name(datafilename + ".bin");
  mille->set_steeringfile_name(steeringfilename + ".txt");
  se->registerSubsystem(mille);

  auto helical = new HelicalFitter;
  helical->Verbosity(verbosity);
  helical->set_datafile_name(datafilename + "_helical.bin");
  helical->set_steeringfile_name(steeringfilename + "_helical.txt");
  se->registerSubsystem(helical);
}

void Tracking_Reco()
{
  /*
   * just a wrapper around track seeding and track fitting methods,
   * to minimize disruption to existing steering macros
   */
  if (G4TRACKING::use_alignment)
  {
    Tracking_Reco_CommissioningTrackSeed();
  }
  else
  {
    Tracking_Reco_TrackSeed();
  }

  if (G4TRACKING::convert_seeds_to_svtxtracks)
  {
    convert_seeds();
    vertexing();
  }
  else
  {
    Tracking_Reco_TrackFit();
  }

  if (G4TRACKING::iterative_seeding)
  {
    Tracking_Reco_TrackSeed_pass1();

    if (G4TRACKING::convert_seeds_to_svtxtracks)
    {
      convert_seeds();
      vertexing();
    }
  }

  if (G4TRACKING::use_alignment)
  {
    alignment();
  }
}

void Filter_Conversion_Electrons(std::string ntuple_outfile)
{
  Fun4AllServer* se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  SecondaryVertexFinder* secvert = new SecondaryVertexFinder;
  secvert->Verbosity(verbosity);
  //  secvert->set_write_electrons_node(true);  // writes copy of filtered electron tracks to node tree
  //  secvert->set_write_ntuple(false);  // writes ntuple for tuning cuts
  secvert->setDecayParticleMass(0.000511);  // for electrons
  secvert->setOutfileName(ntuple_outfile);
  se->registerSubsystem(secvert);
}

#endif
