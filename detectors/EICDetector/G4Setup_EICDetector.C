#ifndef MACRO_G4SETUPEICDETECTOR_C
#define MACRO_G4SETUPEICDETECTOR_C

#include <GlobalVariables.C>

#include <G4_Aerogel.C>
#include <G4_Barrel_EIC.C>
#include <G4_Bbc.C>
#include <G4_BlackHole.C>
#include <G4_CEmc_EIC.C>
#include <G4_DIRC.C>
#include <G4_EEMC.C>
#include <G4_FEMC_EIC.C>
#include <G4_FHCAL.C>
#include <G4_FST_EIC.C>
#include <G4_GEM_EIC.C>
#include <G4_HcalIn_ref.C>
#include <G4_HcalOut_ref.C>
#include <G4_Input.C>
#include <G4_Magnet.C>
#include <G4_Mvtx_EIC.C>
#include <G4_Pipe_EIC.C>
#include <G4_PlugDoor_EIC.C>
#include <G4_RICH.C>
#include <G4_TPC_EIC.C>
#include <G4_Tracking_EIC.C>
#include <G4_User.C>
#include <G4_World.C>

#include <g4detectors/PHG4CylinderSubsystem.h>

#include <g4eval/PHG4DstCompressReco.h>

#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>

#include <phfield/PHFieldConfig.h>

#include <g4decayer/EDecayType.hh>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4decayer.so)
R__LOAD_LIBRARY(libg4detectors.so)

void G4Init()
{
  // First some check for subsystems which do not go together

  if (Enable::TPC && Enable::FST)
  {
    cout << "TPC and FST cannot be enabled together" << endl;
    gSystem->Exit(1);
  }
  else if ((Enable::TPC || Enable::MVTX) && Enable::BARREL)
  {
    cout << "TPC/MVTX and BARREL cannot be enabled together" << endl;
    gSystem->Exit(1);
  }

  // load detector/material macros and execute Init() function
  if (Enable::PIPE) PipeInit();
  if (Enable::PLUGDOOR) PlugDoorInit();
  if (Enable::EGEM) EGEM_Init();
  if (Enable::FGEM) FGEM_Init();
  if (Enable::FST) FST_Init();
  if (Enable::BARREL) BarrelInit();
  if (Enable::MVTX) MvtxInit();
  if (Enable::TPC) TPCInit();
  if (Enable::TRACKING) TrackingInit();
  if (Enable::BBC) BbcInit();
  if (Enable::CEMC) CEmcInit(72);  // make it 2*2*2*3*3 so we can try other combinations
  if (Enable::HCALIN) HCalInnerInit(1);
  if (Enable::MAGNET) MagnetInit();
  MagnetFieldInit(); // We want the field - even if the magnet volume is disabled
  if (Enable::HCALOUT) HCalOuterInit();
  if (Enable::FEMC) FEMCInit();
  if (Enable::FHCAL) FHCALInit();
  if (Enable::EEMC) EEMCInit();
  if (Enable::DIRC) DIRCInit();
  if (Enable::RICH) RICHInit();
  if (Enable::AEROGEL) AerogelInit();
  if (Enable::USER) UserInit();
  if (Enable::BLACKHOLE) BlackHoleInit();
}

int G4Setup()
{
  //---------------
  // Fun4All server
  //---------------

  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4Reco *g4Reco = new PHG4Reco();

  WorldInit(g4Reco);

  g4Reco->set_rapidity_coverage(1.1);  // according to drawings
                                       // uncomment to set QGSP_BERT_HP physics list for productions
                                       // (default is QGSP_BERT for speed)
  //  g4Reco->SetPhysicsList("QGSP_BERT_HP");

  if (G4P6DECAYER::decayType != EDecayType::kAll)
  {
    g4Reco->set_force_decay(G4P6DECAYER::decayType);
  }

  double fieldstrength;
  istringstream stringline(G4MAGNET::magfield);
  stringline >> fieldstrength;
  if (stringline.fail())
  {  // conversion to double fails -> we have a string

    if (G4MAGNET::magfield.find("sPHENIX.root") != string::npos)
    {
      g4Reco->set_field_map(G4MAGNET::magfield, PHFieldConfig::Field3DCartesian);
    }
    else
    {
      g4Reco->set_field_map(G4MAGNET::magfield, PHFieldConfig::kField2D);
    }
  }
  else
  {
    g4Reco->set_field(fieldstrength);  // use const soleniodal field
  }
  g4Reco->set_field_rescale(G4MAGNET::magfield_rescale);

// the radius is an older protection against overlaps, it is not
// clear how well this works nowadays but it doesn't hurt either
  double radius = 0.;

  if (Enable::PIPE) radius = Pipe(g4Reco, radius);
  if (Enable::EGEM) EGEMSetup(g4Reco);
  if (Enable::FGEM) FGEMSetup(g4Reco);
  if (Enable::FST) FSTSetup(g4Reco);
  if (Enable::BARREL) Barrel(g4Reco, radius);
  if (Enable::MVTX) radius = Mvtx(g4Reco, radius);
  if (Enable::TPC) radius = TPC(g4Reco, radius);
  if (Enable::BBC) Bbc(g4Reco);
  if (Enable::CEMC) radius = CEmc(g4Reco, radius);
  if (Enable::HCALIN) radius = HCalInner(g4Reco, radius, 4);
  if (Enable::MAGNET) radius = Magnet(g4Reco, radius);
  if (Enable::HCALOUT) radius = HCalOuter(g4Reco, radius, 4);
  if (Enable::FEMC) FEMCSetup(g4Reco);
  if (Enable::FHCAL) FHCALSetup(g4Reco);
  if (Enable::EEMC) EEMCSetup(g4Reco);

  //----------------------------------------
  // PID

  if (Enable::DIRC) DIRCSetup(g4Reco);
  if (Enable::RICH) RICHSetup(g4Reco);
  if (Enable::AEROGEL) AerogelSetup(g4Reco);

  //----------------------------------------
  // sPHENIX forward flux return door
  if (Enable::PLUGDOOR) PlugDoor(g4Reco);

  if (Enable::USER) UserDetector(g4Reco);
  
  //----------------------------------------
  // BLACKHOLE if enabled, needs info from all previous sub detectors for dimensions
  if (Enable::BLACKHOLE) BlackHole(g4Reco, radius);

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);
  // finally adjust the world size in case the default is too small
  WorldSize(g4Reco, radius);

  se->registerSubsystem(g4Reco);
  return 0;
}

void ShowerCompress()
{
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4DstCompressReco *compress = new PHG4DstCompressReco("PHG4DstCompressReco");
  compress->AddHitContainer("G4HIT_PIPE");
  compress->AddHitContainer("G4HIT_FIELDCAGE");
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

  compress->AddHitContainer("G4HIT_EEMC");
  compress->AddHitContainer("G4HIT_ABSORBER_EEMC");
  compress->AddCellContainer("G4CELL_EEMC");
  compress->AddTowerContainer("TOWER_SIM_EEMC");
  compress->AddTowerContainer("TOWER_RAW_EEMC");
  compress->AddTowerContainer("TOWER_CALIB_EEMC");

  se->registerSubsystem(compress);

  return;
}

void DstCompress(Fun4AllDstOutputManager *out)
{
  if (out)
  {
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

    out->StripNode("G4HIT_EEMC");
    out->StripNode("G4HIT_ABSORBER_EEMC");
    out->StripNode("G4CELL_EEMC");
  }
}
#endif
