#ifndef MACRO_G4TRACKING_C
#define MACRO_G4TRACKING_C

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libqa_modules.so)

#include <GlobalVariables.C>

#include <G4_Intt.C>
#include <G4_Micromegas.C>
#include <G4_Mvtx.C>
#include <G4_TPC.C>
#include <QA.C>

#include <g4eval/SvtxEvaluator.h>

#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHActsTrkFitter.h>
#include <trackreco/PHActsVertexPropagator.h>
#include <trackreco/PHCASeeding.h>
#include <trackreco/PHGenFitTrackProjection.h>
#include <trackreco/PHGhostRejection.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHRaveVertexing.h>
#include <trackreco/PHSiliconTpcTrackMatching.h>
#include <trackreco/PHSimpleKFProp.h>
#include <trackreco/PHSimpleVertexFinder.h>
#include <trackreco/PHTpcClusterMover.h>
#include <trackreco/PHTpcDeltaZCorrection.h>
#include <trackreco/PHTpcTrackSeedCircleFit.h>
#include <trackreco/PHTrackCleaner.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PHTruthSiliconAssociation.h>
#include <trackreco/PHTruthTrackSeeding.h>
#include <trackreco/PHTruthVertexing.h>

#include <tpccalib/PHTpcResiduals.h>

#include <qa_modules/QAG4SimulationTracking.h>
#include <qa_modules/QAG4SimulationUpsilon.h>
#include <qa_modules/QAG4SimulationVertex.h>

#include <fun4all/Fun4AllServer.h>

namespace Enable
{
  bool TRACKING_TRACK = false;
  bool TRACKING_EVAL = false;
  int TRACKING_VERBOSITY = 0;
  bool TRACKING_QA = false;
}  // namespace Enable

namespace G4TRACKING
{
  // Space Charge calibration flag
  bool SC_CALIBMODE = false;                                            // this is anded with G4TPC::ENABLE_DISTORTIONS in TrackingInit()
  bool SC_USE_MICROMEGAS = true;
  bool SC_SAVEHISTOGRAMS = false;
  double SC_COLLISIONRATE = 50e3;                                      // leave at 50 KHz for now, scaling of distortion map not implemented yet
  std::string SC_ROOTOUTPUT_FILENAME = "TpcSpaceChargeMatrices.root";  // space charge calibration output file
  std::string SC_HISTOGRAMOUTPUT_FILENAME = "TpcResiduals.root"; // space charge calibration output file

  // Vertexing
  bool g4eval_use_initial_vertex = true;  // if true, g4eval uses initial vertices in SvtxVertexMap, not final vertices in SvtxVertexMapRefit

  // set to false to disable adding fake surfaces (TPC, Micromegas) to MakeActsGeom
  bool add_fake_surfaces = true;

  // Truth seeding options for diagnostics (can use any or all)
  bool use_truth_silicon_seeding = false;     // if true runs truth silicon seeding instead of acts silicon seeding
  bool use_truth_tpc_seeding = false;         // if true runs truth silicon seeding instead of reco TPC seeding
  bool use_truth_si_matching = false;         // if true, associates silicon clusters using best truth track match to TPC seed tracks - for diagnostics only
                                              // Full truth track seeding
  bool use_full_truth_track_seeding = false;  // makes track seeds using truth info, used for both Acts and Genfit
  bool use_truth_vertexing = false;           // if true runs truth vertexing, if false runs PHSimpleVertexFinder

  // Rave final vertexing (for QA)
  bool use_rave_vertexing = true;  // Use Rave to find and fit for vertex after track fitting - used for QA only
  // This is the setup we have been using  - smeared truth vertex for a single collision per event. Make it the default for now.
  std::string vmethod("avf-smoothing:1");  // only good for 1 vertex events // vmethod is a string used to set the Rave final-vertexing method:

}  // namespace G4TRACKING

void TrackingInit()
{
  if (!Enable::MICROMEGAS)
  {
    G4MICROMEGAS::n_micromegas_layer = 0;
  }

  /// Build the Acts geometry
  Fun4AllServer* se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  /// Geometry must be built before any Acts modules
  MakeActsGeometry* geom = new MakeActsGeometry();
  geom->Verbosity(verbosity);
  geom->setMagField(G4MAGNET::magfield);
  geom->setMagFieldRescale(G4MAGNET::magfield_rescale);
  geom->add_fake_surfaces(G4TRACKING::add_fake_surfaces);
  se->registerSubsystem(geom);
}

void Tracking_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  // Assemble silicon clusters into track stubs - needed for initial vertex finding
  //============================================================
  if (G4TRACKING::use_truth_silicon_seeding)
  {
    // For the silicon, for each truth particle, create a track and associate clusters with it using truth information, write to silicon track map
    // track stubs are given the location of the truth vertex in this module
    PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingSilicon");
    pat_rec->Verbosity(verbosity);
    pat_rec->set_track_map_name("SvtxSiliconTrackMap");
    pat_rec->set_min_layer(0);
    pat_rec->set_max_layer(G4MVTX::n_maps_layer + G4INTT::n_intt_layer);

    se->registerSubsystem(pat_rec);
  }
  else
  {
    PHActsSiliconSeeding* silicon_Seeding = new PHActsSiliconSeeding();
    silicon_Seeding->Verbosity(verbosity);
    silicon_Seeding->fieldMapName(G4MAGNET::magfield);
    se->registerSubsystem(silicon_Seeding);
  }

  //================================================
  // Section 1: Reco chain of track propagation and final fitting
  //================================================
  if (!G4TRACKING::use_full_truth_track_seeding)
  {
    // TPC track seeding (finds all clusters in TPC for tracks)
    //============================================
    if (G4TRACKING::use_truth_tpc_seeding)
    {
      // For the TPC, for each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
      // track stubs are given the position odf the truth vertex in this module
      PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingTpc");
      pat_rec->Verbosity(verbosity);
      pat_rec->set_track_map_name("SvtxTrackMap");
      pat_rec->set_min_layer(G4MVTX::n_maps_layer + G4INTT::n_intt_layer);
      pat_rec->set_max_layer(G4MVTX::n_maps_layer + G4INTT::n_intt_layer + G4TPC::n_gas_layer);

      se->registerSubsystem(pat_rec);
    }
    else
    {
      auto seeder = new PHCASeeding("PHCASeeding");
      seeder->set_field_dir(G4MAGNET::magfield_rescale);  // to get charge sign right
      if (G4MAGNET::magfield.find("3d") != std::string::npos)
      {
        seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
      }
      seeder->Verbosity(verbosity);
      seeder->SetLayerRange(7, 55);
      seeder->SetSearchWindow(0.01, 0.02);  // (eta width, phi width)
      seeder->SetMinHitsPerCluster(0);
      seeder->SetMinClustersPerTrack(3);
      seeder->useConstBField(false);
      seeder->useFixedClusterError(true);
      se->registerSubsystem(seeder);

      PHTpcTrackSeedCircleFit* vtxassoc2 = new PHTpcTrackSeedCircleFit("PrePropagatorPHTpcTrackSeedCircleFit");
      vtxassoc2->Verbosity(verbosity);
      se->registerSubsystem(vtxassoc2);

      PHSimpleKFProp* cprop = new PHSimpleKFProp("PHSimpleKFProp");
      cprop->set_field_dir(G4MAGNET::magfield_rescale);
      if (G4MAGNET::magfield.find("3d") != std::string::npos)
      {
        cprop->set_field_dir(-1 * G4MAGNET::magfield_rescale);
      }
      cprop->useConstBField(false);
      cprop->useFixedClusterError(true);
      cprop->set_max_window(5.);
      cprop->Verbosity(verbosity);
      se->registerSubsystem(cprop);
    }

    // Associate TPC track stubs with silicon and Micromegas
    //=============================================

    /*
     * add cluster mover to apply TPC distortion corrections to clusters belonging to tracks
     * once the correction is applied, the cluster are moved back to TPC surfaces using local track angles
     * moved clusters are stored in a separate map, called CORRECTED_TRKR_CLUSTER
     */
    if( G4TPC::ENABLE_CORRECTIONS ) se->registerSubsystem(new PHTpcClusterMover);

    auto vtxassoc = new PHTpcTrackSeedCircleFit;
    vtxassoc->Verbosity(verbosity);
    se->registerSubsystem(vtxassoc);

    // Choose the best duplicate TPC track seed
    auto ghosts = new PHGhostRejection;
    ghosts->Verbosity(verbosity);
    se->registerSubsystem(ghosts);
      
    // correct for particle propagation in TPC
    se->registerSubsystem(new PHTpcDeltaZCorrection);

    // Silicon cluster matching to TPC track seeds
    if (G4TRACKING::use_truth_si_matching)
    {
      std::cout << "      Using truth Si matching " << std::endl;
      // use truth particle matching in TPC to assign clusters in silicon to TPC tracks from CA seeder
      // intended only for diagnostics
      PHTruthSiliconAssociation* silicon_assoc = new PHTruthSiliconAssociation();
      silicon_assoc->Verbosity(verbosity);
      se->registerSubsystem(silicon_assoc);
    }
    else
    {
      std::cout << "      Using stub matching for Si matching " << std::endl;

      // The normal silicon association methods
      // start with a complete TPC track seed from one of the CA seeders

      // Match the TPC track stubs from the CA seeder to silicon track stubs from PHSiliconTruthTrackSeeding
      PHSiliconTpcTrackMatching* silicon_match = new PHSiliconTpcTrackMatching();
      silicon_match->Verbosity(verbosity);
      silicon_match->set_field(G4MAGNET::magfield);
      silicon_match->set_field_dir(G4MAGNET::magfield_rescale);
      silicon_match->set_sc_calib_mode(G4TRACKING::SC_CALIBMODE);
      if (G4TRACKING::SC_CALIBMODE)
      {
        silicon_match->set_collision_rate(G4TRACKING::SC_COLLISIONRATE);
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
      silicon_match->set_test_windows_printout(false);  // used for tuning search windows only
      se->registerSubsystem(silicon_match);
    }

    // Associate Micromegas clusters with the tracks
    if (G4MICROMEGAS::n_micromegas_layer > 0)
    {
      std::cout << "      Using Micromegas matching " << std::endl;

      // Match TPC track stubs from CA seeder to clusters in the micromegas layers
      PHMicromegasTpcTrackMatching* mm_match = new PHMicromegasTpcTrackMatching();
      mm_match->Verbosity(verbosity);
      mm_match->set_sc_calib_mode(G4TRACKING::SC_CALIBMODE);
      if (G4TRACKING::SC_CALIBMODE)
      {
        // calibration pass with distorted tracks
        mm_match->set_collision_rate(G4TRACKING::SC_COLLISIONRATE);
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

    // Final fitting of tracks using Acts Kalman Filter
    //=====================================

    std::cout << "   Using Acts track fitting " << std::endl;

    PHActsTrkFitter* actsFit = new PHActsTrkFitter("PHActsFirstTrkFitter");
    actsFit->Verbosity(verbosity);
    /// If running with distortions, fit only the silicon+MMs first
    actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
    actsFit->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
    se->registerSubsystem(actsFit);

    if (G4TRACKING::SC_CALIBMODE)
    {
      /// run tpc residual determination with silicon+MM track fit
      auto residuals = new PHTpcResiduals;
      residuals->setOutputfile(G4TRACKING::SC_ROOTOUTPUT_FILENAME);
      residuals->setSavehistograms( G4TRACKING::SC_SAVEHISTOGRAMS );
      residuals->setHistogramOutputfile( G4TRACKING::SC_HISTOGRAMOUTPUT_FILENAME );
      residuals->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
      residuals->Verbosity(verbosity);
      se->registerSubsystem(residuals);
    }

    // Choose the best silicon matched track for each TPC track seed
    PHTrackCleaner* cleaner = new PHTrackCleaner();
    cleaner->Verbosity(verbosity);
    se->registerSubsystem(cleaner);

    if (G4TRACKING::use_truth_vertexing)
    {
      PHTruthVertexing* vtxing = new PHTruthVertexing();
      vtxing->associate_tracks(true);
      std::string trackmapnamef = "SvtxTrackMap";
      vtxing->set_track_map_name(trackmapnamef);
      se->registerSubsystem(vtxing);
    }
    else
    {
      PHSimpleVertexFinder* vtxfinder = new PHSimpleVertexFinder();
      vtxfinder->Verbosity(verbosity);
      se->registerSubsystem(vtxfinder);
    }

    /// Propagate track positions to the vertex position
    PHActsVertexPropagator* vtxProp = new PHActsVertexPropagator();
    vtxProp->Verbosity(verbosity);
    se->registerSubsystem(vtxProp);
  }

  //=========================================================
  // Section 2: Full truth track finding with Acts final fitting
  //=========================================================
  else if (G4TRACKING::use_full_truth_track_seeding)
  {
    std::cout << "  Using full truth track seeding for Acts" << std::endl;

    // For each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
    // track stubs are given the position of the truth vertex in this module, but Genfit does not care
    // Includes clusters for TPC, silicon and MM's
    PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingFull");
    pat_rec->Verbosity(verbosity);
    pat_rec->set_track_map_name("SvtxTrackMap");
    se->registerSubsystem(pat_rec);

    /*
     * add cluster mover to apply TPC distortion corrections to clusters belonging to tracks
     * once the correction is applied, the cluster are moved back to TPC surfaces using local track angles
     * moved clusters are stored in a separate map, called CORRECTED_TRKR_CLUSTER
     */
    if( G4TPC::ENABLE_CORRECTIONS ) se->registerSubsystem(new PHTpcClusterMover);
    
    // correct for particle propagation in TPC
    se->registerSubsystem(new PHTpcDeltaZCorrection);

    // Fitting of tracks using Acts Kalman Filter
    //==================================

    std::cout << "   Using Acts track fitting " << std::endl;

    PHActsTrkFitter* actsFit = new PHActsTrkFitter("PHActsFirstTrkFitter");
    actsFit->Verbosity(verbosity);
    actsFit->doTimeAnalysis(false);
    /// If running with distortions, fit only the silicon+MMs first
    actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
    se->registerSubsystem(actsFit);

    if (G4TRACKING::SC_CALIBMODE)
    {
      /// run tpc residual determination with silicon+MM track fit
      auto residuals = new PHTpcResiduals;
      residuals->setOutputfile( G4TRACKING::SC_ROOTOUTPUT_FILENAME );
      residuals->setSavehistograms( G4TRACKING::SC_SAVEHISTOGRAMS );
      residuals->setHistogramOutputfile( G4TRACKING::SC_HISTOGRAMOUTPUT_FILENAME );
      residuals->setUseMicromegas(G4TRACKING::SC_USE_MICROMEGAS);
      residuals->Verbosity(verbosity);
      se->registerSubsystem(residuals);
    }

    if (G4TRACKING::use_truth_vertexing)
    {
      PHTruthVertexing* vtxing = new PHTruthVertexing();
      vtxing->associate_tracks(true);
      std::string trackmapnamef = "SvtxTrackMap";
      vtxing->set_track_map_name(trackmapnamef);
      se->registerSubsystem(vtxing);
    }
    else
    {
      PHSimpleVertexFinder* vtxfinder = new PHSimpleVertexFinder();
      vtxfinder->Verbosity(verbosity);
      se->registerSubsystem(vtxfinder);
    }

    /// Propagate track positions to the vertex position
    PHActsVertexPropagator* vtxProp = new PHActsVertexPropagator();
    vtxProp->Verbosity(verbosity);
    se->registerSubsystem(vtxProp);
  }

  //==================================
  // Common  to all sections
  //==================================

  // Final vertex finding and fitting with RAVE
  //==================================
  if (G4TRACKING::use_rave_vertexing)
  {
    PHRaveVertexing* rave = new PHRaveVertexing();
    //    rave->set_vertexing_method("kalman-smoothing:1");
    rave->set_over_write_svtxvertexmap(false);
    rave->set_svtxvertexmaprefit_node_name("SvtxVertexMapRave");
    rave->Verbosity(verbosity);
    se->registerSubsystem(rave);
  }

  // Track Projections
  //===============
  PHGenFitTrackProjection* projection = new PHGenFitTrackProjection();
  projection->Verbosity(verbosity);
  se->registerSubsystem(projection);

  return;
}

void Tracking_Eval(const std::string& outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  //----------------
  // Tracking evaluation
  //----------------
  SvtxEvaluator* eval;
  eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile, "SvtxTrackMap",
                           G4MVTX::n_maps_layer,
                           G4INTT::n_intt_layer,
                           G4TPC::n_gas_layer,
                           G4MICROMEGAS::n_micromegas_layer);
  eval->do_cluster_eval(true);
  eval->do_g4hit_eval(true);
  eval->do_hit_eval(true);  // enable to see the hits that includes the chamber physics...
  eval->do_gpoint_eval(true);
  eval->do_vtx_eval_light(true);
  eval->do_eval_light(true);
  eval->set_use_initial_vertex(G4TRACKING::g4eval_use_initial_vertex);
  eval->scan_for_embedded(true);   // take all tracks if false - take only embedded tracks if true
  eval->scan_for_primaries(true);  // defaults to only thrown particles for ntp_gtrack
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  return;
}

void Tracking_QA()
{
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::TRACKING_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  QAG4SimulationTracking* qa = new QAG4SimulationTracking();
  //  qa->addEmbeddingID(2);
  qa->Verbosity(verbosity);
  se->registerSubsystem(qa);

  QAG4SimulationVertex* qa2 = new QAG4SimulationVertex();
  // qa2->addEmbeddingID(2);
  qa2->Verbosity(verbosity);
  se->registerSubsystem(qa2);

  if (G4TRACKING::use_rave_vertexing)
  {
    QAG4SimulationVertex* qav = new QAG4SimulationVertex();
    // qav->addEmbeddingID(2);
    qav->Verbosity(verbosity);
    qav->setVertexMapName("SvtxVertexMapRave");
    se->registerSubsystem(qav);
  }

  //  Acts Kalman Filter vertex finder
  //=================================
  QAG4SimulationVertex* qav = new QAG4SimulationVertex();
  // qav->addEmbeddingID(2);
  qav->Verbosity(verbosity);
  qav->setVertexMapName("SvtxVertexMapActs");
  se->registerSubsystem(qav);

  if (Input::UPSILON)
  {
    QAG4SimulationUpsilon* qa = new QAG4SimulationUpsilon();

    for (int id : Input::UPSILON_EmbedIds)
    {
      qa->addEmbeddingID(id);
    }
    se->registerSubsystem(qa);
  }
}

#endif
