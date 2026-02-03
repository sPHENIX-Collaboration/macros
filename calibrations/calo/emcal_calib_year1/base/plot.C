#include "sPhenixStyle.C"

void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize=0.04) {

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}




void plot(){

SetsPhenixStyle();

std::string file_path = "runList.txt";

std::ifstream file(file_path);

std::vector<int> run_numbers;
std::string line;
while (std::getline(file, line)) {
    std::istringstream iss(line);
    int run_number;
    if (iss >> run_number) {
        run_numbers.push_back(run_number);
    } else {
        std::cerr << "Warning: Invalid integer on line: " << line << std::endl;
    }
}

file.close();

if (run_numbers.empty()) {
    std::cerr << "Error: No valid run numbers found in the file." << std::endl;
    return 1;
}

int first_run = run_numbers.front();
int last_run = run_numbers.back();

string func_str = "[0]/[2]/2.5*exp(-0.5*((x-[1])/[2])^2) + [3] + [4]*x + [5]*x^2 + [6]*x^3";

TFile* fintsc = new TFile("tsc_fitout.root");
TFile* finpi0 = new TFile("fitout_iter21.root");

TH2F* tsc_corr = (TH2F*) fintsc->Get("corrPat");

TCanvas* c1 = new TCanvas("c1","c1",600,600);
tsc_corr->Draw("COLZ");
tsc_corr->SetXTitle("ieta");
tsc_corr->SetYTitle("iphi");

gPad->SetRightMargin(0.15);

myText(0.2,0.98,1,Form("%d-%d",first_run,last_run));

c1->SaveAs("plots/tsc_coef.pdf");



TH1F* h_peak_eta = (TH1F*) finpi0->Get("h_peak_eta");
TH1F* h_sigma_eta = (TH1F*) finpi0->Get("h_sigma_eta");
TH1F* h_p0_eta = (TH1F*) finpi0->Get("h_p0_eta");
TH1F* h_p3_eta = (TH1F*) finpi0->Get("h_p3_eta");
TH1F* h_p4_eta = (TH1F*) finpi0->Get("h_p4_eta");
TH1F* h_p5_eta = (TH1F*) finpi0->Get("h_p5_eta");
TH1F* h_p6_eta = (TH1F*) finpi0->Get("h_p6_eta");

TCanvas* c2 = new TCanvas("c2","c2",600,600);
h_peak_eta->Draw("ex0");
h_peak_eta->SetXTitle("ieta");
h_peak_eta->SetYTitle("#it{#pi}^{0} mass");
myText(0.2,0.98,1,Form("%d-%d",first_run,last_run));

c2->SaveAs("plots/mass_eta.pdf");


TCanvas* c3 = new TCanvas("c3","c3",600,600);
h_p0_eta->Draw("ex0");
h_p0_eta->SetXTitle("ieta");
h_p0_eta->SetYTitle("gauss scale fit parameter");

c3->SaveAs("plots/p0_eta.pdf");


TCanvas* c4 = new TCanvas("c4","c4",600,600);
h_p3_eta->Draw("ex0");
h_p3_eta->SetXTitle("ieta");
h_p3_eta->SetYTitle("0th order pol fit parameter");
myText(0.2,0.98,1,Form("%d-%d",first_run,last_run));

c4->SaveAs("plots/p3_eta.pdf");


TCanvas* c5 = new TCanvas("c5","c5",600,600);
h_p4_eta->Draw("ex0");
h_p4_eta->SetXTitle("ieta");
h_p4_eta->SetYTitle("1st order pol fit parameter");

c5->SaveAs("plots/p4_eta.pdf");


TCanvas* c6 = new TCanvas("c6","c6",600,600);
h_p5_eta->Draw("ex0");
h_p5_eta->SetXTitle("ieta");
h_p5_eta->SetYTitle("2nd order pol fit parameter");

c6->SaveAs("plots/p5_eta.pdf");


TCanvas* c7 = new TCanvas("c7","c7",600,600);
h_p6_eta->Draw("ex0");
h_p6_eta->SetXTitle("ieta");
h_p6_eta->SetYTitle("3rd order pol fit parameter");

c7->SaveAs("plots/p6_eta.pdf");


TCanvas* c8 = new TCanvas("c8","c8",600,600);
h_sigma_eta->Draw("ex0");
h_sigma_eta->SetXTitle("ieta");
h_sigma_eta->SetYTitle("gauss sigma fit parameter");

c8->SaveAs("plots/sigma_eta.pdf");


}
