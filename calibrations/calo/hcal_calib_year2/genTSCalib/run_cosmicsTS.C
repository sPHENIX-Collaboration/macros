#include "../genFinalCalib/tsc_cos_merge.C"
#include "../HcalCalibDatasetinputs.h"

#include <litecaloeval/LiteCaloEval.h>

#include <Rtypes.h> // for R__LOAD_LIBRARY macro
#include <TSystem.h>

#include <algorithm>
#include <array>
#include <format>
#include <fstream>
#include <set>
#include <string>

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

// LiteCaloEval takes one fit range per fit, so an eta-dependent fit minimum is done with
// one fit per distinct fit minimum (each written to <out>_fmin<value>.root). The eta rings
// are fitted independently of each other, so each ring of corrPat is then taken from the
// fit that used the fit minimum of that ring.
std::string fitMinFileName(const std::string &out, float fitMin);
void mergeCorrPat(const std::string &out, const std::array<float, 24> &fitMin);

//make sure TS_list has relative or full paths to actual root files
void run_cosmicsTS(const std::string &runs = "runList.txt", const std::string &TS_list= "lce_output_files.txt", bool run_ohcal=false, bool run_ihcal=false)
{

  gSystem->mkdir("fitResults", true);
  gSystem->mkdir("cdbFiles", true); 

  std::ifstream runlist(runs);
  std::string line;

  std::ifstream tsList(TS_list);
  std::string line2;

  //read from run txt file and txt file w/ lce histos at the same time
  while(getline(runlist,line) && getline(tsList,line2))
  {

    std::cout << "=============================================" << std::endl;
    std::cout << "Processing run number: " << line << std::endl;

    if (line.empty() || line2.empty()) continue;  // Skip blank lines

    std::cout << "analysing file " << line2 << std::endl;

    // fit ranges of the dataset this cosmics run belongs to (see ../HcalCalibDatasetinputs.h)
    const HCalCalibDataset &ds = getHCalCalibDataset(stoi(line));
    std::cout << "using the " << ds.name << " settings" << std::endl;

    // datasets with towerslope periods: the run list holds the first run of each period
    if (!ds.tscPeriods.empty() && std::find(ds.tscPeriods.begin(), ds.tscPeriods.end(), stoi(line)) == ds.tscPeriods.end())
    {
      std::cout << "run " << line << " is not the first run of a " << ds.name << " towerslope period in HcalCalibDatasetinputs.h" << std::endl;
      gSystem->Exit(1);
    }


    if (run_ohcal){

    std::cout << "Running analysis for **HCALOUT**..." << std::endl;

    std::string oh_out = "fitResults/oh_cosmicsMerged_fitted_" + line + ".root";
  
    for (float fitMin : std::set<float>(ds.oh_fitMin.begin(), ds.oh_fitMin.end()))
    {
    std::string oh_fit = fitMinFileName(oh_out, fitMin);

    LiteCaloEval hout;
    hout.CaloType(LiteCaloEval::HCALOUT);
    hout.set_spectra_binWidth(ds.oh_binWidth);
    hout.Get_Histos(line2,oh_fit);
    hout.setFitMin(fitMin);//GeV
    hout.setFitMax(ds.oh_fitMax);
    hout.FitRelativeShifts(&hout,110);
    hout.draw_spectra(oh_fit.c_str());
    //hout.fit_info(oh_fit.c_str(), stoi(line)); // crashes in LiteCaloEval when a tower has no entries (e.g. O+O OHCal towers (2,47-49))
    }
    // corrPat with each eta ring from the fit with its own fit minimum
    mergeCorrPat(oh_out, ds.oh_fitMin);
    
    std::string cosmic_CDB_file_oh = hcalCosmicFile(ds.oh_cosmicFile, line);
    
    //modify the cosmics calibration file based on tsc fit output
    tsc_cos_merge(oh_out,cosmic_CDB_file_oh,std::format("cdbFiles/ohcal_cdb_tsc_cos_calib_{}.root",line),0);
    
    }

    if (run_ihcal){

    std::cout << "Running analysis for **HCALIN**..." << std::endl;

    std::string ih_out = "fitResults/ih_cosmicsMerged_fitted_" + line + ".root";
    
    for (float fitMin : std::set<float>(ds.ih_fitMin.begin(), ds.ih_fitMin.end()))
    {
    std::string ih_fit = fitMinFileName(ih_out, fitMin);

    LiteCaloEval hin;
    hin.CaloType(LiteCaloEval::HCALIN);
    hin.set_spectra_binWidth(ds.ih_binWidth);
    hin.Get_Histos(line2, ih_fit);
    hin.setFitMin(fitMin);
    hin.setFitMax(ds.ih_fitMax);
    hin.set_doQA(true);
    hin.FitRelativeShifts(&hin,110);
    hin.draw_spectra(ih_fit.c_str());
    //hin.fit_info(ih_fit.c_str(), stoi(line));
    }
    // corrPat with each eta ring from the fit with its own fit minimum
    mergeCorrPat(ih_out, ds.ih_fitMin);
    

    std::string cosmic_CDB_file_ih = hcalCosmicFile(ds.ih_cosmicFile, line);
   
    //modify the cosmics calibration file based on tsc fit output
    tsc_cos_merge(ih_out,cosmic_CDB_file_ih,std::format("cdbFiles/ihcal_cdb_tsc_cos_calib_{}.root",line),1);
    }

  }

  std::cout << "Macro is done executing." << std::endl;
  
}//end macro


std::string fitMinFileName(const std::string &out, float fitMin)
{
  return out.substr(0, out.rfind(".root")) + std::format("_fmin{:.2f}.root", fitMin);
}


void mergeCorrPat(const std::string &out, const std::array<float, 24> &fitMin)
{
  TH2 *corrPat = new TH2F("corrPat", "", 24, 0, 24, 64, 0, 64);
  corrPat->SetXTitle("#eta bin");
  corrPat->SetYTitle("#phi bin");
  corrPat->SetDirectory(nullptr);

  // fit minimum used for each eta ring, kept with the result
  TH1 *h_fitMin = new TH1F("h_fitMin", ";#eta bin;fit minimum [GeV]", 24, 0, 24);
  h_fitMin->SetDirectory(nullptr);

  for (int ie = 0; ie < 24; ie++)
  {
    std::string fit_file = fitMinFileName(out, fitMin[ie]);
    TFile *fin = TFile::Open(fit_file.c_str());
    TH2 *h{nullptr};
    if (fin) fin->GetObject("corrPat", h);
    if (!h)
    {
      std::cout << "mergeCorrPat: did not find corrPat in " << fit_file << std::endl;
      gSystem->Exit(1);
    }
    for (int ip = 0; ip < 64; ip++)
    {
      corrPat->SetBinContent(ie + 1, ip + 1, h->GetBinContent(ie + 1, ip + 1));
      corrPat->SetBinError(ie + 1, ip + 1, h->GetBinError(ie + 1, ip + 1));
    }
    h_fitMin->SetBinContent(ie + 1, fitMin[ie]);
    fin->Close();
  }

  TFile *fout = new TFile(out.c_str(), "RECREATE");
  corrPat->Write();
  h_fitMin->Write();
  fout->Close();
}
