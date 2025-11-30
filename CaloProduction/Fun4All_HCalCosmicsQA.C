#ifndef FUN4ALL_HCALCOSMICSQA_C
#define FUN4ALL_HCALCOSMICSQA_C

#include <calovalid/CaloValid.h>

#include <litecaloeval/HCalCosmics.h>

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/CaloTowerStatus.h>
#include <caloreco/CaloWaveformProcessing.h>

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

#include <phool/recoConsts.h>

#include <Rtypes.h>
#include <TSystem.h>

#include <format>
#include <fstream>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libcentrality.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

void Fun4All_HCalCosmicsQA(int nEvents = 100,
                             const std::string& inlist = "files.list",
                             const std::string &outfile_hist1 = "HIST_COSMIC_HCALOUT_run2auau_ana487_2024p018_v001",
                             const std::string &outfile_hist2 = "HIST_COSMIC_HCALIN_run2auau_ana487_2024p018_v001",
                             const std::string &dbtag = "ProdA_2024")
{
  bool useDSTRAW = true;
  // v1 uncomment:
  // CaloTowerDefs::BuilderType buildertype = CaloTowerDefs:::kPRDFTowerv1;
  // v2 uncomment:
  CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kWaveformTowerv2;
  // v3 uncomment:

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
  se->VerbosityDownscale(1000);

  recoConsts *rc = recoConsts::instance();

  // conditions DB global tag
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  CDBInterface::instance()->Verbosity(1);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);

  /////////////////
  // build towers
  CaloTowerBuilder *ctbIHCal = new CaloTowerBuilder("HCALINBUILDER");
  ctbIHCal->set_detector_type(CaloTowerDefs::HCALIN);
  ctbIHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbIHCal->set_builder_type(buildertype);
  ctbIHCal->set_nsamples(12);
  if (useDSTRAW) ctbIHCal->set_offlineflag();
  ctbIHCal->set_softwarezerosuppression(true, 200);
  se->registerSubsystem(ctbIHCal);

  CaloTowerBuilder *ctbOHCal = new CaloTowerBuilder("HCALOUTBUILDER");
  ctbOHCal->set_detector_type(CaloTowerDefs::HCALOUT);
  ctbOHCal->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  ctbOHCal->set_builder_type(buildertype);
  ctbOHCal->set_nsamples(12);
  if (useDSTRAW) ctbOHCal->set_offlineflag();
  ctbOHCal->set_softwarezerosuppression(true, 200);
  se->registerSubsystem(ctbOHCal);

  CaloTowerStatus *statusHCalIn = new CaloTowerStatus("HCALINSTATUS");
  statusHCalIn->set_detector_type(CaloTowerDefs::HCALIN);
  statusHCalIn->set_time_cut(10);
  se->registerSubsystem(statusHCalIn);

  CaloTowerStatus *statusHCALOUT = new CaloTowerStatus("HCALOUTSTATUS");
  statusHCALOUT->set_detector_type(CaloTowerDefs::HCALOUT);
  statusHCALOUT->set_time_cut(10);
  se->registerSubsystem(statusHCALOUT);

  ////////////////////
  // Calibrate towers
  std::cout << "Calibrating OHcal" << std::endl;
  CaloTowerCalib *calibOHCal = new CaloTowerCalib("HCALOUT");
  calibOHCal->set_detector_type(CaloTowerDefs::HCALOUT);
  se->registerSubsystem(calibOHCal);

  std::cout << "Calibrating IHcal" << std::endl;
  CaloTowerCalib *calibIHCal = new CaloTowerCalib("HCALIN");
  calibIHCal->set_detector_type(CaloTowerDefs::HCALIN);
  se->registerSubsystem(calibIHCal);

  // loop over all files in file list and create an input manager for each one
  Fun4AllInputManager *In = nullptr;
  std::ifstream infile;
  infile.open(inlist);
  int iman = 0;
  std::string line;
  bool first{true};
  int runnumber = 0;
  int segment = 99999;
  if (infile.is_open())
  {
    while (std::getline(infile, line))
    {
      if (line[0] == '#')
      {
        std::cout << "found commented out line " << line << std::endl;
        continue;
      }
      // extract run number from first not commented out file in list
      if (first)
      {
	std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(line);
        runnumber = runseg.first;
        segment = runseg.second;
        rc->set_uint64Flag("TIMESTAMP", runnumber);
        first = false;
      }
      std::string magname = "DSTin_" + std::to_string(iman);
      In = new Fun4AllDstInputManager(magname);
      In->Verbosity(1);
      In->AddFile(line);
      se->registerInputManager(In);
      iman++;
    }
    infile.close();
  }
  std::cout << "Adding Geometry file" << std::endl;
  Fun4AllInputManager *intrue2 = new Fun4AllRunNodeInputManager("DST_GEO");
  std::string geoLocation = CDBInterface::instance()->getUrl("calo_geo");
  intrue2->AddFile(geoLocation);
  se->registerInputManager(intrue2);

  ///////////////////////////////////////////
  // Cosmics histMaker
  std::string outfile_hist = std::format("{}-{:08}-{:05}.root",outfile_hist2, runnumber, segment);
  HCalCosmics *wt2 = new HCalCosmics("HCalCalib_HCALIN", outfile_hist);
  wt2->set_tower_threshold(0.2498); 
  wt2->set_vert_threshold(0.2498);  
  wt2->set_veto_threshold(0.17486); 
  wt2->HistBinWidth(0.01);
  wt2->Detector("HCALIN");
  wt2->TowerPrefix("TOWERINFO_CALIB_");
  se->registerSubsystem(wt2);

  outfile_hist = std::format("{}-{:08}-{:05}.root",outfile_hist1, runnumber, segment);
  HCalCosmics *wt3 = new HCalCosmics("HCalCosmics_HCALOUT", outfile_hist);
  wt3->set_tower_threshold(1.665); 
  wt3->set_vert_threshold(1.665);  
  wt3->set_veto_threshold(1.1655); 
  wt3->HistBinWidth(0.05);
  wt3->Detector("HCALOUT");
  wt3->TowerPrefix("TOWERINFO_CALIB_");
  se->registerSubsystem(wt3);


  se->run(nEvents);
  se->End();
  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;

  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}

#endif
