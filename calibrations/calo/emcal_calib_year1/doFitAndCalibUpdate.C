// -- c++ includes --
#include <iostream>
#include <memory>
#include <string>
#include <format>

// -- root includes --
#include <TFile.h>
#include <TSystem.h>

#include <calib_emc_pi0/pi0EtaByEta.h>
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)

void doFitAndCalibUpdate(const std::string &hist_fname = "base/combine_out/out2.root",
                         const std::string &calib_fname = "base/local_calib_copy.root",
                         int iter = 4,
                         const std::string &fieldname = "CEMC_calib_ADC_to_ETower")
{
  std::string m_fieldname = fieldname;

  std::unique_ptr<pi0EtaByEta> caFit = std::make_unique<pi0EtaByEta>("calomodulename", "bla.root");
  caFit->set_massTargetHistFile("/sphenix/user/egm2153/calib_study/emcal_calib_year1/hijing_run14_fb_wupdatedsmear/hijing_run14_uncorr_cls_mass_towerThreshold070MeV.root");  // HIJING RUN14 w/ updated smearing pi0 mass w/ 70MeV cluster tower threshold
  // caFit->set_massTargetHistFile("/sphenix/user/egm2153/calib_study/emcal_calib_year1/hijing_run14_fixed_build/hijing_run14_uncorr_cls_mass_towerThreshold070MeV.root"); // HIJING RUN14 pi0 mass w/ 70MeV cluster tower threshold
  // caFit->set_massTargetHistFile("/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/run10_uncorr_cls_mass.root");// HIJING pi0 mass  w/ 30MeV
  // caFit->set_massTargetHistFile("/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/run10_uncorr_cls_mass_towerThreshold0p06.root");// HIJING pi0 mass
  // caFit->set_scaleAdjFac(152.0/149.0); // effect of worse resolution in data
  caFit->set_calib_fieldname(m_fieldname);
  caFit->fitEtaSlices(hist_fname, std::format("fitout_iter{}.root", iter), calib_fname);

  size_t pos = calib_fname.find_last_of('.');
  std::string f_calib_save_name = calib_fname;
  f_calib_save_name.insert(pos, std::format("_iter{}", iter));

  std::unique_ptr<TFile> f_calib_mod = std::make_unique<TFile>(calib_fname.c_str());
  f_calib_mod->Cp(f_calib_save_name.c_str());

  gSystem->Exit(0);
}

#ifndef __CINT__
int main(int argc, const char *const argv[])
{
  const std::vector<std::string> args(argv, argv + argc);

  if (args.size() < 3 || args.size() > 5)
  {
    std::cout << "usage: " << args[0] << " hist_fname calib_fname [iter] [m_fieldname]" << std::endl;
    return 1;
  }

  const std::string& hist_fname = args[1];
  const std::string& calib_fname = args[2];
  int iter = 4;
  std::string m_fieldname = "CEMC_calib_ADC_to_ETower";

  if (args.size() >= 4)
  {
    iter = std::stoi(args[3]);
  }
  if (args.size() >= 5)
  {
    m_fieldname = args[4];
  }

  doFitAndCalibUpdate(hist_fname, calib_fname, iter, m_fieldname);

  std::cout << "======================================" << std::endl;
  std::cout << "done" << std::endl;
  return 0;
}
#endif
