#ifndef SYS_CALO_C
#define SYS_CALO_C
#include <caloreco/CaloTowerCalib.h>
#include <caloreco/RawClusterBuilderTemplate.h>

#include <fun4all/Fun4AllServer.h>
#include <fun4all/Fun4AllUtils.h>
#include <fun4all/SubsysReco.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libcalo_reco.so)

//draft macro... put all info under namespace, so user only need to turn on and off the option

namespace CALOSYS{

    std::vector<std::string> EMCalinputprefix = {
      "TOWERINFO_CALIB_", 
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_", 
      "TOWERINFO_CALIB_", 
      "TOWERINFO_CALIB_"
    };
    std::vector<std::string> EMCaloutputprefix = {
      "TOWERINFO_CALIB_SYST1", 
      "TOWERINFO_CALIB_SYST2",
      "TOWERINFO_CALIB_SYST3U", 
      "TOWERINFO_CALIB_SYST3D", 
      "TOWERINFO_CALIB_SYST4"
    };
    std::vector<std::string> EMCalfieldname = {
       "cemc_sys", 
       "cemc_sys",
       "cemc_sys", 
       "cemc_sys", 
       "cemc_sys"
    };
    std::vector<std::string> EMCalcaliburl = {
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/pi0_stat_syst.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/pi0_shift_syst.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/TSC_syst_up.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/TSC_syst_down.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/TSC_stat_syst.root"
    };
    std::vector<bool> EMCaldosys = {
      true, 
      true,
      true,
      true,
      true
    };


    std::vector<std::string> OHCalinputprefix = {
      "TOWERINFO_CALIB_", 
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_", 
      "TOWERINFO_CALIB_"
    };
    std::vector<std::string> OHCaloutputprefix = {
      "TOWERINFO_CALIB_SYST1_", 
      "TOWERINFO_CALIB_SYST2_",
      "TOWERINFO_CALIB_SYST3_", 
      "TOWERINFO_CALIB_SYST4_"
    };
    std::vector<std::string> OHCalfieldname = {
       "ohcal_sys", 
       "ohcal_sys",
       "ohcal_sys", 
       "ohcal_sys"
    };

    std::vector<std::string> OHCalcaliburl = {
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ohcal_cos_syst_1.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ohcal_cos_syst_2.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ohcal_cos_syst_3.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ohcal_cos_syst_4.root"
    };
    std::vector<bool> OHCaldosys = {
      true, 
      true,
      true,
      true
    };


    std::vector<std::string> IHCalinputprefix = {
      "TOWERINFO_CALIB_", 
      "TOWERINFO_CALIB_",
      "TOWERINFO_CALIB_", 
      "TOWERINFO_CALIB_"
    };
    std::vector<std::string> IHCaloutputprefix = {
      "TOWERINFO_CALIB_SYST1_", 
      "TOWERINFO_CALIB_SYST2_",
      "TOWERINFO_CALIB_SYST3_", 
      "TOWERINFO_CALIB_SYST4_"
    };
    std::vector<std::string> IHCalfieldname = {
       "ihcal_sys", 
       "ihcal_sys",
       "ihcal_sys", 
       "ihcal_sys"
    };

    std::vector<std::string> IHCalcaliburl = {
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ihcal_cos_syst_1.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ihcal_cos_syst_2.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ihcal_cos_syst_3.root",
      "/sphenix/u/bseidlitz/work/macros/calibrations/calo/emcal_calib_year1/systs/ihcal_cos_syst_4.root"
    };
    std::vector<bool> IHCaldosys = {
      true, 
      true,
      true,
      true
    };
/*
    std::vector<std::string> OHCalinputprefix = {"TOWERINFO_CALIB_", "TOWERINFO_CALIB_"};
    std::vector<std::string> OHCaloutputprefix = {"TOWERINFO_CALIB_SYSALL", "TOWERINFO_CALIB_SYSETA"};
    std::vector<std::string> OHCalfieldname = {"ohcal_sys", "ohcal_sys"};
    std::vector<std::string> OHCalcaliburl = {"/sphenix/user/shuhangli/DB/ohcalAllSys.root", "/sphenix/user/shuhangli/DB/ohcalEtaSys.root"};
    std::vector<bool> OHCaldosys = {true, true};
    std::vector<std::string> IHCalinputprefix = {"TOWERINFO_CALIB_", "TOWERINFO_CALIB_"};
    std::vector<std::string> IHCaloutputprefix = {"TOWERINFO_CALIB_SYSALL", "TOWERINFO_CALIB_SYSETA"};
    std::vector<std::string> IHCalfieldname = {"ihcal_sys", "ihcal_sys"};
    std::vector<std::string> IHCalcaliburl = {"/sphenix/user/shuhangli/DB/ihcalAllSys.root", "/sphenix/user/shuhangli/DB/ihcalEtaSys.root"};
    std::vector<bool> IHCaldosys = {true, true};
*/


}//namespace CALOSYS

void Register_Tower_sys(){
    Fun4AllServer *se = Fun4AllServer::instance();
    //EMCal
    for (int i = 0; i< (int) CALOSYS::EMCaldosys.size(); i++){
        if (CALOSYS::EMCaldosys[i]){
            //tower calib
            std::cout<< "Adding Node:"<< CALOSYS::EMCaloutputprefix[i] <<"_EMCAL"<<std::endl;
            CaloTowerCalib *EMCalsys = new CaloTowerCalib(Form("CaloCalib_CEMC_SYST%d",i));
            EMCalsys->set_inputNodePrefix(CALOSYS::EMCalinputprefix[i]);
            EMCalsys->set_outputNodePrefix(CALOSYS::EMCaloutputprefix[i]);
            EMCalsys->set_directURL(CALOSYS::EMCalcaliburl[i]);
            EMCalsys->setFieldName(CALOSYS::EMCalfieldname[i]);
            EMCalsys->set_detector_type(CaloTowerDefs::CEMC);
            se->registerSubsystem(EMCalsys);

        //cluster stuff?
        }
    }

    //OHCal
    for (int i = 0; i< (int) CALOSYS::OHCaldosys.size(); i++){
        if (CALOSYS::OHCaldosys[i]){
            //tower calib
            std::cout<< "Adding Node:"<< CALOSYS::OHCalfieldname[i] <<"_HCALOUT"<<std::endl;
            CaloTowerCalib *OHCalsys = new CaloTowerCalib(CALOSYS::OHCalfieldname[i]);
            OHCalsys->set_inputNodePrefix(CALOSYS::OHCalinputprefix[i]);
            OHCalsys->set_outputNodePrefix(CALOSYS::OHCaloutputprefix[i]);
            OHCalsys->set_directURL(CALOSYS::OHCalcaliburl[i]);
            OHCalsys->setFieldName(CALOSYS::OHCalfieldname[i]);
            OHCalsys->set_detector_type(CaloTowerDefs::HCALOUT);
            se->registerSubsystem(OHCalsys);

        }
    }

    //IHCal
    for (int i = 0; i< (int) CALOSYS::IHCaldosys.size(); i++){
        if (CALOSYS::IHCaldosys[i]){
            //tower calib
            std::cout<< "Adding Node:"<< CALOSYS::IHCalfieldname[i] <<"_HCALIN"<<std::endl;
            CaloTowerCalib *IHCalsys = new CaloTowerCalib(CALOSYS::IHCalfieldname[i]);
            IHCalsys->set_inputNodePrefix(CALOSYS::IHCalinputprefix[i]);
            IHCalsys->set_outputNodePrefix(CALOSYS::IHCaloutputprefix[i]);
            IHCalsys->set_directURL(CALOSYS::IHCalcaliburl[i]);
            IHCalsys->setFieldName(CALOSYS::IHCalfieldname[i]);
            IHCalsys->set_detector_type(CaloTowerDefs::HCALIN);
            se->registerSubsystem(IHCalsys);

        }
    }

  std::cout << "All calo systematics added" << std::endl;
}

#endif /* SYS_CALO_C */
