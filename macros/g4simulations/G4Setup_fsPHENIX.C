#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include "G4_Pipe.C"
#include "G4_Tracking.C"
#include "G4_CEmc_Spacal.C"
#include "G4_HcalIn_ref.C"
#include "G4_Magnet.C"
#include "G4_HcalOut_ref.C"
#include "G4_FGEM_fsPHENIX.C"
#include "G4_FEMC.C"
#include "G4_FHCAL.C"
#include <g4decayer/EDecayType.hh>
#include <g4detectors/PHG4ConeSubsystem.h>
#include <g4eval/PHG4DstCompressReco.h>
#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <phfield/PHFieldConfig.h>
class SubsysReco;
R__LOAD_LIBRARY(libg4decayer.so)
R__LOAD_LIBRARY(libg4detectors.so)
int make_piston(string name, PHG4Reco* g4Reco);
#else
double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes
bool overlapcheck = false; // set to true if you want to check for overlaps
#endif

void RunLoadTest() {}

void G4Init(bool do_svtx = true,
	    bool do_cemc = true,
	    bool do_hcalin = true,
	    bool do_magnet = true,
	    bool do_hcalout = true,
            bool do_pipe = true,
            bool do_FGEM = true,
	    bool do_FEMC = true,
	    bool do_FHCAL = true,
            int n_TPC_layers = 40) {

  // load detector/material macros and execute Init() function

  if (do_pipe)
    {
      gROOT->LoadMacro("G4_Pipe.C");
      PipeInit();
    }
  if (do_svtx)
    {
      gROOT->LoadMacro("G4_Tracking.C");
      TrackingInit(n_TPC_layers);
    }

  if (do_cemc)
    {
      gROOT->LoadMacro("G4_CEmc_Spacal.C");
      CEmcInit(72); // make it 2*2*2*3*3 so we can try other combinations
    }  

  if (do_hcalin) 
    {
      gROOT->LoadMacro("G4_HcalIn_ref.C");
      HCalInnerInit();
    }

  if (do_magnet)
    {
      gROOT->LoadMacro("G4_Magnet.C");
      MagnetInit();
    }
  if (do_hcalout)
    {
      gROOT->LoadMacro("G4_HcalOut_ref.C");
      HCalOuterInit();
    }

  if (do_FGEM)
    {
      gROOT->LoadMacro("G4_FGEM_fsPHENIX.C");
      FGEM_Init();
    }

  if (do_FEMC)
    {
      gROOT->LoadMacro("G4_FEMC.C");
      FEMCInit();
    }

  if (do_FHCAL) 
    {
      gROOT->LoadMacro("G4_FHCAL.C");
      FHCALInit();
    }
}


int G4Setup(const int absorberactive = 0,
	    const string &field ="1.5",
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
	    const EDecayType decayType = EDecayType::kAll,
#else
	    const EDecayType decayType = TPythia6Decayer::kAll,
#endif
	    const bool do_svtx = true,
	    const bool do_cemc = true,
	    const bool do_hcalin = true,
	    const bool do_magnet = true,
	    const bool do_hcalout = true,
	    const bool do_pipe = true,
	    const bool do_FGEM = true,
	    const bool do_FEMC = false,
	    const bool do_FHCAL = false,
     	    const float magfield_rescale = 1.0) {
  
  //---------------
  // Load libraries
  //---------------

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");

  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  // read-in HepMC events to Geant4 if there is any
  HepMCNodeReader *hr = new HepMCNodeReader();
  se->registerSubsystem(hr);

  PHG4Reco* g4Reco = new PHG4Reco();
  g4Reco->save_DST_geometry(true); //Save geometry from Geant4 to DST
  g4Reco->set_rapidity_coverage(1.1); // according to drawings
// uncomment to set QGSP_BERT_HP physics list for productions 
// (default is QGSP_BERT for speed)
  //  g4Reco->SetPhysicsList("QGSP_BERT_HP"); 
 #if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
  if (decayType != EDecayType::kAll)
#else
  if (decayType != TPythia6Decayer::kAll)
#endif
  {
    g4Reco->set_force_decay(decayType);
  }
  
  double fieldstrength;
  istringstream stringline(field);
  stringline >> fieldstrength;
  if (stringline.fail()) { // conversion to double fails -> we have a string

    if (field.find("sPHENIX.root") != string::npos) {
      g4Reco->set_field_map(field, PHFieldConfig::Field3DCartesian);
    } else {
      g4Reco->set_field_map(field, PHFieldConfig::kField2D);
    }
  } else {
    g4Reco->set_field(fieldstrength); // use const soleniodal field
  }
  g4Reco->set_field_rescale(magfield_rescale);
  
  double radius = 0.;

  //----------------------------------------
  // PIPE
  if (do_pipe) radius = Pipe(g4Reco, radius, absorberactive);
  
  //----------------------------------------
  // SVTX
  if (do_svtx) radius = Tracking(g4Reco, radius, absorberactive);

  //----------------------------------------
  // CEMC
//
  if (do_cemc) radius = CEmc(g4Reco, radius, 8, absorberactive);
//  if (do_cemc) radius = CEmc_Vis(g4Reco, radius, 8, absorberactive);// for visualization substructure of SPACAL, slow to render
  
  //----------------------------------------
  // HCALIN
  
  if (do_hcalin) radius = HCalInner(g4Reco, radius, 4, absorberactive);

  //----------------------------------------
  // MAGNET
  
  if (do_magnet) radius = Magnet(g4Reco, radius, 0, absorberactive);

  //----------------------------------------
  // HCALOUT
  
  if (do_hcalout) radius = HCalOuter(g4Reco, radius, 4, absorberactive);

  //----------------------------------------
  // Forward tracking

  if ( do_FGEM )
    FGEMSetup(g4Reco);

  //----------------------------------------
  // FEMC

  if ( do_FEMC )
    FEMCSetup(g4Reco, absorberactive);

  //----------------------------------------
  // FHCAL

  if ( do_FHCAL )
    FHCALSetup(g4Reco, absorberactive);

  // sPHENIX forward flux return(s) with reduced thickness
  PHG4CylinderSubsystem *flux_return_plus = new PHG4CylinderSubsystem("FWDFLUXRET", 0);
  flux_return_plus->set_int_param("lengthviarapidity",0);
  flux_return_plus->set_double_param("length",10.2);
  flux_return_plus->set_double_param("radius",2.1);
  flux_return_plus->set_double_param("thickness",263.5-5.0);
  flux_return_plus->set_double_param("place_z",335.9);
  flux_return_plus->set_string_param("material","G4_Fe");
  flux_return_plus->SetActive(false);
  flux_return_plus->SuperDetector("FLUXRET_ETA_PLUS");
  flux_return_plus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_plus);

  PHG4CylinderSubsystem *flux_return_minus = new PHG4CylinderSubsystem("FWDFLUXRET", 0);
  flux_return_minus->set_int_param("lengthviarapidity",0);
  flux_return_minus->set_double_param("length",10.2);
  flux_return_minus->set_double_param("radius",2.1);
  flux_return_minus->set_double_param("place_z",-335.9);
  flux_return_minus->set_double_param("thickness",263.5-5.0);
  flux_return_minus->set_string_param("material","G4_Fe");
  flux_return_minus->SetActive(false);
  flux_return_minus->SuperDetector("FLUXRET_ETA_MINUS");
  flux_return_minus->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(flux_return_minus);

  //----------------------------------------
  // piston magnet
//  make_piston("magpiston", g4Reco);

  //----------------------------------------
  // BLACKHOLE
  // minimal space for forward instrumentation
  if (radius<270) radius = 270;

  // swallow all particles coming out of the backend of sPHENIX
  PHG4CylinderSubsystem *blackhole = new PHG4CylinderSubsystem("BH", 1);
blackhole->set_double_param("radius",radius + 10); // add 10 cm

  blackhole->set_int_param("lengthviarapidity",0);
  blackhole->set_double_param("length",g4Reco->GetWorldSizeZ() - no_overlapp); // make it cover the world in length
  blackhole->BlackHole();
  blackhole->set_double_param("thickness",0.1); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  //----------------------------------------
  // FORWARD BLACKHOLEs
  // +Z
  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_PLUS", 1);
  blackhole->SuperDetector("BH_FORWARD_PLUS");
  blackhole->set_double_param("radius",0); // add 10 cm
  blackhole->set_int_param("lengthviarapidity",0);
  blackhole->set_double_param("length",0.1); // make it cover the world in length
  blackhole->set_double_param("place_z",g4Reco->GetWorldSizeZ()/2. - 0.1  - no_overlapp);
  blackhole->BlackHole();
  blackhole->set_double_param("thickness",radius - no_overlapp); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_NEG", 1);
  blackhole->SuperDetector("BH_FORWARD_NEG");
  blackhole->set_double_param("radius",0); // add 10 cm
  blackhole->set_int_param("lengthviarapidity",0);
  blackhole->set_double_param("length",0.1); // make it cover the world in length
  blackhole->set_double_param("place_z", - g4Reco->GetWorldSizeZ()/2. +0.1  + no_overlapp);
  blackhole->BlackHole();
  blackhole->set_double_param("thickness",radius - no_overlapp); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);
  se->registerSubsystem( g4Reco );
  return 0;
}


void ShowerCompress(int verbosity = 0) {

  gSystem->Load("libfun4all.so");
  gSystem->Load("libg4eval.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  
  PHG4DstCompressReco* compress = new PHG4DstCompressReco("PHG4DstCompressReco");
  compress->AddHitContainer("G4HIT_PIPE");
  compress->AddHitContainer("G4HIT_SVTXSUPPORT");
  compress->AddHitContainer("G4HIT_CEMC_ELECTRONICS");
  compress->AddHitContainer("G4HIT_CEMC");
  compress->AddHitContainer("G4HIT_ABSORBER_CEMC");
  compress->AddHitContainer("G4HIT_CEMC_SPT");
  compress->AddHitContainer("G4HIT_ABSORBER_HCALIN");
  compress->AddHitContainer("G4HIT_HCALIN");
  compress->AddHitContainer("G4HIT_HCALIN_SPT");
  compress->AddHitContainer("G4HIT_MAGNET");
  compress->AddHitContainer("G4HIT_ABSORBER_HCALOUT");
  compress->AddHitContainer("G4HIT_HCALOUT");
  compress->AddHitContainer("G4HIT_BH_1");
  compress->AddHitContainer("G4HIT_BH_FORWARD_PLUS");
  compress->AddHitContainer("G4HIT_BH_FORWARD_NEG");
  compress->AddCellContainer("G4CELL_CEMC");
  compress->AddCellContainer("G4CELL_HCALIN");
  compress->AddCellContainer("G4CELL_HCALOUT");
  compress->AddTowerContainer("TOWER_SIM_CEMC");
  compress->AddTowerContainer("TOWER_RAW_CEMC");
  compress->AddTowerContainer("TOWER_CALIB_CEMC");
  compress->AddTowerContainer("TOWER_SIM_HCALIN");
  compress->AddTowerContainer("TOWER_RAW_HCALIN");
  compress->AddTowerContainer("TOWER_CALIB_HCALIN");
  compress->AddTowerContainer("TOWER_SIM_HCALOUT");
  compress->AddTowerContainer("TOWER_RAW_HCALOUT");
  compress->AddTowerContainer("TOWER_CALIB_HCALOUT");

  compress->AddHitContainer("G4HIT_FEMC");
  compress->AddHitContainer("G4HIT_ABSORBER_FEMC");
  compress->AddHitContainer("G4HIT_FHCAL");
  compress->AddHitContainer("G4HIT_ABSORBER_FHCAL"); 
  compress->AddCellContainer("G4CELL_FEMC");
  compress->AddCellContainer("G4CELL_FHCAL");
  compress->AddTowerContainer("TOWER_SIM_FEMC");
  compress->AddTowerContainer("TOWER_RAW_FEMC");
  compress->AddTowerContainer("TOWER_CALIB_FEMC");
  compress->AddTowerContainer("TOWER_SIM_FHCAL");
  compress->AddTowerContainer("TOWER_RAW_FHCAL");
  compress->AddTowerContainer("TOWER_CALIB_FHCAL");
  
  se->registerSubsystem(compress);
  
  return; 
}

void DstCompress(Fun4AllDstOutputManager* out) {
  if (out) {
    out->StripNode("G4HIT_PIPE");
    out->StripNode("G4HIT_SVTXSUPPORT");
    out->StripNode("G4HIT_CEMC_ELECTRONICS");
    out->StripNode("G4HIT_CEMC");
    out->StripNode("G4HIT_ABSORBER_CEMC");
    out->StripNode("G4HIT_CEMC_SPT");
    out->StripNode("G4HIT_ABSORBER_HCALIN");
    out->StripNode("G4HIT_HCALIN");
    out->StripNode("G4HIT_HCALIN_SPT");
    out->StripNode("G4HIT_MAGNET");
    out->StripNode("G4HIT_ABSORBER_HCALOUT");
    out->StripNode("G4HIT_HCALOUT");
    out->StripNode("G4HIT_BH_1");
    out->StripNode("G4HIT_BH_FORWARD_PLUS");
    out->StripNode("G4HIT_BH_FORWARD_NEG");
    out->StripNode("G4CELL_CEMC");
    out->StripNode("G4CELL_HCALIN");
    out->StripNode("G4CELL_HCALOUT");

    out->StripNode("G4HIT_FEMC");
    out->StripNode("G4HIT_ABSORBER_FEMC");
    out->StripNode("G4HIT_FHCAL");
    out->StripNode("G4HIT_ABSORBER_FHCAL"); 
    out->StripNode("G4CELL_FEMC");
    out->StripNode("G4CELL_FHCAL");
  }
}

int
make_piston(string name, PHG4Reco* g4Reco)
{
  double be_pipe_radius    = 2.16;   // 2.16 cm based on spec sheet
  double be_pipe_thickness = 0.0760; // 760 um based on spec sheet
  double be_pipe_length    = 80.0;   // +/- 40 cm

  double al_pipe_radius    = 2.16;   // same as Be pipe
  double al_pipe_thickness = 0.1600; // 1.6 mm based on spec
  double al_pipe_length    = 88.3;   // extension beyond +/- 40 cm

  const double zpos0 = al_pipe_length + be_pipe_length * 0.5; // first large GEM station
  const double zpos1 = 305 - 20; // front of forward ECal/MPC
  const double zpos2 = 335.9 - 10.2 / 2.; // front of the forward field endcap
  const double calorimeter_hole_diamater = 9.92331 *2; // side length of the middle hole of MPC that can hold the piston. Also the max diameter of the piston in that region

  const double beampipe_radius = be_pipe_radius;

  // teeth cone section specific
  const double number_of_wteeth = 100;
  const double teeth_thickness = 0.3504 * 2; //2 X0
  const double eta_inner = -log(tan(atan((beampipe_radius + 0.1) / zpos0) / 2));
  const double eta_outter = 4.2;
  const double eta_teeth_outter = 4.05;
  double pos = zpos0 + (zpos1 - zpos0) / 2;
//  cout << "MAGNETIC PISTON:" << eta_inner << " " << eta_outter << " " << pos
//      << endl;

  PHG4ConeSubsystem *magpiston = new PHG4ConeSubsystem("Piston", 0);
  magpiston->SetZlength((zpos1 - zpos0) / 2);
  magpiston->SetPlaceZ((zpos1 + zpos0) / 2);
  magpiston->SetR1(beampipe_radius,
      tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter)) * zpos0);
  magpiston->SetR2(beampipe_radius,
      tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter)) * zpos1);
  magpiston->SetMaterial("G4_Fe");
  magpiston->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(magpiston);

//  PHG4ConeSubsystem *magpiston = new PHG4ConeSubsystem(name.c_str(), 1);
//  magpiston->SetZlength((zpos1 - zpos0) / 2);
//  magpiston->SetPlaceZ(pos);
//  magpiston->Set_eta_range(eta_outter, eta_inner);
//  magpiston->SetMaterial("G4_Fe");
//  magpiston->SuperDetector(name.c_str());
//  magpiston->SetActive(false);
//  g4Reco->registerSubsystem(magpiston);

  pos = zpos0 + 1.0 + teeth_thickness / 2;
  for (int i = 0; i < number_of_wteeth; i++)
    {
      stringstream s;
      s << name;
      s << "_teeth_";
      s << i;

      magpiston = new PHG4ConeSubsystem(s.str(), i);
      magpiston->SuperDetector(name);
      magpiston->SetZlength(teeth_thickness / 2);
      magpiston->SetPlaceZ(pos);
      magpiston->SetR1(
          //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter - .01))
              * (pos - teeth_thickness / 2), //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_teeth_outter))
              * (pos - teeth_thickness / 2) //
              );
      magpiston->SetR2(
          //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter - .01))
              * (pos + teeth_thickness / 2), //
          tan(PHG4Sector::Sector_Geometry::eta_to_polar_angle(eta_outter - .01))
              * (pos + teeth_thickness / 2) + .1 //
              );
      magpiston->SetMaterial("G4_W");
      magpiston->SuperDetector(name.c_str());
      magpiston->SetActive(false);
      magpiston->OverlapCheck(overlapcheck);
      g4Reco->registerSubsystem(magpiston);
      pos += ((zpos1 - zpos0 - 10) / number_of_wteeth);
    }

  // last piece connect to the field return
  PHG4CylinderSubsystem *magpiston2 = new PHG4CylinderSubsystem("Piston_EndSection", 0);
  magpiston2->set_int_param("lengthviarapidity",0);
  magpiston2->set_double_param("length",zpos2 - zpos1);
  magpiston2->set_double_param("place_z", (zpos2 + zpos1) / 2.);
  magpiston2->set_double_param("radius",beampipe_radius);
  magpiston2->set_double_param("thickness",calorimeter_hole_diamater / 2. - beampipe_radius);
  magpiston2->set_string_param("material","G4_Fe");
  magpiston2->SuperDetector(name);
  magpiston2->SetActive(false);
  magpiston2->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(magpiston2);

  return 0;
}

