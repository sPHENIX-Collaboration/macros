#pragma once

#include "GlobalVariables.C"

#include "G4_Input.C"

#include <g4main/PHG4SimpleEventGenerator.h>

#include <fun4all/Fun4AllServer.h>

#include <map>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)

namespace Input
{
  bool SIMPLE = false;
  int SIMPLE_VERBOSITY = 0;
}  // namespace Input

namespace INPUTSIMPLE
{
  double vxmean = 0.;
  double vymean = 0.;
  double vzmean = 0.;
  double vxwidth = 0.;
  double vywidth = 0.;
  double vzwidth = 5.;
  double etamin = -1.;
  double etamax = 3.;
  double phimin = -1. * M_PI;
  double phimax = 1. * M_PI;
  double ptmin = 0.5;
  double ptmax = 50.;
  double pmin = NAN;
  double pmax = NAN;
  map<string, unsigned int> particles;
  void AddParticle(const string &name, const unsigned int num);
  void set_p_range(double d1, double d2);
  void set_pt_range(double d1, double d2);

  void set_eta_range(double d1, double d2)
  {
    etamin = d1;
    etamax = d2;
  }

  void set_phi_range(double d1, double d2)
  {
    phimin = d1;
    phimax = d2;
  }

  void set_vtx_mean(double vx, double vy, double vz)
  {
    vxmean = vx;
    vymean = vy;
    vzmean = vz;
  }
  void set_vtx_width(double vx, double vy, double vz)
  {
    vxwidth = vx;
    vywidth = vy;
    vzwidth = vz;
  }
}  // namespace INPUTSIMPLE

void INPUTSIMPLE::AddParticle(const string &name, const unsigned int num)
{
  if (num == 0)
  {
    cout << "INPUTSIMPLE::AddParticle(\"" << name << "\"," << num
         << "): number of " << name << " has to be > 0" << endl;
    return;
  }
  auto iter = particles.find(name);
  if (iter != particles.end())
  {
    iter->second += num;
  }
  else
  {
    particles.insert(make_pair(name, num));
  }
}

void INPUTSIMPLE::set_p_range(double d1, double d2)
{
  pmin = d1;
  pmax = d2;
  ptmin = NAN;
  ptmax = NAN;
}

void INPUTSIMPLE::set_pt_range(double d1, double d2)
{
  pmin = NAN;
  pmax = NAN;
  ptmin = d1;
  ptmax = d2;
}

void InputSimpleInit()
{
  if (INPUTSIMPLE::particles.empty())
  {
    cout << "Input::SIMPLE: particle map is empty use for e.g. 5 pi-" << endl;
    cout << "INPUTSIMPLE::particles[\"pi-\"] = 5;" << endl;
    gSystem->Exit(-1);
  }
  Fun4AllServer *se = Fun4AllServer::instance();
  // toss low multiplicity dummy events
  PHG4SimpleEventGenerator *gen = new PHG4SimpleEventGenerator();
  for (map<string, unsigned int>::const_iterator iter = INPUTSIMPLE::particles.begin();
       iter != INPUTSIMPLE::particles.end(); ++iter)
  {
    gen->add_particles(iter->first, iter->second);
  }
  if (Input::HEPMC || Input::EMBED)
  {
    gen->set_reuse_existing_vertex(true);
    gen->set_existing_vertex_offset_vector(0.0, 0.0, 0.0);
  }
  else
  {
    gen->set_vertex_distribution_function(PHG4SimpleEventGenerator::Uniform,
                                          PHG4SimpleEventGenerator::Uniform,
                                          PHG4SimpleEventGenerator::Uniform);
    gen->set_vertex_distribution_mean(INPUTSIMPLE::vxmean, INPUTSIMPLE::vymean, INPUTSIMPLE::vzmean);
    gen->set_vertex_distribution_width(INPUTSIMPLE::vxwidth, INPUTSIMPLE::vywidth, INPUTSIMPLE::vzwidth);
  }
  gen->set_vertex_size_function(PHG4SimpleEventGenerator::Uniform);
  gen->set_vertex_size_parameters(0.0, 0.0);
  gen->set_eta_range(INPUTSIMPLE::etamin, INPUTSIMPLE::etamax);
  gen->set_phi_range(INPUTSIMPLE::phimin, INPUTSIMPLE::phimax);
  if (isfinite(INPUTSIMPLE::ptmin) && isfinite(INPUTSIMPLE::ptmax))
  {
    gen->set_pt_range(INPUTSIMPLE::ptmin, INPUTSIMPLE::ptmax);
  }
  else if (isfinite(INPUTSIMPLE::pmin) && isfinite(INPUTSIMPLE::pmax))
  {
    gen->set_p_range(INPUTSIMPLE::pmin, INPUTSIMPLE::pmax);
  }
  gen->Embed(1);
  gen->Verbosity(Input::SIMPLE_VERBOSITY);
  se->registerSubsystem(gen);
}
