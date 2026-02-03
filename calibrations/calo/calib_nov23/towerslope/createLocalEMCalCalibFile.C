#include <GlobalVariables.C>
#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <ffamodules/CDBInterface.h>

R__LOAD_LIBRARY(libcdbobjects)


void createLocalEMCalCalibFile(const string fname,int runNumber){

    string default_time_independent_calib = "cemc_pi0_twrSlope_v1_default";
    string  m_calibName = "cemc_pi0_twrSlope_v1";

    string calibdir = CDBInterface::instance()->getUrl(m_calibName);
    string filePath;

    if (!calibdir.empty())
    {
      filePath = calibdir;
      //cdbttree = new CDBTTree(calibdir);
    }
    else
    {
      calibdir = CDBInterface::instance()->getUrl(default_time_independent_calib);

      if (calibdir.empty())
      {
        std::cout << "No EMCal Calibration NOT even a default" << std::endl;
        exit(1);
      }
      filePath = calibdir;
      //cdbttree = new CDBTTree(calibdir);
      std::cout << "No specific file for " << m_calibName << " found, using default calib " << default_time_independent_calib << std::endl;
    }

    TFile* f_cdb = new TFile(filePath.c_str());
    f_cdb->Cp(fname.c_str()); 

    delete  f_cdb;
}
