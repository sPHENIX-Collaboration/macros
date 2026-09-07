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
  // please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG", "newcdbtag");
  rc->set_uint64Flag("TIMESTAMP", 61272);
  // 1000000 is the insert timestamp. Higher timestamps work, lower time stamps do not
  CDBInterface *cdb = CDBInterface::instance();
  std::cout << cdb->getUrl("Tracking_Geometry") << std::endl;
  gSystem->Exit(0);
  return;
}

#endif
