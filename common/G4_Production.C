#ifndef MACRO_G4PRODUCTION_C
#define MACRO_G4PRODUCTION_C

#include <GlobalVariables.C>

#include <TSystem.h>

namespace Enable
{
  bool PRODUCTION = false;
}

namespace PRODUCTION
{
  std::string SaveOutputDir = "./";
}

void Production_CreateOutputDir()
{
  PRODUCTION::SaveOutputDir = DstOut::OutputDir;
  DstOut::OutputDir = PRODUCTION::SaveOutputDir + "/ongoing";
  std::string mkdircmd = "mkdir -p " + DstOut::OutputDir;
  gSystem->Exec(mkdircmd.c_str());
}

void Production_MoveOutput()
{
  if (Enable::DSTOUT)
  {
    std::string fulloutfile = DstOut::OutputDir + "/" + DstOut::OutputFile;
    std::string mvcmd = "mv " + fulloutfile + " " + PRODUCTION::SaveOutputDir;
    gSystem->Exec(mvcmd.c_str());
  }
}
#endif
