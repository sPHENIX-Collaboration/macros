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
  gaussian.SetNpx(1000);
  gaussian.SetLineColor(4);

  TH1 *h_projy{nullptr};
  double prev_xmean = -1e-12;  // prev x-value
  double xmean = 0.;  // x-value
  double xsum = 0.;

  for (int ibin=1; ibin<=nbinsx; ibin++)
  {
    name = "hproj_"; name += ibin;
    if ( h_projy==nullptr )
    {
      h_projy = h2->ProjectionY(name,ibin,ibin);
      xmean = h_projy->Integral()*h_projx->GetBinCenter(ibin);
      xsum = h_projy->Integral();
    }
    else  // keep summing until we get enough statistics in bin
    {
      TH1 *h_projyadd =  h2->ProjectionY(name,ibin,ibin);
      h_projy->Add( h_projyadd );

      xmean += h_projyadd->Integral()*h_projx->GetBinCenter(ibin);
      xsum += h_projyadd->Integral();

      delete h_projyadd;
    }

    //if ( xsum>0. ) std::cout << "xmean " << xmean/xsum << std::endl;

    if ( h_projy->Integral()>100 || ibin==nbinsx )
    {
      xmean = xmean/xsum;

      h_projy->Draw();

      int maxbin = h_projy->GetMaximumBin();
      double xmax = h_projy->GetBinCenter( maxbin );
      double ymax = h_projy->GetBinContent( maxbin );
      gaussian.SetParameter(1,xmax);
      gaussian.SetParameter(0,ymax);
      gaussian.SetRange(xmax-0.6,xmax+0.6);

      //std::cout << "xmax " << xmax << "\t" << ymax << "\t" << h_projy->GetRMS() << std::endl;
      auto fitresult = h_projy->Fit("gaussian","QRWWS");

      int n = prof->GetN();

      if ( fitresult->IsValid() )
      {
        double ymean = gaussian.GetParameter(1);
        double ymeanerr = gaussian.GetParError(1);

        prof->SetPoint(n,xmean,ymean);
        prof->SetPointError(n,0,ymeanerr);
        //std::cout << "ymean and ymeanerr " << ymean << "\t" << ymeanerr << std::endl;
      }
      else
      {
        double ymean = h_projy->GetMean();
        double ymeanerr = h_projy->GetRMS();

        prof->SetPoint(n,xmean,ymean);
        prof->SetPointError(n,0,ymeanerr);

        cout << "fit failed, using means " << xmean << "\t" << ymean << endl;
      }

      if ( xmean < prev_xmean )
      {
        std::cout << "ERROR, xmean < prev_xmean, " <<  xmean << "\t" << prev_xmean << std::endl;
      }
      prev_xmean = xmean;

      /*
      gPad->Modified();
      gPad->Update();
      */

      /*
      string junk;
      cin >> junk;
      */

      delete h_projy;
      h_projy = nullptr;
    }

  }

  delete h_projx;

  /*
  // Add points until 1500, to extend fit in low stats region
  int n = prof->GetN();
  double x1,y1;
  double x0,y0;
  prof->GetPoint(n-1,x1,y1);
  prof->GetPoint(n-2,x0,y0);
  double dx = x1-x0;
  double dy = y1-y0;
  //double m = dy/dx;
  //double b = y1-m*x1;
  TF1 *poly1 = new TF1("poly1","pol1",0,15000);
  prof->Fit(poly1,"R");
  double m = poly1->GetParameter(1);
  double b = poly1->GetParameter(0);
  while (xmean<15000)
  {
    int n = prof->GetN();
    xmean += 100;
    double y = m*xmean+b;
    prof->SetPoint(n,xmean,y);
    prof->SetPointError(n,0,0.0001);
  }
  */

  prof->SetBit(TGraph::kIsSortedX);
  return prof;
}

#endif
