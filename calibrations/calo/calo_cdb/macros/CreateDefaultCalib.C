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
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#if defined(__CLING__)
R__LOAD_LIBRARY(libcalo_io.so)
R__LOAD_LIBRARY(libcdbobjects.so)
#endif

// -----------------------------------------------------------------------------
// Helper structure to represent a calibration field definition
// -----------------------------------------------------------------------------
struct CalibField
{
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::string name;
  double value{0.0};
  std::string type{"float"};  // "int", "float", or "double"
  // NOLINTEND(misc-non-private-member-variables-in-classes)

  CalibField(const std::string &n, int val)
    : name(n)
    , value(static_cast<double>(val))
    , type("int")
  {
  }

  CalibField(const std::string &n, float val)
    : name(n)
    , value(static_cast<double>(val))
    , type("float")
  {
  }

  CalibField(const std::string &n, double val, const std::string &t = "float")
    : name(n)
    , value(val)
    , type(t)
  {
  }
};

// -----------------------------------------------------------------------------
// Detector configuration mapping
// -----------------------------------------------------------------------------
struct DetectorConfig
{
  // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
  std::string name;
  int nChannels{0};
  std::function<unsigned int(unsigned int)> encodeFunc;
  // NOLINTEND(misc-non-private-member-variables-in-classes)
};

inline void printSupportedDetectors()
{
  std::cerr << "Supported detector types are:\n"
            << "  - CEMC / EMCAL     (24576 channels, TowerInfoDefs::encode_emcal)\n"
            << "  - HCALIN / IHCAL   (1536 channels, TowerInfoDefs::encode_hcal)\n"
            << "  - HCALOUT / OHCAL  (1536 channels, TowerInfoDefs::encode_hcal)\n"
            << "  - HCAL             (1536 channels, TowerInfoDefs::encode_hcal)\n"
            << "  - SEPD / EPD       (744 channels, TowerInfoDefs::encode_epd)\n"
            << "  - MBD / BBC        (256 channels, TowerInfoDefs::encode_mbd)\n"
            << "  - ZDC              (52 channels, TowerInfoDefs::encode_zdc)\n"
            << "  - RAW              (1:1 channel indexing)\n";
}

inline bool getDetectorConfig(const std::string &detector, DetectorConfig &cfg)
{
  std::string det = detector;
  std::transform(det.begin(), det.end(), det.begin(), ::toupper);

  if (det == "CEMC" || det == "EMCAL" || det == "EMC")
  {
    cfg = {"CEMC", 24576, [](unsigned int ch) { return TowerInfoDefs::encode_emcal(ch); }};
    return true;
  }
  if (det == "HCALIN" || det == "IHCAL" || det == "INNER_HCAL" || det == "IH")
  {
    cfg = {"HCALIN", 1536, [](unsigned int ch) { return TowerInfoDefs::encode_hcal(ch); }};
    return true;
  }
  if (det == "HCALOUT" || det == "OHCAL" || det == "OUTER_HCAL" || det == "OH")
  {
    cfg = {"HCALOUT", 1536, [](unsigned int ch) { return TowerInfoDefs::encode_hcal(ch); }};
    return true;
  }
  if (det == "HCAL")
  {
    cfg = {"HCAL", 1536, [](unsigned int ch) { return TowerInfoDefs::encode_hcal(ch); }};
    return true;
  }
  if (det == "SEPD" || det == "EPD")
  {
    cfg = {"SEPD", 744, [](unsigned int ch) { return TowerInfoDefs::encode_epd(ch); }};
    return true;
  }
  if (det == "MBD" || det == "BBC")
  {
    cfg = {"MBD", 256, [](unsigned int ch) { return TowerInfoDefs::encode_mbd(ch); }};
    return true;
  }
  if (det == "ZDC")
  {
    cfg = {"ZDC", 52, [](unsigned int ch) { return TowerInfoDefs::encode_zdc(ch); }};
    return true;
  }
  if (det == "RAW")
  {
    cfg = {"RAW", 1536, [](unsigned int ch) { return ch; }};
    return true;
  }

  return false;
}

// -----------------------------------------------------------------------------
// Core implementation supporting multiple fields
// -----------------------------------------------------------------------------
void CreateDefaultCalib(const std::vector<CalibField> &fields,
                        const std::string &detector,
                        const std::string &outputFileName = "")
{
  if (fields.empty())
  {
    std::cerr << "Error: No calibration fields specified.\n";
    return;
  }

  DetectorConfig cfg;
  if (!getDetectorConfig(detector, cfg))
  {
    std::cerr << std::format("Error: Unknown detector type '{}'.\n", detector);
    printSupportedDetectors();
    return;
  }


  enum class FieldType
  {
    Int,
    Float,
    Double
  };

  struct NormalizedField
  {
    // NOLINTBEGIN(misc-non-private-member-variables-in-classes)
    std::string name;
    double value{0.0};
    FieldType type{FieldType::Float};
    // NOLINTEND(misc-non-private-member-variables-in-classes)
  };

  std::vector<NormalizedField> normFields;
  normFields.reserve(fields.size());

  for (const auto &f : fields)
  {
    if (f.name.empty())
    {
      std::cerr << "Error: Field name cannot be empty.\n";
      return;
    }

    std::string t = f.type;
    std::transform(t.begin(), t.end(), t.begin(), ::tolower);

    FieldType ft = FieldType::Float;
    if (t == "int" || t == "integer" || t == "i")
    {
      ft = FieldType::Int;
    }
    else if (t == "float" || t == "f")
    {
      ft = FieldType::Float;
    }
    else if (t == "double" || t == "d")
    {
      ft = FieldType::Double;
    }
    else
    {
      std::cerr << std::format("Error: Unsupported storage type '{}' for field '{}'.\n"
                               "Supported types are: \"float\", \"int\", \"double\".\n",
                               f.type, f.name);
      return;
    }

    normFields.push_back({f.name, f.value, ft});
  }


  std::string outFile = outputFileName;
  if (outFile.empty())
  {
    outFile = std::format("{}_{}_default.root", cfg.name, fields.front().name);
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

  std::cout << std::format("==================================================\n"
                           "Creating CDB default calibration tree:\n"
                           "  Detector:    {} ({} channels)\n"
                           "  Output file: {}\n"
                           "  Fields:\n",
                           cfg.name, cfg.nChannels, outFile);

  for (const auto &f : fields)
  {
    std::cout << std::format("    - Name: {:<20} Type: {:<8} Value: {}\n", f.name, f.type, f.value);
  }
  std::cout << "==================================================\n";

  auto cdbttree = std::make_unique<CDBTTree>(outFile);

  for (int channel = 0; channel < cfg.nChannels; ++channel)
  {
    unsigned int key = cfg.encodeFunc(channel);

    for (const auto &nf : normFields)
    {
      switch (nf.type)
      {
        case FieldType::Int:
          cdbttree->SetIntValue(static_cast<int>(key), nf.name, static_cast<int>(nf.value));
          break;
        case FieldType::Double:
          cdbttree->SetDoubleValue(static_cast<int>(key), nf.name, nf.value);
          break;
        case FieldType::Float:
          cdbttree->SetFloatValue(static_cast<int>(key), nf.name, static_cast<float>(nf.value));
          break;
      }
    }
  }

  cdbttree->Commit();
  cdbttree->WriteCDBTTree();

  std::cout << std::format("Successfully wrote CDB tree with {} channels to '{}'.\n",
                           cfg.nChannels, outFile);
}

// -----------------------------------------------------------------------------
// Primary user-facing overloads: (fieldName, defaultValue, detector, outputFileName)
// -----------------------------------------------------------------------------

// Integer default value (e.g. status = 0)
void CreateDefaultCalib(const std::string &fieldName,
                        int defaultValue,
                        const std::string &detector,
                        const std::string &outputFileName = "")
{
  CreateDefaultCalib({CalibField(fieldName, defaultValue)}, detector, outputFileName);
}

// Single-precision float default value
void CreateDefaultCalib(const std::string &fieldName,
                        float defaultValue,
                        const std::string &detector,
                        const std::string &outputFileName = "")
{
  CreateDefaultCalib({CalibField(fieldName, defaultValue)}, detector, outputFileName);
}

// Double-precision / general float value with optional type override ("float", "int", "double")
void CreateDefaultCalib(const std::string &fieldName,
                        double defaultValue,
                        const std::string &detector,
                        const std::string &outputFileName = "",
                        const std::string &valType = "float")
{
  CreateDefaultCalib({CalibField(fieldName, defaultValue, valType)}, detector, outputFileName);
}

// String-based value overload (handy when invoking from bash/python wrappers)
void CreateDefaultCalib(const std::string &fieldName,
                        const std::string &defaultValueStr,
                        const std::string &detector,
                        const std::string &outputFileName = "",
                        const std::string &valType = "")
{
  std::string t = valType;
  std::transform(t.begin(), t.end(), t.begin(), ::tolower);

  try
  {
    size_t pos = 0;

    // Explicit type requested
    if (t == "int" || t == "integer" || t == "i")
    {
      double dval = std::stod(defaultValueStr, &pos);
      if (pos != defaultValueStr.size())
      {
        std::cerr << std::format("Error: Trailing characters in integer value '{}'.\n", defaultValueStr);
        return;
      }
      int val = static_cast<int>(dval);
      CreateDefaultCalib({CalibField(fieldName, val)}, detector, outputFileName);
      return;
    }
    if (t == "double" || t == "d" || t == "float" || t == "f")
    {
      double val = std::stod(defaultValueStr, &pos);
      if (pos != defaultValueStr.size())
      {
        std::cerr << std::format("Error: Trailing characters in floating-point value '{}'.\n", defaultValueStr);
        return;
      }
      CreateDefaultCalib({CalibField(fieldName, val, t)}, detector, outputFileName);
      return;
    }
    if (!t.empty())
    {
      std::cerr << std::format("Error: Unsupported storage type '{}'. Supported types: \"float\", \"int\", \"double\".\n", valType);
      return;
    }

    // Auto-detect type: check for floating-point indicators ('.', 'e', 'E')
    bool isFloat = (defaultValueStr.find('.') != std::string::npos ||
                    defaultValueStr.find('e') != std::string::npos ||
                    defaultValueStr.find('E') != std::string::npos);

    if (isFloat)
    {
      double val = std::stod(defaultValueStr, &pos);
      if (pos != defaultValueStr.size())
      {
        std::cerr << std::format("Error: Trailing characters in floating-point value '{}'.\n", defaultValueStr);
        return;
      }
      CreateDefaultCalib({CalibField(fieldName, val, "float")}, detector, outputFileName);
    }
    else
    {
      double dval = std::stod(defaultValueStr, &pos);
      if (pos != defaultValueStr.size())
      {
        std::cerr << std::format("Error: Trailing characters in integer value '{}'.\n", defaultValueStr);
        return;
      }
      int val = static_cast<int>(dval);
      CreateDefaultCalib({CalibField(fieldName, val)}, detector, outputFileName);
    }
  }
  catch (const std::exception &e)
  {
    std::cerr << std::format("Error: Failed to parse '{}' as a valid number: {}\n", defaultValueStr, e.what());
  }
}

// -----------------------------------------------------------------------------
// Convenience helper to reproduce default HCal maps in a single call
// -----------------------------------------------------------------------------
void CreateDefaultHCalMapsGen(const std::string &outputDir = ".")
{
  std::string hcalin_hotmap = std::format("{}/HCALIN_HotMap_default.root", outputDir);
  std::string hcalout_hotmap = std::format("{}/HCALOUT_HotMap_default.root", outputDir);

  CreateDefaultCalib({CalibField("status", 0), CalibField("HCALIN_sigma", 0.0F)}, "HCALIN", hcalin_hotmap);
  CreateDefaultCalib({CalibField("status", 0), CalibField("HCALOUT_sigma", 0.0F)}, "HCALOUT", hcalout_hotmap);
}

// -----------------------------------------------------------------------------
// Usage guide when invoked without arguments
// -----------------------------------------------------------------------------
void CreateDefaultCalib()
{
  std::cout << "Usage:\n"
            << "  CreateDefaultCalib(fieldName, defaultValue, detector, outputFileName, [valType])\n\n"
            << "Arguments:\n"
            << "  fieldName:      Name of calibration field (e.g. \"status\", \"time\", \"calib\")\n"
            << "  defaultValue:   Default value (int, float, double, or string)\n"
            << "  detector:       Detector name (e.g. \"CEMC\", \"HCALIN\", \"HCALOUT\", \"SEPD\", \"MBD\", \"ZDC\")\n"
            << "  outputFileName: (Optional) Output ROOT file name. Defaults to '{detector}_{fieldName}_default.root'\n"
            << "  valType:        (Optional) Field type: \"float\" (default), \"int\", or \"double\"\n\n"
            << "Examples:\n"
            << "  CreateDefaultCalib(\"status\", 0, \"CEMC\", \"CEMC_status_default.root\")\n"
            << "  CreateDefaultCalib(\"time\", 0.0, \"HCALIN\", \"HCALIN_time_default.root\")\n"
            << "  CreateDefaultCalib(\"calib\", 1.0, \"HCALOUT\", \"HCALOUT_calib_default.root\")\n"
            << "  CreateDefaultCalib({{\"status\", 0}, {\"HCALIN_sigma\", 0.0F}}, \"HCALIN\", \"HCALIN_HotMap_default.root\")\n\n";
  printSupportedDetectors();
}
