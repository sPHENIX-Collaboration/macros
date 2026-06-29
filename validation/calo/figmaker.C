#include <sPhenixStyle.C>

#include <TCanvas.h>
#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TPad.h>
#include <TProfile2D.h>

#include <format>
#include <fstream>

void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize=0.04) {

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}

TH1* proj(TH2* h2);

TH1* FBratio(TH1* h){

   const int Nbin =  h->GetXaxis()->GetNbins();
   TH1* hfb = new TH1F("temp32","",Nbin/2,0,Nbin/2.);

   for(int i=0; i<Nbin/2; i++){
     int b1 = i+1;
     int b2 = Nbin-i;
     float ratio = h->GetBinContent(b1)/h->GetBinContent(b2);
     float err = sqrt( pow(h->GetBinError(b1)/h->GetBinContent(b1),2)+ pow(h->GetBinError(b2)/h->GetBinContent(b2),2))*pow(ratio,2);
     hfb->SetBinContent(i,ratio); 
     hfb->SetBinError(i,err);
   }
   return hfb;
}



void figmaker(){


  SetsPhenixStyle();

  int totalEvents = 0;

  std::ifstream infile("../condor/runList.txt");
  if (!infile) { std::cerr << "Error: Unable to open the file\n"; return;}
  std::vector<int> runList;
  int num;
  while (infile >> num)  runList.push_back(num);

  int Nruns= runList.size();

  for (int ir=0; ir<Nruns; ir++){

    int run = runList[ir];

    TFile* fin = new TFile(std::format("../condor/combine_out/out_{}.root",run).c_str());

    TH2* h_emcal_mbd_correlation  = (TH2*) fin->Get("h_emcal_mbd_correlation" );
    TH2* h_ihcal_mbd_correlation  = (TH2*) fin->Get("h_ihcal_mbd_correlation" );
    TH2* h_ohcal_mbd_correlation  = (TH2*) fin->Get("h_ohcal_mbd_correlation" );
    TH2* h_emcal_hcal_correlation = (TH2*) fin->Get("h_emcal_hcal_correlation");
    TH1* h_InvMass                = (TH1*) fin->Get("h_InvMass"               );
    TH2* h_cemc_etaphi            = (TH2*) fin->Get("h_cemc_etaphi_wQA"           );
    TH2* h_ihcal_etaphi           = (TH2*) fin->Get("h_ihcal_etaphi_wQA"          );
    TH2* h_ohcal_etaphi           = (TH2*) fin->Get("h_ohcal_etaphi_wQA"          );
    TH2* h_zdc_emcal_correlation  = (TH2*) fin->Get("h_zdc_emcal_correlation");
    TH1* hzdcNorthcalib           = (TH1*) fin->Get("hzdcNorthcalib");
    TH1* hzdcSouthcalib           = (TH1*) fin->Get("hzdcSouthcalib");
    TH2* h_etaphi_clus            = (TH2*) fin->Get("h_etaphi_clus"           );
    TH1* hvtx_z_raw               = (TH1*) fin->Get("hvtx_z_raw");
    TProfile2D* h_cemc_etaphi_time = (TProfile2D*) fin->Get("h_cemc_etaphi_time");
    TProfile2D* h_ihcal_etaphi_time = (TProfile2D*) fin->Get("h_ihcal_etaphi_time");
    TProfile2D* h_ohcal_etaphi_time = (TProfile2D*) fin->Get("h_ohcal_etaphi_time");

   TH2* h_cemc_e_chi2 = (TH2*) fin->Get("h_cemc_e_chi2");
   TH2* h_ohcal_e_chi2 = (TH2*) fin->Get("h_ohcal_e_chi2");
   TH2* h_ihcal_e_chi2 = (TH2*) fin->Get("h_ihcal_e_chi2");

    TCanvas* c1 = new TCanvas("c1", "c1", 400, 400);
    h_emcal_mbd_correlation ->Draw("COLZ");
    h_emcal_mbd_correlation ->SetXTitle("#Sigma #it{E}^{EMCal} [Arb]");
    h_emcal_mbd_correlation ->SetYTitle("#Sigma #it{E}^{MBD} [Arb]");
    h_emcal_mbd_correlation->GetXaxis()->SetNdivisions(505);
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

    c1->SaveAs(std::format("../plots/emcal_mbd_correlation_{}.pdf",run).c_str());


    TCanvas* c2 = new TCanvas("c2", "c2", 400, 400);
    h_ihcal_mbd_correlation ->Draw("COLZ");
    h_ihcal_mbd_correlation ->SetXTitle("#Sigma #it{E}^{ihcal} [Arb]");
    h_ihcal_mbd_correlation ->SetYTitle("#Sigma #it{E}^{MBD} [Arb]");
    h_ihcal_mbd_correlation->GetXaxis()->SetNdivisions(505);
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

    c2->SaveAs(std::format("../plots/ihcal_mbd_correlation_{}.pdf",run).c_str());

    TCanvas* c3 = new TCanvas("c3", "c3", 400, 400);
    h_ohcal_mbd_correlation ->Draw("COLZ");
    h_ohcal_mbd_correlation ->SetXTitle("#Sigma #it{E}^{ohcal} [Arb]");
    h_ohcal_mbd_correlation ->SetYTitle("#Sigma #it{E}^{MBD} [Arb]");
    h_ohcal_mbd_correlation->GetXaxis()->SetNdivisions(505);
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

    c3->SaveAs(std::format("../plots/ohcal_mbd_correlation_{}.pdf",run).c_str());


    TCanvas* c4 = new TCanvas("c4", "c4", 400, 400);
    h_emcal_hcal_correlation->Draw("COLZ");
    h_emcal_hcal_correlation->SetXTitle("#Sigma #it{E}^{EMCal} [Arb]");
    h_emcal_hcal_correlation->SetYTitle("#Sigma #it{E}^{HCal} [Arb]");
    h_emcal_hcal_correlation->GetXaxis()->SetNdivisions(505);
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

    c4->SaveAs(std::format("../plots/emcal_hcal_correlation_{}.pdf",run).c_str());


    TCanvas* c5 = new TCanvas("c5", "c5", 400, 400);
    h_InvMass               ->Draw("");
    h_InvMass               ->SetXTitle("#it{M}_{#gamma#gamma}");
    h_InvMass               ->SetYTitle("counts");
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());

    c5->SaveAs(std::format("../plots/InvMass_{}.pdf",run).c_str());


    TCanvas* c6 = new TCanvas("c6", "c6", 400, 400);
    h_cemc_etaphi           ->Draw("COLZ");
    h_cemc_etaphi           ->SetXTitle("#it{#eta}_{i} EMCal");
    h_cemc_etaphi           ->SetYTitle("#it{#phi}_{i} EMCal");
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80,1,"EMCal");
    gPad->SetRightMargin(0.15);

    c6->SaveAs(std::format("../plots/cemc_etaphi_{}.pdf",run).c_str());




    TCanvas* c7 = new TCanvas("c7", "c7", 400, 400);
    h_ihcal_etaphi          ->Draw("COLZ");
    h_ihcal_etaphi          ->SetXTitle("#it{#eta}_{i} iHcal");
    h_ihcal_etaphi          ->SetYTitle("#it{#phi}_{i} iHcal");
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80,1,"iHCal");
    gPad->SetRightMargin(0.15);

    c7->SaveAs(std::format("../plots/ihcal_etaphi_{}.pdf",run).c_str());


    TCanvas* c8 = new TCanvas("c8", "c8", 400, 400);
    h_ohcal_etaphi          ->Draw("COLZ");
    h_ohcal_etaphi          ->SetXTitle("#it{#eta}_{i} oHcal");
    h_ohcal_etaphi          ->SetYTitle("#it{#phi}_{i} oHcal");
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80,1,"oHCal");
    gPad->SetRightMargin(0.15);

    c8->SaveAs(std::format("../plots/ohcal_etaphi_{}.pdf",run).c_str());


    TH1* h_emcal_proj = (TH1*) proj(h_cemc_etaphi)->Clone("h_emcal_proj");

    TCanvas* c9 = new TCanvas("c9", "c9", 400, 400);
    h_emcal_proj->Draw("hist");
    h_emcal_proj->SetYTitle("N^{twr}(E_{T} > 1 GeV)");

    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80,1,"EMCal");
    
    c9->SaveAs(std::format("../plots/cemc_proj_{}.pdf",run).c_str());


    TH1* h_ohcal_proj = (TH1*) proj(h_ohcal_etaphi)->Clone("h_ohcal_proj");

    TCanvas* c10 = new TCanvas("c10", "c10", 400, 400);
    h_ohcal_proj->Draw("hist");
    h_ohcal_proj->SetYTitle("N^{twr}(E_{T} > 1 GeV)");
    h_ohcal_proj->GetYaxis()->SetRangeUser(0, h_ohcal_proj->GetMaximum()*1.05);

    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80,1,"oHCal");
    
    c10->SaveAs(std::format("../plots/ohcal_proj_{}.pdf",run).c_str());


    TH1* h_ihcal_proj = (TH1*) proj(h_ihcal_etaphi)->Clone("h_ihcal_proj");

    TCanvas* c11 = new TCanvas("c11", "c10", 400, 400);
    h_ihcal_proj->Draw("hist");
    h_ihcal_proj->SetYTitle("N^{twr}(E_{T} > 1 GeV)");
    h_ihcal_proj->SetXTitle("#eta_{i}");

    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80,1,"iHCal");
    
    c11->SaveAs(std::format("../plots/ihcal_proj_{}.pdf",run).c_str());


    TH1* h_fb_ratio_emcal = FBratio(h_emcal_proj);
    TH1* h_fb_ratio_ihcal = FBratio(h_ihcal_proj);
    TH1* h_fb_ratio_ohcal = FBratio(h_ohcal_proj);

    TCanvas* c12 = new TCanvas("c12", "c12", 400, 400);
    h_fb_ratio_emcal->Draw("ex0");
    h_fb_ratio_emcal->SetYTitle("N^{twr}(#eta_{i})/N^{twr}(#eta_{N-i})");
    h_fb_ratio_emcal->SetXTitle("#eta_{i}");
    h_fb_ratio_emcal->GetYaxis()->SetRangeUser(0.1,2);

    h_fb_ratio_ohcal->Draw("ex0 same");
    h_fb_ratio_ohcal->SetLineColor(kBlue);
    h_fb_ratio_ohcal->SetMarkerColor(kBlue);
    h_fb_ratio_ohcal->SetMarkerStyle(22);

    h_fb_ratio_ihcal->Draw("ex0 same");
    h_fb_ratio_ihcal->SetLineColor(kRed);
    h_fb_ratio_ihcal->SetMarkerColor(kRed);
    h_fb_ratio_ihcal->SetMarkerStyle(33);

    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80,1,"EMCal");
    myText(0.32,0.80,kBlue,"oHCal");
    myText(0.42,0.80,kRed,"iHCal");
    
    c12->SaveAs(std::format("../plots/h_fb_ratio_emcal_{}.pdf",run).c_str());


    TCanvas* c13 = new TCanvas("c13", "c13", 400, 400);
    h_zdc_emcal_correlation->Draw("COLZ");
    h_zdc_emcal_correlation->SetXTitle("#Sigma #it{E}^{EMCal} [Arb]");
    h_zdc_emcal_correlation->SetYTitle("#Sigma #it{E}^{ZDC} [Arb]");
    h_zdc_emcal_correlation->GetXaxis()->SetNdivisions(505);
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    gPad->SetRightMargin(0.15);
    gPad->SetLogz();

    c13->SaveAs(std::format("../plots/zdc_emcal_correlation_{}.pdf",run).c_str());


    TCanvas* c14 = new TCanvas("c14", "c14", 400, 400);
    hzdcNorthcalib->Draw();
    hzdcNorthcalib->SetLineColor(kBlue);
    hzdcNorthcalib->GetXaxis()->SetRangeUser(0.0,12000);
    hzdcNorthcalib->SetXTitle("#Sigma #it{E}^{ZDC Side}");
    hzdcNorthcalib->SetYTitle("Events");
    hzdcNorthcalib->GetXaxis()->SetNdivisions(505);

    hzdcSouthcalib->Draw("same");
    hzdcSouthcalib->SetLineColor(kRed);
    gPad->SetLogy();

    myText(0.7, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.7, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.75, 0.80, kBlue,"North");
    myText(0.65, 0.80, kRed,"South");

    c14->SaveAs(std::format("../plots/zdc_e_northSouth_{}.pdf",run).c_str());




    TCanvas* c15 = new TCanvas("c15", "c15", 400, 400);
    h_etaphi_clus          ->Draw("COLZ");
    h_etaphi_clus          ->SetXTitle("#it{#eta}_{i} EMCal Clusters");
    h_etaphi_clus          ->SetYTitle("#it{#phi}_{i} EMCal Clusters");
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    gPad->SetRightMargin(0.15);

    c15->SaveAs(std::format("../plots/etaphi_clus_{}.pdf",run).c_str());

    totalEvents += hvtx_z_raw->GetEntries();
    int events =  hvtx_z_raw->GetEntries();

    TCanvas* c16 = new TCanvas("c16", "c16", 400, 400);
    hvtx_z_raw          ->Draw("COLZ");
    hvtx_z_raw          ->SetXTitle("MBD Vertex #it{z} [cm]");
    hvtx_z_raw          ->SetYTitle("Events");
    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80, 1, std::format("events {}", events).c_str());

    c16->SaveAs(std::format("../plots/vtx_z_{}.pdf",run).c_str());


    TCanvas* c17 = new TCanvas("c17", "c17", 400, 400);
    hzdcNorthcalib->Draw();
    hzdcNorthcalib->SetLineColor(kBlue);
    hzdcNorthcalib->GetXaxis()->SetRangeUser(10,300);
    hzdcNorthcalib->SetXTitle("#Sigma #it{E}^{ZDC Side}");
    hzdcNorthcalib->SetYTitle("Events");

    TGraph* gr_1n = new TGraph();
    gr_1n->SetPoint(0,100,0);
    gr_1n->SetPoint(1,100,1e7);
    gr_1n->SetLineStyle(7);
    gr_1n->Draw("l");

    hzdcSouthcalib->Draw("same");
    hzdcSouthcalib->SetLineColor(kRed);
    gPad->SetLogy();

    myText(0.7, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.7, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.75, 0.80, kBlue,"North");
    myText(0.65, 0.80, kRed,"South");

    c17->SaveAs(std::format("../plots/zdc_e_northSouth_1n_{}.pdf",run).c_str());


    TCanvas* c18 = new TCanvas("c18", "c18", 400, 400);
    h_cemc_etaphi_time->Draw("COLZ");
    h_cemc_etaphi_time->SetXTitle("#it{#eta}_{i} EMCal");
    h_cemc_etaphi_time->SetYTitle("#it{#phi}_{i} EMCal");
    h_cemc_etaphi_time->GetXaxis()->SetNdivisions(505);

    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80, 1, "mean hit peak time EMCal");
    gPad->SetRightMargin(0.15);

    c18->SaveAs(std::format("../plots/cemc_etaphi_time{}.pdf",run).c_str());

    TCanvas* c19 = new TCanvas("c19", "c19", 400, 400);
    h_ohcal_etaphi_time->Draw("COLZ");
    h_ohcal_etaphi_time->SetXTitle("#it{#eta}_{i} oHcal");
    h_ohcal_etaphi_time->SetYTitle("#it{#phi}_{i} oHcal");
    h_ohcal_etaphi_time->GetXaxis()->SetNdivisions(505);

    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80, 1, "mean hit peak time Outter HCal");
    gPad->SetRightMargin(0.15);

    c19->SaveAs(std::format("../plots/ohcal_etaphi_time{}.pdf",run).c_str());


    TCanvas* c20 = new TCanvas("c20", "c20", 400, 400);
    h_ihcal_etaphi_time->Draw("COLZ");
    h_ihcal_etaphi_time->SetXTitle("#it{#eta}_{i} iHcal");
    h_ihcal_etaphi_time->SetYTitle("#it{#phi}_{i} iHcal");
    h_ihcal_etaphi_time->GetXaxis()->SetNdivisions(505);

    myText(0.22, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.22, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.22, 0.80, 1, "mean hit peak time Inner HCal");
    gPad->SetRightMargin(0.15);

    c20->SaveAs(std::format("../plots/ihcal_etaphi_time{}.pdf",run).c_str());

    TCanvas* c21 = new TCanvas("c21", "c21", 400, 400);
    h_ihcal_e_chi2->Draw("COLZ");
    h_ihcal_e_chi2->SetXTitle("#i{E} [GeV] iHcal");
    h_ihcal_e_chi2->SetYTitle("chi2 iHcal");
    h_ihcal_e_chi2->GetXaxis()->SetNdivisions(505);
    h_ihcal_e_chi2->GetXaxis()->SetRangeUser(-1,2);
    gPad->SetLogy();
    gPad->SetLogz();

    myText(0.52, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.52, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.52, 0.80, 1, "Inner HCal");
    gPad->SetRightMargin(0.15);

    c21->SaveAs(std::format("../plots/ihcal_e_chi2{}.pdf",run).c_str());


    TCanvas* c22 = new TCanvas("c22", "c22", 400, 400);
    h_ohcal_e_chi2->Draw("COLZ");
    h_ohcal_e_chi2->SetXTitle("#i{E} [GeV] oHcal");
    h_ohcal_e_chi2->SetYTitle("chi2 oHcal");
    h_ohcal_e_chi2->GetXaxis()->SetNdivisions(505);
    h_ohcal_e_chi2->GetXaxis()->SetRangeUser(-1,7);
    gPad->SetLogy();
    gPad->SetLogz();

    myText(0.52, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.52, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.52, 0.80, 1, "Outer HCal");
    gPad->SetRightMargin(0.15);

    c22->SaveAs(std::format("../plots/ohcal_e_chi2{}.pdf",run).c_str());


    TCanvas* c23 = new TCanvas("c23", "c23", 400, 400);
    h_cemc_e_chi2->Draw("COLZ");
    h_cemc_e_chi2->SetXTitle("#i{E} [GeV] EMCal ");
    h_cemc_e_chi2->SetYTitle("chi2 EMCal");
    h_cemc_e_chi2->GetXaxis()->SetNdivisions(505);
    h_cemc_e_chi2->GetXaxis()->SetRangeUser(-1,15);
    gPad->SetLogy();
    gPad->SetLogz();

    myText(0.52, 0.9, 1, "#it{#bf{sPHENIX}} Internal");
    myText(0.52, 0.85, 1, std::format("run {}", run).c_str());
    myText(0.52, 0.80, 1, "EMCal");
    gPad->SetRightMargin(0.15);

    c23->SaveAs(std::format("../plots/cemc_e_chi2{}.pdf",run).c_str());

  }

  for (int ir=0; ir<Nruns; ir++){
    std::cout << runList[ir] << ",";
  }
  std::cout << std::endl;
  std::cout << "total events=" << totalEvents <<std::endl;
  std::cout << std::endl;

}






TH1* proj(TH2* h2){

  int x = h2->GetXaxis()->GetNbins();
  int y = h2->GetYaxis()->GetNbins();
  TH1* h = (TH1*) h2->ProjectionX("temp");
  h->Reset();
  for (int ix=1; ix<x+1; ix++){
    float sum = 0;
    float cc = 0;
    for(int iy=1; iy<y+1; iy++){
      float bc = h2->GetBinContent(ix,iy);
      if (bc==0) cc+=1;
      sum += bc;
    }
    if (cc==y) continue;
    float sum_cor = sum*y/(y-cc);
    h->SetBinContent(ix,sum_cor);
    h->SetBinError(ix,std::sqrt(sum_cor));
  }


  return h;
}

