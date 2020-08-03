#ifndef MACRO_G4INPUT_C
#define MACRO_G4INPUT_C

#include "GlobalVariables.C"

#include <phpythia6/PHPythia6.h>

#include <phpythia8/PHPythia8.h>

#include <g4main/HepMCNodeReader.h>
#include <g4main/PHG4ParticleGeneratorVectorMeson.h>
#include <g4main/PHG4ParticleGun.h>
#include <g4main/PHG4SimpleEventGenerator.h>
#include <g4main/ReadEICFiles.h>

#include <phhepmc/Fun4AllHepMCInputManager.h>
#include <phhepmc/Fun4AllHepMCPileupInputManager.h>

#include <phsartre/PHSartre.h>
#include <phsartre/PHSartreParticleTrigger.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDummyInputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllNoSyncDstInputManager.h>
#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libg4testbench.so)
R__LOAD_LIBRARY(libPHPythia6.so)
R__LOAD_LIBRARY(libPHPythia8.so)
R__LOAD_LIBRARY(libPHSartre.so)

namespace Input
{
  bool GUN = false;
  int GUN_VERBOSITY = 0;
  bool READHITS = false;
  bool PYTHIA6 = false;
  bool PYTHIA8 = false;
  bool SARTRE = false;
  bool SIMPLE = false;
  int SIMPLE_VERBOSITY = 0;
  bool UPSILON = false;
  int UPSILON_VERBOSITY = 0;
  double PILEUPRATE = 0.;
  int VERBOSITY = 0;
}  // namespace Input

namespace INPUTHEPMC
{
  string filename;
}

namespace INPUTREADEIC
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

namespace PYTHIA6
{
  string config_file = string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia6.cfg";
}

namespace PYTHIA8
{
  string config_file = string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia8.cfg";
}

namespace SARTRE
{
  string config_file = string(getenv("CALIBRATIONROOT")) + "/Generators/sartre.cfg";
}

namespace PILEUP
{
  string pileupfile = "/sphenix/sim/sim01/sHijing/sHijing_0-12fm.dat";
  double TpcDriftVelocity = 8.0 / 1000.0;
}  // namespace PILEUP

// collection of pointers to particle generators we can grab in the Fun4All macro
namespace INPUTGENERATOR
{
  PHG4ParticleGeneratorVectorMeson *VectorMesonGenerator = nullptr;
  PHG4SimpleEventGenerator *SimpleEventGenerator = nullptr;
  PHG4ParticleGun *Gun = nullptr;
  PHPythia6 *Pythia6 = nullptr;
  PHPythia8 *Pythia8 = nullptr;
  PHSartre *Sartre = nullptr;
  PHSartreParticleTrigger *SartreTrigger = nullptr;
}  // namespace INPUTGENERATOR

namespace INPUTMANAGER
{
  Fun4AllHepMCInputManager *HepMCInputManager = nullptr;
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
    INPUTGENERATOR::Pythia6 = new PHPythia6();
    INPUTGENERATOR::Pythia6->set_config_file(PYTHIA6::config_file);
  }
  if (Input::PYTHIA8)
  {
    INPUTGENERATOR::Pythia8 = new PHPythia8();
    // see coresoftware/generators/PHPythia8 for example config
    INPUTGENERATOR::Pythia8->set_config_file(PYTHIA8::config_file);
  }
  if (Input::SARTRE)
  {
    INPUTGENERATOR::Sartre = new PHSartre();
    INPUTGENERATOR::Sartre->set_config_file(SARTRE::config_file);
    // particle trigger to enhance forward J/Psi -> ee
    INPUTGENERATOR::SartreTrigger = new PHSartreParticleTrigger("MySartreTrigger");
    INPUTGENERATOR::SartreTrigger->AddParticles(-11);
    //INPUTGENERATOR::SartreTrigger->SetEtaHighLow(4.0,1.4);
    INPUTGENERATOR::SartreTrigger->SetEtaHighLow(1.0, -1.1);  // central arm
    INPUTGENERATOR::SartreTrigger->PrintConfig();
  }

  if (Input::SIMPLE)
  {
    INPUTGENERATOR::SimpleEventGenerator = new PHG4SimpleEventGenerator();
  }
  if (Input::GUN)
  {
    INPUTGENERATOR::Gun = new PHG4ParticleGun();
  }
  if (Input::UPSILON)
  {
    INPUTGENERATOR::VectorMesonGenerator = new PHG4ParticleGeneratorVectorMeson();
  }
  // input managers for which we might need to set options
  if (Input::HEPMC)
  {
    INPUTMANAGER::HepMCInputManager = new Fun4AllHepMCInputManager("HEPMCin");
  }
}

void InputRegister()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  if (Input::PYTHIA6)
  {
    se->registerSubsystem(INPUTGENERATOR::Pythia6);
  }
  if (Input::PYTHIA8)
  {
    se->registerSubsystem(INPUTGENERATOR::Pythia8);
  }
  if (Input::SARTRE)
  {
    INPUTGENERATOR::Sartre->register_trigger((PHSartreGenTrigger *) INPUTGENERATOR::SartreTrigger);
    se->registerSubsystem(INPUTGENERATOR::Sartre);
  }
  if (Input::SIMPLE)
  {
    INPUTGENERATOR::SimpleEventGenerator->Verbosity(Input::SIMPLE_VERBOSITY);
    se->registerSubsystem(INPUTGENERATOR::SimpleEventGenerator);
  }
  if (Input::UPSILON)
  {
    if (Input::HEPMC || Input::SIMPLE)
    {
      INPUTGENERATOR::VectorMesonGenerator->set_reuse_existing_vertex(true);
    }
    INPUTGENERATOR::VectorMesonGenerator->Verbosity(Input::UPSILON_VERBOSITY);
    INPUTGENERATOR::VectorMesonGenerator->Embed(2);
    se->registerSubsystem(INPUTGENERATOR::VectorMesonGenerator);
  }
  if (Input::GUN)
  {
    INPUTGENERATOR::Gun->Verbosity(Input::GUN_VERBOSITY);
    se->registerSubsystem(INPUTGENERATOR::Gun);
  }
  if (Input::READEIC)
  {
    ReadEICFiles *eicr = new ReadEICFiles();
    eicr->OpenInputFile(INPUTREADEIC::filename);
    se->registerSubsystem(eicr);
  }
  // here are the various utility modules which read particles and
  // put them onto the G4 particle stack
  if (Input::HEPMC or Input::PYTHIA8 or Input::PYTHIA6 or Input::READEIC)
  {
    // read-in HepMC events to Geant4 if there is any
    HepMCNodeReader *hr = new HepMCNodeReader();
    se->registerSubsystem(hr);
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
    INPUTMANAGER::HepMCInputManager->Verbosity(Input::VERBOSITY);
    se->registerInputManager(INPUTMANAGER::HepMCInputManager);
    se->fileopen(INPUTMANAGER::HepMCInputManager->Name(), INPUTHEPMC::filename);
  }
  else if (Input::READHITS)
  {
    Fun4AllInputManager *hitsin = new Fun4AllDstInputManager("DSTin");
    hitsin->fileopen(INPUTREADHITS::filename);
    hitsin->Verbosity(Input::VERBOSITY);
    se->registerInputManager(hitsin);
  }
  else
  {
    Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
    in->Verbosity(Input::VERBOSITY);
    se->registerInputManager(in);
  }
  if (Input::PILEUPRATE > 0)
  {
    Fun4AllHepMCPileupInputManager *pileup = new Fun4AllHepMCPileupInputManager("HepMCPileupInput");
    pileup->Verbosity(Input::VERBOSITY);
    pileup->AddFile(PILEUP::pileupfile);
    pileup->set_collision_rate(Input::PILEUPRATE);
    double time_window = 105.5 / PILEUP::TpcDriftVelocity;
    pileup->set_time_window(-time_window, time_window);
    se->registerInputManager(pileup);
  }
}
#endif
