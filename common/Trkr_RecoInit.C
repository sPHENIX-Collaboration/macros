#ifndef MACRO_TRKRRECOINIT_C
#define MACRO_TRKRRECOINIT_C

R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_TrkrVariables.C>

#include <tpc/TpcLoadDistortionCorrection.h>

#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllServer.h>

void TrackingInit()
{
  ACTSGEOM::ActsGeomInit();
  G4TPC::module_edge_correction_filename = CDBInterface::instance()->getUrl("TPC_Module_Edge");

  // space charge correction
  if (G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS || G4TPC::ENABLE_STATIC_CORRECTIONS || G4TPC::ENABLE_AVERAGE_CORRECTIONS)
  {
    auto se = Fun4AllServer::instance();
    auto tpcLoadDistortionCorrection = new TpcLoadDistortionCorrection;

    tpcLoadDistortionCorrection->set_read_phi_as_radians(TpcLoadDistortionCorrection::DistortionType_Static, G4TPC::DISTORTIONS_USE_PHI_AS_RADIANS);
    tpcLoadDistortionCorrection->set_interpolate_2D_to_zero(TpcLoadDistortionCorrection::DistortionType_ModuleEdge, false);

    if (G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS)
    {
      tpcLoadDistortionCorrection->set_correction_filename(TpcLoadDistortionCorrection::DistortionType_ModuleEdge, G4TPC::module_edge_correction_filename);
    }
    if (G4TPC::ENABLE_STATIC_CORRECTIONS)
    {
      tpcLoadDistortionCorrection->set_correction_filename(TpcLoadDistortionCorrection::DistortionType_Static, G4TPC::static_correction_filename);
    }
    if (G4TPC::ENABLE_AVERAGE_CORRECTIONS)
    {
      tpcLoadDistortionCorrection->set_correction_filename(TpcLoadDistortionCorrection::DistortionType_Average, G4TPC::average_correction_filename);
    }
    se->registerSubsystem(tpcLoadDistortionCorrection);
  }
}
#endif
