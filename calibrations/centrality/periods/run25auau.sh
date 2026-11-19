# Run 25 Au+Au: 2781 runs, every one calibrated against reference run 76546 except the 16 with column 3 = 0 in the run
# list (69149-69153 and 69197-69203, whose main bunch is ambiguous, and 69135-69137, 69145, 69146, 69148), which get
# the reference run's tables
DATASET=run3auau
DSTTAG=pro001_pcdb001_v001
BUILD=ana.579
CDBTAG=newcdbtag
RUNLIST=runlists/run25auau.list
# GL1 min-bias bit: MBD N&S >= 2, |z_vtx| < 150 cm
TRIGGER_BIT=14
T0_SEED_WINDOW=3
# Glauber Npart distribution of the NBD+Glauber fit of the reference runs, and the number of centrality divisions:
# the trigger efficiency of that fit, in %
GLAUBER=glauber/lime_auau_hists_nominal.root
NDIVISIONS=92
SEGMENTS_PER_JOB=6
TREE_MEMORY=1700
# measured peak use: calibration 420 MB (76528, 37M events, one of the largest trees)
CALIB_MEMORY=800
