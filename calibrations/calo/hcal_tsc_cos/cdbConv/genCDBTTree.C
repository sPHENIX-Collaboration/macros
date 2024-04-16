#include"cdbHistConv.C"


void checkTsc(TH2F* h);

void genCDBTTree(){
  
  // get 2024 cosmics calibration
  TFile* fin = new TFile("HCal_newcalib_simprecalib_template.root");
  TH2F* h_ohcal = (TH2F*) fin->Get("h_2Dohcal_calib");
  TH2F* h_ihcal = (TH2F*) fin->Get("h_2Dihcal_calib");
  
  // get/apply temperture correction for 2024 -> 2023 calibration
  TFile* ftemp = new TFile("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_cosmics/tempAna/y23Ana/temp_corr_24to23.root");
  TH2F* h_temp_corr_ohcal = (TH2F*) fin->Get("h_temp_corr_tbt_ohcal");
  TH2F* h_temp_corr_ihcal = (TH2F*) fin->Get("h_temp_corr_tbt_ihcal");
   h_ohcal->Multiply(h_temp_corr_ohcal);
   h_ihcal->Multiply(h_temp_corr_ihcal);
  
  // get/apply tsc fine tunning for calibration
  TFile* ftsc_ohcal = new TFile("../fitout_hcalout.root");
  TFile* ftsc_ihcal = new TFile("../fitout_hcalin.root");
  TH2F* h_tsc_corr_ohcal = (TH2F*) ftsc_ohcal->Get("corrPat");
  TH2F* h_tsc_corr_ihcal = (TH2F*) ftsc_ihcal->Get("corrPat");
  h_ohcal->Divide(h_tsc_corr_ohcal);
  h_ihcal->Divide(h_tsc_corr_ihcal);
  
  // generate cdb ttrees for input into calotowercalib
  string outputfile = "ohcal_cdb_calib.root";
  string fieldName = "ohcal_abscalib_mip";
  histToCaloCDBTree( outputfile,  fieldName, 1, h_ohcal);
  
  outputfile = "ihcal_cdb_calib.root";
  fieldName = "ihcal_abscalib_mip";
  histToCaloCDBTree( outputfile,  fieldName, 1, h_ihcal);
  

}


void checkTsc(TH2F* h){

for (int ie=0; ie<24; ie++)
  for (int ip=0; ip<64; ip++)
    if (h->GetBinContent(ie+1,ip+1) < 0.5 || h->GetBinContent(ie+1,ip+1) > 1.3) 
      h->SetBinContent(ie+1,ip+1,1);


}
