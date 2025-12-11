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
    int end = 53862;

    float tbyt_stat;
    float shift;
    float sys_v1;
    float had_frac;
    int eta;
    int phi;
    bool embool;
    float had_resp_unc = 0.117;  // value from test beam

    if (calo == "CEMC")
    {
      tbyt_stat = 0.03;  // tower by tower stat uncertainty
      shift = 0.026;     // global scale shift
      sys_v1 = 0.02;
      //      sys_v2 = 0.02;
      had_frac = 0.61;
      embool = false;
      eta = 96;
      phi = 256;
    }
    if (calo == "HCALOUT")
    {
      tbyt_stat = 0.01;  // tower by tower stat uncertainty
      shift = 0.01;      // global scale shift
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
      shift = 0.1;       // global scale shift
      sys_v1 = 0.02;
      //      sys_v2 = 0.05;
      had_frac = 0.961;
      embool = true;
      eta = 24;
      phi = 64;
    }

    TH2F* h_sys = new TH2F("h_sys", "", eta, 0, eta, phi, 0, phi);
    TH2F* h_stat = (TH2F*) h_sys->Clone("h_stat");
    TH2F* h_shift = (TH2F*) h_sys->Clone("h_shift");
    TH2F* h_v1 = (TH2F*) h_sys->Clone("h_v1");
    TH2F* h_v2 = (TH2F*) h_sys->Clone("h_v2");
    TH2F* h_had_resp = (TH2F*) h_sys->Clone("h_had_resp");

    TRandom3* rnd = new TRandom3();

    for (int ie = 1; ie < h_sys->GetXaxis()->GetNbins() + 1; ie++)
    {
      for (int ip = 1; ip < h_sys->GetYaxis()->GetNbins() + 1; ip++)
      {
        float val = rnd->Gaus(1.0, tbyt_stat);
        h_stat->SetBinContent(ie, ip, val);
        h_shift->SetBinContent(ie, ip, 1.0 + shift);
        float v1 = 1.0 + sys_v1 * std::cos((float) ip / phi * 2. * M_PI);
        h_v1->SetBinContent(ie, ip, v1);
        float v2 = 1.0 + sys_v1 * std::cos(2.0 * (float) ip / phi * 2. * M_PI);
        h_v2->SetBinContent(ie, ip, v2);
        float had_resp = 1.0 + 2.0 * had_resp_unc * had_frac / (sqrt(12));
        std::cout << had_resp << " ";
        h_had_resp->SetBinContent(ie, ip, had_resp);
      }
    }
    std::cout << std::endl;

    histToCaloCDBTree(std::format("cdbFiles/{}_stat_syst_{}_{}.root", calo, begin, end), "calo_sys", embool, h_stat);
    histToCaloCDBTree(std::format("cdbFiles/{}_shift_syst_{}_{}.root", calo, begin, end), "calo_sys", embool, h_shift);
    histToCaloCDBTree(std::format("cdbFiles/{}_v1Modulation_syst_{}_{}.root", calo, begin, end), "calo_sys", embool, h_v1);
    //  histToCaloCDBTree(std::format("cdbFilesAuAu/{}_v2Modulation_syst_{}_{}.root",calo,begin,end),"calo_sys", embool, h_v2);
    histToCaloCDBTree(std::format("cdbFiles/{}_had_resp_syst_{}_{}.root", calo, begin, end), "calo_sys", embool, h_had_resp);

    TCanvas* c1 = new TCanvas("c1", "c1", 600, 600);
    h_had_resp->Draw("COLZ");
    c1->Draw();
    c1->SaveAs(std::format("h_had_resp_%s.png", calo).c_str());

    TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);
    h_shift->Draw("COLZ");
    c2->Draw();
    c2->SaveAs(std::format("h_shift_%s.png", calo).c_str());

    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    h_stat->Draw("COLZ");
    c3->Draw();
    c3->SaveAs(std::format("h_stat_%s.png", calo).c_str());

    TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
    h_v1->Draw("COLZ");
    c4->Draw();
    c4->SaveAs(std::format("h_v1_{}.png", calo).c_str());
  };
}
