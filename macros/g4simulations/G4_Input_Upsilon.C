#pragma once

#include "GlobalVariables.C"

#include "G4_Input.C"

#include <g4main/PHG4ParticleGeneratorVectorMeson.h>

#include <fun4all/Fun4AllServer.h>

#include <map>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)

namespace Input
{
  bool UPSILON = false;
  bool UPSILON_VERBOSITY = 0;
}  // namespace Input

namespace INPUTUPSILON
{
  int istate = 1;
  double etamin = -1.;
  double etamax = 1.;
  double ptmin = 0.;
  double ptmax = 10.;
  deque<tuple<string, string, int>> particles;
  void AddDecayParticles(const string &name1, const string &name2, const int decay_id);
  void set_rapidity_range(double min, double max)
  {
    etamin = min;
    etamax = max;
  }
  void set_pt_range(double min, double max)
  {
    ptmin = min;
    ptmax = max;
  }
}  // namespace INPUTUPSILON

void INPUTUPSILON::AddDecayParticles(const string &name1, const string &name2, const int decay_id)
{
  particles.push_back(make_tuple(name1, name2, decay_id));
}

void InputUpsilonInit()
{
  if (INPUTUPSILON::particles.empty())
  {
    cout << "Input::UPSILON: particle map is empty use for e.g. e+/e-" << endl;
    cout << "INPUTUPSILON::AddDecayParticle(\"e+\", \"e-\", 0);" << endl;
    gSystem->Exit(-1);
  }
  Fun4AllServer *se = Fun4AllServer::instance();
  PHG4ParticleGeneratorVectorMeson *vgen = new PHG4ParticleGeneratorVectorMeson();
  for (auto iter = INPUTUPSILON::particles.begin(); iter != INPUTUPSILON::particles.end(); ++iter)
  {
    vgen->add_decay_particles(get<0>(*iter), get<1>(*iter), get<2>(*iter));
  }
  // event vertex
  if (Input::HEPMC || Input::SIMPLE)
  {
    vgen->set_reuse_existing_vertex(true);
  }

  vgen->set_rapidity_range(INPUTUPSILON::etamin, INPUTUPSILON::etamax);
  vgen->set_pt_range(INPUTUPSILON::ptmin, INPUTUPSILON::ptmax);

  if (INPUTUPSILON::istate == 1)
  {
    // Upsilon(1S)
    vgen->set_mass(9.46);
    vgen->set_width(54.02e-6);
  }
  else if (INPUTUPSILON::istate == 2)
  {
    // Upsilon(2S)
    vgen->set_mass(10.0233);
    vgen->set_width(31.98e-6);
  }
  else
  {
    // Upsilon(3S)
    vgen->set_mass(10.3552);
    vgen->set_width(20.32e-6);
  }

  vgen->Verbosity(Input::UPSILON_VERBOSITY);
  vgen->Embed(2);
  se->registerSubsystem(vgen);
}
