// ROOT-only postprocessing of TpcSeedsQA histograms. See README.TPCDistortionQA.md.
#include <TCanvas.h>
#include <TDirectory.h>
#include <TF1.h>
#include <TFile.h>
#include <TFitResult.h>
#include <TFitResultPtr.h>
#include <TGraphErrors.h>
#include <TH1D.h>
#include <TH2.h>
#include <TH2D.h>
#include <TH3.h>
#include <TKey.h>
#include <TLegend.h>
#include <TLine.h>
#include <TNamed.h>
#include <TPad.h>
#include <TROOT.h>
#include <TStyle.h>
#include <TSystem.h>

#include <algorithm>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace TPCDQA
{
using Hist = std::unique_ptr<TH1>;
using Hists = std::map<std::string, Hist>;
// ROOT installations may still use C++17; Jenkins checks also support C++20.
template <class Container, class Key>
bool hasKey(const Container &container, const Key &key)
{
#if __cplusplus >= 202002L
  return container.contains(key);
#else
  return container.find(key) != container.end();
#endif
}
struct Sample
{
  std::string label{};
  std::string list{};
  std::string driftList{};
  double rawTimeShift{std::numeric_limits<double>::quiet_NaN()};
};
struct Curve
{
  std::string title{};
  std::string xTitle{};
  std::string yTitle{};
  std::string comparison{"difference"};
  std::vector<double> x{};
  std::vector<double> ex{};
  std::vector<double> y{};
  std::vector<double> ey{};
  std::vector<double> edges{};
};
using Curves = std::map<std::string, Curve>;
struct Config
{
  double ptMin, ptMax;
  int minEntries, phiGroup, etaGroup;
  std::string prefix;
  bool saveSlices;
  double ratioFloor;
  bool phiQpt;
  double rawTimeShift;
  bool fitBackground{false};
};
struct Fit
{
  int status{1}, rootStatus{-1}, covariance{-1};
  double background{0};
  int ndf{0};
  double n{0}, amplitude{0}, mean{0}, meanError{0}, sigma{0}, sigmaError{0}, low{0}, high{0},
      chi2ndf{0};
};

bool sameAxis(const TAxis *a, const TAxis *b)
{
  if (a->GetNbins() != b->GetNbins())
  {
    return false;
  }
  for (int i = 1; i <= a->GetNbins() + 1; ++i)
  {
    if (std::abs(a->GetBinLowEdge(i) - b->GetBinLowEdge(i)) > 1e-10)
    {
      return false;
    }
  }
  return true;
}
bool sameBins(const TH1 *a, const TH1 *b)
{
  return a->GetDimension() == b->GetDimension() && sameAxis(a->GetXaxis(), b->GetXaxis()) &&
         (a->GetDimension() < 2 || sameAxis(a->GetYaxis(), b->GetYaxis())) &&
         (a->GetDimension() < 3 || sameAxis(a->GetZaxis(), b->GetZaxis()));
}
std::vector<std::string> wanted()
{
  std::vector<std::string> names = {"nrecotracks1d",   "pt_pos",          "pt_neg",
                                    "nrecotracks_pos", "nrecotracks_neg", "tpc_only_crossing",
                                    "tpc_only_status"};
  for (const std::string side : {"south", "north"})
  {
    names.push_back("tpc_only_" + side + "_rdca_qoverpt");
    for (const std::string q : {"pos", "neg"})
    {
      names.push_back(std::string("dcaxyorigin_phi_").append(side).append("_").append(q));
      for (const std::string obs : {"rdca_phi_pt", "rdca_eta_pt", "pt_phi_eta"})
      {
        names.push_back(
            std::string("tpc_only_").append(side).append("_").append(q).append("_").append(obs));
      }
    }
  }
  return names;
}

// Each observable must be present in EVERY segment of a sample. Partial sums
// would silently give charge-dependent exposures when old/new files are mixed.
Hists load(const Sample &sample, const Config &cfg, std::ostream &availability)
{
  std::ifstream in(sample.list);
  if (!in)
  {
    throw std::runtime_error("Cannot read file list: " + sample.list);
  }
  std::set<std::string> seen;
  std::map<std::string, int> present;
  Hists sums;
  int files = 0;
  std::string path;
  while (std::getline(in, path))
  {
    if (path.empty() || path[0] == '#')
    {
      continue;
    }
    if (!seen.insert(path).second)
    {
      throw std::runtime_error("Duplicate input: " + path);
    }
    std::unique_ptr<TFile> file(TFile::Open(path.c_str(), "READ"));
    if (!file || file->IsZombie() || file->TestBit(TFile::kRecovered))
    {
      throw std::runtime_error("Bad ROOT input: " + path);
    }
    ++files;
    for (const auto &name : wanted())
    {
      auto *h = dynamic_cast<TH1 *>(file->Get((cfg.prefix + name).c_str()));
      if (!h)
      {
        continue;
      }
      int dim = 1;
      if (name.find("rdca_qoverpt") != std::string::npos || name.find("dcaxyorigin_phi") == 0 ||
          name == "nrecotracks_pos" || name == "nrecotracks_neg")
      {
        dim = 2;
      }
      else if (name.find("_pt_phi_eta") != std::string::npos ||
               name.find("_rdca_phi_pt") != std::string::npos ||
               name.find("_rdca_eta_pt") != std::string::npos)
      {
        dim = 3;
      }
      if (h->GetDimension() != dim)
      {
        throw std::runtime_error("Wrong histogram dimension: " + name);
      }
      ++present[name];
      auto it = sums.find(name);
      if (it == sums.end())
      {
        Hist copy(static_cast<TH1 *>(h->Clone(name.c_str())));
        copy->SetDirectory(nullptr);
        sums.emplace(name, std::move(copy));
      }
      else
      {
        if (!sameBins(it->second.get(), h))
        {
          throw std::runtime_error("Incompatible segment binning: " + name);
        }
        if (!it->second->Add(h))
        {
          throw std::runtime_error("Cannot merge: " + name);
        }
      }
    }
  }
  if (!files)
  {
    throw std::runtime_error("Empty input list: " + sample.list);
  }
  for (const auto &name : wanted())
  {
    std::string state = "absent";
    if (present[name] == files)
    {
      state = "complete";
    }
    else if (present[name])
    {
      state = "partial_SKIPPED";
    }
    availability << sample.label << '\t' << name << '\t' << present[name] << '\t' << files << '\t'
                 << state << '\n';
    if (present[name] != files)
    {
      sums.erase(name);
    }
  }
  return sums;
}

Fit gaussian(TH1 *h, int minEntries, bool background = false)
{
  Fit f;
  f.n = h->Integral(1, h->GetNbinsX());
  if (f.n < minEntries)
  {
    return f;
  }
  double probs[] = {.16, .5, .84};
  double qs[3];
  h->GetQuantiles(3, qs, probs);
  const double width = h->GetXaxis()->GetBinWidth(h->GetMaximumBin());
  double sigma = std::max((qs[2] - qs[0]) / 2., width);
  double mean = qs[1];
  if (background)
  {
    // Locate the peak core instead of allowing broad tails to set its width.
    const int peak = h->GetMaximumBin();
    std::vector<double> contents;
    for (int b = 1; b <= h->GetNbinsX(); ++b)
      contents.push_back(h->GetBinContent(b));
    std::sort(contents.begin(), contents.end());
    const double pedestal = contents[contents.size() / 4];
    const double half = pedestal + .5 * (h->GetBinContent(peak) - pedestal);
    int left = peak;
    int right = peak;
    while (left > 1 && h->GetBinContent(left) > half)
      --left;
    while (right < h->GetNbinsX() && h->GetBinContent(right) > half)
      ++right;
    if (left < peak && right > peak && h->GetBinContent(left) <= half &&
        h->GetBinContent(right) <= half)
    {
      const auto crossing = [&](int a, int b)
      {
        const double ya = h->GetBinContent(a);
        const double yb = h->GetBinContent(b);
        return h->GetBinCenter(a) +
               (half - ya) * (h->GetBinCenter(b) - h->GetBinCenter(a)) / (yb - ya);
      };
      const double low = crossing(left, left + 1);
      const double high = crossing(right - 1, right);
      sigma = std::max(width, (high - low) / 2.354820045);
      mean = h->GetBinCenter(peak);
    }
  }
  const double axisLow = h->GetXaxis()->GetXmin();
  const double axisHigh = h->GetXaxis()->GetXmax();
  if (!(sigma > 0) || !std::isfinite(mean))
  {
    f.status = 2;
    return f;
  }
  for (int pass = 0; pass < 2; ++pass)
  {
    const double half = std::max((pass ? 2. : 2.5) * sigma, (background ? 3.5 : 3.) * width);
    int lo = std::max(1, h->GetXaxis()->FindFixBin(mean - half));
    int hi = std::min(h->GetNbinsX(), h->GetXaxis()->FindFixBin(mean + half));
    f.low = h->GetXaxis()->GetBinLowEdge(lo);
    f.high = h->GetXaxis()->GetBinUpEdge(hi);
    if (hi - lo + 1 < 6 || h->Integral(lo, hi) < minEntries)
    {
      f.status = 2;
      return f;
    }
    TF1 model("tpcdqa_gaussian", background ? "gaus(0)+[3]" : "gaus", f.low, f.high);
    const double pedestal =
        background ? std::max(0., std::min(h->GetBinContent(lo), h->GetBinContent(hi))) : 0.;
    model.SetParameters(std::max(1., h->GetMaximum() - pedestal), mean, sigma);
    if (background)
    {
      model.SetParameter(3, pedestal);
      model.SetParLimits(3, 0., std::max(1., h->GetMaximum()));
    }
    model.SetParLimits(0, 1e-9, std::max(10., 100. * h->GetMaximum()));
    model.SetParLimits(1, f.low, f.high);
    model.SetParLimits(2, .25 * width, axisHigh - axisLow);
    // Poisson likelihood, integrate across bins, retain result, no attachment.
    TFitResultPtr result = h->Fit(&model, "QSN0LRI");
    f.rootStatus = int(result);
    if (f.rootStatus != 0 || !result.Get() || !result->IsValid())
    {
      f.status = 3;
      return f;
    }
    f.covariance = result->CovMatrixStatus();
    f.ndf = result->Ndf();
    if (f.covariance < 2)
    {
      f.status = 4;
      return f;
    }
    mean = model.GetParameter(1);
    sigma = std::abs(model.GetParameter(2));
    f.mean = mean;
    f.amplitude = model.GetParameter(0);
    f.background = background ? model.GetParameter(3) : 0.;
    f.sigma = sigma;
    f.meanError = model.GetParError(1);
    f.sigmaError = model.GetParError(2);
    f.chi2ndf = result->Ndf() > 0 ? result->Chi2() / result->Ndf() : -1.;
    if (!std::isfinite(mean) || !std::isfinite(sigma) || !std::isfinite(f.meanError) ||
        !std::isfinite(f.sigmaError) || f.meanError <= 0 || f.sigmaError <= 0 ||
        sigma <= .26 * width || mean <= f.low || mean >= f.high)
    {
      f.status = 5;
      return f;
    }
    // The seed window may contain tails. Its poor chi2 must not prevent the
    // narrower second-pass core fit; apply goodness of fit to the final pass.
    if (pass == 1 && (result->Ndf() < 3 || !std::isfinite(f.chi2ndf) || f.chi2ndf > 10. ||
                      (background && model.GetParameter(0) <= 3. * model.GetParError(0))))
    {
      f.status = 6;
      return f;
    }
  }
  f.status = 0;
  return f;
}

void point(Curve &c, double x, double ex, double y, double ey)
{
  c.x.push_back(x);
  c.ex.push_back(ex);
  c.y.push_back(y);
  c.ey.push_back(ey);
}
std::unique_ptr<TGraphErrors> graph(const Curve &c, const std::string &name)
{
  auto g = std::make_unique<TGraphErrors>(int(c.x.size()), c.x.data(), c.y.data(), c.ex.data(),
                                          c.ey.data());
  g->SetName(name.c_str());
  g->SetTitle((c.title + ";" + c.xTitle + ";" + c.yTitle).c_str());
  return g;
}
std::vector<double> edges(const TAxis *axis)
{
  std::vector<double> e;
  for (int i = 1; i <= axis->GetNbins() + 1; ++i)
  {
    e.push_back(axis->GetBinLowEdge(i));
  }
  return e;
}
struct Location
{
  std::string folder, name;
};
std::map<std::string, std::string> outputIndex;
TDirectory *folder(TDirectory *root, const std::string &path)
{
  std::istringstream parts(path);
  std::string part;
  auto *dir = root;
  while (std::getline(parts, part, '/'))
  {
    if (part.empty())
    {
      continue;
    }
    auto *next = dir->GetDirectory(part.c_str());
    if (!next)
    {
      next = dir->mkdir(part.c_str());
    }
    if (!next)
    {
      throw std::runtime_error("Cannot create output folder: " + path);
    }
    dir = next;
  }
  return dir;
}
bool ends(const std::string &s, const std::string &suffix)
{
  return s.size() >= suffix.size() &&
         s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}
std::string plain(std::string s)
{
  std::replace(s.begin(), s.end(), '_', ' ');
  return s;
}
Location location(std::string key, const std::string &comparison = "difference")
{
  std::string suffix;
  if (ends(key, "_ratio_to_reference"))
  {
    key.resize(key.size() - 19);
    suffix = "_ratio_to_reference";
  }
  else if (ends(key, "_vs_reference"))
  {
    key.resize(key.size() - 13);
    suffix = comparison == "ratio" ? "_ratio_to_reference" : "_difference_from_reference";
  }
  if (key.rfind("drift_", 0) == 0)
  {
    const std::map<std::string, std::string> names = {
        {"drift_t0_bins", "t0_vs_side"},
        {"drift_cm_bins", "cm_time_vs_side"},
        {"drift_data_velocity", "drift_velocity_data_vs_side"},
        {"drift_phg_reference_velocity", "drift_velocity_phgarfield_reference_vs_side"},
        {"drift_phg_configured_t0", "t0_phgarfield_configured_vs_side"},
        {"drift_data_over_reference", "velocity_ratio_data_over_phgarfield_reference_vs_side"},
        {"drift_phg_edge_velocity", "drift_velocity_phgarfield_edges_vs_side"},
        {"drift_phg_baseline_velocity", "drift_velocity_phgarfield_baseline_vs_side"},
        {"drift_phg_adc_weighted_velocity", "drift_velocity_phgarfield_adc_weighted_vs_side"},
        {"drift_phg_over_data", "velocity_ratio_phgarfield_edges_over_data_vs_side"},
        {"drift_data_over_baseline", "velocity_ratio_data_over_phgarfield_baseline_vs_side"},
        {"drift_phg_cm_z", "phgarfield_cm_edge_z_vs_side"},
        {"drift_phg_pad_z", "phgarfield_pad_edge_z_vs_side"}};
    auto found = names.find(key);
    std::string name = found == names.end() ? key.substr(6) : found->second;
    for (const int side : {0, 1})
    {
      const std::string old = "_profile_side" + std::to_string(side);
      if (ends(name, old))
      {
        name = name.substr(0, name.size() - old.size()) + "_edge_profile_" +
               (side == 0 ? "south" : "north");
      }
    }
    return {suffix.empty() ? "drift" : "drift/reference_comparison", name + suffix};
  }
  std::vector<std::string> tokens;
  std::istringstream in(key);
  std::string token;
  while (std::getline(in, token, '_'))
  {
    tokens.push_back(token);
  }
  std::string selection;
  size_t start = 0;
  bool dca = false;
  if (tokens.size() >= 4 && tokens[0] == "legacy" && tokens[1] == "pcaz")
  {
    selection = tokens[2] == "south" ? "legacy_pca_z_nonpositive" : "legacy_pca_z_positive";
    start = 3;
    dca = true;
  }
  else if (tokens.size() >= 3 && tokens[0] == "new")
  {
    selection = "tpc_only_" + tokens[1];
    start = 2;
  }
  else if (tokens.size() >= 3 && tokens[0] == "legacy")
  {
    selection = tokens[1] == "pt" ? "legacy_all_tracks" : "legacy_pt_above_1";
    start = 1;
  }
  else
  {
    return {"diagnostics/unclassified", key + suffix};
  }
  if (tokens[start].rfind("phiBin", 0) == 0)
  {
    std::ostringstream number;
    number << std::setw(2) << std::setfill('0') << std::stoi(tokens[start].substr(6));
    selection += "/phi_bin_" + number.str();
    ++start;
    dca = true;
  }
  const std::string axis = tokens[start];
  const std::string last = tokens.back();
  dca = dca || last == "mean" || last == "sigma" || last == "even" || last == "odd";
  std::string name;
  if (dca)
  {
    if (last == "even" || last == "odd")
    {
      name = "rdca_charge_" + last + "_vs_" + axis;
    }
    else
    {
      name = "rdca_" + last + "_vs_" + axis;
      if (tokens.size() > start + 2 && (tokens[start + 1] == "pos" || tokens[start + 1] == "neg"))
      {
        name += "_" + tokens[start + 1];
      }
    }
  }
  else if (last == "ratio" || last == "asymmetry")
  {
    name = "charge_" + last + "_vs_" + axis;
  }
  else
  {
    name = (last == "counts" ? "track_counts_vs_" : "track_yield_per_frame_vs_") + axis + "_" +
           tokens[start + 1];
  }
  std::string path = (dca ? "rdca/" : "charge/") + selection;
  if (!suffix.empty())
  {
    path += "/reference_comparison";
  }
  // Extra representations remain available, without crowding the physics view.
  if (last == "asymmetry" || last == "yield")
  {
    path = "diagnostics/" + path;
  }
  return {path, name + suffix};
}
Location sliceLocation(const std::string &key)
{
  auto out = location(key + "_mean");
  out.name.replace(0, std::string("rdca_mean_").size(), "rdca_");
  return out;
}

// One visible object per observable. Fit masks and point graphs live separately.
std::string writeCurve(const Curve &c, const std::string &name, TDirectory *dir, bool flat = false)
{
  auto out = location(name, c.comparison);
  if (flat)
  {
    out.folder = "";
    out.name = name;
    const auto p = name.rfind("_side");
    if (p != std::string::npos)
    {
      auto metric = location(name.substr(0, p)).name;
      const auto q = metric.rfind("_vs_side");
      if (q != std::string::npos)
      {
        metric.replace(q, 8, "_vs_run");
      }
      out.name = metric + (name.substr(p + 5) == "0" ? "_south" : "_north");
    }
  }
  folder(dir, "metadata/comparisons")->cd();
  TNamed mode(name.c_str(), c.comparison.c_str());
  mode.Write();
  auto *main = folder(dir, out.folder);
  const std::string diagnostic =
      out.folder.rfind("diagnostics/", 0) == 0 ? out.folder : "diagnostics/" + out.folder;
  auto *checks = folder(dir, diagnostic);
  outputIndex[name] = out.folder + "/" + out.name;
  if (c.edges.size() < 2)
  {
    main->cd();
    auto g = graph(c, out.name);
    g->SetTitle((plain(out.name) + ";" + c.xTitle + ";" + c.yTitle).c_str());
    g->Write();
    return out.folder + "/" + out.name;
  }
  TH1D h(out.name.c_str(),
         (plain(out.name) + " (" + plain(out.folder) + ");" + c.xTitle + ";" + c.yTitle).c_str(),
         int(c.edges.size()) - 1, c.edges.data());
  TH1D mask((out.name + "_valid").c_str(), "Validity mask: 1=usable;axis;valid",
            int(c.edges.size()) - 1, c.edges.data());
  h.SetDirectory(nullptr);
  h.SetStats(false);
  h.SetOption("E1");
  mask.SetDirectory(nullptr);
  for (size_t i = 0; i < c.x.size(); ++i)
  {
    const int b = h.FindFixBin(c.x[i]);
    if (b < 1 || b > h.GetNbinsX())
    {
      continue;
    }
    h.SetBinContent(b, c.y[i]);
    h.SetBinError(b, c.ey[i]);
    mask.SetBinContent(b, 1);
  }
  main->cd();
  h.Write();
  checks->cd();
  mask.Write();
  graph(c, out.name + "_valid_points")->Write();
  folder(dir, "metadata/definitions/" + out.folder)->cd();
  TNamed definition(out.name.c_str(), c.title.c_str());
  definition.Write();
  return out.folder.empty() ? out.name : out.folder + "/" + out.name;
}

void slices(TH2 *h, const std::string &key, const std::string &title, Curves &curves,
            const Sample &sample, const Config &cfg, TDirectory *dir, std::ostream &csv)
{
  Curve mean{title, h->GetXaxis()->GetTitle(), "Gaussian core mean DCA_{xy} [cm]"};
  Curve sigma{title, mean.xTitle, "Gaussian core #sigma DCA_{xy} [cm]", "ratio"};
  mean.edges = sigma.edges = edges(h->GetXaxis());
  const auto target = sliceLocation(key);
  outputIndex[key] = target.folder + "/" + target.name;
  auto *checks = folder(dir, "diagnostics/" + target.folder + "/" + target.name);
  auto *sliceDir = cfg.saveSlices ? folder(checks, "slices") : nullptr;
  TH1D status((key + "_fit_status").c_str(), "Fit status (0=accepted);slice coordinate;status",
              h->GetNbinsX(), mean.edges.data());
  status.SetDirectory(nullptr);
  Hists diagnostics;
  for (const std::string name : {"counts", "chi2_ndf", "root_status", "covariance", "low", "high",
                                 "underflow", "overflow", "ndf", "background"})
  {
    Hist item(new TH1D(
        (std::string(key).append("_fit_").append(name)).c_str(),
        (std::string("Slice fit ").append(name).append(";slice coordinate;").append(name)).c_str(),
        h->GetNbinsX(), mean.edges.data()));
    item->SetDirectory(nullptr);
    diagnostics.emplace(name, std::move(item));
  }
  for (int i = 1; i <= h->GetNbinsX(); ++i)
  {
    Hist slice(h->ProjectionY(("slice_" + std::to_string(i)).c_str(), i, i, "e"));
    slice->SetDirectory(nullptr);
    const Fit f = gaussian(slice.get(), cfg.minEntries, cfg.fitBackground);
    status.SetBinContent(i, f.status);
    diagnostics.at("counts")->SetBinContent(i, f.n);
    diagnostics.at("ndf")->SetBinContent(i, f.ndf);
    diagnostics.at("background")->SetBinContent(i, f.background);
    diagnostics.at("chi2_ndf")->SetBinContent(i, f.chi2ndf);
    diagnostics.at("root_status")->SetBinContent(i, f.rootStatus);
    diagnostics.at("covariance")->SetBinContent(i, f.covariance);
    diagnostics.at("low")->SetBinContent(i, f.low);
    diagnostics.at("high")->SetBinContent(i, f.high);
    diagnostics.at("underflow")->SetBinContent(i, slice->GetBinContent(0));
    diagnostics.at("overflow")->SetBinContent(i, slice->GetBinContent(slice->GetNbinsX() + 1));
    const double x = h->GetXaxis()->GetBinCenter(i);
    const double ex = h->GetXaxis()->GetBinWidth(i) / 2.;
    csv << sample.label << ',' << key << ',' << i << ',' << x << ',' << ex << ',' << f.n << ','
        << f.status << ',' << f.rootStatus << ',' << f.covariance << ',' << f.mean << ','
        << f.meanError << ',' << f.sigma << ',' << f.sigmaError << ',' << f.low << ',' << f.high
        << ',' << f.chi2ndf << ',' << slice->GetBinContent(0) << ','
        << slice->GetBinContent(slice->GetNbinsX() + 1) << ',' << f.ndf << ',' << f.background
        << ',' << (cfg.fitBackground ? "gaussian_background" : "gaussian") << '\n';
    if (sliceDir)
    {
      sliceDir->cd();
      slice->Write();
    }
    if (!f.status)
    {
      TF1 fit("accepted_gaussian", cfg.fitBackground ? "gaus(0)+[3]" : "gaus", f.low, f.high);
      fit.SetParameters(f.amplitude, f.mean, f.sigma);
      if (cfg.fitBackground)
        fit.SetParameter(3, f.background);
      if (sliceDir)
      {
        fit.Write(("fit_" + std::to_string(i)).c_str());
      }
      point(mean, x, ex, f.mean, f.meanError);
      point(sigma, x, ex, f.sigma, f.sigmaError);
    }
  }
  checks->cd();
  status.Write("fit_status");
  for (const auto &entry : diagnostics)
  {
    entry.second->Write(entry.first.c_str());
  }
  // Keep the measured DCA map even when detailed per-bin slices are disabled.
  folder(dir, target.folder)->cd();
  h->SetTitle((plain(target.name) + " (" + plain(target.folder) + ");" + h->GetXaxis()->GetTitle() +
               ";DCA_{xy} to origin [cm]")
                  .c_str());
  h->SetOption("COLZ");
  h->SetStats(false);
  h->Write(target.name.c_str());
  if (key.find("_phi_") != std::string::npos && key.find("phiBin") == std::string::npos)
  {
    const std::string charge = ends(key, "_pos") ? "pos" : "neg";
    Hist distribution(
        h->ProjectionY(("rdca_distribution_" + charge).c_str(), 1, h->GetNbinsX(), "e"));
    distribution->SetDirectory(nullptr);
    distribution->SetOption("HIST");
    distribution->SetTitle(("DCA_{xy} distribution, " + charge + " (" + plain(target.folder) +
                            ");DCA_{xy} to origin [cm];tracks")
                               .c_str());
    distribution->Write();
  }
  curves[key + "_mean"] = std::move(mean);
  curves[key + "_sigma"] = std::move(sigma);
}

bool samePoint(const Curve &a, size_t i, const Curve &b, size_t j)
{
  return std::abs(a.x[i] - b.x[j]) < 1e-9 && std::abs(a.ex[i] - b.ex[j]) < 1e-9;
}
Curve combine(const Curve &a, const Curve &b, const std::string &op, double ratioFloor = .02)
{
  Curve c = a;
  c.x.clear();
  c.ex.clear();
  c.y.clear();
  c.ey.clear();
  for (size_t i = 0; i < a.x.size(); ++i)
  {
    for (size_t j = 0; j < b.x.size(); ++j)
    {
      if (!samePoint(a, i, b, j))
      {
        continue;
      }
      double y = 0;
      double err = 0;
      if (op == "ratio" || op == "signed_ratio")
      {
        if ((op == "ratio" && b.y[j] <= 0) ||
            (op == "signed_ratio" && std::abs(b.y[j]) <= std::max(ratioFloor, 3. * b.ey[j])))
        {
          break;
        }
        y = a.y[i] / b.y[j];
        err = std::hypot(a.ey[i] / b.y[j], a.y[i] * b.ey[j] / (b.y[j] * b.y[j]));
      }
      else
      {
        const double sign = op == "even" ? 1. : -1.;
        const double scale = op == "even" || op == "odd" ? .5 : 1.;
        y = scale * (a.y[i] + sign * b.y[j]);
        err = scale * std::hypot(a.ey[i], b.ey[j]);
      }
      if (std::isfinite(y) && std::isfinite(err))
      {
        point(c, a.x[i], a.ex[i], y, err);
      }
      break;
    }
  }
  return c;
}

void chargeCurves(TH1 *pos, TH1 *neg, const std::string &key, const std::string &title,
                  Curves &curves, double frames)
{
  if (!sameBins(pos, neg))
  {
    throw std::runtime_error("Charge histogram binning mismatch: " + key);
  }
  Curve ratio{title, pos->GetXaxis()->GetTitle(), "N_{+}/N_{-}", "ratio"};
  Curve asym{title, ratio.xTitle, "(N_{+}-N_{-})/(N_{+}+N_{-})"};
  Curve plus{title, ratio.xTitle, "tracks / frame / axis unit", "ratio"};
  Curve minus = plus;
  Curve plusCounts{title, ratio.xTitle, "positive tracks", "ratio"};
  Curve minusCounts{title, ratio.xTitle, "negative tracks", "ratio"};
  ratio.edges = asym.edges = plus.edges = minus.edges = edges(pos->GetXaxis());
  plusCounts.edges = minusCounts.edges = ratio.edges;
  for (int i = 1; i <= pos->GetNbinsX(); ++i)
  {
    const double p = pos->GetBinContent(i);
    const double n = neg->GetBinContent(i);
    const double ep = pos->GetBinError(i);
    const double en = neg->GetBinError(i);
    const double x = pos->GetBinCenter(i);
    const double ex = pos->GetBinWidth(i) / 2.;
    if (p < 0 || n < 0)
    {
      throw std::runtime_error("Negative counts in " + key);
    }
    point(plusCounts, x, ex, p, ep);
    point(minusCounts, x, ex, n, en);
    if (p + n > 0)
    {
      point(asym, x, ex, (p - n) / (p + n), 2. * std::hypot(n * ep, p * en) / ((p + n) * (p + n)));
    }
    if (n > 0)
    {
      point(ratio, x, ex, p / n, std::hypot(ep / n, p * en / (n * n)));
    }
    if (frames > 0)
    {
      const double norm = frames * pos->GetBinWidth(i);
      point(plus, x, ex, p / norm, ep / norm);
      point(minus, x, ex, n / norm, en / norm);
    }
  }
  curves[key + "_charge_ratio"] = ratio;
  curves[key + "_charge_asymmetry"] = asym;
  curves[key + "_pos_counts"] = std::move(plusCounts);
  curves[key + "_neg_counts"] = std::move(minusCounts);
  if (frames > 0)
  {
    curves[key + "_pos_yield"] = plus;
    curves[key + "_neg_yield"] = minus;
  }
}

// Accept pT intervals only at existing edges; never silently include a partial bin.
std::pair<int, int> ptRange(const TAxis *axis, double low, double high)
{
  int a = -1;
  int b = -1;
  for (int i = 1; i <= axis->GetNbins() + 1; ++i)
  {
    if (std::abs(axis->GetBinLowEdge(i) - low) < 1e-7)
    {
      a = i;
    }
    if (std::abs(axis->GetBinLowEdge(i) - high) < 1e-7)
    {
      b = i - 1;
    }
  }
  if (a < 1 || b < a)
  {
    throw std::runtime_error("Requested pT limits must coincide with histogram bin edges");
  }
  return {a, b};
}
void rebin(TH2 *h, int group)
{
  if (group < 1 || h->GetNbinsX() % group)
  {
    throw std::runtime_error("Slice grouping must divide the source axis bins");
  }
  if (group > 1)
  {
    h->RebinX(group);
  }
}
void rebin1(TH1 *h, int group)
{
  if (group < 1 || h->GetNbinsX() % group)
  {
    throw std::runtime_error("Charge grouping must divide the source axis bins");
  }
  if (group > 1)
  {
    h->Rebin(group);
  }
}

// Re-express stored charge-separated (phi,pT,DCA) counts in signed q/pT bins.
// The interval is exact; the histogram cannot recover sub-bin momentum detail.
void phiQoverPt(TH3 *pos, TH3 *neg, const std::string &side, const Sample &sample,
                const Config &cfg, TDirectory *dir, Curves &curves, std::ostream &csv)
{
  if (!sameBins(pos, neg) || pos->GetNbinsX() % cfg.phiGroup)
  {
    throw std::runtime_error("Incompatible phi-q/pT source bins");
  }
  std::vector<double> inverse;
  for (int j = 1; j <= pos->GetNbinsY() + 1; ++j)
  {
    const double pt = pos->GetYaxis()->GetBinLowEdge(j);
    if (pt >= .2 - 1e-7)
    {
      inverse.push_back(1. / pt);
    }
  }
  std::vector<double> qedges;
  qedges.reserve(2 * inverse.size());
  for (double v : inverse)
  {
    qedges.push_back(-v);
  }
  qedges.insert(qedges.end(), inverse.rbegin(), inverse.rend());
  const auto dcaEdges = edges(pos->GetZaxis());
  for (int lo = 1; lo <= pos->GetNbinsX(); lo += cfg.phiGroup)
  {
    const int hi = lo + cfg.phiGroup - 1;
    const std::string key =
        "new_" + side + "_phiBin" + std::to_string((lo - 1) / cfg.phiGroup + 1) + "_qoverpt";
    TH2D counts((key + "_counts").c_str(), ";q/p_{T} [c/GeV];DCA_{xy} [cm]", int(qedges.size()) - 1,
                qedges.data(), int(dcaEdges.size()) - 1, dcaEdges.data());
    counts.SetDirectory(nullptr);
    for (const int sign : {-1, 1})
    {
      auto *source = sign > 0 ? pos : neg;
      for (int j = 1; j <= source->GetNbinsY(); ++j)
      {
        const double pl = source->GetYaxis()->GetBinLowEdge(j);
        const double ph = source->GetYaxis()->GetBinUpEdge(j);
        if (pl < .2 - 1e-7)
        {
          continue;
        }
        const int qbin = counts.GetXaxis()->FindFixBin(sign * .5 * (1. / pl + 1. / ph));
        Hist dca(source->ProjectionZ("phi_qpt_slice", lo, hi, j, j, "e"));
        dca->SetDirectory(nullptr);
        for (int k = 0; k <= dca->GetNbinsX() + 1; ++k)
        {
          counts.SetBinContent(qbin, k, dca->GetBinContent(k));
          counts.SetBinError(qbin, k, dca->GetBinError(k));
        }
      }
    }
    std::ostringstream title;
    title << sample.label << ": TPC-only hardware " << side << ", "
          << pos->GetXaxis()->GetBinLowEdge(lo) << "<=phi<" << pos->GetXaxis()->GetBinUpEdge(hi)
          << ", all stored pT>0.2";
    slices(&counts, key, title.str(), curves, sample, cfg, dir, csv);
  }
}

Curves analyze(Hists &hs, const Sample &sample, const Config &cfg, TDirectory *dir,
               std::ostream &csv, std::ostream &trends)
{
  Curves curves;
  const auto get = [&](const std::string &name) -> TH1 *
  {
    auto i = hs.find(name);
    return i == hs.end() ? nullptr : i->second.get();
  };
  const double frames = get("nrecotracks1d") ? get("nrecotracks1d")->GetEntries() : 0.;
  folder(dir, "metadata")->cd();
  TNamed exposure("exposure",
                  ("QA frames=" + std::to_string(frames) + "; input-list=" + sample.list).c_str());
  exposure.Write();
  for (const std::string n : {"tpc_only_crossing", "tpc_only_status"})
  {
    if (get(n))
    {
      folder(dir, "diagnostics/selection")->cd();
      get(n)->Write(n == "tpc_only_crossing" ? "assigned_crossing" : "track_selection_status");
    }
  }
  // Legacy profiles always remain separate from hardware-side, selected new QA.
  for (const std::string side : {"south", "north"})
  {
    const std::string legacyTitle = sample.label + ": legacy PCA-z " +
                                    (side == "south" ? "<=0" : ">0") + ", no new pT/cluster cut";
    for (const std::string charge : {"pos", "neg"})
    {
      auto *h = dynamic_cast<TH2 *>(
          get(std::string("dcaxyorigin_phi_").append(side).append("_").append(charge)));
      if (!h)
      {
        continue;
      }
      Hist copy(static_cast<TH1 *>(h->Clone()));
      copy->SetDirectory(nullptr);
      auto *h2 = static_cast<TH2 *>(copy.get());
      rebin(h2, cfg.phiGroup);
      slices(h2, std::string("legacy_pcaz_").append(side).append("_phi_").append(charge),
             legacyTitle, curves, sample, cfg, dir, csv);
    }
    const std::string newTitle =
        sample.label + ": TPC-only hardware " + side + ", N_{TPC}>=31, all assigned crossings";
    for (const std::string axis : {"phi", "eta"})
    {
      for (const std::string charge : {"pos", "neg"})
      {
        auto *h = dynamic_cast<TH3 *>(get(std::string("tpc_only_")
                                              .append(side)
                                              .append("_")
                                              .append(charge)
                                              .append("_rdca_")
                                              .append(axis)
                                              .append("_pt")));
        if (!h)
        {
          continue;
        }
        const auto bins = ptRange(h->GetYaxis(), cfg.ptMin, cfg.ptMax);
        h->GetYaxis()->SetRange(bins.first, bins.second);
        std::unique_ptr<TH2> projected(static_cast<TH2 *>(h->Project3D("zxe")));
        projected->SetDirectory(nullptr);
        h->GetYaxis()->SetRange(0, 0);
        rebin(projected.get(), axis == "phi" ? cfg.phiGroup : cfg.etaGroup);
        slices(projected.get(),
               std::string("new_").append(side).append("_").append(axis).append("_").append(charge),
               newTitle + ", " + std::to_string(cfg.ptMin) + "<=pT<" + std::to_string(cfg.ptMax),
               curves, sample, cfg, dir, csv);
      }
    }
    if (auto *h = dynamic_cast<TH2 *>(get("tpc_only_" + side + "_rdca_qoverpt")))
    {
      // No extra pT cut is possible here: the source already integrates pT>0.2.
      slices(h, "new_" + side + "_qoverpt", newTitle + ", pT>0.2 (not the selected pT window)",
             curves, sample, cfg, dir, csv);
    }
    if (cfg.phiQpt)
    {
      auto *p = dynamic_cast<TH3 *>(get("tpc_only_" + side + "_pos_rdca_phi_pt"));
      auto *n = dynamic_cast<TH3 *>(get("tpc_only_" + side + "_neg_rdca_phi_pt"));
      if (p && n)
      {
        phiQoverPt(p, n, side, sample, cfg, dir, curves, csv);
      }
    }
    auto *p3 = dynamic_cast<TH3 *>(get("tpc_only_" + side + "_pos_pt_phi_eta"));
    auto *n3 = dynamic_cast<TH3 *>(get("tpc_only_" + side + "_neg_pt_phi_eta"));
    if (p3 && n3)
    {
      if (!sameBins(p3, n3))
      {
        throw std::runtime_error("New charge source bins differ");
      }
      const auto bins = ptRange(p3->GetZaxis(), cfg.ptMin, cfg.ptMax);
      for (const std::string axis : {"phi", "eta"})
      {
        Hist p(axis == "phi"
                   ? p3->ProjectionX("charge_p", 1, p3->GetNbinsY(), bins.first, bins.second, "e")
                   : p3->ProjectionY("charge_p", 1, p3->GetNbinsX(), bins.first, bins.second, "e"));
        p->SetDirectory(nullptr);
        Hist n(axis == "phi"
                   ? n3->ProjectionX("charge_n", 1, n3->GetNbinsY(), bins.first, bins.second, "e")
                   : n3->ProjectionY("charge_n", 1, n3->GetNbinsX(), bins.first, bins.second, "e"));
        n->SetDirectory(nullptr);
        rebin1(p.get(), axis == "phi" ? cfg.phiGroup : cfg.etaGroup);
        rebin1(n.get(), axis == "phi" ? cfg.phiGroup : cfg.etaGroup);
        chargeCurves(
            p.get(), n.get(), std::string("new_").append(side).append("_").append(axis),
            newTitle + ", selected pT window, charge projections within |eta|<1.2, |phi|<pi",
            curves, frames);
      }
    }
  }
  auto *lp = dynamic_cast<TH2 *>(get("nrecotracks_pos"));
  auto *ln = dynamic_cast<TH2 *>(get("nrecotracks_neg"));
  if (lp && ln)
  {
    if (!sameBins(lp, ln))
    {
      throw std::runtime_error("Legacy charge source bins differ");
    }
    for (const std::string axis : {"phi", "eta"})
    {
      Hist p(axis == "eta" ? lp->ProjectionX("legacy_p", 1, lp->GetNbinsY(), "e")
                           : lp->ProjectionY("legacy_p", 1, lp->GetNbinsX(), "e"));
      p->SetDirectory(nullptr);
      Hist n(axis == "eta" ? ln->ProjectionX("legacy_n", 1, ln->GetNbinsY(), "e")
                           : ln->ProjectionY("legacy_n", 1, ln->GetNbinsX(), "e"));
      n->SetDirectory(nullptr);
      rebin1(p.get(), axis == "phi" ? cfg.phiGroup : cfg.etaGroup);
      rebin1(n.get(), axis == "phi" ? cfg.phiGroup : cfg.etaGroup);
      chargeCurves(p.get(), n.get(), "legacy_" + axis,
                   sample.label + ": legacy track map, pT>1, within stored eta/phi axes", curves,
                   frames);
    }
  }
  if (get("pt_pos") && get("pt_neg"))
  {
    chargeCurves(get("pt_pos"), get("pt_neg"), "legacy_pt",
                 sample.label + ": legacy track map, all pT", curves, frames);
  }
  // Charge-even/odd are HALF sum/difference of separately fitted core means.
  const Curves initial = curves;
  for (const auto &entry : initial)
  {
    const auto &key = entry.first;
    if (key.size() < 9 || key.substr(key.size() - 9) != "_pos_mean")
    {
      continue;
    }
    const auto base = key.substr(0, key.size() - 9);
    auto neg = initial.find(base + "_neg_mean");
    if (neg == initial.end())
    {
      continue;
    }
    for (const std::string op : {"even", "odd"})
    {
      Curve c = combine(entry.second, neg->second, op);
      c.yTitle = op == "even" ? "(#mu_{+}+#mu_{-})/2 [cm]" : "(#mu_{+}-#mu_{-})/2 [cm]";
      curves[std::string(base).append("_charge_").append(op)] = std::move(c);
    }
  }
  dir->cd();
  for (const auto &entry : curves)
  {
    auto g = graph(entry.second, entry.first);
    writeCurve(entry.second, entry.first, dir);
    // Harmonics are diagnostics, not a conversion to a space-charge constant.
    if (entry.first.find("phi_charge_") != std::string::npos &&
        (entry.first.find("_even") != std::string::npos ||
         entry.first.find("_odd") != std::string::npos) &&
        g->GetN() >= 8)
    {
      TF1 harmonics("harmonics", "[0]+[1]*sin(x)+[2]*cos(x)+[3]*sin(2*x)+[4]*cos(2*x)",
                    -std::acos(-1.), std::acos(-1.));
      const auto fit = g->Fit(&harmonics, "QSN0EX0R");
      if (int(fit) == 0 && fit.Get() && fit->CovMatrixStatus() >= 2)
      {
        const auto address = location(entry.first);
        folder(dir, "diagnostics/" + address.folder + "/calibration_fits")->cd();
        fit->Write((address.name + "_harmonic_result").c_str());
        harmonics.Write((address.name + "_harmonics").c_str());
        for (int p = 0; p < 5; ++p)
        {
          trends << sample.label << ',' << entry.first << ",harmonic_" << p << ','
                 << harmonics.GetParameter(p) << ',' << harmonics.GetParError(p) << '\n';
        }
      }
    }
    if (entry.first.find("qoverpt_mean") != std::string::npos && g->GetN() >= 6)
    {
      // Fit each charge separately; don't bridge the unpopulated high-pT gap.
      for (const int sign : {-1, 1})
      {
        TGraphErrors subset;
        for (size_t i = 0; i < entry.second.x.size(); ++i)
        {
          if (sign * entry.second.x[i] > 0)
          {
            const int j = subset.GetN();
            subset.SetPoint(j, entry.second.x[i], entry.second.y[i]);
            subset.SetPointError(j, 0, entry.second.ey[i]);
          }
        }
        if (subset.GetN() < 4)
        {
          continue;
        }
        TF1 line("line", "pol1", sign < 0 ? -5 : 0, sign < 0 ? 0 : 5);
        const auto fit = subset.Fit(&line, "QSN0R");
        if (int(fit) == 0 && fit.Get() && fit->CovMatrixStatus() >= 2)
        {
          const std::string name = entry.first + (sign < 0 ? "_neg" : "_pos");
          const auto address = location(entry.first);
          folder(dir, "diagnostics/" + address.folder + "/calibration_fits")->cd();
          const std::string publicName = address.name + (sign < 0 ? "_neg" : "_pos");
          outputIndex[name] =
              "diagnostics/" + address.folder + "/calibration_fits/" + publicName + "_line_result";
          fit->Write((publicName + "_line_result").c_str());
          line.Write((publicName + "_line").c_str());
          for (int p = 0; p < 2; ++p)
          {
            trends << sample.label << ',' << name << ',' << (p ? "slope" : "intercept") << ','
                   << line.GetParameter(p) << ',' << line.GetParError(p) << '\n';
          }
        }
      }
    }
  }
  return curves;
}

void plot(const Curves &curves, const Curves &reference, const Sample &sample,
          const std::string &refLabel, const std::string &pdf, TDirectory *dir, const Config &cfg)
{
  TCanvas canvas("tpcdqa_canvas", "TPC distortion QA", 1100, 1100);
  const auto readable = [](std::string s)
  {
    std::replace(s.begin(), s.end(), '_', ' ');
    return s;
  };
  std::map<std::string, std::string> pdfs;
  for (const auto &entry : curves)
  {
    if (entry.second.x.empty())
    {
      continue;
    }
    const auto address = location(entry.first, entry.second.comparison);
    const auto category = address.folder.substr(0, address.folder.find('/'));
    if (category == "diagnostics")
    {
      auto referenceCurve = reference.find(entry.first);
      if (sample.label != refLabel && referenceCurve != reference.end())
      {
        auto change = combine(entry.second, referenceCurve->second, entry.second.comparison);
        writeCurve(change, entry.first + "_vs_reference", dir);
      }
      continue;
    }
    if (!hasKey(pdfs, category))
    {
      pdfs[category] = pdf.substr(0, pdf.size() - 4) + "_" + category + ".pdf";
      canvas.Print((pdfs[category] + "[").c_str());
    }
    canvas.Clear();
    const bool signedDCA = entry.first.find("_mean") != std::string::npos ||
                           entry.first.find("_charge_even") != std::string::npos ||
                           entry.first.find("_charge_odd") != std::string::npos;
    auto ref = reference.find(entry.first);
    const bool hasRef =
        sample.label != refLabel && ref != reference.end() && !ref->second.x.empty();
    int panels = 1;
    if (hasRef)
    {
      panels = signedDCA ? 3 : 2;
    }
    canvas.Divide(1, panels);
    for (int p = 1; p <= panels; ++p)
    {
      canvas.cd(p);
      gPad->SetLeftMargin(.16);
      gPad->SetRightMargin(.04);
      gPad->SetTopMargin(.12);
      gPad->SetBottomMargin(.14);
    }
    auto current = graph(entry.second, entry.first);
    std::string caption = readable(sample.label) + ": " + readable(address.name) + " (" +
                          readable(address.folder) + ")";
    if (entry.first.rfind("new_", 0) == 0 && entry.first.find("qoverpt") == std::string::npos)
    {
      std::ostringstream window;
      window << ", " << cfg.ptMin << "<=pT<" << cfg.ptMax;
      caption += window.str();
    }
    current->SetTitle((caption + ";" + entry.second.xTitle + ";" + entry.second.yTitle).c_str());
    current->SetLineColor(kBlue + 1);
    current->SetMarkerColor(kBlue + 1);
    current->SetMarkerStyle(20);
    std::unique_ptr<TGraphErrors> rg;
    std::unique_ptr<TGraphErrors> dg;
    std::unique_ptr<TGraphErrors> ratioGraph;
    canvas.cd(1);
    gPad->SetGrid();
    current->Draw("AP");
    TLegend legend(.70, .76, .97, .94);
    legend.SetTextSize(.035);
    legend.AddEntry(current.get(), sample.label.c_str(), "lp");
    if (hasRef)
    {
      rg = graph(ref->second, "reference");
      rg->SetLineColor(kRed + 1);
      rg->SetMarkerColor(kRed + 1);
      rg->SetMarkerStyle(24);
      rg->Draw("P SAME");
      legend.AddEntry(rg.get(), refLabel.c_str(), "lp");
      double low = std::numeric_limits<double>::infinity();
      double high = -low;
      for (const auto *c : {&entry.second, &ref->second})
      {
        for (size_t i = 0; i < c->y.size(); ++i)
        {
          low = std::min(low, c->y[i] - c->ey[i]);
          high = std::max(high, c->y[i] + c->ey[i]);
        }
      }
      const double margin = std::max(1e-5, .15 * (high - low));
      current->SetMinimum(low - margin);
      current->SetMaximum(high + margin);
    }
    legend.Draw();
    if (hasRef)
    {
      canvas.cd(2);
      gPad->SetGrid();
    }
    std::unique_ptr<TLine> line;
    std::unique_ptr<TLine> ratioLine;
    if (hasRef)
    {
      Curve diff = combine(entry.second, ref->second, entry.second.comparison);
      diff.title =
          readable(address.name) + ": " + readable(sample.label) + " vs " + readable(refLabel);
      diff.yTitle = entry.second.comparison == "ratio"
                        ? "sample / reference"
                        : entry.second.yTitle + " (sample - reference)";
      if (!diff.x.empty())
      {
        dg = graph(diff, entry.first + "_vs_reference");
        dg->SetMarkerStyle(20);
        if (entry.first.find("_profile_side") != std::string::npos)
        {
          double maximum = 0.;
          for (double value : diff.y)
          {
            maximum = std::max(maximum, std::abs(value));
          }
          if (maximum < .01)
          {
            dg->SetMinimum(-.01);
            dg->SetMaximum(.01);
          }
        }
        dg->Draw("AP");
        const double baseline = entry.second.comparison == "ratio" ? 1. : 0.;
        line = std::make_unique<TLine>(diff.x.front() - diff.ex.front(), baseline,
                                       diff.x.back() + diff.ex.back(), baseline);
        line->SetLineStyle(2);
        line->Draw();
        writeCurve(diff, entry.first + "_vs_reference", dir);
      }
      // Retain an empty graph/full zero mask too, when all denominators fail.
      if (diff.x.empty())
      {
        writeCurve(diff, entry.first + "_vs_reference", dir);
      }
      if (signedDCA)
      {
        Curve ratio = combine(entry.second, ref->second, "signed_ratio", cfg.ratioFloor);
        ratio.title =
            readable(address.name) + ": signed ratio, reference |mean| > max(floor, 3*error)";
        ratio.yTitle = "sample / reference (masked near zero)";
        writeCurve(ratio, entry.first + "_ratio_to_reference", dir);
        canvas.cd(3);
        gPad->SetGrid();
        if (!ratio.x.empty())
        {
          ratioGraph = graph(ratio, "ratio_display");
          ratioGraph->SetMarkerStyle(20);
          ratioGraph->Draw("AP");
          ratioLine = std::make_unique<TLine>(ratio.x.front() - ratio.ex.front(), 1.,
                                              ratio.x.back() + ratio.ex.back(), 1.);
          ratioLine->SetLineStyle(2);
          ratioLine->Draw();
        }
      }
    }
    canvas.Print(pdfs.at(category).c_str());
  }
  for (const auto &entry : pdfs)
  {
    canvas.Print((entry.second + "]").c_str());
  }
}

void plotDcaDistributions(TDirectory *dir, const std::string &pdf)
{
  auto *rdca = dir->GetDirectory("rdca");
  if (!rdca)
  {
    return;
  }
  TCanvas canvas("rdca_distributions", "Measured DCA distributions", 1100, 800);
  bool opened = false;
  std::function<void(TDirectory *)> visit = [&](TDirectory *current)
  {
    TIter next(current->GetListOfKeys());
    while (auto *key = dynamic_cast<TKey *>(next()))
    {
      const std::string name = key->GetName();
      if (name == "reference_comparison")
      {
        continue;
      }
      if (auto *child = current->GetDirectory(name.c_str()))
      {
        visit(child);
        continue;
      }
      if (name.rfind("rdca_vs_", 0) != 0 && name.rfind("rdca_distribution_", 0) != 0)
      {
        continue;
      }
      std::unique_ptr<TObject> object(key->ReadObj());
      auto *hist = dynamic_cast<TH1 *>(object.get());
      if (!hist)
      {
        continue;
      }
      hist->SetDirectory(nullptr);
      if (!opened)
      {
        canvas.Print((pdf + "[").c_str());
        opened = true;
      }
      canvas.Clear();
      canvas.cd();
      canvas.SetLeftMargin(.14);
      canvas.SetRightMargin(hist->GetDimension() == 2 ? .16 : .05);
      hist->Draw(hist->GetDimension() == 2 ? "COLZ" : "HIST");
      canvas.Print(pdf.c_str());
    }
  };
  visit(rdca);
  if (opened)
  {
    canvas.Print((pdf + "]").c_str());
  }
}
}  // namespace TPCDQA

#include "TPCDriftQA.h"

// Manifest: label<TAB>absolute path to a newline-delimited ROOT file list.
// Returns 0 on success; the batch wrapper maps exceptions to a nonzero exit.
int TPCDistortionQA(const char *manifest, const char *output = "tpc_distortion_qa",
                    const char *reference = "", double ptMin = .5, double ptMax = 1.,
                    int minEntries = 100, int phiGroup = 3, int etaGroup = 4,
                    const char *prefix = "h_TpcSeedsQA_", bool saveSlices = false,
                    double ratioFloor = .02, bool phiQpt = false, const char *conditions = "[]",
                    double rawTimeShift = 40., bool makePlots = true,
                    const char *phgReferences = "", bool fitBackground = false)
{
  using TPCDQA::analyze;
  using TPCDQA::analyzeDrift;
  using TPCDQA::applyPhgReference;
  using TPCDQA::combine;
  using TPCDQA::Config;
  using TPCDQA::Curves;
  using TPCDQA::DriftInput;
  using TPCDQA::edges;
  using TPCDQA::ends;
  using TPCDQA::folder;
  using TPCDQA::graph;
  using TPCDQA::Hists;
  using TPCDQA::load;
  using TPCDQA::loadDrift;
  using TPCDQA::outputIndex;
  using TPCDQA::plot;
  using TPCDQA::plotDcaDistributions;
  using TPCDQA::point;
  using TPCDQA::Sample;
  using TPCDQA::slices;
  using TPCDQA::writeCurve;
  if (!(ptMin >= .2 && ptMax > ptMin) || minEntries < 20 || phiGroup < 1 || etaGroup < 1 ||
      ratioFloor < 0 || !std::isfinite(ratioFloor) || !std::isfinite(rawTimeShift))
  {
    throw std::runtime_error("Invalid analysis configuration");
  }
  gROOT->SetBatch(true);
  gStyle->SetOptStat(0);
  gStyle->SetTitleFont(42, "XYZ");
  gStyle->SetLabelFont(42, "XYZ");
  gStyle->SetPaperSize(20., 20.);
  TH1::AddDirectory(false);
  std::ifstream in(manifest);
  if (!in)
  {
    throw std::runtime_error("Cannot read manifest");
  }
  std::vector<Sample> samples;
  std::set<std::string> labels;
  std::string row;
  while (std::getline(in, row))
  {
    if (row.empty() || row[0] == '#')
    {
      continue;
    }
    const auto tab = row.find('\t');
    if (tab == std::string::npos)
    {
      throw std::runtime_error("Manifest needs label<TAB>filelist");
    }
    const auto second = row.find('\t', tab + 1);
    const auto third = second == std::string::npos ? std::string::npos : row.find('\t', second + 1);
    Sample s{
        row.substr(0, tab),
        row.substr(tab + 1, second == std::string::npos ? std::string::npos : second - tab - 1),
        second == std::string::npos
            ? ""
            : row.substr(second + 1,
                         third == std::string::npos ? std::string::npos : third - second - 1),
        third == std::string::npos ? std::numeric_limits<double>::quiet_NaN()
                                   : std::stod(row.substr(third + 1))};
    if (s.label.empty() || s.label == "." || s.label == ".." || s.label == "configuration" ||
        s.label == "run_conditions_json" || s.label == "drift_run_trends" ||
        s.label.find_first_not_of(
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_.-") !=
            std::string::npos ||
        !labels.insert(s.label).second)
    {
      throw std::runtime_error("Invalid/duplicate sample label");
    }
    samples.push_back(s);
  }
  if (samples.empty())
  {
    throw std::runtime_error("No samples");
  }
  const std::string refLabel = std::string(reference).empty() ? samples.front().label : reference;
  auto ri = std::find_if(samples.begin(), samples.end(),
                         [&](const Sample &s) { return s.label == refLabel; });
  if (ri == samples.end())
  {
    throw std::runtime_error("Reference label not in manifest");
  }
  std::iter_swap(samples.begin(), ri);
  if (gSystem->mkdir(output, true) != 0 && gSystem->AccessPathName(output))
  {
    throw std::runtime_error("Cannot create output directory");
  }
  const std::string out(output);
  // Explicitly refuse overwriting a previous analysis.
  if (!gSystem->AccessPathName((out + "/results.root").c_str()))
  {
    throw std::runtime_error("Output already exists: use a new directory");
  }
  std::unique_ptr<TFile> result(TFile::Open((out + "/results.root").c_str(), "CREATE"));
  if (!result || result->IsZombie())
  {
    throw std::runtime_error("Cannot create results.root");
  }
  std::ofstream available(out + "/availability.tsv");
  std::ofstream csv(out + "/slice_fits.csv");
  std::ofstream trends(out + "/trends.csv");
  available << "sample\thistogram\tfiles_present\tfiles_total\tstatus\n";
  csv << std::setprecision(12)
      << "sample,observable,bin,x,x_half_width,in_range_counts,status,root_status,covariance,mean,"
         "mean_error,"
         "sigma,sigma_error,fit_low,fit_high,chi2_ndf,underflow,overflow,ndf,background,fit_"
         "model\n";
  trends << std::setprecision(12) << "sample,observable,parameter,value,error\n";
  std::ofstream driftCSV(out + "/drift_summary.csv");
  std::ofstream summary(out + "/analysis_summary.tsv");
  summary << "sample\trdca_status\n";
  driftCSV << std::setprecision(12)
           << "sample,side,source,t0_valid,cm_valid,t0_bin,cm_bin,t0_width10_90,cm_width10_90,v_"
              "data_cm_us,v_"
              "phg_edges_cm_us,v_phg_baseline_cm_us,v_phg_adc_weighted_cm_us,z_pad_edge_cm,z_cm_"
              "edge_cm,z_"
              "pad_geometry_cm,cm_halfwidth_cm,time_bin_ns,raw_to_unpacked_shift_bins\n";
  const Config cfg{ptMin,      ptMax,      minEntries, phiGroup,     etaGroup,     prefix,
                   saveSlices, ratioFloor, phiQpt,     rawTimeShift, fitBackground};
  folder(result.get(), "metadata")->cd();
  std::ostringstream config;
  config << "reference=" << refLabel << "; prefix=" << prefix << "; pt=[" << ptMin << ',' << ptMax
         << "); min_entries=" << minEntries << "; phi_group=" << phiGroup
         << "; eta_group=" << etaGroup
         << "; fits=two-pass Poisson/bin-integrated Gaussian; mean comparison=difference; other "
            "comparisons "
            "as named; errors=independent samples, fixed exposure";
  config << "; signed_ratio_floor_cm=" << ratioFloor
         << "; signed_ratio_reference_significance>3; save_slices=" << saveSlices
         << "; phi_qpt=" << phiQpt;
  config << "; fit_model="
         << (fitBackground ? "Gaussian+constant_background_modal_seed" : "Gaussian_quantile_seed");
  config << "; final_pass_quality_gate_only=true";
  config << "; raw_to_unpacked_shift=" << rawTimeShift << "; output_schema=2";
  TNamed metadata("configuration", config.str().c_str());
  metadata.Write();
  TNamed runConditions("run_conditions_json", conditions);
  runConditions.Write();
  Curves ref;
  Curves runTrends;
  std::vector<std::string> runLabels;
  for (const auto &sample : samples)
  {
    std::cout << "TPCDistortionQA: processing " << sample.label << std::endl;
    Hists hs = load(sample, cfg, available);
    DriftInput drift = loadDrift(sample, available, cfg.rawTimeShift);
    applyPhgReference(drift, sample, phgReferences);
    if (hs.empty() && !drift.time[0] && !drift.time[1])
    {
      throw std::runtime_error("No complete supported QA histograms for " + sample.label);
    }
    auto *dir = result->mkdir(sample.label.c_str());
    outputIndex.clear();
    for (const std::string name : {"rdca", "charge", "drift", "diagnostics", "metadata"})
    {
      folder(dir, name);
    }
    Curves curves = analyze(hs, sample, cfg, dir, csv, trends);
    bool hasDca = false;
    bool hasFit = false;
    for (const auto &entry : hs)
    {
      if (entry.first.find("dcaxyorigin_phi") != std::string::npos ||
          entry.first.find("_rdca_") != std::string::npos)
      {
        hasDca = true;
      }
    }
    for (const auto &entry : curves)
    {
      if (ends(entry.first, "_mean") && !entry.second.x.empty())
      {
        hasFit = true;
      }
    }
    std::string dcaState =
        "No supported DCA input histograms found; check histogram "
        "prefix and availability.tsv";
    if (hasDca)
    {
      dcaState = hasFit ? "DCA inputs found; Gaussian profiles available"
                        : "DCA inputs found, but no accepted Gaussian slices; "
                          "see diagnostics and min-entries";
    }
    std::cout << sample.label << ": " << dcaState << std::endl;
    summary << sample.label << '\t' << dcaState << '\n';
    summary.flush();
    folder(dir, "metadata")->cd();
    TNamed dcaAvailability("rdca_availability", dcaState.c_str());
    dcaAvailability.Write();
    Curves driftCurves = analyzeDrift(drift, sample, dir, driftCSV);
    for (const auto &entry : driftCurves)
    {
      if (entry.second.xTitle != "side (0 South, 1 North)")
      {
        continue;
      }
      for (size_t i = 0; i < entry.second.x.size(); ++i)
      {
        const std::string key = entry.first + "_side" + std::to_string(int(entry.second.x[i]));
        auto &trend = runTrends[key];
        trend.title = key;
        trend.xTitle = "sample index (see bin labels)";
        trend.yTitle = entry.second.yTitle;
        point(trend, runLabels.size() + .5, .5, entry.second.y[i], entry.second.ey[i]);
      }
    }
    runLabels.push_back(sample.label);
    curves.insert(driftCurves.begin(), driftCurves.end());
    if (sample.label == refLabel)
    {
      ref = curves;
    }
    if (makePlots)
    {
      plot(curves, ref, sample, refLabel, out + "/" + sample.label + ".pdf", dir, cfg);
      plotDcaDistributions(dir, out + "/" + sample.label + "_rdca_distributions.pdf");
    }
    else if (sample.label != refLabel)
    {
      for (const auto &entry : curves)
      {
        auto found = ref.find(entry.first);
        if (found == ref.end())
          continue;
        writeCurve(combine(entry.second, found->second, entry.second.comparison),
                   entry.first + "_vs_reference", dir);
        if (entry.first.find("_mean") != std::string::npos ||
            entry.first.find("_charge_even") != std::string::npos ||
            entry.first.find("_charge_odd") != std::string::npos)
          writeCurve(combine(entry.second, found->second, "signed_ratio", cfg.ratioFloor),
                     entry.first + "_ratio_to_reference", dir);
      }
    }
    std::ostringstream index;
    index << "internal_key\tROOT_path_relative_to_run\n";
    for (const auto &entry : outputIndex)
    {
      index << entry.first << '\t' << entry.second << '\n';
    }
    folder(dir, "metadata")->cd();
    TNamed objectIndex("object_index", index.str().c_str());
    objectIndex.Write();
  }
  if (!runTrends.empty())
  {
    auto *dir = folder(result.get(), "run_trends/drift");
    TCanvas overview("drift_overview", "Drift QA run trends", 1400, 700);
    const std::string pdf = out + "/drift_run_trends.pdf";
    if (makePlots)
      overview.Print((pdf + "[").c_str());
    for (auto &entry : runTrends)
    {
      auto &curve = entry.second;
      for (size_t i = 0; i <= runLabels.size(); ++i)
      {
        curve.edges.push_back(i);
      }
      const auto saved = writeCurve(curve, entry.first, dir, true);
      auto *h = dynamic_cast<TH1 *>(dir->Get(saved.c_str()));
      for (size_t i = 0; i < runLabels.size(); ++i)
      {
        h->GetXaxis()->SetBinLabel(i + 1, runLabels[i].c_str());
      }
      dir->cd();
      h->Write("", TObject::kOverwrite);
      auto g = graph(curve, entry.first);
      g->SetMarkerStyle(20);
      g->Draw("AP");
      if (makePlots)
        overview.Print(pdf.c_str());
    }
    for (int side = 0; side < 2; ++side)
    {
      std::vector<std::unique_ptr<TGraphErrors>> graphs;
      TLegend legend(.60, .72, .98, .93);
      double low = std::numeric_limits<double>::infinity();
      double high = -low;
      const std::vector<std::string> names = {"data_velocity", "phg_edge_velocity",
                                              "phg_baseline_velocity", "phg_adc_weighted_velocity"};
      for (size_t index = 0; index < names.size(); ++index)
      {
        auto it = runTrends.find("drift_" + names[index] + "_side" + std::to_string(side));
        if (it == runTrends.end() || it->second.x.empty())
        {
          continue;
        }
        for (double y : it->second.y)
        {
          low = std::min(low, y);
          high = std::max(high, y);
        }
        auto g = graph(it->second, names[index]);
        const int colors[] = {kBlue + 1, kRed + 1, kGreen + 2, kMagenta + 1};
        g->SetMarkerColor(colors[index]);
        g->SetLineColor(colors[index]);
        g->SetMarkerStyle(20 + index);
        legend.AddEntry(g.get(), names[index].c_str(), "lp");
        graphs.push_back(std::move(g));
      }
      if (!graphs.empty())
      {
        overview.Clear();
        graphs.front()->SetTitle(("Data and PHGarfield velocities, side " + std::to_string(side) +
                                  ";sample index;v [cm/#mus]")
                                     .c_str());
        const double margin = std::max(.005, .2 * (high - low));
        graphs.front()->SetMinimum(low - margin);
        graphs.front()->SetMaximum(high + margin);
        graphs.front()->Draw("AP");
        for (size_t i = 1; i < graphs.size(); ++i)
        {
          graphs[i]->Draw("P SAME");
        }
        legend.Draw();
        if (makePlots)
          overview.Print(pdf.c_str());
      }
    }
    if (makePlots)
      overview.Print((pdf + "]").c_str());
  }
  result->Close();
  std::ofstream done(out + "/SUCCESS");
  done << "Analysis completed\n";
  std::cout << "TPCDistortionQA: wrote " << out << std::endl;
  return 0;
}
