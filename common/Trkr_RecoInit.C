#ifndef MACRO_TRKRRECOINIT_C
#define MACRO_TRKRRECOINIT_C

R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_TrkrVariables.C>
#include <fun4all/Fun4AllServer.h>
#include <tpc/TpcLoadDistortionCorrection.h>

void TrackingInit()
{
  G4MAGNET::magfield = string(getenv("CALIBRATIONROOT")) + string("/Field/Map/sphenix3dtrackingmapxyz_gap_rebuild.root");
  G4MAGNET::magfield_rescale = 1.;

  ACTSGEOM::ActsGeomInit();
  // space charge correction
  /* corrections are applied in the track finding, and via TpcClusterMover before the final track fit 
   */
  
  if( G4TPC::ENABLE_CORRECTIONS )
  {
    auto se = Fun4AllServer::instance();
    auto tpcLoadDistortionCorrection = new TpcLoadDistortionCorrection;
    tpcLoadDistortionCorrection->set_distortion_filename( G4TPC::correction_filename );
    se->registerSubsystem(tpcLoadDistortionCorrection);
  }
 
}


#endif
