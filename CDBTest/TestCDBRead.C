#ifndef TESTCDBREAD_C
#define TESTCDBREAD_C

#include <ffamodules/CDBInterface.h>

#include <phool/recoConsts.h>

#include <TSystem.h>

#include <iostream>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBRead()
{
  recoConsts *rc = recoConsts::instance();
// this is our floating cdb tag which contains all up to date data calibrations
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", 61272);
  CDBInterface *cdb = CDBInterface::instance();
  std::cout << cdb->getUrl("Tracking_Geometry") << std::endl;
  gSystem->Exit(0);
  return;
}

#endif
