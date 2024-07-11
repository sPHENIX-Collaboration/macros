#ifndef FUN4ALL_HCALCALIB_C
#define FUN4ALL_HCALCALIB_C

#include <caloreco/CaloTowerBuilder.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <fun4all/Fun4AllUtils.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <phool/recoConsts.h>

#include <cosmictree/HCalCalibTree.h>
#include <caloana/CaloTemp.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libHCalCalibTree.so)
R__LOAD_LIBRARY(libcalotemp.so)


void Fun4All_HCalCalib(int nevents = 5e4, const std::string &fname = "inputdata.txt") {

    Fun4AllServer *se = Fun4AllServer::instance();
    se->Verbosity(0);
    recoConsts *rc = recoConsts::instance();

    Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
    in->AddListFile(fname);
    se->registerInputManager(in);

    ifstream file(fname);
    std::string first_file;
    getline(file, first_file);

    pair<int, int> runseg = Fun4AllUtils::GetRunSegment(first_file);
    int runnumber = runseg.first;

    //===============
    // conditions DB flags
    //===============
    rc->set_StringFlag("CDB_GLOBALTAG", "MDC2");
    rc->set_uint64Flag("TIMESTAMP", 6);

    // Sync Headers and Flags
    SyncReco *sync = new SyncReco();
    se->registerSubsystem(sync);

    HeadReco *head = new HeadReco();
    se->registerSubsystem(head);


    CaloTowerBuilder *ca2 = new CaloTowerBuilder();
    ca2->set_detector_type(CaloTowerDefs::HCALIN);
    ca2->set_nsamples(12);
    ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);//TEMPLATE
    //ca2->set_processing_type(CaloWaveformProcessing::FAST);
    ca2->set_builder_type(CaloTowerDefs::kWaveformTowerv2);
    ca2->set_outputNodePrefix("TOWERSV2_");
    ca2->set_softwarezerosuppression(true, 200);
    se->registerSubsystem(ca2);

    CaloTowerBuilder *ca3 = new CaloTowerBuilder();
    ca3->set_detector_type(CaloTowerDefs::HCALOUT);
    ca3->set_nsamples(12);
    ca3->set_processing_type(CaloWaveformProcessing::TEMPLATE);//TEMPLATE
    //ca3->set_processing_type(CaloWaveformProcessing::FAST);
    ca3->set_builder_type(CaloTowerDefs::kWaveformTowerv2);
    ca3->set_outputNodePrefix("TOWERSV2_");
    ca3->set_softwarezerosuppression(true, 200);
    se->registerSubsystem(ca3);

    HCalCalibTree *wt2 = new HCalCalibTree("HCalCalib_TREE_2",Form("ihcal_hist_%d.root",runnumber));
    wt2->Detector("HCALIN");
    se->registerSubsystem(wt2);

    HCalCalibTree *wt3 = new HCalCalibTree("HCalCalib_TREE_3",Form("ohcal_hist_%d.root",runnumber));
    wt3->Detector("HCALOUT");
    se->registerSubsystem(wt3);


    se->run(nevents);
    CDBInterface::instance()->Print(); // print used DB files
    se->End();
    se->PrintTimer();
    gSystem->Exit(0);
}

#endif
