#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "G4_Pipe.C"
#include "G4_VTX.C"
#include "G4_CTD.C"
#include "G4_Magnet.C"
#include "G4_Gem.C"
#include "G4_JLDIRC.C"
#include "G4_Barrel_Hcal.C"
#include "G4_BeamLine.C"
#include "G4_DRich.C"
#include "G4_EndCap_Electron.C"
#include "G4_EndCap_Hadron.C"

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

void G4Init(const bool do_ctd = true,
            const bool do_vtx = true,
	    const bool do_magnet = true,
	    const bool do_pipe = true,
            const bool do_gem = true,
            const bool do_jldirc = true,
            const bool do_barrel_hcal = true,
            const bool do_drich = true,
            const bool do_endcap_electron = true,
            const bool do_endcap_hadron = true,
            const bool do_beamline = true
	    )
  {

  // load detector/material macros and execute Init() function

  if (do_pipe)
    {
      gROOT->LoadMacro("G4_Pipe.C");
      PipeInit();
    }  
  if (do_ctd)
    {
      gROOT->LoadMacro("G4_CTD.C");
      CTDInit();
    }  
  if (do_magnet)
    {
      gROOT->LoadMacro("G4_Magnet.C");
      MagnetInit();
    }
  if (do_gem)
    {
      gROOT->LoadMacro("G4_Gem.C");
      GemInit();
    }
  if (do_jldirc)
    {
      gROOT->LoadMacro("G4_JLDIRC.C");
      JLDIRCInit();
    }

  if (do_barrel_hcal)
    {
      gROOT->LoadMacro("G4_Barrel_Hcal.C");
      Barrel_HcalInit();
    }

  if (do_drich)
    {
      gROOT->LoadMacro("G4_DRich.C");
      DRichInit();
    }

  if (do_endcap_electron)
    {
      gROOT->LoadMacro("G4_EndCap_Electron.C");
      EndCap_ElectronInit();
    }

  if (do_endcap_hadron)
    {
      gROOT->LoadMacro("G4_EndCap_Hadron.C");
      EndCap_HadronInit();
    }
  if (do_beamline)
  {
      gROOT->LoadMacro("G4_BeamLine.C");
      BeamLineInit();
  }

}


int G4Setup(const int absorberactive = 0,
	    const string &field ="2.0",
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
	    const EDecayType decayType = EDecayType::kAll,
#else
	    const EDecayType decayType = TPythia6Decayer::kAll,
#endif
	    const bool do_ctd = true,
	    const bool do_vtx = true,
	    const bool do_magnet = true,
            const bool do_pipe = true,
            const bool do_gem = true,
            const bool do_jldirc = true,
            const bool do_barrel_hcal = true,
            const bool do_drich = true,
            const bool do_endcap_electron = true,
            const bool do_endcap_hadron = true,
            const bool do_beamline = true,
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
  // VTX
  if (do_vtx) radius = VTX(g4Reco, radius, absorberactive);

  //----------------------------------------
  // CTD
  if (do_ctd) radius = CTD(g4Reco, radius, absorberactive);
  
  //----------------------------------------
  // DIRC
  if (do_jldirc) radius = JLDIRC(g4Reco, radius, absorberactive);
  
  //----------------------------------------
  // MAGNET
  
  if (do_magnet) radius = Magnet(g4Reco, radius, 0, absorberactive);

  //----------------------------------------
  // Barrel Hcal
 
  if (do_barrel_hcal) radius = Barrel_Hcal(g4Reco, radius, 0, absorberactive);


  //----------------------------------------
  // Gem (hadron and electron going side
  
  if (do_gem) double tmp = Gem(g4Reco, radius, 0, absorberactive);

  if (do_drich) double tmp =  DRich(g4Reco, radius, 0, absorberactive);

  if (do_endcap_electron) double tmp =  EndCap_Electron(g4Reco, radius, 0, absorberactive);

  if (do_endcap_hadron) double tmp =  EndCap_Hadron(g4Reco, radius, 0, absorberactive);

  if (do_beamline) double tmp = BeamLine(g4Reco, radius, 0, absorberactive);
  radius = 200.;
  //----------------------------------------
  // BLACKHOLE
  
  // swallow all particles coming out of the backend of sPHENIX
  PHG4CylinderSubsystem *blackhole = new PHG4CylinderSubsystem("BH", 0);
blackhole->set_double_param("radius",radius + 10); // add 10 cm

  blackhole->set_int_param("lengthviarapidity",0);
  blackhole->set_double_param("length",g4Reco->GetWorldSizeZ() - no_overlapp); // make it cover the world in length
  blackhole->BlackHole();
  blackhole->set_double_param("thickness",0.1); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  blackhole->SuperDetector("BLACKHOLE");
  g4Reco->registerSubsystem(blackhole);

  //----------------------------------------
  // FORWARD BLACKHOLEs
  // +Z
  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_PLUS", 1);
  blackhole->SuperDetector("BLACKHOLE");
//  blackhole->SuperDetector("BH_FORWARD_PLUS");
  blackhole->set_double_param("radius",0); // add 10 cm
  blackhole->set_int_param("lengthviarapidity",0);
  blackhole->set_double_param("length",0.1); // make it cover the world in length
  blackhole->set_double_param("place_z",g4Reco->GetWorldSizeZ()/2. - 0.1  - no_overlapp);
  blackhole->BlackHole();
  blackhole->set_double_param("thickness",radius - no_overlapp); // it needs some thickness
  blackhole->SetActive(); // always see what leaks out
  blackhole->OverlapCheck(overlapcheck);
  g4Reco->registerSubsystem(blackhole);

  blackhole = new PHG4CylinderSubsystem("BH_FORWARD_NEG", 2);
  blackhole->SuperDetector("BLACKHOLE");
//  blackhole->SuperDetector("BH_FORWARD_NEG");
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
