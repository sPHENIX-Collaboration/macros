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
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu"); 
  rc->set_uint64Flag("TIMESTAMP",6);
// 1000000 is the insert timestamp. Higher timestamps work, lower time stamps do not
  CDBInterface *cdb = CDBInterface::instance();
  std::cout << "using insert timestamp to retrieve no end time payload" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",10);
  std::cout << cdb->getUrl("TestBeginValidity") << std::endl;
  std::cout << "using larger timestamp to retrieve no end time payload" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",100);
  std::cout << cdb->getUrl("TestBeginValidity") << std::endl;
  std::cout << "using smaller timestamp to retrieve no end time payload" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",1);
  std::cout << cdb->getUrl("TestBeginValidity") << std::endl;

  std::cout << "using timestamp in range to retrieve calibration with end time" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",15);
  std::cout << cdb->getUrl("TestBeginEndValidity") << std::endl;
  std::cout << "using timestamp outside range to retrieve calibration with end time" << std::endl;
  rc->set_uint64Flag("TIMESTAMP",25);
  std::cout << cdb->getUrl("TestBeginEndValidity") << std::endl;
  gSystem->Exit(0);
  return;
}

#endif

