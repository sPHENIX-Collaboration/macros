
#include <Rtypes.h>
#include <TGraphAsymmErrors.h>
#include <TGraphErrors.h>
#include <TH1.h>
#include <TLatex.h>
#include <TLine.h>
#include <TMarker.h>
#include <TPave.h>

#include <iostream>
#include <format>

void ATLAS_LABEL(Double_t x,Double_t y,Color_t color) 
{
  TLatex l; //l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextFont(72);
  l.SetTextColor(color);
  l.DrawLatex(x,y,"ATLAS");
}

TGraphErrors* myTGraphErrorsDivide(TGraphErrors* g1,TGraphErrors* g2) {
 
  const Int_t debug=0; 

  if (!g1) std::cout << "**myTGraphErrorsDivide: g1 does not exist !" << std::endl; 
  if (!g2) std::cout << "**myTGraphErrorsDivide: g2 does not exist !" << std::endl; 


  Int_t n1=g1->GetN();
  Int_t n2=g2->GetN();

  if (n1!=n2) {
    std::cout << "**myTGraphErrorsDivide: vector do not have same number of entries !" << std::endl; 
  }

  TGraphErrors* g3= new TGraphErrors();

  Double_t  x1=0.;
  Double_t  y1=0.;
  Double_t  x2=0.;
  Double_t  y2=0.;
  Double_t dx1=0.;
  Double_t dy1=0.;
  Double_t dy2=0.;

  Int_t iv=0;
  for (Int_t i1=0; i1<n1; i1++) {
   for (Int_t i2=0; i2<n2; i2++) {
     //if (debug) std::cout << std::format("**myTGraphErrorsDivide: {}  {} !",i1,i2) << std::endl;

    g1->GetPoint(i1,x1,y1);
    g2->GetPoint(i2,x2,y2);
    if (x1!=x2) {
      //std::cout << std::format("**myTGraphErrorsDivide: {} x1!=x2  {} {}  !",iv,x1,x2) << std::endl;
    }else{
      //if (debug) std::cout << std::format("**myTGraphErrorsDivide: {} x1=x2  {} {}  !",iv,x1,x2) << std::endl;
     dx1  = g1->GetErrorX(i1);
     if (y1!=0) dy1  = g1->GetErrorY(i1)/y1;
     if (y2!=0) dy2  = g2->GetErrorY(i2)/y2;
   
     if (debug)
       std::cout << std::format("**myTGraphErrorsDivide: {} x1={} x2={} y1={} y2={}",iv,x1,x2,y1,y2) << std::endl;;

     if (y2!=0.) g3->SetPoint(iv, x1,y1/y2);
     else        g3->SetPoint(iv, x1,y2);
   
     Double_t e=0.;
     if (y1!=0 && y2!=0) e=std::sqrt(dy1*dy1+dy2*dy2)*(y1/y2); 
     g3->SetPointError(iv,dx1,e);


     if (debug) {
       //Double_t g3y, g3x,g3e;
       //g3->GetPoint(iv, g3y,g3x);
       //g3e=g3->GetErrorY(iv);
       //std::cout << std::format("{} g3y= {} g3e={}",iv,g3y,g3e) << std::endl;
     }
     iv++;
    }
    //    std::cout << "**myTGraphErrorsDivide: ...next" << std::endl;
   }
  }  
  return g3;

}


TGraphAsymmErrors* myTGraphErrorsDivide(TGraphAsymmErrors* g1,TGraphAsymmErrors* g2) {

  const Int_t debug=1; 

  TGraphAsymmErrors* g3= new TGraphAsymmErrors();
  Int_t n1=g1->GetN();
  Int_t n2=g2->GetN();

  if (n1!=n2) {
    std::cout << " vectors do not have same number of entries !" << std::endl;;
   return g3;
  }

  Double_t   x1=0.;
  Double_t   y1=0.;
  Double_t   x2=0.;
  Double_t   y2=0.;
  Double_t dx1h=0.;
  Double_t dx1l=0.;
  Double_t dy1h=0.;
  Double_t dy1l=0.;
  Double_t dy2h=0.;
  Double_t dy2l=0.;

//  Double_t* X1 = g1->GetX();
//  Double_t* Y1 = g1->GetY();
  Double_t* EXhigh1 = g1->GetEXhigh();
  Double_t* EXlow1 =  g1->GetEXlow();
  Double_t* EYhigh1 = g1->GetEYhigh();
  Double_t* EYlow1 =  g1->GetEYlow();

//  Double_t* X2 = g2->GetX();
//  Double_t* Y2 = g2->GetY();
//  Double_t* EXhigh2 = g2->GetEXhigh();
//  Double_t* EXlow2 =  g2->GetEXlow();
  Double_t* EYhigh2 = g2->GetEYhigh();
  Double_t* EYlow2 =  g2->GetEYlow();
  for (Int_t i=0; i<g1->GetN(); i++) {
    g1->GetPoint(i,x1,y1);
    g2->GetPoint(i,x2,y2);
    dx1h  = EXhigh1[i];
    dx1l  = EXlow1[i];
    if (y1!=0.) dy1h  = EYhigh1[i]/y1;
    else        dy1h  = 0.;
    if (y2!=0.) dy2h  = EYhigh2[i]/y2;
    else        dy2h  = 0.;
    if (y1!=0.) dy1l  = EYlow1 [i]/y1;
    else        dy1l  = 0.;
    if (y2!=0.) dy2l  = EYlow2 [i]/y2;
    else        dy2l  = 0.;
   
    //if (debug)
    //std::cout << std::format("{} x1={} x2={} y1={} y2={}",i,x1,x2,y1,y2) << std::endl;
    if (debug)
      std::cout << std::format("{} dy1={} {} dy2={} {} sqrt= {} {}",i,dy1l,dy1h,dy2l,dy2h,
			       std::sqrt(dy1l*dy1l+dy2l*dy2l), std::sqrt(dy1h*dy1h+dy2h*dy2h)) << std::endl;

    if (y2!=0.) g3->SetPoint(i, x1,y1/y2);
    else       g3->SetPoint(i, x1,y2);
    Double_t el=0.; Double_t eh=0.;

    if (y1!=0. && y2!=0.) el=std::sqrt(dy1l*dy1l+dy2l*dy2l)*(y1/y2);
    if (y1!=0. && y2!=0.) eh=std::sqrt(dy1h*dy1h+dy2h*dy2h)*(y1/y2);

    if (debug) std::cout << std::format("dx1h={}  dx1l={}  el={}  eh={}",dx1h,dx1l,el,eh) << std::endl;
    g3->SetPointError(i,dx1l,dx1h,el,eh);

  }  
  return g3;

}



TGraphAsymmErrors* myMakeBand(TGraphErrors* g0, TGraphErrors* g1,TGraphErrors* g2) {
  // default is g0
    //const Int_t debug=0;

  TGraphAsymmErrors* g3= new TGraphAsymmErrors();

  Double_t  x1=0.;
  Double_t  y1=0.;
  Double_t  x2=0.;
  Double_t  y2=0.;
  Double_t  y0=0;
  Double_t  x3=0.;
  //Double_t dx1=0.;
  Double_t dum;
  for (Int_t i=0; i<g1->GetN(); i++) {
    g0->GetPoint(i, x1,y0);
    g1->GetPoint(i, x1,y1);
    g2->GetPoint(i, x1,y2);

    // if (y1==0) y1=1;
    //if (y2==0) y2=1;

    if (i==g1->GetN()-1) x2=x1;
    else                 g2->GetPoint(i+1,x2,dum);

    if (i==0)            x3=x1;
    else                 g2->GetPoint(i-1,x3,dum);

    Double_t tmp=y2;
    if (y1<y2) {y2=y1; y1=tmp;}
    //Double_t y3=1.;
    Double_t y3=y0;
    g3->SetPoint(i,x1,y3);

    Double_t binwl=(x1-x3)/2.;
    Double_t binwh=(x2-x1)/2.;
    if (binwl==0.)  binwl= binwh;
    if (binwh==0.)  binwh= binwl;
    g3->SetPointError(i,binwl,binwh,(y3-y2),(y1-y3));

  }
  return g3;

}

void myAddtoBand(TGraphErrors* g1, TGraphAsymmErrors* g2) {

  Double_t  x1=0.;
  Double_t  y1=0.;
  Double_t  y2=0.;
  Double_t  y0=0;
  //Double_t dx1=0.;
  //Double_t dum;

  if (g1->GetN()!=g2->GetN())
    std::cout << " graphs have not the same # of elements " << std::endl;

  Double_t* EYhigh = g2-> GetEYhigh();
  Double_t* EYlow  = g2-> GetEYlow();

  for (Int_t i=0; i<g1->GetN(); i++) {
    g1->GetPoint(i, x1,y1);
    g2->GetPoint(i, x1,y2);
    
    if ( y1==0 || y2==0 ) { 
      std::cerr << "check these points very carefully : myAddtoBand() : point " << i << std::endl;  
    }
    //    if (y1==0) y1=1;
    //    if (y2==0) y2=1;

    //    if (i==g1->GetN()-1) x2=x1;
    //    else                 g2->GetPoint(i+1,x2,dum);
    //    if (i==0)            x3=x1;
    //    else                 g2->GetPoint(i-1,x3,dum);

    Double_t eyh=0.;
    Double_t eyl=0.;
    //if (y1<y2) {y2=y1; y1=tmp;}
    //Double_t y3=1.;

    //std::cout << std::format("{}: y1={} y2={} Eyhigh= {} Eylow= {}",i,y1,y2,EYhigh[i],EYlow[i]) << std::endl;

    y0=y1-y2;
    if (y0!=0) {
     if (y0>0){
      eyh=EYhigh[i];
      eyh=std::sqrt(eyh*eyh+y0*y0);
      //std::cout << std::format("high: {}: y0={} eyh={}",i,y0,eyh) << std::endl;
      g2->SetPointEYhigh(i,eyh);
     } else {
      eyl=EYlow[i];
      eyl=std::sqrt(eyl*eyl+y0*y0);
      // std::cout << std::format("low: {}: y0={} eyl={}",i,y0,eyl) << std::endl;
      g2->SetPointEYlow (i,eyl);
     }
    }
  }
  return;

}

TGraphErrors* TH1TOTGraph(TH1 *h1){


  if (!h1) std::cout << "TH1TOTGraph: histogram not found !" << std::endl;

 TGraphErrors* g1= new TGraphErrors();

 Double_t x;
 Double_t y;
 Double_t ex;
 Double_t ey;
 for (Int_t i=1 ; i<=h1->GetNbinsX(); i++) {
   y=h1->GetBinContent(i);
   ey=h1->GetBinError(i);
   x=h1->GetBinCenter(i);
   ex=h1->GetBinWidth(i)/2.;
   
  //   std::cout << " x,y = " << x << " " << y << " ex,ey = " << ex << " " << ey << std::endl;

   g1->SetPoint(i-1,x,y);
   g1->SetPointError(i-1,ex,ey);

 }

 //g1->Print();

 return g1;
}

void myText(Double_t x,Double_t y,Color_t color, const char *text, Double_t tsize) {

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.SetTextColor(color);
  l.DrawLatex(x,y,text);
}
 

void myBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,Int_t bstyle,Int_t lcolor,Int_t lstyle, const char *text) 
{

  Double_t tsize=0.04;

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.25*tsize;
  Double_t y2=y+0.25*tsize;
  Double_t x2=x-0.3*tsize;
  Double_t x1=x2-boxsize;

  std::cout << std::format("x1= {} x2= {} y1= {} y2= {}",x1,x2,y1,y2) << std::endl;

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  //mbox->SetFillStyle(1001);
  mbox->SetFillStyle(bstyle);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(2);
  mline.SetLineColor(lcolor);
  mline.SetLineStyle(lstyle);
  //mline.SetLineStyle(5);
/*
  Double_t y_new=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y_new,x2,y_new);
*/
  mline.DrawLineNDC(x1,y1,x2,y1);
  mline.DrawLineNDC(x1,y2,x2,y2);
  mline.DrawLineNDC(x1,y1,x1,y2);
  mline.DrawLineNDC(x2,y1,x2,y2);

}


void myMarkerText(Double_t x,Double_t y,Int_t color,Int_t mstyle, const char *text,Float_t msize, Double_t tsize) 
{
  TMarker *marker = new TMarker(x-(0.55*tsize),y,8);
  marker->SetMarkerColor(color);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(msize);
  marker->Draw();

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.5*tsize;
  Double_t y2=y+0.5*tsize;
  Double_t x2=x-0.15*tsize;
  Double_t x1=x-0.95*tsize;

  TLine mline;
  mline.SetLineWidth(2);
  mline.SetLineColor(color);
  mline.SetLineStyle(1);
//  Double_t y_new=(y1+y2)/2.;
  Double_t x_new=(x1+x2)/2.;

  double size_y = 0.3*tsize;
//  double size_x = 0.28*tsize;
  //double size = msize;
  //mline.DrawLineNDC(x1,y_new,x1+size_x,y_new); 
  //mline.DrawLineNDC(x2-size_x,y_new,x2,y_new); 
  mline.DrawLineNDC(x_new,y2-size_y,x_new,y2); 
  mline.DrawLineNDC(x_new,y1,x_new,y1+size_y); 
}

void myMarkerLineText(Double_t x, Double_t y,Float_t msize,Int_t mcolor,Int_t mstyle,Int_t lcolor,Int_t lstyle, const char *text, Double_t tsize, bool EX0)
{
  //tsize = 0.04;
  TMarker *marker = new TMarker(x-(0.55*tsize),y,8);
  marker->SetMarkerColor(mcolor);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(msize);

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.5*tsize;
  Double_t y2=y+0.5*tsize;
  Double_t x2=x-0.15*tsize;
  Double_t x1=x-0.95*tsize;

  TLine mline;
  mline.SetLineWidth(2);
  mline.SetLineColor(lcolor);
  mline.SetLineStyle(lstyle);
  Double_t y_new=(y1+y2)/2.;
  Double_t x_new=(x1+x2)/2.;

  double size_y = 0.3*tsize;
  double size_x = 0.28*tsize;
  //double size = msize;
  if (msize!=0) {
    if (!EX0) {
      mline.DrawLineNDC(x1,       y_new,x1+size_x,y_new); 
      mline.DrawLineNDC(x2-size_x,y_new,x2,       y_new); 
    }
    mline.DrawLineNDC(x_new,y2-size_y,x_new,y2); 
    mline.DrawLineNDC(x_new,y1,x_new,y1+size_y); 
  } else {
    mline.DrawLineNDC(x-0.95*tsize,y_new,x-0.15*tsize,y_new); 
  }
  marker->Draw();

}



void mySmallMarkerLineText(Double_t x, Double_t y,Float_t msize,Int_t mcolor,Int_t mstyle,Int_t lcolor,Int_t lstyle, const char *text)
{
  Double_t tsize=0.03;
  TMarker *marker = new TMarker(x-(0.55*tsize),y,8);
  marker->SetMarkerColor(mcolor);  marker->SetNDC();
  marker->SetMarkerStyle(mstyle);
  marker->SetMarkerSize(msize); 
  //marker->Draw();
  
  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.DrawLatex(x,y,text);
  
  Double_t y1=y-0.5*tsize;
  Double_t y2=y+0.5*tsize;
  Double_t x2=x-0.15*tsize;
  Double_t x1=x-0.95*tsize;

  TLine mline;
  mline.SetLineWidth(2);
  mline.SetLineColor(lcolor);
  mline.SetLineStyle(lstyle);
  Double_t y_new=(y1+y2)/2.;
  Double_t x_new=(x1+x2)/2.;
  double size_y = 0.22*tsize;
  double size_x = 0.22*tsize;
  if (msize!=0) {
    mline.DrawLineNDC(x1,y_new,x1+size_x,y_new); 
    mline.DrawLineNDC(x2-size_x,y_new,x2,y_new); 
    mline.DrawLineNDC(x_new,y2-size_y,x_new,y2); 
    mline.DrawLineNDC(x_new,y1,x_new,y1+size_y); 
  } else {
    mline.DrawLineNDC(x-0.95*tsize,y_new,x-0.15*tsize,y_new); 
  }
  marker->Draw();
}



void myOnlyBoxText(Double_t x, Double_t y,Double_t  /*boxsize*/,Int_t mcolor,Int_t lcolor,Int_t lstyle, const char *text, Double_t tsize, Int_t bstyle) 
{


  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize); 
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.25*tsize;
  Double_t y2=y+0.25*tsize;
  Double_t x2=x-0.15*tsize;
  Double_t x1=x-0.95*tsize;

  std::cout << std::format("x1= {} x2= {} y1= {} y2= {}",x1,x2,y1,y2) << std::endl;

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(bstyle);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(1);
  mline.SetLineColor(lcolor);
  //mline.SetLineStyle(lstyle);
  mline.SetLineStyle(lstyle);
//  Double_t y_new=(y1+y2)/2.;
  //mline.DrawLineNDC(x1,y_new,x2,y_new);
  mline.DrawLineNDC(x1,y1,x2,y1);
  mline.DrawLineNDC(x1,y2,x2,y2);
  mline.DrawLineNDC(x1,y1,x1,y2);
  mline.DrawLineNDC(x2,y1,x2,y2);

}

void mySmallBoxText(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor,Int_t lcolor,Int_t lstyle, const char *text)
{

  Double_t tsize=0.03;

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.25*tsize;
  Double_t y2=y+0.25*tsize;
  Double_t x2=x-0.3*tsize;
  Double_t x1=x2-boxsize;

  std::cout << std::format("x1= {} x2= {} y1= {} y2= {}",x1,x2,y1,y2) << std::endl;

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColor(mcolor);
  mbox->SetFillStyle(3015);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(2);
  mline.SetLineColor(lcolor);
  mline.SetLineStyle(lstyle);
//  Double_t y_new=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y1,x2,y1);
  mline.DrawLineNDC(x1,y2,x2,y2);
  mline.DrawLineNDC(x1,y1,x1,y2);
  mline.DrawLineNDC(x2,y1,x2,y2);

}


void myBoxTextAlpha(Double_t x, Double_t y,Double_t boxsize,Int_t mcolor, Double_t falpha,Int_t lcolor,Int_t lstyle, const char *text)
{
  //gStyle->SetCanvasPreferGL(true);
  Double_t tsize=0.04;

  TLatex l; l.SetTextAlign(12); l.SetTextSize(tsize);
  l.SetNDC();
  l.DrawLatex(x,y,text);

  Double_t y1=y-0.25*tsize;
  Double_t y2=y+0.25*tsize;
  Double_t x2=x-0.3*tsize;
  Double_t x1=x2-boxsize;

  std::cout << std::format("x1= {} x2= {} y1= {} y2= {}",x1,x2,y1,y2) << std::endl;

  TPave *mbox= new TPave(x1,y1,x2,y2,0,"NDC");

  mbox->SetFillColorAlpha(mcolor,falpha);
  //mbox->SetFillStyle(1001);
  //mbox->SetFillStyle(bstyle);
  mbox->Draw();

  TLine mline;
  mline.SetLineWidth(2);
  mline.SetLineColor(lcolor);
  mline.SetLineStyle(lstyle);
  //mline.SetLineStyle(5);
/*
  Double_t y_new=(y1+y2)/2.;
  mline.DrawLineNDC(x1,y_new,x2,y_new);
*/
  mline.DrawLineNDC(x1,y1,x2,y1);
  mline.DrawLineNDC(x1,y2,x2,y2);
  mline.DrawLineNDC(x1,y1,x1,y2);
  mline.DrawLineNDC(x2,y1,x2,y2);

}
