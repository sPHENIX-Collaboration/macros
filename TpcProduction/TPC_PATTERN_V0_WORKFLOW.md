# TPC Pattern-Track Fitting and V0 Campaign Workflow

This document describes the current real-data workflow for fitting TPC pattern
tracks, reconstructing two-track V0 candidates, and running the work through
HTCondor. It documents the code used by the scripts in this directory; it is
not a description of the standard ACTS tracking chain.

## Workflow Overview

```text
pattern-reconstruction DST
  |-- TPC_POLYCLUSTERS
  |-- TPC_POLYTRACKS
  |-- TPC_CROSSING_DECISIONS (required for crossing selection)
  `-- TPC_POLYTRACKVERTICES (optional)
          |
          v
Fun4All_TpcPatternRecoV0.C
          |
          v
TpcV0CandidateTree
  |-- TpcTrackHelixFitter
  |-- TpcTrackKalmanFitter
  `-- optional V0 pair PCA, topology, AP variables, and invariant masses
          |
          v
ROOT output
  |-- trackTree
  |-- pairTree
  `-- clusterResidualTree (optional)
```

The campaign layer and the reconstruction layer are separate. Changing from a
file campaign to an event campaign changes only how events are assigned to
Condor jobs. It does not change the track fitter, V0 algorithm, or cuts.

## Code Ownership

| Component | Responsibility |
|---|---|
| `submit_tpc_pattern_v0_campaign.sh` | Splits work by input files and submits Condor jobs. |
| `submit_tpc_pattern_v0_event_campaign.sh` | Builds exact event ranges, then invokes the file submitter in manifest mode. |
| `make_tpc_v0_event_chunks.py` | Reads the `T` tree entry count with uproot and writes non-overlapping event chunks. |
| `tpc_pattern_v0.job` | Condor resource request, environment, log paths, and queue definition. |
| `run_tpc_pattern_v0.sh` | Sets up sPHENIX, selects a file or event chunk, runs ROOT, and moves validated output to `completed/`. |
| `Fun4All_TpcPatternRecoV0.C` | Loads libraries, configures `TpcV0CandidateTree`, registers the DST input manager, and runs Fun4All. |
| `offline/packages/TrackingDiagnostics/TpcV0CandidateTree.{h,cc}` | Builds tracklets, invokes a fitter, calculates track DCA and pair PCA, applies cuts, and writes the ROOT trees. |
| `offline/packages/tpctrackreco/TpcTrackHelixFitter.{h,cc}` | Point ordering, circle/helix fitting, helix propagation, DCA, and helix-pair PCA. |
| `offline/packages/tpctrackreco/TpcTrackKalmanFitter.{h,cc}` | Six-state Kalman filtering and smoothing, covariance transport, propagation, diagnostics, and DCA. |
| `offline/packages/tpctrackreco/TpcTrackFit.h` | Shared track point, helix, state, Kalman configuration, and fit-result data structures. |

The runtime libraries are `libtpctrackreco.so` and
`libTrackingDiagnostics.so`; the ROOT macro explicitly loads both. The worker
uses the current `new` release by default. Set `V0_SOFTWARE_RELEASE` to select a
specific compatible release. A development installation can be overlaid by
setting `V0_LOCAL_INSTALL`; no local overlay is used by default.

If the required `TpcV0CandidateTree` API has been merged into coresoftware but
has not yet reached the selected CVMFS release, set `V0_LOCAL_INSTALL` to a
compatible local coresoftware installation until the next release is available.

Do not mix headers from the current package layout with incompatible legacy
binaries. Do not rebuild an active development installation or edit the runtime
macro while a production campaign is in flight unless the change is
intentionally meant for those jobs.

## Input DST Contract

The ROOT DST must have an event tree named `T`. The reconstruction macro reads:

```text
TPC_POLYCLUSTERS       cluster centroids associated with pattern tracks
TPC_POLYTRACKS         upstream track state, charge, dE/dx, and cluster count
TPC_CROSSING_DECISIONS selected crossing and crossing-quality information,
                       keyed by source assembled-track ID
TPC_POLYTRACKVERTICES  optional collision-vertex candidates
EventHeader            optional run and event identifiers
```

`TPC_POLYCLUSTERS` and `TPC_POLYTRACKS` are required. Events missing either
container are skipped. Valid `TPC_POLYTRACKVERTICES` entries can supply the
collision vertex used for individual-track DCA; otherwise the configured fixed
vertex is the fallback. Pair flight vectors, DIRA, and transverse decay radius
use the configured fixed primary vertex. Raw `TRKR_HITSET` content is explicitly
disabled by the input manager because this workflow uses cluster centroids, not
raw TPC hits.

The track charge comes from `TPC_POLYTRACKS`. Tracks with zero charge, invalid
fit status, too few valid points, or a failed requested refit are not available
for pairing.

`TPC_CROSSING_DECISIONS` is optional when no crossing-dependent selection is
requested. In that case, tracks without a selected crossing are retained and
the crossing branch uses `32767` as the unknown sentinel. It becomes a strict
input requirement when a required crossing, a maximum crossing tier, or
same-crossing pairing is enabled. This prevents a missing node from being
silently interpreted as crossing zero.

## Fitting Modes

| Mode | Settings | Track trajectory used for V0 reconstruction |
|---|---|---|
| Helix refit | `V0_FIT_METHOD=helix`, `V0_USE_FINAL_TRACK_HELIX=false` | Fits the associated `TPC_POLYCLUSTERS` with `TpcTrackHelixFitter`. |
| Kalman refit | `V0_FIT_METHOD=kalman`, `V0_USE_FINAL_TRACK_HELIX=false` | Fits and smooths the associated clusters with `TpcTrackKalmanFitter`. |
| Upstream-state helix | `V0_FIT_METHOD=helix`, `V0_USE_FINAL_TRACK_HELIX=true` | Constructs a helix from the position, momentum, and charge stored in `TPC_POLYTRACKS`; clusters anchor its allowed PCA search range. |
| Line diagnostic | `V0_FIT_METHOD=line` | Uses the upstream state without a helix or Kalman refit. This is not the normal production mode. |

The historical setting name `V0_USE_FINAL_TRACK_HELIX` does not mean that the
macro reads a separate `FINALTRACKS` node. In this workflow it uses the fitted
state stored in `TPC_POLYTRACKS`.

`V0_POINT_ORDER=auto` uses radius ordering for ordinary tracks and a theta-z
ordering for tracks classified as loopers. Other accepted values are `path`,
`input`, `radius`, and `theta-z`.

Set `V0_RECONSTRUCT_PAIRS=false` to run the selected fitter without the
quadratic pair loop. The module still builds and fits every usable input track,
calculates its DCA and residual diagnostics, and fills `trackTree`. It creates
an empty `pairTree` so the output schema and validation remain compatible with
the V0 workflow. The `V0_PRE_*` and `V0_PAIR_*` settings control pair
eligibility and do not filter rows from `trackTree`.

## Prepare an Input List

Use absolute paths and sort deterministically:

```bash
cd /path/to/macros/TpcProduction

find /path/to/output_DST \
  -maxdepth 1 -type f -name '*.root' | LC_ALL=C sort \
  > output/dst_my_sample.list

wc -l output/dst_my_sample.list
head output/dst_my_sample.list
```

Regenerate the list if files may have been added, removed, or replaced by the
upstream producer.

## Choose a Campaign Splitter

### File campaign

```text
./submit_tpc_pattern_v0_campaign.sh \
  CAMPAIGN INPUT_LIST [FILES_PER_JOB] [EVENTS_PER_INPUT_FILE] [TOTAL_FILES]
```

Example:

```bash
./submit_tpc_pattern_v0_campaign.sh \
  tpc_pattern_v0_example \
  output/dst_my_sample.list \
  1 \
  100000 \
  1000
```

This requests at most the first 1000 list entries, assigns one file to each
job, and asks Fun4All for at most 100000 events from that job. The fourth
argument is an event cap used to form `nevents`; it is not the campaign's total
event count. With multiple files per job, the cap is
`FILES_PER_JOB * EVENTS_PER_INPUT_FILE` across the selected input stream, not a
separate guaranteed count for each file.

This mode is simple and efficient when one file per job has a reasonable and
roughly uniform runtime. It processes every event only when the requested event
cap is at least as large as the available input stream.

### Event campaign

```text
./submit_tpc_pattern_v0_event_campaign.sh \
  CAMPAIGN INPUT_LIST [EVENTS_PER_JOB] [MAX_JOBS]
```

Example:

```bash
./submit_tpc_pattern_v0_event_campaign.sh \
  tpc_pattern_v0_example_event_chunks \
  output/dst_my_sample.list \
  2000 \
  10000
```

The event submitter opens every usable DST, reads `T->GetEntries()` through
uproot, and writes:

```text
output/CAMPAIGN/event_chunks.tsv
```

Each manifest row contains one input file, an event offset, and an exact event
count. Several jobs can therefore process non-overlapping ranges of the same
large DST. The last range in a file can be shorter than `EVENTS_PER_JOB`.

`MAX_JOBS` is a safety ceiling. It is not the requested number of jobs and does
not truncate the sample. If the complete sample requires more jobs, submission
stops and reports a larger suggested `EVENTS_PER_JOB` value. Unreadable files or
files without a `T` tree are reported and omitted from the manifest.

Use event mode when complete event coverage matters and files are large or
uneven. Use file mode when the natural one-file-per-job split is already
appropriate.

## Production Example: Uniform-Field Kalman Refit

The following is a template, not a universal physics selection. Choose the
vertex and cuts for the data set being processed.

```bash
cd /path/to/macros/TpcProduction

V0_PRIMARY_VERTEX_X=0.0 \
V0_PRIMARY_VERTEX_Y=0.0 \
V0_PRIMARY_VERTEX_Z=0.0 \
V0_FIT_METHOD=kalman \
V0_USE_FINAL_TRACK_HELIX=false \
V0_POINT_ORDER=auto \
V0_RECONSTRUCT_PAIRS=true \
V0_WRITE_SAME_SIGN_PAIRS=true \
V0_WRITE_CLUSTER_RESIDUAL_TREE=false \
V0_WRITE_KALMAN_INNOVATION_DIAGNOSTICS=false \
V0_KALMAN_SIGMA_RPHI_CM=0.08 \
V0_KALMAN_SIGMA_R_CM=0.3 \
V0_KALMAN_SIGMA_Z_CM=0.23 \
V0_USE_KALMAN_FIELD_MAP=false \
V0_KALMAN_UNIFORM_PROPAGATOR=analytic \
V0_PRE_TRACK_PT_MIN=0.20 \
V0_PRE_TRACK_DCA_XY_MIN=-1.0 \
V0_PRE_TRACK_QUALITY_MAX=-1.0 \
V0_PRE_TRACK_NPOINTS_MIN=21 \
V0_PRE_PAIR_DCA_MAX=6.0 \
V0_PRE_LPROJ_MIN=-1.0 \
V0_PRE_COS_THETA_MIN=-2.0 \
V0_PAIR_PCA_Z_MAX=18.0 \
V0_PAIR_PCA_DZ_MAX=1.0 \
V0_PAIR_DECAY_RADIUS_MIN=2.0 \
V0_PAIR_ALPHA_ABS_MAX=-1.0 \
V0_PAIR_DCA_MAX=4.0 \
V0_PAIR_DIRA_MIN=0.75 \
V0_COARSE_STEPS=64 \
V0_PCA_CANDIDATES=32 \
V0_PRINT_TIMING=false \
./submit_tpc_pattern_v0_event_campaign.sh \
  tpc_pattern_v0_my_sample_kalman_v1 \
  output/dst_my_sample.list \
  2000 \
  10000
```

To use the file splitter with the identical reconstruction configuration,
replace only the last command:

```bash
./submit_tpc_pattern_v0_campaign.sh \
  tpc_pattern_v0_my_sample_kalman_v1 \
  output/dst_my_sample.list \
  1 \
  100000 \
  1000
```

## Helix and Upstream-State Examples

For a cluster helix refit, retain the desired common settings and use:

```bash
V0_FIT_METHOD=helix \
V0_USE_FINAL_TRACK_HELIX=false \
V0_POINT_ORDER=auto \
./submit_tpc_pattern_v0_event_campaign.sh \
  tpc_pattern_v0_my_sample_helix_refit_v1 \
  output/dst_my_sample.list \
  2000 \
  10000
```

For the upstream-state helix mode:

```bash
V0_FIT_METHOD=helix \
V0_USE_FINAL_TRACK_HELIX=true \
V0_FINAL_TRACK_HELIX_MAX_UPSTREAM_CM=80 \
V0_FINAL_TRACK_HELIX_DOWNSTREAM_MARGIN_CM=5 \
V0_COARSE_STEPS=64 \
V0_PCA_CANDIDATES=32 \
./submit_tpc_pattern_v0_event_campaign.sh \
  tpc_pattern_v0_my_sample_upstream_helix_v1 \
  output/dst_my_sample.list \
  2000 \
  10000
```

Shell assignments apply only to the command immediately following them. Put
all desired common cuts in the same command or export them first. The two short
examples above show the mode switch; settings omitted from them take their
script defaults.

## Track-Fit-Only Example

Use the same file or event campaign and disable only pair reconstruction. Pair
preselection and exact-pair cut settings are ignored in this mode.

```bash
cd /path/to/macros/TpcProduction

V0_FIT_METHOD=kalman \
V0_USE_FINAL_TRACK_HELIX=false \
V0_POINT_ORDER=auto \
V0_RECONSTRUCT_PAIRS=false \
V0_WRITE_CLUSTER_RESIDUAL_TREE=false \
V0_WRITE_KALMAN_INNOVATION_DIAGNOSTICS=false \
V0_KALMAN_SIGMA_RPHI_CM=0.08 \
V0_KALMAN_SIGMA_R_CM=0.3 \
V0_KALMAN_SIGMA_Z_CM=0.23 \
V0_USE_KALMAN_FIELD_MAP=false \
V0_KALMAN_UNIFORM_PROPAGATOR=analytic \
./submit_tpc_pattern_v0_event_campaign.sh \
  tpc_track_fit_my_sample_kalman_v1 \
  output/dst_my_sample.list \
  2000 \
  10000
```

For a helix-only track tree, change `V0_FIT_METHOD=helix`; the same switch
skips pair reconstruction.

## Bunch-Crossing Selection

Crossing metadata is joined to each `TPC_POLYTRACKS` entry through
`get_source_assembled_track_id()` and the corresponding entry in
`TPC_CROSSING_DECISIONS`. The selection occurs before the helix or Kalman fit,
so rejected tracks consume no fitter or pair-PCA time.

To fit and reconstruct V0 candidates using only tracks selected as crossing
zero:

```bash
V0_REQUIRED_CROSSING=0 \
V0_REQUIRE_SAME_CROSSING=true \
V0_MAX_CROSSING_TIER=1 \
./submit_tpc_pattern_v0_event_campaign.sh \
  tpc_pattern_v0_my_sample_crossing0_v1 \
  output/dst_my_sample.list \
  2000 \
  10000
```

For a crossing-zero `trackTree` without V0 pair reconstruction, use the same
selection with track-fit-only mode:

```bash
V0_REQUIRED_CROSSING=0 \
V0_MAX_CROSSING_TIER=1 \
V0_RECONSTRUCT_PAIRS=false \
V0_FIT_METHOD=kalman \
V0_USE_KALMAN_FIELD_MAP=false \
V0_KALMAN_UNIFORM_PROPAGATOR=analytic \
./submit_tpc_pattern_v0_event_campaign.sh \
  tpc_pattern_tracks_my_sample_crossing0_v1 \
  output/dst_my_sample.list \
  2000 \
  10000
```

`V0_REQUIRE_SAME_CROSSING` is unnecessary in track-fit-only mode because no
pairs are formed.

The tier selection means:

```text
0  selected with tight silicon-vertex compatibility
1  also accept the loose silicon-vertex tier
2  also accept the TPC-containment fallback
-1 do not apply a tier cut
```

`V0_REQUIRED_CROSSING=any` disables the track-level crossing filter.
`V0_REQUIRE_SAME_CROSSING=true` still requires both daughters to have valid,
equal selected crossings and rejects mismatched pairs before trajectory PCA.
When `V0_REQUIRED_CROSSING=0` is active, the same-crossing requirement is
logically redundant but documents the intended pair contract.

Before launching a strict campaign, verify that an input DST contains
`TPC_CROSSING_DECISIONS`. Older DSTs containing only `TPC_POLYTRACKS` and
`TPC_POLYCLUSTERS` cannot recover the selected crossing after the fact and must
be regenerated with the crossing-decision node persisted.

## Kalman Field Configuration

Uniform field with the original analytic helix transport:

```bash
V0_USE_KALMAN_FIELD_MAP=false
V0_KALMAN_UNIFORM_PROPAGATOR=analytic
```

Uniform field with numerical RKN4 transport:

```bash
V0_USE_KALMAN_FIELD_MAP=false
V0_KALMAN_UNIFORM_PROPAGATOR=rk
```

Explicit nonuniform field map:

```bash
V0_USE_KALMAN_FIELD_MAP=true
V0_KALMAN_FIELD_MAP=/absolute/path/to/field_map.root
V0_KALMAN_RK_MAX_STEP_CM=5.0
V0_KALMAN_RK_STEP_TOLERANCE=1.0e-4
V0_KALMAN_RK_MAX_STEP_TRIALS=12
V0_KALMAN_RK_MAX_TOTAL_STEPS=2000
```

When a nonuniform `PHField` is present, Kalman state transport samples that
field during the numerical propagation. Field-map fitting is substantially
more expensive than analytic uniform-field propagation.

The field-map speed controls default to:

```text
V0_KALMAN_FAST_FIELD_JACOBIAN=true
V0_KALMAN_FAST_FIELD_PCA=true
V0_KALMAN_FIELD_PCA_REFINE_ITERATIONS=6
```

The first uses a local uniform-Bz surrogate for the finite-difference transport
Jacobian. The second uses a uniform-Bz trajectory to locate coarse pair-PCA
candidates and then refines them with the full field map. Disable these only
for dedicated accuracy studies because full mapped-field Jacobians and coarse
PCA searches are much more expensive.

The measurement uncertainties are configured in local cylindrical directions:

```text
V0_KALMAN_SIGMA_R_CM       radial position uncertainty [cm]
V0_KALMAN_SIGMA_RPHI_CM    tangential r*phi uncertainty [cm]
V0_KALMAN_SIGMA_Z_CM       longitudinal uncertainty [cm]
```

All three affect Kalman weighting. The r*phi and z values also define the helix
residual-based quality calculation. They are not the Kalman initial-state
covariance.

## Cut Stages

There are two cut stages.

### Preselection

Preselection runs before the expensive exact two-trajectory PCA search.

| Variable | Meaning | Script default |
|---|---|---:|
| `V0_PRE_TRACK_PT_MIN` | Minimum fitted daughter pT [GeV/c]. | `0.05` |
| `V0_PRE_TRACK_DCA_XY_MIN` | Minimum daughter transverse DCA to the selected collision vertex [cm]. | `-1` (off) |
| `V0_PRE_TRACK_QUALITY_MAX` | Maximum per-track fit chi2/ndf. | `-1` (off) |
| `V0_PRE_TRACK_NPOINTS_MIN` | Minimum number of valid associated cluster centroids. | `0` (off) |
| `V0_PRE_PAIR_DCA_MAX` | Maximum rough straight-line pair DCA [cm]. | `10` |
| `V0_PRE_LPROJ_MIN` | Minimum rough 3D displacement magnitude from the primary vertex [cm]. | `-1` (off) |
| `V0_PRE_COS_THETA_MIN` | Minimum rough pointing cosine. | `-2` (off) |

The rough quantities reduce CPU use; they are not replacements for the final
fitted-trajectory quantities. Pair-level preselection is not evaluated when
`V0_RECONSTRUCT_PAIRS=false`; track fitting and `trackTree` filling are still
performed.

### Exact pair selection

These cuts run after the helix/Kalman pair PCA has been calculated.

| Variable | Meaning | Script default |
|---|---|---:|
| `V0_PAIR_PCA_Z_MAX` | Maximum absolute z of the pair-PCA midpoint [cm]. | `-1` (off) |
| `V0_PAIR_PCA_DZ_MAX` | Maximum `abs(pca1_z-pca2_z)` [cm]. | `-1` (off) |
| `V0_PAIR_DECAY_RADIUS_MIN` | Minimum transverse distance from the configured primary vertex to the pair-PCA midpoint [cm]. | `-1` (off) |
| `V0_PAIR_ALPHA_ABS_MAX` | Maximum absolute AP alpha. Use `-1` when producing an unbiased AP plot. | `-1` (off) |
| `V0_PAIR_DCA_MAX` | Maximum exact trajectory-pair DCA [cm]. | `-1` (off) |
| `V0_PAIR_DIRA_MIN` | Minimum final pointing cosine, stored as `cosThetaReco`. | `-2` (off) |

`V0_WRITE_SAME_SIGN_PAIRS=true` stores both unlike-sign and like-sign pairs.
Apply `charge1*charge2<0` when plotting the unlike-sign signal sample. The
like-sign entries can be used for background studies.

## Other Important Settings

| Variable | Meaning | Default |
|---|---|---:|
| `V0_RECONSTRUCT_PAIRS` | Run the two-track V0 pair loop. Set false for track fitting and `trackTree` output only. | `true` |
| `V0_REQUIRED_CROSSING` | Keep only tracks with this selected crossing; use `any` to disable. | `any` |
| `V0_REQUIRE_SAME_CROSSING` | Require both V0 daughters to have valid, equal selected crossings. | `false` |
| `V0_MAX_CROSSING_TIER` | Maximum accepted crossing-confidence tier; `-1` disables. | `-1` |
| `V0_CROSSING_DECISION_NODE` | Input crossing-decision container. | `TPC_CROSSING_DECISIONS` |
| `V0_SOFTWARE_RELEASE` | sPHENIX software release used by the worker. It must match any local build. | `new` |
| `V0_LOCAL_INSTALL` | Optional local install prefix containing compatible development libraries. | `none` |
| `V0_OUTPUT_BASE_DIR` | Base directory for campaign output and completion files. | `TpcProduction/output` |
| `V0_INPUT_PATH_BASE` | Base directory prepended to relative paths in an input list. | `TpcProduction` |
| `V0_PRIMARY_VERTEX_X/Y/Z` | Fixed fallback primary vertex [cm]. | `0,0,0` |
| `V0_POINT_ORDER` | `auto`, `radius`, `theta-z`, `path`, or `input`. | `auto` |
| `V0_COARSE_STEPS` | Coarse samples per trajectory in pair-PCA candidate search. | `64` |
| `V0_PCA_CANDIDATES` | Number of coarse candidate minima retained for refinement. | `32` |
| `V0_WRITE_CLUSTER_RESIDUAL_TREE` | Write one additional row per cluster. Cluster residual vectors remain in `trackTree` even when this is false. | `false` |
| `V0_WRITE_KALMAN_INNOVATION_DIAGNOSTICS` | Write detailed per-measurement Kalman innovation vectors. | `false` |
| `V0_PRINT_TIMING` | Print per-stage and Kalman propagation timing. | `false` |

## Output Trees

Successful jobs are moved to:

```text
output/CAMPAIGN/completed/
```

The ROOT macro validates that both mandatory trees exist before the worker
moves a file into `completed/`.

`trackTree` contains one row per successfully built/fitted track before V0 pair
preselection. It includes fitted kinematics, DCA, fit quality, cluster counts,
and vector-valued cluster residual information. Its crossing fields include
`crossing`, `has_crossing_decision`, `has_selected_crossing`,
`crossing_status`, `crossing_tier`, and `crossing_score`. Kalman-specific
fields are filled for Kalman fits.

`pairTree` contains only pairs that passed all enabled campaign-level cuts. It
includes daughter momenta and charge, pair DCA/PCA, AP `alpha` and `qT`, DIRA,
decay position, daughter quality, dE/dx, and Kshort/Lambda mass hypotheses. In
addition to the real `cross1` and `cross2` values, it stores decision presence,
selection validity, status, tier, and score for both daughters. In
track-fit-only mode the tree is present with zero entries.

`clusterResidualTree` is optional and duplicates per-cluster information as
scalar rows. Keep it disabled for large productions unless that schema is
specifically needed.

## Monitor a Campaign

```bash
condor_q "$USER" -nobatch

campaign=tpc_pattern_v0_my_sample_kalman_v1
find "output/${campaign}/completed" -maxdepth 1 -type f -name '*.root' | wc -l
find "log/${campaign}" -maxdepth 1 -type f -name '*.err' -size +0 -print | head
```

For event campaigns, the required number of output chunks is:

```bash
wc -l "output/${campaign}/event_chunks.tsv"
```

An `.err` file can contain nonfatal ROOT warnings, so inspect its contents and
the corresponding `.out` file before classifying the job as failed.

## Merge Ordinary Outputs

After all intended jobs finish:

```bash
cd /path/to/macros/TpcProduction

campaign=tpc_pattern_v0_my_sample_kalman_v1
list="output/${campaign}/root_files.list"
merged="output/${campaign}_merged.root"

find "output/${campaign}/completed" \
  -maxdepth 1 -type f -name '*.root' | LC_ALL=C sort > "${list}"

wc -l "${list}"
hadd -f -k -j 8 "${merged}" @"${list}"
```

Run this in an sPHENIX environment where ROOT is set up. Check the list count
against the expected file-job count or event manifest count before merging.

ROOT cannot write a single TTree larger than its configured maximum file size.
For very large productions, keep several merged shards or analyze the completed
files with a `TChain` instead of forcing one monolithic file:

```cpp
TChain pairTree("pairTree");
pairTree.Add("output/CAMPAIGN/completed/*.root");
std::cout << pairTree.GetEntries() << std::endl;
```

## Analyze an AP Plot

The Armenteros-Podolanski coordinates are stored as `alpha` and `qT` in
`pairTree`. For example, after opening an output file in ROOT:

```cpp
pairTree->Draw("qT:alpha", "charge1*charge2<0", "colz");
```

Additional plotting selections can be applied at this stage. Remember that
`pairTree` has already been reduced by all cuts enabled during reconstruction;
analysis cannot recover pairs that were not written. For exploratory studies,
use loose campaign cuts and apply tighter selections afterward.

## Common Failure Checks

1. Confirm every input path in the list still exists.
2. Confirm the DST contains `T`, `TPC_POLYCLUSTERS`, and `TPC_POLYTRACKS`.
3. Confirm the runtime installation contains compatible
   `libtpctrackreco.so` and `libTrackingDiagnostics.so`.
4. Read `log/CAMPAIGN/pattern_v0_PROCESS.out` for the printed configuration and
   final output validation.
5. Read the matching `.err` and Condor `.log` files for crashes, holds, memory
   eviction, or disk failures.
6. For event mode, inspect `event_chunks.tsv` and the skipped-file report shown
   at submission.
