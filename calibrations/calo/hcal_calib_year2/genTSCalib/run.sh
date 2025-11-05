# This performs the hadd-ing, fitting, and combining with a cosmics of
# a set TSC spectra files.  See get_tsFiles_flat.sh for meaning of args


run_ohcal=$1  # first argument: true/false for HCALOUT
run_ihcal=$2  # second argument: true/false for HCALIN

sh get_tsFiles_flat.sh cos_runs.txt tsc_merge_list.txt 2 $run_ohcal $run_ihcal
