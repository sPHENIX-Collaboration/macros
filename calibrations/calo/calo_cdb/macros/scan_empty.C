// Events per run and the plots without any content, for the flags on the web pages.
// input: runfiles.txt of pass 2 (run, histogram file or "-"), folder of the per-run plot objects (run<r>.root)
// output: one line per run: run, all events, MB events (from h_CDBCaloQA_evt, -1 without file), empty plot keys ("-" for none)
#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <TKey.h>
#include <TProfile.h>

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

namespace
{
  // 1 if the object has content, 0 if not, -1 if it is not a histogram or graph
  int filled(TObject *o)
  {
    if (auto *p = dynamic_cast<TProfile *>(o))
    {
      for (int i = 1; i <= p->GetNbinsX(); i++)
      {
        if (p->GetBinEntries(i) != 0) return 1;
      }
      return 0;
    }
    if (auto *h = dynamic_cast<TH1 *>(o))
    {
      for (int i = 1; i <= h->GetNbinsX(); i++)
      {
        for (int j = 1; j <= std::max(1, h->GetNbinsY()); j++)
        {
          if (h->GetBinContent(i, j) != 0) return 1;
        }
      }
      return 0;
    }
    if (auto *g = dynamic_cast<TGraph *>(o))
    {
      for (int i = 0; i < g->GetN(); i++)
      {
        if (g->GetY()[i] != 0) return 1;
      }
      return 0;
    }
    return -1;
  }
}  // namespace

void scan_empty(const std::string &runfiles, const std::string &plotdir, const std::string &out)
{
  std::ifstream in(runfiles);
  std::ofstream os(out);
  std::string line;
  while (std::getline(in, line))
  {
    std::istringstream ls(line);
    int run;
    std::string hf;
    if (!(ls >> run >> hf)) continue;
    double nall = -1;
    double nmb = -1;
    if (hf != "-")
    {
      TFile *f = TFile::Open(hf.c_str());
      if (f && !f->IsZombie())
      {
        if (auto *h = dynamic_cast<TH1 *>(f->Get("h_CDBCaloQA_evt")))
        {
          nall = h->GetBinContent(1);
          nmb = h->GetBinContent(2);
        }
      }
      delete f;
    }
    // plot key -> any content (objects <key>_<i>)
    std::map<std::string, int> keys;
    TFile *p = TFile::Open(Form("%s/run%d.root", plotdir.c_str(), run));
    if (p && !p->IsZombie())
    {
      for (auto *k : *p->GetListOfKeys())
      {
        std::string n = k->GetName();
        size_t u = n.rfind('_');
        if (u == std::string::npos || n.find_first_not_of("0123456789", u + 1) != std::string::npos) continue;
        int c = filled(static_cast<TKey *>(k)->ReadObj());
        if (c < 0) continue;
        int &v = keys.emplace(n.substr(0, u), 0).first->second;
        v = std::max(v, c);
      }
    }
    delete p;
    std::string empty;
    for (auto &[k, c] : keys)
    {
      if (!c) empty += (empty.empty() ? "" : ",") + k;
    }
    os << run << " " << nall << " " << nmb << " " << (empty.empty() ? "-" : empty) << "\n";
  }
}
