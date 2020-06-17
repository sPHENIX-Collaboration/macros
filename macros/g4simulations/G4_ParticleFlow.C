#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <particleflowreco/ParticleFlowReco.h>
#include <particleflowreco/ParticleFlowJetInput.h>
#include <g4jets/FastJetAlgo.h>
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libparticleflow.so)
#endif

void ParticleFlow(int verbosity = 0) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4jets.so");
  gSystem->Load("libparticleflow.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  // note: assumes topoCluster input already configured 
  ParticleFlowReco *pfr = new ParticleFlowReco();
  pfr->Verbosity( verbosity );
  se->registerSubsystem( pfr ); 

  JetReco *particleflowjetreco = new JetReco("PARTICLEFLOWJETRECO");
  particleflowjetreco->add_input( new ParticleFlowJetInput() );
  particleflowjetreco->add_algo( new FastJetAlgo(Jet::ANTIKT,0.4), "AntiKt_ParticleFlow_r04" );
  particleflowjetreco->set_algo_node("ANTIKT");
  particleflowjetreco->set_input_node("PARTICLEFLOW");
  particleflowjetreco->Verbosity( verbosity );
  se->registerSubsystem( particleflowjetreco ); 
  
  return;

}  
