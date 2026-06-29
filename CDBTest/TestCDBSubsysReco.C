// This example runs the CDBInterface like in the real reconstruction
// once the CDBInterface instance is created it registers itself with the 
// Fun4AllServer and creates (or adds to) the CdbUrl Node which contains a 
// record of the files and timestamps which were used
#ifndef TESTCDBREAD_C
#define TESTCDBREAD_C

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllDummyInputManager.h>

#include <phool/recoConsts.h>

#include <Rtypes.h>
#include <TSystem.h>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBSubsysReco()
{
  recoConsts *rc = recoConsts::instance();
// please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu"); 
  rc->set_uint64Flag("TIMESTAMP",6);
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(1);

  CDBInterface *cdb = CDBInterface::instance();
  std::cout << "using insert timestamp to retrieve" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",10);
  std::cout << cdb->getUrl("TestBeginValidity") << std::endl;
  std::cout << "using larger timestamp to retrieve" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",100);
  std::cout << cdb->getUrl("TestBeginValidity") << std::endl;
  std::cout << "using smaller timestamp to retrieve" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",1);
  std::cout << cdb->getUrl("TestBeginValidity") << std::endl;

  std::cout << "using timestamp in range to retrieve calibration" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",15);
  std::cout << cdb->getUrl("TestBeginEndValidity") << std::endl;
  std::cout << "using timestamp outside range to retrieve calibration" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",25);
  std::cout << cdb->getUrl("TestBeginEndValidity") << std::endl;

  Fun4AllInputManager *in = new Fun4AllDummyInputManager("JADE");
  in->Verbosity(1);
  se->registerInputManager(in);
  se->run(1);
  CDBInterface::instance()->Print(); // print used DB files
  se->End();
  se->Print("NODETREE");
  std::cout << "All done" << std::endl;
  delete se;

  gSystem->Exit(0);
  return;
}

#endif

