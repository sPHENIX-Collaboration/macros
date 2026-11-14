#ifndef FUN4ALL_HCAL_C
#define FUN4ALL_HCAL_C


#include "HcalCalibDatasetinputs.h" // run list, DST tag and cosmics calibration files of each dataset

#include <cdbobjects/CDBTTree.h>  // for CDBTTree

#include <litecaloeval/LiteCaloEval.h>

#include <calib_emc_pi0/pi0EtaByEta.h>

#include <zdcinfo/ZdcReco.h>

#include<caloana/CaloTemp.h>

#include <calotrigger/MinimumBiasClassifier.h>
#include <calotrigger/TriggerRunInfoReco.h>

#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/RawClusterBuilderTemplate.h>
#include <caloreco/RawClusterPositionCorrection.h>
#include <caloreco/CaloTowerBuilder.h>

#include <centrality/CentralityInfo.h>

#include <ffamodules/CDBInterface.h>
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

#include <globalvertex/GlobalVertexReco.h>

#include <mbd/MbdReco.h>

#include <phool/recoConsts.h>

#include <Rtypes.h> // for R__LOAD_LIBRARY macro
#include <TSystem.h>
#include <TSystemDirectory.h>

#include <unistd.h> // for isatty

#include <format>
#include <fstream>
#include <iostream>
#include <regex>

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
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libglobalvertex.so)

std::string askDataset(const std::string &def);
std::string findCalib(const HCalCalibDataset &ds, int isIHcal, int runnumber, std::string calib);
std::string findRunByRunCalib(const std::string &dir, int runnumber);
std::string findTscCalib(const HCalCalibDataset &ds, int isIHcal, int runnumber);

// nevents   : number of events, 0 = all
// fname     : list of DST_CALOFITTING files of one run
// dataset   : run2pp, run2auau, run3oo, ... (see HcalCalibDatasetinputs.h). Empty (default): asked for in a terminal,
//             the default answer is the dataset of the run number. Not in a terminal (condor): the dataset of the run number.
// ohcalCalib, ihcalCalib: calibration of each detector
//             empty or "cosmics": the cosmics only file of the dataset in HcalCalibDatasetinputs.h (the default)
//             "cosmics_tsc"     : the cosmics + towerslope file of the dataset (tscDir in HcalCalibDatasetinputs.h)
//             "cosmics_tsc_temp": the final file of the run, cosmics + towerslope + temperature (oh_finalDir, ih_finalDir)
//             a file            : this CDB file, e.g. genTSCalib/cdbFiles/ohcal_cdb_tsc_cos_calib_<run>.root
//             a folder          : the file cdb_*_<first run>_<last run>*.root of the folder whose run range holds the run (final calibration)
//             "cdb"             : the calibration of the conditions database
void Fun4All_HCal(int nevents = 1e3, const std::string &fname = "inputdata.txt", std::string dataset = "", std::string ohcalCalib = "", std::string ihcalCalib = "")
{

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  CDBInterface::instance()->Verbosity(1);

  // se->Verbosity(verbosity);
  recoConsts *rc = recoConsts::instance();

  std::ifstream file(fname);
  std::string first_file;
  getline(file, first_file);

  //===============
  // conditions DB flags
  //===============
  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
  int runnumber = runseg.first;
  std::cout << "run number = " << runnumber << std::endl;

  //===============
  // dataset and calibration files (HcalCalibDatasetinputs.h)
  //===============
  if (dataset.empty())
  {
    dataset = askDataset(getHCalCalibDataset(runnumber).name);
  }
  const HCalCalibDataset *ds = findHCalCalibDataset(dataset);
  if (!ds)
  {
    std::cout << "no dataset " << dataset << " in HcalCalibDatasetinputs.h (" << hcalCalibDatasetNames() << ")" << std::endl;
    gSystem->Exit(1);
  }
  if (ohcalCalib.empty())
  {
    ohcalCalib = "cosmics";
  }
  if (ihcalCalib.empty())
  {
    ihcalCalib = "cosmics";
  }
  std::string calib_fname_oh = findCalib(*ds, 0, runnumber, ohcalCalib);
  std::string calib_fname_ih = findCalib(*ds, 1, runnumber, ihcalCalib);
  std::cout << "dataset           : " << ds->name << std::endl;
  std::cout << "OHCal calibration : " << (calib_fname_oh.empty() ? "conditions database" : calib_fname_oh) << std::endl;
  std::cout << "IHCal calibration : " << (calib_fname_ih.empty() ? "conditions database" : calib_fname_ih) << std::endl;

  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", runnumber);

  Fun4AllInputManager *in = new Fun4AllDstInputManager("DST_TOWERS");
  in->AddListFile(fname);
  se->registerInputManager(in);

  // MBD/BBC Reconstruction
  MbdReco *mbdreco = new MbdReco();
  se->registerSubsystem(mbdreco);
  // Official vertex storage
  GlobalVertexReco *gvertex = new GlobalVertexReco();
  se->registerSubsystem(gvertex);

  /////////////////////////////////////////
  // Hot tower masking for inner and outer hcal
  CaloTowerStatus *statusHCalIn = new CaloTowerStatus("HCALINSTATUS");
  statusHCalIn->set_detector_type(CaloTowerDefs::HCALIN);
  se->registerSubsystem(statusHCalIn);

  CaloTowerStatus *statusHCALOUT = new CaloTowerStatus("HCALOUTSTATUS");
  statusHCALOUT->set_detector_type(CaloTowerDefs::HCALOUT);
  se->registerSubsystem(statusHCALOUT);

  //////////////////////
  // calibrate towers - takes raw towers and calibrate thems
  CaloTowerCalib *calibOhcal = new CaloTowerCalib("HCALOUTCALIB");
  calibOhcal->set_detector_type(CaloTowerDefs::HCALOUT);
  if (!calib_fname_oh.empty())
  {
    calibOhcal->set_directURL(calib_fname_oh);
  }
  se->registerSubsystem(calibOhcal);

  CaloTowerCalib *calibIhcal = new CaloTowerCalib("HCALINCALIB");
  calibIhcal->set_detector_type(CaloTowerDefs::HCALIN);
  if (!calib_fname_ih.empty())
  {
    calibIhcal->set_directURL(calib_fname_ih);
  }
  se->registerSubsystem(calibIhcal);

  std::string filename = first_file.substr(first_file.find_last_of("/\\") + 1);
  std::string OutFile = std::format("OUTHIST_OHCAL_{}" , filename);
  std::string OutFile2 = std::format("OUTHIST_IHCAL_{}" , filename);

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
  //  CaloTemp* calotemp = new CaloTemp("CALOTEMP",std::format("ohcal_temp_hist_{}.root",runnumber));
  //  calotemp->Detector("HCALOUT");
  //  se->registerSubsystem(calotemp);

  //  CaloTemp* calotemp2 = new CaloTemp("CALOTEMP",std::format("ihcal_temp_hist_{}.root",runnumber));
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



// ask for the dataset, an empty answer gives the default. Not run in a terminal (condor job): the default is used.
std::string askDataset(const std::string &def)
{
  if (!isatty(STDIN_FILENO))
  {
    return def;
  }
  std::cout << "dataset (" << hcalCalibDatasetNames() << ") [" << def << "]: " << std::flush;
  std::string answer;
  std::getline(std::cin, answer);
  size_t b = answer.find_first_not_of(" \t");
  size_t e = answer.find_last_not_of(" \t\r");
  return (b == std::string::npos) ? def : answer.substr(b, e - b + 1);
}

// calibration file of a detector for a run, empty for the calibration of the conditions database
std::string findCalib(const HCalCalibDataset &ds, int isIHcal, int runnumber, std::string calib)
{
  if (calib == "cdb")
  {
    return "";
  }
  if (calib.empty() || calib == "cosmics")
  {
    calib = (isIHcal == 1) ? findCosmicCalib_ihcal(ds, runnumber) : findCosmicCalib(ds, runnumber);
  }
  else if (calib == "cosmics_tsc")
  {
    calib = findTscCalib(ds, isIHcal, runnumber);
  }
  else if (calib == "cosmics_tsc_temp")
  {
    const std::string &dir = (isIHcal == 1) ? ds.ih_finalDir : ds.oh_finalDir;
    if (dir.empty())
    {
      std::cout << "no folder with the final files (" << (isIHcal == 1 ? "ih_finalDir" : "oh_finalDir") << ") for " << ds.name << " in HcalCalibDatasetinputs.h" << std::endl;
      gSystem->Exit(1);
    }
    calib = findRunByRunCalib(dir, runnumber);
  }
  else if (!gSystem->AccessPathName(calib.c_str()) && gSystem->OpenDirectory(calib.c_str()))
  {
    calib = findRunByRunCalib(calib, runnumber);
  }
  if (calib.empty() || gSystem->AccessPathName(calib.c_str()))
  {
    std::cout << "did not find the " << (isIHcal == 1 ? "IHCal" : "OHCal") << " calibration file \"" << calib << "\" for run " << runnumber << std::endl;
    gSystem->Exit(1);
  }
  return calib;
}

// cosmics + towerslope: the file of the towerslope period the run is in, or of the last cosmics run
// at or before the run for datasets without periods ({o,i}hcal_cdb_tsc_cos_calib_<run>.root in tscDir)
std::string findTscCalib(const HCalCalibDataset &ds, int isIHcal, int runnumber)
{
  if (ds.tscDir.empty())
  {
    std::cout << "no folder with the cosmics + towerslope files (tscDir) for " << ds.name << " in HcalCalibDatasetinputs.h" << std::endl;
    return "";
  }
  const std::vector<int> &runs = !ds.tscPeriods.empty() ? ds.tscPeriods : ((isIHcal == 1) ? ds.ih_cosmicRuns : ds.oh_cosmicRuns);
  int tscRun = 0;
  for (int run : runs)
  {
    if (run <= runnumber && run > tscRun) tscRun = run;
  }
  if (tscRun == 0)
  {
    std::cout << "run " << runnumber << " is before the first towerslope period or cosmics run of " << ds.name << std::endl;
    return "";
  }
  return std::format("{}/{}_cdb_tsc_cos_calib_{}.root", ds.tscDir, (isIHcal == 1) ? "ihcal" : "ohcal", tscRun);
}

// final calibration: the file <name>_<first run>_<last run>*.root of the folder whose run range holds the run
std::string findRunByRunCalib(const std::string &dir, int runnumber)
{
  TSystemDirectory sysdir(dir.c_str(), dir.c_str());
  TList *list = sysdir.GetListOfFiles();
  if (list)
  {
    for (TObject *obj : *list)
    {
      std::string name = obj->GetName();
      if (name.size() < 5 || name.substr(name.size() - 5) != ".root") continue;
      // the run range is the two run numbers in the name: ..._<first run>_<last run>...
      std::smatch match;
      if (!std::regex_search(name, match, std::regex("_([0-9]{5,6})_([0-9]{5,6})[_.]"))) continue;
      int first = std::stoi(match[1]);
      int last = std::stoi(match[2]);
      if (runnumber >= first && runnumber <= last)
      {
        return dir + "/" + name;
      }
    }
  }
  std::cout << "no file with a run range that holds run " << runnumber << " in " << dir << std::endl;
  return "";
}

#endif
