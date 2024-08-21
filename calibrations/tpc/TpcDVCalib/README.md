# TpcDVCalib
macros for sPHENIX TPC drift velocity calibration module

**Do not forget to change Initialdir, RunNumber, input DST list in Arguments, and the number of jobs you want to submit**

```
Initialdir     = /sphenix/u/xyu3/hftg01
Executable     = $(Initialdir)/run_data.sh
RunNumber      = 51103
Arguments      = "./runList/trackrunlist/run$(RunNumber)_$INT(Process,%04d).txt ./runList/run$(RunNumber)_calo.list $(DVCorrTag)"
Queue 9
```

- Output root file saved in Reconstructed/[runnumber]

- Do offline analysis to calibrate drift velocity
```
./offline_dv_calib.sh [runnumber]
```

- Figure saved in ``figure/`` or ``figure_failed`` directory
- CDBTTree saved in ``cdbttree/`` or ``cdbttree_failed`` directory
