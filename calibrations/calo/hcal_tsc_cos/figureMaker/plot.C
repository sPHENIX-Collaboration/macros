#include "sPhenixStyle.C"

void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize=0.04) {

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}


TH2F* DivideColumnsByAverage(TH2F* hist);


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


TFile* f_in = new TFile("../fitout_hcalin.root");
TFile* f_out = new TFile("../fitout_hcalout.root");

TH2F* tsc_in = (TH2F*) f_in->Get("corrPat");
TH2F* tsc_out = (TH2F*) f_out->Get("corrPat");


TCanvas* c1 = new TCanvas("c1","c1",600,600);
tsc_in->Draw("COLZ");
tsc_in->SetXTitle("ieta");
tsc_in->SetYTitle("iphi");

gPad->SetRightMargin(0.15);

myText(0.1,0.98,1,Form("%d-%d",first_run,last_run));
myText(0.7,0.98,1,"#it{#bf{sPHENIX}} Internal");
myText(0.5,0.98,1,"HCal IN");

c1->SaveAs("../plots/tsc_in.pdf");


TCanvas* c2 = new TCanvas("c2","c2",600,600);
tsc_out->Draw("COLZ");
tsc_out->SetXTitle("ieta");
tsc_out->SetYTitle("iphi");

gPad->SetRightMargin(0.15);

myText(0.1,0.98,1,Form("%d-%d",first_run,last_run));
myText(0.7,0.98,1,"#it{#bf{sPHENIX}} Internal");
myText(0.5,0.98,1,"HCal OUT");

c2->SaveAs("../plots/tsc_out.pdf");


////////////////////////////////////////
// plot beam data output from calo valid
int runCV = 23727;
TFile* fin = new TFile(Form("../condor/combine_out/out_%d.root",runCV));
TH2F* h_ohcal_hit = (TH2F*) fin->Get("h_ohcal_etaphi_wQA");
TH2F* h_ihcal_hit = (TH2F*) fin->Get("h_ihcal_etaphi_wQA");
//TH2F* h_ihcal_hit = (TH2F*) fin->Get("h_ihcal_etaphi_wQA");

TH2F*  h_ohcal_hit_avg = DivideColumnsByAverage(h_ohcal_hit);
TH2F*  h_ihcal_hit_avg = DivideColumnsByAverage(h_ihcal_hit);

TCanvas* c3 = new TCanvas("c3","c3",600,600);
h_ohcal_hit_avg->Draw("COLZ");
h_ohcal_hit_avg->SetXTitle("ohcal ieta");
h_ohcal_hit_avg->SetYTitle("ohcal iphi");
h_ohcal_hit_avg->GetZaxis()->SetRangeUser(0.5,1.5);

gPad->SetRightMargin(0.15);

myText(0.1,0.98,1,Form("%d",runCV));
myText(0.7,0.98,1,"#it{#bf{sPHENIX}} Internal");
myText(0.5,0.98,1,"HCal OUT");

c3->SaveAs("../plots/hitsNorm_ohcal.pdf");


TCanvas* c4 = new TCanvas("c4","c4",600,600);
h_ihcal_hit_avg->Draw("COLZ");
h_ihcal_hit_avg->SetXTitle("ihcal ieta");
h_ihcal_hit_avg->SetYTitle("ihcal iphi");
h_ihcal_hit_avg->GetZaxis()->SetRangeUser(0.8,1.2);

gPad->SetRightMargin(0.15);

myText(0.1,0.98,1,Form("%d",runCV));
myText(0.7,0.98,1,"#it{#bf{sPHENIX}} Internal");
myText(0.5,0.98,1,"HCal IN");

c4->SaveAs("../plots/hitsNorm_ihcal.pdf");


}



TH2F* DivideColumnsByAverage(TH2F* hist) {
    int numBinsX = hist->GetNbinsX();
    int numBinsY = hist->GetNbinsY();

    TH2F* dividedHistogram = new TH2F("dividedHistogram", "Histogram with Columns Divided by Average",
                                      numBinsX, hist->GetXaxis()->GetXmin(), hist->GetXaxis()->GetXmax(),
                                      numBinsY, hist->GetYaxis()->GetXmin(), hist->GetYaxis()->GetXmax());

    for (int binX = 1; binX <= numBinsX; binX++) {
        double sum = 0.0;
        for (int binY = 1; binY <= numBinsY; binY++) {
            sum += hist->GetBinContent(binX, binY);
        }
        double average = (numBinsY > 0) ? sum / numBinsY : 0.0;
        for (int binY = 1; binY <= numBinsY; binY++) {
            double content = hist->GetBinContent(binX, binY);
            double dividedContent = (average != 0.0) ? content / average : 0.0;
            dividedHistogram->SetBinContent(binX, binY, dividedContent);
        }
    }
    return dividedHistogram;
}
