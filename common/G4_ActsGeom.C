#ifndef MACRO_G4ACTSGEOM_C
#define MACRO_G4ACTSGEOM_C

// GlobalVariables.C has to be first, an empty line afterwards
// protects its position against reshuffling by clang-format
#include <GlobalVariables.C>

#include <G4_Magnet.C>
#include <G4_TrkrVariables.C>  // for Enable::MVTX_APPLYMISALIGNMENT, INTT_USEG4SURVEYGEOM

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundefined-internal"
#include <trackreco/MakeActsGeometry.h>
#pragma GCC diagnostic pop

#include <fun4all/Fun4AllServer.h>

#include <Rtypes.h>  // for R__LOAD_LIBRARY

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)

namespace ACTSGEOM
{

  unsigned int mvtxMisalignment = 1;
  unsigned int inttMisalignment = 1;
  unsigned int tpcMisalignment = 1;
  unsigned int tpotMisalignment = 1;

  bool mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
  bool inttsurvey = Enable::INTT_USEG4SURVEYGEOM;

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
    ACTSGEOM::mvtx_applymisalignment = Enable::MVTX_APPLYMISALIGNMENT;
    MagnetFieldInit();

    // Build the Acts geometry
    auto *se = Fun4AllServer::instance();
    int verbosity = Enable::VERBOSITY;

    // Geometry must be built before any Acts modules
    MakeActsGeometry *geom = new MakeActsGeometry();
    geom->set_drift_velocity(G4TPC::tpc_drift_velocity_reco);
    geom->set_apply_tpc_tzero_correction(G4TPC::apply_tpc_tzero_correction);  // set true to apply tpc tzero correction
    geom->set_tpc_tzero(G4TPC::tpc_tzero_reco);
    geom->set_sampa_tzero_bias(G4TPC::sampa_tzero_bias);
    std::cout << "G4_ActsGeom: " << std::endl;
    std::cout << "     Setting apply_tpc_tzero_correction flag to " << G4TPC::apply_tpc_tzero_correction << std::endl;
    std::cout << "     Setting tpc_tzero_reco to " << G4TPC::tpc_tzero_reco << std::endl;
    std::cout << "     Setting sampa_tzero_bias to " << G4TPC::sampa_tzero_bias << std::endl;
    std::cout << "     Setting tpc_drift_velocity_reco to " << G4TPC::tpc_drift_velocity_reco << std::endl;
    geom->Verbosity(verbosity);
    for (int i = 0; i < 57; i++)
    {
      if (i < 3)
      {
        geom->misalignmentFactor(i, ACTSGEOM::mvtxMisalignment);
      }
      else if (i < 7)
      {
        geom->misalignmentFactor(i, ACTSGEOM::inttMisalignment);
      }
      else if (i < 55)
      {
        geom->misalignmentFactor(i, ACTSGEOM::tpcMisalignment);
      }
      else
      {
        geom->misalignmentFactor(i, ACTSGEOM::tpotMisalignment);
      }
    }

    geom->loadMagField(G4TRACKING::init_acts_magfield);
    geom->setMagField(G4MAGNET::magfield_tracking);
    geom->setMagFieldRescale(G4MAGNET::magfield_rescale);
    std::cout << "ActsGeomInit: Mvtx apply misalignment? ACTSGEOM::mvtx_applymisalignment=" << ACTSGEOM::mvtx_applymisalignment << std::endl;
    geom->set_mvtx_applymisalign(ACTSGEOM::mvtx_applymisalignment);
    std::cout << "ActsGeomInit: Use survey geometry? ACTSGEOM::inttsurvey=" << ACTSGEOM::inttsurvey << std::endl;
    geom->set_intt_survey(ACTSGEOM::inttsurvey);
    se->registerSubsystem(geom);
  }
}  // namespace ACTSGEOM

#endif
