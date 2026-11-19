// -- sPHENIX includes
#include <calobase/TowerInfoDefs.h>

// for CDBTTree
#include <cdbobjects/CDBTTree.h>

#include <Rtypes.h>

// -- c++ includes
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <format>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#if defined(__CLING__)
R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libcdbobjects.so)
#endif

// -----------------------------------------------------------------------------
// Detector configuration mapping
// -----------------------------------------------------------------------------
struct BadTowerDetectorConfig
{
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::string name;
  unsigned int nEta{0};
  unsigned int nPhi{0};
  std::function<unsigned int(unsigned int, unsigned int)> encodeFunc;
  // NOLINTEND(misc-non-private-member-variables-in-classes)
};

inline void printSupportedBadTowerDetectors()
{
  std::cerr << "Supported detector types are:\n"
            << "  - CEMC / EMCAL     (96 x 256 towers, TowerInfoDefs::encode_emcal)\n"
            << "  - HCALIN / IHCAL   (24 x 64 towers, TowerInfoDefs::encode_hcal)\n"
            << "  - HCALOUT / OHCAL  (24 x 64 towers, TowerInfoDefs::encode_hcal)\n";
}

inline bool getBadTowerDetectorConfig(const std::string &detector, BadTowerDetectorConfig &cfg)
{
  std::string det = detector;
  std::transform(det.begin(), det.end(), det.begin(), ::toupper);

  if (det == "CEMC" || det == "EMCAL" || det == "EMC")
  {
    cfg = {"CEMC", 96, 256, [](unsigned int ieta, unsigned int iphi) { return TowerInfoDefs::encode_emcal(ieta, iphi); }};
    return true;
  }
  if (det == "HCALIN" || det == "IHCAL" || det == "INNER_HCAL" || det == "IH")
  {
    cfg = {"HCALIN", 24, 64, [](unsigned int ieta, unsigned int iphi) { return TowerInfoDefs::encode_hcal(ieta, iphi); }};
    return true;
  }
  if (det == "HCALOUT" || det == "OHCAL" || det == "OUTER_HCAL" || det == "OH")
  {
    cfg = {"HCALOUT", 24, 64, [](unsigned int ieta, unsigned int iphi) { return TowerInfoDefs::encode_hcal(ieta, iphi); }};
    return true;
  }

  return false;
}

// -----------------------------------------------------------------------------
// Create a global bad tower map (domain: <DET>_GlobalBadTowerMap, field: "status")
// from a text list of towers, one per line: ieta iphi status
// Towers not in the list get status 0. CaloTowerStatus masks towers with status > 0.
// -----------------------------------------------------------------------------
void CreateGlobalBadTowerMap(const std::string &inputList,
                             const std::string &detector,
                             const std::string &outputFileName = "")
{
  BadTowerDetectorConfig cfg;
  if (!getBadTowerDetectorConfig(detector, cfg))
  {
    std::cerr << std::format("Error: Unknown detector type '{}'.\n", detector);
    printSupportedBadTowerDetectors();
    return;
  }

  std::ifstream input(inputList);
  if (!input.is_open())
  {
    std::cerr << std::format("Error: Could not open input list '{}'.\n", inputList);
    return;
  }

  const std::string fieldName = "status";

  // key -> status
  std::map<unsigned int, int> badTowers;

  std::string line;
  int lineNum = 0;
  int nErrors = 0;
  while (std::getline(input, line))
  {
    ++lineNum;

    // allow comma separated values
    std::replace(line.begin(), line.end(), ',', ' ');

    // skip blank lines and comments
    auto first = line.find_first_not_of(" \t\r");
    if (first == std::string::npos || line[first] == '#')
    {
      continue;
    }

    std::istringstream iss(line);
    int ieta = 0;
    int iphi = 0;
    int status = 0;
    if (!(iss >> ieta >> iphi >> status))
    {
      std::cerr << std::format("Error: Line {}: Malformed entry '{}', expected 'ieta iphi status'. Skipping.\n", lineNum, line);
      ++nErrors;
      continue;
    }

    if (ieta < 0 || std::cmp_greater_equal(ieta, cfg.nEta) || iphi < 0 || std::cmp_greater_equal(iphi, cfg.nPhi))
    {
      std::cerr << std::format("Error: Line {}: Tower (ieta={}, iphi={}) out of range for {} (ieta < {}, iphi < {}). Skipping.\n",
                               lineNum, ieta, iphi, cfg.name, cfg.nEta, cfg.nPhi);
      ++nErrors;
      continue;
    }

    if (status <= 0)
    {
      std::cerr << std::format("Warning: Line {}: Tower (ieta={}, iphi={}) has status {} <= 0, it will not be masked.\n",
                               lineNum, ieta, iphi, status);
    }

    unsigned int key = cfg.encodeFunc(static_cast<unsigned int>(ieta), static_cast<unsigned int>(iphi));

    if (badTowers.contains(key))
    {
      std::cerr << std::format("Warning: Line {}: Duplicate tower (ieta={}, iphi={}), overwriting status {} with {}.\n",
                               lineNum, ieta, iphi, badTowers[key], status);
    }

    badTowers[key] = status;
  }

  std::string outFile = outputFileName;
  if (outFile.empty())
  {
    outFile = std::format("{}_GlobalBadTowerMap.root", cfg.name);
  }

  // Create parent directory if needed
  std::filesystem::path outPath(outFile);
  if (outPath.has_parent_path())
  {
    std::error_code ec;
    std::filesystem::create_directories(outPath.parent_path(), ec);
    if (ec)
    {
      std::cerr << std::format("Error: Failed to create directory '{}': {}\n",
                               outPath.parent_path().string(), ec.message());
      return;
    }
  }

  auto cdbttree = std::make_unique<CDBTTree>(outFile);

  std::map<int, int> statusCounts;
  for (unsigned int ieta = 0; ieta < cfg.nEta; ++ieta)
  {
    for (unsigned int iphi = 0; iphi < cfg.nPhi; ++iphi)
    {
      unsigned int key = cfg.encodeFunc(ieta, iphi);

      int status = 0;
      if (auto it = badTowers.find(key); it != badTowers.end())
      {
        status = it->second;
        ++statusCounts[status];
      }

      cdbttree->SetIntValue(static_cast<int>(key), fieldName, status);
    }
  }

  cdbttree->Commit();
  cdbttree->WriteCDBTTree();

  std::cout << std::format("==================================================\n"
                           "Created global bad tower map:\n"
                           "  Detector:     {} ({} channels)\n"
                           "  Input list:   {}\n"
                           "  Output file:  {}\n"
                           "  Field:        {}\n"
                           "  Bad towers:   {}\n"
                           "  Skipped lines: {}\n",
                           cfg.name, cfg.nEta * cfg.nPhi, inputList, outFile, fieldName, badTowers.size(), nErrors);

  for (const auto &[status, count] : statusCounts)
  {
    std::cout << std::format("    - status {:<4}: {} towers\n", status, count);
  }
  std::cout << "==================================================\n";
}

// -----------------------------------------------------------------------------
// Usage guide when invoked without arguments
// -----------------------------------------------------------------------------
void CreateGlobalBadTowerMap()
{
  std::cout << "Usage:\n"
            << "  CreateGlobalBadTowerMap(inputList, detector, [outputFileName])\n\n"
            << "Arguments:\n"
            << "  inputList:      Text file with one tower per line: 'ieta iphi status'\n"
            << "                  (space or comma separated, '#' starts a comment line)\n"
            << "  detector:       Detector name: \"CEMC\", \"HCALIN\", or \"HCALOUT\"\n"
            << "  outputFileName: (Optional) Output ROOT file name. Defaults to '{detector}_GlobalBadTowerMap.root'\n\n"
            << "Towers not listed get status 0. CaloTowerStatus masks towers with status > 0.\n\n"
            << "Examples:\n"
            << "  CreateGlobalBadTowerMap(\"bad_cemc.txt\", \"CEMC\")\n"
            << "  CreateGlobalBadTowerMap(\"bad_ohcal.txt\", \"HCALOUT\", \"output/HCALOUT_GlobalBadTowerMap.root\")\n\n";
  printSupportedBadTowerDetectors();
}
