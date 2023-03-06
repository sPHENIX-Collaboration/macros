#ifndef TESTCDBREAD_C
#define TESTCDBREAD_C

#include <dbtools/ReadCalib.h>
#include <phool/recoConsts.h>

R__LOAD_LIBRARY(libdbtools.so)
R__LOAD_LIBRARY(libphool.so)

void TestCDBRead()
{
  recoConsts *rc = recoConsts::instance();
// please choose a unique name, if it is your username it's easier to see who created it
  rc->set_StringFlag("CDB_GLOBALTAG","pinkenbu"); 
  ReadCalib *rb = new ReadCalib();
// 1000000 is the insert timestamp. Higher timestamps work, lower time stamps do not
  cout << "using insert timestamp to retrieve" << endl;
  cout << rb->getCalibrationFile("TestBeginValidity",10) << endl;
  cout << "using larger timestamp to retrieve" << endl;
  cout << rb->getCalibrationFile("TestBeginValidity",100) << endl;
  cout << "using smaller timestamp to retrieve" << endl;
  cout << rb->getCalibrationFile("TestBeginValidity",1) << endl;

  cout << "using timestamp in range to retrieve calibration" << endl;
  cout << rb->getCalibrationFile("TestBeingEndValidity",15) << endl;
  cout << "using timestamp outside range to retrieve calibration" << endl;
  cout << rb->getCalibrationFile("TestBeingEndValidity",25) << endl;
   
  return;
}

#endif

