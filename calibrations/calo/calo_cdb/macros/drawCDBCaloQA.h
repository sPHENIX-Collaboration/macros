#ifndef DRAWCDBCALOQA_H
#define DRAWCDBCALOQA_H

#include <TGraph.h>
#include <TH1.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TString.h>

void drawText(const char *text, float xp, float yp, int textColor = kBlack, int textSize = 18)
{
  TLatex *tex = new TLatex(xp, yp, text);
  tex->SetTextFont(43);
  tex->SetTextSize(textSize);
  tex->SetTextColor(textColor);
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw();
}

void easyLeg(TLegend *a, const TString &head = "")
{
  a->SetBorderSize(0);
  a->SetHeader(head);
  a->SetTextFont(42);
  a->SetLineColor(1);
  a->SetLineStyle(1);
  a->SetLineWidth(1);
  a->SetFillColor(0);
  a->SetFillStyle(0);
}

void handsomeTH1(TH1 *a, int col = 1, float size = 1, int markerstyle = 20)
{
  a->SetMarkerColor(col);
  a->SetMarkerSize(size);
  a->SetMarkerStyle(markerstyle);
  a->SetLineColor(col);
  a->GetYaxis()->SetTitleOffset(1.25);
  a->GetXaxis()->CenterTitle();
  a->GetYaxis()->CenterTitle();
}

void SetHistStyle(TH1 *h, int c, int m)
{
  const int colorArr[] = {kGray + 3, kRed + 1, kBlue + 1, kGreen + 2, kOrange + 7, kAzure + 9, kViolet - 1, kGreen + 1, kBlack};
  const int markerFullArr[] = {kFullCircle, kFullTriangleUp, kFullTriangleDown, kFullSquare, kFullStar, kFullDiamond};
  const int markerOpenArr[] = {kOpenCircle, kOpenTriangleUp, kOpenTriangleDown, kOpenSquare, kOpenStar, kOpenDiamond};
  h->SetMarkerColor(colorArr[c]);
  h->SetMarkerStyle(m < 10 ? markerFullArr[m] : markerOpenArr[m - 10]);
  h->SetMarkerSize(1.0);
  h->SetLineColor(colorArr[c]);
  h->SetLineWidth(2.);
  h->GetXaxis()->CenterTitle();
  h->GetYaxis()->CenterTitle();
}

void SetGraphStyle(TGraph *gr, int c, int m)
{
  const int colorArr[] = {kPink - 6, kBlue - 3, kGreen + 2, kViolet - 6, kBlack};
  const int markerFullArr[] = {kFullCircle, kFullSquare, kFullDiamond, kFullCross};
  const int markerOpenArr[] = {kOpenCircle, kOpenSquare, kOpenDiamond, kOpenCross};
  gr->SetMarkerColor(colorArr[c]);
  gr->SetMarkerStyle(m < 4 ? markerFullArr[m] : markerOpenArr[m - 4]);
  gr->SetMarkerSize((m == 2 || m == 3) ? 2.6 : 1.5);
  gr->SetLineColor(colorArr[c]);
  gr->SetLineWidth(1);
}

#endif
