#include "CalAnalyzer.C"

void run_calanalyzer(std::string_view runflist = "run24pp.list")
{
  TString name = runflist.data();
  name.ReplaceAll(".list","");

  CalAnalyzer *cal = new CalAnalyzer( name );

  // Load data
  ifstream runlistfile( runflist.data() );
  if (!runlistfile.is_open())
  {
    std::cerr << "Error: Could not open runlist file " << runflist << std::endl;
    return;
  }

  int run{0};
  while ( runlistfile >> run )
  {
    if (runlistfile.fail())
    {
      std::cerr << "Warning: Failed to read run number from " << runflist << std::endl;
      break;
    }
    cal->LoadRun(run, Form("results/%d/savefitspass2.3_mip-%d.txt", run, run));
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
