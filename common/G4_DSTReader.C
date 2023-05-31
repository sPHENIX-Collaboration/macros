#ifndef MACRO_G4DSTREADER_C
#define MACRO_G4DSTREADER_C

#include <GlobalVariables.C>

#include <G4_Bbc.C>
#include <G4_BlackHole.C>
#include <G4_CEmc_Spacal.C>
#include <G4_EPD.C>
#include <G4_HcalIn_ref.C>
#include <G4_HcalOut_ref.C>
#include <G4_TrkrVariables.C>
/*
#include <G4_Intt.C>
#include <G4_Magnet.C>
#include <G4_Mvtx.C>
#include <G4_TPC.C>
*/
#include <g4eval/PHG4DSTReader.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libg4eval.so)

//////////////////////////////////////////////////////////////////
/*!
 \file G4_DSTReader.C
 \brief Convert DST to human command readable TTree for quick poke around the outputs
 \author  Jin Huang
 \version $Revision:  $
 \date    $Date: $
 */
//////////////////////////////////////////////////////////////////

namespace Enable
{
  bool DSTREADER = false;
  int DSTREADER_VERBOSITY = 0;
}  // namespace Enable

namespace G4DSTREADER
{
  bool save_g4_raw = true;
  double tower_zero_supp = 1.e-6;
}  // namespace G4DSTREADER

void G4DSTreaderInit() {}

void G4DSTreader(const string &outputFile = "G4sPHENIXCells.root")
{
  int verbosity = max(Enable::VERBOSITY, Enable::DSTREADER_VERBOSITY);

  // save a comprehensive  evaluation file
  PHG4DSTReader *ana = new PHG4DSTReader(outputFile);
  ana->set_save_particle(true);
  ana->set_load_all_particle(false);
  ana->set_load_active_particle(true);
  ana->set_save_vertex(true);

  ana->Verbosity(verbosity);

  if (G4DSTREADER::save_g4_raw)
  {
    if (Enable::MVTX)
    {
      ana->AddNode("MVTX");
    }
    if (Enable::INTT)
    {
      ana->AddNode("INTT");
    }
    if (Enable::TPC)
    {
      ana->AddNode("TPC");
    }

    if (Enable::BBC)
    {
      ana->AddNode("BBC");
    }

    if (Enable::CEMC)
    {
      ana->AddNode("CEMC");
      if (Enable::ABSORBER || Enable::CEMC_ABSORBER)
      {
        ana->AddNode("ABSORBER_CEMC");
        ana->AddNode("CEMC_ELECTRONICS");
        ana->AddNode("CEMC_SPT");
      }
    }

    if (Enable::HCALIN)
    {
      ana->AddNode("HCALIN");
      if (Enable::ABSORBER || Enable::HCALIN_ABSORBER)
      {
        ana->AddNode("ABSORBER_HCALIN");
      }
    }

    if (Enable::MAGNET)
    {
      if (Enable::ABSORBER || Enable::MAGNET_ABSORBER)
      {
        ana->AddNode("MAGNET");
      }
    }

    if (Enable::HCALOUT)
    {
      ana->AddNode("HCALOUT");
      if (Enable::ABSORBER || Enable::HCALOUT_ABSORBER)
      {
        ana->AddNode("ABSORBER_HCALOUT");
      }
    }

    if (Enable::EPD)
    {
      ana->AddNode("EPD");
    }

    if (Enable::BLACKHOLE)
    {
      ana->AddNode("BH_1");
      ana->AddNode("BH_FORWARD_PLUS");
      ana->AddNode("BH_FORWARD_NEG");
    }
  }

  ana->set_tower_zero_sup(G4DSTREADER::tower_zero_supp);
  if (Enable::CEMC_TOWER)
  {
    ana->AddTower("SIM_CEMC");
    ana->AddTower("RAW_CEMC");
    ana->AddTower("CALIB_CEMC");
  }
  if (Enable::HCALIN_TOWER)
  {
    ana->AddTower("SIM_HCALIN");
    ana->AddTower("RAW_HCALIN");
    ana->AddTower("CALIB_HCALIN");
  }
  if (Enable::HCALOUT_TOWER)
  {
    ana->AddTower("SIM_HCALOUT");
    ana->AddTower("RAW_HCALOUT");
    ana->AddTower("CALIB_HCALOUT");
  }
  // Jets disabled for now
  //  if (do_jet_reco)
  //    {
  //
  //      ana->AddJet("AntiKt06JetsInPerfect");
  //      ana->AddJet("G4TowerJets_6");
  //    }
  //  if (embed_input_file && do_jet_reco)
  //    {
  //      ana->AddJet("G4TowerJets_combined_6");
  //    }

  Fun4AllServer *se = Fun4AllServer::instance();
  se->registerSubsystem(ana);
}
#endif
