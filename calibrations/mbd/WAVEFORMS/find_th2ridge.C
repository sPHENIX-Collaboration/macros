#ifndef MACRO_FIND_TH2RIDGE_C
#define MACRO_FIND_TH2RIDGE_C

#include <TF1.h>
#include <TFitResult.h>
#include <TGraphErrors.h>
#include <TH2.h>
#include <TPad.h>

// find the ridge of the TH2
TGraphErrors *find_th2ridge(const TH2* h2)
{
  int nbinsx = h2->GetNbinsX();
  int nbinsy = h2->GetNbinsY();
//  double min_xrange = h2->GetXaxis()->GetBinLowEdge(1);
//  double max_xrange = h2->GetXaxis()->GetBinLowEdge(nbinsx+1);
  double min_yrange = h2->GetYaxis()->GetBinLowEdge(1);
  double max_yrange = h2->GetYaxis()->GetBinLowEdge(nbinsy+1);

  TString name;
  TString title;
  name = "aaa";
  //title = "aaa";
  TGraphErrors *prof = new TGraphErrors();
  prof->SetName(name);
  //prof->SetTitle(title);

  TH1 *h_projx = h2->ProjectionX("projx");

  TF1 gaussian("gaussian","gaus",min_yrange,max_yrange);
  gaussian.SetLineColor(4);

  TH1 *h_projy{nullptr};
  double adcmean = 0.;  // x-value
//  double adcnum = 0.;

  for (int ibin=1; ibin<=nbinsx; ibin++)
  {
    name = "hproj_"; name += ibin;
    if ( h_projy==nullptr )
    {
      h_projy = h2->ProjectionY(name,ibin,ibin);
      adcmean = h_projx->GetBinCenter(ibin);
//      adcnum = 1.0;
    }
    /*
    else  // keep summing until we get enough statistics in bin
    {
      TH1 *h_projyadd =  h2->ProjectionY(name,ibin,ibin);
      h_projy->Add( h_projyadd );
      delete h_projyadd;

      adcmean += h_projx->GetBinCenter(ibin);
      adcnum += 1.0;
    }
    */

    std::cout << "adcmean " << adcmean << std::endl;

    if ( h_projy->Integral()>100 || ibin==nbinsx )
    {
      //adcmean = adcmean/adcnum;

      h_projy->Draw();

      int maxbin = h_projy->GetMaximumBin();
      double xmax = h_projy->GetBinCenter( maxbin );
      double ymax = h_projy->GetBinContent( maxbin );
      gaussian.SetParameter(1,xmax);
      gaussian.SetParameter(0,ymax);
      gaussian.SetRange(xmax-0.6,xmax+0.6);

      std::cout << "xmax " << xmax << "\t" << ymax << "\t" << h_projy->GetRMS() << std::endl;
      auto fitresult = h_projy->Fit("gaussian","RWWS");

      int n = prof->GetN();

      if ( fitresult->IsValid() )
      {
        double mean = gaussian.GetParameter(1);
        double meanerr = gaussian.GetParError(1);

        prof->SetPoint(n,adcmean,mean);
        prof->SetPointError(n,0,meanerr);
        //std::cout << "mean and meanerr " << mean << "\t" << meanerr << std::endl;
      }
      else
      {
        prof->SetPoint(n,adcmean,h_projy->GetMean());
        prof->SetPointError(n,0,h_projy->GetRMS());
      }

      gPad->Modified();
      gPad->Update();

      /*
      string junk;
      cin >> junk;
      */

      delete h_projy;
      h_projy = nullptr;
    }

  }

  delete h_projx;

  prof->SetBit(TGraph::kIsSortedX);
  return prof;
}

#endif
