
# Welcome

This is the standard macros respository for [ECCE](https://www.ecce-eic.org/). Following is the 10-min tutorial on using the default macros.

[![Build Status](https://web.sdcc.bnl.gov/jenkins-sphenix/buildStatus/icon?job=sPHENIX/sPHENIX_CoreSoftware_MasterBranch)](https://web.sdcc.bnl.gov/jenkins-sphenix/job/sPHENIX/job/sPHENIX_CoreSoftware_MasterBranch/)
[![Doxygen](https://img.shields.io/badge/code%20reference-Doxygen-green.svg)](https://ecce-eic.github.io/doxygen/)
[![Singularity](https://img.shields.io/badge/container-Singularity%20via%20CVMFS-green.svg)](https://github.com/ECCE-EIC/Singularity)

# Get started

Please follow [software day-1 checklist](https://ecce-eic.github.io/tutorials_day1.html) to get started.

# Check out macro repository

Download the newest macro repository from GitHub:

```
[test]$ git clone https://github.com/ECCE-EIC/macros.git
Cloning into 'macros'...
remote: Counting objects: 1125, done.
remote: Total 1125 (delta 0), reused 0 (delta 0), pack-reused 1125
Receiving objects: 100% (1125/1125), 181.75 KiB | 0 bytes/s, done.
Resolving deltas: 100% (471/471), done.
Checking connectivity... done.
[test]$ cd macros/detectors
[detectors]$ # here are all the subdirectories e.g. an example EIC detector simulations
```

The default EIC detector simulation + reconstruction macro is [`detectors/EICDetector/Fun4All_G4_EICDetector.C`](detectors/EICDetector/Fun4All_G4_EICDetector.C), which is a collection of steering flags that drives [the common macros](/common). They are not a black box!. You are welcome to open/edit it to work for your purpose of study.

# Try an event display

Then let's see what is implemented by firing up one event:
```
[EICDetector]$ source /cvmfs/eic.opensciencegrid.org/ecce/gcc-8.3/opt/fun4all/core/bin/ecce_setup.sh -n   # setup environment if not already done so
# Note, for csh users, please use /cvmfs/eic.opensciencegrid.org/ecce/gcc-8.3/opt/fun4all/core/bin/ecce_setup.csh instead
**Please edit Fun4All_G4_EICDetector.C to change flag Enable::DISPLAY = true;**
[EICDetector]$ root
root [] .x Fun4All_G4_EICDetector.C // here negative value in event number start an event display of one event
root [] se->run(1)
root [] g4->ApplyCommand("/vis/viewer/refresh");
root [] displaycmd() # this one show more Geant4 command we can run from the ROOT prompt
```


# Run more events

This is simpler, just run with first parameter as the intended number of event:
```
[EICDetector]$ source /cvmfs/eic.opensciencegrid.org/ecce/gcc-8.3/opt/fun4all/core/bin/ecce_setup.sh -n   # setup sPHENIX environment if not already done so
Please recover Enable::DISPLAY = false; as in Fun4All_G4_EICDetector.C 
[EICDetector]$ root
root [] .x Fun4All_G4_EICDetector.C(10) // run 10 events
```

# What next?

Now you are able to run the whole  simulation + reconstruction chain. Many next-step topics are listed in the [software](https://wiki.bnl.gov/sPHENIX/index.php/Software) page. And specifically, to use the simulation for your study, a few thing you might want to try:

* Checkout the quick evaluation Ntuples output from your 10 events for a generic look of the reconstructed data.
* Run [software tutorials](https://ecce-eic.github.io/tutorials_landing_page.html).
* [Write your analysis module for more dedicated analysis](https://github.com/ECCE-EIC/tutorials/tree/master/AnaTutorial), for which the module that access to the entire simulation+reco information and can be customized for your analysis


