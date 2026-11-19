#ifndef FUN4ALL_FITTING_C
#define FUN4ALL_FITTING_C

// This macro runs the Calo Tower Building over raw data files
// we do not do this in the production, this macro can be used to quickly
// build towers from single raw data files. It works for all prdf's
// (cemc (seb00-seb15), hcal (seb16,17), mbd (seb18) or sepd/zdc (seb20))

// There are four test files in cvmfs (guaranteed to be there) containing 1000 events each:
// /cvmfs/sphenix.sdcc.bnl.gov/files/prdf/physics_seb00-00078645-9000.prdf
// /cvmfs/sphenix.sdcc.bnl.gov/files/prdf/physics_seb16-00078645-9000.prdf
// /cvmfs/sphenix.sdcc.bnl.gov/files/prdf/physics_seb18-00078645-9000.prdf
// /cvmfs/sphenix.sdcc.bnl.gov/files/prdf/physics_seb20-00078645-9000.prdf

// For files which are on disk you can just use the filename, Fun4All uses the filecatalog
// to locate them - you do not need the full path

#include <Calo_Fitting.C>
#include <QA.C>

#include <calotrigger/TriggerRunInfoReco.h>

#include <calovalid/CaloFittingQA.h>

#include <calopacketskimmer/CaloPacketSkimmer.h>

#include <mbd/MbdReco.h>

#include <ffamodules/CDBInterface.h>
#include <ffamodules/FlagHandler.h>
#include <ffamodules/HeadReco.h>
#include <ffamodules/SyncReco.h>

#include <fun4all/Fun4AllDstInputManager.h>
#include <fun4all/Fun4AllDstOutputManager.h>
#include <fun4all/Fun4AllInputManager.h>
#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <fun4allraw/Fun4AllPrdfInputManager.h>

#include <phool/recoConsts.h>
#include <phool/RunnumberRange.h>

#include <TSystem.h>

#include <fstream>

R__LOAD_LIBRARY(libfun4allraw.so)
R__LOAD_LIBRARY(libcalovalid.so)
R__LOAD_LIBRARY(libcalotrigger.so)

// this pass containis the reco process that's stable wrt time stamps(raw tower building)
void Fun4All_Raw_Fitting(int nEvents = 100,
                           const std::string &infile = "/cvmfs/sphenix.sdcc.bnl.gov/files/prdf/physics_seb00-00078645-9000.prdf",
                           const std::string &outfile1 = "DST_CALOFITTING.root",
                           const std::string &dbtag = "newcdbtag")
{
  gSystem->Load("libg4dst.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);
//  se->VerbosityDownscale(10000);

  recoConsts *rc = recoConsts::instance();

  std::pair<int, int> runseg = Fun4AllUtils::GetRunSegment(infile);
  int runnumber=runseg.first;

  // conditions DB flags and timestamp
  rc->set_StringFlag("CDB_GLOBALTAG", dbtag);
  rc->set_uint64Flag("TIMESTAMP", runnumber);
  CDBInterface::instance()->Verbosity(1);

  FlagHandler *flag = new FlagHandler();
  se->registerSubsystem(flag);
  CaloTowerDefs::BuilderType buildertype = CaloTowerDefs::kPRDFTowerv4;
  Process_Calo_Fitting();
  if (infile.find("seb18") == std::string::npos)
  {
    se->unregisterSubsystem(se->getSubsysReco("MbdReco"));
  }
  CaloTowerBuilder *caZDC = new CaloTowerBuilder("ZDCBUILDER");
  caZDC->set_detector_type(CaloTowerDefs::ZDC);
  caZDC->set_builder_type(buildertype);
  //if ((runnumber > RunnumberRange::RUN2PP_FIRST && runnumber < RunnumberRange::RUN2PP_LAST) || (runnumber > RunnumberRange::RUN3PP_FIRST && runnumber < RunnumberRange::RUN3PP_LAST))
  //{
  //caZDC->set_processing_type(CaloWaveformProcessing::FAST);
  //}
  //else
  //{

  // Now all systems and calibrations use FUNCFIT type 2
  
  caZDC->set_processing_type(CaloWaveformProcessing::FUNCFIT);
  caZDC->set_funcfit_type(2);
  //}
  se->registerSubsystem(caZDC);
  Fun4AllInputManager *In = new Fun4AllPrdfInputManager("PRDFin");
  In->AddFile(infile);
  se->registerInputManager(In);

  Fun4AllDstOutputManager *out = new Fun4AllDstOutputManager("DSTOUT", outfile1);
  se->registerOutputManager(out);


  if (nEvents < 0)
  {
    return;
  }
  se->run(nEvents);
  se->End();

  CDBInterface::instance()->Print();  // print used DB files
  se->PrintTimer();
  delete se;
  std::cout << "All done!" << std::endl;
  gSystem->Exit(0);
}
#endif
