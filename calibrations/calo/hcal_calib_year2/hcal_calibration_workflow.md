For any questions, please contact: asasikuma@bnl.gov/asmenon1995@gmail.com
# HCal Calibration Workflow (IHCal / OHCal)

Sample macros for every Step live in: `sphnx_macros/calibrations/calo/hcal_calib_year2/`

All Steps are IHCal/OHCal-symmetric — select the detector via the `isIHcal` flags.


## Overview

```
Step 0          Step 1                      Step 2                Step 3
Baseline         Towerslope                   Temperature            Half height 
cosmics calib    correction                   correction             calibration
→ CDB file       (LiteCaloEval                → per-run-range        → manually edit 
                 FitRelativeShifts)           correction           the CDB files
                 → fold into                  → CDB files 
                   baseline → new CDB           per run 
```

---

## Prerequisites

- sPHENIX environment: `source /opt/sphenix/core/bin/sphenix_setup.sh -n new`
- HTCondor access (all Steps submit condor jobs for the Fun4All Step to produce spectra with a certain calibration)
- `CreateDstList.pl` for pulling DST file lists from the file catalog
- Read access to the `sphnxdaqdbreplica`/`daq` Postgres DB (`psql`) — needed for temperature queries (Step 2)
---

## Step 0 — Baseline Cosmics Calibration

The absolute per-tower cosmics (MIP) calibration is derived with a separate package, documented here:
<https://github.com/sPHENIX-Collaboration/analysis/tree/master/HCalCosmicCalib/offline>

The result is a CDB tree ROOT file keyed by `TowerInfoDefs::encode_hcal(ieta, iphi)`, field
`HCALIN_calib_ADC_to_ETower` / `HCALOUT_calib_ADC_to_ETower`.

**Output**: baseline CDB tree file (e.g. `ihcal_CDBTTree_82716_82744_onlycos.root`, named after the first and last cosmics run).

---

## Producing spectra (used by Step 1 to derive towerslope and for estimating calibration stability and systematics)

`Fun4All_HCal.C` + `condor_histmaker/` produce per-tower energy spectra with whatever calibration you point them at.

### Datasets

The numbers and files that differ between datasets (run2pp, run2auau, run3oo) are in `HcalCalibDatasetinputs.h`:
beam run list (good run list, change the path there to use another GRL), DST production tag, cosmics only calibration files, towerslope fit ranges and temperature coefficient.
`Fun4All_HCal.C`, `condor_histmaker/run.sh`, `genTSCalib/run_cosmicsTS.C` and `genFinalCalib/genFinalCalib.C` read them from there.

### Running Fun4All locally

```bash
root Fun4All_HCal.C                              # default: 1000 events
root -b "Fun4All_HCal.C(0,\"inputdata.txt\")"    # 0 = all events
root -b "Fun4All_HCal.C(0,\"inputdata.txt\",\"run3oo\",\"cosmics\",\"cosmics\")"
```

Requires `inputdata.txt` in the same folder — a list of `DST_CALOFITTING_*.root` file paths of one run, one per line.

`Fun4All_HCal(nevents, file list, dataset, OHCal calibration, IHCal calibration)`. When the dataset is not given, the macro asks
for it in a terminal (default answer: the dataset of the run number); in a condor job it uses that default without asking.
The calibration of each detector is not asked for; it defaults to `cosmics`.

With `cosmics`, datasets with one cosmics file per cosmics run (`..._{}.root` in `HcalCalibDatasetinputs.h`) use the file of the last
cosmics run at or before the beam run. The cosmics runs are the run numbers in the file names of the folder, so adding a new
cosmics file only needs the file. A dataset with one file (run3oo) uses it for every run.

The calibration of a detector is one of:
- `cosmics` (default): the cosmics only file of the dataset in `HcalCalibDatasetinputs.h`. For datasets with one file per cosmics run, `findCosmicCalib()` / `findCosmicCalib_ihcal()` take the cosmics runs from the file names in the folder and use the last cosmics run before the beam run.
- `cosmics_tsc`: the cosmics + towerslope file of the dataset (Step 1 output), `{o,i}hcal_cdb_tsc_cos_calib_<run>.root` in the folder `tscDir` of `HcalCalibDatasetinputs.h`; `<run>` is the towerslope period the beam run is in (or the last cosmics run before it for datasets without periods).
- `cosmics_tsc_temp`: the final file of the run (cosmics + towerslope + temperature, Step 2 output) from the folders `oh_finalDir` / `ih_finalDir` of `HcalCalibDatasetinputs.h`.
- a CDB file, e.g. `genTSCalib/cdbFiles/ohcal_cdb_tsc_cos_calib_<run>.root`.
- a folder with the final run-range files (`cdb_*_<first run>_<last run>*.root`): the file whose range holds the run is used.
- `cdb`: the calibration of the conditions database.

**What `Fun4All_HCal.C` does**:
- Parses the run number from the input filename, sets the CDB global tag + timestamp.
- Runs `MbdReco` + `GlobalVertexReco`, `CaloTowerStatus` and `CaloTowerCalib` on OHCal and IHCal, with the calibration files chosen above (`set_directURL()`).
- Two `LiteCaloEval` instances write per-tower spectra: `OUTHIST_OHCAL_<file>.root`, `OUTHIST_IHCAL_<file>.root`.

### Running on Condor

```bash
cd condor_histmaker
sh run.sh                         # asks for the dataset
sh run.sh run3oo                  # run list, DST tag and cosmics only calibration of run3oo from HcalCalibDatasetinputs.h
sh run.sh run3oo cosmics_tsc           # spectra with the cosmics + towerslope files
sh run.sh run3oo cosmics_tsc_temp      # spectra with the final files (cosmics + towerslope + temperature)
sh run.sh run3oo myRuns.txt <OHCal calibration> <IHCal calibration> <name>
```
`run.sh [dataset] [run list] [OHCal calibration] [IHCal calibration] [name]`: prints the dataset, run list, DST tag, calibration files and output folder and asks before submitting.
The run list can be left out when the calibration is one of the keywords; an empty run list (`""`) is the run list of the dataset. The IHCal calibration is the same kind as the OHCal one if it is not given.

The output folders are named after the calibration, so spectra made with different calibrations are kept apart:

| calibration | jobs | merged | spectra |
|---|---|---|---|
| `cosmics` | `condorout` | `hadd_condor` | `out_run` |
| `cosmics_tsc` | `condorout_cosmics_tsc` | `hadd_condor_cosmics_tsc` | `out_run_cosmics_tsc` |
| `cosmics_tsc_temp` | `condorout_cosmics_tsc_temp` | `hadd_condor_cosmics_tsc_temp` | `out_run_cosmics_tsc_temp` |

For a calibration file or folder of your own, or different calibrations for the two detectors, give the name as the 5th argument (asked for if not given).
`cosmics_tsc` and `cosmics_tsc_temp` need the folders of the dataset in `HcalCalibDatasetinputs.h` (noted for run3oo); `run.sh` stops if they are not noted or not there.
Splits each run's DST file list (from `CreateDstList.pl -dataset <dataset> --tag <DST tag>`) into ~50 jobs and submits them.

### Merging output

```bash
sh hadd_condor.sh <runlist.txt>  # hadd per-job outputs into one out_<run>.root per run
sh copy_outfiles.sh              # collect merged files into out_run/
# spectra made with another calibration: give its name
sh hadd_condor.sh <runlist.txt> cosmics_tsc   # condorout_cosmics_tsc -> hadd_condor_cosmics_tsc
sh copy_outfiles.sh cosmics_tsc               # -> out_run_cosmics_tsc/
```
The towerslope (Step 1) is made from the cosmics only spectra in `out_run`.
---

## Step 1 — Derive Towerslope Correction

**Goal**: correct tower-to-tower gain variation within each eta ring, then fold that into the Step 0 baseline.

**Input**: spectra from `condor_histmaker/` using beam data with the Step 0 cosmic calibration applied.

**Sample macros**: `genTSCalib/`

1. Create `cos_runs.txt` with the cosmics reference run number(s).
2. Run `sh run.sh <run_ohcal> <run_ihcal>` (e.g. `sh run.sh false true` for IHCal only).

For datasets with towerslope periods in `HcalCalibDatasetinputs.h` (run3oo: one period from all O+O runs) the run list holds the first run of each period instead of cosmics runs, and all runs of a period are merged:

```bash
sh run.sh true true tsc_periods_run3oo.txt all /path/to/out_run
```

`run.sh <run_ohcal> <run_ihcal> [run list] [runs to merge | all] [spectra folder]`; without the last three arguments it uses `cos_runs.txt`, 2 runs after each cosmics run, and the spectra folder `../condor_histmaker/out_run`.
The outputs are named after the run in the list (cosmics run or first run of the period).

This calls `get_tsFiles_flat.sh`, which:
- Finds and hadds the `LiteCaloEval` output files for each cosmics run into merged ROOT files.
- Calls `run_cosmicsTS.C`, which:
  - Looks up the dataset of the cosmics run in `HcalCalibDatasetinputs.h` (a run belongs to the first dataset whose last run is ≥ the run number) and takes the fit range from there. The fit minimum is set per ieta ring (`oh_fitMin` / `ih_fitMin`, 24 values).
  - Runs `LiteCaloEval::FitRelativeShifts()` to perform the fits → correction pattern histogram (`corrPat`, 24×64).
    `LiteCaloEval` has one fit range per fit, so there is one fit per distinct fit minimum, written to `fitResults/{oh,ih}_cosmicsMerged_fitted_<run>_fmin<value>.root` (spectra and fit plots are in these files).
    `fitResults/{oh,ih}_cosmicsMerged_fitted_<run>.root` then holds only the merged `corrPat` (each eta ring from the fit with its own fit minimum) and `h_fitMin` (the fit minimum used per ring).
  - Calls `tsc_cos_merge()` to combine the towerslope pattern with the baseline calibration:
    - `checkTsc`: clamps outlier fit values (outside [0.5, 1.3] → 1).
    - `checkCos`: clamps outlier baseline values (<1% or >10× the detector-wide average → average, with a warning printed). Dead towers (constant exactly 0) are kept at 0.
    - `rescaleTSC`: renormalizes per eta ring using unbiased phi regions; forces the chimney/support regions to 1 for OHCal only (see Gotchas).
    - Final calib = baseline ÷ normalized towerslope → written as a new CDB tree.
    - The baseline is the single cosmics file of the dataset if `oh_cosmicFile` / `ih_cosmicFile` is set in `HcalCalibDatasetinputs.h` (run3oo: cosmics 82716–82744), otherwise one file per cosmics run.

**Output**: `cdbFiles/{ih,oh}cal_cdb_tsc_cos_calib_<run>.root`

---

### QA (which can be done each step of deriving new calibration)

Give `Fun4All_HCal.C` / `condor_histmaker/run.sh` the Step 1 calibration files and produce spectra on beam `DST_CALOFITTING` data, to check that the hitrate is flat across towers.

`fileQA/ana_hits.C` — loops over the merged `out_run/out_<run>.root` files of the run list, fills per-run hit and temperature histograms and the 2D map of the hit rate per tower → writes `output/anaOut_{o,i}hcal.root` with `runNumberHist`, `h_temp_run`, `h_hits`, `h_events_run`, `pr2d_temp`, `h_hits_2d`.
`h_hits_2d` is the hits of each tower from all runs of the list, divided by the average over the live towers of its eta ring (1 = flat; dead towers are 0 and are left out of the average).

---

## Step 2 — Derive Temperature Correction

**Goal**: correct for SiPM/tower gain drift with temperature between the cosmics reference run and each beam run.
Therefore, you need to get the temperatures of calorimeter towers before doing this correction. 

### Step 2a — Get the temperatures

Two temperatures go into the correction: the average temperature of each **beam run** (`T_beam`) and of the **cosmics reference run** (`T_cosmic`).
Both come from the `hcal_heartbeat` table in the `daq` DB (`sphnxdaqdbreplica`): per-tower readings written every ~10 min (`detector=0` OHCal, `detector=1` IHCal).
Every macro uses the heartbeat reading **closest to the run start time** (`run.brtimestamp`), i.e. a snapshot at the start of the run.

**Sample macros**: `makeTemp/` (beam runs) and `genFinalCalib/getCosmicTemps.C` (cosmics runs).

#### 1. Per-tower temperature map for each beam run

```bash
source /opt/sphenix/core/bin/sphenix_setup.sh -n new      # provides psql
cd makeTemp
root -l -q 'loopCaloTemp.C("runList.txt","HCALOUT")'      # OHCal
root -l -q 'loopCaloTemp.C("runList.txt","HCALIN")'       # IHCal
```

- `runList.txt`: the beam (golden) run list, one run number per line, same list as used for the calibration.
- For each run `getCaloTemp.C` writes a 24×64 `TProfile2D` (`h_HCALOUT_temp` / `h_HCALIN_temp`, binned in ieta × iphi) to
  `temp_ohcal/HCALOUT_temp_<run>.root` or `temp_ihcal/HCALIN_temp_<run>.root`.
- Runs whose output file already exists are skipped, so the macro can simply be re-run after an interruption.
  Runs that fail (DB timeouts) are listed in `failed_runs_pass<N>_<detector>.txt`; pass `passes > 1` (6th argument) to retry them automatically.
- The output folder name is set in **both** `getCaloTemp.C` and `loopCaloTemp.C` (`outdir`); change it in both places (e.g. `temp_ohcal_run3oo`) so different run periods do not mix.
- A list of ~800 runs takes a while — run it in `screen`/`tmux`.

#### 2. Average temperature per run (check the trend)

```bash
root -l -q makeAvgTempVsRun.C
```

- Edit the defaults first: `dir` (the folder from step 1), `runlist_path`, `outroot`, `outcsv`. For OHCal also change `HCALIN` → `HCALOUT` in the input file name and histogram name.
- Averages all towers with `Tmin < T < Tmax` (default 10–32 °C) → `h_avg_vs_idx` (vs run index) and `g_avg_vs_run` (vs run number) in `outroot`, plus a CSV (`run,avg_temp_C,ntowers_used`).
- Check the plot and the CSV before going on: look for jumps, runs with `avg_temp_C = 0` (missing file), and runs with fewer towers than usual in `ntowers_used`.

#### 3. Beam-run temperature input for `genFinalCalib.C`

`genFinalCalib.C` reads the step 1 temperature map of each beam run directly and takes `T_beam` as the average of the towers with 10 < T < 32 °C.
The folder with the maps is its 4th argument (default `../makeTemp/temp_ohcal` or `../makeTemp/temp_ihcal`). A beam run without a map gets no calibration file (a warning is printed).

`fileQA/ana_hits.C` is not needed for the temperature correction. It is QA: it loops over the beam run list, reads the merged spectra (`out_run/out_<run>.root`) and
the step 1 temperature map of each run, and stores the per-run average temperature (same tower selection) together with the hit rates.

```bash
cd fileQA
root -l -b -q 'ana_hits.C("runList.txt", 0)'   # OHCal
root -l -b -q 'ana_hits.C("runList.txt", 1)'   # IHCal
```

`ana_hits.C(<run list>, <isIHcal>, <spectra folder>, <temperature folder>, <output file>, <eLow>, <eHigh>)`; nothing has to be edited in the macro. Defaults:
- spectra folder: `../condor_histmaker/out_run`.
- temperature folder: `../makeTemp/temp_ohcal` or `../makeTemp/temp_ihcal` (the step 1 output).
- output file: `output/anaOut_ohcal.root` or `output/anaOut_ihcal.root`. Give another name to keep the files of different calibrations apart (cosmics only, cosmics + towerslope, ...).
- energy window of a hit: 1 to 5 GeV (OHCal), 0.2 to 1 GeV (IHCal).

Runs with a missing spectrum or temperature file are listed at the end of the printout; their `h_temp_run` entry is 0.

Beam runs with `T_beam < 10` °C (no temperature found) are skipped by `genFinalCalib.C`.

#### 4. Temperature of the cosmics reference runs

`genFinalCalib.C` takes `T_cosmic` for every run in `cos_runs.txt` from the temperatures noted in `HcalCalibDatasetinputs.h` (`oh_/ih_cosmicTemps`).
If none is noted it reads it from the DB with `getCosmicTemps.C` (towers with 10 < T < 32 °C, the same selection as for the beam runs):

- cosmics calibration made from several runs (`oh_/ih_cosmicRuns` = first run, `oh_/ih_cosmicLastRuns` = last run, e.g. run3oo 82716–82744): `fetch_hcal_temp_degC_period()`, the average over all cosmics runs from the first to the last run, weighted by run length;
- no last run noted: `fetch_hcal_temp_degC()`, the closest heartbeat (with valid tower temperatures) to the start of the cosmics run.

Nothing has to be produced beforehand, but check:

- The detector passed by the `fetch_ohcal_temp_degC()` wrapper at the top of `genFinalCalib.C` (`"HCALOUT"` / `"HCALIN"`) matches the detector being calibrated — the wrapper name stays `fetch_ohcal_...` in the IHCal version.
- To look at the cosmics temperatures before running `genFinalCalib.C`, call the function in ROOT:
  ```
  root -l getCosmicTemps.C
  root [1] fetch_hcal_temp_degC(<cosmic_run>, "HCALOUT")
  ```
  **Choosing cosmics runs**: use **high-gain** cosmics only. High-gain runs have an HCAL Singles trigger rate in the kHz range (low-gain runs: Hz range).
The GL1 index of the HCAL Singles trigger changes between run periods (e.g. 24 before run 79142, 42 from run 79142 on), so look it up by name rather than hardcoding the index:


#### Notes

- `ana_hits.C`, `makeAvgTempVsRun.C` and `getCosmicTemps.C` all average the towers with 10 < T < 32 °C. Dead towers report unphysical temperatures (below 0 °C); with a wider window they pull the average down (IHCal cosmics run 82716: 18.21 °C with all towers, 19.42 °C with 10–32 °C).
- Within one run the towers differ by ~1 °C RMS (a fixed η/φ pattern that stays the same from run to run). The correction uses one detector-wide `deltaT`, so this pattern cancels as long as the calibration is per tower; it does not cancel if per-tower absolute temperatures are used.

**Sample macros**: `genFinalCalib/`

`genFinalCalib_pp24.C` is the macro the served p+p 2024 OHCal calibration was made with (nearest cosmics run, slope 0.045, cosmics temperature from the DB). It is kept unchanged as the record of that calibration and does not read `HcalCalibDatasetinputs.h`.

`genFinalCalib.C` reads `HcalCalibDatasetinputs.h` and works for both detectors and for run2pp, run2auau and run3oo:

```bash
root -l -b -q 'genFinalCalib.C("<dataset>", <isIHcal>, "<temperature folder>", "<half height list>", "<folder of Step 1 files>")'
# run3oo (no half height correction, temperature maps in ../makeTemp/temp_ohcal and temp_ihcal):
root -l -b -q 'genFinalCalib.C("run3oo", 0)'   # OHCal
root -l -b -q 'genFinalCalib.C("run3oo", 1)'   # IHCal
# temperature maps in another folder:
root -l -b -q 'genFinalCalib.C("run3oo", 0, "/path/to/temp_ohcal_run3oo")'
root -l -b -q 'genFinalCalib.C("run3oo", 1, "/path/to/temp_ihcal_run3oo")'
```

- `<dataset>`: run2pp, run2auau, run3oo. The macro takes from `HcalCalibDatasetinputs.h`: the beam run list (`runList`), the towerslope periods (`tscPeriods`) or, for datasets without periods, the cosmics runs (`oh_/ih_cosmicRuns`), the temperature slope and the cosmics temperatures. A beam run uses the last cosmics run before it, or the period it is in.
- Before any file is made the macro prints the inputs it will use (run list and number of runs, cosmics + towerslope files, `T_cosmic` and where it comes from, temperature slope, temperature folder and how many beam runs have a map, half height list, output folder). In a terminal it then asks `make the files with these inputs? (y/n)`; in a script or a condor job it goes on without asking.
- If an input is missing (dataset, run list, temperature slope, a cosmics + towerslope file, the temperature maps, the half height list) it prints a `MISSING:` line for each and makes nothing.
- `<temperature folder>`: folder with the temperature maps of the beam runs from `makeTemp/loopCaloTemp.C`, `""` for `../makeTemp/temp_ohcal` (`temp_ihcal`). `T_beam` is the average of the towers with 10 < T < 32 °C of the map of the run.
- `<half height list>`: `""` (default) for no half height correction; run2pp: `halfHieghtList.csv`.
- `<folder of Step 1 files>`: default `../genTSCalib/cdbFiles`; the macro reads `{o,i}hcal_cdb_tsc_cos_calib_<run>.root` from there (it no longer merges cosmics and towerslope itself).
- The cosmics runs and their temperatures are noted per dataset in `HcalCalibDatasetinputs.h` (`oh_/ih_cosmicRuns`, `oh_/ih_cosmicLastRuns`, `oh_/ih_cosmicTemps`); a cosmics run that is not noted is read from the DB with `getCosmicTemps.C`.
- A cosmics calibration made from several runs (run3oo: 82716–82744) uses the temperature of the whole period: `fetch_hcal_temp_degC_period(first run, last run, detector)` in `getCosmicTemps.C` averages the run start temperatures with the run length as weight.
- `<temperature folder>`: folder with the temperature maps of the beam runs from `makeTemp/loopCaloTemp.C`, `""` for `../makeTemp/temp_ohcal` (`temp_ihcal`). `T_beam` is the average of the towers with 10 < T < 32 °C of the map of the run.
- For each beam run: `deltaT = T_beam − T_cosmic`, correction = `1 + alpha * deltaT`; alpha is the temperature slope of the dataset and detector in `HcalCalibDatasetinputs.h` (run3oo: 0.0365 OHCal, 0.0320 IHCal; run2pp and run2auau: 0.045).
- Scales the Step 1 CDB calibration by this uniform scalar → writes one CDB file per beam-run range.

**Helpers**:
- `getCosmicTemps.C` — fetches detector temperature from the daq DB for a given run.
- `tsc_cos_merge.C` — shared towerslope+cosmics merge logic; also defines `genCdbCorr()` / `genCdbCorr_HH()` for applying the scalar (and optional per-tower half-height) corrections.
- `cdbHistConv.C` — `histToCaloCDBTree()` / `CaloCDBTreeToHist()` for converting between TH2 and CDB tree format.

**Output**: `final_<det>_calib_<dataset>/cdb_<det>_<dataset>_cosmic_tsc_temp[_hh]_<run_lo>_<run_hi>.root` (`_hh` only with a half height list) — run-range-binned final calibration files.

---

## Step 3 — Half Height Correction

**Goal**: apply per-tower correction factors to individual towers over the run ranges where they need them, on top of the temperature-corrected calibration.

**Sample macros**: `genFinalCalib.C` + `genCdbCorr_HH()` in `tsc_cos_merge.C`

1. Make a CSV file of the towers to correct (`halfHieghtList.csv`, name set in `genFinalCalib.C`), one tower per line:
   ```
   ieta,iphi,calib_factor,run_lo,run_hi
   ```
   To spot the half height towers, make "hit rate plots per tower" using the condor job outputs and spot any runnumber ranges where the scale is shifted by 2 or 0.5.
2. Run `genFinalCalib.C`. For each beam run it:
   - Selects the CSV entries whose run range contains the run (`getTowersForRun`, `run_lo < run < run_hi`).
   - Calls `genCdbCorr_HH()`: multiplies those towers' calibration by `calib_factor`, then scales the whole map by the Step 2 temperature correction.

To produce files without the half height correction, use `genCdbCorr()` instead of `genCdbCorr_HH()` in `genFinalCalib.C`.

**Output**: `final_<det>_calib_<dataset>/cdb_{ih,oh}cal_<dataset>_cosmic_tsc_temp_hh_<run_lo>_<run_hi>.root` — final run-range-binned calibration files.

---

## Final QA — Validate the Final Calibration

Same architecture as QA described above, but the calibration lookup is **run-dependent**: give the folder with the Step 3 files as the calibration, `findRunByRunCalib()` in `Fun4All_HCal.C` picks the file whose run range holds the beam run.

Same condor / hadd / QA flow. Compare the QA output with the one from Step 1 (before the temperature and half height corrections) to confirm that run-to-run gain drift is now flat.

---

## Additional Utilities

| Macro | Purpose |
|---|---|
| `genRunlists/genGoldenRunList.py` | Queries FileCatalog + Production DBs for runs with ≥1M events and GOLDEN status → `runList.txt` |
| `makeTemp/getCaloTemp.C` | Fetches per-tower temperature from the daq DB → TProfile2D ROOT file |
| `makeTemp/loopCaloTemp.C` | Loops `getCaloTemp` over a run list |
| `makeTemp/makeAvgTempVsRun.C` | Average temperature vs run number plots |
| `fileQA/plot_hits.C` | Plotting utilities for QA output |
| `fileQA/BlairUtils.C` | Shared ROOT utility functions |
| `fileQA/new_get_DbInfo.py` | DB query helper |

---

## Gotchas

- **Hardcoded paths and dates everywhere.** Run numbers, output date tags and absolute `/sphenix/user/...` paths are baked into `genFinalCalib.C` and `get_tsFiles_flat.sh`; the ones that differ between datasets are in `HcalCalibDatasetinputs.h`. Add a new dataset there.
- **Temperature coefficient**: measured for run3oo (set in `HcalCalibDatasetinputs.h`); run2pp and run2auau use the assumed 0.045.
- **Chimney/support regions** in `rescaleTSC` (chimney ieta 0–4, iphi 12–20; support ieta 21–23, iphi 12–20) are set to unity for OHCal only. IHCal sits inside the magnet and is not shadowed by the chimney/support, so no IHCal towers are excluded (`rescaleTSC` takes `isIHcal`). These eta/phi windows are the long-standing values in this code; they have not been cross-checked against other chimney definitions in the calo code.
- **Step dependencies**: Steps 2–3 read the Step 1 output directly by path, and Final QA reads the Step 3 output. Keep intermediate files around.
