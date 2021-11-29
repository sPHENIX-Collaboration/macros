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
#include <trackreco/PHSimpleKFProp.h>
#include <trackreco/PHGenFitTrackProjection.h>
#include <trackreco/PHGenFitTrkFitter.h>
#include <trackreco/PHGenFitTrkProp.h>
#include <trackreco/PHGhostRejection.h>
#include <trackreco/PHHoughSeeding.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHRaveVertexing.h>
#include <trackreco/PHSiliconTpcTrackMatching.h>
#include <trackreco/PHTpcTrackSeedCircleFit.h>
#include <trackreco/PHTrackCleaner.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PHTruthSiliconAssociation.h>
#include <trackreco/PHTruthTrackSeeding.h>
#include <trackreco/PHTruthVertexing.h>
#include <trackreco/PHActsVertexPropagator.h>
#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHActsTrkFitter.h>
#include <trackreco/PHSimpleVertexFinder.h>

#include <tpccalib/TpcSpaceChargeReconstruction.h>
#include <tpccalib/PHTpcResiduals.h>

#include <trackbase/TrkrHitTruthAssoc.h>

#include <phtpctracker/PHTpcTracker.h>

#include <qa_modules/QAG4SimulationTracking.h>
#include <qa_modules/QAG4SimulationUpsilon.h>
#include <qa_modules/QAG4SimulationVertex.h>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)
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
  std::string SC_ROOTOUTPUT_FILENAME = "TpcSpaceChargeMatrices.root"; // space charge calibration output file

  // Tracking reconstruction setup parameters and flags
  //=====================================

  // The normal (default) Acts tracking chain is:
  //   PHActsSiliconSeeding                    // make silicon track seeds
  //   PHCASeeding                                    // TPC track seeds
  //    PHTpcTrackSeedVertexAssoc    // Associates TPC track seeds with a vertex, refines phi and eta
  //   PHSiliconTpcTrackMatching      // match TPC track seeds to silicon track seeds
  //   PHMicromegasTpcTrackMatching   // associate Micromegas clusters with TPC track stubs
  //   PHActsTrkFitter (1)                         // Kalman fitter makes fit to assembled tracks
  //   PHSimpleVertexFinder                       // final vertexing using fitted Acts tracks
  //   PHActsVertexPropagator          // propagates track parameters to vertex position to get final dca

  // Possible variations 
  //====================================
  //Fittting
  bool use_genfit = false;                 // if false, acts KF is run on proto tracks. If true, use Genfit track propagation and fitting

  // Vertexing
  bool g4eval_use_initial_vertex = true;  // if true, g4eval uses initial vertices in SvtxVertexMap, not final vertices in SvtxVertexMapRefit

  // TPC seeding options
  bool use_PHTpcTracker_seeding = false;  // false for using the default PHCASeeding to get TPC track seeds, true to use PHTpcTracker
  bool use_hybrid_seeding = false;                  // false for using the default PHCASeeding, true to use PHHybridSeeding (STAR core, ALICE KF)
  bool use_propagator = true;             // use PHSimpleKFProp for CA seeding if true

  // set to false to disable adding fake surfaces (TPC, Micromegas) to MakeActsGeom
  bool add_fake_surfaces = true;

  // Truth seeding options (can use any or all)
  bool use_truth_silicon_seeding = false;        // if true runs truth silicon seeding instead of acts silicon seeding
  bool use_truth_tpc_seeding = false;              // if true runs truth silicon seeding instead of reco TPC seeding
  bool use_truth_si_matching = false;              // if true, associates silicon clusters using best truth track match to TPC seed tracks - for diagnostics only
 // Full truth track seeding 
  bool use_full_truth_track_seeding = false;  // makes track seeds using truth info, used for both Acts and Genfit
  bool use_truth_vertexing = false;    // if true runs truth vertexing, if false runs PHSimpleVertexFinder

  // Rave final vertexing (for QA)
  bool use_rave_vertexing = true;                     // Use Rave to find and fit for vertex after track fitting - used for QA only
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

  // For now the TpcSpaceChargeCorrection module only works with the GenFit tracking chain
  if (G4TPC::ENABLE_CORRECTIONS && !G4TRACKING::use_genfit)
  {
    std::cout << "Cannot enable space charge correction if not using GenFit tracking chain" << std::endl;
    G4TPC::ENABLE_CORRECTIONS = false;
  }

  // Check for colliding switches
  if(G4TRACKING::use_hybrid_seeding && G4TRACKING::use_PHTpcTracker_seeding)
    {
      std::cerr << "***WARNING: MULTIPLE SEEDER OPTIONS SELECTED!***" << std::endl;
      std::cerr << "  Current config selects both PHTpcTracker and PHHybridSeeding." << std::endl;
      std::cerr << "  Revert to default...." << std::endl;
      G4TRACKING::use_hybrid_seeding = false;
      G4TRACKING::use_PHTpcTracker_seeding = false;
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
  geom->add_fake_surfaces( G4TRACKING::add_fake_surfaces );
  
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

  //----------------------------------------------------------------------------
  // Tracking
  // Sections 1, 2 and 3 are alternatives to each other
  // Section 1: Normal Acts tracking chain, with options for truth seeding
  // Section 2: Truth track seeding with Acts fitting
  // Section 3: Genfit tracking chain with option for truth track seeding
  //------------------------------------------------------------------------------

  //====================
  // Common to all sections
  // Silicon seeding
  //=====================

  // Assemble silicon clusters into track stubs - needed for initial vertex finding
  //============================================================
  if(G4TRACKING::use_truth_silicon_seeding)  
    {
      
      // For the silicon, for each truth particle, create a track and associate clusters with it using truth information, write to silicon track map
      // track stubs are given the location of the truth vertex in this module
      PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingSilicon");
      pat_rec->Verbosity(verbosity);
      pat_rec->set_track_map_name("SvtxSiliconTrackMap");
      pat_rec->set_min_layer(0);
      pat_rec->set_max_layer( G4MVTX::n_maps_layer +  G4INTT::n_intt_layer);
      
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
  // Section 1: Acts chain of track propagation and final fitting 
  //================================================
  if(!G4TRACKING::use_genfit && !G4TRACKING::use_full_truth_track_seeding)
    {      
      // TPC track seeding (finds all clusters in TPC for tracks)
      //============================================
      if(G4TRACKING::use_truth_tpc_seeding)
	{
	  // For the TPC, for each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
	  // track stubs are given the position odf the truth vertex in this module
	  PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingTpc");
	  pat_rec->Verbosity(verbosity);
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
	      hseeder->Verbosity(verbosity);
	      se->registerSubsystem(hseeder);
	    }
	  else
	    {
	      std::cout << "   Using PHCASeeding track seeding " << std::endl;
	      
	      auto seeder = new PHCASeeding("PHCASeeding");
	      seeder->set_field_dir(G4MAGNET::magfield_rescale);  // to get charge sign right
	      if(G4MAGNET::magfield.find("3d") != std::string::npos)
		{ seeder->set_field_dir(-1*G4MAGNET::magfield_rescale); }
	      seeder->Verbosity(verbosity);
	      seeder->SetLayerRange(7, 55);
	      seeder->SetSearchWindow(0.01, 0.02);  // (eta width, phi width)
	      seeder->SetMinHitsPerCluster(0);
	      if(G4TRACKING::use_propagator) seeder->SetMinClustersPerTrack(3);
	      else seeder->SetMinClustersPerTrack(20);
	      seeder->useConstBField(false);
	      seeder->useFixedClusterError(true);
	      se->registerSubsystem(seeder);

	      if(G4TRACKING::use_propagator)
	      {
	        PHTpcTrackSeedCircleFit* vtxassoc2 = new PHTpcTrackSeedCircleFit("PrePropagatorPHTpcTrackSeedCircleFit");
	        vtxassoc2->Verbosity(verbosity);
	        se->registerSubsystem(vtxassoc2);

	        std::cout << "   Using PHSimpleKFProp propagator " << std::endl;
	        PHSimpleKFProp* cprop = new PHSimpleKFProp("PHSimpleKFProp");
		cprop->set_field_dir(G4MAGNET::magfield_rescale);
		if(G4MAGNET::magfield.find("3d") != std::string::npos)
		  { cprop->set_field_dir(-1*G4MAGNET::magfield_rescale); }
	        cprop->useConstBField(false);
	        cprop->useFixedClusterError(true);
	        cprop->set_max_window(5.);
	        cprop->Verbosity(verbosity);
		se->registerSubsystem(cprop);
	      }
	    }
	}

      // Associate TPC track stubs with silicon and Micromegas
      //=============================================

      // This does not care which seeder is used
      // It refines the phi and eta of the TPC tracklet prior to matching with the silicon tracklet
      PHTpcTrackSeedCircleFit *vtxassoc = new PHTpcTrackSeedCircleFit();
      vtxassoc->Verbosity(verbosity);
      se->registerSubsystem(vtxassoc);
      
      // Choose the best duplicate TPC track seed
      PHGhostRejection *ghosts= new PHGhostRejection();
      ghosts->Verbosity(verbosity);
      se->registerSubsystem(ghosts);
            
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
      actsFit->doTimeAnalysis(false);
      /// If running with distortions, fit only the silicon+MMs first
      actsFit->fitSiliconMMs(G4TRACKING::SC_CALIBMODE);
      se->registerSubsystem(actsFit);
      
         if (G4TRACKING::SC_CALIBMODE)
	{
	  /// run tpc residual determination with silicon+MM track fit
	  auto residuals = new PHTpcResiduals;
	  residuals->setOutputfile( G4TRACKING::SC_ROOTOUTPUT_FILENAME );
	  residuals->Verbosity(verbosity);
	  se->registerSubsystem(residuals);
	}

      // Choose the best silicon matched track for each TPC track seed
      PHTrackCleaner *cleaner= new PHTrackCleaner();
      cleaner->Verbosity(verbosity);
      se->registerSubsystem(cleaner);

      if(G4TRACKING::use_truth_vertexing)
	{
	  PHTruthVertexing *vtxing = new PHTruthVertexing();
	  vtxing->associate_tracks(true);
	  std::string trackmapnamef = "SvtxTrackMap";
	  vtxing->set_track_map_name(trackmapnamef);
	  se->registerSubsystem(vtxing);
	}
      else
	{
	  PHSimpleVertexFinder *vtxfinder = new PHSimpleVertexFinder();
	  vtxfinder->Verbosity(verbosity);
	  se->registerSubsystem(vtxfinder);
	}

      /// Propagate track positions to the vertex position
      PHActsVertexPropagator *vtxProp = new PHActsVertexPropagator();
      vtxProp->Verbosity(verbosity);
      se->registerSubsystem(vtxProp);

    }

  //=========================================================    
  // Section 2: Full truth track finding with Acts final fitting 
  //=========================================================
  if( !G4TRACKING::use_genfit && G4TRACKING::use_full_truth_track_seeding)
    {
      std::cout << "  Using full truth track seeding for Acts" << std::endl;
      
      // For each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
      // track stubs are given the position of the truth vertex in this module, but Genfit does not care
      // Includes clusters for TPC, silicon and MM's
      PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingFull");
      pat_rec->Verbosity(verbosity);
      pat_rec->set_track_map_name("SvtxTrackMap");
      se->registerSubsystem(pat_rec);
      
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
    residuals->Verbosity(verbosity);
	  se->registerSubsystem(residuals);
	}
            
      PHSimpleVertexFinder *finder = new PHSimpleVertexFinder();
      finder->Verbosity(verbosity);
      se->registerSubsystem(finder);
      
    }
  
  //=========================================================    
  // Section 3: Genfit track propagation and final fitting
  // Silicon association is by track propagation
  //========================================================
  if (G4TRACKING::use_genfit)
    {
      // Track seeding
      //============
      if(G4TRACKING::use_full_truth_track_seeding)
	{
	  std::cout << "  Using truth track seeding for Genfit" << std::endl;

	  // For each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
	  // tracks are given the position of the truth particle vertex in this module, but Genfit does not care
	  // Includes clusters for TPC, silicon and MM's
	  PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingFull");
	  pat_rec->Verbosity(verbosity);
	  pat_rec->set_track_map_name("SvtxTrackMap");
	  se->registerSubsystem(pat_rec);
	}
      else
	{
	  // need TPC track seeds to give to GenfitTrkProp

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
	      hseeder->Verbosity(verbosity);
	      se->registerSubsystem(hseeder);
	    }
	  else
	    {
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

	  std::cout << "   Using PHGenFitTrkProp " << std::endl;
	  
	  // Association of TPC track seeds with all TPC layers, silicon layers and Micromegas layers
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
	}
        
      std::cout << "   Using Genfit track fitting " << std::endl;
      
      auto kalman = new PHGenFitTrkFitter;
      kalman->Verbosity(verbosity);
      kalman->set_vertexing_method(G4TRACKING::vmethod);
      kalman->set_use_truth_vertex(false);      
      
      // in space charge calibration mode, disable the tpc
      if( G4TRACKING::SC_CALIBMODE )
      {
        std::cout << "Tracking_reco - Disabling TPC layers from kalman filter" << std::endl;
        for( int layer = 7; layer < 23; ++layer ) { kalman->disable_layer( layer ); }
        for( int layer = 23; layer < 39; ++layer ) { kalman->disable_layer( layer ); }
        for( int layer = 39; layer < 55; ++layer ) { kalman->disable_layer( layer ); }
      }

      se->registerSubsystem(kalman);

      if( G4TRACKING::SC_CALIBMODE )
      {
        // Genfit based Tpc space charge Reconstruction
        auto tpcSpaceChargeReconstruction = new TpcSpaceChargeReconstruction;
        tpcSpaceChargeReconstruction->set_outputfile( G4TRACKING::SC_ROOTOUTPUT_FILENAME );
        se->registerSubsystem(tpcSpaceChargeReconstruction);
      }
      
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
  eval->set_use_genfit_vertex(G4TRACKING::use_genfit);
  eval->scan_for_embedded(true);  // take all tracks if false - take only embedded tracks if true
  eval->scan_for_primaries(true); // defaults to only thrown particles for ntp_gtrack
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
  if (!G4TRACKING::use_genfit)
  {
#if __cplusplus >= 201703L

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
