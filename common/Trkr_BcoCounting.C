#include <fun4all/Fun4AllServer.h>
#include <bcolumicount/BcoLumiReco.h>
#include <ffamodules/SyncReco.h>
#include <bcolumicount/StreamingBcoReco.h>
#include <bcolumicount/StreamingLumiReco.h>

R__LOAD_LIBRARY(libfun4all.so)
R__LOAD_LIBRARY(libbcolumicount.so)
R__LOAD_LIBRARY(libffamodules.so)

void StreamingBco()
{
  auto *se = Fun4AllServer::instance();
  // Determines appropriate streaming window length based on distance between triggers
  // Determines which triggered BCOs can be used for streaming analysis
  StreamingBcoReco *streaming_bco = new StreamingBcoReco();
  //streaming_bco->Verbosity(3);
  se->registerSubsystem(streaming_bco);
}

void StreamingLumi()
{
  auto *se = Fun4AllServer::instance();
  // Requires StreamingBcoReco module output on the node tree
  // Determines the luminosity in the streaming data (both per bunch ID and integrated)
  StreamingLumiReco *streaming_lumi = new StreamingLumiReco();
  streaming_lumi->Verbosity(10);
  se->registerSubsystem(streaming_lumi);
}