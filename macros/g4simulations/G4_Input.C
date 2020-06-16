#pragma once

#include "GlobalVariables.C"

#include "G4_Input_Gun.C"
#include "G4_Input_Simple.C"

#include <phpythia6/PHPythia6.h>

#include <phpythia8/PHPythia8.h>

#include <phhepmc/Fun4AllHepMCInputManager.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
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
}  // namespace Input

namespace INPUTHEPMC
{
  string filename;
}

namespace INPUTREADHITS
{
  string filename;
}

namespace INPUTEMBED
{
  string filename;
}

void InputInit()
{
  // first consistency checks - not all input generators play nice
  // with each other
  if (Input::READHITS && Input::EMBED)
  {
    cout << "Reading Hits and Embedding into background at the same time is not supported" << endl;
    gSystem->Exit(1);
  }
  if (Input::PYTHIA6 && Input::PYTHIA8)
  {
    cout << "Pythia6 and Pythia8 cannot be run together - might be possible but needs R&D" << endl;
    gSystem->Exit(1);
  }

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
  if (Input::GUN)
  {
    InputGunInit();
  }
}

void InputManagers()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  if (Input::EMBED)
  {
    gSystem->Load("libg4dst.so");
    Fun4AllDstInputManager *in1 = new Fun4AllNoSyncDstInputManager("DSTinEmbed");
    in1->AddFile(INPUTEMBED::filename);  // if one use a single input file
    in1->Repeat();                       // if file(or filelist) is exhausted, start from beginning
    se->registerInputManager(in1);
  }
  if (Input::HEPMC)
  {
    Fun4AllInputManager *in = new Fun4AllHepMCInputManager("HEPMCin");
    in->Verbosity(Input::HEPMC_VERBOSITY);
    se->registerInputManager(in);
    se->fileopen(in->Name(), INPUTHEPMC::filename);
  }
  else if (Input::READHITS)
  {
    Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
    hitsin->fileopen(INPUTREADHITS::filename);
    hitsin->Verbosity(1);
    se->registerInputManager(hitsin);
  }
  else
  {
    Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
    in->Verbosity(1);
    se->registerInputManager(in);
  }
}
