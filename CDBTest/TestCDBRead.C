#ifndef TESTCDBREAD_C
#define TESTCDBREAD_C

#include <sphenixnpc/sphenixnpc.h>

#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libsphenixnpc.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBRead()
{
  recoConsts *rc = recoConsts::instance();
// please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu"); 
// 1000000 is the insert timestamp. Higher timestamps work, lower time stamps do not
  sphenixnpc *cdb = sphenixnpc::instance(rc->get_StringFlag("CDB_GLOBALTAG"));
  cout << "using insert timestamp to retrieve" << endl;
  cout << cdb->getCalibrationFile("TestBeginValidity",10) << endl;
  cout << "using larger timestamp to retrieve" << endl;
  cout << cdb->getCalibrationFile("TestBeginValidity",100) << endl;
  cout << "using smaller timestamp to retrieve" << endl;
  cout << cdb->getCalibrationFile("TestBeginValidity",1) << endl;

  cout << "using timestamp in range to retrieve calibration" << endl;
  cout << cdb->getCalibrationFile("TestBeingEndValidity",15) << endl;
  cout << "using timestamp outside range to retrieve calibration" << endl;
  cout << cdb->getCalibrationFile("TestBeingEndValidity",25) << endl;
   
  return;
}

#endif

