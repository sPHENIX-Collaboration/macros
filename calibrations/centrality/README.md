# Au+Au MBD centrality calibration

Makes the three centrality CDB payloads (`Centrality`, `CentralityScale`, `CentralityVertexScale`) that
`MinimumBiasClassifier` and `CentralityReco` read, for the three bunch classes of Au+Au running: the main bunch
and the -5 ns and +5 ns bunches. The classes are column suffixes in each payload (`""`, `_m5`, `_p5`).

The code is the `centralitycalib` package in coresoftware (`calibrations/centrality`):
`MbdCentralityTree` (the tree writer) and `CentralityCalibrator` (the three calibration steps). This directory has
the macros that run them, the condor scripts and the configuration of each running period.

## Steps

| step | macro | per | output |
|---|---|---|---|
| trees | `macros/Fun4All_CentralityTree.C` | DST segment, then `hadd` per run | `run<run>.root`: MBD vertex and t0, tube charges and times, ZDC sums, GL1 scaled bits |
| t0 offset | `macros/fit_t0offset.C` | run | `t0offset_<run>.csv`: the main-bunch t0, fitted; all runs make `t0_offsets.csv` |
| calibration | `macros/calibrate_run.C` | run | `centrality_calib_<run>.root`: per class, vertex scales and the run scale relative to the reference run; for the reference run also the NBD+Glauber fit and the divisions |
| payloads | `macros/make_payloads.C` | period | `payloads/{divs,scales,vertexscales}/*.root`, `payloads/payload_list.csv` |

The reference runs are calibrated first; every other run is calibrated against the reference run in column 2 of the
run list. A reference run's charge sum is fitted with an NBD convolved with the Glauber Npart distribution
(`glauber/lime_auau_hists_nominal.root`); the fit's trigger efficiency (`trigeff_balanced` in the file, printed in the
log) is the fraction of the cross section the min-bias selection sees, and sets the number of divisions
(`NDIVISIONS` in the period configuration). Every payload carries the reference run's divisions. A run whose column 3
is 0, or whose t0 offset is not usable (status other than 0 or 3), gets the reference run's vertex scales and scale 1.

## Running a period

```
source /opt/sphenix/core/bin/sphenix_setup.sh -n ana.579
export CENT_TREEDIR=<bulk disk for the trees> CENT_WORKDIR=<work directory>
export CENT_INSTALL=<local install of calibrations/centrality>   # only until it is in the release
scripts/make_dag.sh run25auau
```

`make_dag.sh` gets the DST segments from the file catalog and writes one DAG for the period: the tree jobs of each run,
then its per-run tree and t0 fit, the t0 offset table, the reference runs, the other runs, and the payloads. It prints
the submit command. One step can also be run by hand, for example
`scripts/centrality_job.sh run25auau calib 68749 76546`.

The payloads are local files. They are not inserted into the CDB.

## Periods

`periods/<period>.sh` sets the DST production, the software build, the CDB tag of the reconstruction, the GL1 min-bias
trigger bit, the t0 fit seed window, the Glauber file and the number of divisions, the segments per tree job and the
memory requests. `runlists/<period>.list` has
one `run refrun calibrate` line per run.

| period | runs | reference runs | trigger bit |
|---|---|---|---|
| `run24auau` | 114 | 54546, 54803, 54912, 54944 (one per block) | 10 |
| `run25auau` | 2781 | 76546 | 14 |

## Selection

- GL1 scaled min-bias bit (above)
- at least 2 hit tubes in each MBD arm: q > 0.4 and |t_hit - t0 offset| < 25 ns; the charge sum is over these tubes
- ZDC south and north energy > 40
- |z_vtx| <= 60 cm (MBD vertex)
- bunch class from the event t0 - t0 offset: main [-2.5, 2.5] ns, -5 ns [-7.5, -2.5) ns, +5 ns (2.5, 7.5] ns
- divisions: `NDIVISIONS` equal-population bins of the reference run's vertex-balanced charge sum (balanced sum < 2100)
- NBD+Glauber fit: charge sum bins 700-2000

The t0 fit uses the same trigger, hit, and ZDC cuts with no t0 offset, |z_vtx| <= 150 cm, and the first 500k events.
