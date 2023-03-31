#ifndef MACRO_G4INTT_C
#define MACRO_G4INTT_C

#include <GlobalVariables.C>
#include <QA.C>

#include <G4_Mvtx.C>

#include <g4intt/PHG4InttDeadMapLoader.h>
#include <g4intt/PHG4InttDefs.h>
#include <g4intt/PHG4InttDigitizer.h>
#include <g4intt/PHG4InttHitReco.h>
#include <g4intt/PHG4InttSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <intt/InttClusterizer.h>
#include <qa_modules/QAG4SimulationIntt.h>

#include <fun4all/Fun4AllServer.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4intt.so)
R__LOAD_LIBRARY(libintt.so)
R__LOAD_LIBRARY(libqa_modules.so)

void InttInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 20.);  // estimated from display, can be made smaller but good enough
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 410. / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -410. / 2.);
  // the mvtx is not called if disabled but the default number of layers is set to 3, so we need to set it
  // to zero
  if (!Enable::MVTX)
  {
    G4MVTX::n_maps_layer = 0;
  }
}

double Intt(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0)
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  bool intt_overlapcheck = Enable::OVERLAPCHECK || Enable::INTT_OVERLAPCHECK;

  // instantiate the INTT subsystem and register it
  // We make one instance of PHG4INTTSubsystem for all four layers of tracker
  // dimensions are in mm, angles are in radians

  // PHG4InttSubsystem creates the detetor layer using PHG4InttDetector
  // and instantiates the appropriate PHG4SteppingAction

  // The length of vpair is used to determine the number of layers
  std::vector<std::pair<int, int>> vpair;  // (sphxlayer, inttlayer)
  for (int i = 0; i < G4INTT::n_intt_layer; i++)
  {
    // We want the sPHENIX layer numbers for the Intt to be from n_maps_layer to n_maps_layer+n_intt_layer - 1
    vpair.push_back(std::make_pair(G4MVTX::n_maps_layer + i, i));  // sphxlayer=n_maps_layer+i corresponding to inttlayer=i
    if (verbosity) cout << "Create strip tracker layer " << vpair[i].second << " as  sphenix layer  " << vpair[i].first << endl;
  }

  PHG4InttSubsystem* sitrack = new PHG4InttSubsystem("INTT", vpair);
  sitrack->Verbosity(verbosity);
  sitrack->SetActive(1);
  sitrack->OverlapCheck(intt_overlapcheck);
  if (Enable::INTT_ABSORBER)
  {
    sitrack->SetAbsorberActive();
  }
  if (Enable::INTT_SUPPORT)
  {
    sitrack->set_int_param(PHG4InttDefs::SUPPORTPARAMS, "supportactive", 1);
  }
  g4Reco->registerSubsystem(sitrack);

  // Set the laddertype and ladder spacing configuration

  cout << "Intt has " << G4INTT::n_intt_layer << " layers with layer setup:" << endl;
  for (int i = 0; i < G4INTT::n_intt_layer; i++)
  {
    cout << " Intt layer " << i << " laddertype " << G4INTT::laddertype[i] << " nladders " << G4INTT::nladder[i]
         << " sensor radius " << G4INTT::sensor_radius[i] << " offsetphi " << G4INTT::offsetphi[i] << endl;
    sitrack->set_int_param(i, "laddertype", G4INTT::laddertype[i]);
    sitrack->set_int_param(i, "nladder", G4INTT::nladder[i]);
    sitrack->set_double_param(i, "sensor_radius", G4INTT::sensor_radius[i]);  // expecting cm
    sitrack->set_double_param(i, "offsetphi", G4INTT::offsetphi[i]);          // expecting degrees
  }

  // outer radius marker (translation back to cm)
  radius = G4INTT::intt_radius_max * 0.1;
  return radius;
}

// Central detector cell reco is disabled as EIC setup use the fast tracking sim for now
void Intt_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  if (G4INTT::InttDeadMapOption != G4INTT::kInttNoDeadMap)
  {
    // Load pre-defined deadmaps
    PHG4InttDeadMapLoader* deadMapINTT = new PHG4InttDeadMapLoader("INTT");

    for (int i = 0; i < G4INTT::n_intt_layer; i++)
    {
      string DeadMapConfigName = Form("intt_layer%d/", i);

      if (G4INTT::InttDeadMapOption == G4INTT::kInttDeadMap)
      {
        string DeadMapPath = string(getenv("CALIBRATIONROOT")) + string("/Tracking/INTT/DeadMap/");
        //string DeadMapPath = "/sphenix/u/wxie/sphnx_software/INTT" + string("/DeadMap/");

        DeadMapPath += DeadMapConfigName;

        deadMapINTT->deadMapPath(G4MVTX::n_maps_layer + i, DeadMapPath);
      }
      else
      {
        cout << "G4_Intt.C - fatal error - invalid InttDeadMapOption = " << G4INTT::InttDeadMapOption << endl;
        exit(1);
      }
    }

    deadMapINTT->Verbosity(verbosity);
    //deadMapINTT -> Verbosity(1);
    se->registerSubsystem(deadMapINTT);
  }
  // new storage containers
  PHG4InttHitReco* reco = new PHG4InttHitReco();

  // The timing window defaults are set in the INTT ladder model, they can be overridden here
  double extended_readout_time = 0.0;
  if(TRACKING::pp_mode) extended_readout_time = TRACKING::pp_extended_readout_time;
  reco->set_double_param("tmax", 80.0 + extended_readout_time);
  reco->set_double_param("tmin", -20.0);
  std::cout << "INTT readout window is set to -20 to " << 80.0 + extended_readout_time << std::endl;
  reco->Verbosity(verbosity);
  se->registerSubsystem(reco);

  // Intt digitization
  //===========
  // these should be used for the Intt
  /*
     How threshold are calculated based on default FPHX settings
     Four part information goes to the threshold calculation:
     1. In 320 um thick silicon, the MIP e-h pair for a nominally indenting tracking is 3.87 MeV/cm * 320 um / 3.62 eV/e-h = 3.4e4 e-h pairs
     2. From DOI: 10.1016/j.nima.2014.04.017, FPHX integrator amplifier gain is 100mV / fC. That translate MIP voltage to 550 mV.
     3. From [FPHX Final Design Document](https://www.phenix.bnl.gov/WWW/fvtx/DetectorHardware/FPHX/FPHX2_June2009Revision.doc), the DAC0-7 setting for 8-ADC thresholds above the V_ref, as in Table 2 - Register Addresses and Defaults
     4, From [FPHX Final Design Document](https://www.phenix.bnl.gov/WWW/fvtx/DetectorHardware/FPHX/FPHX2_June2009Revision.doc) section Front-end Program Bits, the formula to translate DAC setting to comparitor voltages.
     The result threshold table based on FPHX default value is as following
     | FPHX Register Address | Name            | Default value | Voltage - Vref (mV) | To electrons based on calibration | Electrons | Fraction to MIP |
     |-----------------------|-----------------|---------------|---------------------|-----------------------------------|-----------|-----------------|
     | 4                     | Threshold DAC 0 | 8             | 32                  | 2500                              | 2000      | 5.85E-02        |
     | 5                     | Threshold DAC 1 | 16            | 64                  | 5000                              | 4000      | 1.17E-01        |
     | 6                     | Threshold DAC 2 | 32            | 128                 | 10000                             | 8000      | 2.34E-01        |
     | 7                     | Threshold DAC 3 | 48            | 192                 | 15000                             | 12000     | 3.51E-01        |
     | 8                     | Threshold DAC 4 | 80            | 320                 | 25000                             | 20000     | 5.85E-01        |
     | 9                     | Threshold DAC 5 | 112           | 448                 | 35000                             | 28000     | 8.18E-01        |
     | 10                    | Threshold DAC 6 | 144           | 576                 | 45000                             | 36000     | 1.05E+00        |
     | 11                    | Threshold DAC 7 | 176           | 704                 | 55000                             | 44000     | 1.29E+00        |
     DAC0-7 threshold as fraction to MIP voltage are set to PHG4InttDigitizer::set_adc_scale as 3-bit ADC threshold as fractions to MIP energy deposition.
     */
  std::vector<double> userrange;  // 3-bit ADC threshold relative to the mip_e at each layer.
  userrange.push_back(0.0584625322997416);
  userrange.push_back(0.116925064599483);
  userrange.push_back(0.233850129198966);
  userrange.push_back(0.35077519379845);
  userrange.push_back(0.584625322997416);
  userrange.push_back(0.818475452196383);
  userrange.push_back(1.05232558139535);
  userrange.push_back(1.28617571059432);

  // new containers
  PHG4InttDigitizer* digiintt = new PHG4InttDigitizer();
  digiintt->Verbosity(verbosity);
  //digiintt->Verbosity(3);
  for (int i = 0; i < G4INTT::n_intt_layer; i++)
  {
    digiintt->set_adc_scale(G4MVTX::n_maps_layer + i, userrange);
  }
  se->registerSubsystem(digiintt);

  return;
}

void Intt_Clustering()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::INTT_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();

  InttClusterizer* inttclusterizer = new InttClusterizer("InttClusterizer", G4MVTX::n_maps_layer, G4MVTX::n_maps_layer + G4INTT::n_intt_layer - 1);
  inttclusterizer->Verbosity(verbosity);
  inttclusterizer->set_cluster_version(G4TRACKING::cluster_version);
  // no Z clustering for Intt type 1 layers (we DO want Z clustering for type 0 layers)
  // turning off phi clustering for type 0 layers is not necessary, there is only one strip
  // per sensor in phi
  for (int i = G4MVTX::n_maps_layer; i < G4MVTX::n_maps_layer + G4INTT::n_intt_layer; i++)
  {
    if (G4INTT::laddertype[i - G4MVTX::n_maps_layer] == PHG4InttDefs::SEGMENTATION_PHI)
    {
      inttclusterizer->set_z_clustering(i, false);
    }
  }
  se->registerSubsystem(inttclusterizer);
}

void Intt_QA()
{
  int verbosity = std::max(Enable::QA_VERBOSITY, Enable::INTT_VERBOSITY);

  Fun4AllServer* se = Fun4AllServer::instance();
  QAG4SimulationIntt* qa = new QAG4SimulationIntt;
  qa->Verbosity(verbosity);
  qa->set_cluster_version(G4TRACKING::cluster_version);
  se->registerSubsystem(qa);
}

#endif
