//provisional macro to standardize plotting scheme for approved sPHENIX plots
//May 30 2023

#include "sPhenixStyle.h"
#include "sPhenixStyle.C"

void sPHENIXplots()
{
  SetsPhenixStyle();

  TCanvas *c = new TCanvas("c", "c", 685, 630);
  gPad->SetLogz();
  gPad->SetRightMargin(0.15);

  TLegend *leg = new TLegend(.12,.78,.4,.9);
  leg->SetFillStyle(0);
  leg->AddEntry("","#it{#bf{sPHENIX}} Preliminary","");
  leg->AddEntry("","Au+Au #sqrt{s_{NN}} = 200 GeV","");

  TH2D *h = new TH2D("", "", 100, 0, 1, 100, 0, 1);
  h->GetYaxis()->SetNdivisions(405);
  h->GetXaxis()->SetNdivisions(405);
  h->SetXTitle("x-axis title [arb. units]");
  h->SetYTitle("y-axis title [arb. units]");
  h->Draw("col z");
  leg->Draw("same");
  c->Print("plot1.pdf");
}
