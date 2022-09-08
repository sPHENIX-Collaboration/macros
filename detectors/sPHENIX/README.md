[//]: # (This section starts with a 2nd level heading and get embedded in the result QA report at https://github.com/sPHENIX-Collaboration/QA-gallery/tree/QA-tracking-pythiajet)

## Tracking QA with high pT Pythia events

PYTHIA high $p_T$ tracking QA consists full tracker + reconstruction of events with the default Pythia configuration file. This cfg file has a single hard QCD ccbar event with minimum partonic $p_T$ of 10 GeV and truth jet triggering of $p_T>20$ GeV. 

Last a D0->piK filter is applied so the same sample canbe used in D0 reco validation. Please note the calorimeters are disabled to improve execution speed, which also removed some of the correlated albedo background for the tracker. 

In addition to the Pythia event, an Upsilon is embedded to probe an agregated tracking resolution. 

The source code of the macro can be found at https://github.com/sPHENIX-Collaboration/macros/tree/QA-tracking-pythiajet or [comparing it to the master branch](https://github.com/sPHENIX-Collaboration/macros/compare/QA-tracking-pythiajet?expand=1).
