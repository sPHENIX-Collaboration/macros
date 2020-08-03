#ifndef MACRO_G4PSTOF_C
#define MACRO_G4PSTOF_C

#include <g4detectors/PHG4PSTOFSubsystem.h>
#include <g4main/PHG4Reco.h>

R__LOAD_LIBRARY(libg4detectors.so)

// $Id$

/*!
 * \file ${file_name}
 * \brief Macro setting up the barrel mRPC psTOF
 * \author Mickey Chiu <chiu@bnl.gov>
 * \version $Revision$
 * \date $Date$
 */
namespace Enable
{
  bool PSTOF = false;
}

void PSTOFInit()
{
}

double PSTOF(PHG4Reco* g4Reco, double radius)
{
  PHG4PSTOFSubsystem* pstof = new PHG4PSTOFSubsystem("PSTOF");
  pstof->SuperDetector("PSTOF");
  //  pstof->SetActive(-1,1); // set all modules active
  g4Reco->registerSubsystem(pstof);

  // returns the outer radius to check for overlaps with next detector
  return 90.;
}
#endif
