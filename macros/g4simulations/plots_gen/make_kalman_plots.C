// layout 0 = four layers, laddertypes 0-1-1-1
// layout 1 = four layers, laddertypes 1-1-0-1
// layout 2 = three outer layers, laddertypes 1-0-1
// layout 3 = three outer layers, laddertypes 1-1-1
// layout 4 = two outer layers, laddertypes 0-1
// layout 5 = two outer layers, laddertypes 1-1

#include <string>
#include "TFile.h"

// define constants common to all sets
const int n_layouts = 9;				// number of INTT layouts we are testing
const int n_intt_layers[n_layouts] = {4,4,3,3,2,2,1,0,2};	// number of INTT layers in each layout
const string basepath = "/gpfs/mnt/gpfs04/sphenix/user/mitay/data/";	// base path to where data is located
const string layout[n_layouts] = {"zppp","ppzp","0pzp","0ppp","00zp","00pp","000p","0000","00pp_out"};	// convert layout# to layout configuration
	
const int n_datasets = 8;
const string studypath = "mom_scan";//doubles as a naming convention?
const string subpath[n_datasets] = {"pi+_pt0.5GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				    "pi+_pt0.6GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				    "pi+_pt0.7GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				    "pi+_pt0.8GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				    "pi+_pt0.9GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				    "pi+_pt1.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				    "pi+_pt2.0GeV_phi-180-180d_z0cm_eta-1.2-1.2",
				    "pi+_pt3.0GeV_phi-180-180d_z0cm_eta-1.2-1.2"};
const string setname[n_datasets] = {"0.5GeV","0.6GeV","0.7GeV","0.8GeV","0.9GeV","1.0GeV","2.0GeV","3.0GeV"};

// define variables that will make our life easier to be able to access directly
TFile *fin[n_datasets][n_layouts];
bool isZombie[n_datasets][n_layouts];
TTree *ntuple[n_datasets][n_layouts];	// pointers to trees in each of the respective data files

// generally, Ross organized these as returned pointers, the canvas, the stuff needed to generate the histogram, then the names/labels/limits associated with it
void drawAndSaveSet2D(TH2F **histout, TCanvas *c, string drawcommand, string histname, string axislabels, int xbins, float xlow, float xhigh, int ybins, float ylow, float yhigh);
void drawAndFitAndSaveSet1D(TH1F **histout, TF1 **fitout, TCanvas *c, string drawcommand, string histname, string axislabels, int xbins, float xlow, float xhigh);
// void drawFitParamAndSaveSet(TH1F **histout, TCanvas *c, TF1 **fit, int param, string histname, string axislabels, float minrange, float maxrange);
// void drawFitErrorAndSaveSet(TH1F **histout, TCanvas *c, TF1 **fit, int param, string histname, string axislabels, float minrange, float maxrange);
void drawRMSAndSaveSet(TH1F **histout, TCanvas *c, TH1F **histin, string histname, string axislabels, float minrange, float maxrange);

void make_kalman_plots() {
	gSystem->Load("libg4hough.so");

	// files, branches and their associated 'isZombie' (this could be called each time by fin[z][q]->isZombie() as well)
  	const string path = basepath + studypath + "/";

	// open all the files we need;  first index is subpath, second index is layout
	for (int i = 0; i < n_datasets; i++) {
		for (int j = 0; j < n_layouts; j++) {
	    		isZombie[i][j] = false;
	    		fin[i][j] = new TFile((path + subpath[i] + "/G4_sPHENIX_" + layout[j] + ".root_g4kalman_eval.root").c_str(),"READ");
	    		isZombie[i][j] = fin[i][j]->IsZombie();

	    		if (isZombie[i][j]) continue; // skip them if the file is broken
	    		
			fin[i][j]->GetObject("kalman_extrapolation_eval",ntuple[i][j]);
	  	}
	}

	// start drawing things
	
	// create a canvas to draw all the 2D plots
	TCanvas *c0 = new TCanvas("c0","c0",1000,800);
	c0->Divide(4,2);
	gStyle->SetOptStat(111111);
	
	// show the extrapolation Z error versus pt
	TH2F *hZDiffPz[n_datasets][n_layouts];
	drawAndSaveSet2D((TH2F**) hZDiffPz,c0,"(z2te-z2t)*1e4:pz","hZDiffPz",";z mom (GeV);z guess-g4(um)",50,-2,2,50,-5000,5000);

	// compare pt
	TH1F *hPt[n_datasets][n_layouts];
	TF1 *fPt[n_datasets][n_layouts];
	drawAndFitAndSaveSet1D((TH1F**) hPt,(TF1**) fPt, c0,"pt","hPt",";pt (GeV)",50,-5,5);

	TCanvas *c1=new TCanvas("c1","c1",800,800);

	TH1F *hRMSPt[n_layouts];
	drawRMSAndSaveSet((TH1F**)hRMSPt, c1, (TH1F **) hPt, "hRMSPt", ";Gaus. #sigma of pt",0,4);

	gSystem->Exit(0);
	return;
}

void drawAndSaveSet2D (TH2F **histout, TCanvas *c, string drawcommand, string histname, string axislabels, int xbins, float xlow, float xhigh, int ybins, float ylow, float yhigh) {
	//assumes c has enough divisions to cover all of these.
	int nsets=n_datasets;
	int nsubs=n_layouts;
	for (int i = 0; i < nsets; i++) {
		for (int j = 0; j < nsubs; j++) {
			histout[i*nsubs+j] = new TH2F((string(histname) + string(setname[i]) + string(layout[j])).c_str(),(string(layout[j]) + string(axislabels)).c_str(),xbins,xlow,xhigh,ybins,ylow,yhigh);
			c->cd(j+1);

			if (!isZombie[i][j])
				ntuple[i][j]->Draw((drawcommand + ">>" + histname + setname[i] + layout[j]).c_str());

			histout[i*nsubs+j]->Draw("colz");
		}

		c->SaveAs((histname + "_" + studypath + "_" + setname[i] + ".pdf").c_str());
	}

	return;
}

void drawAndFitAndSaveSet1D (TH1F **histout, TF1 **fitout, TCanvas *c, string drawcommand, string histname, string axislabels, int xbins, float xlow, float xhigh) {
  //assumes c has just enough divisions to cover all of these.
  int nsets=n_datasets;
  int nsubs=n_layouts;
  TH1F* ht;
  TF1* ft;
  for (int i = 0; i < nsets; i++){
    for (int j = 0; j < nsubs; j++){
      c->cd(j+1);
      ht=histout[i*nsubs+j]= new TH1F((string(histname) + string(setname[i]) + string(layout[j])).c_str(),(string(layout[j])+string(axislabels)).c_str(),xbins,xlow,xhigh);
      ft=fitout[i*nsubs+j]=new TF1(("fit"+histname + setname[i]+ layout[j]).c_str(),"gaus(0)",xlow,xhigh);

      ft->SetParNames("scale","x0","sigma");
      ft->SetParameters(ht->GetEntries(),ht->GetMean(),ht->GetRMS());
      if (!isZombie[i][j])
	ntuple[i][j]->Draw((drawcommand+">>"+histname + setname[i]+ layout[j]).c_str());

      histout[i*nsubs+j]->Draw();
      ht->Fit(ft);
    }
    c->SaveAs((histname+"_"+studypath+"_"+setname[i]+".pdf").c_str());

  }
  return;
}

/*
void drawFitParamAndSaveSet (TH1F **histout, TCanvas *c, TF1 **fit, int param, string histname, string axislabels, float minrange, float maxrange) {
  TLegend *leg;
  c->cd();
	
  leg = new TLegend(0.20,0.75,0.480,0.9);
  leg->SetNColumns(2);

  for (int i=0;i<n_layouts;i++){
    histout[i]=new TH1F((histname+ layout[i]).c_str(),axislabels.c_str(),n_datasets,-0.5,n_datasets-0.5);
    histout[i]->SetLineColor(i+1);
    histout[i]->SetLineWidth(2);
    for (int j=0;j<n_datasets;j++){
      int bin=histout[i]->Fill(setname[j].c_str(),fit[j*n_layouts+i]->GetParameter(param));
      histout[i]->SetBinError(bin,fit[j*n_layouts+i]->GetParError(param));
    }

    if (i==0){
      histout[i]->GetYaxis()->SetRangeUser(minrange,maxrange);
      histout[i]->Draw();
    }else {
      histout[i]->Draw("SAME");
    }
  }
  for (int i=0;i<n_layouts;i++){
    leg->AddEntry(histout[i],layout[i].c_str(),"l");
  }
  leg->Draw();
	  
  c->SaveAs((histname+studypath+".pdf").c_str());
  return;
}
*/

/*
void drawFitErrorAndSaveSet(TH1F **histout, TCanvas *c, TF1 **fit, int param, string histname, string axislabels, float minrange, float maxrange) {
  TLegend *leg;
  c->cd();
	
  leg = new TLegend(0.20,0.75,0.480,0.9);
  leg->SetNColumns(2);

  for (int i=0;i<n_layouts;i++){
    histout[i]=new TH1F((histname+ layout[i]).c_str(),axislabels.c_str(),n_datasets,-0.5,n_datasets-0.5);
    histout[i]->SetLineColor(i+1);
    histout[i]->SetLineWidth(2);
    for (int j=0;j<n_datasets;j++){
      int bin=histout[i]->Fill(setname[j].c_str(),fit[j*n_layouts+i]->GetParError(param));
      //histout[i]->SetBinError(bin,fit[j*n_layouts+i]->GetParError(param));
    }

    if (i==0){
      histout[i]->GetYaxis()->SetRangeUser(minrange,maxrange);
      histout[i]->Draw();
    }else {
      histout[i]->Draw("SAME");
    }
  }
  for (int i=0;i<n_layouts;i++){
    leg->AddEntry(histout[i],layout[i].c_str(),"l");
  }
  leg->Draw();
	  
  c->SaveAs((histname+studypath+".pdf").c_str());
  return;
}
*/

void drawRMSAndSaveSet(TH1F **histout, TCanvas *c, TH1F **histin, string histname, string axislabels, float minrange, float maxrange)
{
  TLegend *leg;
  c->cd();
	
  leg = new TLegend(0.20,0.15,0.480,0.3);
  leg->SetNColumns(2);

  for (int i=0;i<n_layouts;i++){
    histout[i]=new TH1F((histname + layout[i]).c_str(),axislabels.c_str(),n_datasets,-0.5,n_datasets-0.5);
    histout[i]->SetLineColor(i+1);
    histout[i]->SetLineWidth(2);
    for (int j=0;j<n_datasets;j++){
      int bin=histout[i]->Fill(setname[j].c_str(),histin[j*n_layouts+i]->GetRMS());
      histout[i]->SetBinError(bin,histin[j*n_layouts+i]->GetRMSError());
    }

    if (i==0){
      histout[i]->GetYaxis()->SetRangeUser(minrange,maxrange);
      histout[i]->Draw();
    }else {
      histout[i]->Draw("SAME");
    }
  }
  for (int i=0;i<n_layouts;i++){
    leg->AddEntry(histout[i],layout[i].c_str(),"l");
  }
  leg->Draw();
	  
  c->SaveAs((histname+studypath+".pdf").c_str());
  return;
}
