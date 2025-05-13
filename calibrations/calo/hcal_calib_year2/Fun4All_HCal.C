#ifndef FUN4ALL_EMCAL_C
#define FUN4ALL_EMCAL_C

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>
#include <caloreco/CaloTowerBuilder.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <phool/recoConsts.h>

#include <calotrigger/MinimumBiasClassifier.h>
#include <centrality/CentralityInfo.h>

#include <zdcinfo/ZdcReco.h>
#include <calotrigger/TriggerRunInfoReco.h>


#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <ffamodules/CDBInterface.h>
//#include <GlobalVariables.C>

#include <litecaloeval/LiteCaloEval.h>
#include <calib_emc_pi0/pi0EtaByEta.h>

#include<caloana/CaloTemp.h>


R__LOAD_LIBRARY(libcdbobjects)
R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)
R__LOAD_LIBRARY(libcalotrigger.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libzdcinfo.so)
R__LOAD_LIBRARY(libcalotemp.so)

void createLocalEMCalCalibFile(const string fname, int runNumber);
string findCosmicCalib(int runnumber);
string findCosmicCalib_ihcal(int runnumber);
string findRunByRunCalib(int runnumber);



void Fun4All_HCal(int nevents = 1e3, const std::string &fname = "inputdata.txt")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  CDBInterface::instance()->Verbosity(1);

  // se->Verbosity(verbosity);
  recoConsts *rc = recoConsts::instance();

  ifstream file(fname);
  string first_file;
  getline(file, first_file);

  //===============
  // conditions DB flags
  //===============
  pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
  int runnumber = runseg.first;
  cout << "run number = " << runnumber << endl;

  rc->set_StringFlag("CDB_GLOBALTAG", "ProdA_2024");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DST_TOWERS");
  in->AddListFile(fname);
  se->registerInputManager(in);

  //string calib_fname = findCosmicCalib(runnumber); 
  string calib_fname = findRunByRunCalib(runnumber); 


  //////////////////////
  // calibrate towers - takes raw towers and calibrate thems
  CaloTowerCalib *calibEMC = new CaloTowerCalib("CEMCCALIB");
  calibEMC->set_detector_type(CaloTowerDefs::HCALOUT);
  //calibEMC->setFieldName("ohcal_cosmic_calibration");
  //calibEMC->set_directURL(calib_fname.c_str());
  se->registerSubsystem(calibEMC);

  // get file paths for local calibrations
  calib_fname = findCosmicCalib_ihcal(runnumber); 

  CaloTowerCalib *calibIhcal= new CaloTowerCalib("CEMCCALIB");
  calibIhcal->set_detector_type(CaloTowerDefs::HCALIN);
  //calibIhcal->setFieldName("ihcal_cosmic_calibration"); // change fild if needed 
  //calibIhcal->set_directURL(calib_fname.c_str());
  se->registerSubsystem(calibIhcal);

  std::string filename = first_file.substr(first_file.find_last_of("/\\") + 1);
  std::string OutFile = Form("OUTHIST_OHCAL_%s" , filename.c_str());
  std::string OutFile2 = Form("OUTHIST_IHCAL_%s" , filename.c_str());

  ///////////////////
  // Histmakers
  // makes tower energy spectra for general use and 
  // the tower slope code.
  LiteCaloEval *eval7e = new LiteCaloEval("CEMCEVALUATOR1", "CEMC",OutFile);
  eval7e->CaloType(LiteCaloEval::HCALOUT);
  eval7e->setInputTowerNodeName("TOWERINFO_CALIB_HCALOUT");
  eval7e->set_reqMinBias(true);
  se->registerSubsystem(eval7e);

  LiteCaloEval *eval7e2 = new LiteCaloEval("CEMCEVALUATOR2", "CEMC2",OutFile2);
  eval7e2->CaloType(LiteCaloEval::HCALIN);
  eval7e2->setInputTowerNodeName("TOWERINFO_CALIB_HCALIN");
  eval7e2->set_reqMinBias(true);
  se->registerSubsystem(eval7e2);


  /////////////////////////////////////////
  // Use this handy code to get a temperature hist of
  // the requested hcal for the log closest to the start
  // of the trun. Because many DB calls seem to colide, only
  // submit 1 job per a run. 
  //////////////////////////////////////////
  //  CaloTemp* calotemp = new CaloTemp("CALOTEMP",Form("ohcal_temp_hist_%d.root",runnumber));
  //  calotemp->Detector("HCALOUT");
  //  se->registerSubsystem(calotemp);

  //  CaloTemp* calotemp2 = new CaloTemp("CALOTEMP",Form("ihcal_temp_hist_%d.root",runnumber));
  //  calotemp2->Detector("HCALIN");
  //  se->registerSubsystem(calotemp2);
  // se->run(1);
  // se->End();
  // return;

  se->run(nevents);
  se->End();
  se->PrintTimer();
  delete se;

  gSystem->Exec("echo DONE > DONE.txt");

  gSystem->Exit(0);
}



string findCosmicCalib(int runnumber){

  int cos_run[14] = {
    44732,
    45767,
    48795,
    49170,
    49478,
    50572,
    50758,
    51020,
    52930,
    53321,
    53883,
    54608,
    55061,
    55349
  };

 int target = 0;
  for (int ir=0; ir<14; ir++){
    if(runnumber < cos_run[ir]) break; 
    target=ir;
  }
  string path = Form("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cosmicCalibFiles/ohcal_cosmic_calibration_%d.root",cos_run[target]); 
 cout << "path to calib " << path << endl;

 return path;
}


string findCosmicCalib_ihcal(int runnumber){

  int cos_run[14] = {
    44732,
    45767,
    48795,
    53883,
    54608
  };

 int target = 0;
  for (int ir=0; ir<14; ir++){
    if(runnumber < cos_run[ir]) break; 
    target=ir;
  }
  string path = Form("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cosmicCalibFiles/ihcal_cosmic_calibration_%d.root",cos_run[target]); 
 cout << "path to calib " << path << endl;

 return path;
}


string findRunByRunCalib(int runnumber){

  ifstream runlist("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/runDeptCalib/cdbFiles/runList.txt");
  if (!runlist){
    cout << "did not find runList for calibraiton" << endl;
  }
  string line;

  int minDelta = 10000000;
  int calibRunnumber = 0;
  while(getline(runlist,line))
  {
    int runnumber_calib = stoi(line);
    int delta = abs(runnumber - runnumber_calib);

    if( delta < minDelta){
     minDelta = delta;
     calibRunnumber = runnumber_calib;
   }
  }
  string cdbFile = Form("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/runDeptCalib/cdbFiles/cdb_ohcal_tsc_temp_cosmic_hh_%d.root",calibRunnumber);
 cout << " using run-by-run calibration " << cdbFile.c_str() << endl;
  return cdbFile;
}




#endif
