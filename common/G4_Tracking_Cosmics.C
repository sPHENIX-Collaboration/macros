#ifndef MACRO_G4TRACKING_C
#define MACRO_G4TRACKING_C

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! NOTICE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// To use, this must be included in BOTH your Fun4All macro AND common/G4_Micromegas.C!

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libqa_modules.so)

#include <G4_ActsGeom.C>
#include <G4_TPC.C>
#include <G4_Mvtx.C>
#include <G4_Intt.C>
#include <G4_Magnet.C>
#include <G4_Micromegas.C>

#include <g4eval/SvtxEvaluator.h>
#include <g4eval/SvtxTruthRecoTableEval.h>
#include <g4eval/TrackSeedTrackMapConverter.h>

#include <trackreco/MakeActsGeometry.h>
#include <trackreco/PHActsSiliconSeeding.h>
#include <trackreco/PHCASeeding.h>
#include <trackreco/PHMicromegasTpcTrackMatching.h>
#include <trackreco/PHSiliconHelicalPropagator.h>
#include <trackreco/PHSimpleKFProp.h>
#include <trackreco/PHSimpleVertexFinder.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PHTruthSiliconAssociation.h>
#include <trackreco/PHTruthTrackSeeding.h>

#include <tpc/TpcLoadDistortionCorrection.h>

#include <trackermillepedealignment/MakeMilleFiles.h>
#include <trackermillepedealignment/HelicalFitter.h>

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
  bool use_alignment = false;
  bool use_full_truth_track_seeding = false;
  bool use_truth_tpc_seeding = false;
  bool g4eval_use_initial_vertex = true;
  bool SC_CALIBMODE = false;
}

void TrackingInit()
{
  ACTSGEOM::ActsGeomInit();
  // space charge correction
  /* corrections are applied in the track finding, and via TpcClusterMover before the final track fit */
  if( G4TPC::ENABLE_CORRECTIONS )
  {
    auto se = Fun4AllServer::instance();
    auto tpcLoadDistortionCorrection = new TpcLoadDistortionCorrection;
    tpcLoadDistortionCorrection->set_distortion_filename( G4TPC::correction_filename );
    se->registerSubsystem(tpcLoadDistortionCorrection);
  }
}

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

  if (!G4TRACKING::use_full_truth_track_seeding)
  {  
    // Assemble TPC clusters into track stubs 
    if (G4TRACKING::use_truth_tpc_seeding)
    {
      // For the TPC, for each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
      // track stubs are given the position odf the truth vertex in this module
      auto pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingTpc");
      pat_rec->Verbosity(verbosity);
      pat_rec->set_track_map_name("TpcTrackSeedContainer");
      pat_rec->set_min_layer(G4MVTX::n_maps_layer + G4INTT::n_intt_layer);
      pat_rec->set_max_layer(G4MVTX::n_maps_layer + G4INTT::n_intt_layer + G4TPC::n_gas_layer);
      se->registerSubsystem(pat_rec);

    } else {
      
      auto seeder = new PHCASeeding("PHCASeeding");
      seeder->set_field_dir(G4MAGNET::magfield_rescale);  // to get charge sign right
      if (G4MAGNET::magfield.find("3d") != std::string::npos)
      {
        seeder->set_field_dir(-1 * G4MAGNET::magfield_rescale);
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
      cprop->useConstBField(false);
      cprop->useFixedClusterError(true);
      cprop->set_max_window(5.);
      cprop->set_cluster_version(G4TRACKING::cluster_version);
      cprop->Verbosity(verbosity);
      se->registerSubsystem(cprop);
    }

    PHSiliconHelicalPropagator* hprop = new PHSiliconHelicalPropagator("HelicalPropagator");
    hprop->Verbosity(verbosity);
    se->registerSubsystem(hprop);
 
    // Associate Micromegas clusters with the tracks
    if( Enable::MICROMEGAS )
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

  } else {
    
    // full truth track finding
    std::cout << "Tracking_Reco_TrackSeed - Using full truth track seeding" << std::endl;

    // For each truth particle, create a track and associate clusters with it using truth information, write to Svtx track map
    // track stubs are given the position of the truth vertex in this module, but Genfit does not care
    // Includes clusters for TPC, silicon and MM's
    auto pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeedingFull");
    pat_rec->Verbosity(verbosity);
    pat_rec->set_track_map_name("SvtxTrackSeedContainer");
    se->registerSubsystem(pat_rec);

  }

   /*
   * all done
   * at this stage tracks are fully assembled. They contain clusters spaning Silicon detectors, TPC and Micromegas
   * they are ready to be fit.
   */ 
    convert_seeds();
}

void vertexing()
{
  // SvtxEvaluator will not run without vertexing, even though it produces nothing useful in this case
  Fun4AllServer* se = Fun4AllServer::instance();
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  auto vtxfinder = new PHSimpleVertexFinder;
  vtxfinder->Verbosity(verbosity);
  se->registerSubsystem(vtxfinder);
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
  se->registerSubsystem(mille);

  auto helical = new HelicalFitter;
  helical->Verbosity(verbosity);
  helical->set_datafile_name(datafilename + "_helical.bin");
  helical->set_steeringfile_name(steeringfilename + "_helical.txt");
  se->registerSubsystem(helical);

}

void Tracking_Reco()
{
  Tracking_Reco_TrackSeed();
  vertexing();
  if(G4TRACKING::use_alignment) alignment();
}

void build_truthreco_tables()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();
  
  // this module builds high level truth track association table.
  // If this module is used, this table should be called before any evaluator calls.
  // Removing this module, evaluation will still work but trace truth association through the layers of G4-hit-cluster
  SvtxTruthRecoTableEval *tables = new SvtxTruthRecoTableEval();
  tables->Verbosity(verbosity);
  se->registerSubsystem(tables);

  return;
}

void Tracking_Eval(const std::string& outputfile)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer* se = Fun4AllServer::instance();
  build_truthreco_tables(); 

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
  bool embed_scan = true;
  if(TRACKING::pp_mode) embed_scan = false;
  eval->scan_for_embedded(embed_scan);   // take all tracks if false - take only embedded tracks if true
  eval->scan_for_primaries(embed_scan);  // defaults to only thrown particles for ntp_gtrack
  std::cout << "SvtxEvaluator: pp_mode set to " << TRACKING::pp_mode << " and scan_for_embedded set to " << embed_scan << std::endl;
  eval->Verbosity(verbosity);
  eval->set_cluster_version(G4TRACKING::cluster_version);

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

  build_truthreco_tables();

  QAG4SimulationTracking* qa = new QAG4SimulationTracking();
  //  qa->addEmbeddingID(2);
  qa->Verbosity(verbosity);
  se->registerSubsystem(qa);

  QAG4SimulationVertex* qa2 = new QAG4SimulationVertex();
  // qa2->addEmbeddingID(2);
  qa2->Verbosity(verbosity);
  se->registerSubsystem(qa2);

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


#endif // MACRO_G4TRACKING_C
