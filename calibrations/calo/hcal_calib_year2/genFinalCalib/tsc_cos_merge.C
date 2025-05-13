#include"cdbHistConv.C"


void rescaleTSC(TH2F* h_tsc,TH2F* h_cos);
void checkTsc(TH2F* h);
void checkCos(TH2F* h);

void tsc_cos_merge(string tsc_file, string cos_cdb_file,string output_name, int isIHcal){
  
  // get 2024 cosmics calibration
  string fieldName_in = "ohcal_cosmic_calibration";
  if (isIHcal ==1) fieldName_in = "ihcal_cosmic_calibration";
  TH2F* h_ohcal = CaloCDBTreeToHist(cos_cdb_file,fieldName_in,1);
  
  // get/apply tsc fine tunning for calibration
  TFile* ftsc_ohcal = new TFile(tsc_file.c_str());
  TH2F* h_tsc_corr_ohcal = (TH2F*) ftsc_ohcal->Get("corrPat");

  checkTsc(h_tsc_corr_ohcal);
  checkCos(h_ohcal);
  rescaleTSC(h_tsc_corr_ohcal,h_ohcal); 
  h_ohcal->Divide(h_tsc_corr_ohcal);
  
  // generate cdb ttrees for input into calotowercalib
  string fieldName = "HCALOUT_calib_ADC_to_ETower";
  if (isIHcal ==1) fieldName = "HCALIN_calib_ADC_to_ETower";
  histToCaloCDBTree( output_name,  fieldName, 1, h_ohcal);

}


void checkTsc(TH2F* h){

for (int ie=0; ie<24; ie++)
  for (int ip=0; ip<64; ip++)
    if (h->GetBinContent(ie+1,ip+1) < 0.5 || h->GetBinContent(ie+1,ip+1) > 1.3) 
      h->SetBinContent(ie+1,ip+1,1);
}

void checkCos(TH2F* h){
  float avg=0;
  int cc=0;
  for (int ie=0; ie<24; ie++){
    for (int ip=0; ip<64; ip++){
      avg += h->GetBinContent(ie+1,ip+1);
      cc++;
    }
  }
  avg/=cc; 
  cout << "avg=" << avg << endl;
  
  for (int ie=0; ie<24; ie++)
    for (int ip=0; ip<64; ip++)
      if (h->GetBinContent(ie+1,ip+1) < 0.3*avg || h->GetBinContent(ie+1,ip+1) > 2.3*avg) 
        h->SetBinContent(ie+1,ip+1,avg);

}


void rescaleTSC(TH2F* h_tsc,TH2F* h_cos){

  //  chimeny and support regions which are invalid for TSC
  // set TSC=1 add 1 bin of padding. 
  int chimEtaLow  = 0;
  int chimEtaHigh = 4;
  int chimPhiLow = 12;
  int chimPhiHigh= 20;
  int supportEtaLow  = 21;
  int supportEtaHigh = 23;
  int supportPhiLow  = 12;
  int supportPhiHigh = 20;


  //  phi bins cosmic without bias
  int bin1l = 26;
  int bin1h = 37;
  int bin2h = 58;
  int bin2l = 5;
  float etaAvg_tsc[24] = {0};
  float etaAvg_cos[24] = {0};

  for (int ie=0; ie<24; ie++){
    int c = 0;
    for (int ip=0; ip<64; ip++){
       if ( ip <= bin2l || (ip >= bin1l && ip <= bin1h) || ip >= bin2h){
           etaAvg_tsc[ie] += h_tsc->GetBinContent(ie+1,ip+1);
           etaAvg_cos[ie] += h_cos->GetBinContent(ie+1,ip+1);
            c++;
       }
    }
    etaAvg_tsc[ie] /= c; 
    cout << ie << "  " <<  etaAvg_tsc[ie] << endl;
    etaAvg_cos[ie] /= c; 
    for (int ip=0; ip<64; ip++){
       float val = h_tsc->GetBinContent(ie+1,ip+1);
       val /= etaAvg_tsc[ie];
       h_tsc->SetBinContent(ie+1,ip+1,val);
    }
    for (int ip=0; ip<64; ip++){
      bool isChim    = ( ie >= chimEtaLow && ie <= chimEtaHigh && ip >= chimPhiLow && ip <= chimPhiHigh);
      bool isSupport = ( ie >= supportEtaLow && ie <= supportEtaHigh && ip >= supportPhiLow && ip <= supportPhiHigh);
      if(isChim || isSupport)  h_tsc->SetBinContent(ie+1,ip+1,1);
    }
  }
  


}


void genCdbCorr_HH(float corr, string cos_cdb_file,string output_name, int isIHcal,vector<vector<float>> halfhighs){
  
  // get 2024 cosmics calibration
  string fieldName_in = "ohcal_abscalib_mip";
  if (isIHcal ==1) fieldName_in = "ihcal_cosmic_calibration";
  TH2F* h_ohcal = CaloCDBTreeToHist(cos_cdb_file,fieldName_in,1);

  if (!h_ohcal){
     cout << "cosmic hist not found exiting" << endl;
     return;
  }
  for(int ic=0; ic<halfhighs.size(); ic++){
    float val = h_ohcal->GetBinContent(halfhighs[ic][0]+1,halfhighs[ic][1]+1);  
    float new_val = val*halfhighs[ic][2];
    cout << "correct half hieght " << halfhighs[ic][0] << "," << halfhighs[ic][1] << "  "  << halfhighs[ic][2]  << endl;
    h_ohcal->SetBinContent(halfhighs[ic][0]+1,halfhighs[ic][1]+1,new_val);
  }

  h_ohcal->Scale(corr);
  
  // generate cdb ttrees for input into calotowercalib
  string fieldName = "HCALOUT_calib_ADC_to_ETower";
  if (isIHcal ==1) fieldName = "HCALIN_calib_ADC_to_ETower";
  histToCaloCDBTree( output_name,  fieldName, 1, h_ohcal);
  delete h_ohcal;
}




void genCdbCorr(float corr, string cos_cdb_file,string output_name, int isIHcal){
  
  cout << "heeeelllloooooo" << endl;
  // get 2024 cosmics calibration
  string fieldName_in = "ohcal_abscalib_mip";
  if (isIHcal ==1) fieldName_in = "ihcal_cosmic_calibration";
  TH2F* h_ohcal = CaloCDBTreeToHist(cos_cdb_file,fieldName_in,1);

  if (!h_ohcal){
     cout << "cosmic hist not found exiting" << endl;
     return;
  }
    

  h_ohcal->Scale(corr);
  
  // generate cdb ttrees for input into calotowercalib
  string fieldName = "ohcal_abscalib_mip";
  if (isIHcal ==1) fieldName = "ihcal_abscalib_mip";
  histToCaloCDBTree( output_name,  fieldName, 1, h_ohcal);
}



