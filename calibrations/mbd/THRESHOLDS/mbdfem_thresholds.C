#include <mbd/MbdCalib.h>

#include <TCanvas.h>
#include <TChain.h>
#include <TF1.h>
#include <TFile.h>
#include <TH2.h>
#include <TMath.h>
#include <TStyle.h>

#include <filesystem>
#include <fstream>

R__LOAD_LIBRARY(libmbd.so)

TH2 *h2_qraw{nullptr};
TH2 *h2_qcut{nullptr};
TH2 *h2_qratio{nullptr};
TH1 *h_qraw[128]{nullptr};
TH1 *h_qcut[128]{nullptr};
TH1 *h_qratio[128]{nullptr};
TF1 *turnon[128] = {nullptr};

TCanvas *ac{nullptr};

Double_t TrigTurnOn(Double_t *x, Double_t *par)  // NOLINT(readability-non-const-parameter)
{
  // par[0] is the amplitude (turn on max)
  // par[1] is the stretch (how long it takes to reach max)
  // par[2] is the offset (where it reaches max)
  // x[0] is the energy (or pt)
  Double_t y = 0.5 * par[0] * TMath::Erfc(-(sqrt(2) / par[1]) * (x[0] - par[2]));

  return y;
}

void ana_channels(const std::string &fname = "thresholds-00042123-0001.root")
{
  TFile *infile{nullptr};
  if (!fname.empty())
  {
    infile = new TFile(fname.c_str(), "READ");
    h2_qraw = (TH2F *) infile->Get("h2_qraw");
    h2_qcut = (TH2F *) infile->Get("h2_qcut");
    h2_qratio = (TH2F *) infile->Get("h2_qratio");
  }

  // outfile is used for updating thresholds in 1008
  TString name = fname;
  name.ReplaceAll(".root", ".out");
  std::ofstream outfile(name.Data());

  name = fname;
  name.ReplaceAll(".root", ".calib");
  std::ofstream calibfile(name.Data());

  ac = new TCanvas("ac", "threshold turn-on", 800, 600);

  TString pdfname = fname;
  pdfname.ReplaceAll(".root", ".pdf");
  ac->Print(pdfname + "[");

  TString title;
  for (int ipmt = 0; ipmt < 128; ipmt++)
  {
    name = "h_qraw";
    name += ipmt;
    h_qraw[ipmt] = h2_qraw->ProjectionX(name, ipmt + 1, ipmt + 1);
    name = "h_qcut";
    name += ipmt;
    h_qcut[ipmt] = h2_qcut->ProjectionX(name, ipmt + 1, ipmt + 1);
    name = "h_ratio";
    name += ipmt;
    h_qratio[ipmt] = h2_qratio->ProjectionX(name, ipmt + 1, ipmt + 1);
    title = "ch";
    title += ipmt;
    title += "threshold";
    h_qratio[ipmt]->SetTitle(title);

    h_qraw[ipmt]->Sumw2();
    h_qcut[ipmt]->Sumw2();

    /*
    name = "h_ratio"; name += ipmt;
    h_qratio[ipmt] = (TH1*)h_qcut[ipmt]->Clone( name );
    h_qratio[ipmt]->Divide( h_qcut[ipmt], h_qraw[ipmt], 1, 1, "B" );
    */

    // force eff=0 at adc=0
    h_qratio[ipmt]->SetBinContent(1, 0.);
    h_qratio[ipmt]->SetBinError(1, 0.001);

    name = "turnon";
    name += ipmt;
    turnon[ipmt] = new TF1(name, TrigTurnOn, 0, 200, 3);
    turnon[ipmt]->SetLineColor(4);

    turnon[ipmt]->SetParameters(1, 20, 40);
    h_qratio[ipmt]->Fit(turnon[ipmt], "R");
    h_qratio[ipmt]->Draw();
    gPad->Modified();
    gPad->Update();

    ac->Print(pdfname);

    double mean = turnon[ipmt]->GetParameter(2);   // midpoint of turn-on
    double width = turnon[ipmt]->GetParameter(1);  // width of turn-on
    double eff = turnon[ipmt]->GetParameter(0);    // efficiency at max

    // For changing thresholds
    double threshold_to_set = 5;                                       // if we want threshold at ADC=50
    double effic_at = threshold_to_set - round((mean + width) / 10.);  // about 10 mV per ADC

    double meanerr = turnon[ipmt]->GetParError(2);   // error, midpoint of turn-on
    double widtherr = turnon[ipmt]->GetParError(1);  // error, width of turn-on
    double efferr = turnon[ipmt]->GetParError(0);    // error, efficiency

    double chi2ndf = turnon[ipmt]->GetChisquare() / turnon[ipmt]->GetNDF();  // error, width of turn-on

    outfile << ipmt << "\t" << mean << "\t" << width << "\t" << effic_at << std::endl;
    calibfile << ipmt << "\t" << mean << "\t" << meanerr << "\t" << width << "\t" << widtherr << "\t"
              << eff << "\t" << efferr << chi2ndf << std::endl;

    /*
    if ( ipmt==23 )
    {
      string junk;
      std::cout << "? ";
      cin >> junk;
    }
    */
  }
  ac->Print(pdfname + "]");

  outfile.close();
  calibfile.close();
}

void mbdfem_thresholds(const std::filesystem::path &fname = "DST_UNCALMBD-00042123-0001.root")
{
  gStyle->SetOptStat(0);

  TChain *T = new TChain("T");
  TString savefname = "thresholds_";
  savefname += fname.stem().c_str();

  if (fname.extension() == ".list")
  {
    std::cout << "is list" << std::endl;
    std::string dstfname;

    std::ifstream inflist(fname.c_str());
    while (inflist >> dstfname)
    {
      std::cout << "Added " << dstfname << std::endl;
      T->Add(dstfname.c_str());
    }

    savefname += "-99999.root";
  }
  else
  {
    std::cout << "is root file" << std::endl;
    T->Add(fname.c_str());

    savefname.ReplaceAll("DST_UNCALMBD_", "");
    savefname.ReplaceAll("DST_CALO_", "");
    savefname.ReplaceAll("DST_CALOFITTING_", "");
    savefname += ".root";
  }

  TFile *savefile = new TFile(savefname, "RECREATE");

  h2_qraw = new TH2F("h2_qraw", "ch vs bq", 200, 0.5, 200.5, 128, 0, 128);
  h2_qraw->SetXTitle("ADC");
  h2_qraw->SetYTitle("pmt");
  h2_qcut = (TH2 *) h2_qraw->Clone("h2_qcut");
  h2_qcut->SetTitle("ch vs bq, cut");
  h2_qratio = (TH2 *) h2_qraw->Clone("h2_qratio");
  h2_qratio->SetTitle("ch vs bq, cut/raw ratio");

  //  TCanvas *ac = new TCanvas("ac","ac",550*1.5,425*1.5);

  T->Draw("bpmt:bq>>h2_qraw", "bq>0&&bq<200.5", "colz");
  T->Draw("bpmt:bq>>h2_qcut", "bq>0&&bq<200.5&&abs(btt)<50", "colz");

  h2_qratio->Divide(h2_qcut, h2_qraw, 1, 1, "B");
  h2_qratio->Draw("colz");

  // TCanvas *bc = new TCanvas("bc","bc",550*1.5,425*1.5);
  // ana_channels(savefname.Data());

  savefile->cd();

  h2_qraw->Write();
  h2_qcut->Write();
  h2_qratio->Write();

  /*
     for (int ich=0; ich<128; ich++)
     {
     h_qraw[ich]->Write();
     h_qcut[ich]->Write();
     h_qcut[ich]->SetLineColor(2);
     h_qcut[ich]->SetMarkerColor(2);
     h_qratio[ich]->Write();
     }
     */

  savefile->Write();
}
