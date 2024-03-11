#ifndef FUN4ALL_CALOPRODUCTION_C
#define FUN4ALL_CALOPRODUCTION_C

#include <caloreco/CaloTowerBuilder.h>
#include <caloreco/CaloWaveformProcessing.h>
#include <caloreco/CaloGeomMapping.h>

#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>
#include <ffamodules/CDBInterface.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>


#include <phool/recoConsts.h>



R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalo_reco.so)
R__LOAD_LIBRARY(libffamodules.so)

void Fun4All_CaloProduction(const std::string &fname = "/sphenix/user/trinn/combined_event_prdfs/calib-00007377-0000.prdf", const std::string &outfile = "testfile.root", int nskip = 0, int nevents = 0)
{ 
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
 recoConsts *rc = recoConsts::instance();

 //===============
  // conditions DB flags
  //===============
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

  //default is FAST
  CaloTowerBuilder *ca3 = new CaloTowerBuilder();
  ca3->set_detector_type(CaloTowerBuilder::ZDC);
  ca3->set_nsamples(31);
  se->registerSubsystem(ca3);

  CaloGeomMapping *cgm = new CaloGeomMapping();
  cgm->set_detector_name("CEMC");
  se->registerSubsystem(cgm);

  CaloGeomMapping *cgm1 = new CaloGeomMapping();
  cgm1->set_detector_name("HCALIN");
  se->registerSubsystem(cgm1);

  CaloGeomMapping *cgm2 = new CaloGeomMapping();
  cgm2->set_detector_name("HCALOUT");
  se->registerSubsystem(cgm2);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("in");
  in->fileopen(fname);
  se->registerInputManager(in);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile);
  se->registerOutputManager(out);


  se->skip(nskip);
  se->run(nevents);
  CDBInterface::instance()->Print(); // print used DB files
  se->End();
  se->PrintTimer();
  gSystem->Exit(0);
}

#endif
