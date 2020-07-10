#ifndef MACRO_READCLUSTERSRUNTRACKING_C
#define MACRO_READCLUSTERSRUNTRACKING_C

#include "GlobalVariables.C"

#include "G4Setup_sPHENIX.C"

#include <fun4all/Fun4AllServer.h>

#include <g4eval/TrkrEvaluator.h>
#include <g4eval/SvtxEvaluator.h>

#include <g4intt/PHG4InttDefs.h>
#include <g4intt/PHG4InttDigitizer.h>
#include <g4intt/PHG4InttSubsystem.h>
#include <g4intt/PHG4InttHitReco.h>
#include <g4intt/PHG4InttDefs.h>

#include <g4main/PHG4Reco.h>

#include <g4mvtx/PHG4MvtxDefs.h>
#include <g4mvtx/PHG4MvtxDigitizer.h>
#include <g4mvtx/PHG4MvtxSubsystem.h>
#include <g4mvtx/PHG4MvtxHitReco.h>

#include <g4tpc/PHG4TpcDigitizer.h>
#include <g4tpc/PHG4TpcElectronDrift.h>
#include <g4tpc/PHG4TpcPadPlane.h>
#include <g4tpc/PHG4TpcPadPlaneReadout.h>
#include <g4tpc/PHG4TpcSubsystem.h>

#include <intt/InttClusterizer.h>
#include <mvtx/MvtxClusterizer.h>
#include <tpc/TpcClusterizer.h>

#include <trackreco/PHGenFitTrkFitter.h>
#include <trackreco/PHGenFitTrkProp.h>
//#include <trackreco/PHRTreeSeeding.h>
#include <trackreco/PHHoughSeeding.h>
#include <trackreco/PHInitVertexing.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PHInitVertexing.h>
#include <trackreco/PHTrackSeeding.h>
#include <trackreco/PHTruthVertexing.h>
#include <trackreco/PHTruthTrackSeeding.h>
//#include <trackreco/PHGenFitTrackProjection.h>

#include <trackbase/TrkrHitTruthAssoc.h>


R__LOAD_LIBRARY(libg4tpc.so)
R__LOAD_LIBRARY(libg4intt.so)
R__LOAD_LIBRARY(libg4mvtx.so)
R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libmvtx.so)
R__LOAD_LIBRARY(libtpc.so)
R__LOAD_LIBRARY(libtrack_reco.so)

//#define _ONLY_SEEDING_
void ReadClustersRunTracking(
     const int nEvents = 1, 
     const char *inputFile = "G4sPHENIX.root",
     const char *outputFile = "G4sPHENIX_tracking.root")
{


  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(10);

  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libphool.so");
  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4dst.so");
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4eval.so");
  
  gROOT->LoadMacro("G4_Tracking.C"); 

  Tracking_Reco(0);
  
  // explicitely configure the evaluator
  SvtxEvaluator* eval;
  const int n_maps_layer = 3;
  const int n_intt_layer = 4;
  int n_gas_layer  = 48;

  eval = new SvtxEvaluator("SVTXEVALUATOR", outputFile, "SvtxTrackMap", n_maps_layer, n_intt_layer, n_gas_layer);
  eval->do_cluster_eval(false);
  eval->do_g4hit_eval(false);
  eval->do_hit_eval(false);  // enable to see the hits that includes the chamber physics...
  eval->do_gpoint_eval(false);
  eval->do_eval_light(true);
  eval->scan_for_embedded(true);  // take all tracks if false - take only embedded tracks if true
  eval->Verbosity(0);
  se->registerSubsystem(eval);
  
  Fun4AllInputManager *in = new Fun4AllDstInputManager("DSTin");
  
  TString tstr_input(inputFile);
  if (tstr_input.EndsWith(".root"))
    in->AddFile(inputFile);
  else
    in->AddListFile(inputFile);
  se->registerInputManager(in);
  
  se->run(nEvents);
  std::cout << " Done Run, ending... " << std::endl;
  se->End();

  se->PrintTimer();

  std::cout << " Success!! " << std::endl;
  //       delete se;
  gSystem->Exit(0);
  
  
}

#endif
