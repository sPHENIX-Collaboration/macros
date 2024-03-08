
#include <calib_emc_pi0/pi0EtaByEta.h>
R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)

void doFitAndCalibUpdate(const std::string &hist_fname = "base/combine_out/out2.root", const std::string &calib_fname = "base/local_calib_copy.root" ,int iter = 2)
{


  pi0EtaByEta *caFit = new pi0EtaByEta("calomodulename", "bla.root");
  //caFit->set_massTargetHistFile("/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/run10_uncorr_cls_mass.root");
  caFit->fitEtaSlices(hist_fname.c_str(), Form("fitout_iter%d.root", iter), calib_fname.c_str());

  size_t pos = calib_fname.find_last_of('.'); 
  string f_calib_save_name = calib_fname;
  f_calib_save_name.insert(pos,Form("_iter%d",iter));

  TFile* f_calib_mod = new TFile(calib_fname.c_str());
  f_calib_mod->Cp(f_calib_save_name.c_str());

  gSystem->Exit(0);
}


