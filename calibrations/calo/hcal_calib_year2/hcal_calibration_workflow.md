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

**Output**: baseline CDB tree file (e.g. `ihcal_cdbttree_cos_final.root`).

---

## Producing spectra (used by Step 1 to derive towerslope and for estimating calibration stability and systematics)

`Fun4All_HCal.C` + `condor_histmaker/` produce per-tower energy spectra with whatever calibration you point them at.

### Running Fun4All locally

```bash
root Fun4All_HCal.C                              # default: 1000 events
root -b "Fun4All_HCal.C(0,\"inputdata.txt\")"    # 0 = all events
```

Requires `inputdata.txt` in the same folder — a list of `DST_CALOFITTING_*.root` file paths, one per line.
(Example: `/sphenix/user/anjsmenon/work_2026/onlycosmics_run3oo_hcalspectra/inputdata.txt`)

**What `Fun4All_HCal.C` does**:
- Parses the run number from the input filename, sets the CDB global tag + timestamp.
- Runs `CaloTowerCalib` on OHCal and IHCal, with calibration files pointed at via `set_directURL()` or taken from the CDB
  (edit `findCosmicCalib` / `findRunByRunCalib` to point at your calibration files).
- Two `LiteCaloEval` instances write per-tower spectra: `OUTHIST_OHCAL_<file>.root`, `OUTHIST_IHCAL_<file>.root`.

### Running on Condor

```bash
cd condor_histmaker
sh run.sh <runlist.txt>          # one run number per line (default: runList.txt)
```
Splits each run's DST file list into ~50 jobs and submits them. 

### Merging output

```bash
sh hadd_condor.sh <runlist.txt>  # hadd per-job outputs into one out_<run>.root per run
sh copy_outfiles.sh              # collect merged files into out_run/
```
---

## Step 1 — Derive Towerslope Correction

**Goal**: correct tower-to-tower gain variation within each eta ring, then fold that into the Step 0 baseline.

**Input**: spectra from `condor_histmaker/` using beam data with the Step 0 cosmic calibration applied.

**Sample macros**: `genTSCalib/`

1. Create `cos_runs.txt` with the cosmics reference run number(s).
2. Run `sh run.sh <run_ohcal> <run_ihcal>` (e.g. `sh run.sh false true` for IHCal only).

This calls `get_tsFiles_flat.sh`, which:
- Finds and hadds the `LiteCaloEval` output files for each cosmics run into merged ROOT files.
- Calls `run_cosmicsTS.C`, which:
  - Runs `LiteCaloEval::FitRelativeShifts()` to perform the fits → correction pattern histogram (`corrPat`, 24×64).
  - Calls `tsc_cos_merge()` to combine the towerslope pattern with the baseline calibration:
    - `checkTsc`: clamps outlier fit values (outside [0.5, 1.3] → 1).
    - `checkCos`: clamps outlier baseline values (<1% or >10× the detector-wide average → average, with a warning printed).
    - `rescaleTSC`: renormalizes per eta ring using unbiased phi regions; forces the chimney/support regions to 1 (see Gotchas).
    - Final calib = baseline ÷ normalized towerslope → written as a new CDB tree.

**Output**: `cdbFiles/{ih,oh}cal_cdb_tsc_cos_calib_<run>.root`

---

### QA (which can be done each step of deriving new calibration)

Point `Fun4All_HCal.C` at the Step 1 calibration and produce spectra on beam `DST_CALOFITTING` data, to check that the hitrate is flat across towers.

`fileQA/ana_hits.C` — loops over the merged `out_run/out_<run>.root` files, fills per-run hit-rate and temperature histograms → writes `anaOut.root` with `runNumberHist`, `h_temp_run`, `h_hits`, etc.

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
- The output folder name is set in **both** `getCaloTemp.C` and `loopCaloTemp.C` (`outdir`); change it in both places (e.g. `temp_ohcal_run3pp`) so different run periods do not mix.
- A list of ~800 runs takes a while — run it in `screen`/`tmux`.

#### 2. Average temperature per run (check the trend)

```bash
root -l -q makeAvgTempVsRun.C
```

- Edit the defaults first: `dir` (the folder from step 1), `runlist_path`, `outroot`, `outcsv`. For OHCal also change `HCALIN` → `HCALOUT` in the input file name and histogram name.
- Averages all towers with `Tmin < T < Tmax` (default 10–30 °C) → `h_avg_vs_idx` (vs run index) and `g_avg_vs_run` (vs run number) in `outroot`, plus a CSV (`run,avg_temp_C,ntowers_used`).
- Check the plot and the CSV before going on: look for jumps, runs with `avg_temp_C = 0` (missing file), and runs with fewer towers than usual in `ntowers_used`.

#### 3. Beam-run temperature input for `genFinalCalib.C`

`genFinalCalib.C` does **not** read the `makeTemp` output directly. It reads `T_beam` from `h_temp_run` (with `runNumberHist`) in the `anaOut` ROOT file made by
`fileQA/ana_hits_ohcal.C` / `fileQA/ana_hits_ihcal.C`. These loop over the beam run list, read the merged spectra (`out_run/out_<run>.root`) and the step 1 temperature map of each run,
and store the per-run average temperature (towers with 15 < T < 30 °C) together with the hit rates.

```bash
cd fileQA
root -l -q 'ana_hits_ohcal.C("runList.txt")'
root -l -q 'ana_hits_ihcal.C("runList.txt")'
```

Edit for each dataset:
- `out_root` — name of the output `anaOut` file.
- `infile` — path to the merged condor output, `.../out_run/out_%d.root`.
- `tpath` in `getTempRun()` — path to the step 1 temperature maps, e.g. `.../temp_ohcal_run3pp/HCALOUT_temp_%d.root` (OHCal) or `.../temp_ihcal_run3pp/HCALIN_temp_%d.root` (IHCal).

Runs with a missing spectrum or temperature file are listed at the end of the printout; their `h_temp_run` entry is 0.
Then point `genFinalCalib.C` at that `anaOut` file (the `TFile::Open("analyze_hits/anaOut_....root")` line).

Beam runs with `T_beam < 10` °C (no temperature found) are skipped by `genFinalCalib.C`.

#### 4. Temperature of the cosmics reference runs

`genFinalCalib.C` gets `T_cosmic` itself for every run in `cos_runs.txt`, by calling `fetch_hcal_temp_degC()` from `getCosmicTemps.C`
(average over all towers at the closest heartbeat, −50 < T < 100 °C). Nothing has to be produced beforehand, but check:

- The detector passed by the `fetch_ohcal_temp_degC()` wrapper at the top of `genFinalCalib.C` (`"HCALOUT"` / `"HCALIN"`) matches the detector being calibrated — the wrapper name stays `fetch_ohcal_...` in the IHCal version.
- To look at the cosmics temperatures before running `genFinalCalib.C`, call the function in ROOT:
  ```
  root -l getCosmicTemps.C
  root [1] fetch_hcal_temp_degC(<cosmic_run>, "HCALOUT")
  ```
  **Choosing cosmics runs**: use **high-gain** cosmics only. High-gain runs have an HCAL Singles trigger rate in the kHz range (low-gain runs: Hz range).
The GL1 index of the HCAL Singles trigger changes between run periods (e.g. 24 before run 79142, 42 from run 79142 on), so look it up by name rather than hardcoding the index:


#### Notes

- `ana_hits.C` (15–30 °C), `makeAvgTempVsRun.C` (10–30 °C by default) and `getCosmicTemps.C` (−50 to 100 °C) use different temperature windows. In practice this changes the run averages very little (a few readings out of ~10⁶ fall outside 10–30 °C), but use the same window everywhere when comparing numbers.
- Within one run the towers differ by ~1 °C RMS (a fixed η/φ pattern that stays the same from run to run). The correction uses one detector-wide `deltaT`, so this pattern cancels as long as the calibration is per tower; it does not cancel if per-tower absolute temperatures are used.

**Sample macros**: `genFinalCalib/`

`genFinalCalib.C`:
- Reads the cosmics reference run(s) from `cos_runs.txt`.
- Looks up beam-run temperatures from the Step 1 QA output (`anaOut.root` → `runNumberHist` + `h_temp_run`).
- Gets the cosmics-run temperature (via `getCosmicTemps.C` → psql query to the `hcal_heartbeat` table, or from the nearest beam run's temperature).
- For each beam run: `deltaT = T_beam − T_cosmic`, correction = `1 + alpha * deltaT` (hardcoded **alpha = 0.045** /°C).
- Scales the Step 1 CDB calibration by this uniform scalar → writes one CDB file per beam-run range.

**Helpers**:
- `getCosmicTemps.C` — fetches detector temperature from the daq DB for a given run.
- `tsc_cos_merge.C` — shared towerslope+cosmics merge logic; also defines `genCdbCorr()` / `genCdbCorr_HH()` for applying the scalar (and optional per-tower half-height) corrections.
- `cdbHistConv.C` — `histToCaloCDBTree()` / `CaloCDBTreeToHist()` for converting between TH2 and CDB tree format.

**Output**: `cdb_{ih,oh}cal_tsc_temp_cosmic_<run_lo>_<run_hi>.root` — run-range-binned final calibration files.

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

**Output**: `cdb_{ih,oh}cal_tsc_temp_cosmic_hh_<run_lo>_<run_hi>.root` — final run-range-binned calibration files.

---

## Final QA — Validate the Final Calibration

Same architecture as QA described above, but the calibration lookup is **run-dependent**: `findRunByRunCalib()` in `Fun4All_HCal.C` picks the bracketing run-range CDB file from Step 3 for each beam run.

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

- **Hardcoded paths and dates everywhere.** Run numbers, output date tags and absolute `/sphenix/user/...` paths are baked into `Fun4All_HCal.C` (`findCosmicCalib*`, `findRunByRunCalib*`), `genFinalCalib.C` and `get_tsFiles_flat.sh`. Update them by hand for each new dataset.
- **alpha = 0.045** (temperature coefficient) is fixed assumption, not re-derived per run period.
- **Chimney/support regions** in `rescaleTSC` (chimney ieta 0–4, iphi 12–20; support ieta 21–23, iphi 12–20) are currently set to unity for both detectors. This needs an edit in the future so that they are not set to unity for IHCal.
- **Step dependencies**: Steps 2–3 read the Step 1 output directly by path, and Final QA reads the Step 3 output. Keep intermediate files around.
