#pragma once
// this macro loads a given macro from the zoo of our reco macros
// includes it and runs a function from it to check if all
// include files and libraries it needs are included in it
// basically it checks if the macro can run standalone and
// does not rely on other macros to load its prerequisites 
//
// uncomment the ifdef for the macro you want to check
//#define TEST_G4_AEROGEL
//#define TEST_G4_BBC
//#define TEST_G4_CALOTRIGGER
//#define TEST_G4_CEMC
//#define TEST_G4_CEMC_EIC
//#define TEST_G4_DIRC
//#define TEST_G4_DSTREADER
//#define TEST_G4_DSTREADER_EICDETECTOR
//#define TEST_G4_DSTREADER_FSPHENIX
//#define TEST_G4_EEMC
//#define TEST_G4_FEMC
//#define TEST_G4_FGEM_FSPHENIX
//#define TEST_G4_FHCAL
//#define TEST_G4_FWDJETS
//#define TEST_G4_FUN4ALL_FSPHENIX
//#define TEST_G4_FUN4ALL_SPHENIX
//#define TEST_G4_FUN4ALL_EICDETECTOR
//#define TEST_G4_GLOBAL
//#define TEST_G4_GEM_EIC
//#define TEST_G4_HCALIN
//#define TEST_G4_HCALOUT
//#define TEST_G4_HIJETRECO
//#define TEST_G4_JETS
//#define TEST_G4_MAGNET
//#define TEST_G4_PLUGDOOR
//#define TEST_G4_PLUGDOOR_EIC
//#define TEST_G4_PIPE
//#define TEST_G4_PSTOF
//#define TEST_G4_RICH
//#define TEST_G4SETUP_EICDETECTOR
//#define TEST_G4SETUP_FSPHENIX
//#define TEST_G4SETUP_SPHENIX
//#define TEST_G4_SVTX_FSPHENIX
//#define TEST_G4_SVTX_EIC
//#define TEST_G4_TRACKING
//#define TEST_G4_TRACKING_EIC

#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#ifdef TEST_G4_AEROGEL
#include "G4_Aerogel.C"
#endif
#ifdef TEST_G4_BBC
#include "G4_Bbc.C"
#endif
#ifdef TEST_G4_CALOTRIGGER
#include "G4_CaloTrigger.C"
#endif
#ifdef TEST_G4_CEMC
#include "G4_CEmc_Spacal.C"
#endif
#ifdef TEST_G4_CEMC_EIC
#include "G4_CEmc_EIC.C"
#endif
#ifdef TEST_G4_DIRC
#include "G4_DIRC.C"
#endif
#ifdef TEST_G4_DSTREADER
#include "G4_DSTReader.C"
#endif
#ifdef TEST_G4_DSTREADER_EICDETECTOR
#include "G4_DSTReader_EICDetector.C"
#endif
#ifdef TEST_G4_DSTREADER_FSPHENIX
#include "G4_DSTReader_fsPHENIX.C"
#endif
#ifdef TEST_G4_EEMC
#include "G4_EEMC.C"
#endif
#ifdef TEST_G4_FEMC
#include "G4_FEMC.C"
#endif
#ifdef TEST_G4_FGEM_FSPHENIX
#include "G4_FGEM_fsPHENIX.C"
#endif
#ifdef TEST_G4_FHCAL
#include "G4_FHCAL.C"
#endif
#ifdef TEST_G4_FUN4ALL_FSPHENIX
#include "Fun4All_G4_fsPHENIX.C"
#endif
#ifdef TEST_G4_FUN4ALL_SPHENIX
#include "Fun4All_G4_sPHENIX.C"
#endif
#ifdef TEST_G4_FUN4ALL_EICDETECTOR
#include "Fun4All_G4_EICDetector.C"
#endif
#ifdef TEST_G4_FWDJETS
#include "G4_FwdJets.C"
#endif
#ifdef TEST_G4_GLOBAL
#include "G4_Global.C"
#endif
#ifdef TEST_G4_GEM_EIC
#include "G4_GEM_EIC.C"
#endif
#ifdef TEST_G4_HCALIN
#include "G4_HcalIn_ref.C"
#endif
#ifdef TEST_G4_HCALOUT
#include "G4_HcalOut_ref.C"
#endif
#ifdef TEST_G4_HIJETRECO
#include "G4_HIJetReco.C"
#endif
#ifdef TEST_G4_JETS
#include "G4_Jets.C"
#endif
#ifdef TEST_G4_MAGNET
#include "G4_Magnet.C"
#endif
#ifdef TEST_G4_PIPE
#include "G4_Pipe.C"
#endif
#ifdef TEST_G4_PLUGDOOR
#include "G4_PlugDoor.C"
#endif
#ifdef TEST_G4_PLUGDOOR_EIC
#include "G4_PlugDoor_EIC.C"
#endif
#ifdef TEST_G4_PSTOF
#include "G4_PSTOF.C"
#endif
#ifdef TEST_G4_RICH
#include "G4_RICH.C"
#endif
#ifdef TEST_G4SETUP_EICDETECTOR
#include "G4Setup_EICDetector.C"
#endif
#ifdef TEST_G4SETUP_FSPHENIX
#include "G4Setup_fsPHENIX.C"
#endif
#ifdef TEST_G4SETUP_SPHENIX
#include "G4Setup_sPHENIX.C"
#endif
#ifdef TEST_G4_SVTX_EIC
#include "G4_Svtx_maps_ladders+intt_ladders+tpc_KalmanPatRec_EIC.C"
#endif
#ifdef TEST_G4_SVTX_FSPHENIX
#include "G4_Svtx_maps_ladders+intt_ladders+tpc_KalmanPatRec.C"
#endif
#ifdef TEST_G4_TRACKING
#include "G4_Tracking.C"
#endif
#ifdef TEST_G4_TRACKING_EIC
#include "G4_Tracking_EIC.C"
#endif

#endif

void QuickTest()
{

#ifdef TEST_G4_AEROGEL
  AerogelInit();
#endif
#ifdef TEST_G4_BBC
  BbcInit();
#endif
#ifdef TEST_G4_CALOTRIGGER
  CaloTriggerInit();
#endif
#ifdef TEST_G4_CEMC
  CEmcInit();
#endif
#ifdef TEST_G4_CEMC_EIC
  CEmcInit();
#endif
#ifdef TEST_G4_DIRC
  DIRCInit();
#endif
#ifdef TEST_G4_DSTREADER
  G4DSTreaderInit();
#endif
#ifdef TEST_G4_DSTREADER_EICDETECTOR
  G4DSTreader_EICDetectorInit();
#endif
#ifdef TEST_G4_DSTREADER_FSPHENIX
  G4DSTreader_fsPHENIXInit();
#endif
#ifdef TEST_G4_EEMC
  EEMCInit();
#endif
#ifdef TEST_G4_FEMC
  FEMCInit();
#endif
#ifdef TEST_G4_FGEM_FSPHENIX
  FGEM_Init();
#endif
#ifdef TEST_G4_FHCAL
  FHCALInit();
#endif
#ifdef TEST_G4_FUN4ALL_SPHENIX
  RunFFALoadTest();
#endif
#ifdef TEST_G4_FUN4ALL_SPHENIX
  RunFFALoadTest();
#endif
#ifdef TEST_G4_FUN4ALL_EICDETECTOR
  RunFFALoadTest();
#endif
#ifdef TEST_G4_FWDJETS
  Jet_FwdRecoInit();
#endif
#ifdef TEST_G4_GEM_EIC
  EGEM_Init();
#endif
#ifdef TEST_G4_GLOBAL
  GlobalInit();
#endif
#ifdef TEST_G4_HCALIN
  HCalInnerInit();
#endif
#ifdef TEST_G4_HCALOUT
  HCalOuterInit();
#endif
#ifdef TEST_G4_HIJETRECO
  HIJetRecoInit();
#endif
#ifdef TEST_G4_JETS
  JetInit();
#endif
#ifdef TEST_G4_MAGNET
  MagnetInit();
#endif
#ifdef TEST_G4_PIPE
  PipeInit();
#endif 
#ifdef TEST_G4_PLUGDOOR
  PlugDoorInit();
#endif
#ifdef TEST_G4_PLUGDOOR_EIC
  PlugDoorInit();
#endif
#ifdef TEST_G4_PSTOF
 PSTOFInit();
#endif
#ifdef TEST_G4_RICH
 RICHInit();
#endif
#ifdef TEST_G4SETUP_EICDETECTOR
 RunLoadTest();
#endif
#ifdef TEST_G4SETUP_FSPHENIX
 RunLoadTest();
#endif
#ifdef TEST_G4SETUP_SPHENIX
 RunLoadTest();
#endif
#ifdef TEST_G4_SVTX_EIC
 SvtxInit();
#endif
#ifdef TEST_G4_SVTX_FSPHENIX
 SvtxInit();
#endif
#ifdef TEST_G4_TRACKING
  TrackingInit();
#endif
#ifdef TEST_G4_TRACKING_EIC
  TrackingInit();
#endif
}
