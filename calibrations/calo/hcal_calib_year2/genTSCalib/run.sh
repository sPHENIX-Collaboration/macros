# This performs the hadd-ing, fitting, and combining with a cosmics of
# a set TSC spectra files.  See get_tsFiles_flat.sh for meaning of args


run_ohcal=$1  # first argument: true/false for HCALOUT
run_ihcal=$2  # second argument: true/false for HCALIN
runList=${3:-cos_runs.txt}  # cosmics runs, or the first run of each towerslope period (tsc_periods_<dataset>.txt)
mergeNum=${4:-2}            # number of runs to merge after each cosmics run, or "all" for all runs of each period
spectraDir=$5               # folder with the spectra, default is ../condor_histmaker/out_run

# e.g. sh run.sh true true tsc_periods_run3oo.txt all /path/to/out_run

sh get_tsFiles_flat.sh $runList tsc_merge_list.txt $mergeNum $run_ohcal $run_ihcal $spectraDir
