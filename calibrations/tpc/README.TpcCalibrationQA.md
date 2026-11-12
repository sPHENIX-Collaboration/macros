# TPC calibration QA

Requires the coresoftware package `offline/QA/TpcCalibrationQA` installed in the
active sPHENIX environment. The macro configures the compiled library; the
Python script handles production inventory, per-run process isolation, caching,
conditions/reference exports and portable summary collection.

## Production workflow

```bash
python3 run_tpc_calibration_qa.py discover \
  --source run3pp=/path/to/production/DST_TRKR_SEED \
  --run-list runs.txt --fast --max-files 10 --out inventory

python3 run_tpc_calibration_qa.py run \
  --inventory inventory/inventory.json --work work --out summary \
  --cdb-tag newcdbtag --reference-field 376.85 \
  --keff-fallback phgarfield-defaults \
  --reference run3pp=81558 --pt-windows 0.5:1 \
  --fit-model gaussian-background --max-files 10 --jobs 1
```

Use actual production paths and an available reference run. `run` exports the
conditions and PHGarfield tables before processing; failed exports stop the
workflow. It writes `summary/tpc_qa_summary.root`, preserving the earlier ROOT
object paths. CDB base coefficients remain distinct from historically verified
applied corrections. The default field and CDB tag above are an explicit example,
not an inferred description of a production campaign.

Use `process` with `--conditions-csv` and `--phg-reference-csv` to process already
exported snapshots. `collect` operates on an existing `latest_batch.json`.
`conditions` or `phgarfield` runs only the corresponding exporter. `analyze`
accepts explicit datasets or file lists and supports independent reference
samples, raw-time offsets, detailed slices, phi-q/pT studies and optional PDFs.
Every entry point has `--help`.

The runner hashes the resolved installed library, itself and the macro for
cache invalidation. Use a fresh work directory if CDB content changes under
the same tag. Reconstruction is never rerun and production files are not changed.

## Direct ROOT macro

A manifest contains `label<TAB>/absolute/path/to/inputs.list` for each sample;
the input list contains one absolute ROOT filename per line. Optional manifest
columns supply a separate drift-file list and a raw-time shift.

```cpp
.L Fun4All_TpcCalibrationQA.C
Fun4All_TpcCalibrationQA("samples.tsv", "new_output", "reference_label");
```

The histogram postprocessor is an ordinary C++ class, with no artificial event
loop. Only the PHGarfield reference uses Fun4All run initialization. Reference
exports run in isolated ROOT processes, not alongside active reconstruction.

The optional `DriftVelocityQA` event collector is separate from this workflow.

## Drift input availability

Official upstream QA provides the `TpcRawHitQA` timing fallback. The optional
`h_TpcDriftVelocityQA_*` and `h_PHGarfieldDriftVelocityQA_*` inputs require
additional histogram-producing modules that are not part of upstream
coresoftware. This package reads those histograms when supplied; it does not
produce them. Missing ADC-weighted timing/z diagnostics are reported as
unavailable rather than inferred from thresholded sample counts.

Native calibration sums from the companion producer use `TH1D` (double
precision). Geometry and clock compatibility retain the existing absolute
`1e-6` tolerance in the respective cm/ns units. Float calibration-sum inputs
are not an established supported producer format.

## Batch recovery and central installation

New batch jobs use persistent `.lock` files with POSIX advisory locks. A killed
process releases its lock automatically; child ROOT processes retain it until
they exit. Do not delete `.lock` files: their presence does not mean a job is
running. An active job reports `Already running`. Legacy `.running` directories
are preserved: verify that the old job has stopped before removing one.

`run --allow-partial` permits per-run export failures and records/excludes
those runs during processing. Collection still requires a completed reference
run for every selected production group. Without the flag, failed exports
stop processing. Plotting flags control PDF creation only, not analysis ROOT
objects.

Central release installation additionally requires a registration in the
separate `sPHENIX-Collaboration/utilities` repository at
`utils/rebuild/packages.txt`, after the TPC QA entry. The source bundle includes
`utilities_packages.patch` for this companion change.
