/*!
 *  \file	    RunGenFit.C	
 *  \brief	    Example macro to run PHG4KalmanPatRec
 *  \author	    Haiwang Yu <yuhw@nmsu.edu>
 */
#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6, 00, 0)
#include "GlobalVariables.C"

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
#endif

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllOutputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <g4main/PHG4ParticleGeneratorBase.h>
#include <g4main/PHG4ParticleGenerator.h>
#include <g4main/PHG4SimpleEventGenerator.h>
#include <g4main/PHG4ParticleGeneratorVectorMeson.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/HepMCNodeReader.h>
#include <g4detectors/PHG4DetectorSubsystem.h>
#include <phool/recoConsts.h>
#include <phpythia6/PHPythia6.h>
#include <phpythia8/PHPythia8.h>
#include <phhepmc/Fun4AllHepMCPileupInputManager.h>
#include <phhepmc/Fun4AllHepMCInputManager.h>
#include "G4Setup_sPHENIX.C"
#include "G4_Bbc.C"
#include "G4_Global.C"
#include "G4_CaloTrigger.C"
#include "G4_Jets.C"
#include "G4_HIJetReco.C"
#include "G4_DSTReader.C"
#include "DisplayOn.C"
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libphhepmc.so)
R__LOAD_LIBRARY(libPHPythia6.so)
R__LOAD_LIBRARY(libPHPythia8.so)
#endif


//#define _ONLY_SEEDING_
void ReadClustersRunTracking(
     const int nEvents = 1, 
     const char *inputFile = "G4sPHENIX.root",
     const char *outputFile = "G4sPHENIX_tracking.root")
{

  gSystem->Load("libtrack_reco.so");

  gROOT->LoadMacro("G4Setup_sPHENIX.C");

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

