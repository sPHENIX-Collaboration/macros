#include <TCanvas.h>
#include <TFile.h>
#include <TH2.h>
#include <TStyle.h>

TH2* DivideColumnsByAverage(TH2* hist) {
    // Get the number of bins along x and y axes
    int numBinsX = hist->GetNbinsX();
    int numBinsY = hist->GetNbinsY();

    // Create a new TH2F histogram for storing the result
    TH2* dividedHistogram = new TH2F("dividedHistogram", "Histogram with Columns Divided by Average",
                                      numBinsX, hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(),
                                      numBinsY, hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());

    // Loop over all columns
    for (int binX = 1; binX <= numBinsX; binX++) {
        // Calculate the average bin content for the current column
        double sum = 0.0;
        for (int binY = 1; binY <= numBinsY; binY++) {
            sum += hist->GetBinContent(binX, binY);
        }
        double average = (numBinsY > 0) ? sum / numBinsY : 0.0;

        // Divide each bin content in the column by the average
        for (int binY = 1; binY <= numBinsY; binY++) {
            double content = hist->GetBinContent(binX, binY);
            double dividedContent = (average != 0.0) ? content / average : 0.0;
            dividedHistogram->SetBinContent(binX, binY, dividedContent);
        }
    }

    return dividedHistogram;
}


void quickPlot(){

  TFile* fin = new TFile("combine_out/out_23727.root");
  TH2* h_ohcal_hit {nullptr};
  fin->GetObject("h_ohcal_etaphi_wQA",h_ohcal_hit);
  TH2* h_ihcal_hit {nullptr};
  fin->GetObject("h_ihcal_etaphi_wQA",h_ihcal_hit);
  //TH2* h_ihcal_hit {nullptr};
  //fin->GetObject("h_ihcal_etaphi_wQA",h_ihcal_hit);
  

  TH2*  h_ohcal_hit_avg = DivideColumnsByAverage(h_ohcal_hit);
  TH2*  h_ihcal_hit_avg = DivideColumnsByAverage(h_ihcal_hit);

  TCanvas* c1 = new TCanvas("c1","c1",600,600);
  h_ohcal_hit_avg->Draw("COLZ");
  h_ohcal_hit_avg->SetXTitle("ohcal ieta");
  h_ohcal_hit_avg->SetYTitle("ohcal iphi");
  c1->Update();
  
  TCanvas* c2 = new TCanvas("c2","c2",600,600);
  h_ihcal_hit_avg->Draw("COLZ");
  h_ihcal_hit_avg->SetXTitle("ihcal ieta");
  h_ihcal_hit_avg->SetYTitle("ihcal iphi");
  c2->Update();
  

  TFile* fout = new TFile("quickPlotSave.root","recreate");
  h_ohcal_hit_avg->Write("h_ohcal_hit_avg");
  h_ihcal_hit_avg->Write("h_ihcal_hit_avg");
   
  gStyle->SetOptStat(0);

  h_ohcal_hit->Write();
  h_ihcal_hit->Write();
  fout->Close();

}
