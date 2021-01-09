
# Welcome

This is the standard macros respository for [sPHENIX collaboration](https://www.sphenix.bnl.gov/). Following is the 10-min tutorial on using the sPHENIX macros from [this wiki link](https://wiki.bnl.gov/sPHENIX/index.php/Tutorial/sPHENIX_simulation).

[![Build Status](https://web.sdcc.bnl.gov/jenkins-sphenix/buildStatus/icon?job=sPHENIX/sPHENIX_CoreSoftware_MasterBranch)](https://web.sdcc.bnl.gov/jenkins-sphenix/job/sPHENIX/job/sPHENIX_CoreSoftware_MasterBranch/)
[![Doxygen](https://img.shields.io/badge/code%20reference-Doxygen-green.svg)](https://www.phenix.bnl.gov/WWW/sPHENIX/doxygen/html/)
[![Singularity](https://img.shields.io/badge/container-Singularity%20via%20CVMFS-green.svg)](https://github.com/sPHENIX-Collaboration/Singularity)

[![Open Issues](https://img.shields.io/github/issues/sPHENIX-Collaboration/macros.svg)](https://github.com/sPHENIX-Collaboration/macros/issues)
[![Open Pull Request](https://img.shields.io/github/issues-pr/sPHENIX-Collaboration/macros.svg)](https://github.com/sPHENIX-Collaboration/macros/pulls)
[![Monthly Commits](https://img.shields.io/github/commit-activity/m/sPHENIX-Collaboration/macros.svg)](https://github.com/sPHENIX-Collaboration/macros/commits/master)
[![Last Commit](https://img.shields.io/github/last-commit/sPHENIX-Collaboration/macros.svg)](https://github.com/sPHENIX-Collaboration/macros/commits/master)

# Get started

Please follow [SPHENIX software day-1 checklist](https://wiki.bnl.gov/sPHENIX/index.php/SPHENIX_software_day-1_checklist) to get started on [RHIC computing facility at BNL](https://www.racf.bnl.gov/), or follow the [sPHENIX container setup](https://github.com/sPHENIX-Collaboration/Singularity) for your own computing environment.

# Check out macro repository

Download the newest macro repository from GitHub:

```
[jinhuang@rcas2073 test]$ git clone https://github.com/sPHENIX-Collaboration/macros.git
Cloning into 'macros'...
remote: Counting objects: 1125, done.
remote: Total 1125 (delta 0), reused 0 (delta 0), pack-reused 1125
Receiving objects: 100% (1125/1125), 181.75 KiB | 0 bytes/s, done.
Resolving deltas: 100% (471/471), done.
Checking connectivity... done.
[jinhuang@rcas2073 test]$ cd macros/detectors
[jinhuang@rcas2073 detectors]$ # here are all the subdirectories with macros to run sPHENIX, fsPHENIX and EIC detector simulations
```

The default sPHENIX simulation + reconstruction macro is Fun4All_G4_sPHENIX.C, which is self-explanatory. It is not a black box!. You are welcome to open/edit it to work for your purpose of study.

# Try an event display

Then let's see what is implemented by firing up one event:
```
[jinhuang@rcas2072 sPHENIX]$ source /opt/sphenix/core/bin/sphenix_setup.csh -n   # setup sPHENIX environment if not already done so
# Note, for bash users, please use /opt/sphenix/core/bin/sphenix_setup.sh instead
[jinhuang@rcas2072 sPHENIX]$ root
root [] .x Fun4All_G4_sPHENIX.C(-1) // here negative value in event number start an event display of one event
root [] .L DisplayOn.C 
root [] PHG4Reco* g4 = DisplayOn()
root [] Fun4AllServer *se = Fun4AllServer::instance();
root [] se->run(1)
root [] displaycmd() # this one show more Geant4 command we can run from the ROOT prompt
```


# Run more events

This is simpler, just run with first parameter as the intended number of event:
```
[jinhuang@rcas2072 sPHENIX]$ source /opt/sphenix/core/bin/sphenix_setup.csh -n   # setup sPHENIX environment if not already done so
[jinhuang@rcas2072 sPHENIX]$ root
root [] .x Fun4All_G4_sPHENIX.C(10) // run 10 events
```

# What next?

Now you are able to run the whole sPHENIX simulation + reconstruction chain. Many next-step topics are listed in the [software](https://wiki.bnl.gov/sPHENIX/index.php/Software) page. And specifically, to use the simulation for your study, a few thing you might want to try:

* Checkout the [evaluator Ntuples](https://wiki.bnl.gov/sPHENIX/index.php/Tracking) output from your 10 events for a generic look of the reconstructed data.
* Run [sPHENIX software tutorials](https://github.com/sPHENIX-Collaboration/tutorials).
* [Write your analysis module for more dedicated analysis](https://wiki.bnl.gov/sPHENIX/index.php/Example_of_using_DST_nodes), for which the module produced the evaluator NTuple ([CaloEvaluator](https://sphenix-collaboration.github.io/doxygen/dd/d59/classCaloEvaluator.html), [JetEvaluator](https://sphenix-collaboration.github.io/doxygen/dd/d59/classCaloEvaluator.html), [SvtxEvaluator](https://sphenix-collaboration.github.io/doxygen/d6/d11/classSvtxEvaluator.html)) can usually serve as good examples.
