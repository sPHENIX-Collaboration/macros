#ifndef TESTCDBINSERT_C
#define TESTCDBINSERT_C

#include <dbtools/InsertCalib.h>
#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libdbtools.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBInsert()
{
  recoConsts *rc = recoConsts::instance();
// please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu"); 
  InsertCalib *ins = new InsertCalib();
  ins->createGlobalTag(rc->get_StringFlag("CDB_GLOBALTAG"));
  ins->createPayloadType("TestBeginValidity");
  ins->insertcalib("test.root","TestBeginValidity",10);
  ins->createPayloadType("TestBeingEndValidity");
  ins->insertcalib("test.root","TestBeingEndValidity",10,20);
  return;
}

#endif

