#include <phool/recoConsts.h>
#include <ffamodules/CDBInterface.h>

#include <TSystem.h>

#include <iostream>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)

void TestGarfield()
{
  recoConsts *rc = recoConsts::instance();
  rc->set_StringFlag("CDB_GLOBALTAG","newcdbtag"); 
  rc->set_uint64Flag("TIMESTAMP",6);
  CDBInterface *cdb = CDBInterface::instance();
  std::cout << cdb->getUrl("PHGARFIELD_GAS") << std::endl;
  gSystem->Exit(0);
  return;
}
