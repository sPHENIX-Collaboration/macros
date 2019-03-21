#pragma once
#if ROOT_VERSION_CODE >= ROOT_VERSION(6,00,0)
#include "GlobalVariables.C"
#include <g4detectors/PHG4mRICHSubsystem.h>
#include <g4main/PHG4Reco.h>
R__LOAD_LIBRARY(libg4detectors.so)
#endif
// $Id: G4_Aerogel.C,v 1.2 2013/10/09 01:08:17 jinhuang Exp $

/*!
 * \file G4_Aerogel.C
 * \brief Aerogel RICH for EIC detector
 * \author Jin Huang <jhuang@bnl.gov>
 * \version $Revision: 1.2 $
 * \date $Date: 2013/10/09 01:08:17 $
 */

void
AerogelInit()
{

}

//-1: single module
// 0: build h-side sectors and e-side wall
// 1: build h-side sectors
// 2: build e-side wall
// 3: build h-side wall
// 4: build h-side wall and e-side wall
void
AerogelSetup(PHG4Reco* g4Reco, const int detectorSetup = 0, //1: full setup; 0:skeleton 
    const int mRICHsystemSetup = 0 
    )
{
  PHG4mRICHSubsystem *mRICH = new PHG4mRICHSubsystem("mRICH",0);
  mRICH->set_int_param("detectorSetup",detectorSetup);
  mRICH->set_int_param("subsystemSetup",mRICHsystemSetup);
  mRICH->UseCalibFiles(PHG4DetectorSubsystem::xml);
  mRICH->SetCalibrationFileDir(string(getenv("CALIBRATIONROOT")) + string("/mRICH/Geometry/") );
  mRICH->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem(mRICH);
}

