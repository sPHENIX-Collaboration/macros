// This code creates a 1D distribution of temperature vs runnumber/runid by averaging the temperatures of the towers in the root file made by Calotemp macro. 
// how to run: root -l makeAvgTempVsRun.C
// You have to edit the input file directory, file names of the input an output accordingly. This macro by default analyzes the case of inner hcal. 

#include <TFile.h>
#include <TProfile2D.h>
#include <TH1D.h>
#include <TGraph.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>

void makeAvgTempVsRun(
  const char* dir = "/sphenix/user/anjsmenon/work_2025/macros/calibrations/calo/final_24_hcalcalib/makeTemp/temp_ihcal_pp_2024",
  //const char* dir = "/sphenix/user/anjsmenon/work_2025/macros/calibrations/calo/final_24_hcalcalib/condor_histmaker/makenewTemp/temp_ohcal_pp_2024",
  const char* runlist_path = "Full_ppGoldenRunList_Version3.list", // one run per line
  const char* outroot = "avgTemp_HCALIN.root",
  const char* outcsv  = "avgTemp_HCALIN.csv",
  double Tmin = 10.0, double Tmax = 30.0)
{
  std::ifstream rin(runlist_path);
  if (!rin.is_open()) { std::cerr << "Cannot open runlist: " << runlist_path << std::endl; return; }

  std::vector<int> runs;
  for (std::string line; std::getline(rin, line); ) {
    if (line.empty()) continue;
    runs.push_back(std::stoi(line));
  }
  if (runs.empty()) { std::cerr << "Run list is empty.\n"; return; }

  TH1D* h_avg_vs_idx = new TH1D("h_avg_vs_idx","iHCal Avg Temperature per Run;run index;avg T [^{o}C]",
                                runs.size(), 0.5, runs.size()+0.5);
  for (size_t i=0;i<runs.size();++i) h_avg_vs_idx->GetXaxis()->SetBinLabel(i+1, Form("%d", runs[i]));
  h_avg_vs_idx->SetMarkerStyle(20);

  std::vector<double> x_run(runs.size()), y_avg(runs.size());

  std::ofstream csv(outcsv);
  csv << "run,avg_temp_C,ntowers_used\n";

  size_t idx = 0;
  for (int run : runs)
  {
    std::string fname = Form("%s/HCALIN_temp_%d.root", dir, run);
    TFile f(fname.c_str(), "READ");
    if (f.IsZombie()) { 
      std::cerr << "[skip] cannot open " << fname << "\n";
      ++idx; continue;
    }

    auto prof = dynamic_cast<TProfile2D*>(f.Get("h_HCALIN_temp"));
    if (!prof) {
      std::cerr << "[skip] missing h_HCALIN_temp in " << fname << "\n";
      ++idx; continue;
    }

    const int nx = prof->GetNbinsX();
    const int ny = prof->GetNbinsY();
    double sum = 0.0;
    int nused = 0;

    for (int ix=1; ix<=nx; ++ix) {
      for (int iy=1; iy<=ny; ++iy) {
        const int bin = prof->GetBin(ix,iy);
        // Only consider bins with entries
        if (prof->GetBinEntries(bin) <= 0) continue;
        const double val = prof->GetBinContent(bin);
        if (val < Tmin || val > Tmax) continue;
        sum += val;
        ++nused;
      }
    }

    const double avg = (nused>0) ? sum / nused : 0.0;

    // Fill outputs
    h_avg_vs_idx->SetBinContent(idx+1, avg);
    x_run[idx] = run;
    y_avg[idx] = avg;
    csv << run << "," << avg << "," << nused << "\n";

    ++idx;
  }
  csv.close();

  auto gr = new TGraph((int)runs.size(), x_run.data(), y_avg.data());
  gr->SetName("g_avg_vs_run");
  gr->SetTitle("IHCal Avg Temperature per Run;run number;avg T [^{o}C]");
  gr->SetMarkerStyle(20);

  gStyle->SetOptStat(0);
  TCanvas* c1 = new TCanvas("c_avg_idx","Avg T vs run index",1200,500);
  h_avg_vs_idx->Draw("P");
  c1->SetBottomMargin(0.20); 
  //c1->SaveAs("avgTemp_vs_runIndex_ihcal.png");

  TCanvas* c2 = new TCanvas("c_avg_run","Avg T vs run number",900,600);
  gr->Draw("AP");
  //c2->SaveAs("avgTemp_vs_runNumber_ihcal.png");

  // Save to ROOT file
  TFile outf(outroot, "RECREATE");
  h_avg_vs_idx->Write();
  gr->Write();
  outf.Close();

  std::cout << "Done. Plots: avgTemp_vs_runIndex.png, avgTemp_vs_runNumber.png\n"
            << "ROOT: " << outroot << "\nCSV: " << outcsv << std::endl;
}

