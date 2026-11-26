// Pass 2 of CDBCaloQA: per-run plots, summary vs run number, CDB status map. PDF only.
// root -b -q 'drawCDBCaloQA.C("runfiles.txt", "cdbstatus.txt", "outdir", "Run24pp")'
// runfiles.txt : one line per run "<run> <HIST_CDBCALOQA file>"
// cdbstatus.txt: one line per run and CDB domain "<run> <domain> <status>", status = green/orange/red/blue
#include "drawCDBCaloQA.h"

#include <phool/RunnumberRange.h>

#include <calobase/TowerInfoDefs.h>

#include <cdbobjects/CDBTTree.h>

#include <TCanvas.h>
#include <TColor.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TFile.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include <TH2Poly.h>
#include <TKey.h>
#include <TLine.h>
#include <TNamed.h>
#include <TPad.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libcdbobjects.so)

namespace
{
  const std::vector<std::string> layers{"cemc", "ihcal", "ohcal"};
  const std::vector<std::string> zsxcalibname{"CEMC_ZSCrossCalib", "HCALIN_ZSCrossCalib", "HCALOUT_ZSCrossCalib"};
  const std::vector<std::string> calodet{"CEMC", "HCALIN", "HCALOUT"};
  const std::vector<std::string> layerlabel{"EMCal", "IHCal", "OHCal"};
  const std::vector<std::string> trigs{"MB", "Photon", "Jet"};
  std::vector<std::string> g_trigname{"MB", "Photon", "Jet"};  // full trigger names, read from each run file
  const std::vector<int> trigcolor{kBlack, kBlue + 1, kRed + 1};
  std::string g_tag;    // "Run 47289" label on every plot
  std::string g_coll;   // collision system and energy, from the run number
  std::string g_label;  // text line of the last label(), written with the next plot's objects for the web page

  std::string collisionLabel(int run)
  {
    if (run >= RunnumberRange::RUN2PP_FIRST && run <= RunnumberRange::RUN2PP_LAST) return "Run24 p+p #sqrt{s} = 200 GeV";
    if (run >= RunnumberRange::RUN2AUAU_FIRST && run <= RunnumberRange::RUN2AUAU_LAST) return "Run24 Au+Au #sqrt{s_{NN}} = 200 GeV";
    if (run >= RunnumberRange::RUN3AUAU_FIRST && run <= RunnumberRange::RUN3AUAU_LAST) return "Run25 Au+Au #sqrt{s_{NN}} = 200 GeV";
    if (run >= RunnumberRange::RUN3PP_FIRST && run <= RunnumberRange::RUN3PP_LAST) return "Run25 p+p #sqrt{s} = 200 GeV";
    if (run >= RunnumberRange::RUN3OO_FIRST && run <= RunnumberRange::RUN3OO_LAST) return "Run25 O+O #sqrt{s_{NN}} = 200 GeV";
    // simulation (MDC2 run numbers)
    if (run == 28) return "MC p+p PYTHIA8 Detroit #sqrt{s} = 200 GeV";
    if (run == 31) return "MC Au+Au HIJING 0-20 fm #sqrt{s_{NN}} = 200 GeV";
    if (run == 35) return "MC O+O HIJING 0-15 fm #sqrt{s_{NN}} = 200 GeV";
    return "";
  }

  // axis maximum so that data drawn between ymin and ymax stay below the fraction frac of the frame height
  double headroom(double ymin, double ymax, bool logy, double frac)
  {
    if (logy)
    {
      return ymin * std::pow(ymax / ymin, 1. / frac);
    }
    return ymax / frac;
  }

  TH1 *get1(TFile *f, const std::string &n) { return dynamic_cast<TH1 *>(f->Get(("h_CDBCaloQA_" + n).c_str())); }
  TH2 *get2(TFile *f, const std::string &n) { return dynamic_cast<TH2 *>(f->Get(("h2_CDBCaloQA_" + n).c_str())); }

  void label(const std::string &extra = "")
  {
    g_label = extra;
    drawText("#it{#bf{sPHENIX}} Internal", 0.16, 0.92, kBlack, 18);
    drawText((g_coll + (g_tag.empty() ? "" : ", " + g_tag)).c_str(), 0.42, 0.92, kBlack, 15);
    if (!extra.empty())
    {
      drawText(extra.c_str(), 0.18, 0.85, kBlack, 16);
    }
  }

  TCanvas *newCanvas(bool logy = false)
  {
    TCanvas *c = new TCanvas("c", "", 650, 550);
    c->SetLeftMargin(0.14);
    c->SetRightMargin(0.06);
    c->SetTopMargin(0.10);
    c->SetLogy(logy);
    c->SetTicks(1, 1);
    return c;
  }

  // plotted objects of the current run (or of the summary) for the tag comparison on the web pages
  TFile *g_objfile = nullptr;
  bool g_runplots = true;  // false: only the summary vs run is drawn (per-run PDFs and objects are not written)

  // key of a plot from its PDF name: run47289_cemc_e.pdf -> cemc_e, mbd_zvtx_vs_run.pdf -> mbd_zvtx
  std::string plotKey(const std::string &file)
  {
    std::string k = file.substr(file.rfind('/') + 1);
    k = k.substr(0, k.rfind(".pdf"));
    if (k.rfind("run", 0) == 0 && k.find('_') != std::string::npos && isdigit(k[3]))
    {
      k = k.substr(k.find('_') + 1);
    }
    if (k.size() > 7 && k.substr(k.size() - 7) == "_vs_run")
    {
      k = k.substr(0, k.size() - 7);
    }
    return k;
  }

  // write the objects of one plot: <key>_0, <key>_1, ... and <key>_meta (draw options, legend names, log axes)
  void keep(const std::string &file, const std::vector<TObject *> &objs, const std::vector<std::string> &opts,
            const std::vector<std::string> &names, bool logy, bool logx = false)
  {
    if (!g_objfile)
    {
      return;
    }
    const std::string key = plotKey(file);
    TDirectory *was = gDirectory;
    g_objfile->cd();
    std::string meta = Form("{\"logy\":%d,\"logx\":%d,\"opts\":[", logy, logx);
    for (size_t i = 0; i < objs.size(); i++)
    {
      objs[i]->Clone(Form("%s_%zu", key.c_str(), i))->Write();
      meta += Form("%s\"%s\"", i ? "," : "", opts[i].c_str());
    }
    meta += "],\"names\":[";
    for (size_t i = 0; i < names.size(); i++)
    {
      std::string n = names[i];
      for (size_t p = n.find('"'); p != std::string::npos; p = n.find('"', p + 2))
      {
        n.replace(p, 1, "\\\"");
      }
      meta += Form("%s\"%s\"", i ? "," : "", n.c_str());
    }
    std::string text = g_label;
    for (size_t p = text.find('"'); p != std::string::npos; p = text.find('"', p + 2))
    {
      text.replace(p, 1, "\\\"");
    }
    meta += "],\"text\":\"" + text + "\"}";
    g_label.clear();
    TNamed(Form("%s_meta", key.c_str()), meta.c_str()).Write();
    was->cd();
  }

  void save(TCanvas *c, const std::string &file)
  {
    if (!g_runplots && file.find("/runs/") != std::string::npos)
    {
      delete c;
      return;
    }
    c->SaveAs(file.c_str());
    delete c;
  }

  // dashed reference line across the frame of the current pad: vertical at x = v, or horizontal at y = v
  void refline(double v, bool vertical)
  {
    gPad->Update();
    double x1 = gPad->GetUxmin();
    double x2 = gPad->GetUxmax();
    double y1 = gPad->GetUymin();
    double y2 = gPad->GetUymax();
    if (gPad->GetLogx())
    {
      x1 = std::pow(10, x1);
      x2 = std::pow(10, x2);
    }
    if (gPad->GetLogy())
    {
      y1 = std::pow(10, y1);
      y2 = std::pow(10, y2);
    }
    if (vertical ? (v <= x1 || v >= x2) : (v <= y1 || v >= y2))
    {
      return;
    }
    TLine *l = vertical ? new TLine(v, y1, v, y2) : new TLine(x1, v, x2, v);
    l->SetLineStyle(2);
    l->SetLineColor(kGray + 2);
    l->Draw();
  }

  // overlay of several histograms, each normalised to unit area; vlines: dashed vertical reference lines
  void overlay(const std::vector<TH1 *> &hs, const std::vector<std::string> &names, const std::vector<int> &cols,
               const std::string &xtitle, const std::string &file, bool logy, const std::string &extra = "", bool logx = false,
               const std::vector<double> &vlines = {})
  {
    TCanvas *c = newCanvas(logy);
    c->SetLogx(logx);
    // legend only for more than one histogram; the data stay below the legend and the text line
    const bool doleg = hs.size() > 1;
    const double ytop = std::min(doleg ? 0.86 - 0.055 * hs.size() : 0.88, extra.empty() ? 0.88 : 0.82) - 0.02;
    const double frac = (ytop - 0.10) / 0.80;
    TLegend *leg = new TLegend(0.40, 0.86 - 0.055 * hs.size(), 0.92, 0.86);
    easyLeg(leg);
    leg->SetTextFont(43);  // fixed pixel size, independent of the number of entries
    leg->SetTextSize(14);
    double ymax = 0;
    double ymin = 1e30;
    for (auto *h : hs)
    {
      if (h->Integral() > 0)
      {
        h->Scale(1. / h->Integral());
      }
      ymax = std::max(ymax, h->GetMaximum());
      double m = h->GetMinimum(0);
      if (m > 0)
      {
        ymin = std::min(ymin, m);
      }
    }
    for (size_t i = 0; i < hs.size(); i++)
    {
      TH1 *h = hs[i];
      handsomeTH1(h, cols[i], 0.6);
      h->SetLineWidth(2);
      h->GetXaxis()->SetTitle(xtitle.c_str());
      h->GetYaxis()->SetTitle("Normalized counts");
      h->SetTitle("");
      if (logy)
      {
        const double lo = (ymin < 1e30 && ymax > 0) ? ymin * 0.5 : 1e-6;
        h->SetMinimum(lo);
        h->SetMaximum(ymax > lo ? headroom(lo, ymax, true, frac) : 1);
      }
      else
      {
        h->SetMinimum(0);
        h->SetMaximum(ymax > 0 ? headroom(0, ymax, false, frac) : 1);
      }
      h->Draw(i == 0 ? "hist" : "hist same");
      leg->AddEntry(h, names[i].c_str(), "l");
    }
    for (double v : vlines)
    {
      refline(v, true);
    }
    if (doleg)
    {
      leg->Draw();
    }
    label(extra);
    keep(file, std::vector<TObject *>(hs.begin(), hs.end()), std::vector<std::string>(hs.size(), "hist"),
         doleg ? names : std::vector<std::string>(), logy, logx);
    save(c, file);
  }

  void single(TH1 *h, const std::string &xtitle, const std::string &file, bool logy, const std::string &extra = "", bool logx = false,
              const std::vector<double> &vlines = {})
  {
    overlay({h}, {""}, {kBlack}, xtitle, file, logy, extra, logx, vlines);
  }

  // noise method of ZSCrossCalib/noisefit_zs.C: E<0 side mirrored to +E, rebinned above the ADC step,
  // core fitted with an iterative +-2 sigma Gaussian; per calorimeter and per ieta
  const int NOISE_REBIN[3] = {20, 2, 20};
  const double NOISE_RMSWIN[3] = {0.10, 0.01, 0.05};
  const double NOISE_SIGYMAX[3] = {50., 4., 25.};

  TH1D *mirror_negative(TH1 *h, const char *name)
  {
    TH1D *m = new TH1D(name, ";E [GeV];Towers", h->GetNbinsX(), h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax());
    int b0 = h->FindBin(0.0);
    for (int b = 1; b < b0; ++b)
    {
      double c = h->GetBinContent(b);
      double e = h->GetBinError(b);
      m->SetBinContent(b, c);
      m->SetBinError(b, e);
      int bp = m->FindBin(-h->GetBinCenter(b));
      m->SetBinContent(bp, c);
      m->SetBinError(bp, e);
    }
    return m;
  }

  // iterative core Gaussian fit; returns (mean, sigma, meanerr, sigmaerr)
  std::array<double, 4> fitcore(TH1D *m, double rmswin)
  {
    const double bw = m->GetBinWidth(1);
    const double rmin = 4 * bw;
    double rms = 0.03;
    m->GetXaxis()->SetRangeUser(-rmswin, rmswin);
    if (m->GetRMS() > 0) rms = m->GetRMS();
    m->GetXaxis()->SetRange(0, 0);
    TF1 f("fg", "gaus", -1, 1);
    f.SetParameters(m->GetMaximum(), 0, rms);
    f.SetParLimits(1, -2 * bw, 2 * bw);
    double s = rms;
    for (int it = 0; it < 3; ++it)
    {
      double r = std::max(2 * s, rmin);
      m->Fit(&f, "QN0B", "", -r, r);
      s = fabs(f.GetParameter(2));
      if (s <= 0 || !std::isfinite(s))
      {
        s = rms;
        break;
      }
    }
    double r = std::max(2 * s, rmin);
    m->Fit(&f, "Q0B", "", -r, r);
    return {f.GetParameter(1), fabs(f.GetParameter(2)), f.GetParError(1), f.GetParError(2)};
  }

  double mbdMipPeak(TH1 *q)
  {
    // most probable charge between 0.5 and 2.5
    int b1 = q->FindBin(0.5);
    int b2 = q->FindBin(2.5);
    int bmax = b1;
    for (int b = b1; b <= b2; b++)
    {
      if (q->GetBinContent(b) > q->GetBinContent(bmax))
      {
        bmax = b;
      }
    }
    return q->GetBinCenter(bmax);
  }

  // sEPD tile MIP peak (nMIP): Gaussian fit from 0.25 below to 0.35 above the highest bin in 0.5-2.5;
  // the highest bin if the fit fails or its mean is outside 0.5-2.5
  double sepdMipPeak(TH1 *q)
  {
    const double x0 = mbdMipPeak(q);
    TF1 g("gmip", "gaus", x0 - 0.25, x0 + 0.35);
    g.SetParameters(q->GetBinContent(q->FindBin(x0)), x0, 0.3);
    const int st = q->Fit(&g, "QN0R");
    const double m = g.GetParameter(1);
    return (st == 0 && m > 0.5 && m < 2.5) ? m : x0;
  }

  // ZDC arm energies, normalized counts, south and north, with the north / south ratio below; dotted lines at n x 100 GeV
  // heavy ions (fit1n): single-neutron peak, Gaussian fit in fixed 70-130 GeV starting at 100 GeV, drawn on the plot;
  // p1n: fitted means south, north (NaN: fewer than 50 events in 70-130 GeV)
  void zdcRatio(TH1 *hs, TH1 *hn, const std::string &xtitle, const std::string &file, const std::string &extra, bool fit1n,
                std::array<double, 2> &p1n)
  {
    const bool nlines = fit1n;
    std::array<TF1 *, 2> fits{nullptr, nullptr};
    std::array<double, 2> n1n{0, 0};
    for (int a = 0; a < 2; a++)
    {
      TH1 *h = a ? hn : hs;
      n1n[a] = h->Integral(h->FindBin(70.01), h->FindBin(129.99));
      p1n[a] = std::nan("");
    }
    TCanvas *c = new TCanvas("c", "", 650, 700);
    TPad *p1 = new TPad("p1", "", 0, 0.30, 1, 1);
    TPad *p2 = new TPad("p2", "", 0, 0, 1, 0.30);
    for (TPad *p : {p1, p2})
    {
      p->SetLeftMargin(0.14);
      p->SetRightMargin(0.06);
      p->SetTicks(1, 1);
    }
    p1->SetTopMargin(0.10);
    p1->SetBottomMargin(0.02);
    p1->SetLogy();
    p2->SetTopMargin(0.02);
    p2->SetBottomMargin(0.32);
    p1->Draw();
    p2->Draw();
    p1->cd();
    double ymax = 0;
    double ymin = 1e30;
    for (TH1 *h : {hs, hn})
    {
      if (h->Integral() > 0)
      {
        h->Scale(1. / h->Integral());
      }
      ymax = std::max(ymax, h->GetMaximum());
      const double m = h->GetMinimum(0);
      if (m > 0)
      {
        ymin = std::min(ymin, m);
      }
    }
    handsomeTH1(hs, kRed + 1, 0.6);
    handsomeTH1(hn, kBlue + 1, 0.6);
    for (TH1 *h : {hs, hn})
    {
      h->SetLineWidth(2);
      h->SetTitle("");
    }
    hs->GetXaxis()->SetLabelSize(0);
    hs->GetXaxis()->SetTitle("");
    hs->GetYaxis()->SetTitle("Normalized counts");
    const double lo = (ymin < 1e30 && ymax > 0) ? ymin * 0.5 : 1e-6;
    hs->SetMinimum(lo);
    hs->SetMaximum(ymax > lo ? headroom(lo, ymax, true, 0.70) : 1);
    hs->Draw("hist");
    hn->Draw("hist same");
    if (nlines)
    {
      for (int k = 1; k * 100. < hs->GetXaxis()->GetBinUpEdge(hs->GetXaxis()->GetLast()); k++)
      {
        TLine *l = new TLine(k * 100., lo, k * 100., hs->GetMaximum());
        l->SetLineStyle(3);
        l->SetLineColor(kGray + 1);
        l->Draw();
      }
    }
    TLegend *leg = new TLegend(0.66, 0.70, 0.92, 0.82);
    easyLeg(leg);
    leg->SetTextFont(43);
    leg->SetTextSize(16);
    leg->AddEntry(hs, "south", "l");
    leg->AddEntry(hn, "north", "l");
    leg->Draw();
    for (int a = 0; fit1n && a < 2; a++)
    {
      TH1 *h = a ? hn : hs;
      if (n1n[a] < 50)
      {
        drawText(Form("%s: %.0f events in 70-130 GeV, no fit", a ? "north" : "south", n1n[a]), 0.18, 0.78 - 0.05 * a, kBlack, 15);
        continue;
      }
      TF1 *g = new TF1(Form("g1n_%d", a), "gaus", 70., 130.);
      g->SetParameters(h->GetBinContent(h->FindBin(100.)), 100., 20.);
      h->Fit(g, "QN0R");
      g->SetLineColor(a ? kBlue + 3 : kRed + 3);
      g->SetLineStyle(2);
      g->SetLineWidth(3);
      g->Draw("same");
      p1n[a] = g->GetParameter(1);
      fits[a] = g;
      drawText(Form("%s 1n: %.1f #pm %.1f GeV, #sigma %.1f GeV", a ? "north" : "south", g->GetParameter(1), g->GetParError(1), g->GetParameter(2)),
               0.18, 0.78 - 0.05 * a, a ? kBlue + 1 : kRed + 1, 15);
    }
    label(extra);
    p2->cd();
    TH1 *r = (TH1 *) hn->Clone();
    r->Divide(hs);
    handsomeTH1(r, kBlack, 0.6);
    r->SetMarkerStyle(20);
    r->SetMarkerSize(0.5);
    r->SetMinimum(0.8);
    r->SetMaximum(1.2);
    r->GetXaxis()->SetTitle(xtitle.c_str());
    r->GetYaxis()->SetTitle("north / south");
    r->GetXaxis()->SetTitleSize(0.12);
    r->GetXaxis()->SetLabelSize(0.11);
    r->GetYaxis()->SetTitleSize(0.11);
    r->GetYaxis()->SetLabelSize(0.10);
    r->GetYaxis()->SetTitleOffset(0.55);
    r->GetYaxis()->SetNdivisions(505);
    r->Draw("e");
    TLine *one = new TLine(r->GetXaxis()->GetBinLowEdge(r->GetXaxis()->GetFirst()), 1., r->GetXaxis()->GetBinUpEdge(r->GetXaxis()->GetLast()), 1.);
    one->SetLineStyle(2);
    one->Draw();
    // page objects: the upper histograms with the x axis of the ratio panel, and the 1n fits as lines
    TH1 *ks = (TH1 *) hs->Clone();
    ks->GetXaxis()->SetTitle(xtitle.c_str());
    ks->GetXaxis()->SetLabelSize(hn->GetXaxis()->GetLabelSize());
    std::vector<TObject *> ko{ks, hn};
    std::vector<std::string> kopt{"hist", "hist"};
    for (TF1 *g : fits)
    {
      if (g)
      {
        ko.push_back(g);
        kopt.emplace_back("l");
      }
    }
    keep(file, ko, kopt, {"south", "north"}, true);
    save(c, file);
  }

  // 2D histogram, log z
  void draw2d(TH2 *h, const std::string &xtitle, const std::string &ytitle, const std::string &file, const std::string &extra)
  {
    TCanvas *c = newCanvas(false);
    c->SetRightMargin(0.15);
    c->SetLogz();
    h->SetTitle("");
    h->GetXaxis()->SetTitle(xtitle.c_str());
    h->GetYaxis()->SetTitle(ytitle.c_str());
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->Draw("colz");
    label(extra);
    keep(file, {h}, {"colz"}, {}, true);  // logy flag of the page: log z for a 2D histogram
    save(c, file);
  }

  // sEPD tiles of one arm in the transverse plane (x = r cos phi, y = r sin phi), one value per channel (NaN: tile not drawn);
  // ring 0: 12 tiles of 30 deg, rings 1-15: 24 tiles of 15 deg; ring edges halfway between the EpdReco tile radii
  TH2Poly *epdPoly(const std::string &name, const std::vector<double> &val, int arm)
  {
    static const double rc[16]{6.8, 11.2, 15.6, 20.565, 26.095, 31.625, 37.155, 42.685,
                               48.215, 53.745, 59.275, 64.805, 70.335, 75.865, 81.395, 86.925};
    TH2Poly *h = new TH2Poly(name.c_str(), "", -95, 95, -95, 95);
    for (int ring = 0; ring < 16; ring++)
    {
      const double r1 = (ring == 0) ? rc[0] - 0.5 * (rc[1] - rc[0]) : 0.5 * (rc[ring - 1] + rc[ring]);
      const double r2 = (ring == 15) ? rc[15] + 0.5 * (rc[15] - rc[14]) : 0.5 * (rc[ring] + rc[ring + 1]);
      const int nphi = (ring == 0) ? 12 : 24;
      for (int ip = 0; ip < nphi; ip++)
      {
        const int ch = TowerInfoDefs::decode_epd(TowerInfoDefs::encode_epd(arm, ring, ip));
        if (!std::isfinite(val[ch]))
        {
          continue;
        }
        const int ns = 6;  // arc segments
        double x[2 * ns + 2];
        double y[2 * ns + 2];
        int k = 0;
        for (int s = 0; s <= ns; s++)
        {
          const double p = 2 * M_PI * (ip + double(s) / ns) / nphi;
          x[k] = r1 * std::cos(p);
          y[k++] = r1 * std::sin(p);
        }
        for (int s = ns; s >= 0; s--)
        {
          const double p = 2 * M_PI * (ip + double(s) / ns) / nphi;
          x[k] = r2 * std::cos(p);
          y[k++] = r2 * std::sin(p);
        }
        h->SetBinContent(h->AddBin(k, x, y), val[ch]);
      }
    }
    return h;
  }

  // one value per sEPD channel as tiles: south and north side by side, common colour scale
  // zlo < zhi: fixed colour scale (tiles outside it in the end colours); otherwise the range of the values
  void epdMaps(const std::vector<double> &val, const std::string &ztitle, const std::string &file, const std::string &extra,
               double zlo = 0., double zhi = 0.)
  {
    double vlo = 1e30;
    double vhi = -1e30;
    for (double v : val)
    {
      if (std::isfinite(v))
      {
        vlo = std::min(vlo, v);
        vhi = std::max(vhi, v);
      }
    }
    if (vlo > vhi)
    {
      return;
    }
    if (zlo < zhi)
    {
      vlo = zlo;
      vhi = zhi;
    }
    const std::string key = plotKey(file);
    TCanvas *c = new TCanvas("c", "", 1100, 560);
    std::vector<TObject *> ko;
    for (int arm = 0; arm < 2; arm++)
    {
      c->cd();
      TPad *p = new TPad(Form("p%d", arm), "", 0.5 * arm, 0., 0.5 * (arm + 1), 0.82);
      p->SetLeftMargin(0.14);
      p->SetRightMargin(0.20);
      p->SetTopMargin(0.06);
      p->Draw();
      p->cd();
      TH2Poly *h = epdPoly(Form("%s_%s_%d", key.c_str(), arm ? "north" : "south", (int) val.size()), val, arm);
      h->SetMinimum(vlo - 1e-6 * std::max(1., std::fabs(vlo)));
      h->SetMaximum(vhi + 1e-6 * std::max(1., std::fabs(vhi)));
      h->GetXaxis()->SetTitle("x (cm)");
      h->GetYaxis()->SetTitle("y (cm)");
      h->GetZaxis()->SetTitle(ztitle.c_str());
      h->GetZaxis()->SetTitleOffset(1.5);
      h->GetXaxis()->CenterTitle();
      h->GetYaxis()->CenterTitle();
      h->Draw("colz");
      drawText(arm ? "north" : "south", 0.18, 0.88, kBlack, 18);
      ko.push_back(h);
    }
    c->cd();
    drawText("#it{#bf{sPHENIX}} Internal", 0.05, 0.94, kBlack, 18);
    drawText((g_coll + (g_tag.empty() ? "" : ", " + g_tag)).c_str(), 0.30, 0.94, kBlack, 15);
    drawText(extra.c_str(), 0.05, 0.875, kBlack, 16);
    keep(file, ko, {"colz", "colz"}, {"south", "north"}, false);
    save(c, file);
  }
}  // namespace

void drawCDBCaloQA(const std::string &runfiles = "runfiles.txt",
                   const std::string &statusfile = "cdbstatus.txt",
                   const std::string &outdir = "plots",
                   const std::string &period = "Run24pp",
                   bool runplots = true, bool summary = true, bool zsmap = false)
{
  g_runplots = runplots;
  gROOT->SetBatch(true);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gSystem->mkdir((outdir + "/runs").c_str(), true);
  gSystem->mkdir((outdir + "/summary").c_str(), true);

  std::vector<std::pair<int, std::string>> runs;
  {
    std::ifstream in(runfiles);
    int r;
    std::string f;
    while (in >> r >> f)
    {
      runs.emplace_back(r, f);
    }
  }
  std::sort(runs.begin(), runs.end());
  // runs where no MB event reached the QA (e.g. all events removed by CaloStatusSkimmer): per-run plots only, no point in the summaries
  std::set<int> noevt;
  TH1 *ohsum = nullptr;
  for (auto &[run, fname] : runs)
  {
    if (fname == "-")
    {
      continue;
    }
    TFile *f = TFile::Open(fname.c_str());
    TH1 *h = (f && !f->IsZombie()) ? dynamic_cast<TH1 *>(f->Get("h_CDBCaloQA_evt")) : nullptr;
    if (h && h->GetBinContent(2) == 0)
    {
      noevt.insert(run);
    }
    TH1 *ho = (f && !f->IsZombie()) ? dynamic_cast<TH1 *>(f->Get("h_CDBCaloQA_ohcal_esum")) : nullptr;
    if (ho && !noevt.contains(run))
    {
      if (!ohsum)
      {
        ohsum = (TH1 *) ho->Clone("ohsum_period");
        ohsum->SetDirectory(nullptr);
      }
      else
      {
        ohsum->Add(ho);
      }
    }
    delete f;
  }
  std::cout << noevt.size() << " runs without MB events, not in the summaries" << std::endl;
  // ZDC scale of heavy ions: OHCal energy sum in the period's 30-70% band of MB events
  double ohlo = 0;
  double ohhi = 1e9;
  if (ohsum && ohsum->GetEntries() > 0)
  {
    double q[2] = {0.3, 0.7};
    double v[2];
    ohsum->GetQuantiles(2, v, q);
    ohlo = v[0];
    ohhi = v[1];
    std::cout << "ZDC: OHCal band " << ohlo << " - " << ohhi << " GeV" << std::endl;
  }
  const bool zhi = period.find("pp") == std::string::npos;
  // period sums: ZDC energy vs MBD t0 relative to the run's main peak (0.5 ns steps), bunch spectra, O+O peripheral spectra
  std::array<std::map<int, std::array<double, 3>>, 2> zt0;  // rel. bin -> sum E, entries, sum (n err)^2
  std::array<std::array<TH1 *, 3>, 2> zspec{};
  std::array<TH1 *, 2> zperiph{};

  // zsmap: only the maps of the mean ZSCrossCalib factor per ieta vs run (one bin per run), one per calorimeter, from the
  // per-ieta means of the per-run plots (<layer>_zsxcalib_ieta_0 in runs/run<N>.root): run after the per-run jobs
  if (zsmap)
  {
    g_tag = "";
    g_coll = runs.empty() ? "" : collisionLabel(runs.back().first);
    // maps with their run lists, for the 2D ratio of two global tags on the web page
    TFile *zf = TFile::Open(Form("%s/summary/zsmap.root", outdir.c_str()), "RECREATE");
    for (size_t l = 0; l < layers.size(); l++)
    {
      std::map<int, std::vector<double>> zsieta;
      for (auto &[run, fname] : runs)
      {
        if (noevt.contains(run))
        {
          continue;
        }
        TFile *rf = TFile::Open(Form("%s/runs/run%d.root", outdir.c_str(), run));
        if (!rf || rf->IsZombie())
        {
          continue;
        }
        if (TProfile *pz = dynamic_cast<TProfile *>(rf->Get((layers[l] + "_zsxcalib_ieta_0").c_str())))
        {
          std::vector<double> m(pz->GetNbinsX(), std::nan(""));
          for (int b = 1; b <= pz->GetNbinsX(); b++)
          {
            if (pz->GetBinEntries(b) > 0)
            {
              m[b - 1] = pz->GetBinContent(b);
            }
          }
          zsieta[run] = m;
        }
        rf->Close();
      }
      if (zsieta.empty())
      {
        continue;
      }
      const int neta = (l == 0) ? 96 : 24;
      const int nrun = zsieta.size();
      TH2F *hm = new TH2F(Form("%s_zsx_map", layers[l].c_str()), "", nrun, 0, nrun, neta, -0.5, neta - 0.5);
      const int step = std::max(1, (int) std::ceil(nrun / 20.));  // at most about 20 run labels
      double vlo = 1e9;
      double vhi = -1e9;
      int i = 0;
      for (auto &[r, m] : zsieta)
      {
        i++;
        if ((i - 1) % step == 0)
        {
          hm->GetXaxis()->SetBinLabel(i, Form("%d", r));
        }
        for (int ie = 0; ie < neta && ie < (int) m.size(); ie++)
        {
          if (std::isfinite(m[ie]))
          {
            hm->SetBinContent(i, ie + 1, m[ie]);
            vlo = std::min(vlo, m[ie]);
            vhi = std::max(vhi, m[ie]);
          }
        }
      }
      TCanvas *c = newCanvas(false);
      c->SetRightMargin(0.19);
      c->SetBottomMargin(0.17);
      hm->SetMinimum(vlo - 1e-6);  // bins without good towers stay empty
      hm->SetMaximum(vhi + 1e-6);
      hm->GetXaxis()->SetTitle("run number");
      hm->GetXaxis()->SetTitleOffset(1.9);
      hm->GetXaxis()->LabelsOption("v");
      hm->GetXaxis()->SetLabelSize(0.03);
      hm->GetYaxis()->SetTitle(Form("%s i#eta", layerlabel[l].c_str()));
      hm->GetZaxis()->SetTitle("mean ZSCrossCalib factor");
      hm->GetZaxis()->SetTitleOffset(1.6);
      hm->GetXaxis()->CenterTitle();
      hm->GetYaxis()->CenterTitle();
      hm->Draw("colz");
      label();
      save(c, outdir + "/summary/" + layers[l] + "_zsxcalib_map_vs_run.pdf");
      std::string rl;
      for (auto &[r, m] : zsieta)
      {
        rl += (rl.empty() ? "" : ",") + std::to_string(r);
      }
      zf->cd();
      TNamed((layers[l] + "_zsxcalib_map_meta").c_str(), ("{\"opts\": [\"colz\"], \"runs\": [" + rl + "]}").c_str()).Write();
      hm->Write((layers[l] + "_zsxcalib_map_0").c_str());
    }
    zf->Close();
    return;
  }

  // summary quantities vs run
  std::map<std::string, TGraph *> gr;
  bool skipsum = false;                      // run without MB events: no summary point
  std::map<std::string, TProfile *> tscsum;  // HCal per-tower profiles summed over the runs: <layer>_tsc_e, <layer>_tsc_occ
  auto addpt = [&gr, &skipsum](const std::string &key, int run, double val)
  {
    if (skipsum)
    {
      return;
    }
    if (!gr.contains(key))
    {
      gr[key] = new TGraph();
    }
    gr[key]->SetPoint(gr[key]->GetN(), run, val);
  };

  for (auto &[run, fname] : runs)
  {
    if (fname == "-")
    {
      continue;  // QA job of this run failed: no histograms, only the CDB status
    }
    TFile *f = TFile::Open(fname.c_str());
    if (!f || f->IsZombie())
    {
      std::cout << "cannot open " << fname << std::endl;
      continue;
    }
    g_tag = Form("Run %d", run);
    skipsum = noevt.contains(run);
    for (size_t l = 1; !skipsum && l < layers.size(); l++)
    {
      for (const std::string q : {"_tsc_e", "_tsc_occ"})
      {
        TProfile *p = dynamic_cast<TProfile *>(f->Get(("p_CDBCaloQA_" + layers[l] + q).c_str()));
        if (!p)
        {
          continue;
        }
        TProfile *&s = tscsum[layers[l] + q];
        if (s)
        {
          s->Add(p);
        }
        else
        {
          s = static_cast<TProfile *>(p->Clone(("sum_" + layers[l] + q).c_str()));
          s->SetDirectory(nullptr);
        }
      }
    }
    g_coll = collisionLabel(run);
    for (size_t t = 0; t < trigs.size(); t++)
    {
      TNamed *tn = dynamic_cast<TNamed *>(f->Get(("trig_" + trigs[t]).c_str()));
      g_trigname[t] = tn ? tn->GetTitle() : trigs[t];
    }
    const std::string mbname = g_trigname[0];
    const std::string pre = Form("%s/runs/run%d_", outdir.c_str(), run);
    g_objfile = runplots ? TFile::Open(Form("%s/runs/run%d.root", outdir.c_str(), run), "RECREATE") : nullptr;

    //------------------------ MBD ------------------------//
    TH2 *hq = get2(f, "mbd_q_pmt");
    TH2 *ht = get2(f, "mbd_t_pmt");
    std::vector<int> selpmt{0, 32, 64, 96};
    std::vector<int> selcol{kBlack, kBlue + 1, kRed + 1, kGreen + 2};
    {
      std::vector<TH1 *> hs;
      std::vector<std::string> nm;
      for (int p : selpmt)
      {
        hs.push_back(hq->ProjectionY(Form("q_pmt%d_%d", p, run), p + 1, p + 1)->Rebin(4));
        hs.back()->GetXaxis()->SetRangeUser(-0.5, 5);  // MIP peak region
        nm.emplace_back(Form("PMT %d", p));
      }
      overlay(hs, nm, selcol, "PMT charge, valid TDC", pre + "mbd_q_selpmt.pdf", true, mbname, false, {0., 1.});
      hs.clear();
      for (int p : selpmt)
      {
        hs.push_back(ht->ProjectionY(Form("t_pmt%d_%d", p, run), p + 1, p + 1)->Rebin(5));
      }
      overlay(hs, nm, selcol, "PMT time (ns), q > 0", pre + "mbd_t_selpmt.pdf", true, mbname);
    }
    TH1 *hqall = hq->ProjectionY(Form("q_all_%d", run));
    hqall->GetXaxis()->SetRangeUser(-0.5, 5);  // MIP peak region
    single(hqall, "PMT charge, all PMTs, valid TDC", pre + "mbd_q_allpmt.pdf", true, mbname, false, {0., 1.});
    TH1 *htall = ht->ProjectionY(Form("t_all_%d", run));
    single(htall, "PMT time (ns), all PMTs, q > 0", pre + "mbd_t_allpmt.pdf", true, mbname);
    overlay({get1(f, "mbd_qavg_south"), get1(f, "mbd_qavg_north")}, {"south", "north"}, {kBlue + 1, kRed + 1},
            "average PMT charge per event", pre + "mbd_qavg_ns.pdf", true, mbname);
    overlay({get1(f, "mbd_npmt_south"), get1(f, "mbd_npmt_north")}, {"south", "north"}, {kBlue + 1, kRed + 1},
            "N_{PMT} with q > 0.4", pre + "mbd_npmt_ns.pdf", false, mbname);
    TH1 *hz = get1(f, "mbd_zvtx");
    TH1 *ht0 = get1(f, "mbd_t0");
    double zmean = hz->GetMean();
    double zrms = hz->GetRMS();
    double t0mean = ht0->GetMean();
    single(hz, "MBD z-vertex (cm)", pre + "mbd_zvtx.pdf", false, mbname, false, {0.});
    single(ht0, "MBD t0 (ns)", pre + "mbd_t0.pdf", false, mbname, false, {0.});

    // charge > 0 part for the PMT mean and MIP peak
    TH1 *hqpos = hq->ProjectionY(Form("q_pos_%d", run));
    hqpos->GetXaxis()->SetRangeUser(0.01, hqpos->GetXaxis()->GetXmax());
    addpt("mbd_qmean", run, hqpos->GetMean());
    addpt("mbd_mip", run, mbdMipPeak(hq->ProjectionY(Form("q_mip_%d", run))));
    addpt("mbd_tmean", run, ht->ProjectionY(Form("t_m_%d", run))->GetMean());
    addpt("mbd_t0", run, t0mean);
    addpt("mbd_zvtx", run, zmean);
    addpt("mbd_zvtx_rms", run, zrms);
    addpt("mbd_qsum", run, 64 * (get1(f, "mbd_qavg_south")->GetMean() + get1(f, "mbd_qavg_north")->GetMean()));
    // mean charge sum per arm = 64 x mean average PMT charge of the arm
    for (const std::string arm : {"south", "north"})
    {
      addpt("mbd_qsum_" + arm + "_mean", run, 64 * get1(f, "mbd_qavg_" + arm)->GetMean());
    }

    //------------------------ ZDC (not in the simulation) ------------------------//
    TH1 *hzs = get1(f, "zdc_e_south");
    TH1 *hzn = get1(f, "zdc_e_north");
    TNamed *tzdc = dynamic_cast<TNamed *>(f->Get("trig_ZDC"));
    const std::string zdcname = tzdc ? tzdc->GetTitle() : mbname;
    // ZdcReco energy with the satellite correction: GeV; single-neutron peak in heavy ions only (none in p+p)
    const bool zdcfitu = period.find("AuAu") != std::string::npos || period.find("OO") != std::string::npos;
    const std::string zu = "GeV";
    // energy plots: 5 GeV bins, 0-750 GeV (one-neutron peak at 100 GeV)
    auto zdczoom = [](TH1 *h)
    {
      TH1 *z = (TH1 *) h->Clone();
      z->Rebin(2);
      z->GetXaxis()->SetRangeUser(0., 750.);
      return z;
    };
    if (hzs && hzn && hzs->GetEntries() > 0)
    {
      for (const std::string arm : {"south", "north"})
      {
        TH1 *h = (arm == "south") ? hzs : hzn;
        addpt("zdc_emean_" + arm, run, h->GetMean());
        std::vector<TH1 *> hm;
        for (int m = 1; m <= 3; m++)
        {
          hm.push_back(zdczoom(get1(f, Form("zdc_mod_e_%s_%d", arm.c_str(), m))));
        }
        std::string ztitle = "ZDC ";
        ztitle.append(arm).append(" compartment energy (").append(zu).append(")");
        std::string pdfname = pre;
        pdfname.append("zdc_mod_").append(arm).append(".pdf");
        overlay(hm, {"compartment 1", "compartment 2", "compartment 3"}, {kBlack, kBlue + 1, kRed + 1}, ztitle, pdfname, true, zdcname);
      }
      std::array<double, 2> p1n{};
      zdcRatio(zdczoom(hzs), zdczoom(hzn), "ZDC arm energy (" + zu + ")", pre + "zdc_e_ns.pdf", zdcname, zdcfitu, p1n);
      for (int a = 0; a < 2; a++)
      {
        if (std::isfinite(p1n[a]))
        {
          addpt(std::string("zdc_1n_") + (a ? "north" : "south"), run, p1n[a]);
        }
      }
    }
    {
      // p+p energy scale: mean arm energy of MB events above 12 GeV (Nagle's p+p closure, about 48 GeV)
      for (int a = 0; !zdcfitu && a < 2; a++)
      {
        TH1 *hmb = get1(f, a ? "zdc_e_north_mb" : "zdc_e_south_mb");
        if (hmb && hmb->GetEntries() > 0)
        {
          hmb->GetXaxis()->SetRange(hmb->FindBin(12.5), hmb->GetNbinsX());
          addpt(std::string("zdc_mbmean_") + (a ? "north" : "south"), run, hmb->GetMean());
        }
      }
      // ZDC as in its calibration: main bunch |t0 - peak| < 2.5 ns, satellites at -5.3 and +5.3 ns; heavy ions: OHCal sum in
      // the period's 30-70% band, G = main-bunch mean; p+p: E > 12 GeV
      for (int a = 0; !skipsum && a < 2; a++)
      {
        const std::string arm = a ? "north" : "south";
        TProfile2D *p2 = dynamic_cast<TProfile2D *>(f->Get(("p2_CDBCaloQA_zdc_t0_ohcal_" + arm).c_str()));
        TH2 *hzdct = get2(f, "zdc_e_t0_" + arm);
        if (!p2 || !hzdct || hzdct->GetEntries() == 0)
        {
          continue;
        }
        TH1D *cnt = hzdct->ProjectionX(Form("zt0c_%d_%d", run, a));
        const int pk = cnt->GetMaximumBin();
        const int j12 = hzdct->GetYaxis()->FindBin(12.5);
        // sum E, entries, (n err)^2 in t0 bin i
        auto tbin = [&](int i)
        {
          std::array<double, 3> r{0, 0, 0};
          if (zhi)
          {
            for (int j = 1; j <= p2->GetNbinsY(); j++)
            {
              const double o = p2->GetYaxis()->GetBinCenter(j);
              const int b = p2->GetBin(i, j);
              const double n = p2->GetBinEntries(b);
              if (o < ohlo || o > ohhi || n <= 0)
              {
                continue;
              }
              r[0] += p2->GetBinContent(b) * n;
              r[1] += n;
              r[2] += std::pow(n * p2->GetBinError(b), 2);
            }
          }
          else
          {
            double s2 = 0;
            for (int j = j12; j <= hzdct->GetNbinsY(); j++)
            {
              const double n = hzdct->GetBinContent(i, j);
              const double e = hzdct->GetYaxis()->GetBinCenter(j);
              r[0] += n * e;
              r[1] += n;
              s2 += n * e * e;
            }
            r[2] = r[1] > 1 ? (s2 - r[0] * r[0] / r[1]) : 0;  // n^2 err^2 = n var
          }
          return r;
        };
        auto wmean = [&](double dt)
        {
          double s = 0;
          double n = 0;
          for (int i = 1; i <= hzdct->GetNbinsX(); i++)
          {
            if (std::abs(hzdct->GetXaxis()->GetBinCenter(i) - cnt->GetBinCenter(pk) - dt) < 2.5)
            {
              const auto r = tbin(i);
              s += r[0];
              n += r[1];
            }
          }
          return n > 0 ? s / n : std::nan("");
        };
        const double m0 = wmean(0);
        const double mm = wmean(-5.3);
        const double mp = wmean(5.3);
        if (zhi && std::isfinite(m0))
        {
          addpt("zdc_g_" + arm, run, m0);
        }
        if (std::isfinite(m0) && std::isfinite(mm))
        {
          addpt("zdc_fm5_" + arm, run, mm / m0);
        }
        if (std::isfinite(m0) && std::isfinite(mp))
        {
          addpt("zdc_fp5_" + arm, run, mp / m0);
        }
        for (int i = 1; i <= hzdct->GetNbinsX(); i++)
        {
          const auto r = tbin(i);
          auto &z = zt0[a][i - pk];
          z[0] += r[0];
          z[1] += r[1];
          z[2] += r[2];
        }
        for (int w = 0; w < 3; w++)
        {
          const std::array<double, 3> wdt{0, -5.3, 5.3};
          const double dt = wdt[w];
          const int i0 = hzdct->GetXaxis()->FindBin(cnt->GetBinCenter(pk) + dt - 2.49);
          const int i1 = hzdct->GetXaxis()->FindBin(cnt->GetBinCenter(pk) + dt + 2.49);
          TH1 *py = hzdct->ProjectionY(Form("zsp_%d_%d_%d", run, a, w), i0, i1);
          if (!zspec[a][w])
          {
            zspec[a][w] = (TH1 *) py->Clone(Form("zspec_%d_%d", a, w));
            zspec[a][w]->SetDirectory(nullptr);
          }
          else
          {
            zspec[a][w]->Add(py);
          }
        }
        TH2 *ho = get2(f, "zdc_e_ohcal_" + arm);
        if (period.find("OO") != std::string::npos && ho)
        {
          TH1 *pp = ho->ProjectionY(Form("zpe_%d_%d", run, a), 1, ho->GetXaxis()->FindBin(1.99));
          if (!zperiph[a])
          {
            zperiph[a] = (TH1 *) pp->Clone(Form("zperiph_%d", a));
            zperiph[a]->SetDirectory(nullptr);
          }
          else
          {
            zperiph[a]->Add(pp);
          }
        }
      }
      // north vs south: MB events (older files: ZDC-triggered events)
      TH2 *h2mb = get2(f, "zdc_e_ns_mb");
      const bool mb2d = h2mb && h2mb->GetEntries() > 0;
      if (mb2d || (hzs && hzs->GetEntries() > 0))
      {
        draw2d(mb2d ? h2mb : get2(f, "zdc_e_ns"), "ZDC south energy (" + zu + ")", "ZDC north energy (" + zu + ")", pre + "zdc_e_2d.pdf",
               mb2d ? mbname : zdcname);
      }
    }

    //------------------------ sEPD ------------------------//
    TH2 *hsq = get2(f, "sepd_q_ch");
    if (hsq && hsq->GetEntries() > 0)
    {
      // selected tiles: rings 0 and 15 of each arm, tile 0
      std::vector<TH1 *> hs;
      std::vector<std::string> nm;
      for (const auto &[arm, ring] : std::vector<std::pair<int, int>>{{0, 0}, {0, 15}, {1, 0}, {1, 15}})
      {
        const int ch = TowerInfoDefs::decode_epd(TowerInfoDefs::encode_epd(arm, ring, 0));
        hs.push_back(hsq->ProjectionY(Form("sepd_q_ch%d_%d", ch, run), ch + 1, ch + 1));
        hs.back()->GetXaxis()->SetRangeUser(-0.5, 5);  // MIP peak region
        nm.emplace_back(Form("%s ring %d tile 0", arm ? "north" : "south", ring));
      }
      overlay(hs, nm, selcol, "sEPD tile charge (nMIP)", pre + "sepd_q_seltile.pdf", true, mbname, false, {0., 1.});
      TH1 *hqt = hsq->ProjectionY(Form("sepd_q_all_%d", run));
      hqt->GetXaxis()->SetRangeUser(-0.5, 5);
      single(hqt, "sEPD tile charge, all tiles (nMIP)", pre + "sepd_q_alltile.pdf", true, mbname, false, {0., 1.});
      // per tile: MIP peak (most probable charge in 0.5-2.5 nMIP, at least 50 entries there), mean charge per event,
      // fraction of events with q > 0.5 nMIP
      const int nch = hsq->GetNbinsX();
      std::vector<double> mip(nch, std::nan(""));
      std::vector<double> qmean(nch, std::nan(""));
      std::vector<double> hitfrac(nch, std::nan(""));
      std::array<double, 2> mipsum{0, 0};
      std::array<int, 2> nmip{0, 0};
      for (int ch = 0; ch < nch; ch++)
      {
        TH1D *p = hsq->ProjectionY(Form("sepd_p%d_%d", ch, run), ch + 1, ch + 1);
        const double n = p->Integral(0, p->GetNbinsX() + 1);
        if (n > 0)
        {
          qmean[ch] = p->GetMean();
          hitfrac[ch] = p->Integral(p->FindBin(0.5 + 1e-6), p->GetNbinsX() + 1) / n;
          if (p->Integral(p->FindBin(0.5), p->FindBin(2.5)) >= 50)
          {
            mip[ch] = sepdMipPeak(p);
            const int arm = TowerInfoDefs::get_epd_arm(TowerInfoDefs::encode_epd(ch));
            mipsum[arm] += mip[ch];
            nmip[arm]++;
          }
        }
        delete p;
      }
      epdMaps(mip, "MIP peak (nMIP)", pre + "sepd_mip_map.pdf", mbname, 0.8, 1.2);
      epdMaps(qmean, "mean charge per event (nMIP)", pre + "sepd_qmean_map.pdf", mbname);
      epdMaps(hitfrac, "fraction of events with q > 0.5 nMIP", pre + "sepd_hit_map.pdf", mbname);
      for (int a = 0; a < 2; a++)
      {
        const std::string arm = a ? "north" : "south";
        if (nmip[a] > 0)
        {
          addpt("sepd_mip_" + arm, run, mipsum[a] / nmip[a]);
        }
        addpt("sepd_qsum_mean_" + arm, run, get1(f, "sepd_qsum_" + arm)->GetMean());
        addpt("sepd_nhit_mean_" + arm, run, get1(f, "sepd_nhit_" + arm)->GetMean());
      }
      overlay({get1(f, "sepd_nhit_south"), get1(f, "sepd_nhit_north")}, {"south", "north"}, {kBlue + 1, kRed + 1},
              "sEPD tiles with q > 0.5 nMIP per event", pre + "sepd_nhit_ns.pdf", true, mbname);
      overlay({get1(f, "sepd_qsum_south"), get1(f, "sepd_qsum_north")}, {"south", "north"}, {kBlue + 1, kRed + 1},
              "sEPD charge sum per event (nMIP)", pre + "sepd_qsum_ns.pdf", true, mbname);
      draw2d(get2(f, "sepd_qsum_ns"), "sEPD south charge sum (nMIP)", "sEPD north charge sum (nMIP)", pre + "sepd_qsum_2d.pdf", mbname);
    }

    //------------------------ calorimeters ------------------------//
    for (size_t l = 0; l < layers.size(); l++)
    {
      const std::string &L = layers[l];
      const std::string &LL = layerlabel[l];
      TH1 *he = get1(f, L + "_e");
      TH1 *hn = get1(f, L + "_e_noZSXCalib");
      const double towsum = get1(f, L + "_esum")->GetMean();
      overlay({(TH1 *) he->Clone(), (TH1 *) hn->Clone()}, {"with ZSCrossCalib", "no ZSCrossCalib"}, {kBlack, kRed + 1},
              LL + " tower energy (GeV)", pre + L + "_e.pdf", true, mbname, false, {0.});
      // ZSCrossCalib factors of the CDB file this run used: good towers per ieta (colz) and mean per ieta (points)
      TNamed *zsurl = runplots ? dynamic_cast<TNamed *>(f->Get(("cdb_" + zsxcalibname[l]).c_str())) : nullptr;
      if (runplots && !zsurl)
      {
        zsurl = dynamic_cast<TNamed *>(f->Get(("cdb_" + zsxcalibname[l] + "_default").c_str()));  // tag without a payload for this run
      }
      if (zsurl)
      {
        CDBTTree zst(zsurl->GetTitle());
        zst.LoadCalibrations();
        // bad towers as in CaloTowerStatus: BadTowerMap status > 0, fracBadChi2 > 0.01, GlobalBadTowerMap status > 0
        auto cdbOf = [f](const std::string &payload) -> CDBTTree *
        {
          TNamed *u = dynamic_cast<TNamed *>(f->Get(("cdb_" + payload).c_str()));
          if (!u)
          {
            u = dynamic_cast<TNamed *>(f->Get(("cdb_" + payload + "_default").c_str()));
          }
          if (!u)
          {
            return nullptr;
          }
          CDBTTree *t = new CDBTTree(u->GetTitle());
          t->LoadCalibrations();
          return t;
        };
        CDBTTree *badmap = cdbOf(calodet[l] + "_BadTowerMap");
        CDBTTree *chi2map = cdbOf(calodet[l] + "_hotTowers_fracBadChi2");
        CDBTTree *globmap = cdbOf(calodet[l] + "_GlobalBadTowerMap");
        const int neta = (l == 0) ? 96 : 24;
        const int nphi = (l == 0) ? 256 : 64;
        std::vector<std::pair<int, double>> vals;
        double vmax = 1;
        for (int ie = 0; ie < neta; ie++)
        {
          for (int ip = 0; ip < nphi; ip++)
          {
            const unsigned int key = (l == 0) ? TowerInfoDefs::encode_emcal(ie, ip) : TowerInfoDefs::encode_hcal(ie, ip);
            if ((badmap && badmap->GetIntValue(key, "status") > 0) || (chi2map && chi2map->GetFloatValue(key, "fraction") > 0.01) ||
                (globmap && globmap->GetIntValue(key, "status") > 0))
            {
              continue;  // bad tower
            }
            const float v = zst.GetFloatValue(key, "ratio");
            if (!std::isfinite(v))
            {
              continue;
            }
            vals.emplace_back(ie, v);
            vmax = std::max(vmax, (double) v);
          }
        }
        delete badmap;
        delete chi2map;
        delete globmap;
        // fixed y range; towers above or below it are counted in the text line
        const double ylo = 0.5;
        const double yhi = 1.2;
        TH2F *hzsx = new TH2F(Form("%s_zsx_%d", L.c_str(), run), "", neta, -0.5, neta - 0.5, 150, ylo, yhi);
        TProfile *pz = new TProfile(Form("%s_zsx_prof_%d", L.c_str(), run), "", neta, -0.5, neta - 0.5);
        int nhi = 0;
        int nlo = 0;
        for (auto &[ie, v] : vals)
        {
          hzsx->Fill(ie, v);
          pz->Fill(ie, v);
          nhi += (v > yhi);
          nlo += (v < ylo);
        }
        std::string outtxt;
        if (nhi > 0)
        {
          outtxt += Form("%d tower%s with factor > %.1f (up to %.1f)", nhi, nhi > 1 ? "s" : "", yhi, vmax);
        }
        if (nlo > 0)
        {
          outtxt += Form("%s%d tower%s with factor < %.1f", outtxt.empty() ? "" : ", ", nlo, nlo > 1 ? "s" : "", ylo);
        }
        TCanvas *c = newCanvas(false);
        c->SetRightMargin(0.13);
        c->SetLogz();
        hzsx->GetXaxis()->SetTitle(Form("%s i#eta", LL.c_str()));
        hzsx->GetYaxis()->SetTitle("ZSCrossCalib factor");
        hzsx->GetZaxis()->SetTitle("towers");
        hzsx->GetXaxis()->CenterTitle();
        hzsx->GetYaxis()->CenterTitle();
        hzsx->Draw("colz");
        pz->SetMarkerStyle(20);
        pz->SetMarkerSize(0.6);
        pz->SetMarkerColor(kRed + 1);
        pz->SetLineColor(kRed + 1);
        pz->Draw("E same");
        refline(1., false);
        label();
        if (!outtxt.empty())
        {
          drawText(outtxt.c_str(), 0.18, 0.85, kBlack, 13);
        }
        keep(pre + L + "_zsxcalib_ieta.pdf", {pz}, {"e"}, {}, false);
        save(c, pre + L + "_zsxcalib_ieta.pdf");
      }
      TH1 *hocc = get1(f, L + "_occ");
      TH1 *hnz = get1(f, L + "_nonzs_frac");
      TH1 *htime = get1(f, L + "_time");
      double occmean = dynamic_cast<TProfile *>(f->Get(("p_CDBCaloQA_" + L + "_occ_mean").c_str()))->GetBinContent(1);
      double timemean = htime->GetMean();
      single(hocc, LL + " fraction of good towers above threshold, per event", pre + L + "_occ.pdf", true, mbname + ", E_{tower} > 0.1 GeV");
      single(hnz, LL + " fraction of non-ZS towers", pre + L + "_nonzs_frac.pdf", true, mbname);
      single(htime, LL + " tower time (samples)", pre + L + "_time.pdf", true, mbname + ", above threshold");
      single(get1(f, L + "_time_mbdt0"), LL + " tower time - MBD t0 (ns)", pre + L + "_time_mbdt0.pdf", true, mbname + ", above threshold",
             false, {0.});

      // noise: MB trigger, MBD |vz| < 100 cm, all good towers
      {
        TH1 *hf = get1(f, L + "_noise_e_fine");
        TH1 *hr = hf->Rebin(NOISE_REBIN[l], Form("%s_noise_rebin_%d", L.c_str(), run));
        TH1D *hm = mirror_negative(hr, Form("%s_noise_mirror_%d", L.c_str(), run));
        auto res = fitcore(hm, NOISE_RMSWIN[l]);
        addpt(L + "_noise", run, 1e3 * res[1]);

        TCanvas *c = newCanvas(true);
        TH1D *hd = (TH1D *) hm->Clone(Form("%s_noise_draw_%d", L.c_str(), run));
        hd->Scale(1. / hd->Integral(), "width");
        SetHistStyle(hd, 0, 0);
        hd->SetMarkerSize(0.7);
        const double xview = std::min(12 * res[1], 0.249);
        hd->GetXaxis()->SetRangeUser(-xview, xview);
        hd->SetMaximum(50 * hd->GetMaximum());
        hd->SetMinimum(1e-4 * hd->GetMaximum());
        hd->GetXaxis()->SetTitle((LL + " tower energy (GeV)").c_str());
        hd->GetYaxis()->SetTitle("Normalized counts");
        hd->Draw("E");
        TF1 *fd = new TF1(Form("%s_noise_fit_%d", L.c_str(), run), "gaus", -2 * res[1], 2 * res[1]);
        fd->SetParameters(1. / (std::sqrt(2 * M_PI) * res[1]), res[0], res[1]);
        fd->SetLineColor(kRed + 1);
        fd->Draw("same");
        label(mbname + ", |v_{z}| < 100 cm");
        drawText("E < 0 mirrored", 0.18, 0.80, kBlack, 14);
        drawText(Form("#sigma = %.2f #pm %.2f MeV", 1e3 * res[1], 1e3 * res[3]), 0.60, 0.80, kRed + 1, 16);
        keep(pre + L + "_noise.pdf", {hd, fd}, {"e", "l"}, {}, true);
        save(c, pre + L + "_noise.pdf");

        TH2 *h2 = get2(f, L + "_noise_e_fine_ieta");
        TGraphErrors *gs = new TGraphErrors();
        for (int ie = 0; runplots && ie < h2->GetNbinsX(); ie++)
        {
          TH1D *hp = h2->ProjectionY(Form("%s_py_%d_%d", L.c_str(), run, ie), ie + 1, ie + 1);
          if (hp->GetEntries() < 500)
          {
            delete hp;
            continue;
          }
          hp->Rebin(NOISE_REBIN[l]);
          TH1D *hmi = mirror_negative(hp, Form("%s_hm_%d_%d", L.c_str(), run, ie));
          auto ri = fitcore(hmi, NOISE_RMSWIN[l]);
          int np = gs->GetN();
          gs->SetPoint(np, ie, 1e3 * ri[1]);
          gs->SetPointError(np, 0, 1e3 * ri[3]);
          delete hp;
          delete hmi;
        }
        TCanvas *cs = newCanvas(false);
        SetGraphStyle(gs, 0, 0);
        gs->SetTitle(";i#eta;#sigma_{noise} [MeV]");
        gs->SetMinimum(0);
        gs->SetMaximum(NOISE_SIGYMAX[l]);
        gs->GetXaxis()->SetLimits(-1, h2->GetNbinsX());
        gs->Draw("AP");
        std::string ltext = LL;
        ltext.append(", ").append(mbname).append(", |v_{z}| < 100 cm");
        label(ltext);
        keep(pre + L + "_noise_ieta.pdf", {gs}, {"p"}, {}, false);
        save(cs, pre + L + "_noise_ieta.pdf");
      }
      addpt(L + "_occ", run, occmean);
      addpt(L + "_time", run, timemean);
      addpt(L + "_towsum", run, towsum);
    }

    //------------------------ clusters, pi0, jets ------------------------//
    {
      // jet eta above 5 GeV (p+p) or 10 GeV (Au+Au, O+O)
      const double jetetapt = (period.find("pp") != std::string::npos) ? 5. : 10.;
      // only trigger categories that are set up for this period (Au+Au: MB only)
      std::vector<TH1 *> he;
      std::vector<TH1 *> heta;
      std::vector<TH1 *> hj;
      std::vector<TH1 *> hjeta;
      std::vector<std::string> tnames;
      std::vector<int> tcols;
      for (size_t it = 0; it < trigs.size(); it++)
      {
        const std::string &t = trigs[it];
        if (it > 0 && (g_trigname[it].empty() || get2(f, "clus_e_eta_" + t)->GetEntries() == 0))
        {
          continue;
        }
        tnames.push_back(g_trigname[it]);
        tcols.push_back(trigcolor[it]);
        TH2 *hc = get2(f, "clus_e_eta_" + t);
        he.push_back(hc->ProjectionX(Form("clusE_%s_%d", t.c_str(), run)));
        heta.push_back(hc->ProjectionY(Form("clusEta_%s_%d", t.c_str(), run)));
        TH2 *hjt = get2(f, "jet_pt_eta_" + t);
        hj.push_back(hjt->ProjectionX(Form("jetPt_%s_%d", t.c_str(), run)));
        hjeta.push_back(hjt->ProjectionY(Form("jetEta_%s_%d", t.c_str(), run), hjt->GetXaxis()->FindBin(jetetapt + 1e-3), hjt->GetNbinsX()));
      }
      overlay(he, tnames, tcols, "EMCal cluster E_{core} (GeV)", pre + "clus_e.pdf", true);
      overlay(heta, tnames, tcols, "EMCal cluster #eta", pre + "clus_eta.pdf", false);
      overlay(hj, tnames, tcols, "R=0.4 jet p_{T} (GeV)", pre + "jet_pt.pdf", true);
      overlay(hjeta, tnames, tcols, "R=0.4 jet #eta", pre + "jet_eta.pdf", false, Form("p_{T} > %.0f GeV", jetetapt));
    }
    // centrality (Au+Au, O+O; not in the simulation, which has no ZDC)
    TH1 *hc = (mbname.rfind("MC", 0) == 0) ? nullptr : get1(f, "cent");
    if (hc)
    {
      hc->Rebin(5);  // 5% intervals
      // fractions of the MB events (MB event selection) in 0-5% and 75-80%, compared with the reference run below
      if (hc->Integral() > 0)
      {
        addpt("cent_frac_0_5", run, hc->Integral(hc->FindBin(0.5), hc->FindBin(4.5)) / hc->Integral());
        addpt("cent_frac_75_80", run, hc->Integral(hc->FindBin(75.5), hc->FindBin(79.5)) / hc->Integral());
        addpt("cent_n", run, hc->Integral());  // for the binomial errors of the fractions
      }
      single(hc, "centrality (%)", pre + "cent.pdf", false, mbname + ", MB event selection");
      single(get1(f, "cent_mbd_qsum"), "MBD charge sum (N+S)", pre + "cent_mbd_qsum.pdf", true, mbname + ", MB event selection");
    }
    {
      TH1 *hm = get1(f, "pi0_mass");
      TCanvas *c = newCanvas(false);
      handsomeTH1(hm, kBlack, 0.8);
      hm->GetXaxis()->SetTitle("M_{#gamma#gamma} (GeV)");
      hm->GetYaxis()->SetTitle("pairs");
      hm->SetMinimum(0);
      hm->SetMaximum(hm->GetMaximum() > 0 ? headroom(0, hm->GetMaximum(), false, 0.85) : 1);
      hm->Draw("pe");
      label(mbname);
      keep(pre + "pi0_mass.pdf", {hm}, {"e"}, {}, false);
      save(c, pre + "pi0_mass.pdf");
    }
    if (g_objfile)
    {
      g_objfile->Close();
      g_objfile = nullptr;
    }
    f->Close();
  }

  if (!summary)
  {
    return;  // per-run plots of one chunk of runs only (parallel pass 2)
  }

  //------------------------ summary vs run ------------------------//
  g_tag = "";  // summary plots: collision system only (set from the last run)
  g_objfile = TFile::Open((outdir + "/summary/summary.root").c_str(), "RECREATE");
  // hline: dashed horizontal reference line (NaN: none); extra: text line under the label
  auto drawgr = [&gr, &outdir](const std::string &key, const std::string &ytitle, double hline = std::nan(""),
                               const std::string &extra = "")
  {
    if (!gr.contains(key))
    {
      return;
    }
    TGraph *g = gr[key];
    TCanvas *c = newCanvas(false);
    c->SetRightMargin(0.04);
    g->SetMarkerStyle(20);
    g->SetMarkerSize(0.8);
    g->GetXaxis()->SetTitle("run number");
    g->GetYaxis()->SetTitle(ytitle.c_str());
    g->GetXaxis()->CenterTitle();
    g->GetYaxis()->CenterTitle();
    g->GetXaxis()->SetNoExponent();
    g->GetXaxis()->SetNdivisions(505);
    double xlo = g->GetPointX(0);
    double xhi = g->GetPointX(g->GetN() - 1);
    g->GetXaxis()->SetLimits(xlo - 5 - 0.02 * (xhi - xlo), xhi + 5 + 0.02 * (xhi - xlo));
    g->Draw("AP");
    if (std::isfinite(hline))
    {
      refline(hline, false);
    }
    label(extra.empty() ? std::string("MB trigger") : "MB trigger, " + extra);
    keep(outdir + "/summary/" + key + "_vs_run.pdf", {g}, {"p"}, {}, false);
    save(c, outdir + "/summary/" + key + "_vs_run.pdf");
  };
  drawgr("mbd_qmean", "MBD mean PMT charge (q > 0)");
  drawgr("mbd_tmean", "MBD mean PMT time (ns)");
  drawgr("mbd_mip", "MBD MIP peak", 1.);
  drawgr("mbd_t0", "MBD mean t0 (ns)", 0.);
  drawgr("mbd_zvtx", "MBD mean z-vertex (cm)", 0.);
  drawgr("mbd_zvtx_rms", "MBD z-vertex RMS (cm)");
  drawgr("mbd_qsum", "MBD mean charge sum");
  // centrality fractions relative to the sum of all runs of the period (one run off can not move the others)
  if (gr.contains("cent_frac_0_5"))
  {
    const std::vector<std::string> keys{"cent_frac_0_5", "cent_frac_75_80"};
    const std::vector<std::string> names{"0-5%", "75-80%"};
    const std::vector<int> cols{kBlue + 1, kRed + 1};
    std::vector<TGraph *> rg;
    double nref = 0;
    for (int i = 0; i < gr["cent_n"]->GetN(); i++)
    {
      nref += gr["cent_n"]->GetPointY(i);
    }
    for (const auto &k : keys)
    {
      // fraction of the sum of all runs
      double ref = 0;
      for (int i = 0; i < gr[k]->GetN(); i++)
      {
        ref += gr[k]->GetPointY(i) * gr["cent_n"]->GetPointY(i) / nref;
      }
      if (ref <= 0)
      {
        std::cout << "no centrality: no centrality ratio plot" << std::endl;
        rg.clear();
        break;
      }
      // error: binomial errors of the run and of the sum of all runs, added in quadrature
      TGraphErrors *g = new TGraphErrors();
      for (int i = 0; i < gr[k]->GetN(); i++)
      {
        const double v = gr[k]->GetPointY(i);
        const double n = gr["cent_n"]->GetPointY(i);
        const double rel = std::hypot(std::sqrt(v * (1 - v) / n) / v, std::sqrt(ref * (1 - ref) / nref) / ref);
        g->SetPoint(i, gr[k]->GetPointX(i), v / ref);
        g->SetPointError(i, 0, rel * v / ref);
      }
      rg.push_back(g);
    }
    if (!rg.empty())
    {
      TCanvas *c = newCanvas(false);
      c->SetRightMargin(0.04);
      double xlo = rg[0]->GetPointX(0);
      double xhi = rg[0]->GetPointX(rg[0]->GetN() - 1);
      TH1F *fr = new TH1F("fr_centref", ";run number;fraction of MB events / all runs", 100,
                          xlo - 5 - 0.02 * (xhi - xlo), xhi + 5 + 0.02 * (xhi - xlo));
      double ymin = 1;
      double ymax = 1;
      for (auto *g : rg)
      {
        for (int i = 0; i < g->GetN(); i++)
        {
          ymin = std::min(ymin, g->GetPointY(i) - g->GetErrorY(i));
          ymax = std::max(ymax, g->GetPointY(i) + g->GetErrorY(i));
        }
      }
      const double pad = 0.1 * (ymax - ymin + 0.02);
      fr->SetMinimum(ymin - pad);
      fr->SetMaximum(ymin - pad + (ymax - ymin + 2 * pad) / 0.75);
      fr->GetXaxis()->CenterTitle();
      fr->GetYaxis()->CenterTitle();
      fr->GetXaxis()->SetNoExponent();
      fr->GetXaxis()->SetNdivisions(505);
      fr->Draw("AXIS");
      TLegend *leg = new TLegend(0.18, 0.78, 0.60, 0.86);
      easyLeg(leg);
      leg->SetTextFont(43);
      leg->SetTextSize(14);
      leg->SetNColumns(2);
      for (size_t k = 0; k < rg.size(); k++)
      {
        rg[k]->SetMarkerStyle(20);
        rg[k]->SetMarkerSize(0.8);
        rg[k]->SetMarkerColor(cols[k]);
        rg[k]->SetLineColor(cols[k]);
        rg[k]->Draw("P same");
        leg->AddEntry(rg[k], names[k].c_str(), "pe");
      }
      refline(1., false);
      leg->Draw();
      label("MB trigger");
      keep(outdir + "/summary/cent_frac_ref_vs_run.pdf", {rg[0], rg[1]}, {"p", "p"}, names, false);
      save(c, outdir + "/summary/cent_frac_ref_vs_run.pdf");
    }
  }
  // mean charge sum per event, south (blue) and north (red)
  if (gr.contains("mbd_qsum_south_mean"))
  {
    TCanvas *c = newCanvas(false);
    c->SetRightMargin(0.04);
    const std::vector<std::string> keys{"mbd_qsum_south_mean", "mbd_qsum_north_mean"};
    const std::vector<std::string> names{"south", "north"};
    const std::vector<int> cols{kBlue + 1, kRed + 1};
    const std::vector<int> mstyle{20, 20};
    double ymax = 0;
    for (const auto &k : keys)
    {
      for (int i = 0; i < gr[k]->GetN(); i++)
      {
        ymax = std::max(ymax, gr[k]->GetPointY(i));
      }
    }
    TGraph *g0 = gr[keys[0]];
    double xlo = g0->GetPointX(0);
    double xhi = g0->GetPointX(g0->GetN() - 1);
    TH1F *fr = new TH1F("fr_qsum_ns", ";run number;MBD mean charge sum per event", 100, xlo - 5 - 0.02 * (xhi - xlo), xhi + 5 + 0.02 * (xhi - xlo));
    fr->SetMinimum(0);
    fr->SetMaximum(ymax > 0 ? headroom(0, ymax, false, 0.72) : 1);
    fr->GetXaxis()->CenterTitle();
    fr->GetYaxis()->CenterTitle();
    fr->GetXaxis()->SetNoExponent();
    fr->GetXaxis()->SetNdivisions(505);
    fr->Draw("AXIS");
    TLegend *leg = new TLegend(0.18, 0.78, 0.60, 0.86);
    easyLeg(leg);
    leg->SetTextFont(43);
    leg->SetTextSize(14);
    leg->SetNColumns(2);
    for (size_t k = 0; k < keys.size(); k++)
    {
      TGraph *g = gr[keys[k]];
      g->SetMarkerStyle(mstyle[k]);
      g->SetMarkerSize(0.8);
      g->SetMarkerColor(cols[k]);
      g->SetLineColor(cols[k]);
      g->Draw("P same");
      leg->AddEntry(g, names[k].c_str(), "p");
    }
    leg->Draw();
    label("MB trigger");
    std::vector<TObject *> ko;
    for (const auto &k : keys)
    {
      // the PDF takes its axis titles from the frame fr; the saved graphs need them too for the web overlay
      gr[k]->GetHistogram()->GetXaxis()->SetTitle(fr->GetXaxis()->GetTitle());
      gr[k]->GetHistogram()->GetYaxis()->SetTitle(fr->GetYaxis()->GetTitle());
      ko.push_back(gr[k]);
    }
    keep(outdir + "/summary/mbd_qsum_ns_vs_run.pdf", ko, std::vector<std::string>(keys.size(), "p"), names, false);
    save(c, outdir + "/summary/mbd_qsum_ns_vs_run.pdf");
  }
  // south (blue) and north (red) of one quantity vs run: graphs <key>_south and <key>_north -> summary/<key>_ns_vs_run.pdf
  auto drawns = [&gr, &outdir](const std::string &key, const std::string &ytitle, double ylo = 0, double yhi = -1)
  {
    const std::vector<std::string> keys{key + "_south", key + "_north"};
    if (!gr.contains(keys[0]) || !gr.contains(keys[1]))
    {
      return;
    }
    const std::vector<std::string> names{"south", "north"};
    const std::vector<int> cols{kBlue + 1, kRed + 1};
    double ymax = 0;
    double xlo = 1e9;
    double xhi = -1e9;
    for (const auto &k : keys)
    {
      for (int i = 0; i < gr[k]->GetN(); i++)
      {
        ymax = std::max(ymax, gr[k]->GetPointY(i));
        xlo = std::min(xlo, gr[k]->GetPointX(i));
        xhi = std::max(xhi, gr[k]->GetPointX(i));
      }
    }
    TCanvas *c = newCanvas(false);
    c->SetRightMargin(0.04);
    TH1F *fr = new TH1F(("fr_" + key).c_str(), (";run number;" + ytitle).c_str(), 100, xlo - 5 - 0.02 * (xhi - xlo), xhi + 5 + 0.02 * (xhi - xlo));
    fr->SetMinimum(yhi > ylo ? ylo : 0);
    const double frmax = (ymax > 0) ? headroom(0, ymax, false, 0.72) : 1;
    fr->SetMaximum(yhi > ylo ? yhi : frmax);
    fr->GetXaxis()->CenterTitle();
    fr->GetYaxis()->CenterTitle();
    fr->GetXaxis()->SetNoExponent();
    fr->GetXaxis()->SetNdivisions(505);
    fr->Draw("AXIS");
    TLegend *leg = new TLegend(0.18, 0.78, 0.60, 0.86);
    easyLeg(leg);
    leg->SetTextFont(43);
    leg->SetTextSize(14);
    leg->SetNColumns(2);
    std::vector<TObject *> ko;
    for (size_t k = 0; k < keys.size(); k++)
    {
      TGraph *g = gr[keys[k]];
      g->SetMarkerStyle(20);
      g->SetMarkerSize(0.8);
      g->SetMarkerColor(cols[k]);
      g->SetLineColor(cols[k]);
      g->Draw("P same");
      leg->AddEntry(g, names[k].c_str(), "p");
      // the PDF takes its axis titles from the frame fr; the saved graphs need them too for the web overlay
      g->GetHistogram()->GetXaxis()->SetTitle(fr->GetXaxis()->GetTitle());
      g->GetHistogram()->GetYaxis()->SetTitle(fr->GetYaxis()->GetTitle());
      ko.push_back(g);
    }
    leg->Draw();
    label("MB trigger");
    keep(outdir + "/summary/" + key + "_ns_vs_run.pdf", ko, std::vector<std::string>(keys.size(), "p"), names, false);
    save(c, outdir + "/summary/" + key + "_ns_vs_run.pdf");
  };
  drawns("zdc_1n", "ZDC one-neutron peak (GeV)");
  drawns("zdc_emean", "ZDC mean arm energy, E > 0 (GeV)");
  drawns("zdc_mbmean", "ZDC MB mean arm energy, E > 12 GeV (GeV)");
  for (const std::string arm : {"south", "north"})
  {
    auto it = gr.find("zdc_g_" + arm);
    if (it == gr.end() || it->second->GetN() == 0)
    {
      continue;
    }
    std::vector<double> y(it->second->GetY(), it->second->GetY() + it->second->GetN());
    std::nth_element(y.begin(), y.begin() + y.size() / 2, y.end());
    const double med = y[y.size() / 2];
    for (int i = 0; i < it->second->GetN(); i++)
    {
      it->second->SetPointY(i, it->second->GetPointY(i) / med);
    }
  }
  drawns("zdc_g", Form("ZDC main-bunch mean, OHCal %.0f-%.0f GeV / median", ohlo, ohhi), 0.8, 1.3);
  drawns("zdc_fm5", "ZDC -5 ns bunch / main bunch", 0.6, 1.5);
  drawns("zdc_fp5", "ZDC +5 ns bunch / main bunch", 0.6, 1.5);
  const std::string zsel = zhi ? Form("MB |z| < 10 cm, OHCal %.0f-%.0f GeV", ohlo, ohhi) : "MB, E > 12 GeV";
  // <ZDC> / <ZDC main bunch> vs MBD t0 - main peak, all runs
  if (!zt0[0].empty())
  {
    TCanvas *c = newCanvas(false);
    c->SetRightMargin(0.04);
    TH1F *fr = new TH1F("fr_zdct0", ";MBD t0 - main peak (ns);<ZDC> / <ZDC main bunch>", 100, -8, 8);
    fr->SetMinimum(0.8);
    fr->SetMaximum(1.3);
    fr->GetXaxis()->CenterTitle();
    fr->GetYaxis()->CenterTitle();
    fr->Draw("AXIS");
    TLegend *leg = new TLegend(0.18, 0.78, 0.60, 0.86);
    easyLeg(leg);
    leg->SetTextFont(43);
    leg->SetTextSize(14);
    leg->SetNColumns(2);
    std::vector<TObject *> ko;
    for (int a = 0; a < 2; a++)
    {
      double s0 = 0;
      double n0 = 0;
      for (auto &[k, z] : zt0[a])
      {
        if (std::abs(k * 0.5) < 2.5)
        {
          s0 += z[0];
          n0 += z[1];
        }
      }
      TH1D *h = new TH1D(Form("zdct0_%d", a), ";MBD t0 - main peak (ns);<ZDC> / <ZDC main bunch>", 32, -8, 8);
      for (auto &[k, z] : zt0[a])
      {
        const int b = h->FindBin(k * 0.5 + 0.01);
        if (z[1] > 20 && n0 > 0 && b >= 1 && b <= 32)
        {
          h->SetBinContent(b, z[0] / z[1] / (s0 / n0));
          h->SetBinError(b, std::sqrt(z[2]) / z[1] / (s0 / n0));
        }
      }
      h->SetMarkerStyle(20);
      h->SetMarkerSize(0.8);
      h->SetMarkerColor(a ? kRed + 1 : kBlue + 1);
      h->SetLineColor(a ? kRed + 1 : kBlue + 1);
      h->Draw("e same");
      leg->AddEntry(h, a ? "north" : "south", "p");
      ko.push_back(h);
    }
    refline(1., false);
    leg->Draw();
    label(zsel);
    keep(outdir + "/summary/zdc_t0prof_vs_run.pdf", ko, {"e", "e"}, {"south", "north"}, false);
    save(c, outdir + "/summary/zdc_t0prof_vs_run.pdf");
  }
  // spectra of the three bunches, all runs, each normalized to unit area
  for (int a = 0; a < 2 && zspec[a][0]; a++)
  {
    const std::string arm = a ? "north" : "south";
    std::vector<TH1 *> hs;
    for (int w = 0; w < 3; w++)
    {
      TH1 *h = zspec[a][w];
      h->Rebin(2);
      if (h->Integral() > 0)
      {
        h->Scale(1. / h->Integral());
      }
      h->GetXaxis()->SetRangeUser(0., zhi ? 750. : 300.);
      hs.push_back(h);
    }
    std::string pdfname = outdir;
    pdfname.append("/summary/zdc_bunchspec_").append(arm).append("_vs_run.pdf");
    overlay(hs, {"main bunch", "-5 ns", "+5 ns"}, {kBlack, kBlue + 1, kRed + 1}, "ZDC " + arm + " energy (GeV)", pdfname, true,
            zhi ? "MB |z| < 10 cm, all runs" : "MB, all runs");
  }
  // O+O: peripheral MB events (OHCal < 2 GeV), all runs
  if (zperiph[0] && zperiph[1])
  {
    std::vector<TH1 *> hs;
    for (int a = 0; a < 2; a++)
    {
      TH1 *h = zperiph[a];
      h->Rebin(2);
      h->GetXaxis()->SetRangeUser(0., 750.);
      hs.push_back(h);
    }
    overlay(hs, {"south", "north"}, {kBlue + 1, kRed + 1}, "ZDC energy (GeV)", outdir + "/summary/zdc_periph_vs_run.pdf", false,
            "MB |z| < 10 cm, OHCal < 2 GeV, all runs");
  }
  drawns("sepd_mip", "sEPD mean tile MIP peak (nMIP)");
  drawns("sepd_qsum_mean", "sEPD mean charge sum per event (nMIP)");
  drawns("sepd_nhit_mean", "sEPD mean tiles with q > 0.5 nMIP");
  for (size_t l = 0; l < layers.size(); l++)
  {
    drawgr(layers[l] + "_occ", layerlabel[l] + " mean fraction of good towers above threshold", std::nan(""), "E_{tower} > 0.1 GeV");
    drawgr(layers[l] + "_time", layerlabel[l] + " mean tower time (samples)");
    drawgr(layers[l] + "_towsum", layerlabel[l] + " mean tower energy sum (GeV)");
    drawgr(layers[l] + "_noise", layerlabel[l] + " noise #sigma (MeV)");
  }
  // HCal vs iphi, all runs: tower value / mean of its ieta ring (towers with >= 10 entries), averaged over ieta at each iphi
  for (const auto &[key, p] : tscsum)
  {
    const bool isE = key.find("_tsc_e") != std::string::npos;
    const size_t l = key.rfind("ihcal", 0) == 0 ? 1 : 2;
    std::array<double, 1536> v{};
    std::array<double, 1536> ev{};
    std::array<bool, 1536> ok{};
    for (int i = 0; i < 1536; i++)
    {
      ok[i] = p->GetBinEntries(i + 1) >= 10 && p->GetBinContent(i + 1) > 0;
      v[i] = p->GetBinContent(i + 1);
      ev[i] = p->GetBinError(i + 1);
    }
    std::array<double, 64> sum{};
    std::array<double, 64> sum2{};
    std::array<int, 64> n{};
    for (int ie = 0; ie < 24; ie++)
    {
      double r = 0;
      int nr = 0;
      for (int ip = 0; ip < 64; ip++)
      {
        if (ok[ie * 64 + ip])
        {
          r += v[ie * 64 + ip];
          nr++;
        }
      }
      for (int ip = 0; nr > 0 && ip < 64; ip++)
      {
        const int i = ie * 64 + ip;
        if (ok[i])
        {
          sum[ip] += v[i] / (r / nr);
          sum2[ip] += std::pow(ev[i] / (r / nr), 2);
          n[ip]++;
        }
      }
    }
    TH1D *h = new TH1D(("tsc_" + key).c_str(),
                       (std::string(";i#phi;") + (isE ? Form("<E> in %s GeV / i#eta ring mean", l == 1 ? "0.3-1.0" : "1.0-2.5")
                                                      : Form("fraction E > %s GeV / i#eta ring mean", l == 1 ? "0.3" : "1.0")))
                           .c_str(),
                       64, -0.5, 63.5);
    double dev = 0.01;
    for (int ip = 0; ip < 64; ip++)
    {
      if (n[ip] == 0)
      {
        continue;
      }
      h->SetBinContent(ip + 1, sum[ip] / n[ip]);
      h->SetBinError(ip + 1, std::sqrt(sum2[ip]) / n[ip]);
      dev = std::max(dev, std::abs(sum[ip] / n[ip] - 1) + std::sqrt(sum2[ip]) / n[ip]);
    }
    TCanvas *c = newCanvas(false);
    c->SetRightMargin(0.04);
    h->SetMarkerStyle(20);
    h->SetMarkerSize(0.8);
    h->SetMinimum(1 - 1.6 * dev);
    h->SetMaximum(1 + 1.6 * dev);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->GetXaxis()->SetNdivisions(816);
    h->Draw("e");
    refline(1., false);
    label(layerlabel[l] + ", all runs, good towers averaged over i#eta");
    std::string pdfname = outdir;
    pdfname.append("/summary/").append(key).append("_vs_run.pdf");
    keep(pdfname, {h}, {"e"}, {}, false);
    save(c, pdfname);
  }

  g_objfile->Close();
  g_objfile = nullptr;

  //------------------------ CDB status map ------------------------//
  std::map<std::pair<int, std::string>, std::string> st;
  std::set<std::string> domains;
  std::set<int> stRuns;
  {
    std::ifstream in(statusfile);
    std::string line;
    while (std::getline(in, line))
    {
      std::istringstream ls(line);
      int r;
      std::string d;
      std::string s;
      if (!(ls >> r >> d >> s))
      {
        continue;
      }
      st[{r, d}] = s;
      domains.insert(d);
      stRuns.insert(r);
    }
  }
  if (!st.empty())
  {
    std::vector<std::string> dl(domains.begin(), domains.end());
    std::vector<int> rl(stRuns.begin(), stRuns.end());
    TH2 *hmap = new TH2F("cdbmap", "", rl.size(), 0, rl.size(), dl.size(), 0, dl.size());
    // same colours as the CDB table of the HTML pages
    const std::vector<std::string> stname{"run", "range", "all", "default", "missing", "updated"};
    const std::vector<std::string> sthex{"#81c784", "#ffe082", "#e0e0e0", "#ffb74d", "#e57373", "#90caf9"};
    const std::vector<std::string> stlabel{"run-specific", "run range", "all runs", "default", "missing", "updated since last QA"};
    std::map<std::string, int> code;
    int pal[6];
    for (size_t k = 0; k < stname.size(); k++)
    {
      code[stname[k]] = k + 1;
      pal[k] = TColor::GetColor(sthex[k].c_str());
    }
    for (size_t i = 0; i < rl.size(); i++)
    {
      hmap->GetXaxis()->SetBinLabel(i + 1, Form("%d", rl[i]));
      for (size_t j = 0; j < dl.size(); j++)
      {
        auto it = st.find({rl[i], dl[j]});
        hmap->SetBinContent(i + 1, j + 1, it == st.end() ? code["missing"] : code.at(it->second));
      }
    }
    for (size_t j = 0; j < dl.size(); j++)
    {
      hmap->GetYaxis()->SetBinLabel(j + 1, dl[j].c_str());
    }
    gStyle->SetPalette(6, pal);
    hmap->SetMinimum(0.5);
    hmap->SetMaximum(6.5);
    hmap->SetContour(6);
    TCanvas *c = new TCanvas("cmap", "", 900, std::max(500, 22 * (int) dl.size() + 170));
    c->SetLeftMargin(0.38);
    c->SetRightMargin(0.04);
    c->SetBottomMargin(0.12);
    c->SetTopMargin(0.10);
    hmap->GetYaxis()->SetLabelSize(0.022);
    hmap->GetXaxis()->SetLabelSize(rl.size() > 40 ? 0.015 : 0.03);
    hmap->GetXaxis()->LabelsOption("v");
    hmap->Draw("col");
    // colour legend: boxes above the map
    for (size_t k = 0; k < stname.size(); k++)
    {
      const double x = 0.02 + 0.16 * k;
      TPad *p = new TPad(Form("pk%zu", k), "", x, 0.94, x + 0.022, 0.97);
      p->SetFillColor(pal[k]);
      p->Draw();
      c->cd();
      drawText(stlabel[k].c_str(), x + 0.026, 0.947, kBlack, 12);
    }
    c->SaveAs((outdir + "/summary/cdb_map.pdf").c_str());
    delete c;
  }
}
