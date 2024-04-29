#ifndef MACRO_G4ACTSGEOM_C
#define MACRO_G4ACTSGEOM_C

R__LOAD_LIBRARY(libg4eval.so)
R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)

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

    MagnetFieldInit();

    // Build the Acts geometry
    auto se = Fun4AllServer::instance();
    int verbosity = Enable::VERBOSITY;

    // Geometry must be built before any Acts modules
    MakeActsGeometry* geom = new MakeActsGeometry();
    geom->set_drift_velocity(G4TPC::tpc_drift_velocity_reco);
    geom->Verbosity(verbosity);
    for(int i = 0; i < 57; i++)
      {
	if(i<3) {
	  geom->misalignmentFactor(i, ACTSGEOM::mvtxMisalignment);
	} else if (i < 7) {
	  geom->misalignmentFactor(i, ACTSGEOM::inttMisalignment);
	} else if (i < 55) {
	  geom->misalignmentFactor(i, ACTSGEOM::tpcMisalignment);
	} else {
	  geom->misalignmentFactor(i, ACTSGEOM::tpotMisalignment);
	}
      }
    
    geom->loadMagField(G4TRACKING::init_acts_magfield);
    geom->setMagField(G4MAGNET::magfield_tracking);
    geom->setMagFieldRescale(G4MAGNET::magfield_rescale);
    std::cout << "ActsGeomInit: Use survey geometry? ACTSGEOM::inttsurvey=" << ACTSGEOM::inttsurvey << std::endl;
    geom->set_intt_survey(ACTSGEOM::inttsurvey);
    se->registerSubsystem(geom);
  }
}  // namespace ACTSGEOM

#endif
