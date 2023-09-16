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
  cout << "using insert timestamp to retrieve" << endl;
  rc->set_uint64Flag("TIMESTAMP",10);
  cout << cdb->getUrl("TestBeginValidity") << endl;
  cout << "using larger timestamp to retrieve" << endl;
  rc->set_uint64Flag("TIMESTAMP",100);
  cout << cdb->getUrl("TestBeginValidity") << endl;
  cout << "using smaller timestamp to retrieve" << endl;
  rc->set_uint64Flag("TIMESTAMP",1);
  cout << cdb->getUrl("TestBeginValidity") << endl;

  cout << "using timestamp in range to retrieve calibration" << endl;
  rc->set_uint64Flag("TIMESTAMP",15);
  cout << cdb->getUrl("TestBeginEndValidity") << endl;
  cout << "using timestamp outside range to retrieve calibration" << endl;
  rc->set_uint64Flag("TIMESTAMP",25);
  cout << cdb->getUrl("TestBeginEndValidity") << endl;

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

