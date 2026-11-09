// SDCC CDB snapshot: no event input, GL1 packet, reconstruction or collector build.
#include "TPCRunConditionsSnapshot.h"
#include <cdbobjects/CDBTTree.h>
#include <ffamodules/CDBInterface.h>
#include <phool/recoConsts.h>
#include <TSystem.h>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
R__LOAD_LIBRARY(libphool.so)
R__LOAD_LIBRARY(libffamodules.so)
R__LOAD_LIBRARY(libcdbobjects.so)

namespace TPCSnapshot
{
inline std::string quote(const std::string &text)
{
  std::string out = "\"";
  for (char c : text)
  {
    if (c == '\"')
      out += '\"';
    out += c;
  }
  return out + '\"';
}
inline std::string number(double x)
{
  if (!std::isfinite(x))
    return "";
  std::ostringstream out;
  out << std::setprecision(15) << x;
  return out.str();
}
inline std::string joined(const std::vector<std::string> &values)
{
  std::string out;
  for (const auto &s : values)
  {
    if (!out.empty())
      out += ';';
    out += s;
  }
  return out;
}
} // namespace TPCSnapshot

void ExportTPCRunConditions(int run, const char *group, const char *output,
                            const char *seriesOutput, const char *cdbTag, bool useFallback = false,
                            double referenceKeff = 1.55, double par0 = .185, double par1 = .815,
                            double referenceSouth = 3.36138, double referenceNorth = 3.20357)
{
  using namespace TPCSnapshot;
  auto *rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER", run);
  rc->set_uint64Flag("TIMESTAMP", run);
  rc->set_StringFlag("CDB_GLOBALTAG", cdbTag);
  auto *cdb = CDBInterface::instance();
  const std::string conditionFile = cdb->getUrl("TPC_CONDITIONS");
  const std::string keffFile = cdb->getUrl("Tpc_PolyClusterizer_kEff");
  std::unique_ptr<CDBTTree> conditions, keff;
  if (!conditionFile.empty())
  {
    conditions = std::make_unique<CDBTTree>(conditionFile);
    conditions->LoadCalibrations();
  }
  if (!keffFile.empty())
  {
    keff = std::make_unique<CDBTTree>(keffFile);
    keff->LoadCalibrations();
  }
  std::map<std::uint64_t, int> channels;
  if (conditions)
    for (const auto &entry : conditions->GetUInt64EntryMap())
    {
      const auto found = entry.second.find("gbco");
      if (found == entry.second.end() || found->second == 0 ||
          found->second == std::numeric_limits<std::uint64_t>::max())
        continue;
      if (!channels.emplace(found->second, entry.first).second)
        throw std::runtime_error("Duplicate conditions BCO");
    }
  std::vector<double> currents[2], pressure, temperature;
  std::ofstream series(seriesOutput);
  if (!series)
    throw std::runtime_error("Cannot write conditions series");
  series << "group,run,side,bco,gem_current_uA,pressure,temperature_K\n";
  for (const auto &entry : channels)
  {
    const double p = conditions->GetFloatValue(entry.second, "gas_pressure");
    double t = conditions->GetFloatValue(entry.second, "gas_temperature");
    if (std::isfinite(t) && t < 200.)
      t += 273.15;
    pressure.push_back(p);
    temperature.push_back(t);
    for (int side = 0; side < 2; ++side)
    {
      std::vector<double> values;
      for (const auto &name : r1Channels(side))
        values.push_back(conditions->GetFloatValue(entry.second, name));
      const double value = median(values);
      currents[side].push_back(value);
      series << quote(group) << ',' << run << ',' << side << ',' << entry.first << ','
             << number(value) << ',' << number(p) << ',' << number(t) << '\n';
    }
  }
  series.close();
  const auto p = summarize(pressure), t = summarize(temperature);
  std::ofstream out(output);
  if (!out)
    throw std::runtime_error("Cannot write conditions snapshot");
  out << "group,run,side,gem_current_uA,gem_current_rms_uA,gem_current_min_uA,gem_current_max_uA,"
         "keff,keff_base,keff_applied,keff_kind,keff_source,conditions_status,current_scope,"
         "current_definition,current_channels,conditions_samples,valid_current_samples,bco_first,"
         "bco_last,pressure,temperature_K,cdb_tag,conditions_payload,keff_payload,source,recipe_"
         "reference_keff,recipe_par0,recipe_par1,recipe_reference_current_uA\n";
  for (int side = 0; side < 2; ++side)
  {
    const auto values = summarize(currents[side]);
    double scale = missing();
    std::string source = "missing", kind = "missing";
    if (keff)
    {
      const auto &entries = keff->GetSingleFloatEntryMap();
      const auto found = entries.find("Fkeffside" + std::to_string(side));
      if (found != entries.end() && std::isfinite(found->second))
      {
        scale = found->second;
        source = "CDB base coefficient snapshot";
        kind = "cdb_base_not_verified_applied";
      }
      // A present but invalid CDB coefficient must not silently become a fallback.
    }
    else if (useFallback)
    {
      scale = fallbackKeff(values.mean, side, referenceKeff, par0, par1, referenceSouth,
                           referenceNorth);
      if (std::isfinite(scale))
      {
        source = "explicit PHGarfield GEM-current fallback recipe";
        kind = "derived_base_not_verified_applied";
      }
    }
    const bool complete = !channels.empty() && values.n == channels.size();
    const std::string status = !complete
                                   ? "missing_or_partial_current"
                                   : (!std::isfinite(scale) ? "missing_keff" : "complete_snapshot");
    out << quote(group) << ',' << run << ',' << side << ',' << number(values.mean) << ','
        << number(values.rms) << ',' << number(values.low) << ',' << number(values.high) << ','
        << number(scale) << ',' << number(scale) << ",," << quote(kind) << ',' << quote(source)
        << ',' << quote(status)
        << ",full_CDB_run_not_selected_QA_segments,unweighted_mean_of_R1_undamaged_channel_medians,"
        << quote(joined(r1Channels(side))) << ',' << channels.size() << ',' << values.n << ',';
    if (!channels.empty())
      out << channels.begin()->first;
    out << ',';
    if (!channels.empty())
      out << channels.rbegin()->first;
    out << ',' << number(p.mean) << ',' << number(t.mean) << ',' << quote(cdbTag) << ','
        << quote(conditionFile) << ',' << quote(keffFile)
        << ",\"current CDB snapshot; historical applied value not recoverable from ordinary QA "
           "histograms\","
        << number(referenceKeff) << ',' << number(par0) << ',' << number(par1) << ','
        << number(side == 0 ? referenceSouth : referenceNorth) << '\n';
  }
  out.close();
}
