#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include "TH1F.h"
#include "TH1D.h"
#include "TGraph.h"
#include "TCanvas.h"

class CalAnalyzer {
private:

  const int NPMT{128};
  int NPAR{9};

  std::string run_name;

  struct CalibrationStats {
        double mean;
        double rms;
        int count;
  };

  // Data storage: [RunID][ChannelIdx]
  std::map<int, std::vector<std::vector<double>>> dataStore;

  // Stats: [ParamIdx][ChannelIdx]
  std::vector<std::vector<CalibrationStats>> cleanStats;

  // Plots: [ParamIdx][ChannelIdx]
  std::vector<std::vector<TH1F*>> hists;
  std::vector<std::vector<TGraph*>> graphs;

public:
  explicit CalAnalyzer(std::string_view input_name = "none")
  : run_name{input_name}
  {
    if ( run_name.find("auau") != std::string::npos )
    {
      NPAR = 5;
    }

    // Initialize 2D vectors for hists and graphs: NPAR parameters x NPMT channels
    hists.resize(NPAR, std::vector<TH1F*>(NPMT));
    graphs.resize(NPAR, std::vector<TGraph*>(NPMT));
    cleanStats.resize(NPAR, std::vector<CalibrationStats>(NPMT));

    TString name;
    TString title;
    for (int ch = 0; ch < NPMT; ch++)
    {
      for (int p = 0; p < NPAR; p++)
      {
        name = "h_ch"; name += ch; name += "_p"; name += p;
        title = "Dist: Param "; title += p; title += ", Ch "; title += ch; title += ";Value;Entries";
        hists[p][ch] = new TH1F(name, title, 200, 0., -1.);

        graphs[p][ch] = new TGraph();
        name = "g_ch"; name += ch; name += "_p"; name += p;
        graphs[p][ch]->SetName( name );
        title = "Param "; title += p; title += ", Ch "; title += ch; title += " vs Run;Run ID;Value";
        graphs[p][ch]->SetTitle( title );
        graphs[p][ch]->SetMarkerStyle(20);
        graphs[p][ch]->SetMarkerSize(0.2);
      }
    }
  }

  ~CalAnalyzer()
  {
    for (int ch = 0; ch < NPMT; ch++)
    {
      for (int p = 0; p < NPAR; p++)
      {
        delete hists[p][ch];
        delete graphs[p][ch];
      }
    }
  }

  CalAnalyzer(const CalAnalyzer&) = delete;
  CalAnalyzer& operator=(const CalAnalyzer&) = delete;
  CalAnalyzer(CalAnalyzer&&) = delete;
  CalAnalyzer& operator=(CalAnalyzer&&) = delete;


  void LoadRun(int runID, const std::string& filename)
  {
    std::ifstream file(filename);
    if (!file.is_open()) {
      std::cerr << "Error: Could not open " << filename << std::endl;
      return;
    }

    std::vector<std::vector<double>> channels;
    channels.resize(NPAR, std::vector<double>(NPMT));
    int pmtch{-1};
    while (file >> pmtch)
    {
      //cout << runID << "\t" << pmtch << endl;
      if (pmtch >= 0 && pmtch < NPMT)
      {
        // other stuff
        double seed_threshold, seed_minrej, seed_natpeak, seed_maxrej, seed_qmax; // NOLINT
        file >> seed_threshold >> seed_minrej >> seed_natpeak >> seed_maxrej >> seed_qmax;

        //cout << "xxx " << pmtch << "\t" << seed_qmax << endl;
        for (int p = 0; p < NPAR; ++p) {
          double val;
          file >> val;

          /*
             if ( p==1 && pmtch==0 )
             {
             cout << "xxx " << runID << "\t" << p << "\t" << pmtch << "\t" << val << endl;
             }
             */

          channels[p][pmtch] = val;

          // Update Histogram
          hists[p][pmtch]->Fill( val );

          // Update Graph
          int nextPt = graphs[p][pmtch]->GetN();
          // should add second axis with runnums
          //graphs[p][pmtch]->SetPoint(nextPt, (double)runID, val);
          graphs[p][pmtch]->SetPoint(nextPt, (double)nextPt, val);
        }
      }
    }
    dataStore[runID] = channels;


    file.close();
  }

  void CalculateCleanStats(double sigmaEquivalent = 3.0)
  {
    // Set Min and Max of graphs
    for (int ipmt=0; ipmt<NPMT; ipmt++)
    {
      for (int ipar=0; ipar<NPAR; ipar++)
      {
        CalculateCleanStats(ipar,ipmt,sigmaEquivalent);
      }
    }
  }

  /**
   * Calculates Mean and RMS after removing outliers
   * @param p parameter index (0-NPAR)
   * @param ch pmtch (0-NPMT)
   * @param sigmaEquivalent number of standard deviations to keep (usually 3.0)
   */
  void CalculateCleanStats(int p, int ch, double sigmaEquivalent = 3.0)
  {
    //cout << "In CalculateCleanStats " << ch << "\t" << p << endl;
    if (p < 0 || p >= NPAR || ch < 0 || ch >= NPMT) return;

    std::vector<double> values;
    for (auto const& [run, runData] : dataStore)
    {
      if ( std::isnan(runData[p][ch]) )
      {
        continue;
      }

      values.push_back(runData[p][ch]);
    }

    if (values.empty()) return;

    // 1. Find the Median
    std::sort(values.begin(), values.end());
    double middle = values[values.size() / 2];

    // 2. Find the Median Absolute Deviation (MAD)
    std::vector<double> diffs;
    for (double v : values)
    {
      diffs.push_back(std::abs(v - middle));  // NOLINT
    }
    std::sort(diffs.begin(), diffs.end());
    double mad = diffs[diffs.size() / 2];

    // 3. Convert MAD to a "Sigma Equivalent" 
    // For normal distributions, 1 sigma approx 1.4826 * MAD
    double robustSigma = 1.4826 * mad;
    if (robustSigma == 0) robustSigma = 1e-9; // Avoid division by zero

    // 4. Pass 2: Calculate Mean/RMS only for points within the robust window
    double sum = 0;
    double sumSq = 0;
    int n = 0;
    for (double v : values) {
      if (std::abs(v - middle) < (sigmaEquivalent * robustSigma)) {
        sum += v;
        sumSq += v * v;
        n++;
      }
    }

    if (n > 0) {
      double mean = sum / n;
      double rms = std::sqrt(std::abs((sumSq / n) - (mean * mean)));
      cleanStats[p][ch] = {mean, rms, n};

      //cout << "aaa " << ch << "\t" << p << "\t" << middle << "\t" << mad << "\t" << mean << "\t" << rms << "\t" << n << endl;

      // Update the to focus on the "Clean" area
      graphs[p][ch]->SetMinimum(mean - 12*rms);
      graphs[p][ch]->SetMaximum(mean + 10*rms);

      delete hists[p][ch];
      TString name = "h_ch"; name += ch; name += "_"; name += p;
      TString title = "Dist: Param"; title += p; title += ", Ch "; title += ch; title += ";Value;Entries";
      hists[p][ch] = new TH1F(name, title, 200, mean-12*rms, mean+10*rms);
      for (double v : values)
      {
        hists[p][ch]->Fill( v );
      }
    }

  }

  CalibrationStats GetCleanStats(int ch, int p) { return cleanStats[p][ch]; }

  /**
   * Saves the calculated means to a text file.
   * Format: ch  par0  par1  par2  ...  NPAR
   */
  void SaveCleanMeans(const std::string& outFilename = "temp_mipseeds.txt")
  {
    std::ofstream outFile(outFilename);
    if (!outFile.is_open()) {
      std::cerr << "Error: Could not open " << outFilename << " for writing." << std::endl;
      return;
    }

    /*
    // Header
    outFile << "# ch";
    for (int p = 0; p < NPAR; ++p)
    {
      outFile << "      par" << p;
    }
    outFile << std::endl;
    */

    // Set fixed precision for clean columns
    outFile << std::fixed << std::setprecision(6);

    for (int ch = 0; ch < NPMT; ++ch) {
      outFile << std::setw(3) << ch; // Channel number column

      for (int p = 0; p < NPAR; ++p) {
        // Access the clean mean we calculated using the MAD algorithm
        double meanVal = cleanStats[p][ch].mean;

        // If stats haven't been calculated for this channel yet, 
        // fallback to the raw histogram mean
        if (cleanStats[p][ch].count == 0) {
          meanVal = hists[p][ch]->GetMean();
        }

        outFile << "  " << std::setw(10) << meanVal;
      }
      outFile << std::endl;
    }

    outFile.close();
    std::cout << "Successfully saved clean parameters to: " << outFilename << std::endl;
  }

  // Accessors
  double GetVal(int runID, int ch, int p)
  {
    return (dataStore.count(runID)) ? dataStore[runID][p][ch] : -1e9;
  }

  TH1* GetHist(int ch, int p) { return hists[p][ch]; }

  TGraph* GetGraph(int ch, int p) { return graphs[p][ch]; }

  void DrawSummary(int ch, int p)
  {
    TString name = "c_"; name += ch; name += "_"; name += p;
    TCanvas *c = new TCanvas(name, "Calibration Summary", 800, 400);
    c->Divide(2, 1);
    c->cd(1);
    hists[p][ch]->Draw();
    c->cd(2);
    graphs[p][ch]->Draw("AP");
  }

  /**
   * Generates a multi-page PDF for each parameter.
   * Each page contains the TH1 and TGraph for one channel.
   */
  void SaveParameterPDFs()
  {
    TCanvas *c = new TCanvas("c_print", "Printing Canvas", 1000, 500);
    c->Divide(2, 1);

    // num mip fits out of range
    TString mipfitname = "mbd_"; mipfitname += run_name; mipfitname += "_mip.result";
    std::ofstream mipfile( mipfitname.Data() );

    int par_mip = 6;
    if ( run_name.find("auau") != std::string::npos )
    {
      par_mip = 1;
    }

    for (int p = 0; p < NPAR; ++p)
    {
      TString pdfname = "mbd_"; pdfname += run_name; pdfname += "_par"; pdfname += p; pdfname += ".pdf";

      // Start the PDF file
      c->Print(pdfname + "[");

      for (int ch = 0; ch < NPMT; ++ch)
      {
        // Left side: Histogram
        c->cd(1);
        hists[p][ch]->SetLineColor(kBlue);
        hists[p][ch]->Draw();

        // Right side: TGraph (Run-by-Run)
        c->cd(2);
        graphs[p][ch]->SetMarkerColor(kRed);
        //graphs[p][ch]->SetDrawOption("AP");
        graphs[p][ch]->Draw("AP");

        // Print current page
        c->Print(pdfname);

        if ( p == par_mip )
        {
          int oflowbin = hists[p][ch]->GetNbinsX() + 1;
          mipfile << ch << "\t" << hists[p][ch]->GetBinContent(0) << "\t" << hists[p][ch]->GetBinContent(oflowbin) << std::endl;
        }

      }

      // Close the PDF file
      c->Print(pdfname + "]"); 
      std::cout << "Created: " << pdfname << std::endl;

      if ( p==6 )
      {
        mipfile.close();
      }
    }
    delete c;
  }
};
