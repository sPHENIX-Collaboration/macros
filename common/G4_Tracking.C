#ifndef MACRO_G4TRACKING_C
#define MACRO_G4TRACKING_C

#include <GlobalVariables.C>
#include <QA.C>

#include <G4_Intt.C>
#include <G4_Micromegas.C>
#include <G4_Mvtx.C>
#include <G4_TPC.C>

#include <fun4all/Fun4AllServer.h>

#include <g4eval/SvtxEvaluator.h>

#include <trackreco/PHCASeeding.h>
#include <trackreco/PHHybridSeeding.h>
#include <trackreco/PHGenFitTrackProjection.h>
#include <trackreco/PHGenFitTrkFitter.h>
#include <trackreco/PHGenFitTrkProp.h>
#include <trackreco/PHHoughSeeding.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHRaveVertexing.h>
#include <trackreco/PHSiliconTpcTrackMatching.h>
#include <trackreco/PHTpcTrackSeedVertexAssoc.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PHTruthSiliconAssociation.h>
#include <trackreco/PHTruthTrackSeeding.h>
#include <trackreco/PHTruthVertexing.h>

#if __cplusplus >= 201703L
#include <trackreco/ActsEvaluator.h>
#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsSourceLinks.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHActsTracks.h>
#include <trackreco/PHActsTrkFitter.h>
#include <trackreco/PHActsInitialVertexFinder.h>
#include <trackreco/PHActsVertexFinder.h>
#include <trackreco/PHTpcResiduals.h>
#endif

#include <trackbase/TrkrHitTruthAssoc.h>

#include <phtpctracker/PHTpcTracker.h>

#include <qa_modules/QAG4SimulationTracking.h>
#include <qa_modules/QAG4SimulationUpsilon.h>
#include <qa_modules/QAG4SimulationVertex.h>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libPHTpcTracker.so)
R__LOAD_LIBRARY(libqa_modules.so)

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
  bool SC_CALIBMODE = true;        // this is anded with G4TPC::ENABLE_DISTORTIONS in TrackingInit()
  double SC_COLLISIONRATE = 50e3;  // leave at 50 KHz for now, scaling of distortion map not implemented yet

  // Tracking reconstruction setup parameters and flags
  //=====================================

  // The normal (default) Acts tracking chain is:
  //   PHActsSiliconSeeding                    // make silicon track seeds
  //   PHActsInitialVertexing                    // event vertex from silicon track stubs
  //   PHCASeeding                                    // TPC track seeds
  //   PHSiliconTpcTrackMatching      // match TPC track seeds to silicon track seeds
  //   PHMicromegasTpcTrackMatching   // associate Micromagas clusters with TPC track stubs
  //   PHActsTracks                                  // convert SvtxTracks to Acts tracks
  //   PHActsTrkFitter                               // Kalman fitter makes final fit to assembled tracks
  //   Final vertexing

  bool g4eval_use_initial_vertex = true;  // if true, g4eval uses initial vertices in SvtxVertexMap, not final vertices in SvtxVertexMapRefit
  bool use_acts_init_vertexing = false;    // if true runs acts initial vertex finder, false runs truth vertexing

  // Possible variations - these are normally false
  bool use_PHTpcTracker_seeding = false;   // false for using the default PHCASeeding to get TPC track seeds, true to use PHTpcTracker
  bool use_hybrid_seeding = false;         // false for using the default PHCASeeding, true to use PHHybridSeeding (STAR core, ALICE KF)
  bool use_truth_silicon_seeding = false;   // if true runs truth silicon seeding instead of acts silicon seeding
  bool use_truth_tpc_seeding = false;   // if true runs truth silicon seeding in stead of rec o TPC seeding
  //bool use_truth_track_seeding = false;    // use true to run with truth track seeding  ***** WORKS FOR GENFIT ONLY
  bool use_truth_si_matching = false;      // if true, associates silicon clusters using best truth track match to TPC seed tracks - for diagnostics only
  bool use_Genfit = false;                 // if false, acts KF is run on proto tracks assembled above, if true, use Genfit track propagation and fitting

  bool use_rave_vertexing = true;          // Use Rave to find and fit for vertex after track fitting
  // This is the setup we have been using  - smeared truth vertex for a single collision per event. Make it the default for now.
  std::string vmethod("avf-smoothing:1");  // only good for 1 vertex events // vmethod is a string used to set the Rave final-vertexing method:

}  // namespace G4TRACKING

void TrackingInit()
{
#if __cplusplus < 201703L
  std::cout << std::endl;
  std::cout << "Cannot run tracking without gcc-8.3 (c++17) environment. Please run:" << std::endl;
  //
  // the following gymnastics is needed to print out the correct shell script to source
  // We have three cvmfs volumes:
  //          /cvmfs/sphenix.sdcc.bnl.gov (BNL internal)
  //          /cvmfs/sphenix.opensciencegrid.org (worldwide readable)
  //          /cvmfs/eic.opensciencegrid.org (Fun4All@EIC)
  // We support tcsh and bash
  //
  std::string current_opt = getenv("OPT_SPHENIX");
  std::string x8664_sl7 = "x8664_sl7";
  std::string gcc83 = "gcc-8.3";
  size_t x8664pos = current_opt.find(x8664_sl7);
  current_opt.replace(x8664pos, x8664_sl7.size(), gcc83);
  std::string setupscript = "sphenix_setup";
  std::string setupscript_ext = ".csh";
  if (current_opt.find("eic") != string::npos)
    setupscript = "eic_setup";
  std::string shell = getenv("SHELL");
  if (shell.find("tcsh") == string::npos)
    setupscript_ext = ".sh";
  std::cout << "source " << current_opt << "/bin/"
            << setupscript << setupscript_ext << " -n" << std::endl;
  std::cout << "to set it up and try again" << std::endl;
  gSystem->Exit(1);
#endif

  if (!Enable::MICROMEGAS)
  {
    G4MICROMEGAS::n_micromegas_layer = 0;
  }

  // SC_CALIBMODE makes no sense if distortions are not present
  G4TRACKING::SC_CALIBMODE = (G4TPC::ENABLE_STATIC_DISTORTIONS || G4TPC::ENABLE_TIME_ORDERED_DISTORTIONS ) && G4TRACKING::SC_CALIBMODE;

  // Genfit does final vertexing, Acts does not
  if (G4TRACKING::use_Genfit)
    G4TRACKING::g4eval_use_initial_vertex = false;

  // For now the TpcSpaceChargeCorrection module only works with the GenFit tracking chain
  if (G4TPC::ENABLE_CORRECTIONS && !G4TRACKING::use_Genfit)
  {
    std::cout << "Cannot enable space charge correction if not using GenFit tracking chain" << std::endl;
    G4TPC::ENABLE_CORRECTIONS = false;
  }

  /// Built the Acts geometry
  Fun4AllServer* se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  #if __cplusplus >= 201703L
  /// Geometry must be built before any Acts modules
  MakeActsGeometry* geom = new MakeActsGeometry();
  geom->Verbosity(verbosity);
  geom->setMagField(G4MAGNET::magfield);
  geom->setMagFieldRescale(G4MAGNET::magfield_rescale);
  
  /// Need a flip of the sign for constant field in tpc tracker
  if(G4TRACKING::use_PHTpcTracker_seeding && 
     G4MAGNET::magfield.find(".root") == std::string::npos)
    {
      geom->setMagFieldRescale(-1 * G4MAGNET::magfield_rescale);
    }
  se->registerSubsystem(geom);
  #endif  

}

void Tracking_Reco()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  // processes the TrkrHits to make clusters, then reconstruct tracks and vertices

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();

  //-------------
  // Tracking
  //------------

  // Assemble silicon clusters into track stubs 
  if(G4TRACKING::use_truth_silicon_seeding)  
    {

      // For the silicon, for each truth particle, create a track and associate clusters with it using truth information, write to silicon track map
      // track stubs are given the location of the truth vertex in this module
      PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingSilicon");
      pat_rec->Verbosity(10);
      pat_rec->set_track_map_name("SvtxSiliconTrackMap");
      pat_rec->set_min_layer(0);
      pat_rec->set_max_layer( G4MVTX::n_maps_layer +  G4INTT::n_intt_layer);
      
      se->registerSubsystem(pat_rec);
    }
  else
    {
      PHActsSiliconSeeding* silicon_Seeding = new PHActsSiliconSeeding();
      silicon_Seeding->Verbosity(0);
      se->registerSubsystem(silicon_Seeding);
    }
    
  // Initial vertex finding
  //=================================
  if(G4TRACKING::use_acts_init_vertexing)
    {
      
      PHActsInitialVertexFinder* init_vtx = new PHActsInitialVertexFinder();
      init_vtx->Verbosity(10);
      init_vtx->setSvtxTrackMapName("SvtxSiliconTrackMap");
      init_vtx->setSvtxVertexMapName("SvtxVertexMap");
      se->registerSubsystem(init_vtx);
    }
  else
    {
      PHTruthVertexing *init_vtx = new PHTruthVertexing();
      init_vtx->Verbosity(verbosity);
      init_vtx->set_acts_silicon(true);
      se->registerSubsystem(init_vtx);
    }
    
  // TPC track seeding (finds all clusters in TPC for tracks)
  //============================================
  if(G4TRACKING::use_truth_tpc_seeding)
    {
      // For the TPC, for each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
      // track stubs are given the position odf the truth vertex in this module
      PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingTpc");
      pat_rec->Verbosity(10);
      pat_rec->set_track_map_name("SvtxTrackMap");
      pat_rec->set_min_layer( G4MVTX::n_maps_layer +  G4INTT::n_intt_layer);
      pat_rec->set_max_layer( G4MVTX::n_maps_layer +  G4INTT::n_intt_layer + G4TPC::n_gas_layer);
      
      se->registerSubsystem(pat_rec);
    }
  else
    {
      std::cout << "Using normal TPC track seeding " << std::endl;
      
      // TPC track seeding from data
      if (G4TRACKING::use_PHTpcTracker_seeding && !G4TRACKING::use_hybrid_seeding)
	{
	  std::cout << "   Using PHTpcTracker track seeding " << std::endl;
	  
	  PHTpcTracker* tracker = new PHTpcTracker("PHTpcTracker");
	  tracker->set_seed_finder_options(3.0, M_PI / 8, 10, 6.0, M_PI / 8, 5, 1);   // two-pass CA seed params
	  tracker->set_seed_finder_optimization_remove_loopers(true, 20.0, 10000.0);  // true if loopers not needed
	  tracker->set_track_follower_optimization_helix(true);                       // false for quality, true for speed
	  tracker->set_track_follower_optimization_precise_fit(false);                // true for quality, false for speed
	  tracker->enable_json_export(false);                                         // save event as json, filename is automatic and stamped by current time in ms
	  tracker->enable_vertexing(false);                                           // rave vertexing is pretty slow at large multiplicities...
	  tracker->Verbosity(verbosity);
	  se->registerSubsystem(tracker);
	}
      else if(G4TRACKING::use_hybrid_seeding && !G4TRACKING::use_PHTpcTracker_seeding)
	{
	  std::cout << "   Using PHHybridSeeding track seeding " << std::endl;
	  PHHybridSeeding* hseeder = new PHHybridSeeding("PHHybridSeeding");
	  hseeder->set_field_dir(G4MAGNET::magfield_rescale);
	  hseeder->setSearchRadius(3.,6.); // mm (iter1, iter2)
	  hseeder->setSearchAngle(M_PI/8.,M_PI/8.); // radians (iter1, iter2)
	  hseeder->setMinTrackSize(10,5); // (iter1, iter2)
	  hseeder->setNThreads(1);
	  hseeder->Verbosity(0);
	  se->registerSubsystem(hseeder);
	}
      else
	{
	  if(G4TRACKING::use_hybrid_seeding && G4TRACKING::use_PHTpcTracker_seeding)
	    {
	      std::cerr << "***WARNING: MULTIPLE SEEDER OPTIONS SELECTED!***" << std::endl;
	      std::cerr << "  Current config selects both PHTpcTracker and PHHybridSeeding." << std::endl;
	      std::cerr << "  Since config doesn't make sense, reverting to default..." << std::endl;
	    }
	  std::cout << "   Using PHCASeeding track seeding " << std::endl;
	  
	  auto seeder = new PHCASeeding("PHCASeeding");
	  seeder->set_field_dir(G4MAGNET::magfield_rescale);  // to get charge sign right
	  seeder->Verbosity(verbosity);
	  seeder->SetLayerRange(7, 55);
	  seeder->SetSearchWindow(0.01, 0.02);  // (eta width, phi width)
	  seeder->SetMinHitsPerCluster(2);
	  seeder->SetMinClustersPerTrack(20);
	  se->registerSubsystem(seeder);
	}
    }
  
  // This does not care which seeder is used
  PHTpcTrackSeedVertexAssoc *vtxassoc = new PHTpcTrackSeedVertexAssoc();
  vtxassoc->Verbosity(0);
  se->registerSubsystem(vtxassoc);

  
  // Genfit track propagation and final fitting (starts from TPC track seeds)
  // Ignores Silicon track seeds, silicon association is by track propagation
  //========================================================
  if (G4TRACKING::use_Genfit)
    {
      
      std::cout << "   Using PHGenFitTrkProp " << std::endl;
      
      // Association of TPC track seeds with silicon layers and Micromegas layers
      // Find all clusters associated with each seed track
      auto track_prop = new PHGenFitTrkProp("PHGenFitTrkProp",
					    G4MVTX::n_maps_layer,
					    G4INTT::n_intt_layer,
					    G4TPC::n_gas_layer,
					    G4MICROMEGAS::n_micromegas_layer);
      track_prop->Verbosity(verbosity);
      se->registerSubsystem(track_prop);
      for (int i = 0; i < G4INTT::n_intt_layer; i++)
	{
	  // strip length is along theta
	  track_prop->set_max_search_win_theta_intt(i, 0.200);
	  track_prop->set_min_search_win_theta_intt(i, 0.200);
	  track_prop->set_max_search_win_phi_intt(i, 0.0050);
	  track_prop->set_min_search_win_phi_intt(i, 0.000);
	}
        
      std::cout << "   Using Genfit track fitting " << std::endl;
      
      PHGenFitTrkFitter* kalman = new PHGenFitTrkFitter();
      kalman->Verbosity(verbosity);
      kalman->set_vertexing_method(G4TRACKING::vmethod);
      kalman->set_use_truth_vertex(false);
      
      se->registerSubsystem(kalman);
    }
  
  // Acts tracking chain (starts from TPC track seeds)
  //===================================
  if (!G4TRACKING::use_Genfit)
    {
      std::cout << "   Using normal Acts matching chain for silicon and MM's " << std::endl;
      
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
    }

  // Final fitting of tracks using Acts Kalman Filter
  //=================================
  if (!G4TRACKING::use_Genfit)
  {
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
      PHTpcResiduals* residuals = new PHTpcResiduals();
      residuals->Verbosity(verbosity);
      se->registerSubsystem(residuals);
    }


    PHActsVertexFinder *finder = new PHActsVertexFinder();
    finder->Verbosity(verbosity);
    se->registerSubsystem(finder);

    PHActsTrkFitter* actsFit2 = new PHActsTrkFitter("PHActsSecondTrKFitter");
    actsFit2->Verbosity(verbosity);
    actsFit2->doTimeAnalysis(false);
    actsFit2->fitSiliconMMs(false);
    se->registerSubsystem(actsFit2);
  }

  // Final vertex finding and fitting with RAVE
  //=================================
  if (G4TRACKING::use_rave_vertexing)
  {
    PHRaveVertexing* rave = new PHRaveVertexing();
    //    rave->set_vertexing_method("kalman-smoothing:1");
    rave->set_over_write_svtxvertexmap(false);
    rave->set_svtxvertexmaprefit_node_name("SvtxVertexMapRave");
    rave->Verbosity(verbosity);
    se->registerSubsystem(rave);
  }

  //------------------
  // Track Projections
  //------------------
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
  eval->do_gpoint_eval(false);
  eval->do_eval_light(true);
  eval->set_use_initial_vertex(G4TRACKING::g4eval_use_initial_vertex);
  eval->scan_for_embedded(true);  // take all tracks if false - take only embedded tracks if true
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
  if (!G4TRACKING::use_Genfit)
  {
#if __cplusplus >= 201703L

    //    PHActsVertexFinder* vtxer = new PHActsVertexFinder();
    //    vtxer->Verbosity(verbosity);
    //    se->registerSubsystem(vtxer);

    QAG4SimulationVertex* qav = new QAG4SimulationVertex();
    // qav->addEmbeddingID(2);
    qav->Verbosity(verbosity);
    qav->setVertexMapName("SvtxVertexMapActs");
    se->registerSubsystem(qav);
#endif
  }

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
