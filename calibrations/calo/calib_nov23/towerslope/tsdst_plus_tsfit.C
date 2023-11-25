#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
//#include <rawwaveformtowerbuilder/RawWaveformTowerBuilder.h>
#include <fun4all/Fun4AllDstOutputManager.h>

#include <litecaloeval/LiteCaloEval.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>
//#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

#include <phool/recoConsts.h>


R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
//R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)



#endif

// to get files from my local area
void tsdst_plus_tsfit(int nevents = 1e5, const char *fname = "/sphenix/user/sregmi/FINAL_VERSION/tower_slope_old_tower_method/3_RawWaveformTowerBuilder/macro/testoutput_400.root", const char * outfile =  "defout.root", const char * fitoutfile = "deffitout.root")
//,const int runNumber)
{

  Fun4AllServer *se = Fun4AllServer::instance();


  Fun4AllInputManager *in = new Fun4AllDstInputManager("in");

  TString infiletstr(fname);
  if (infiletstr.Contains(".list"))
    in->AddListFile(fname);
  else
    in->fileopen(fname);


  se->registerInputManager(in);

  //JF to Blair Nov 23 -- leaving these next blocks, similar code
  //  would be needed if we run a second iteration of towerslope
  // which is fairly possible.

  //CaloTowerBuilder *ca = new CaloTowerBuilder();

  // // recoConsts *rc = recoConsts::instance();
  // // rc->set_StringFlag("CDB_GLOBALTAG","ProdA_2023"); // this points to the global  tag in the CDB
  // // The calibrations have a validity range set by the beam clock which is not read out of the prdfs as of now
  //rc->set_uint64Flag("TIMESTAMP",runNumber);
  
  
  // CaloTowerCalib *calib = new CaloTowerCalib("CEMCCALIB");
  // //  calib->setCalibName("cemc_abscalib_cosmic");// these two lines are needed to choose your own calibration
  // //  calib->setFieldName("cemc_abscalib_cosmic");
  // calib->set_detector_type(CaloTowerCalib::CEMC);
  // se->registerSubsystem(calib);
  
  std::cout << "in tow dst " << std::endl;


  LiteCaloEval *eval7e = new LiteCaloEval("CEMCEVALUATOR2", "CEMC",outfile);
  //  LiteCaloEval *eval = new LiteCaloEval("HOCEMCEVALUATOR2", "HCALOUT", outputfile.c_str());
  //  eval->Verbosity(verbosity);
  eval7e->CaloType(LiteCaloEval::CEMC);
  eval7e->setInputTowerNodeName("TOWERINFO_CALIB_CEMC");
  //eval->CaloType(LiteCaloEval::HCALOUT);
  se->registerSubsystem(eval7e);


  se->run(nevents);

  se->End();
  se->PrintTimer();


  LiteCaloEval modlce;

  //reflce.CaloType(LiteCaloEval::CEMC);
  modlce.CaloType(LiteCaloEval::CEMC);


  modlce.Get_Histos(outfile,fitoutfile);

  modlce.m_myminbin =  8;       // this really is min bin (no -1/+1 needed) 
                                //  but not sure if for summer 23 this should
                                // be 8 or 9 I think 8 according to Joey Clement
                                // on ~Nov 1, mattermost post
  modlce.m_mymaxbin =  95 + 1 ;  // this actually means maxbin is 1 less
  //  modlce.m_mymaxbin =  95+1;  // so that means maxbin is 95 which is
  //  really the max eta bin for emc, indexed 0 through 95


  modlce.setFitMin(0.12);
  modlce.setFitMax(0.7);

  modlce.FitRelativeShifts(&modlce,110);


  gSystem->Exit(0);
}
