#include <string>
#include <litecaloeval/LiteCaloEval.h>
#include"../genFinalCalib/tsc_cos_merge.C"

R__LOAD_LIBRARY(libLiteCaloEvalTowSlope.so)

//make sure TS_list has relative or full paths to actual root files
void run_cosmicsTS(const char *runs = "runList.txt", const char *TS_list= "lce_output_files.txt")
{
  ifstream runlist(runs);
  std::string line;

  ifstream tsList(TS_list);
  std::string line2;

  //read from run txt file and txt file w/ lce histos at the same time
  while(getline(runlist,line) && getline(tsList,line2))
  {
    
    std::string oh_out = "fitResults/oh_cosmicsMerged_fitted_" + line + ".root";
    std::string ih_out = "fitResults/ih_cosmicsMerged_fitted_" + line + ".root";

    cout << "analysing file " << line2 << endl;
  
    LiteCaloEval hout;
    hout.CaloType(LiteCaloEval::HCALOUT);
    hout.set_spectra_binWidth(0.08);
    hout.Get_Histos(line2,oh_out.c_str());
    hout.setFitMin(0.4);//GeV
    hout.setFitMax(2.5);
    hout.FitRelativeShifts(&hout,110);
    //hout.draw_spectra(oh_out.c_str());
    //hout.fit_info(oh_out.c_str(), stoi(line));
    
    LiteCaloEval hin;
    hin.CaloType(LiteCaloEval::HCALIN);
    hin.set_spectra_binWidth(0.05);
    hin.Get_Histos(line2, ih_out.c_str());
    hin.setFitMin(0.20);
    hin.setFitMax(1.5);
    hin.set_doQA(true);
    //hin.FitRelativeShifts(&hin,110);
    //hin.draw_spectra(ih_out.c_str());
    //hin.fit_info(ih_out.c_str(), stoi(line));
    

    string cosmic_CDB_file_oh = Form("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cosmicCalibFiles/ohcal_cosmic_calibration_%s.root",line.c_str());
    //string cosmic_CDB_file_ih = Form("/sphenix/u/bseidlitz/work/macros/calibrations/calo/hcal_towerSlope_y2/cosmicCalibFiles/ihcal_cosmic_calibration_%s.root",line.c_str());

    // modify the cosmics calibration file based on tsc fit output
    tsc_cos_merge(oh_out,cosmic_CDB_file_oh,Form("cdbFiles/ohcal_cdb_tsc_cos_calib_%s.root",line.c_str()),0);
    //tsc_cos_merge(ih_out,cosmic_CDB_file_ih,Form("ihcal_cdb_tsc_cos_calib_%s.root",line.c_str()),1);

  }

 std:cout << "Macro is done executing." << std::endl;
  
}//end macro
