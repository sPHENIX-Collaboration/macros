#ifndef TESTCDBINSERT_C
#define TESTCDBINSERT_C

#include <sphenixnpc/sphenixnpc.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libsphenixnpc.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBInsert()
{
  recoConsts *rc = recoConsts::instance();
// please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu");
  sphenixnpc *cdb = new sphenixnpc(rc->get_StringFlag("CDB_GLOBALTAG"));
  cdb->createDomain("TestBeginValidity");
  cdb->insertcalib("TestBeginValidity","test.root",10);
  cdb->createDomain("TestBeingEndValidity");
  cdb->insertcalib("TestBeingEndValidity","test.root",10,20);
  return;
}

#endif
