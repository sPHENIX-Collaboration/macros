Various macros related to running the pi0 Tower by Tower calib module  in various situations:

macros for running tower slope method on MDC2 files 
FOR using various decalibration/calibration updates to the sim and reco frameworks in coresoftware, see towerslope macros directories 

Macro/script for running over MDC2 Hits files with embedding (Production 62 currently) 
-------------------------------------------

Fun4All_pi0tbt_SIMPLE_EMBED.C

Notes: does not need filelist input (e.g. from CreateFileList.pl), see comments inside, takes single 
integer input, assuming filenames of Production 62 MDC2 Hits files.  For other production
runs beginning of macro/filename parsing part my need up dated. Note that per usual MDC2 
and presumably all sphenix default production/sim file access, only the filename is specified
and lustre system/dcache finds the file and decides/provides appropriate "full path" access method.
Valid filenames can be obtained from CreateFileLists.pl


Macros for doing the actual fitting steps and iterations
----------------------

runLCELoop.C

this runs the TbT iterations, the first (or zeroth) iteration if used is run during initial
the fun4all running that also creates the reduced cluster ntuple which this runLCELoop macro
operates on.  Reads in output of previous iterations correction Values applies them, makes
new set (the current iteration's) of tower by tower mass histograms. 


save_corr*.C macros

reads in the mass histograms file, fits the mass distributions, extracts correction values and
produces new file of those.  Usually takes as input the ouput of the runLCELoop except possibly 
for the first (or "zeroth") iteration.


                                 

