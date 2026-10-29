# Run 24 Au+Au: the 114 runs of the calorimeter good run list with pro001 DST_CALOFITTING and DST_ZDC_RAW, in four
# blocks with their own reference run (54546, 54803, 54912, 54944: the blocks of the earlier Run 24 payloads)
DATASET=run2auau
DSTTAG=pro001_pcdb001_v001
BUILD=ana.579
CDBTAG=newcdbtag
RUNLIST=runlists/run24auau.list
# GL1 min-bias bit: MBD N&S >= 2 (bit 14 is absent from the early Run 24 runs)
TRIGGER_BIT=10
# the main-bunch t0 fit seed searches +-11.5 ns: in 54424-54498 and 54935 the whole bunch pattern is shifted by
# -5 / -10 ns
T0_SEED_WINDOW=11.5
# Glauber Npart distribution of the NBD+Glauber fit of the reference runs, and the number of centrality divisions:
# the trigger efficiency of that fit, in %
GLAUBER=glauber/lime_auau_hists_nominal.root
NDIVISIONS=92
SEGMENTS_PER_JOB=1
# measured peak use: tree jobs 1.1 GB
TREE_MEMORY=1400
# measured peak use: calibration 460 MB (54897, 3.1M events)
CALIB_MEMORY=800
