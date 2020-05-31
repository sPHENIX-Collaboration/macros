#pragma once

#include "GlobalVariables.C"

#include "G4_Input.C"

#include <g4main/PHG4ParticleGun.h>

#include <fun4all/Fun4AllServer.h>

#include <map>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)

namespace Input
{
  bool GUN = false;
  bool GUN_VERBOSITY = 0;
}

namespace INPUTGUN
{
  double vx = 0.;
  double vy = 0.;
  double vz = 0.;
  deque<tuple<string,double,double,double>> particles;
  void AddParticle(const string &name, const double px, const double py, const double pz);
}

void INPUTGUN::AddParticle(const string &name, const double px, const double py, const double pz)
{
  particles.push_back(make_tuple(name,px,py,pz));
}


void InputGunInit()
{
  if (INPUTGUN::particles.empty())
  {
    cout << "Input::GUN: particle map is empty use for e.g. 1 pi-" << endl;
    cout << "INPUTGUN::particles.push_back(make_tuple(\"pi-\",0,1,0));" << endl; 
    gSystem->Exit(-1);
  }
  Fun4AllServer *se = Fun4AllServer::instance();
    // toss low multiplicity dummy events
      PHG4ParticleGun *gun = new PHG4ParticleGun();
      for (auto iter =  INPUTGUN::particles.begin(); iter != INPUTGUN::particles.end(); ++iter)
      {
	gun->AddParticle(get<0>(*iter),get<1>(*iter),get<2>(*iter),get<3>(*iter));
      }   
    gun->Verbosity(Input::GUN_VERBOSITY);
    se->registerSubsystem(gun);
}
