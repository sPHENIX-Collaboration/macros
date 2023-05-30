#ifndef TESTCDBINSERT_C
#define TESTCDBINSERT_C

#include <sphenixnpc/CDBUtils.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libsphenixnpc.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBInsert()
{
  recoConsts *rc = recoConsts::instance();
// please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu");
  CDBUtils *cdb = new CDBUtils(rc->get_StringFlag("CDB_GLOBALTAG"));
  cdb->createPayloadType("TestBeginValidity");
  cdb->insertPayload("TestBeginValidity","test.root",10);
  cdb->createPayloadType("TestBeingEndValidity");
  cdb->insertPayload("TestBeingEndValidity","test.root",10,20);
  return;
}

#endif
