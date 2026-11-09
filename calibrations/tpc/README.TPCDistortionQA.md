For version 4, use `NEXT_PASS_ALL_SYSTEMS.md` at the extracted package root.
It includes current/base-k_eff export, corrected PHGarfield reference initialization,
the core-background fit option and guarded 10-segment pilots. Earlier general usage
below remains supported.

For production-scale discovery, restartable per-run processing and a single compact
summary, see `../../PRODUCTION_GUIDE.md` in the extracted package (or copy that
guide alongside your installed macros). The original runner below remains supported;
it now also accepts `--no-plots` and `--phg-reference-csv`.

# Run-by-run TPC distortion and drift QA — output schema 2

Postprocess already produced QA files with ROOT. No tracking reconstruction is
rerun, and no changes to the coresoftware track-QA PR are required. Install these
files under `macros/calibrations/tpc/`. The optional `DriftVelocityQA/` collector
is needed only to produce new cleaned timing/PHGarfield distributions.


## Browsing the output: schema 2

Each run contains five top-level folders:

| Folder | What to open |
|---|---|
| `rdca` | Measured DCA maps/distributions and Gaussian mean/width profiles |
| `charge` | Positive/negative counts and N+/N- |
| `drift` | Raw/unpacked timing, edge profiles, velocity and PHGarfield comparisons |
| `diagnostics` | Masks, valid-point graphs, fit statistics, optional slices, extra yield/asymmetry plots |
| `metadata` | Selections, time convention/offset, availability and an object-name index |

Examples in `79513/`:

```
rdca/legacy_pca_z_positive/rdca_vs_phi_pos
rdca/legacy_pca_z_positive/rdca_distribution_pos
rdca/legacy_pca_z_positive/rdca_mean_vs_phi_pos
rdca/legacy_pca_z_positive/rdca_sigma_vs_phi_pos
rdca/legacy_pca_z_positive/rdca_charge_odd_vs_phi
rdca/tpc_only_north/rdca_mean_vs_qoverpt
charge/legacy_pt_above_1/charge_ratio_vs_eta
drift/time_raw_north
drift/time_unpacked_north
drift/t0_vs_side
drift/drift_velocity_data_vs_side
```

Only objects supported by the inputs are present. Existing production files
normally populate the legacy PCA-z folders; new QA populates `tpc_only_south`
and `tpc_only_north`. Comparisons are in each selection's `reference_comparison`
folder, with explicit `_difference_from_reference` or `_ratio_to_reference`
suffixes. There are no visible `_h1`/graph duplicates. A binned observable is a
TH1; its validity mask and valid-point graph are in the corresponding diagnostics
folder. Do not treat a zero-error empty profile bin as a valid zero measurement.
Main derived histograms use an error-bar drawing option and suppress meaningless
entry/mean statistics boxes.

For a profile `rdca_mean_vs_phi_pos`, its mask is
`diagnostics/rdca/<selection>/rdca_mean_vs_phi_pos_valid`, and its graph is
`.../rdca_mean_vs_phi_pos_valid_points`. Detailed fit status is
`diagnostics/rdca/<selection>/rdca_vs_phi_pos/fit_status`.

`metadata/object_index` maps the stable internal keys used in the CSV tables to
the new ROOT paths. `metadata/rdca_availability` distinguishes missing DCA inputs
from available inputs with no accepted fits. Measured DCA maps and distributions
are present even if the Gaussian fits fail. Their titles state the side/selection
instead of inheriting an unrelated first-sector label.

## Important timing-coordinate correction

Production `TpcRawHitQA` fills the RAW ADC sample index. The TPC unpacker instead
stores `TRKR_HITSET` hits at

```
unpacked_time = raw_sample - presampleShift - unpacker_t0
```

The default presampleShift is 40 for the current run era, so a raw edge near 48
corresponds to an unpacked edge near 8. Version 1 of this postprocessor missed
that conversion in its production-timing fallback. Regenerate those results.

The updated reader preserves `drift/time_raw_<side>` and produces a separately
labelled `drift/time_unpacked_<side>`. It fits edges ONLY in the unpacked
coordinate. The histogram's x axis is shifted without moving/smearing counts;
ROOT's cached statistics are recomputed. Negative transformed time bins are
retained for context. These raw QA counts still have their original threshold
selection; a coordinate conversion does not reproduce the unpacker's cleaning.

Default: `--raw-time-shift 40`. This is the TOTAL presampleShift + unpacker t0;
use the actual reconstruction setting for nondefault production. Older run eras
(before 41624 with the standard unpacker) require 0 rather than 40. No offset is
inferred by forcing a data edge to 8. Different samples can supply a
`raw_time_shift` column in `--conditions-csv`; it overrides the global value.

Already-unpacked new/standalone drift QA is NEVER shifted by this option.
Applied shifts are saved per side in metadata and `drift_summary.csv`. The
unpacked fixed edge-search windows are applied after the conversion. A constant
offset cancels in CM-t0, but wrong search windows can still invalidate the
velocity extraction. The old standalone's separate +0.5-bin display alignment
correction remains unchanged.

## Start on existing production QA

Source the same environment used for the QA production, then:

```bash
cd /sphenix/user/mitrankov/macros/calibrations/tpc
python3 run_tpc_distortion_qa.py \
  --runs 79513,79516 --reference 79513 \
  --out /sphenix/user/mitrankov/QA/tpc_qa_check_01 \
  --max-files 2
```

The default production path is
`/sphenix/data/data02/sphnxpro/production/run3pp/physics/ana581_2026p005_v001`.
Files are found in `DST_TRKR_SEED/run_00079500_00079600/hist/` for the example
runs. `--production PATH` changes that base. `--input-template` accepts a glob
with `{run:08d}`, `{lo:08d}`, and `{hi:08d}` (100-run group boundaries).
Discovery is strict: an absent requested run is an error.

Use `--dry-run` to print input counts without writing anything. For the full
run list, remove `--max-files`:

```bash
python3 run_tpc_distortion_qa.py \
  --run-list runs.txt --reference 79513 \
  --out /sphenix/user/mitrankov/QA/tpc_qa_all_runs_01
```

`runs.txt` contains run numbers separated by whitespace or commas; `#` comments
are allowed. The reference must be included in the list. Output directories must
be new. A successful invocation leaves a `SUCCESS` marker; an interrupted or
failed directory must not be treated as a complete analysis.

Custom files or alternative reconstructions of one run:

```bash
python3 run_tpc_distortion_qa.py \
  --dataset 'reference=/path/reference/HIST*.root' \
  --dataset 'candidate=/path/candidate/HIST*.root' \
  --reference reference --out /path/qa_comparison_01
```

Quote globs as shown. `--file-list label=files.txt` is an alternative; paths in
the list are relative to the list's directory. Local files only; no XRootD URL
support in this version. Duplicate files/symlink aliases are rejected, including
files shared by two dataset labels. Different files containing the same events
cannot be detected automatically; their comparison errors are correlated.

## Legacy and new track histograms

Both sets are analyzed independently when present. There is no substitution of
an old selection for a new selection and no comparison between incompatible keys.

| Input | Output |
|---|---|
| Existing `dcaxyorigin_phi_{north,south}_{pos,neg}` | Gaussian core DCA mean/width versus phi; half-sum and half-difference of positive/negative means; phi harmonics |
| Existing `nrecotracks_{pos,neg}` | Charge ratio/asymmetry and per-frame yields versus phi and eta, for the stored pT>1 selection |
| Existing `pt_{pos,neg}` | Charge ratio/asymmetry and per-frame pT spectra |
| New `tpc_only_*_rdca_phi_pt` | Core DCA mean/width versus phi in a chosen pT interval |
| New `tpc_only_*_rdca_eta_pt` | Core DCA mean/width versus eta in that interval |
| New `tpc_only_*_rdca_qoverpt` | Core DCA mean/width versus signed q/pT; separate positive/negative linear diagnostic fits |
| New `tpc_only_*_pt_phi_eta` | Charge balance and yields versus phi/eta in that pT interval |
| New crossing/status | Preserved in compact ROOT output |

Legacy north/south means **PCA-z>0 / PCA-z<=0**, not the hardware endcap. Its DCA
plots have no newly introduced pT/cluster cut. Legacy charge maps use the stored
track map and pT>1; the postprocessor cannot prove its detector composition.
New side categories are **hardware South/North**, requiring a TPC seed and no
silicon seed, >=31 TPC clusters, pT>0.2, a valid assigned crossing, and one physical
TPC side. Mixed-side tracks are excluded. These are different populations.

New QA options:

```bash
python3 run_tpc_distortion_qa.py \
  --dataset 'ref=/path/new/reference/*.root' \
  --dataset 'test=/path/new/test/*.root' \
  --reference ref --out /path/new_qa_scan_01 \
  --pt-windows '0.3:0.5,0.5:1,1:2,2:4' --phi-qpt
```

Each pT window gets a separate directory. Limits must coincide with existing pT
edges: 0.2,0.3,0.4,0.5,0.7,1,1.5,2,3,4,6,8,12,20. No partial bins or fractional
counts are invented. Default window: 0.5<=pT<1 GeV/c.

`--phi-qpt` additionally derives q/pT profiles in each phi group from the stored
charge-separated (phi,pT,DCA) histograms. Reciprocal pT **intervals** are mapped
exactly, but their sub-bin distributions are unknown. The central unmeasured
high-pT gap is masked. These profiles and the existing global q/pT histogram
cover **all stored pT>0.2**, independent of the chosen pT window. They are repeated
in each window directory. New pT projections exclude overflow/underflow in their
integrated axes. Phi/eta charge projections stay within the stored axes.

Default grouping: 3 neighboring phi bins and 4 eta bins. This gives 24 phi bins
and 12 eta bins for new QA; legacy 300/100-bin axes give 100/25 bins. Change with
`--phi-group` and `--eta-group`; grouping must divide every applicable axis.
The original DCA bins are 0.2 cm wide: no postprocessing can restore finer data.

## Gaussian core fits and comparisons

Slices use a two-pass Gaussian core fit with Poisson likelihood, bin integration,
and ROOT fit/covariance checks. Quantiles seed the first fit. Each core window is
at least six bins wide. Default minimum: 100 counts both in the full in-range
slice and in each fit window. Fits with bad covariance, a boundary/invalid
solution, fewer than three degrees of freedom, or chi2/ndf>10 are rejected.
This is a core monitoring model, not a proof that the full DCA shape is Gaussian.

Status codes in `slice_fits.csv` and `*_fit_status`:
0 accepted; 1 low statistics; 2 unusable seed/window; 3 fitter failure;
4 bad covariance; 5 invalid/boundary solution; 6 poor fit/insufficient ndf.
Rejected fits are omitted from graphs and have zero validity masks in TH1s.
Do not interpret an unmasked zero-valued TH1 bin as a measurement.

Charge-even = (mu+ + mu-)/2; charge-odd = (mu+ - mu-)/2, using separately fitted
Gaussian means. First/second phi harmonics and linear q/pT coefficients are
diagnostics, not automatic k_eff estimates. A nonzero charge-odd observable can
have contributions other than space charge.

Reference comparisons retain:

* mean/even/odd DCA differences, with errors;
* signed DCA ratios, requiring |reference|>max(0.02 cm,3*reference_error);
* width, charge-ratio, and yield ratios; charge-asymmetry differences.

Use `--ratio-floor` to change the absolute signed-DCA denominator threshold.
Negative reference means are allowed; zero/insignificant denominators are masked.
Signed ratios are **not k_eff ratios** until a response relation is established.
All raw fitted means/errors/reference values remain available for that analysis
away from SDCC. Propagated comparison errors assume independent samples and do
not include model choice, selection differences, common detector systematics,
or covariance between repeated reconstructions of the same events. Counts are
unit-weight track counts; weighted input histograms are not a supported likelihood
model. Yield normalization is per processed QA frame, not per collision or vertex.

For a controlled k_eff scan, use labels for each setting. Optional
`--conditions-csv settings.csv` embeds a table in every ROOT output, for example:

```csv
label,run,k_eff_south,k_eff_north,geometry,drift_setting
ref,79513,1.55,1.55,survey,baseline
test,79513,1.60,1.60,survey,baseline
```

Values are user-supplied metadata, not inferred from histogram contents. The
table must have exactly one row for every dataset label. Keep input events,
geometry, drift settings, cuts and field maps fixed when measuring the response.
The macro does not write CDB constants or apply a correction to reconstruction.

## Side-integrated drift velocity and t0

Timing is detected automatically, in this order:

1. New `TpcDriftVelocityQA`: 1D cleaned ADC-weighted time/z histograms per side.
2. Attached-style `PHGarfieldDriftVelocityQA`: sum all 312 FEE columns **before**
   extracting edges. No FEE-by-FEE fitting or output is performed.
3. Existing production `TpcRawHitQA`: sum all 12 sectors and 3 regions per side
   from `nhits_sample_sec*_R*`. These are thresholded sample **counts**, with a
   different noise selection. PHGarfield z/velocity cannot be reconstructed from
   those counts alone; unavailable quantities stay absent/NaN.

For separate drift files from the same run, retain the track input and specify:

```bash
python3 run_tpc_distortion_qa.py \
  --dataset '79513=/path/production/HIST*.root' \
  --drift-dataset '79513=/path/drift_qa/*79513*root' \
  --reference 79513 --out /path/track_and_drift_01
```

`--drift-file-list label=list.txt` is also supported. Labels must match the track
sample labels. Drift/track files may cover different events, but must describe
the same run and intended conditions; that is not inferred from filenames.

The notebook's median-level, 10/50/90% edge method and windows are used:
t0 levels 2.5..5.5 / 12..16, search 5..14; CM levels 180..225 / 300..360,
search 210..290. Apply a light 3-point median to time. Unlike the notebook's
nearest-bin fallback, a missing bracket or wrong edge direction is rejected.
CM and pad z edges use the notebook's folded upper-branch selection (~1.05 cm
windows, separately below CM/in drift/above pad), suppressing isolated downward
aliases without crossing a physical boundary. The pad background slope is fitted
on u=-25..-2 cm and its level anchored at 103..105 cm; pad search is 100..104.9 cm.
The edge widths are diagnostics, **not statistical errors**. Drift quantities
have no estimated errors in this version; their zero ROOT errors mean unknown,
not exact. Saved 1D time/z profiles permit better fits/uncertainties offline.

Side 0=South, side 1=North, u=-z for South and +z for North; u may be negative
beyond the CM and is never replaced by abs(z). Defaults inherited from the study:

```
clock = 56.8 ns/bin
z_pad = 102.605 cm
CM_halfwidth = 0.28 cm
L_drift = 102.325 cm
v_data = L_drift / [(t_CM - t0)*clock] * 1000   (cm/us)
v_PHG,edges = (u_pad - u_CM) / [(t_CM - t0)*clock] * 1000
```

New collector files carry geometry/clock metadata; those override defaults.
Legacy production timing alone does not establish the clock/geometry, so its
default convention is explicitly recorded. The old standalone time histogram
was filled at integer bins into [0,1000); its +0.5-bin display offset is corrected
here. Both time edges shift equally, so its derived velocity is unchanged.

Three PHGarfield velocities are kept distinct: recovered time/z edge mapping;
equal-FEE mean InitRun baseline; and (new collector only) ADC-weighted mean mapping
velocity. They need not be identical. Divide segment-summed calibration values
by their calibration count after hadd. Do not divide the ordinary ADC spectra
by that count before edge extraction.

## Optional small timing collector

The supplied `DriftVelocityQA/` is adapted from the attached standalone module.
It reuses its mapping, waveform cleaning and initialization-only PHGarfield
integration, but writes only side-integrated 1D histograms and small calibration
sums. FEE coordinates remain an internal sampling grid; there are no FEE maps
or FEE fits in the output. Weighted spectra use Sumw2. A private PHGarfield
instance does not publish to the shared node tree, avoiding a dangling node
after its initialization integrator is released.

Build against your installed sPHENIX/PHGarfield dependencies:

```bash
source ~/.login_spy
cd /sphenix/user/mitrankov/macros/calibrations/tpc/DriftVelocityQA
mkdir -p build
cd build
../autogen.sh --prefix="$MYINSTALL" && make -j4 install
```

In the **test** reconstruction macro, add:

```cpp
#include <tpcdriftvelocityqa/TpcDriftVelocityQA.h>
R__LOAD_LIBRARY(libTpcDriftVelocityQA.so)
```

Then register once, after all TPC unpackers and before any subsystem that changes
`TRKR_HITSET` or rejects events:

```cpp
auto *driftQA = new TpcDriftVelocityQA;
se->registerSubsystem(driftQA);
```

Run your existing runner for 10 events first. No shell/job changes are needed.
The ordinary QA output manager writes the additional histograms. First inspect
`h_TpcDriftVelocityQA_h_unmapped_samples`: it should be zero, and the processed
event count should match the collector's position in the subsystem order.

Historical PHGarfield settings are retained: reference field **377 V/cm**,
P_ref=30.0968906, T_ref=299.98646566 K, t0_PHG=8 bins, step=5 ns. The current
PHGarfield default can differ (the reviewed branch uses 376.85 V/cm). Set
`driftQA->setReferenceCMFieldVcm(...)` explicitly if testing another baseline.
P/T correction is E_eff=E_ref*(P_ref/P_run)*(T_run/T_ref). Celsius is converted to K.
Run-dependent CDB drift factors are disabled for this historical-baseline
diagnostic. It is not automatically the exact PHGarfield instance/configuration
used by the tracking chain. Pressure, temperature, effective/reference field,
geometry, clock, t0 and step are stored in additive calibration metadata.

## Outputs and moving the analysis off SDCC

Per pT window:

* `results.root`: independent run folders with one visible representation per observable,
  with clear names; masks, point graphs, fit diagnostics and harmonic/line fit results are grouped under diagnostics,
  crossing/status histograms, and side-level time/z spectra when available.
  Measured DCA TH2 maps and charge-separated DCA distributions are retained. No event trees, input TH3s or FEE maps are copied.
* `<label>_rdca.pdf`, `<label>_charge.pdf`, `<label>_drift.pdf`: grouped observables and reference comparisons. `<label>_rdca_distributions.pdf` shows measured DCA maps/distributions. A PDF is written only when its observables exist.
* `run_trends/drift/` in ROOT plus `drift_run_trends.pdf`: side-by-side-run scalar
  results, with sample labels on the TH1 bins.
* `slice_fits.csv`, `trends.csv`, `drift_summary.csv`, `availability.tsv`.

The top directory also contains file lists, arguments, conditions, input sizes
and mtimes in `provenance.json`, plus one log per pT window. Keep the ROOT summary
and provenance together when copying offsite. The ROOT file itself contains the
analysis configuration and user-supplied conditions JSON. It is sufficient for
response/ratio studies; repeating a fit requires the saved distributions.

Use `--save-slices` only when you also want every individual DCA slice and its fit
function for offline refitting; measured DCA maps are already retained by default; this increases the file
size. The raw summed 1D drift time/z distributions are retained even without it.

Fit masks and source selections are part of the result. An observable must be
present in every segment of its dataset; partial old/new schema sums are skipped
and reported. Incompatible axes, recovered/broken ROOT inputs and mixed timing
selections abort the analysis. Reference comparisons require matching bin
intervals. Missing new QA never produces fabricated zero profiles.

## Validation scope

See `validation/VALIDATION.md` in the package. Tests use real ROOT and known
synthetic DCA/edge injections, not detector simulation. The postprocessing macro
is ROOT-only; the optional collector requires a real SDCC build and short raw-data
test before production use. Nothing in this package has been submitted to Condor
or pushed to your repositories.
