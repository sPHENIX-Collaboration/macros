#ifndef MACRO_TRKRRECOINIT_C
#define MACRO_TRKRRECOINIT_C

#include <GlobalVariables.C>

#include <G4_ActsGeom.C>
#include <G4_TrkrVariables.C>

#include <fun4all/Fun4AllRunNodeInputManager.h>
#include <tpc/TpcLoadDistortionCorrection.h>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <tpc/TpcClusterZCrossingCorrection.h>
#pragma GCC diagnostic pop
#include <ffamodules/CDBInterface.h>

#include <fun4all/Fun4AllServer.h>

R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)

void TrackingInit()
{
   // server
  auto *se = Fun4AllServer::instance();
  
  std::string geofile = CDBInterface::instance()->getUrl("Tracking_Geometry");

  Fun4AllRunNodeInputManager *ingeo = new Fun4AllRunNodeInputManager("GeoIn");
  ingeo->AddFile(geofile);
  se->registerInputManager(ingeo);
  
  TpcClusterZCrossingCorrection::_vdrift = G4TPC::tpc_drift_velocity_reco;

  ACTSGEOM::ActsGeomInit();
  G4TPC::module_edge_correction_filename = CDBInterface::instance()->getUrl("TPC_Module_Edge");

  // space charge correction
  if (G4TPC::ENABLE_MODULE_EDGE_CORRECTIONS || G4TPC::ENABLE_STATIC_CORRECTIONS || G4TPC::ENABLE_AVERAGE_CORRECTIONS)
  {
    auto *tpcLoadDistortionCorrection = new TpcLoadDistortionCorrection;

    tpcLoadDistortionCorrection->set_read_phi_as_radians(TpcLoadDistortionCorrection::DistortionType_Static, G4TPC::USE_PHI_AS_RAD_STATIC_CORRECTIONS);
    tpcLoadDistortionCorrection->set_read_phi_as_radians(TpcLoadDistortionCorrection::DistortionType_Average, G4TPC::USE_PHI_AS_RAD_AVERAGE_CORRECTIONS);

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
      tpcLoadDistortionCorrection->set_interpolate_2D_to_zero(TpcLoadDistortionCorrection::DistortionType_Average, G4TPC::average_correction_interpolate);
    }
    se->registerSubsystem(tpcLoadDistortionCorrection);
  }
}
#endif
