#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
//#include <rawwaveformtowerbuilder/RawWaveformTowerBuilder.h>
#include <fun4all/Fun4AllDstOutputManager.h>

//#include <litecaloeval/LiteCaloEval.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

#include <phool/recoConsts.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
//R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)



#endif

// to get files from my local area
void dst_simple_towerslope_runmacro(int nevents = 1e5, const char *fname = "/sphenix/user/sregmi/FINAL_VERSION/tower_slope_old_tower_method/3_RawWaveformTowerBuilder/macro/testoutput_400.root", const char * outfile =  "defout.root", const int runNumber = 0)

{
  gSystem->Load("libg4dst");

  Fun4AllServer *se = Fun4AllServer::instance();

  // CaloWaveFormSim *ca = new CaloWaveFormSim("CALOWAVEFORMSIM",outfile);
  // ca->Detector("CEMC");
  // se->registerSubsystem(ca);

  // Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
  // in->fileopen(fname);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("in");

  TString infiletstr(fname);
  if (infiletstr.Contains(".list"))
    in->AddListFile(fname);
  else
    in->fileopen(fname);


  se->registerInputManager(in);

  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  intrue2->AddFile("updated_geo.root");
  se->registerInputManager(intrue2);


  //CaloTowerBuilder *ca = new CaloTowerBuilder();

  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2023"); // this points to the global tag in the CDB
  // The calibrations have a validity range set by the beam clock which is not read out of the prdfs as of now
  //  rc->set_uint64Flag("TIMESTAMP",0);
  rc->set_uint64Flag("TIMESTAMP",runNumber);
  
  
  CaloTowerCalib *calib = new CaloTowerCalib("CEMCCALIB");
  // calib->setCalibName("cemc_abscalib_cosmic");// these two lines are needed to choose your own calibration
  // calib->setFieldName("cemc_abscalib_cosmic");
  calib->set_detector_type(CaloTowerCalib::CEMC);
  se->registerSubsystem(calib);
  

   RawClusterBuilderTemplate *ClusterBuilder = new RawClusterBuilderTemplate("EmcRawClusterBuilderTemplate2");
  ClusterBuilder->Detector("CEMC");
  ClusterBuilder->Verbosity(10);
  ClusterBuilder->set_threshold_energy(0.032);  // This threshold should be the same as in CEMCprof_Thresh**.root file below
  std::string emc_prof = getenv("CALIBRATIONROOT");
  emc_prof += "/EmcProfile/CEMCprof_Thresh30MeV.root";
  ClusterBuilder->LoadProfile(emc_prof);
  ClusterBuilder->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower
  se->registerSubsystem(ClusterBuilder);


  CaloCalibEmc_Pi0 *eval_pi2 = new CaloCalibEmc_Pi0("dummy", outfile);
                                                // this call is needed for embedding
  //  eval_pi2->set_centrality_nclusters_cut(350);  // which uses more central events
                                                // than we will for data to enhance Bkg
                                                // to match the enhanced signal from embed
  eval_pi2->set_UseTowerInfo(1); // to use towerinfo objects rather than old RawTower
  se->registerSubsystem(eval_pi2);
  cout << "successful registration of pi0 " << endl;



  /*
  LiteCaloEval *eval7e = new LiteCaloEval("CEMCEVALUATOR2", "CEMC",outfile);
  //  LiteCaloEval *eval = new LiteCaloEval("HOCEMCEVALUATOR2", "HCALOUT", outputfile.c_str());
  //  eval->Verbosity(verbosity);
  eval7e->CaloType(LiteCaloEval::CEMC);
  //eval->CaloType(LiteCaloEval::HCALOUT);
  se->registerSubsystem(eval7e);

  */


  // in->AddListFile("g4hits.list");

// Fun4All
//  se->registerInputManager(in);

  //Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", "testoutput_400.root");

  //out->StripNode("WAVEFORMS_CEMC");
  //se->registerOutputManager(out);

  se->run(nevents);

  se->End();
  se->PrintTimer();
  gSystem->Exit(0);
}
