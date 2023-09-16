#ifndef TESTCDBREAD_C
#define TESTCDBREAD_C

#include <ffamodules/CDBInterface.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBRead()
{
  recoConsts *rc = recoConsts::instance();
// please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu"); 
  rc->set_uint64Flag("TIMESTAMP",6);
// 1000000 is the insert timestamp. Higher timestamps work, lower time stamps do not
  CDBInterface *cdb = CDBInterface::instance();
  cout << "using insert timestamp to retrieve no end time payload" << endl;
  rc->set_uint64Flag("TIMESTAMP",10);
  cout << cdb->getUrl("TestBeginValidity") << endl;
  cout << "using larger timestamp to retrieve no end time payload" << endl;
  rc->set_uint64Flag("TIMESTAMP",100);
  cout << cdb->getUrl("TestBeginValidity") << endl;
  cout << "using smaller timestamp to retrieve no end time payload" << endl;
  rc->set_uint64Flag("TIMESTAMP",1);
  cout << cdb->getUrl("TestBeginValidity") << endl;

  cout << "using timestamp in range to retrieve calibration with end time" << endl;
  rc->set_uint64Flag("TIMESTAMP",15);
  cout << cdb->getUrl("TestBeginEndValidity") << endl;
  cout << "using timestamp outside range to retrieve calibration with end time" << endl;
  rc->set_uint64Flag("TIMESTAMP",25);
  cout << cdb->getUrl("TestBeginEndValidity") << endl;
  gSystem->Exit(0);
  return;
}

#endif

