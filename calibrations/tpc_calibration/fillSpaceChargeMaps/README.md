# Writing analysis from scratch
Start here for setting up environment: [Example_of_using_DST_nodes (Wiki)](https://wiki.bnl.gov/sPHENIX/index.php/Example_of_using_DST_nodes#Building%20a%20package)
- Setup local compilation for bash shel:

```
source /opt/sphenix/core/bin/sphenix_setup.sh -n
export MYINSTALL=/sphenix/user/shulga/tpc2019_install
source /opt/sphenix/core/bin/setup_local.sh $MYINSTALL
```
- Creating package files:
`CreateSubsysRecoModule.pl --all --overwrite fillSpaceChargeMaps` 
(*very useful script providing all files needed for new package*)

- Compilation of an empty package:
```
mkdir build
cd build
../autogen.sh --prefix=$MYINSTALL
make -j 4
make install
```
- reading first file:

... (to be updated)


# Analysis workflow:
- Files with G4Hits from 100'000 Hijing events used for the analysis: 
    - __/sphenix/sim/sim01/sphnxpro/Micromegas/3/G4Hits_sHijing_0-12fm_*__

- File with  bunchcrossing id and time (ns) assuming 106ns between bunches and 50kHz collision rate: __timestamps_50kHz.txt__. The file is used to mimic the bunchcrossing;

- Running over G4Hits containers in the files is performed with Fun4All environment. Main code is Fun4All_FillChargesMap_300evts*.C, it is run with run_files.sh, which takes as an input the first and last file number:
```
#This will run first 5 files with G4Hits and create 5 files 
#with histograms:
source macros/run_files.sh 0 5 
```

-  As soon as files are available the histograms are inside;
- To create bunch of bash files and condor job files to start condor jobs scripts are available:
```
#Run through G4Hits:
scripts/generate_run_files_300evts_pp.py
scripts/generate_run_files_300evts_AA.py
scripts/generate_run_files_avg_pp.py
scripts/generate_run_files_avg_AA.py
```
- Scripts above will also generate bash files to submit all jobs, __all bash scripts created above should be provided executable rights before that__:
```
../run_all_jobs*  

```