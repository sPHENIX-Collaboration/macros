#ifndef MACRO_G4TRKRSIM_C
#define MACRO_G4TRKRSIM_C

#include <GlobalVariables.C>
#include <G4_TrkrVariables.C>
#include <G4_ActsGeom.C>

#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4mvtx/PHG4MvtxDefs.h>
#include <g4mvtx/PHG4MvtxDigitizer.h>
#include <g4mvtx/PHG4MvtxHitReco.h>
#include <g4mvtx/PHG4MvtxSubsystem.h>

#include <g4intt/PHG4InttDeadMapLoader.h>
#include <g4intt/PHG4InttDefs.h>
#include <g4intt/PHG4InttDigitizer.h>
#include <g4intt/PHG4InttHitReco.h>
#include <g4intt/PHG4InttSubsystem.h>

#include <g4tpc/PHG4TpcCentralMembrane.h>
#include <g4tpc/PHG4TpcDigitizer.h>
#include <g4tpc/PHG4TpcDirectLaser.h>
#include <g4tpc/PHG4TpcDistortion.h>
#include <g4tpc/PHG4TpcElectronDrift.h>
#include <g4tpc/PHG4TpcEndCapSubsystem.h>
#include <g4tpc/PHG4TpcPadPlane.h>
#include <g4tpc/PHG4TpcPadPlaneReadout.h>
#include <g4tpc/PHG4TpcSubsystem.h>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <tpc/TpcClusterZCrossingCorrection.h>
#pragma GCC diagnostic pop

#include <g4micromegas/PHG4MicromegasDigitizer.h>
#include <g4micromegas/PHG4MicromegasHitReco.h>
#include <g4micromegas/PHG4MicromegasSubsystem.h>

#include <g4main/PHG4Reco.h>

#include <cmath>
#include <vector>

R__LOAD_LIBRARY(libg4tpc.so)
R__LOAD_LIBRARY(libg4mvtx.so)
R__LOAD_LIBRARY(libg4intt.so)
R__LOAD_LIBRARY(libg4micromegas.so)

void MvtxInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 12.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -165.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 24.);
}

double Mvtx(PHG4Reco* g4Reco, double radius,
            const int absorberactive = 0)
{
  bool maps_overlapcheck = Enable::OVERLAPCHECK || Enable::MVTX_OVERLAPCHECK;
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);

  PHG4MvtxSubsystem* mvtx = new PHG4MvtxSubsystem("MVTX");
  mvtx->Verbosity(verbosity);

  for (int ilayer = 0; ilayer < G4MVTX::n_maps_layer; ilayer++)
  {
    double radius_lyr = PHG4MvtxDefs::mvtxdat[ilayer][PHG4MvtxDefs::kRmd];
//    mvtx->set_double_param(ilayer, "layer_z_offset", G4MVTXAlignment::z_offset[ilayer]);
    if (verbosity)
    {
      cout << "Create Maps layer " << ilayer << " with radius " << radius_lyr << " mm." << endl;
    }
    radius = radius_lyr / 10.;
  }
//  mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "alignment_path",  G4MVTXAlignment::alignment_path);
  mvtx->set_string_param(PHG4MvtxDefs::GLOBAL, "stave_geometry_file", string(getenv("CALIBRATIONROOT")) + string("/Tracking/geometry/mvtx_stave.gdml"));

  mvtx->SetActive();
  mvtx->OverlapCheck(maps_overlapcheck);
  g4Reco->registerSubsystem(mvtx);
  radius += G4MVTX::radius_offset;
  return radius;
}

void Mvtx_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::MVTX_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();
  // new storage containers
  PHG4MvtxHitReco* maps_hits = new PHG4MvtxHitReco("MVTX");
  maps_hits->Verbosity(verbosity);

  double maps_readout_window = 5000.0;  // ns
  double extended_readout_time = 0.0;
  if(TRACKING::pp_mode) extended_readout_time = TRACKING::pp_extended_readout_time;
  // override the default timing window - default is +/- 5000 ns
  maps_hits->set_double_param("mvtx_tmin",  -maps_readout_window);
  maps_hits->set_double_param("mvtx_tmax",  maps_readout_window + extended_readout_time);

  std::cout << "PHG4MvtxHitReco: readout window is from " << -maps_readout_window << " to " <<  maps_readout_window + extended_readout_time << std::endl;
  se->registerSubsystem(maps_hits);

  PHG4MvtxDigitizer* digimvtx = new PHG4MvtxDigitizer();
  digimvtx->Verbosity(verbosity);
  // energy deposit in 25 microns = 9.6 KeV = 1000 electrons collected after recombination
  //digimvtx->set_adc_scale(0.95e-6);  // default set in code is 0.95e-06, which is 99 electrons
  se->registerSubsystem(digimvtx);

  return;
}


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

void TPCInit()
{
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, G4TPC::tpc_outer_radius);

  if (Enable::TPC_ENDCAP)
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 130.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -130.);
  }
  else
  {
    BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 211. / 2.);
    BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -211. / 2.);
  }

  // the mvtx is not called if disabled but the default number of layers is set to 3,
  // so we need to set it to zero
  if (!Enable::MVTX)
  {
    G4MVTX::n_maps_layer = 0;
  }
  // same for the INTT
  if (!Enable::INTT)
  {
    G4INTT::n_intt_layer = 0;
  }

  // Set the (static) drift velocity in the cluster Z crossing correction module
  TpcClusterZCrossingCorrection::_vdrift = G4TPC::tpc_drift_velocity_reco;
}

//! TPC end cap, wagon wheel, electronics
void TPC_Endcaps(PHG4Reco* g4Reco)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TPC_OVERLAPCHECK;
  bool AbsorberActive = Enable::ABSORBER || Enable::TPC_ABSORBER;

  PHG4TpcEndCapSubsystem* tpc_endcap = new PHG4TpcEndCapSubsystem("TPC_ENDCAP");
  tpc_endcap->SuperDetector("TPC_ENDCAP");

  if (AbsorberActive) tpc_endcap->SetActive();
  tpc_endcap->OverlapCheck(OverlapCheck);

  //  tpc_endcap->set_int_param("construction_verbosity", 2);

  g4Reco->registerSubsystem(tpc_endcap);

  return;
}

double TPC(PHG4Reco* g4Reco,
           double radius)
{
  bool OverlapCheck = Enable::OVERLAPCHECK || Enable::TPC_OVERLAPCHECK;
  bool AbsorberActive = Enable::ABSORBER || Enable::TPC_ABSORBER;

  PHG4TpcSubsystem* tpc = new PHG4TpcSubsystem("TPC");
  tpc->SetActive();
  tpc->SuperDetector("TPC");
  tpc->set_double_param("steplimits", 1);  // 1cm steps
  
  if (AbsorberActive)
    {
      tpc->SetAbsorberActive();
    }
  tpc->OverlapCheck(OverlapCheck);
  
  g4Reco->registerSubsystem(tpc);
  
  if (Enable::TPC_ENDCAP)
    {
      TPC_Endcaps(g4Reco);
    }
  
  radius = G4TPC::tpc_outer_radius;
  
  radius += no_overlapp;
  
  return radius;
}

void TPC_Cells()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TPC_VERBOSITY);
  auto se = Fun4AllServer::instance();

  // central membrane G4Hit generation
  if( G4TPC::ENABLE_CENTRAL_MEMBRANE_HITS )
  {
    auto centralMembrane = new PHG4TpcCentralMembrane;
    centralMembrane->setCentralMembraneDelay(0);
    centralMembrane->setCentralMembraneEventModulo(1);
    se->registerSubsystem(centralMembrane);
  }

  // direct laser G4Hit generation
  if( G4TPC::ENABLE_DIRECT_LASER_HITS )
  {
    auto directLaser = new PHG4TpcDirectLaser;

    // setup phi and theta steps
    /* use 5deg steps */
    static constexpr double deg_to_rad = M_PI/180.;
    directLaser->SetPhiStepping( 72, 0*deg_to_rad, 360*deg_to_rad );
    directLaser->SetThetaStepping( 17, 5*deg_to_rad, 90*deg_to_rad );
    directLaser->SetDirectLaserAuto( true );
    directLaser->set_double_param("drift_velocity", G4TPC::tpc_drift_velocity_sim);
    se->registerSubsystem(directLaser);
  }

  //=========================
  // setup Tpc readout for filling cells
  // g4tpc/PHG4TpcElectronDrift uses
  // g4tpc/PHG4TpcPadPlaneReadout
  //=========================

  auto padplane = new PHG4TpcPadPlaneReadout;
  padplane->Verbosity(verbosity);

  auto edrift = new PHG4TpcElectronDrift;
  edrift->Detector("TPC");
  edrift->Verbosity(verbosity);
  if( G4TPC::ENABLE_STATIC_DISTORTIONS || G4TPC::ENABLE_TIME_ORDERED_DISTORTIONS )
  {
    auto distortionMap = new PHG4TpcDistortion;
    distortionMap->set_do_static_distortions( G4TPC::ENABLE_STATIC_DISTORTIONS );
    distortionMap->set_static_distortion_filename( G4TPC::static_distortion_filename );

    distortionMap->set_do_time_ordered_distortions( G4TPC::ENABLE_TIME_ORDERED_DISTORTIONS );
    distortionMap->set_time_ordered_distortion_filename( G4TPC::time_ordered_distortion_filename );

    distortionMap->Init();
    edrift->setTpcDistortion( distortionMap );
  }

  double tpc_readout_time = 105.5/ G4TPC::tpc_drift_velocity_sim;  // ns
  double extended_readout_time = 0.0;
  if(TRACKING::pp_mode) extended_readout_time = TRACKING::pp_extended_readout_time;
  edrift->set_double_param("max_time", tpc_readout_time + extended_readout_time);
  std::cout << "PHG4TpcElectronDrift readout window is from 0 to " <<  tpc_readout_time + extended_readout_time << std::endl;

  // override the default drift velocity parameter specification
  edrift->set_double_param("drift_velocity", G4TPC::tpc_drift_velocity_sim);
  padplane->SetDriftVelocity(G4TPC::tpc_drift_velocity_sim);

  // fudge factors to get drphi 150 microns (in mid and outer Tpc) and dz 500 microns cluster resolution
  // They represent effects not due to ideal gas properties and ideal readout plane behavior
  // defaults are 0.085 and 0.105, they can be changed here to get a different resolution
  edrift->registerPadPlane(padplane);
  se->registerSubsystem(edrift);

  // The pad plane readout default is set in PHG4TpcPadPlaneReadout

  // We may want to change the number of inner layers, and can do that here
  padplane->set_int_param("tpc_minlayer_inner", G4MVTX::n_maps_layer + G4INTT::n_intt_layer);  // sPHENIX layer number of first Tpc readout layer
  padplane->set_int_param("ntpc_layers_inner", G4TPC::n_tpc_layer_inner);
  padplane->set_int_param("ntpc_phibins_inner", G4TPC::tpc_layer_rphi_count_inner);

  // Tpc digitizer
  //=========
  PHG4TpcDigitizer* digitpc = new PHG4TpcDigitizer();
  digitpc->SetTpcMinLayer(G4MVTX::n_maps_layer + G4INTT::n_intt_layer);
  double ENC = 670.0;  // standard
  digitpc->SetENC(ENC);
  double ADC_threshold = 4.0 * ENC;
  digitpc->SetADCThreshold(ADC_threshold);  // 4 * ENC seems OK
  digitpc->Verbosity(verbosity);
  cout << " Tpc digitizer: Setting ENC to " << ENC << " ADC threshold to " << ADC_threshold
       << " maps+Intt layers set to " << G4MVTX::n_maps_layer + G4INTT::n_intt_layer << endl;
  digitpc ->set_skip_noise_flag(false);
  se->registerSubsystem(digitpc);

}

void MicromegasInit()
{
  if (!Enable::MVTX)
  {
    G4MVTX::n_maps_layer = 0;
  }
  // same for the INTT
  if (!Enable::INTT)
  {
    G4INTT::n_intt_layer = 0;
  }
  if (!Enable::TPC)
  {
    G4TPC::n_gas_layer = 0;
  }
  BlackHoleGeometry::max_radius = std::max(BlackHoleGeometry::max_radius, 88.);
  BlackHoleGeometry::max_z = std::max(BlackHoleGeometry::max_z, 220. / 2.);
  BlackHoleGeometry::min_z = std::min(BlackHoleGeometry::min_z, -220. / 2.);
}

void Micromegas(PHG4Reco* g4Reco)
{
  const int mm_layer = G4MVTX::n_maps_layer + G4INTT::n_intt_layer + G4TPC::n_gas_layer;
  auto mm = new PHG4MicromegasSubsystem("MICROMEGAS", mm_layer);
  mm->OverlapCheck( Enable::OVERLAPCHECK );
  mm->SetActive();
  g4Reco->registerSubsystem(mm);
}

void Micromegas_Cells()
{
// the acts geometry needs to go here since it will be used by the PHG4MicromegasHitReco
  ACTSGEOM::ActsGeomInit();
  auto se = Fun4AllServer::instance();
  // micromegas
  auto reco = new PHG4MicromegasHitReco;
  reco->Verbosity(0);
  se->registerSubsystem(reco);

  se->registerSubsystem(new PHG4MicromegasDigitizer);
}

#endif
