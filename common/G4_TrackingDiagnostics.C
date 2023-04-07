#ifndef MACRO_G4TRACKINGDIAGNOSTICS_C
#define MACRO_G4TRACKINGDIAGNOSTICS_C

R__LOAD_LIBRARY(libTrackingDiagnostics.so)

#include <G4_Tracking.C>

#include <trackingdiagnostics/KshortReconstruction.h>
#include <trackingdiagnostics/helixResiduals.h>

#include <fun4all/Fun4AllServer.h>



void G4KshortReconstruction(const std::string outputfile)
{
  Fun4AllServer* se         = Fun4AllServer::instance();
  KshortReconstruction *ksr = new KshortReconstruction();
  ksr->set_output_file(outputfile);
  se->registerSubsystem(ksr);
}

void seedResiduals(const std::string outputfile)
{
  Fun4AllServer* se  = Fun4AllServer::instance();

  if(!Enable::MICROMEGAS)
    {
      std::cout<< "Micromegas not enabled"<<std::endl;
      return;
    }

  TrackingInit();

  helixResiduals *eval = new helixResiduals("eval_residuals");
  eval->set_output_file(outputfile);
  se->registerSubsystem(eval);
  
}

void Tracking_Diagnostics(const std::string outputfile)
{
  G4KshortReconstruction(outputfile);
  seedResiduals(outputfile);
}
#endif
