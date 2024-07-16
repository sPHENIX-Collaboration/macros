#ifndef MACRO_TPCREADOUTINIT_C
#define MACRO_TPCREADOUTINIT_C

R__LOAD_LIBRARY(libtrack_reco.so)
R__LOAD_LIBRARY(libtpccalib.so)

#include <GlobalVariables.C>

#include <G4_TrkrVariables.C>
#include <fun4all/Fun4AllServer.h>

void TpcReadoutInit(const int RunNumber = 41989)
{

  TRACKING::reco_tpc_is_configured = true;
  G4TPC::tpc_drift_velocity_reco = (8.0 / 1000) * 107.0 / 105.0;
  if(RunNumber<41624){
    TRACKING::reco_tpc_maxtime_sample = 425;
    TRACKING::reco_tpc_time_presample = 40;//120 - 80
  }else{
    TRACKING::reco_tpc_maxtime_sample = 420;
    TRACKING::reco_tpc_time_presample = 0;// 80
  }
}


#endif
