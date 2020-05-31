#pragma once

#include "GlobalVariables.C"

#include <g4main/PHG4SimpleEventGenerator.h>
#include <phpythia6/PHPythia6.h>

#include <phpythia8/PHPythia8.h>

#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libPHPythia6.so)
R__LOAD_LIBRARY(libPHPythia8.so)

namespace Input
{
  bool READHITS = false;
  bool PYTHIA6 = false;
  bool PYTHIA8 = false;
  bool GUN = false;
}

#include "G4_Input_Simple.C"

void InputInit()
{
// first consistency checks - not all input generators play nice
// with each other

  Fun4AllServer *se = Fun4AllServer::instance();
  if (Input::PYTHIA6)
  {
    PHPythia6 *pythia6 = new PHPythia6();
    pythia6->set_config_file("phpythia6.cfg");
    se->registerSubsystem(pythia6);
  }
  if (Input::PYTHIA8)
  {
    PHPythia8 *pythia8 = new PHPythia8();
    // see coresoftware/generators/PHPythia8 for example config
    pythia8->set_config_file("phpythia8.cfg");
    se->registerSubsystem(pythia8);
  }
  if (Input::SIMPLE)
  {
    InputSimpleInit();
  }

}

void InputManagers()
{
  Fun4AllServer *se = Fun4AllServer::instance();
    Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
    se->registerInputManager(in);
}
