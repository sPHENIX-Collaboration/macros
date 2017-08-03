// $Id$

/*!
 * \file ${file_name}
 * \brief Macro setting up the barrel mRPC psTOF
 * \author Mickey Chiu <chiu@bnl.gov>
 * \version $Revision$
 * \date $Date$
 */

void PSTOFInit()
{}

double PSTOF(PHG4Reco* g4Reco, double radius, const int absorberactive = 0)
{
  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");
  

  PHG4PSTOFSubsystem *pstof = new PHG4PSTOFSubsystem( "PSTOF" );
  pstof->SuperDetector("PSTOF");
//  pstof->SetActive(-1,1); // set all modules active
  g4Reco->registerSubsystem( pstof );

// returns the outer radius to check for overlaps with next detector
  return 90.; 
}
