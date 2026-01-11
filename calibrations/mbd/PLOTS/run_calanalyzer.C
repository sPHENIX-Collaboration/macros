#include "CalAnalyzer.C"

#include <format>

void run_calanalyzer(const std::string& runflist = "run24pp.list")
{
  // Load data
  std::ifstream runlistfile( runflist.c_str() );  // NOLINT(bugprone-suspicious-stringview-data-usage)
  if (!runlistfile.is_open())
  {
    std::cerr << "Error: Could not open runlist file " << runflist << std::endl;
    return;
  }

  // checks if there is a suffix ".list"
  // if not needed
  // name.erase(name.size() - 5);
  // could be used
  const std::string suffix = ".list";
  std::string name = runflist;
  auto pos = name.rfind(suffix);
  if (pos != std::string::npos)
  {
    name.erase(pos, suffix.length());
  }

  CalAnalyzer *cal = new CalAnalyzer( name );

  int run{0};
  while ( runlistfile >> run )
  {
    if (runlistfile.fail())
    {
      std::cerr << "Warning: Failed to read run number from " << runflist << std::endl;
      break;
    }
    cal->LoadRun(run, std::format("results/{}/savefitspass2.3_mip-{}.txt", run, run));
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
