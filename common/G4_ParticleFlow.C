#ifndef MACRO_G4PARTICLEFLOW_C
#define MACRO_G4PARTICLEFLOW_C

#include <GlobalVariables.C>

#include <g4jets/FastJetAlgo.h>
#include <g4jets/JetReco.h>

#include <particleflowreco/ParticleFlowJetInput.h>
#include <particleflowreco/ParticleFlowReco.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4jets.so)
R__LOAD_LIBRARY(libparticleflow.so)

namespace Enable
{
  bool PARTICLEFLOW = false;
  int PARTICLEFLOW_VERBOSITY = 0;
}  // namespace Enable

void ParticleFlow()
{
  int verbosity = max(Enable::VERBOSITY, Enable::PARTICLEFLOW_VERBOSITY);
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  // note: assumes topoCluster input already configured
  ParticleFlowReco *pfr = new ParticleFlowReco();
  pfr->set_energy_match_Nsigma(1.5);
  pfr->Verbosity(verbosity);
  se->registerSubsystem(pfr);

  JetReco *particleflowjetreco = new JetReco("PARTICLEFLOWJETRECO");
  particleflowjetreco->add_input(new ParticleFlowJetInput());
  particleflowjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.2), "AntiKt_ParticleFlow_r02");
  particleflowjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_ParticleFlow_r03");
  particleflowjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.4), "AntiKt_ParticleFlow_r04");
  particleflowjetreco->add_algo(new FastJetAlgo(Jet::ANTIKT, 0.3), "AntiKt_ParticleFlow_r03");
  particleflowjetreco->set_algo_node("ANTIKT");
  particleflowjetreco->set_input_node("PARTICLEFLOW");
  particleflowjetreco->Verbosity(verbosity);
  se->registerSubsystem(particleflowjetreco);

  return;
}
#endif
