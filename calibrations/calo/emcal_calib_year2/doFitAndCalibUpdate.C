#include <calib_emc_pi0/pi0EtaByEta.h>

#include <Rtypes.h>
#include <TFile.h>
#include <TSystem.h>

#include <format>

R__LOAD_LIBRARY(libcalibCaloEmc_pi0.so)

void doFitAndCalibUpdate(const std::string &hist_fname = "OUTHIST_iter4_DST_CALO_run2pp_new_2024p003-00046754-00000.root", const std::string &calib_fname = "base/local_calib_copy.root" ,int iter = 2)
{


  pi0EtaByEta *caFit = new pi0EtaByEta("calomodulename", "bla.root");
  //caFit->fitEtaSlices(hist_fname.c_str(), Form("fitout_iter%d.root", iter), calib_fname.c_str());
  caFit->Init(nullptr);
  caFit->SetTargetMass(0.1375);  
  caFit->Split3DHist(hist_fname,"split.root");
  caFit->fitEtaPhiTowers("split.root", std::format("fitout_iter{}.root", iter), calib_fname);

  size_t pos = calib_fname.find_last_of('.'); 
  std::string f_calib_save_name = calib_fname;
  f_calib_save_name.insert(pos,std::format("_iter{}",iter));

  TFile* f_calib_mod = new TFile(calib_fname.c_str());
  f_calib_mod->Cp(f_calib_save_name.c_str());

  gSystem->Exit(0);
}
