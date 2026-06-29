#include <calib_emc_pi0/CaloCalibEmc_Pi0.h>

#include <Rtypes.h>

R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)

void save_correc_value(const std::string &ifile = "", const std::string &ofile = "", const std::string &infilent = "")
{
  CaloCalibEmc_Pi0 calo_obj("CaloCalibEmc_Pi0", ofile);
  calo_obj.Get_Histos(ifile, ofile);
  calo_obj.Add_96();
  calo_obj.Fit_Histos_Eta_Phi_Add96(infilent);
  calo_obj.End(nullptr);
}
