#ifndef MACRO_G4RUNSETTINGS
#define MACRO_G4RUNSETTINGS

#include <GlobalVariables.C>

#include <G4_Input.C>

void RunSettings(int runnumber, const std::string & /*type*/ = "")
{
  switch (runnumber)
  {
  case 21:  // zero beam xing angle, mvtx rotated
  case 28:  // zero beam xing angle, mvtx rotated
    Input::BEAM_CONFIGURATION = Input::pp_ZEROANGLE;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    break;
  case 22:  // 1.5mRad xing angle, mvtx rotated
  case 29:  // 1.5mRad xing angle, mvtx rotated
    Input::BEAM_CONFIGURATION = Input::pp_COLLISION;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    break;
  case 23:  // AuAu 1mRad xing angle, mvtx rotated
  case 30:  // AuAu 1mRad xing angle, mvtx rotated
    Input::BEAM_CONFIGURATION = Input::AA_COLLISION;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    break;
  case 24:  // single particle sims
    break;
  case 25:  // run 25 AuAu: field off
    G4MAGNET::magfield = "0";
    break;
  case 26:  // run 26 ppg02 sims: field off, ppg02 beam settings
    Input::BEAM_CONFIGURATION = Input::ppg02;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    G4MAGNET::magfield = "0";
    std::cout << "use ppg02 settings" << std::endl;
    break;
  case 27:  // run 27 ppg02 sims: field off, ppg02 beam settings 40% strangeness fraction
    Input::BEAM_CONFIGURATION = Input::ppg02;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    G4MAGNET::magfield = "0";
    INPUTHEPMC::HEPMC_STRANGENESS_FRACTION = 40.;
    std::cout << "use ppg02 settings" << std::endl;
    break;
  case 31:  // run 31 ppg08, AuAu 1mRad xing angle, mvtx rotated, flow flucuations enabled
    Input::BEAM_CONFIGURATION = Input::AA_COLLISION;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    INPUTHEPMC::FLOW_FLUCTUATIONS = true;
    INPUTHEPMC::FLOW_SCALING = 1.0;
    std::cout << "use ppg08 run31 settings" << std::endl;
    break;
  case 32:  // run 32 ppg08, AuAu 1mRad xing angle, mvtx rotated, flow fluctuations disabled, scale 2
    Input::BEAM_CONFIGURATION = Input::AA_COLLISION;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    INPUTHEPMC::FLOW_FLUCTUATIONS = false;
    INPUTHEPMC::FLOW_SCALING = 2.0;
    std::cout << "use ppg08 run32 settings" << std::endl;
    break;
  case 33:  // run 33 ppg08, AuAu 1mRad xing angle, mvtx rotated, flow afterburner disabled
    Input::BEAM_CONFIGURATION = Input::AA_COLLISION;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    INPUTHEPMC::FLOW_FLUCTUATIONS = false;
    INPUTHEPMC::FLOW = false;
    INPUTHEPMC::FLOW_SCALING = 0.0;
    std::cout << "use ppg08 run33 settings" << std::endl;
    break;
  default:
    std::cout << "runnnumber " << runnumber << " not implemented" << std::endl;
    gSystem->Exit(1);
    break;
  }
  return;
}

#endif
