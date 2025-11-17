// add up two t0 calibrations
#include <mbd/MbdCalib.h>
#include <mbd/MbdGeomV1.h>
#include <mbd/MbdDefs.h>

#include <Rtypes.h> // defines R__LOAD_LIBRARY macro for clang-tidy
#include <TString.h>

#include <filesystem>

R__LOAD_LIBRARY(libmbd.so)

// const int NPOINTS = 16000; // number of points in correction LUT
// const int MINADC = 0;       // subtracted adc
// const int MAXADC = 15999;

void addt0(const char *calfile1 = "results/0/mbd_tt_t0.calib",
    const char *calfile2 = "results/0/pass1_mbd_tt_t0.calib")
{
//  MbdGeom *mbdgeom = new MbdGeomV1();

  MbdCalib *mcal1 = new MbdCalib();
  mcal1->Download_TTT0( calfile1 );

  MbdCalib *mcal2 = new MbdCalib();
  mcal2->Download_TTT0( calfile2 );

  // Now download the tq_t0's
  TString tqcalfile1 = calfile1;
  tqcalfile1.ReplaceAll("tt_t0","tq_t0");
  MbdCalib *mcaltq1 = new MbdCalib();
  mcaltq1->Download_TQT0( tqcalfile1.Data() );

  TString tqcalfile2 = calfile2;
  tqcalfile2.ReplaceAll("tt_t0","tq_t0");
  MbdCalib *mcaltq2 = new MbdCalib();
  mcaltq2->Download_TQT0( tqcalfile2.Data() );

  // Add the t0's and update the calib
  for (int ipmtch=0; ipmtch<MbdDefs::MBD_N_PMT; ipmtch++)
  {
    float ttt01 = mcal1->get_tt0( ipmtch );
    float ttt02 = mcal2->get_tt0( ipmtch );
    float new_ttt0 = ttt01 + ttt02;

    mcal1->set_tt0( ipmtch, new_ttt0 );

    float tqt01 = mcaltq1->get_tq0( ipmtch );
    float tqt02 = mcaltq2->get_tq0( ipmtch );
    float new_tqt0 = tqt01 + tqt02;

    mcaltq1->set_tq0( ipmtch, new_tqt0 );
  }

  // Write out new t0's to temp calib files
  const std::string dir = std::filesystem::path{ calfile1 }.parent_path().string();
  const std::string& ttt0_fname = dir + "/add_mbd_tt_t0.calib";
  std::cout << ttt0_fname << std::endl;
  mcal1->Write_TTT0( ttt0_fname );

  const std::string& tqt0_fname = dir + "/add_mbd_tq_t0.calib";
  std::cout << tqt0_fname << std::endl;
  mcaltq1->Write_TQT0( tqt0_fname );

}
