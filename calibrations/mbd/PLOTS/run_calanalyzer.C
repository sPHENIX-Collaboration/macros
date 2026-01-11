#include "CalAnalyzer.C"

#include <format>

void run_calanalyzer(const std::string& runflist = "run24pp.list")
{
  // Load data
  std::ifstream runlistfile( runflist.c_str() );
  if (!runlistfile.is_open())
  {
    std::cerr << "Error: Could not open runlist file " << runflist << std::endl;
    return;
  }

  TString name = runflist.c_str();
  name.ReplaceAll(".list","");

  CalAnalyzer *cal = new CalAnalyzer( name );

  int run{0};
  TString savefname;
  while ( runlistfile >> run )
  {
    if (runlistfile.fail())
    {
      std::cerr << "Warning: Failed to read run number from " << runflist << std::endl;
      break;
    }

    savefname = "results/"; savefname += run; savefname += "/savefitspass2.3_mip-"; savefname += run; savefname += ".txt";
    cal->LoadRun( run, savefname.Data() );
  }

  cal->CalculateCleanStats();

  // Inspect results
  /*
     int ch = 127;
     for (int ipar=0; ipar<9; ipar++)
     {
     cal->DrawSummary(ch, ipar); // Show Hist for Param 0 and Graph for Ch 64/Param 0
     }
     */

  cal->SaveCleanMeans();
  cal->SaveParameterPDFs();
}
