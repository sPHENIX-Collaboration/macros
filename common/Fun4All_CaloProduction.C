#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include <fun4all/SubsysReco.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4allraw/Fun4AllPrdfInputManager.h>
#include <fun4all/Fun4AllDstInputManager.h>
#include <phool/recoConsts.h>

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloWaveformProcessing.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllDstOutputManager.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)

#endif
void Fun4All_CaloProduction(const char *fname = "/gpfs/mnt/gpfs02/sphenix/user/trinn/combined_event_prdfs/calib-00007377-0000.prdf", const char *outfile = "testfile.root", int nskip = 0, int nevents = 0)
{ 
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
 recoConsts *rc = recoConsts::instance();

 //===============
  // conditions DB flags
  //===============
  // ENABLE::CDB = true;
  // global tag
  rc->set_StringFlag("CDB_GLOBALTAG","MDC2");
  rc->set_uint64Flag("TIMESTAMP",6);

  CaloTowerBuilder *ca = new CaloTowerBuilder();
  ca->set_detector_type(CaloTowerBuilder::CEMC);
  ca->set_nsamples(31);
  ca->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  se->registerSubsystem(ca);

  CaloTowerBuilder *ca1 = new CaloTowerBuilder();
  ca1->set_detector_type(CaloTowerBuilder::HCALIN);
  ca1->set_nsamples(31);
  ca1->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  se->registerSubsystem(ca1);

  CaloTowerBuilder *ca2 = new CaloTowerBuilder();
  ca2->set_detector_type(CaloTowerBuilder::HCALOUT);
  ca2->set_nsamples(31);
  ca2->set_processing_type(CaloWaveformProcessing::TEMPLATE);
  se->registerSubsystem(ca2);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
  in->fileopen(fname);
  se->registerInputManager(in);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile);
  se->registerOutputManager(out);


  se->skip(nskip);
  se->run(nevents);
  se->End();
  se->PrintTimer();
  gSystem->Exit(0);
}
