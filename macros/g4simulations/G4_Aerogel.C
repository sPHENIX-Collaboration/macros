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

void
AerogelSetup(PHG4Reco* g4Reco, const int detectorSetup = 0, //1: full setup; 0:skeleton 
    const int mRICHsystemSetup = 8 //-1: single module
                                   //0: build hemi-spherical wall
                                   //>0: mRICH wall as sector. "subsystemSetup" becomes num. of sector (max is 8)
    )
{
  PHG4mRICHSubsystem *mRICH = new PHG4mRICHSubsystem("mRICH",0);
  mRICH->set_int_param("detectorSetup",detectorSetup);
  mRICH->set_int_param("subsystemSetup",mRICHsystemSetup);
  mRICH->UseCalibFiles(PHG4DetectorSubsystem::xml);
  mRICH->SetCalibrationFileDir(string(getenv("CALIBRATIONROOT")) + string("/Prototype4/Geometry/") );
  mRICH->OverlapCheck(overlapcheck);

  g4Reco->registerSubsystem(mRICH);
}

