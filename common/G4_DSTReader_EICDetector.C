#ifndef MACRO_G4DSTREADEREICDETECTOR_C
#define MACRO_G4DSTREADEREICDETECTOR_C

#include <GlobalVariables.C>

#include <G4_Barrel_EIC.C>
#include <G4_CEmc_EIC.C>
#include <G4_DIRC.C>
#include <G4_EEMC.C>
#include <G4_FEMC_EIC.C>
#include <G4_FHCAL.C>
#include <G4_FST_EIC.C>
#include <G4_GEM_EIC.C>
#include <G4_HcalIn_ref.C>
#include <G4_HcalOut_ref.C>
#include <G4_Magnet.C>
#include <G4_Mvtx_EIC.C>
#include <G4_RICH.C>
#include <G4_TPC_EIC.C>

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

void G4DSTreader_EICDetectorInit() {}
void G4DSTreader_EICDetector(const string &outputFile = "G4sPHENIXCells.root")
{
  //! debug output on screen?
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
    if (Enable::PIPE && Enable::PIPE_ABSORBER)
    {
      ana->AddNode("PIPE");
    }
    if (Enable::BARREL)
    {
      if (G4BARREL::SETTING::BARRELV5 ||G4BARREL::SETTING::BARRELV6) {
	int nLayer1 = 3;   //barrel 1                                                                                                      
	int nLayer2 = 2;   //barrel 2                                                                                                      
	if (G4BARREL::SETTING::BARRELV6) nLayer2 = 1;  //compactible w/ TPC                                                                
	int nLayer[2]={nLayer1,nLayer2};

	for (int n=0;n<2;n++) 
	{
	  for (int i;i<nLayer[n];i++) 
	  {
	    ana->AddNode(Form("BARREL%d_%d",n,i));
	  }
	}
      }
      else
      {
	int nLayer=5;
	if (G4BARREL::SETTING::BARRELV4) nLayer=6;
	for (int i;i<nLayer;i++)
	{
	  ana->AddNode(Form("BARREL%d",i));
	}
      }
    }	
    if (Enable::MVTX)
    {
      ana->AddNode("MVTX");
    }
    if (Enable::TPC)
    {
      ana->AddNode("TPC");
    }

    if (Enable::EGEM)
    {
      ana->AddNode("EGEM_0");
      ana->AddNode("EGEM_1");
      ana->AddNode("EGEM_2");
      ana->AddNode("EGEM_3");
    }
    if (Enable::FGEM)
    {
      ana->AddNode("FGEM_2");
      ana->AddNode("FGEM_3");
      ana->AddNode("FGEM_4");
    }
    if (Enable::FST)
    {
      ana->AddNode("FST_0");
      ana->AddNode("FST_1");
      ana->AddNode("FST_2");
      ana->AddNode("FST_3");
      ana->AddNode("FST_4");
      if (G4FST::SETTING::FSTV4 || G4FST::SETTING::FSTV5)
      {
	ana->AddNode("FST_5");
      }
    }

    if (Enable::CEMC)
    {
      ana->AddNode("CEMC");
      if (Enable::ABSORBER || Enable::CEMC_ABSORBER)
      {
        ana->AddNode("ABSORBER_CEMC");
        ana->AddNode("CEMC_ELECTRONICS_0");
      }
    }

    if (Enable::HCALIN)
    {
      ana->AddNode("HCALIN");
      if (Enable::ABSORBER || Enable::HCALIN_ABSORBER)
      {
        ana->AddNode("ABSORBER_HCALIN");
        ana->AddNode("HCALIN_SPT");
      }
    }

    if (Enable::MAGNET)
    {
      if (Enable::ABSORBER || Enable::MAGNET_ABSORBER)
        ana->AddNode("MAGNET");
    }

    if (Enable::HCALOUT)
    {
      ana->AddNode("HCALOUT");
      if (Enable::ABSORBER || Enable::HCALOUT_ABSORBER)
        ana->AddNode("ABSORBER_HCALOUT");
    }

    if (Enable::FHCAL)
    {
      ana->AddNode("FHCAL");
      if (Enable::ABSORBER || Enable::FHCAL_ABSORBER)
        ana->AddNode("ABSORBER_FHCAL");
    }

    if (Enable::FEMC)
    {
      ana->AddNode("FEMC");
      if (Enable::ABSORBER || Enable::FEMC_ABSORBER)
        ana->AddNode("ABSORBER_FEMC");
    }

    if (Enable::EEMC)
    {
      ana->AddNode("EEMC");
    }
    if (Enable::DIRC)
    {
      ana->AddNode("DIRC");
    }
    if (Enable::RICH)
    {
      ana->AddNode("RICH");
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
  if (Enable::FHCAL_TOWER)
  {
    ana->AddTower("SIM_FHCAL");
    ana->AddTower("RAW_FHCAL");
    ana->AddTower("CALIB_FHCAL");
  }
  if (Enable::FEMC_TOWER)
  {
    ana->AddTower("SIM_FEMC");
    ana->AddTower("RAW_FEMC");
    ana->AddTower("CALIB_FEMC");
  }
  if (Enable::EEMC_TOWER)
  {
    ana->AddTower("SIM_EEMC");
    ana->AddTower("RAW_EEMC");
    ana->AddTower("CALIB_EEMC");
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
