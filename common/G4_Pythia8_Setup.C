#ifndef MACRO_G4PYTHIA8SETUP_C
#define MACRO_G4PYTHIA8SETUP_C

#include <G4_Input.C>

#include <phpythia8/PHPy8JetTrigger.h>
#include <phpythia8/PHPy8ParticleTrigger.h>


void Pythia8_Generator_Setup(const std::string &jettrigger, const int index = 0)
{
  if (! Input::PYTHIA8)
  {
    return;
  }
  std::string pythia8_config_file = std::string(getenv("CALIBRATIONROOT")) + "/Generators/JetStructure_TG/";
  if (jettrigger == "PhotonJet")
  {
    pythia8_config_file += "phpythia8_JS_GJ_MDC2.cfg";
  }
  else if (jettrigger == "PhotonJet3" || jettrigger == "PhotonJet5")
  {
    pythia8_config_file += "phpythia8_5GeV_JS_MDC2.cfg"; // 5GeV config on purpose
  }
  else if (jettrigger == "PhotonJet10")
  {
    pythia8_config_file += "phpythia8_10GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Jet20" || jettrigger == "PhotonJet20")
  {
    pythia8_config_file += "phpythia8_20GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Jet30")
  {
    pythia8_config_file += "phpythia8_30GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Jet40")
  {
    pythia8_config_file += "phpythia8_40GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Jet50")
  {
    pythia8_config_file += "phpythia8_50GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Jet60")
  {
    pythia8_config_file += "phpythia8_60GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Jet70")
  {
    pythia8_config_file += "phpythia8_70GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Jet80")
  {
    pythia8_config_file += "phpythia8_80GeV_JS_MDC2.cfg";
  }
  else if (jettrigger == "Detroit" || jettrigger == "Jet3" || jettrigger == "Jet5" || jettrigger == "Jet8" || jettrigger == "Jet10" || jettrigger == "Jet12" || jettrigger == "Jet15" || jettrigger == "Pi03" || jettrigger == "Pi08" || jettrigger == "Eta3" || jettrigger == "Eta8")
  {
    pythia8_config_file = std::string(getenv("CALIBRATIONROOT")) + "/Generators/phpythia8_detroit_minBias.cfg";
  }
  else
  {
    std::cout << "Invalid jet trigger " << jettrigger << std::endl;
    gSystem->Exit(1);
  }
  PYTHIA8::config_file[index] = pythia8_config_file;
}

void Pythia8_Trigger_Setup(const std::string &jettrigger, const int index = 0)
{
  if (! Input::PYTHIA8)
  {
    return;
  }
  if (jettrigger.find("PhotonJet") != std::string::npos)
  {
    PHPy8ParticleTrigger *p8_photon_jet_trigger = new PHPy8ParticleTrigger();
    p8_photon_jet_trigger->SetStableParticleOnly(false);  // process unstable particles that include quarks
    p8_photon_jet_trigger->AddParticles(22);
    p8_photon_jet_trigger->SetEtaHighLow(1.5, -1.5);  // sample a rapidity range higher than the sPHENIX tracking pseudorapidity
    std::vector<int> partentsId{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -22, -21, -20, -19, -18, -17, -16, -15, -14, -13, -12, -11, -10, -9, -8, -7, -6, -5, -4, -3, -2, -1};
    p8_photon_jet_trigger->AddParents(partentsId);
    if (jettrigger == "PhotonJet3")
    {
      p8_photon_jet_trigger->SetPtLow(3);
    }
    else if (jettrigger == "PhotonJet5")
    {
      p8_photon_jet_trigger->SetPtLow(5);
    }
    else if (jettrigger == "PhotonJet10")
    {
      p8_photon_jet_trigger->SetPtLow(10);
    }
    else if (jettrigger == "PhotonJet20")
    {
      p8_photon_jet_trigger->SetPtLow(20);
    }
    else
    {
      std::cout << "invalid jettrigger: " << jettrigger << std::endl;
      gSystem->Exit(1);
    }
    INPUTGENERATOR::Pythia8[index]->register_trigger(p8_photon_jet_trigger);
    INPUTGENERATOR::Pythia8[index]->set_trigger_OR();
  }
  else if (jettrigger.find("Jet") != std::string::npos)
  {
    PHPy8JetTrigger *p8_js_signal_trigger = new PHPy8JetTrigger();
    p8_js_signal_trigger->SetEtaHighLow(1.5, -1.5);  // Set eta acceptance for particles into the jet between +/- 1.5
    p8_js_signal_trigger->SetJetR(0.4);              // Set the radius for the trigger jet

    if (jettrigger == "Jet3")
    {
      p8_js_signal_trigger->SetMinJetPt(3);  // require a 3 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet5")
    {
      p8_js_signal_trigger->SetMinJetPt(5);  // require a 5 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet8")
    {
      p8_js_signal_trigger->SetMinJetPt(8);  // require a 8 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet10")
    {
      p8_js_signal_trigger->SetMinJetPt(10);  // require a 10 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet12")
    {
      p8_js_signal_trigger->SetMinJetPt(12);  // require a 12 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet15")
    {
      p8_js_signal_trigger->SetMinJetPt(15);  // require a 15 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet20")
    {
      p8_js_signal_trigger->SetMinJetPt(20);  // require a 20 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet30")
    {
      p8_js_signal_trigger->SetMinJetPt(30);  // require a 30 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet40")
    {
      p8_js_signal_trigger->SetMinJetPt(40);  // require a 40 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet50")
    {
      p8_js_signal_trigger->SetMinJetPt(50);  // require a 50 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet60")
    {
      p8_js_signal_trigger->SetMinJetPt(60);  // require a 60 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet70")
    {
      p8_js_signal_trigger->SetMinJetPt(70);  // require a 70 GeV minimum pT jet in the event
    }
    else if (jettrigger == "Jet80")
    {
      p8_js_signal_trigger->SetMinJetPt(80);  // require a 80 GeV minimum pT jet in the event
    }
    else
    {
      std::cout << "invalid jettrigger: " << jettrigger << std::endl;
      gSystem->Exit(1);
    }
    INPUTGENERATOR::Pythia8[index]->register_trigger(p8_js_signal_trigger);
    INPUTGENERATOR::Pythia8[index]->set_trigger_AND();
  }
  else if (jettrigger == "Detroit")
  {
    std::cout << "using detroit - no cuts" << std::endl;
  }
  else if (jettrigger.find("Pi0") != std::string::npos)
  {
    PHPy8ParticleTrigger *p8_photon_jet_trigger = new PHPy8ParticleTrigger();
    p8_photon_jet_trigger->SetStableParticleOnly(true);  // process unstable particles that include quarks
    p8_photon_jet_trigger->AddParticles(111);
    p8_photon_jet_trigger->SetEtaHighLow(1.5, -1.5);  // sample a rapidity range higher than the sPHENIX tracking pseudorapidity
    if (jettrigger == "Pi03")
    {
      p8_photon_jet_trigger->SetPtLow(3);
    }
    else if (jettrigger == "Pi08")
    {
      p8_photon_jet_trigger->SetPtLow(8);
    }
    else
    {
      std::cout << "invalid jettrigger: " << jettrigger << std::endl;
      gSystem->Exit(1);
    }
    INPUTGENERATOR::Pythia8[index]->register_trigger(p8_photon_jet_trigger);
    INPUTGENERATOR::Pythia8[index]->set_trigger_OR();
  }
  else if (jettrigger.find("Eta") != std::string::npos)
  {
    PHPy8ParticleTrigger *p8_photon_jet_trigger = new PHPy8ParticleTrigger();
    p8_photon_jet_trigger->SetStableParticleOnly(true);  // process unstable particles that include quarks
    p8_photon_jet_trigger->AddParticles(221);
    p8_photon_jet_trigger->SetEtaHighLow(1.5, -1.5);  // sample a rapidity range higher than the sPHENIX tracking pseudorapidity
    if (jettrigger == "Eta3")
    {
      p8_photon_jet_trigger->SetPtLow(3);
    }
    else if (jettrigger == "Eta8")
    {
      p8_photon_jet_trigger->SetPtLow(8);
    }
    else
    {
      std::cout << "invalid jettrigger: " << jettrigger << std::endl;
      gSystem->Exit(1);
    }
    INPUTGENERATOR::Pythia8[index]->register_trigger(p8_photon_jet_trigger);
    INPUTGENERATOR::Pythia8[index]->set_trigger_OR();
  }
  else
  {
    std::cout << "Invalid jettrigger for cuts " << jettrigger << std::endl;
    gSystem->Exit(1);
  }
  Input::ApplysPHENIXBeamParameter(INPUTGENERATOR::Pythia8);
}

#endif // MACRO_G4PYTHIA8SETUP_C
