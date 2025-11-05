#include <calo_cdb/genStatus.h>
#include <sphenixnpc/CDBUtils.h>

#include <filesystem>
#include <format>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include <regex>
#include <map>

R__LOAD_LIBRARY(libcalo_cdb.so)
R__LOAD_LIBRARY(libsphenixnpc.so)

class ProductionManager
{
 public:
  ProductionManager(std::string input_list, std::string output_dir, bool force_gen, std::string db_tag);

  void run();

 private:
  // Private helper methods
  void validate_args() const;
  void parse_run_info();
  bool check_cdb_status() const;

  static const std::array<std::string, 7> CDB_NAMES;

  // Map to handle special filename formats
  static const std::map<std::string, std::string> SPECIAL_SUFFIX_FORMATS;

  // Member variables to store state
  std::filesystem::path m_input_list_path;
  std::filesystem::path m_output_dir_path;
  bool m_force_gen;
  std::string m_db_tag;
  std::string m_run;
  std::string m_dataset;
  std::unique_ptr<CDBUtils> m_cdb_utils;
};

const std::array<std::string, 7> ProductionManager::CDB_NAMES = {
    "CEMC_BadTowerMap", "CEMC_meanTime", "CEMC_hotTowers_fracBadChi2",
    "HCALIN_meanTime", "HCALIN_hotTowers_fracBadChi2",
    "HCALOUT_meanTime", "HCALOUT_hotTowers_fracBadChi2"
};

const std::map<std::string, std::string> ProductionManager::SPECIAL_SUFFIX_FORMATS = {
    {"CEMC_BadTowerMap", "EMCalHotMap_{1}_{0}cdb.root"}  // {0} is run, {1} is dataset
};

ProductionManager::ProductionManager(std::string input_list, std::string output_dir, bool force_gen, std::string db_tag)
    : m_input_list_path(std::move(input_list)),
      m_output_dir_path(std::move(output_dir)),
      m_force_gen(force_gen),
      m_db_tag(std::move(db_tag))
{
    validate_args();
    parse_run_info();
    m_cdb_utils = std::make_unique<CDBUtils>();
    m_cdb_utils->setGlobalTag(m_db_tag);
}

void ProductionManager::run() {
    if (!m_force_gen && check_cdb_status()) {
        std::cout << std::format("The CDB already contains the latest calibrations for: {}\n", m_input_list_path.string());
        return;
    }
    GenStatus genStat;
    genStat.process(m_input_list_path.string(), m_output_dir_path.string());
}


void ProductionManager::validate_args() const
{
  // Validate 'input_list'
  // Check if the string is not empty
  if (m_input_list_path.string().empty())
  {
    throw std::invalid_argument("Error: 'input_list' cannot be empty.");
  }

  // Check if the file exists and is a regular file
  if (!std::filesystem::exists(m_input_list_path))
  {
    throw std::invalid_argument(std::format("Error: The file '{}' does not exist.", m_input_list_path.string()));
  }
  if (!std::filesystem::is_regular_file(m_input_list_path))
  {
    throw std::invalid_argument(std::format("Error: The path '{}' is not a regular file.", m_input_list_path.string()));
  }

  // Check if the directory does not exist, then create it
  if (!std::filesystem::exists(m_output_dir_path))
  {
    std::cout << std::format("Output directory '{}' not found. Creating it...\n", m_output_dir_path.string());
    if (!std::filesystem::create_directories(m_output_dir_path))
    {
      throw std::runtime_error(std::format("Error: Failed to create output directory '{}'.", m_output_dir_path.string()));
    }
  }
  // If it does exist, ensure it is a directory and not a file
  else if (!std::filesystem::is_directory(m_output_dir_path))
  {
    throw std::invalid_argument(std::format("Error: The path '{}' exists but is not a directory.", m_output_dir_path.string()));
  }
}

void ProductionManager::parse_run_info()
{
  std::string basename = m_input_list_path.filename().stem().string();
  // Regex to capture a number at the start (run) and the rest (dataset)
  std::regex name_regex("(\\d+)_(.*)");
  std::smatch matches;

  if (std::regex_match(basename, matches, name_regex) && matches.size() == 3)
  {
    m_run = matches[1].str();
    m_dataset = matches[2].str();
  }
  else
  {
    throw std::runtime_error(std::format("Could not parse run number and dataset from filename: {}", basename));
  }
}

bool ProductionManager::check_cdb_status() const
{
  for (const auto &cdbName : CDB_NAMES)
  {
    std::string cdb_url = m_cdb_utils->getUrl(cdbName, std::stoul(m_run));

    std::string suffix;

    // Check if the current cdbName is in our map of special cases
    if (SPECIAL_SUFFIX_FORMATS.contains(cdbName))
    {
      // If yes, use the special format string from the map
      suffix = std::vformat(SPECIAL_SUFFIX_FORMATS.at(cdbName), std::make_format_args(m_run, m_dataset));
    }
    else
    {
      // Otherwise, use the default format
      suffix = std::format("{}_{}_{}.root", cdbName, m_dataset, m_run);
    }

    if (!cdb_url.ends_with(suffix))
    {
      if (cdb_url.starts_with("DataBaseException"))
      {
        std::cout << std::format("No CDB Map Exists for {}.\n", cdbName);
      }
      else
      {
        std::cout << std::format(
            "CDB Map is Outdated for {}.\n"
            "Current: {}\n"
            "Requested: {}",
            cdbName, cdb_url, m_dataset);
      }
      return false;
    }
  }

  // All CDB Maps exists and are the latest
  return true;
}

void runProd(const std::string &input_list,
             const std::string &output_directory = ".",
             const bool forceGen = false,
             const std::string &db_tag = "newcdbtag")
{
  try
  {
    ProductionManager manager(input_list, output_directory, forceGen, db_tag);
    manager.run();
  }
  catch (const std::exception &e)
  {
    std::cout << std::format("An error occurred: {}\n", e.what());
  }
}
