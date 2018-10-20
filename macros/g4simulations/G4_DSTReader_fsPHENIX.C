#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/Fun4AllServer.h>
#include <g4eval/PHG4DSTReader.h>
R__LOAD_LIBRARY(libg4eval.so)
#endif

//////////////////////////////////////////////////////////////////
/*!
 \file G4_DSTReader.C
 \brief Convert DST to human command readable TTree for quick poke around the outputs
 \author  Jin Huang
 \version $Revision:  $
 \date    $Date: $
 */
//////////////////////////////////////////////////////////////////

#include <string>

void G4DSTreader_fsPHENIXInit() {}

void
G4DSTreader_fsPHENIX( const char * outputFile = "G4sPHENIXCells.root",//
    int absorberactive = 1, //
    bool do_svtx = true, //
    bool do_cemc = true, //
    bool do_hcalin = true, //
    bool do_magnet = true, //
    bool do_hcalout = true, //
    bool do_cemc_twr = true, //
    bool do_hcalin_twr = true, //
    bool do_hcalout_twr = true, //
    bool do_FGEM = true, //
    bool do_FHCAL = true, //
    bool do_FHCAL_twr = true, //
    bool do_FEMC = true, //
    bool do_FEMC_twr = true //
    )
{

  //! debug output on screen?
  const bool debug = false;

  //! save raw g4 hits
  const bool save_g4_raw = true;

  // save a comprehensive  evaluation file
  PHG4DSTReader* ana = new PHG4DSTReader(
      string(outputFile) + string("_DSTReader.root"));
  ana->set_save_particle(true);
  ana->set_load_all_particle(false);
  ana->set_load_active_particle(true);
  ana->set_save_vertex(true);

  if (debug)
    {
      ana->Verbosity(2);
    }

  if (save_g4_raw)
    {
      if (do_svtx)
        {
          ana->AddNode("SVTX");
        }

      if (do_cemc)
        {
          ana->AddNode("CEMC");
          if (absorberactive)
            {
              ana->AddNode("ABSORBER_CEMC");
              ana->AddNode("CEMC_ELECTRONICS");
              ana->AddNode("CEMC_SPT");
            }
        }

      if (do_hcalin)
        {
          ana->AddNode("HCALIN");
          if (absorberactive)
            ana->AddNode("ABSORBER_HCALIN");
        }

      if (do_magnet)
        {
          if (absorberactive)
            ana->AddNode("MAGNET");
        }

      if (do_hcalout)
        {
          ana->AddNode("HCALOUT");
          if (absorberactive)
            ana->AddNode("ABSORBER_HCALOUT");
        }

      if (do_FHCAL)
        {
          ana->AddNode("FHCAL");
          if (absorberactive)
            ana->AddNode("ABSORBER_FHCAL");
        }

      if (do_FEMC)
        {
          ana->AddNode("FEMC");
          if (absorberactive)
            ana->AddNode("ABSORBER_FEMC");
        }

      if (do_FGEM)
        {
          ana->AddNode("FGEM_0");
          ana->AddNode("FGEM_1");
          ana->AddNode("FGEM_2");
          ana->AddNode("FGEM_3");
          ana->AddNode("FGEM_4");
        }

      ana->AddNode("BH_1");
      ana->AddNode("BH_FORWARD_PLUS");
      ana->AddNode("BH_FORWARD_NEG");

    }

  ana->set_tower_zero_sup(1e-6);
  if (do_cemc_twr)
    {
      ana->AddTower("SIM_CEMC");
      ana->AddTower("RAW_CEMC");
      ana->AddTower("CALIB_CEMC");
    }
  if (do_hcalin_twr)
    {
      ana->AddTower("SIM_HCALIN");
      ana->AddTower("RAW_HCALIN");
      ana->AddTower("CALIB_HCALIN");
    }
  if (do_hcalout_twr)
    {
      ana->AddTower("SIM_HCALOUT");
      ana->AddTower("RAW_HCALOUT");
      ana->AddTower("CALIB_HCALOUT");
    }
  if (do_FHCAL_twr)
    {
      ana->AddTower("SIM_FHCAL");
      ana->AddTower("RAW_FHCAL");
      ana->AddTower("CALIB_FHCAL");
    }
  if (do_FEMC_twr)
    {
      ana->AddTower("SIM_FEMC");
      ana->AddTower("RAW_FEMC");
      ana->AddTower("CALIB_FEMC");
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
