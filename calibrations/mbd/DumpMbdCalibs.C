// This example runs the CDBInterface like in the real reconstruction
// once the CDBInterface instance is created it registers itself with the 
// Fun4AllServer and creates (or adds to) the CdbUrl Node which contains a 
// record of the files and timestamps which were used
#ifndef DUMPMBDCALIBS_C
#define DUMPMBDCALIBS_C

#include <TSystem.h>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDummyInputManager.h>

#include <phool/recoConsts.h>
#include "mbd/MbdCalib.h"

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libmbd_io.so)

// pcdb001
void DumpMbdCalibs(const int runno, const char* dbtag = "newcdbtag")
{
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG",dbtag); 
  rc->set_uint64Flag("TIMESTAMP",runno);
  rc->set_uint64Flag("RUNNUMBER",runno);
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

  /*
  CDBInterface *cdb = CDBInterface::instance();
  std::string timecorr_url = cdb->getUrl("MBD_TIMECORR");
  std::string slewcorr_url = cdb->getUrl("MBD_SLEWCORR");
  cout << "xxxurl: " << cdb->getUrl("MBD_SAMPMAX") << endl;
  cout << "xxxurl: " << cdb->getUrl("MBD_QFIT") << endl;
  cout << "xxxurl: " << cdb->getUrl("MBD_T0CORR") << endl;
  std::string t0corr_url = cdb->getUrl("MBD_T0CORR");
  cout << t0corr_url.size() << endl;
  */

  MbdCalib *mcal = new MbdCalib();
  mcal->Verbosity(1);
  mcal->Download_All();
  mcal->Write_All();
  //mcal->Download_TimeCorr(timecorr_url);
  //mcal->Write_TimeCorr("tcorr.calib");
  //mcal->Download_SlewCorr(slewcorr_url);
  //mcal->Write_SlewCorr("slewcorr.calib");
  //mcal->Write_TQT0("a.calib");

  std::cout << "All done" << std::endl;

  gSystem->Exit(0);
  return;
}

#endif  // DUMPMBDCALIBS_C

