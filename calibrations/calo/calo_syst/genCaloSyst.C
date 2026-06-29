#include "cdbHistConv.C"

#include <TCanvas.h>
#include <TRandom3.h>

#include <format>

void genCaloSyst()
{
  // std::string calo = "CEMC";
  // std::string calo = "HCALOUT";
  // std::string calo = "HCALIN";
  std::vector<std::string> calos = {"CEMC", "HCALOUT", "HCALIN"};

  for (auto calo : calos)
  {
    // int begin = 54908;
    // int end   = 54921;
    int begin = 46000;
    int end   = 53862;
  
    float tbyt_stat;
    float calib_unc;
    float sys_v1;
//    float sys_v2;
    float had_frac;
    int eta;
    int phi;
    bool embool; // embool = 0 for EMCal shape, embool = 1 for HCal shape
    float had_resp_unc = 0.117;  // value from test beam

    if (calo == "CEMC")
    {
      tbyt_stat = 0.03;  // tower by tower stat uncertainty
      calib_unc = 0.026;     // global scale shift
      sys_v1 = 0.02;
      //      sys_v2 = 0.02;
      had_frac = 0.61;
      embool = false;
      eta = 96;
      phi = 256;
    }
    if (calo == "HCALOUT")
    {
      tbyt_stat = 0.02;  // tower by tower stat uncertainty
      calib_unc = 0.02;      // global scale shift
      sys_v1 = 0.02;
      //      sys_v2 = 0.02;
      had_frac = 0.972;
      embool = true;
      eta = 24;
      phi = 64;
    }
    if (calo == "HCALIN")
    {
      tbyt_stat = 0.05;  // tower by tower stat uncertainty
      calib_unc = 0.05;       // global scale shift
      sys_v1 = 0.02;
      //      sys_v2 = 0.05;
      had_frac = 0.961;
      embool = true;
      eta = 24;
      phi = 64;
    }
  
    TH2F* h_sys = new TH2F("h_sys","",eta,0,eta,phi,0,phi);
    TH2F* h_calib_stat      = (TH2F*) h_sys->Clone("h_calib_stat");
    TH2F* h_calib_unc_up     = (TH2F*) h_sys->Clone("h_calib_unc_up");
    TH2F* h_v1        = (TH2F*) h_sys->Clone("h_v1");
    TH2F* h_v2        = (TH2F*) h_sys->Clone("h_v2");
    TH2F* h_had_resp_up  = (TH2F*) h_sys->Clone("h_had_resp_up");
    TH2F* h_no_calib  = (TH2F*) h_sys->Clone("h_no_calib");
    TH2F* h_calib_unc_down = (TH2F*) h_sys->Clone("h_calib_unc_down");
    TH2F* h_had_resp_down = (TH2F*) h_sys->Clone("h_had_resp_down");
  
    TRandom3* rnd = new TRandom3();

    for (int ie = 1; ie < h_sys->GetXaxis()->GetNbins() + 1; ie++)
    {
      for (int ip = 1; ip < h_sys->GetYaxis()->GetNbins() + 1; ip++)
      {
        float val = rnd->Gaus(1.0, tbyt_stat);
        h_calib_stat->SetBinContent(ie, ip, val);
        h_calib_unc_up->SetBinContent(ie, ip, 1.0 + calib_unc);
        float v1 = 1.0 + sys_v1 * std::cos((float) ip / phi * 2. * M_PI);
        h_v1->SetBinContent(ie, ip, v1);
        float v2 = 1.0 + sys_v1 * std::cos(2.0 * (float) ip / phi * 2. * M_PI);
        h_v2->SetBinContent(ie, ip, v2);
        float had_resp_up = 1.0 + 2.0 * had_resp_unc * had_frac / (sqrt(12));
        h_had_resp_up->SetBinContent(ie,ip,had_resp_up);
        h_no_calib->SetBinContent(ie,ip,1.0);
        h_calib_unc_down->SetBinContent(ie, ip, 1.0 - calib_unc);
        float had_resp_down = 1.0 - 2.0 * had_resp_unc * had_frac / (sqrt(12));
        h_had_resp_down->SetBinContent(ie, ip, had_resp_down);
      }
    }
        
    histToCaloCDBTree(std::format("cdbFiles/{}_calib_stat_syst_{}_{}.root"        ,calo,begin,end),"calo_sys", embool, h_calib_stat);
    histToCaloCDBTree(std::format("cdbFiles/{}_calib_unc_up_syst_{}_{}.root"       ,calo,begin,end),"calo_sys", embool, h_calib_unc_up);
    histToCaloCDBTree(std::format("cdbFiles/{}_v1Modulation_syst_{}_{}.root",calo,begin,end),"calo_sys", embool, h_v1);
  //  histToCaloCDBTree(std::format("cdbFilesAuAu/{}_v2Modulation_syst_{}_{}.root",calo.c_str(),begin,end),"calo_sys", embool, h_v2);
    histToCaloCDBTree(std::format("cdbFiles/{}_had_resp_up_syst_{}_{}.root"    ,calo,begin,end),"calo_sys", embool, h_had_resp_up);
    histToCaloCDBTree(std::format("cdbFiles/{}_no_calib_syst_{}_{}.root"    ,calo,begin,end),"calo_sys", embool, h_no_calib);
    histToCaloCDBTree(std::format("cdbFiles/{}_calib_unc_down_syst_{}_{}.root"    ,calo,begin,end),"calo_sys", embool, h_calib_unc_down);
    histToCaloCDBTree(std::format("cdbFiles/{}_had_resp_down_syst_{}_{}.root" ,calo,begin,end),"calo_sys", embool, h_had_resp_down);

    TCanvas* c1 = new TCanvas("c1","c1",600,600);
    h_had_resp_up->Draw("COLZ");
    c1->Draw();
    c1->SaveAs(std::format("h_had_resp_up_{}.png", calo).c_str());

    TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);
    h_calib_unc_up->Draw("COLZ");
    c2->Draw();
    c2->SaveAs(std::format("h_calib_unc_up_{}.png", calo).c_str());

    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    h_calib_stat->Draw("COLZ");
    c3->Draw();
    c3->SaveAs(std::format("h_calib_stat_{}.png", calo).c_str());

    TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
    h_v1->Draw("COLZ");
    c4->Draw();
    c4->SaveAs(std::format("h_v1_{}.png",calo).c_str());

    TCanvas* c5 = new TCanvas("c5","c5",600,600);
    h_no_calib->Draw("COLZ");
    c5->Draw();
    c5->SaveAs(std::format("h_no_calib_{}.png",calo).c_str());

    TCanvas* c6 = new TCanvas("c6","c6",600,600);
    h_calib_unc_down->Draw("COLZ");
    c6->Draw();
    c6->SaveAs(std::format("h_calib_unc_down_{}.png",calo).c_str());

    TCanvas* c7 = new TCanvas("c7","c7",600,600);
    h_had_resp_down->Draw("COLZ");
    c7->Draw();
    c7->SaveAs(std::format("h_had_resp_down_{}.png",calo).c_str());

  };
}
