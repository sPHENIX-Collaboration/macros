// -- c++ includes --
#include <format>
#include <memory>
#include <string>

// -- root includes --
#include <TFile.h>
#include <TH2F.h>
#include <TSystem.h>

#include <cdbobjects/CDBTTree.h>  // for CDBTTree
#include <litecaloeval/LiteCaloEval.h>
// TowerInfo
#include <calobase/TowerInfoDefs.h>

#include <sPhenixStyle.C>

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)
R__LOAD_LIBRARY(libcdbobjects)

void TSCtoCDBTTree(const char *infile, const char *outputfile, const std::string &m_fieldname);
void mergeCDBTTrees(const char *infile1, const char *infile2, const char *outputfile, const std::string &m_fieldname);

void doTscFit(const std::string &hist_fname = "base/combine_out/out1.root",
              const std::string &calib_fname = "base/local_calib_copy.root",
              int iter = 1,
              const std::string &m_fieldname = "CEMC_calib_ADC_to_ETower")
{
  std::string fitoutfile = std::format("tsc_fitout_it{}.root", iter);

  if (iter <= 2)
  {
    LiteCaloEval modlce;
    modlce.CaloType(LiteCaloEval::CEMC);
    modlce.set_spectra_binWidth(0.02);
    modlce.Get_Histos(hist_fname.c_str(), fitoutfile.c_str());
    modlce.m_myminbin = 0;
    modlce.m_mymaxbin = 96;
    modlce.setFitMin(0.40F);
    modlce.setFitMax(1.2F);
    modlce.set_doQA();
    // if (iter==2) modlce.set_doQA(true);
    modlce.FitRelativeShifts(&modlce, 110);
  }

  if (iter == 3)
  {
    SetsPhenixStyle();
    LiteCaloEval modlce;
    modlce.CaloType(LiteCaloEval::CEMC);
    modlce.set_spectra_binWidth(0.005);
    modlce.Get_Histos(hist_fname.c_str(), fitoutfile.c_str());
    modlce.plot_cemc("figures");
  }

  // create the cdbttree from tsc output andd multiply the corrections
  // into the base calibration to pickup for pi0 first iteration

  TSCtoCDBTTree(fitoutfile.c_str(), std::format("tsc_output_cdb_it{}.root", iter).c_str(), m_fieldname);
  mergeCDBTTrees(std::format("tsc_output_cdb_it{}.root", iter).c_str(), calib_fname.c_str(), calib_fname.c_str(), m_fieldname);

  size_t pos = calib_fname.find_last_of('.');
  std::string f_calib_save_name = calib_fname;
  f_calib_save_name.insert(pos, std::format("_postTSC_it{}", iter));

  std::unique_ptr<TFile> f_calib_mod = std::make_unique<TFile>(calib_fname.c_str());
  f_calib_mod->Cp(f_calib_save_name.c_str());

  gSystem->Exit(0);
}

void mergeCDBTTrees(const char *infile1, const char *infile2, const char *outputfile, const std::string &m_fieldname)
{
  std::unique_ptr<CDBTTree> cdbttree1 = std::make_unique<CDBTTree>(infile1);
  std::unique_ptr<CDBTTree> cdbttree2 = std::make_unique<CDBTTree>(infile2);
  std::unique_ptr<CDBTTree> cdbttreeOut = std::make_unique<CDBTTree>(outputfile);

  for (unsigned int i = 0; i < 96; i++)
  {
    for (unsigned int j = 0; j < 256; j++)
    {
      unsigned int key = TowerInfoDefs::encode_emcal(i, j);
      float val1 = cdbttree1->GetFloatValue(static_cast<int>(key), m_fieldname);
      float val2 = cdbttree2->GetFloatValue(static_cast<int>(key), m_fieldname);
      cdbttreeOut->SetFloatValue(static_cast<int>(key), m_fieldname, val1 * val2);
    }
  }

  cdbttreeOut->Commit();
  cdbttreeOut->WriteCDBTTree();

}  // end macro

void TSCtoCDBTTree(const char *infile, const char *outputfile, const std::string &m_fieldname)
{
  bool chk4file = gSystem->AccessPathName(infile);
  std::unique_ptr<TFile> f = nullptr;

  if (!chk4file)
  {
    f = std::make_unique<TFile>(infile, "READ");
  }
  else
  {
    std::cout << "File " << infile << " cant be found in current directory." << std::endl;
    exit(0);
  }

  // write to cdb tree
  std::unique_ptr<CDBTTree> cdbttree = std::make_unique<CDBTTree>(outputfile);

  // gain values lie in the 2d histogram called corrPat
  TH2F *cp = static_cast<TH2F *>(f->Get("corrPat"));

  for (int i = 0; i < 96; i++)
  {
    for (int j = 0; j < 256; j++)
    {
      unsigned int key = TowerInfoDefs::encode_emcal(static_cast<unsigned int>(i), static_cast<unsigned int>(j));
      float gain = static_cast<float>(1.0 / cp->GetBinContent(i + 1, j + 1));
      if (cp->GetBinContent(i + 1, j + 1) <= 0)
      {
        gain = 0;
      }
      if (std::isnan(cp->GetBinContent(i + 1, j + 1)))
      {
        gain = 0;
        std::cout << "nan calib from tsc " << i << "," << j << std::endl;
      }
      cdbttree->SetFloatValue(static_cast<int>(key), m_fieldname.c_str(), gain);
    }
  }

  cdbttree->Commit();
  cdbttree->WriteCDBTTree();
  // cdbttree->Print();
  f->Close();
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
  int iter = 1;
  std::string m_fieldname = "CEMC_calib_ADC_to_ETower";

  if (args.size() >= 4)
  {
    iter = std::stoi(args[3]);
  }
  if (args.size() >= 5)
  {
    m_fieldname = args[4];
  }

  doTscFit(hist_fname, calib_fname, iter, m_fieldname);

  std::cout << "======================================" << std::endl;
  std::cout << "done" << std::endl;
  return 0;
}
#endif
