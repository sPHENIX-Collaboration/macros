#ifndef MACRO_G4HERWIG7SETUP_C
#define MACRO_G4HERWIG7SETUP_C

#include <G4_Input.C>

#include <phherwig7/PHHerwig7ParticleTrigger.h>

void Herwig7_Generator_Setup(const std::string &jettrigger, const int index = 0)
{
  if (! Input::HERWIG7)
  {
    return;
  }
  std::cout << "start h setup" << std::endl;
  std::string herwig7_config_dir = std::string(getenv("CALIBRATIONROOT")) + "/Generators/Herwig7/in/";
  std::string herwig7_config_file;
  if (jettrigger == "PhotonJet5")
  {
    herwig7_config_file = "Herwig_Photon5_direct.in";
  }
  else if (jettrigger == "PhotonJet5Frag")
  {
    herwig7_config_file = "Herwig_Photon5_frag.in";
  }
  else if (jettrigger == "PhotonJet10")
  {
    herwig7_config_file = "Herwig_Photon10_direct.in";
  }
  else if (jettrigger == "PhotonJet10Frag")
  {
    herwig7_config_file = "Herwig_Photon10_frag.in";
  }
  else if (jettrigger == "PhotonJet20")
  {
    herwig7_config_file = "Herwig_Photon20_direct.in";
  }
  else if (jettrigger == "PhotonJet20Frag")
  {
    herwig7_config_file = "Herwig_Photon20_frag.in";
  }
  else
  {
    std::cout << "Invalid jet trigger " << jettrigger << std::endl;
    gSystem->Exit(1);
  }
  std::cout << "using config " << herwig7_config_dir << herwig7_config_file << std::endl;
  HERWIG7::config_file[index] = herwig7_config_dir + herwig7_config_file;
}

void Herwig7_Trigger_Setup(const std::string &jettrigger, const int index = 0)
{
  if (! Input::HERWIG7)
  {
    return;
  }
  PHHerwig7ParticleTrigger *photonTrigger = new PHHerwig7ParticleTrigger(jettrigger);
  photonTrigger->SetStableParticleOnly(true);
  photonTrigger->SetPromptOnly(true);
  photonTrigger->AddParticles(22);
  photonTrigger->SetEtaHighLow(1.5, -1.5);
  if (jettrigger.find("PhotonJet5") != std::string::npos)
  {
    photonTrigger->SetPtLow(5);
  }
  else if (jettrigger.find("PhotonJet10") != std::string::npos)
  {
    photonTrigger->SetPtLow(10);
  }
  else if (jettrigger.find("PhotonJet20") != std::string::npos)
  {
    photonTrigger->SetPtLow(20);
  }
  else
  {
    std::cout << "Invalid jet trigger " << jettrigger << std::endl;
    gSystem->Exit(1);
  }
  photonTrigger->PrintConfig();
  INPUTGENERATOR::Herwig7[index]->register_trigger(photonTrigger);
  //! Nominal collision geometry is selected by Input::BEAM_CONFIGURATION
  Input::ApplysPHENIXBeamParameter(INPUTGENERATOR::Herwig7);
}

#endif // MACRO_G4HERWIG7SETUP_C
