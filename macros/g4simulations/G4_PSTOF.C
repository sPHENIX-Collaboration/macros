// $Id$

/*!
 * \file ${file_name}
 * \brief Macro setting up the barrel mRPC psTOF
 * \author Mickey Chiu <chiu@bnl.gov>
 * \version $Revision$
 * \date $Date$
 */

#include <cmath>

void PSTOFInit()
{
  std::cout << "In PSTOFInit()" << std::endl;
}

double PSTOF(PHG4Reco* g4Reco, double radius, const int absorberactive = 0)
{
  std::cout << "In PSTOF" << std::endl;

  gSystem->Load("libg4detectors.so");
  gSystem->Load("libg4testbench.so");
  
  /*
  const double psTOF_R = 85.0;
  const double length = 2*74.0;
  const double z_start = 0.5*length;
  const double z_end = -0.5*length;
  */

  PHG4PSTOFSubsystem *pstof = new PHG4PSTOFSubsystem( "PSTOF" );
  pstof->set_int_param("active",1);
  pstof->set_int_param("use_g4steps",0);
  //pstof->OverlapCheck( overlapcheck );
 
  g4Reco->registerSubsystem( pstof );

  return 90.; // returns the radius??
}
