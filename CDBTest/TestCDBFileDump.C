#ifndef TESTCDBFILEDUMP_C
#define TESTCDBFILEDUMP_C

#include <ffamodules/CDBInterface.h>

#include <phool/recoConsts.h>

#include <TSystem.h>

#include <iostream>

R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libphool.so)


// if you want to use calibration outside of scdf, you need to source the sphenix_setup script
// from the opensciencegrid cvmfs area
// source /cvmfs/sphenix.opensciencegrid.org/alma9.2-gcc-14.2.0/opt/sphenix/core/bin/sphenix_setup.csh -n

// during reading there is no check on the global tag or timestamp (runnumber). All this is set
// during the writing of the calibration list file.

void WriteFile(uint64_t timestamp = 82476)
{
  recoConsts *rc = recoConsts::instance();
  // we need to set the cdb tag and timestamp to select the calibrations to write
  rc->set_StringFlag("CDB_GLOBALTAG","newcdbtag"); 
  rc->set_uint64Flag("TIMESTAMP",timestamp);
  CDBInterface *cdb = CDBInterface::instance();
  cdb->DumpCalibrations("calibs.list");

  return;
}

void ReadFile()
{
  // there is no timestamp or global tag when reading - this is set when you create the dump
  CDBInterface *cdb = CDBInterface::instance();
  cdb->ReadCalibrationsFromFile("calibs.list");
  // print a random calibration to see if it works
  std::cout << cdb->getUrl("ohcal_abscalib_mip_bldg912") << std::endl;
  return;
}

#endif
