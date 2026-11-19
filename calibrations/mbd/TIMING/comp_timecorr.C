#include <TH1D.h>
#include <TCanvas.h>
#include <TLine.h>
#include <TStyle.h>
#include <TFile.h>
#include <TTree.h>
#include <TGraph.h>
#include <TMultiGraph.h>

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <cmath>
#include <filesystem>

#include <fun4all/Fun4AllUtils.h>

#include <mbd/MbdCalib.h>
#include <mbd/MbdGeomV2.h>
#include "get_runstr.h"

R__LOAD_LIBRARY(libmbd.so)
R__LOAD_LIBRARY(libmbd_io.so)


// For each run in listfile, load the timecorr LUT and compute the per-TDC-bin ratio
// relative to the first run for each channel.  The mean ratio over all TDC bins is
// stored as one TTree entry per (run, channel) in timecorr_avg.root.
void average_timecorr(const std::string& listfile = "tcorr.list")
{
  constexpr int NPMT = 128;

  MbdGeom* mbdgeom = new MbdGeomV2();

  // Read file list
  std::vector<std::string> files;
  std::vector<int>         runs;
  {
    std::ifstream lf(listfile);
    if (!lf.is_open())
    {
      std::cerr << "Error: Could not open " << listfile << std::endl;
      return;
    }
    // Parse run number from the directory component: 000<run>-0000
    auto parse_run = [](const std::string& path) -> int
    {
      size_t slash2 = path.rfind('/');
      size_t slash1 = (slash2 == std::string::npos) ? std::string::npos : path.rfind('/', slash2 - 1);
      size_t start  = (slash1 == std::string::npos) ? 0 : slash1 + 1;
      std::string dir = path.substr(start, (slash2 == std::string::npos ? path.size() : slash2) - start);
      size_t dash = dir.find('-');
      if (dash == std::string::npos) return -1;
      try { return std::stoi(dir.substr(0, dash)); }
      catch (...) { return -1; }
    };

    std::string path;
    while (lf >> path)
    {
      int runno = parse_run(path);
      if (runno < 0)
      {
        std::cerr << "Warning: Could not parse run number from: " << path << std::endl;
        continue;
      }
      files.push_back(path);
      runs.push_back(runno);
    }
  }

  if (files.empty())
  {
    std::cerr << "Error: no valid files in " << listfile << std::endl;
    return;
  }

  int nruns = (int)files.size();

  // Load all calibrations
  std::vector<MbdCalib*> calibs(files.size());
  for (size_t i = 0; i < files.size(); i++)
  {
    std::cout << "Loading " << files[i] << std::endl;
    calibs[i] = new MbdCalib();
    calibs[i]->Download_TimeCorr(files[i]);
  }

  // Determine LUT size from first file
  int min_tdc, max_tdc, step; // NOLINT(readability-isolate-declaration)
  calibs[0]->get_tcorr_range(0, min_tdc, max_tdc, step);
  int ntdc = (max_tdc - min_tdc) / step + 1;
  std::cout << "LUT size: " << ntdc << " bins  [" << min_tdc << ", " << max_tdc << "]  step=" << step << std::endl;

  // TTree output
  TFile* fout = new TFile("timecorr_avg.root", "RECREATE");
  TTree* t = new TTree("t", "TimCorr LUT ratios vs first run");
  int    t_run;
  int    t_ch;
  double t_ratio;
  double t_delta;
  t->Branch("run",   &t_run,   "run/I");
  t->Branch("ch",    &t_ch,    "ch/I");
  t->Branch("ratio", &t_ratio, "ratio/D");
  t->Branch("delta", &t_delta, "delta/D");

  // ratio_by_ch / delta_by_ch [ch][run_index] — filled alongside TTree for the PDF
  std::vector<std::vector<double>> ratio_by_ch(NPMT, std::vector<double>(files.size(), 0.));
  std::vector<std::vector<double>> delta_by_ch(NPMT, std::vector<double>(files.size(), 0.));

  // lut_sum[ch][ilut]: sum of val across all runs for every bin (all tdc range)
  std::vector<std::vector<double>> lut_sum(NPMT, std::vector<double>(ntdc, 0.));

  // For each run and channel, compute mean(LUT[run][ch][tdc] / LUT[ref][ch][tdc]) over all tdc bins
  for (size_t irun = 0; irun < files.size(); irun++)
  {
    t_run = runs[irun];
    for (int ipmt = 0; ipmt < NPMT; ipmt++)
    {
      int ifeech = mbdgeom->get_feech(ipmt, 0);

      double sum       = 0.;
      double sum_delta = 0.;
      int    ngood     = 0;
      for (int ilut = 0; ilut < ntdc; ilut++)
      {
        int itdc = ilut*step;
        float val = calibs[irun]->get_tcorr(ifeech, itdc);
        lut_sum[ipmt][ilut] += val; // accumulate over all bins for avg LUT

        // select good region of itdc (13450 is min good tdc, in ch116)
        if ( itdc<1400. || itdc>13450. )
        {
          continue;
        }

        float ref = calibs[0]->get_tcorr(ifeech, itdc);
        //if (std::abs(ref) < 1e-9) continue; // skip zero reference bins
        sum       += val / ref;
        sum_delta += val - ref;
        ngood++;
      }

      t_ch    = ipmt;
      t_ratio = (ngood > 0) ? sum       / ngood : 0.;
      t_delta = (ngood > 0) ? sum_delta / ngood : 0.;
      ratio_by_ch[ipmt][irun] = t_ratio;
      delta_by_ch[ipmt][irun] = t_delta;
      t->Fill();
    }
  }

  fout->cd();
  t->Write();
  std::cout << "Saved " << t->GetEntries() << " entries to timecorr_avg.root" << std::endl;

  // Per-channel mean delta: <delta>[ch] = mean over runs of delta_by_ch[ch][irun]
  std::vector<double> mean_delta(NPMT, 0.);
  for (int ipmt = 0; ipmt < NPMT; ipmt++)
  {
    for (int ir = 0; ir < nruns; ir++) mean_delta[ipmt] += delta_by_ch[ipmt][ir];
    mean_delta[ipmt] /= nruns;
  }

  // avg_lut[ch][ilut] = mean(val) - <delta>[ch]  (shift whole LUT by mean offset)
  std::vector<std::vector<double>> avg_lut(NPMT, std::vector<double>(ntdc, 0.));
  for (int ipmt = 0; ipmt < NPMT; ipmt++)
  {
    for (int ilut = 0; ilut < ntdc; ilut++)
    {
      avg_lut[ipmt][ilut] = lut_sum[ipmt][ilut] / nruns - mean_delta[ipmt];
    }
  }

  // Write average calibration file in same format as mbd_timecorr.calib
  {
    std::ofstream calout("avg_mbd_timecorr.calib");
    calout << std::fixed << std::setprecision(5);
    for (int ipmt = 0; ipmt < NPMT; ipmt++)
    {
      int ifeech = mbdgeom->get_feech(ipmt,0);
      calout << ifeech << "\t" << ntdc << "\t" << min_tdc << "\t" << max_tdc << "\n";
      for (int ilut = 0; ilut < ntdc; ilut++)
      {
        calout << avg_lut[ipmt][ilut];
        calout << ((ilut % 10 == 9) ? "\n" : " ");
      }
      if (ntdc % 10 != 0) calout << "\n";
    }
    std::cout << "Wrote avg_mbd_timecorr.calib" << std::endl;
  }

  // PDF: two pages per channel
  //   Page A: ratio vs run index
  //   Page B: corrected LUT overlays (all runs shifted by -t_delta, plus reference)
  TCanvas* cpdf = new TCanvas("cpdf", "TimCorr Average", 1000, 500);
  cpdf->Divide(2, 1);
  const TString pdfname = "timecorr_average.pdf";
  cpdf->Print(pdfname + "[");

  // colour palette cycling over runs (skip white=0)
  const int NCOLS = 9;
  const int cols[NCOLS] = {kBlue, kRed, kGreen+2, kMagenta, kCyan+1,
                            kOrange+1, kViolet+1, kTeal+1, kPink+1};

  for (int ipmt = 0; ipmt < NPMT; ipmt++)
  {
    int ifeech = mbdgeom->get_feech(ipmt, 0);

    // --- Left pad: ratio vs run ---
    TGraph* gratio = new TGraph(nruns);
    TString gname  = "g_ratio_ch"; gname += ipmt;
    TString gtitle = "Ch "; gtitle += ipmt; gtitle += " ratio vs run;Run index;Mean LUT ratio";
    gratio->SetName(gname);
    gratio->SetTitle(gtitle);
    gratio->SetMarkerStyle(20);
    gratio->SetMarkerSize(0.7);
    for (int ir = 0; ir < nruns; ir++)
      gratio->SetPoint(ir, ir, ratio_by_ch[ipmt][ir]);

    cpdf->cd(1);
    gratio->Draw("AP");

    // --- Right pad: corrected LUT overlay (val - <delta>) for each run + avg ---
    // Build a multigraph so axes are set automatically
    TMultiGraph* mg = new TMultiGraph();
    TString mgtitle = "Ch "; mgtitle += ipmt;
    mgtitle += " corrected LUT;TDC;Time - <#Delta> (ns)";
    mg->SetTitle(mgtitle);

    for (size_t irun = 0; irun < files.size(); irun++)
    {
      TGraph* glut = new TGraph();
      for (int ilut = 0; ilut < ntdc; ilut++)
      {
        int itdc = ilut*step;
        if ( itdc>14200. )
        {
          continue;
        }
        float val = calibs[irun]->get_tcorr(ifeech, itdc);
        glut->AddPoint(itdc, val - mean_delta[ipmt]);
      }
      int col = cols[irun % NCOLS];
      glut->SetLineColor(col);
      glut->SetMarkerColor(col);
      glut->SetMarkerStyle(1);
      mg->Add(glut, "L");
    }

    // Average LUT overlay (bold black)
    TGraph* gavg = new TGraph();
    for (int ilut = 0; ilut < ntdc; ilut++)
    {
      int itdc = ilut*step;
      if ( itdc>13450. ) continue;
      gavg->AddPoint(itdc, avg_lut[ipmt][ilut]);
    }
    gavg->SetLineColor(kBlack);
    gavg->SetLineWidth(2);
    gavg->SetMarkerStyle(1);
    mg->Add(gavg, "L");

    cpdf->cd(2);
    mg->Draw("A");

    cpdf->Update();
    cpdf->Print(pdfname);

    delete gratio;
    delete mg; // also deletes the owned TGraph children
  }

  cpdf->Print(pdfname + "]");
  std::cout << "Created: " << pdfname << std::endl;
  delete cpdf;
}


// Store the full LUT for every run, channel, and TDC entry in a flat TTree.
void timecorr_all(const std::string& listfile = "tcorr.list")
{
  constexpr int NPMT = 128;

  MbdGeom* mbdgeom = new MbdGeomV2();

  // --- parse file list (same logic as average_timecorr) ---
  std::vector<std::string> files;
  std::vector<int>         runs;
  {
    std::ifstream lf(listfile);
    if (!lf.is_open())
    {
      std::cerr << "Error: Could not open " << listfile << std::endl;
      return;
    }
    auto parse_run = [](const std::string& path) -> int
    {
      size_t slash2 = path.rfind('/');
      size_t slash1 = (slash2 == std::string::npos) ? std::string::npos : path.rfind('/', slash2 - 1);
      size_t start  = (slash1 == std::string::npos) ? 0 : slash1 + 1;
      std::string dir = path.substr(start, (slash2 == std::string::npos ? path.size() : slash2) - start);
      size_t dash = dir.find('-');
      if (dash == std::string::npos) return -1;
      try { return std::stoi(dir.substr(0, dash)); }
      catch (...) { return -1; }
    };
    std::string path;
    while (lf >> path)
    {
      int runno = parse_run(path);
      if (runno < 0)
      {
        std::cerr << "Warning: Could not parse run number from: " << path << std::endl;
        continue;
      }
      files.push_back(path);
      runs.push_back(runno);
    }
  }
  if (files.empty())
  {
    std::cerr << "Error: no valid files in " << listfile << std::endl;
    return;
  }

  // --- load calibrations ---
  std::vector<MbdCalib*> calibs(files.size());
  for (size_t i = 0; i < files.size(); i++)
  {
    std::cout << "Loading " << files[i] << std::endl;
    calibs[i] = new MbdCalib();
    calibs[i]->Download_TimeCorr(files[i]);
  }

  // --- LUT geometry from first file ---
  int min_tdc, max_tdc, step; // NOLINT(readability-isolate-declaration)
  calibs[0]->get_tcorr_range(0, min_tdc, max_tdc, step);
  int ntdc = (max_tdc - min_tdc) / step + 1;
  std::cout << "LUT: " << ntdc << " entries  tdc=[" << min_tdc << "," << max_tdc << "]  step=" << step << std::endl;

  // --- TTree ---
  TFile* fout = new TFile("timecorr.root", "RECREATE");
  TTree* t    = new TTree("t", "Full timecorr LUT dump");

  int      t_run;
  Short_t  t_ch;
  Short_t  t_index;
  UShort_t t_tdc;
  Float_t  t_time;

  t->Branch("run",   &t_run,   "run/I");
  t->Branch("ch",    &t_ch,    "ch/S");
  t->Branch("index", &t_index, "index/S");
  t->Branch("tdc",   &t_tdc,   "tdc/s");   // lowercase s = unsigned short
  t->Branch("time",  &t_time,  "time/F");

  for (size_t irun = 0; irun < files.size(); irun++)
  {
    t_run = runs[irun];
    for (int ipmt = 0; ipmt < NPMT; ipmt++)
    {
      int ifeech = mbdgeom->get_feech(ipmt, 0);
      t_ch = (Short_t)ipmt;
      for (int ilut = 0; ilut < ntdc; ilut++)
      {
        int itdc   = min_tdc + ilut * step;
        t_index    = (Short_t)ilut;
        t_tdc      = (UShort_t)itdc;
        t_time     = calibs[irun]->get_tcorr(ifeech, itdc);
        t->Fill();
      }
    }
  }

  fout->cd();
  t->Write();
  std::cout << "Saved " << t->GetEntries() << " entries to timecorr.root" << std::endl;

  for (auto* c : calibs) delete c;
  delete fout;
}


void comp_timecorr(const std::string &file1 = "00065735-0000/mbd_timecorr.calib", const std::string &file2 = "00078986-0000/mbd_timecorr.calib")
{
  gStyle->SetOptStat(0);

  constexpr int NPMT  = 128;   // MBD PMTs
  constexpr int NTDC = 1000;   // 1000 values stored per channel
  constexpr double BAD_THRESHOLD = 0.1;   // 100 ps

  int verbose = 0;

  // Get run numbers
  int runno1 = Fun4AllUtils::GetRunSegment( std::filesystem::path( std::filesystem::path(file1).parent_path() ).filename().append(".root") ).first;
  int runno2 = Fun4AllUtils::GetRunSegment( std::filesystem::path( std::filesystem::path(file2).parent_path() ).filename().append(".root") ).first;
  std::cout << "Processing " << runno1 << "\t" << runno2 << std::endl;

  MbdGeom *mbdgeom = new MbdGeomV2();
  // ------------------------------------------------------------
  // Load calibrations
  // ------------------------------------------------------------
  MbdCalib *calib1 = new MbdCalib();
  MbdCalib *calib2 = new MbdCalib();

  std::cout << "Downloading time corrections from:\n" << "  " << file1 << "\n" << "  " << file2 << std::endl;

  calib1->Download_TimeCorr(file1);
  calib2->Download_TimeCorr(file2);

  int step1, min1, max1;  // NOLINT(readability-isolate-declaration)
  int step2, min2, max2;  // NOLINT(readability-isolate-declaration)
  calib1->get_tcorr_range(0,min1,max1,step1);
  calib2->get_tcorr_range(0,min2,max2,step2);
  if ( (step1 != step2) || (min1!=min2) || (max1!=max2) )
  {
    std::cerr << "timecorr shape differs" << std::endl;
    std::cerr << "min: " << min1 << "\t" << min2 << std::endl;
    std::cerr << "max: " << max1 << "\t" << max2 << std::endl;
    std::cerr << "steps: " << step1 << "\t" << step2 << std::endl;
    return;
  }

  // ------------------------------------------------------------
  // Histograms
  // ------------------------------------------------------------
  TH1 *h_tdiff[NPMT];
  TH1 *h_tdifftot;
  TString name;
  TString title;

  int nbins = ((max1-min1)/step1) + 1;
  std::cout << nbins << std::endl;
  for (int ipmt = 0; ipmt < NPMT; ++ipmt)
  {
    name = "h_tdiff"; name += ipmt;
    title = name; title += ", runs "; title += runno1; title += "-"; title += runno2;
    h_tdiff[ipmt] = new TH1F( name, title, nbins, min1-0.5, max1+0.5);
    h_tdiff[ipmt]->SetXTitle("TDC");
    h_tdiff[ipmt]->SetYTitle("tdiff [ns]");
  }
  title = "time diffs, all channels and TDCs, runs "; title += runno1; title += "-"; title += runno2;
  h_tdifftot = new TH1F( "h_tdifftot", title, 1000, -0.2, 0.2);
  h_tdifftot->SetXTitle("tdiff [ns]");

  // ------------------------------------------------------------
  // Comparison
  // ------------------------------------------------------------
  if ( verbose )
  {
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Ch  TDC  TimeCorr1  TimeCorr2  Diff" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
  }
  std::ofstream outfile("bad_comp_timecorr.txt");
  outfile << std::fixed << std::setprecision(4);
  outfile << "Ch  TDC  TimeCorr1  TimeCorr2  Diff" << std::endl;
  outfile << "----------------------------------------" << std::endl;
  
  for (int ipmt = 0; ipmt < NPMT; ++ipmt)
  {
    int ifeech = mbdgeom->get_feech(ipmt,0);

    for (int itdc = 0; itdc < NTDC; ++itdc)
    {
      float t1 = calib1->get_tcorr(ifeech, itdc);
      float t2 = calib2->get_tcorr(ifeech, itdc);
      float diff = t2 - t1;

      h_tdiff[ipmt]->SetBinContent(itdc+1,diff);
      h_tdifftot->Fill(diff);

      if (std::abs(diff) > BAD_THRESHOLD)
      {
        if ( verbose )
        {
          std::cout << std::setw(3) << ipmt << "  "
            << std::setw(3) << itdc << "  "
            << std::setw(10) << t1 << "  "
            << std::setw(10) << t2 << "  "
            << std::setw(8)  << diff
            << std::endl;
        }
        outfile << std::setw(3) << ipmt << "  "
          << std::setw(3) << itdc << "  "
          << std::setw(10) << t1 << "  "
          << std::setw(10) << t2 << "  "
          << std::setw(8)  << diff
          << std::endl;
      }
    }
  }

  // ------------------------------------------------------------
  // Plot
  // ------------------------------------------------------------
  TCanvas *ac[100];
  int icv = 0;
  
  ac[icv] = new TCanvas("ac0", "MBD TimeCorr by PMT", 1200, 600);
  TString pdfname = "comp_timecorr_"; pdfname += runno1; pdfname += "_"; pdfname += runno2; pdfname += ".pdf";
  ac[icv]->Print(pdfname + "[");

  // NOLINTBEGIN(modernize-loop-convert)
  for (int ipmt = 0; ipmt < NPMT; ++ipmt)
  {
    h_tdiff[ipmt]->Draw("hist");

    gPad->Modified();
    gPad->Update();
    ac[icv]->Print(pdfname);
  }
  // NOLINTEND(modernize-loop-convert)

  h_tdifftot->Draw();
  ac[icv]->Print(pdfname);
  ac[icv]->Print(pdfname + "]");

  icv++;

  outfile.close();
}
