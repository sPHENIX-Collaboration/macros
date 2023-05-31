#ifndef MACRO_TRKRTRUTHTABLES_C
#define MACRO_TRKRTRUTHTABLES_C

#include <GlobalVariables.C>

#include <g4eval/SvtxTruthRecoTableEval.h>

R__LOAD_LIBRARY(libg4eval.so)

void build_truthreco_tables()
{
  int verbosity = std::max(Enable::VERBOSITY, Enable::TRACKING_VERBOSITY);
  Fun4AllServer* se = Fun4AllServer::instance();
  
  // this module builds high level truth track association table.
  // If this module is used, this table should be called before any evaluator calls.
  // Removing this module, evaluation will still work but trace truth association through the layers of G4-hit-cluster
  SvtxTruthRecoTableEval *tables = new SvtxTruthRecoTableEval();
  tables->Verbosity(verbosity);
  se->registerSubsystem(tables);

  return;
}

#endif
