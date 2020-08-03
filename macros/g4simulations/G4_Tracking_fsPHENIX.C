#pragma once

#include "GlobalVariables.C"

#include "G4_Mvtx.C"
#include "G4_Intt.C"
#include "G4_TPC.C"

#include <fun4all/Fun4AllServer.h>

#include <g4eval/SvtxEvaluator.h>

#include <g4main/PHG4Reco.h>


#include <trackreco/PHGenFitTrkFitter.h>
#include <trackreco/PHGenFitTrkProp.h>
#include <trackreco/PHHoughSeeding.h>
#include <trackreco/PHInitZVertexing.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PHTruthVertexing.h>
#include <trackreco/PHTruthTrackSeeding.h>
#include <trackreco/PHGenFitTrackProjection.h>

#include <trackbase/TrkrHitTruthAssoc.h>

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)

namespace Enable
{
  bool TRACKING_TRACK = false;
  bool TRACKING_EVAL = false;
  int TRACKING_VERBOSITY = 0;
}

namespace G4TRACKING
{

// Tracking simulation setup parameters and flag - leave them alone!
//==============================================
// Tracking reconstruction setup parameters and flags
//=====================================
const int init_vertexing_min_zvtx_tracks = 2; // PHInitZvertexing parameter for reducing spurious vertices, use 2 for Pythia8 events, 5 for large multiplicity events
const bool use_track_prop = true;   // true for normal track seeding, false to run with truth track seeding instead
const bool g4eval_use_initial_vertex = false;   // if true, g4eval uses initial vertices in SvtxVertexMap, not final vertices in SvtxVertexMapRefit
const bool use_primary_vertex = false;  // if true, refit tracks with primary vertex included - adds second node to node tree, adds second evaluator and outputs separate ntuples

// This is the setup we have been using before PHInitZVertexing was implemented - smeared truth vertex for a single collision per event. Make it the default for now.
std::string vmethod("avf-smoothing:1");  // only good for 1 vertex events // vmethod is a string used to set the Rave final-vertexing method:
const bool use_truth_vertex = true;   // set to false to get initial vertex from MVTX hits using PHInitZVertexing, true for using smeared truth vertex

// This is the setup that uses PHInitZvertexing to find initial vertices, and allows for multiple collisions per event
//const bool use_truth_vertex = false;   // set to false to get initial vertex from MVTX hits using PHInitZVertexing, true for using smeared truth vertex
//std::string vmethod("avr-smoothing:1-minweight:0.5-primcut:9-seccut:9");  // seems to handle multi-vertex events.
}


void TrackingInit(int verbosity = 0)
{
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
  if (G4TRACKING::use_track_prop)
    {
      //--------------------------------------------------
      // Normal track seeding and propagation
      //--------------------------------------------------

      if(G4TRACKING::use_truth_vertex)
	{
	  // We cheat to get the initial vertex for the full track reconstruction case
	  PHInitVertexing* init_vtx  = new PHTruthVertexing("PHTruthVertexing");
	  init_vtx->Verbosity(verbosity);
	  se->registerSubsystem(init_vtx);
	}
      else
	{
	  // get the initial vertex for track fitting from the MVTX hits
	    PHInitZVertexing* init_zvtx  = new PHInitZVertexing(7, 7, "PHInitZVertexing");
	    int seed_layer[7] = {0,1,2,3,4,5,6};
	    init_zvtx->set_seeding_layer(seed_layer,7);
	    // this is the minimum number of associated MVTX triplets for a vertex to be accepted as a candidate.
	    // Suggest to use 2 for Pythia8 and 5 for Au+Au (to reduce spurious vertices).
	    init_zvtx->set_min_zvtx_tracks(G4TRACKING::init_vertexing_min_zvtx_tracks);
	    init_zvtx->Verbosity(verbosity);
	    se->registerSubsystem(init_zvtx);
	}

      // find seed tracks using a subset of TPC layers
      int min_layers = 4;
      int nlayers_seeds = 12;
      PHHoughSeeding* track_seed = new PHHoughSeeding("PHHoughSeeding", G4MVTX::n_maps_layer, G4INTT::n_intt_layer, G4TPC::n_gas_layer, nlayers_seeds, min_layers);
      track_seed->Verbosity(0);
      se->registerSubsystem(track_seed);

      // Find all clusters associated with each seed track
      PHGenFitTrkProp* track_prop = new PHGenFitTrkProp("PHGenFitTrkProp", G4MVTX::n_maps_layer, G4INTT::n_intt_layer, G4TPC::n_gas_layer);
      track_prop->Verbosity(verbosity);
      se->registerSubsystem(track_prop);
      for(int i = 0;i<G4INTT::n_intt_layer;i++)
	{
	  // strip length is along theta
	  track_prop->set_max_search_win_theta_intt(i, 0.200);
	  track_prop->set_min_search_win_theta_intt(i, 0.200);
	  track_prop->set_max_search_win_phi_intt(i, 0.0050);
	  track_prop->set_min_search_win_phi_intt(i, 0.000);
	}
    }
  else
    {
      //-------------------------------------------------------
      // Track finding using truth information only
      //------------------------------------------------------

      PHInitVertexing* init_vtx  = new PHTruthVertexing("PHTruthVertexing");
      init_vtx->Verbosity(verbosity);
      se->registerSubsystem(init_vtx);

      // For each truth particle, create a track and associate clusters with it using truth information
      PHTruthTrackSeeding* pat_rec = new PHTruthTrackSeeding("PHTruthTrackSeeding");
      pat_rec->Verbosity(verbosity);
      se->registerSubsystem(pat_rec);
    }

  //------------------------------------------------
  // Fitting of tracks using Kalman Filter
  //------------------------------------------------


  PHGenFitTrkFitter* kalman = new PHGenFitTrkFitter();
  kalman->Verbosity(verbosity);

  if (G4TRACKING::use_primary_vertex)
  {
    kalman->set_fit_primary_tracks(true);  // include primary vertex in track fit if true
  }
  kalman->set_vertexing_method(G4TRACKING::vmethod);
  kalman->set_use_truth_vertex(false);

  se->registerSubsystem(kalman);

  //------------------
  // Track Projections
  //------------------
  PHGenFitTrackProjection* projection = new PHGenFitTrackProjection();
  projection->Verbosity(verbosity);
  se->registerSubsystem(projection);

  return;
}


 void Tracking_Eval(const std::string &outputfile)
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
  eval = new SvtxEvaluator("SVTXEVALUATOR", outputfile, "SvtxTrackMap", G4MVTX::n_maps_layer, G4INTT::n_intt_layer, G4TPC::n_gas_layer);
  eval->do_cluster_eval(true);
  eval->do_g4hit_eval(true);
  eval->do_hit_eval(true);  // enable to see the hits that includes the chamber physics...
  eval->do_gpoint_eval(false);
  eval->do_eval_light(true);
  eval->set_use_initial_vertex(G4TRACKING::g4eval_use_initial_vertex);
  eval->scan_for_embedded(false);  // take all tracks if false - take only embedded tracks if true
  eval->Verbosity(verbosity);
  se->registerSubsystem(eval);

  if (G4TRACKING::use_primary_vertex)
  {
    // make a second evaluator that records tracks fitted with primary vertex included
    // good for analysis of prompt tracks, particularly if Mvtx is not present
    SvtxEvaluator* evalp;
    evalp = new SvtxEvaluator("SVTXEVALUATOR", outputfile + "_primary_eval.root", "PrimaryTrackMap", G4MVTX::n_maps_layer, G4INTT::n_intt_layer, G4TPC::n_gas_layer);
    evalp->do_cluster_eval(true);
    evalp->do_g4hit_eval(true);
    evalp->do_hit_eval(false);
    evalp->do_gpoint_eval(false);
    evalp->scan_for_embedded(true);  // take all tracks if false - take only embedded tracks if true
    evalp->Verbosity(verbosity);
    se->registerSubsystem(evalp);
  }

  return;
}
