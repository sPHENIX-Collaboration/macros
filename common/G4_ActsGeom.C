#ifndef MACRO_G4ACTSGEOM_C
#define MACRO_G4ACTSGEOM_C

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)
R__LOAD_LIBRARY(libqa_modules.so)

#include <GlobalVariables.C>

#include <G4_Magnet.C>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <trackreco/MakeActsGeometry.h>
#pragma GCC diagnostic pop

#include <fun4all/Fun4AllServer.h>


namespace ACTSGEOM
{

  unsigned int mvtxMisalignment = 1;
  unsigned int inttMisalignment = 1;
  unsigned int tpcMisalignment = 1;
  unsigned int tpotMisalignment = 1;

  void ActsGeomInit()
  {
    static bool wasCalled = false;
    if (wasCalled)
    {
      return;
    }
    wasCalled = true;

    if (!Enable::MICROMEGAS)
    {
      G4MICROMEGAS::n_micromegas_layer = 0;
    }

    // Build the Acts geometry
    auto se = Fun4AllServer::instance();
    int verbosity = Enable::VERBOSITY;

    // Geometry must be built before any Acts modules
    MakeActsGeometry* geom = new MakeActsGeometry();
    geom->set_drift_velocity(G4TPC::tpc_drift_velocity_reco);
    geom->Verbosity(verbosity);
     
    geom->misalignmentFactor(TrkrDefs::TrkrId::mvtxId, ACTSGEOM::mvtxMisalignment);
    geom->misalignmentFactor(TrkrDefs::TrkrId::inttId, ACTSGEOM::inttMisalignment);
    geom->misalignmentFactor(TrkrDefs::TrkrId::tpcId, ACTSGEOM::tpcMisalignment);
    geom->misalignmentFactor(TrkrDefs::TrkrId::micromegasId, ACTSGEOM::tpotMisalignment);
    
    geom->loadMagField(G4TRACKING::init_acts_magfield);
    geom->setMagField(G4MAGNET::magfield);
    geom->setMagFieldRescale(G4MAGNET::magfield_rescale);
    se->registerSubsystem(geom);
  }
}  // namespace ACTSGEOM

#endif
