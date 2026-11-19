// plot_mbdtrkzdiff.C
//
// Reads a list of mbdtrk_vertex.root files, fits the highest peak in
// h_mbdtrkz with a Gaussian, and plots the mean vs run number.
// Run number is extracted from the parent directory name
//
// Usage:
//   root -b -q 'plot_mbdtrkzdiff.C("MBDTRKZ/f.list")'

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <format>

#include <TCanvas.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraphErrors.h>
#include <TH1F.h>
#include <TLatex.h>
#include <TStyle.h>

// Extract the run number from a path of the form .../MBDTRKZ/<run>/<seg>/filename
// Returns -1 on failure.
int run_from_path(const std::string &path)
{
  // find last two '/' to get the directory name
  size_t slash2 = path.rfind('/');
  if (slash2 == std::string::npos) return -1;
  size_t slash1 = path.rfind('/', slash2 - 1);
  std::string rundir = (slash1 != std::string::npos)
                       ? path.substr(slash1 + 1, slash2 - slash1 - 1)
                       : path.substr(0, slash2);
  size_t us = rundir.find('_');
  if (us == std::string::npos) return -1;
  try { return std::stoi(rundir.substr(0, us)); }
  catch (...) { return -1; }
}

void plot_mbdtrkzdiff(const std::string &filelist = "MBDTRKZ/f.list")
{
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(111111);

  // --- read file list ---
  std::vector<std::string> files;
  {
    std::ifstream ifs(filelist);
    if (!ifs)
    {
      std::cerr << "Cannot open " << filelist << std::endl;
      return;
    }
    std::string line;
    while (std::getline(ifs, line))
    {
      if (!line.empty()) files.push_back(line);
    }
  }

  if (files.empty())
  {
    std::cerr << "No files in " << filelist << std::endl;
    return;
  }

  auto *g_mbdtrkzdiff = new TGraphErrors();
  g_mbdtrkzdiff->SetName("g_mbdtrkzdiff");
  g_mbdtrkzdiff->SetTitle("MBD - Tracker z-vertex;run;#Deltaz [cm]");

  int ipt = 0;
  for (const auto &fname : files)
  {
    int runnumber = run_from_path(fname);
    if (runnumber < 0)
    {
      std::cerr << "WARNING: cannot parse run number from: " << fname << std::endl;
      continue;
    }

    TFile *f = TFile::Open(fname.c_str(), "READ");
    if (!f || f->IsZombie())
    {
      std::cerr << "WARNING: cannot open " << fname << std::endl;
      continue;
    }

    auto *h = dynamic_cast<TH1F *>(f->Get("h_mbdtrkz"));
    if (!h)
    {
      std::cerr << "WARNING: h_mbdtrkz not found in " << fname << std::endl;
      f->Close();
      continue;
    }
    h->SetDirectory(nullptr);
    f->Close();

    if (h->GetEntries() == 0)
    {
      std::cerr << "WARNING: h_mbdtrkz empty in " << fname << std::endl;
      delete h;
      continue;
    }

    // locate highest peak
    int    peak_bin = h->GetMaximumBin();
    double peak_x   = h->GetBinCenter(peak_bin);

    // first pass: rough Gaussian fit ±3 cm around peak
    TF1 gfit("gfit", "gaus", peak_x - 3.0, peak_x + 3.0);
    int status = h->Fit(&gfit, "RQ0");
    if (status != 0)
    {
      std::cerr << "WARNING: initial fit failed for run " << runnumber << std::endl;
      delete h;
      continue;
    }

    double sigma = gfit.GetParameter(2);
    if (sigma <= 0)
    {
      std::cerr << "WARNING: bad sigma from initial fit for run " << runnumber << std::endl;
      delete h;
      continue;
    }

    // second pass: refit within ±2σ of first-pass mean
    double mean1 = gfit.GetParameter(1);
    gfit.SetRange(mean1 - 2.0 * sigma, mean1 + 2.0 * sigma);
    status = h->Fit(&gfit, "RQ0");
    if (status != 0)
    {
      std::cerr << "WARNING: second fit failed for run " << runnumber << std::endl;
      delete h;
      continue;
    }

    double mean     = gfit.GetParameter(1);
    double mean_err = gfit.GetParError(1);

    g_mbdtrkzdiff->SetPoint(ipt, runnumber, mean);
    g_mbdtrkzdiff->SetPointError(ipt, 0., mean_err);
    ++ipt;

    delete h;
  }

  if (g_mbdtrkzdiff->GetN() == 0)
  {
    std::cerr << "No points in graph — nothing to plot." << std::endl;
    return;
  }

  g_mbdtrkzdiff->Sort();

  // pol0 fit
  TF1 *fpol0 = new TF1("fpol0", "pol0", g_mbdtrkzdiff->GetX()[0]-10, g_mbdtrkzdiff->GetX()[g_mbdtrkzdiff->GetN() - 1]+10);
  fpol0->SetLineColor(kRed);
  fpol0->SetLineWidth(2);
  g_mbdtrkzdiff->Fit(fpol0, "QR");

  // draw
  auto *c = new TCanvas("c_mbdtrkzdiff", "MBD-Tracker dz", 900, 600);
  c->SetLeftMargin(0.12);
  c->SetBottomMargin(0.12);

  g_mbdtrkzdiff->SetMarkerStyle(20);
  g_mbdtrkzdiff->SetMarkerSize(0.8);
  g_mbdtrkzdiff->Draw("AP");

  fpol0->Draw("same");

  TLatex tex;
  tex.SetNDC();
  tex.SetTextSize(0.035);
  tex.DrawLatex(0.15, 0.85, std::format("pol0 fit: {:.3f} #pm {:.3f} cm", fpol0->GetParameter(0), fpol0->GetParError(0)).c_str() );

  c->SaveAs("mbdtrkzdiff.pdf");
  std::cout << "Saved mbdtrkzdiff.pdf" << std::endl;
}
