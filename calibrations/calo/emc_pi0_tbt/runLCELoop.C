#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

#include <Rtypes.h>

R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)

void runLCELoop(int nevents = -1, const std::string &ifile = "treetest_g4cemc_eval.root", const std::string &ofile = "treetest1_g4cemc_eval.root")
{
  CaloCalibEmc_Pi0 obj_LCE("CaloCalibEmc_Pi0", ofile);
  obj_LCE.InitRun(nullptr);
  obj_LCE.Loop(nevents, ifile);
  obj_LCE.End(nullptr);
  //  obj_LCE.FittingHistos();
}
