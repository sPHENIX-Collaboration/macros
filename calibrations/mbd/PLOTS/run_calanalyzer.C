#include "CalAnalyzer.C"

void run_calanalyzer(std::string_view runflist = "run24pp.list")
{
  TString name = runflist.data();
  name.ReplaceAll(".list","");

  CalAnalyzer *cal = new CalAnalyzer( name );

  // Load data
  ifstream runlistfile( runflist.data() );
  int run{0};
  while ( runlistfile >> run )
  {
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
