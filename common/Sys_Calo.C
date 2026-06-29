#ifndef SYS_CALO_C
#define SYS_CALO_C

#include <caloreco/CaloTowerCalib.h>

#include <ffamodules/CDBInterface.h>
#include <fun4all/Fun4AllServer.h>
#include <fun4all/SubsysReco.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalo_reco.so)

// =====================================================================
//  SYSTEMATIC CONFIGURATION
// =====================================================================
namespace CALOSYS 
{

struct SysConfig {
  std::string label;                   // systematic name
  std::string payload;                   // CDB payload name root
  bool do_calo[3];                     // calo 1 = EMCal, 2 = IHCal, 3 = OHCal
};

inline std::vector<SysConfig> default_variations()
{
  return {
  {"EMCal calib unc up", "_calib_unc_up_syst", {true, false, false}},
  {"HCal calib unc up", "_calib_unc_up_syst", {false, true, true}},
  {"EMCal calib stat unc", "_calib_stat_syst", {true, false, false}},
  {"IHCal calib stat unc", "_calib_stat_syst", {false, true, false}},
  {"OHCal calib stat unc", "_calib_stat_syst", {false, false, true}},
  //{"EMCal v1Mod", "_v1Modulation_syst", {true, false, false}}, // v1 modulation syst currently not implemented by default
  //{"IHCal v1Mod", "_v1Modulation_syst", {false, true, false}},
  //{"OHCal v1Mod", "_v1Modulation_syst", {false, false, true}},
  {"Had Resp up", "_had_resp_up_syst", {true, true, true}},
  {"EMCal calib unc down", "_calib_unc_down_syst", {true, false, false}},
  {"HCal calib unc down", "_calib_unc_down_syst", {false, true, true}},
  {"Had Resp down", "_had_resp_down_syst", {true, true, true}},
  };
}

static const std::string inputPrefix = "TOWERINFO_CALIB_";
std::string detName[3] = {"CEMC","HCALIN","HCALOUT"};

} // namespace CALOSYS

// =====================================================================
//  RUN ONE SYSTEMATIC INDEX
// =====================================================================
void Register_Tower_sys(int syst_index, const std::vector<CALOSYS::SysConfig>& variations)
{
    size_t nsyst = variations.size();
    if(syst_index < 1 || syst_index > (int)nsyst) {
        std::cerr << "ERROR: Systematic index " << syst_index << " is out of range (1–" << nsyst << ")\n";
        return;
    }

    Fun4AllServer* se = Fun4AllServer::instance();
    const CALOSYS::SysConfig& cfg = variations[syst_index - 1];
    const std::string outputPrefix = "TOWERINFO_CALIB_SYST" + std::to_string(syst_index) + "_";

    for(int ic = 0; ic < 3; ++ic)
    {
      std::string det = CALOSYS::detName[ic];
      std::string fullPayload = det + "_no_calib_syst";
      if (cfg.do_calo[ic]) {
        fullPayload = det + cfg.payload;
      }

      std::string caliburl = CDBInterface::instance()->getUrl(fullPayload);
      CaloTowerCalib* calib = new CaloTowerCalib("CaloCalib_syst_" + det + "_" + std::to_string(syst_index));
      calib->set_inputNodePrefix(CALOSYS::inputPrefix);
      calib->set_outputNodePrefix(outputPrefix);
      calib->set_directURL(caliburl);
      calib->setFieldName("calo_sys");
      calib->set_doCalibOnly(true);

      if(ic == 0)      calib->set_detector_type(CaloTowerDefs::CEMC);
      else if(ic == 1) calib->set_detector_type(CaloTowerDefs::HCALIN);
      else              calib->set_detector_type(CaloTowerDefs::HCALOUT);
      se->registerSubsystem(calib);
    }
}

// =====================================================================
//  RUN ONE DEFAULT SYSTEMATIC
// =====================================================================
void Register_Tower_sys(int syst_index)
{
  auto vars = CALOSYS::default_variations();
  std::cout << ">>> [Sys_Calo] Running ONE default systematic: index SYST" << syst_index << " of " << vars.size() << " (“" << vars[syst_index-1].label << "”)" << std::endl;
  Register_Tower_sys(syst_index, vars);
}

// =====================================================================
//  RUN ALL DEFAULT SYSTEMATICS
// =====================================================================
void Register_Tower_sys()
{
  auto vars = CALOSYS::default_variations();
  std::cout << ">>> [Sys_Calo] Running ALL default systematics (" << vars.size() << " variations)" << std::endl;
  for (size_t i = 1; i <= vars.size(); ++i) {
    std::cout << ">>>   Running SYST" << i << " / " << vars.size() << " (“" << vars[i-1].label << "”)" << std::endl;
    Register_Tower_sys(i, vars);
  }
}

// =====================================================================
//  RUN ALL USER-PROVIDED SYSTEMATICS
// =====================================================================
void Register_Tower_sys(const std::vector<CALOSYS::SysConfig>& vars)
{
  std::cout << ">>> [Sys_Calo] Running ALL USER-PROVIDED systematics (" << vars.size() << " variations)" << std::endl;
  for (size_t i = 1; i <= vars.size(); ++i)
  {
    std::cout << ">>>   Running SYST" << i << " / " << vars.size() << " (“" << vars[i-1].label << "”)" << std::endl;
    Register_Tower_sys(i, vars);
  }
}

#endif /* SYS_CALO_C */
