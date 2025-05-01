#ifndef MACRO_G4RUNSETTINGS
#define MACRO_G4RUNSETTINGS

#include <GlobalVariables.C>

#include <G4_Input.C>

void RunSettings(int runnumber, const std::string type = "")
{
  switch (runnumber)
  {
  case 21:  // zero beam xing angle, mvtx rotated
    Input::BEAM_CONFIGURATION = Input::pp_ZEROANGLE;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    break;
  case 22:  // 1.5mRad xing angle, mvtx rotated
    Input::BEAM_CONFIGURATION = Input::pp_COLLISION;
    Enable::MVTX_APPLYMISALIGNMENT = true;
    break;
  case 23:  // AuAu 1mRad xing angle, mvtx rotated
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
  default:
    cout << "runnnumber " << runnumber << " not implemented" << endl;
    gSystem->Exit(1);
    break;
  }
  return;
}

#endif
