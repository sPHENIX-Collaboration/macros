#include "BlairUtils.C"
#include "sPhenixStyle.C"

std::vector<int> runNumbers;    // Stores the run numbers
std::vector<double> timestamps;  // Stores the timestamps
std::vector<double> values;     // Stores the corresponding values
std::vector<int> hours;     // Stores the corresponding values

std::vector<int> runNumbers_cosmic;    // Stores the run numbers
std::vector<double> timestamps_cosmic;  // Stores the timestamps

void getInfo();
void getCosmicInfo();
float corrDouble(float hits_per_event,double rate);

void plot_hits() {


   SetsPhenixStyle();

  getInfo();
  getCosmicInfo();


  TFile* fin = new TFile("output/anaOut.root");
  TH1F* h_events_run = (TH1F*) fin->Get("h_events_run");
  TH1F* h_hits= (TH1F*) fin->Get("h_hits");
  TH1F* h_runnumber = (TH1F*) fin->Get("runNumberHist");
  TH1F* h_temp_run = (TH1F*) fin->Get("h_temp_run");

  TH1F * h_hits_per_event = (TH1F*) h_hits->Clone("h_hits_per_event");
  h_hits_per_event->Sumw2();
  h_events_run->Sumw2();
  h_hits_per_event->Divide(h_events_run);

   int n_runs = h_runnumber->GetNbinsX();

  //TH1F* h_hits_time = new TH1F("h_hits_time","",n_runs
  TGraphErrors *gr_hits_time = new TGraphErrors();
  TGraphErrors *gr_hits_run  = new TGraphErrors();
  TGraphErrors *gr_rate_time = new TGraphErrors();
  TGraphErrors *gr_hits_time_corr = new TGraphErrors();
  TProfile* pr_temp_hour = new TProfile("pr_temp_hour","",24,-0.5,23.5);
  TGraphErrors* gr_temp_time = new TGraphErrors();
  TH1F* h_hits_corr_dis = new TH1F("h_hits_corr_dis","",100,0.002,0.003);

   float mean_corr_hits = 0;
   int   mean_corr_hitsC = 0;

  for(int it=0; it<n_runs; it++){
    int runnumber = h_runnumber->GetBinContent(1+it);
    for (int ir=0; ir<runNumbers.size(); ir++){
      if(runnumber == runNumbers[ir]){
        float hits = h_hits_per_event->GetBinContent(it+1);
        float hits_err = h_hits_per_event->GetBinError(it+1);
        gr_hits_time->SetPoint(it,timestamps[ir],hits);
        gr_hits_time->SetPointError(it,0,hits_err); 

        gr_hits_run->SetPoint(it,runnumber,hits);
        gr_hits_run->SetPointError(it,0,hits_err); 

        gr_rate_time->SetPoint(it,timestamps[ir],values[ir]);
        cout << "timestamps[ir] " << timestamps[ir] << " values[ir]] " << values[ir] << endl;

        pr_temp_hour->Fill(hours[ir],h_temp_run->GetBinContent(1+it));
        gr_temp_time->SetPoint(it,timestamps[ir],h_temp_run->GetBinContent(1+it));
        
        if (values[ir] != 0){
           float corr_hits = corrDouble(hits,values[ir]); 
           gr_hits_time_corr->SetPoint(it,timestamps[ir],corr_hits);
           gr_hits_time_corr->SetPointError(it,0,hits_err);
           mean_corr_hits += corr_hits; mean_corr_hitsC++; 
           h_hits_corr_dis->Fill(corr_hits);
        }
      }
      
    }
  }


   mean_corr_hits /= mean_corr_hitsC;
   TGraph* gr_mean_corr_hits = new TGraph();
   gr_mean_corr_hits->SetPoint(0,0,mean_corr_hits);
   gr_mean_corr_hits->SetPoint(1,1e10,mean_corr_hits);
   gr_mean_corr_hits->SetLineColor(kBlue);
   

   TGraph* gr_cosmic_times = new TGraph();
   

  for(int ir=0; ir<runNumbers_cosmic.size(); ir++){
    cout << timestamps_cosmic[ir] << endl;
    gr_cosmic_times->SetPoint(ir*2,timestamps_cosmic[ir],10000*(ir%2));
    gr_cosmic_times->SetPoint(ir*2+1,timestamps_cosmic[ir],10000*((ir+1)%2));
  }

   TCanvas *c1 = new TCanvas("c1","c1",600,600);
   //h_hits_per_event->Draw("ex0");
   gr_hits_run->Draw("ap");

    TCanvas* c2 = new TCanvas("c2","c2",2000,600);
    TH1F* h_frame = new TH1F("h_frame","",100,1.7183e+09,1.729e+09);
    h_frame->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
    h_frame->GetXaxis()->SetTimeDisplay(1);
    h_frame->GetXaxis()->SetTitle("run time");
    h_frame->GetYaxis()->SetTitle("mean hits / event / run");
    h_frame->GetXaxis()->SetRangeUser(1.7183e+09,1.729e+09);
    //h_frame->GetXaxis()->SetRangeUser(1.72e+09,1.721e+09);
    h_frame->GetYaxis()->SetRangeUser(0,0.005);
    h_frame->GetYaxis()->SetTitleOffset(0.7);
    h_frame->Draw("axis");
    gPad->SetLeftMargin(0.08);

    gr_hits_time->Draw("p");
    gr_hits_time->SetMarkerStyle(20);
    gr_hits_time->SetMarkerSize(1.5);
    gr_hits_time->GetXaxis()->SetTimeDisplay(1);
    gr_hits_time->GetYaxis()->SetRangeUser(0,0.005);
    gr_hits_time->GetXaxis()->SetNdivisions(-503);
    gr_hits_time->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
    gr_hits_time->GetXaxis()->SetTitle("run time");
    gr_hits_time->GetYaxis()->SetTitle("mean hits / event / run");
    //gr_hits_time->GetXaxis()->SetRangeUser(1.7185e+09,1.729e+09);
    gr_hits_time->GetXaxis()->SetRangeUser(1.72e+09,1.721e+09);

    gr_hits_time_corr->Draw("p");
    gr_hits_time_corr->SetMarkerStyle(33);;
    gr_hits_time_corr->SetMarkerColor(kBlue);;
    gr_hits_time_corr->SetLineColor(kBlue);;
/*
    gr_cosmic_times->Draw("l");
    gr_cosmic_times->SetLineStyle(7);
    gr_cosmic_times->SetLineWidth(2);
    gr_cosmic_times->SetLineColor(kRed);
    gr_cosmic_times->GetXaxis()->SetTimeDisplay(1);
    gr_cosmic_times->GetXaxis()->SetNdivisions(-503);
    gr_cosmic_times->GetXaxis()->SetTimeFormat("%m-%d %H:%M");

   gr_mean_corr_hits->Draw("l");
*/
   myText         (0.11,0.90-0.5,1,"#bf{#it{sPHENIX}} Internal",0.05);
   myMarkerLineText(0.14,0.85-0.5,1.5,kBlack ,20,kBlack,1,"data", 0.05, true);
   myMarkerLineText(0.14,0.80-0.5,1.5,kBlue ,33,kBlue,1,"pileup corrected", 0.05, true);
   //myMarkerLineText(0.14,0.75-0.5,0  ,kRed,1,kRed,7,"cosmic calib runs", 0.05, true);
     myText         (0.5,0.90-0.5,1,"Not final calib - no temp correction or cosmics",0.05);

   c2->SaveAs("figures/hit_rate_vs_time.pdf");


   TCanvas* c11 = new TCanvas("c1","c1",600,600);
   h_hits_corr_dis->Draw();
   h_hits_corr_dis->Fit("gaus");
   h_hits_corr_dis->SetXTitle("Hits/run");
   h_hits_corr_dis->SetYTitle("runs");
   h_hits_corr_dis->GetXaxis()->SetNdivisions(505);
 
   TF1* fitFunc = h_hits_corr_dis->GetFunction("gaus");
   double fmean = fitFunc->GetParameter(1);      // Mean (mu)
   double fsigma = fitFunc->GetParameter(2);     // Standard deviation (sigma)
   
   myText(0.20, 0.9, 1, "#bf{#it{sPHENIX}} Internal", 0.05);
   myText(0.20, 0.85, 1, "OHCal", 0.05);
   myText(0.20, 0.8, 1, Form("Fit #mu/#sigma = %.3f", fsigma/fmean), 0.05);
   myText(0.20, 0.75, 1, Form("RMS/Mean = %.3f", h_hits_corr_dis->GetRMS()/h_hits_corr_dis->GetMean()), 0.05);

   c11->SaveAs("figures/hit_rate_allRuns.pdf");

  ///////////////////////
  // temp plots
  /////////////////////////

  TProfile2D* pr2d_temp     = (TProfile2D*) fin->Get("pr2d_temp");
  //TH2F* pr2d_temp_var = (TH2F*) pr2d_temp->Clone("pr2d_temp_var");
  TH2F* pr2d_temp_var = new TH2F("pr2d_temp_var","",24,0,24,64,0,64); 
  pr2d_temp_var->Reset(); 
  for(int ix=0; ix<24; ix++){
    for(int iy=0; iy<64; iy++){
      float var = pr2d_temp->GetBinError(ix+1,iy+1);
      cout << var << endl;
      pr2d_temp_var->SetBinContent(ix+1,iy+1,var);
      pr2d_temp_var->SetBinError(ix+1,iy+1,0);
    }
  }

   TCanvas *c3 = new TCanvas("c3","c3",600,600);
   //pr_temp_hour->Draw();
   pr2d_temp->Draw("COLZ");
   pr2d_temp->SetXTitle("OHCal #it{#eta}^{i}");
   pr2d_temp->SetYTitle("OHCal #it{#phi}^{i}");
   gPad->SetRightMargin(0.15);

  TCanvas *c4 = new TCanvas("c4","c4",2000,600);
  TH1F* h_frame2 = new TH1F("h_frame2","",100,1.7183e+09,1.729e+09);
  h_frame2->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
  h_frame2->GetXaxis()->SetTimeDisplay(1);
  h_frame2->GetXaxis()->SetTitle("run time");
  h_frame2->GetYaxis()->SetTitle("mean OHCal temperature / run");
  h_frame2->GetXaxis()->SetRangeUser(1.7183e+09,1.729e+09);
  h_frame2->GetYaxis()->SetRangeUser(0,25);
  h_frame2->GetYaxis()->SetTitleOffset(0.7);
  h_frame2->Draw("axis");
  gPad->SetLeftMargin(0.08);

  gr_temp_time->Draw("p");
  gr_temp_time->SetMarkerStyle(20);
  gr_temp_time->SetMarkerSize(1.0);
  gr_temp_time->GetXaxis()->SetTimeDisplay(1);
  gr_temp_time->GetYaxis()->SetRangeUser(0,25);
  gr_temp_time->GetXaxis()->SetNdivisions(-503);
  gr_temp_time->GetXaxis()->SetTimeFormat("%m-%d %H:%M");
  gr_temp_time->GetXaxis()->SetTitle("run time");
  gr_temp_time->GetYaxis()->SetTitle("ohcal tower avg temp [C]");
  gr_temp_time->GetXaxis()->SetRangeUser(1.7185e+09,1.729e+09);

  myText         (0.11,0.90-0.5,1,"#bf{#it{sPHENIX}} Internal",0.05);

  c4->SaveAs("figures/OHCal_mean_temp.pdf");

}


float corrDouble(float hits_per_event,double rate){
  //double newnumber = TMath::Abs(lambda * (double)(endtime-starttime)*(111*78e3));
  //double prob = newnumber / 78e3/111; 
  double prob1orMore = rate / 78.e3/111.;
  double lambda = -1.0*TMath::Log(1-prob1orMore);
  // labda = prob of a single collision
  // p_double = prob of an additional collsion which = lambda
  // x = true probability of a hit per a single mb event 
  // obs_x is the observed probability (prob per bunch crossing) 
  // (x*events +  x*event*p_double ) / events = ob_x
  // x + x*p_double = ob_x  
  // x = ob_x / (1+p_double)
  return hits_per_event / (1 + lambda);
}




void getInfo(){
    // Define vectors to store the data

    // Open the CSV file
    std::ifstream file("run_timestamps_with_rate.csv");  // Replace with your actual CSV file path
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string runNumberStr, timestampStr, valueStr;

        // Read the run number, timestamp, and value from each line
        std::getline(ss, runNumberStr, ',');
        std::getline(ss, timestampStr, ',');
        std::getline(ss, valueStr, ',');

        // Convert the run number to integer
        int runNumber = std::stoi(runNumberStr);


    
        int year, month, day, hour, minute, second;
        sscanf(timestampStr.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
        TDatime t(year, month, day, hour, minute, second);
        double timeStamp = t.Convert();

        // Convert the value to double
        double value = valueStr.empty() ? 0.0 : std::stod(valueStr);

       // cout << runNumber << "  "   << timeStamp << "  "  << value << endl;
        // Store the values in the vectors
        runNumbers.push_back(runNumber);
        timestamps.push_back(timeStamp);
        values.push_back(value);
        hours.push_back(t.GetHour());
    }

    file.close();


}




void getCosmicInfo(){
    std::ifstream file("cosmic_times.csv");  // Replace with your actual CSV file path
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string runNumberStr, timestampStr, valueStr;

        std::getline(ss, runNumberStr, ',');
        std::getline(ss, timestampStr, ',');
        std::getline(ss, valueStr, ',');

        int runNumber = std::stoi(runNumberStr);

        int year, month, day, hour, minute, second;
        sscanf(timestampStr.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);
        TDatime t(year, month, day, hour, minute, second);
        double timeStamp = t.Convert();
    
         cout << runNumber << "  "   << timeStamp << endl;

        runNumbers_cosmic.push_back(runNumber);
        timestamps_cosmic.push_back(timeStamp);
    }

    file.close();


}









