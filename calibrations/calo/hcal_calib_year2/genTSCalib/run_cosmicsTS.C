#include "../genFinalCalib/tsc_cos_merge.C"

#include <litecaloeval/LiteCaloEval.h>

#include <Rtypes.h> // for R__LOAD_LIBRARY macro
#include <TSystem.h>

#include <format>
#include <fstream>
#include <string>

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

//make sure TS_list has relative or full paths to actual root files
void run_cosmicsTS(const std::string &runs = "runList.txt", const std::string &TS_list= "lce_output_files.txt", bool run_ohcal=false, bool run_ihcal=false)
{

  gSystem->mkdir("fitResults", true);
  gSystem->mkdir("cdbFiles", true); 

  std::ifstream runlist(runs);
  std::string line;

  std::ifstream tsList(TS_list);
  std::string line2;

  //read from run txt file and txt file w/ lce histos at the same time
  while(getline(runlist,line) && getline(tsList,line2))
  {

    std::cout << "=============================================" << std::endl;
    std::cout << "Processing run number: " << line << std::endl;

    if (line.empty() || line2.empty()) continue;  // Skip blank lines

    std::cout << "analysing file " << line2 << std::endl;


    if (run_ohcal){

    std::cout << "Running analysis for **HCALOUT**..." << std::endl;

    std::string oh_out = "fitResults/oh_cosmicsMerged_fitted_" + line + ".root";
  
    LiteCaloEval hout;
    hout.CaloType(LiteCaloEval::HCALOUT);
    hout.set_spectra_binWidth(0.08);
    hout.Get_Histos(line2,oh_out);
    hout.setFitMin(0.4);//GeV
    hout.setFitMax(2.5);
    hout.FitRelativeShifts(&hout,110);
    hout.draw_spectra(oh_out.c_str());
    hout.fit_info(oh_out.c_str(), stoi(line));
    
    std::string cosmic_CDB_file_oh = std::format("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cosmicCalibFiles/ohcal_cosmic_calibration_{}.root",line);
    
    //modify the cosmics calibration file based on tsc fit output
    tsc_cos_merge(oh_out,cosmic_CDB_file_oh,std::format("cdbFiles/ohcal_cdb_tsc_cos_calib_{}.root",line),0);
    
    }

    if (run_ihcal){

    std::cout << "Running analysis for **HCALIN**..." << std::endl;

    std::string ih_out = "fitResults/ih_cosmicsMerged_fitted_" + line + ".root";
    
    LiteCaloEval hin;
    hin.CaloType(LiteCaloEval::HCALIN);
    hin.set_spectra_binWidth(0.05);
    hin.Get_Histos(line2, ih_out);
    hin.setFitMin(0.20);
    hin.setFitMax(1.5);
    hin.set_doQA(true);
    hin.FitRelativeShifts(&hin,110);
    hin.draw_spectra(ih_out.c_str());
    //hin.fit_info(ih_out.c_str(), stoi(line));
    

    std::string cosmic_CDB_file_ih = std::format("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cosmicCalibFiles/ihcal_cosmic_calibration_{}.root",line);
   
    //modify the cosmics calibration file based on tsc fit output
    tsc_cos_merge(ih_out,cosmic_CDB_file_ih,std::format("cdbFiles/ihcal_cdb_tsc_cos_calib_{}.root",line),1);
    }

  }

  std::cout << "Macro is done executing." << std::endl;
  
}//end macro
