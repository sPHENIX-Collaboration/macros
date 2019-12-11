#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "G4_Pipe.C"
#include "G4_Tracking.C"
#include "G4_PSTOF.C"
#include "G4_CEmc_Spacal.C"
#include "G4_HcalIn_ref.C"
#include "G4_Magnet.C"
#include "G4_HcalOut_ref.C"
#include "G4_PlugDoor.C"
#include "G4_FEMC.C"

#include <g4eval/PHG4DstCompressReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <g4decayer/EDecayType.hh>
#include <g4detectors/PHG4CylinderSubsystem.h>
#include <g4main/PHG4TruthSubsystem.h>
#include <g4main/PHG4Reco.h>
#include <phfield/PHFieldConfig.h>
#include <g4main/HepMCNodeReader.h>
class SubsysReco;
R__LOAD_LIBRARY(libg4decayer.so)
R__LOAD_LIBRARY(libg4detectors.so)
#else
bool overlapcheck = false; // set to true if you want to check for overlaps
double no_overlapp = 0.0001; // added to radii to avoid overlapping volumes
#endif

// This function is only used to test if we can load this as root6 macro
// without running into unresolved libraries and include files
void RunLoadTest() {}

void G4Init(const bool do_tracking = true,
      const bool do_pstof = true,
	    const bool do_cemc = true,
	    const bool do_hcalin = true,
	    const bool do_magnet = true,
	    const bool do_hcalout = true,
	    const bool do_pipe = true,
	    const bool do_plugdoor = false,
	    const bool do_FEMC = false
	    )
  {

  // load detector/material macros and execute Init() function

  if (do_pipe)
    {
      gROOT->LoadMacro("G4_Pipe.C");
      PipeInit();
    }  
  if (do_tracking)
    {
      gROOT->LoadMacro("G4_Tracking.C"); 
      TrackingInit();
    }

  if (do_pstof) 
    {
      gROOT->LoadMacro("G4_PSTOF.C");
      PSTOFInit();
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

  if (do_pipe)
    {
      gROOT->LoadMacro("G4_PlugDoor.C");
      PlugDoorInit();
    }
  if (do_FEMC)
    {
      gROOT->LoadMacro("G4_FEMC.C");
      FEMCInit();
    }

}


int G4Setup(const int absorberactive = 0,
	    const string &field ="1.5",
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
	    const EDecayType decayType = EDecayType::kAll,
#else
	    const EDecayType decayType = TPythia6Decayer::kAll,
#endif
	    const bool do_tracking = true,
	    const bool do_pstof = true,
	    const bool do_cemc = true,
	    const bool do_hcalin = true,
	    const bool do_magnet = true,
	    const bool do_hcalout = true,
      const bool do_pipe = true,
      const bool do_plugdoor = false,
//	    const bool do_plugdoor = true,
	    const bool do_FEMC = false, 
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
  // TRACKING
  if (do_tracking) radius = Tracking(g4Reco, radius, absorberactive);

  //----------------------------------------
  // PSTOF
  
  if (do_pstof) radius = PSTOF(g4Reco, radius, absorberactive);

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
  // sPHENIX forward flux return door
  if (do_plugdoor) PlugDoor(g4Reco, absorberactive);

  // forward EMC
  if(do_FEMC) FEMCSetup(g4Reco, absorberactive);

  //----------------------------------------
  // BLACKHOLE
  
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
  compress->AddCellContainer("G4CELL_FEMC");
  compress->AddTowerContainer("TOWER_SIM_FEMC");
  compress->AddTowerContainer("TOWER_RAW_FEMC");
  compress->AddTowerContainer("TOWER_CALIB_FEMC");
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
    out->StripNode("G4CELL_FEMC");
  }
}
