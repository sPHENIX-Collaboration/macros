#ifndef SYS_CALO_C
#define SYS_CALO_C
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

#include <ffamodules/CDBInterface.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalo_reco.so)

// draft macro... put all info under namespace, so user only need to turn on and off the option

namespace CALOSYS
{

  std::vector<std::string> EMCalinputprefix = {
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_"};
  std::vector<std::string> EMCaloutputprefix = {
      "TOWERINFO_CALIB_SYST1_",
      "TOWERINFO_CALIB_SYST2_",
      "TOWERINFO_CALIB_SYST3_"};

  std::vector<std::string> EMCalpayload = {
      "CEMC_stat_syst",
      "CEMC_shift_syst",
      "CEMC_v1Modulation_syst"};
  std::vector<bool> EMCaldosys = {
      true,
      true,
      true};

  std::vector<std::string> OHCalinputprefix = {
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_"};
  std::vector<std::string> OHCaloutputprefix = {
      "TOWERINFO_CALIB_SYST1_",
      "TOWERINFO_CALIB_SYST2_",
      "TOWERINFO_CALIB_SYST3_"};

  std::vector<std::string> OHCalpayload = {
      "HCALOUT_stat_syst",
      "HCALOUT_shift_syst",
      "HCALOUT_v1Modulation_syst"};
  std::vector<bool> OHCaldosys = {
      true,
      false,
      false};

  std::vector<std::string> IHCalinputprefix = {
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_"};
  std::vector<std::string> IHCaloutputprefix = {
      "TOWERINFO_CALIB_SYST1_",
      "TOWERINFO_CALIB_SYST2_",
      "TOWERINFO_CALIB_SYST3_"};

  std::vector<std::string> IHCalpayload = {
      "HCALIN_stat_syst",
      "HCALIN_shift_syst",
      "HCALIN_v1Modulation_syst"};
  std::vector<bool> IHCaldosys = {
      true,
      false,
      false};

}  // namespace CALOSYS

void Register_Tower_sys()
{
  Fun4AllServer *se = Fun4AllServer::instance();
  // EMCal
  for (int i = 0; i < (int) CALOSYS::EMCaldosys.size(); i++)
  {
    if (CALOSYS::EMCaldosys[i])
    {
      // tower calib
      std::cout << "Adding Node:" << CALOSYS::EMCaloutputprefix[i] << "_EMCAL" << std::endl;
      std::string caliburl = CDBInterface::instance()->getUrl(CALOSYS::EMCalpayload[i]);
      CaloTowerCalib *EMCalsys = new CaloTowerCalib("CaloCalib_calo_sysT" + std::to_string(i));
      EMCalsys->set_inputNodePrefix(CALOSYS::EMCalinputprefix[i]);
      EMCalsys->set_outputNodePrefix(CALOSYS::EMCaloutputprefix[i]);
      EMCalsys->set_directURL(caliburl);
      EMCalsys->setFieldName("calo_sys");
      EMCalsys->set_doCalibOnly(true);
      EMCalsys->set_detector_type(CaloTowerDefs::CEMC);
      se->registerSubsystem(EMCalsys);

      // cluster stuff?
    }
  }

  // OHCal
  for (int i = 0; i < (int) CALOSYS::OHCaldosys.size(); i++)
  {
    if (CALOSYS::OHCaldosys[i])
    {
      // tower calib
      std::cout << "Adding Node:" << "_HCALOUT" << std::endl;
      std::string caliburl = CDBInterface::instance()->getUrl(CALOSYS::OHCalpayload[i]);
      CaloTowerCalib *OHCalsys = new CaloTowerCalib();
      OHCalsys->set_inputNodePrefix(CALOSYS::OHCalinputprefix[i]);
      OHCalsys->set_outputNodePrefix(CALOSYS::OHCaloutputprefix[i]);
      OHCalsys->set_directURL(caliburl);
      OHCalsys->setFieldName("calo_sys");
      OHCalsys->set_doCalibOnly(true);
      OHCalsys->set_detector_type(CaloTowerDefs::HCALOUT);
      se->registerSubsystem(OHCalsys);
    }
  }

  // IHCal
  for (int i = 0; i < (int) CALOSYS::IHCaldosys.size(); i++)
  {
    if (CALOSYS::IHCaldosys[i])
    {
      // tower calib
      std::cout << "Adding Node:" << "_HCALIN" << std::endl;
      std::string caliburl = CDBInterface::instance()->getUrl(CALOSYS::IHCalpayload[i]);
      CaloTowerCalib *IHCalsys = new CaloTowerCalib();
      IHCalsys->set_inputNodePrefix(CALOSYS::IHCalinputprefix[i]);
      IHCalsys->set_outputNodePrefix(CALOSYS::IHCaloutputprefix[i]);
      IHCalsys->set_directURL(caliburl);
      IHCalsys->setFieldName("calo_sys");
      IHCalsys->set_doCalibOnly(true);
      IHCalsys->set_detector_type(CaloTowerDefs::HCALIN);
      se->registerSubsystem(IHCalsys);
    }
  }

  std::cout << "All calo systematics added" << std::endl;
}

#endif /* SYS_CALO_C */
