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

void
G4DSTreader( const char * outputFile = "G4sPHENIXCells.root",//
    int absorberactive = 1, //
    bool do_svtx = true, //
    bool do_preshower = false, //
    bool do_cemc = true, //
    bool do_hcalin = true, //
    bool do_magnet = true, //
    bool do_hcalout = true, //
    bool do_cemc_twr = true, //
    bool do_hcalin_twr = true, //
    bool do_magnet = true, //
    bool do_hcalout_twr = true //
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
            ana->AddNode("ABSORBER_CEMC");
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
            ana->AddNode("MAGNET_0");
        }

      if (do_hcalout)
        {
          ana->AddNode("HCALOUT");
          if (absorberactive)
            ana->AddNode("ABSORBER_HCALOUT");
        }


      ana->AddNode("BH_1");
      ana->AddNode("BH_FORWARD_PLUS");
      ana->AddNode("BH_FORWARD_NEG");

    }

  ana->set_tower_zero_sup(1e-6);
  if (do_cemc_twr)
    ana->AddTower("CEMC");
  if (do_hcalin_twr)
    {
      ana->AddTower("HCALIN");
    }
  if (do_hcalout_twr)
    {
      ana->AddTower("HCALOUT");
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
