#ifndef MACRO_G4SETUPFSPHENIX_C
#define MACRO_G4SETUPFSPHENIX_C

#include <GlobalVariables.C>

#include <G4_Bbc.C>
#include <G4_BlackHole.C>
#include <G4_CEmc_Spacal.C>
#include <G4_FEMC.C>
#include <G4_FGEM_fsPHENIX.C>
#include <G4_FHCAL.C>
#include <G4_HcalIn_ref.C>
#include <G4_HcalOut_ref.C>
#include <G4_Magnet.C>
#include <G4_Mvtx.C>
#include <G4_Pipe.C>
#include <G4_Piston.C>
#include <G4_PlugDoor_fsPHENIX.C>
#include <G4_TPC.C>
#include <G4_User.C>
#include <G4_World.C>

#include <g4eval/PHG4DstCompressReco.h>

#include <g4main/PHG4Reco.h>
#include <g4main/PHG4TruthSubsystem.h>

#include <phfield/PHFieldConfig.h>

#include <fun4all/Fun4AllDstOutputManager.h>

R__LOAD_LIBRARY(libg4decayer.so)
R__LOAD_LIBRARY(libg4detectors.so)

void ShowerCompress(int verbosity = 0);
void DstCompress(Fun4AllDstOutputManager *out);

void RunLoadTest() {}

void G4Init()
{
  // load detector/material macros and execute Init() function

  if (Enable::PIPE) PipeInit();
  if (Enable::MVTX) MvtxInit();
  if (Enable::INTT) InttInit();
  if (Enable::TPC) TPCInit();
  if (Enable::BBC) BbcInit();
  if (Enable::CEMC) CEmcInit();
  if (Enable::HCALIN) HCalInnerInit();
  if (Enable::MAGNET) MagnetInit();
// We want the field - even if the magnet volume is disabled
  MagnetFieldInit();
  if (Enable::HCALOUT) HCalOuterInit();
  if (Enable::FGEM) FGEM_Init();
  if (Enable::FEMC) FEMCInit();
  if (Enable::FHCAL) FHCALInit();
  if (Enable::PISTON) PistonInit();
  if (Enable::PLUGDOOR) PlugDoorInit();
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
  g4Reco->save_DST_geometry(true);  //Save geometry from Geant4 to DST
  WorldInit(g4Reco);
  g4Reco->set_rapidity_coverage(1.1);  // according to drawings
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

  double radius = 0.;

  //----------------------------------------
  // PIPE
  if (Enable::PIPE)
  {
    radius = Pipe(g4Reco, radius);
  }
  if (Enable::MVTX)
  {
    radius = Mvtx(g4Reco, radius);
  }
  if (Enable::INTT)
  {
    radius = Intt(g4Reco, radius);
  }
  if (Enable::TPC)
  {
    radius = TPC(g4Reco, radius);
  }

  //----------------------------------------
  // BBC

  if (Enable::BBC) Bbc(g4Reco);

  //----------------------------------------
  // CEMC
  //
  if (Enable::CEMC)
  {
    radius = CEmc(g4Reco, radius, 8);
  }

  //----------------------------------------
  // HCALIN

  if (Enable::HCALIN)
  {
    radius = HCalInner(g4Reco, radius, 4);
  }

  //----------------------------------------
  // MAGNET

  if (Enable::MAGNET)
  {
    radius = Magnet(g4Reco, radius);
  }

  //----------------------------------------
  // HCALOUT

  if (Enable::HCALOUT)
  {
    radius = HCalOuter(g4Reco, radius, 4);
  }

  //----------------------------------------
  // Forward tracking

  if (Enable::FGEM)
  {
    FGEMSetup(g4Reco);
  }

  //----------------------------------------
  // FEMC

  if (Enable::FEMC)
  {
    FEMCSetup(g4Reco);
  }
  //----------------------------------------
  // FHCAL

  if (Enable::FHCAL)
  {
    FHCALSetup(g4Reco);
  }
  if (Enable::PISTON)
  {
    Piston(g4Reco);
  }

  if (Enable::PLUGDOOR)
  {
    PlugDoor(g4Reco);
  }

  if (Enable::USER)
  {
    UserDetector(g4Reco);
  }

  if (Enable::BLACKHOLE)
  {
    BlackHole(g4Reco, radius);
  }

  PHG4TruthSubsystem *truth = new PHG4TruthSubsystem();
  g4Reco->registerSubsystem(truth);

  // finally adjust the world size in case the default is too small
  WorldSize(g4Reco, radius);

  se->registerSubsystem(g4Reco);
  return 0;
}

void ShowerCompress(int verbosity = 0)
{
  Fun4AllServer *se = Fun4AllServer::instance();

  PHG4DstCompressReco *compress = new PHG4DstCompressReco("PHG4DstCompressReco");
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
  }
}
#endif
