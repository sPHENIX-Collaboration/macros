# sPHENIX Good Run List Generator

Generates Good Run Lists (GRLs) by querying the DAQ, FileCatalog, and Production databases and applying quality cuts.

## Quick Start

```bash
source setup.sh
./runListGen.py --dataset run3pp \
  --tag new_newcdbtag_v008 \
  --dsttype DST_CALOFITTING \
  --subsystems hcal emcal mbd \
  --min_bias_triggers 12 \
  --livetime_threshold 12:0.7 \
  --offline_status "emcal == GOLDEN" "ohcal == GOLDEN" "ihcal == GOLDEN"

# Or use a pre-made script
cd scripts && ./run3pp.sh
```

## Setup

Requires access to SDCC network (or VPN) and the sPHENIX environment.

```bash
source setup.sh
```

This loads the sPHENIX environment and sets `RUNLISTGEN_EXE`.

## Arguments

### Required

| Argument | Description |
|----------|-------------|
| `--dataset` | `run2pp`, `run2auau`, `run3pp`, `run3auau`, `run3oo` |
| `--tag` | Production tag (e.g., `ana509_2024p022_v001`, `new_newcdbtag_v008`) |
| `--dsttype` | DST type (e.g., `DST_CALOFITTING`) |

### L0 Cuts (DAQ-level)

| Argument | Default | Description |
|----------|---------|-------------|
| `--subsystems` | `[]` | Subsystems to require: `intt`, `tpc`, `mvtx`, `emcal`, `sepd`, `zdc`, `hcal`, `mbd`, `tpot`, `ll1` |
| `--runtype` | `physics` | Allowed run type(s) |
| `--min_run_length` | `300` | Min run length (seconds) |
| `--require_magnet_on` | `True` | Require magnet ON with current ≥ 4.5E7 |
| `--min_bias_triggers` | `[10,12,14]` | MB trigger bits to track |
| `--livetime_threshold` | `[]` | Format: `BIT:THRESHOLD` (e.g., `12:0.7`) |
| `--min_scalers` | `[]` | Format: `BIT:TYPE:VALUE` where TYPE is `S`/`L`/`R` |

### L1 Cuts (QA)

| Argument | Default | Description |
|----------|---------|-------------|
| `--min_reco_events` | `1,000,000` | Min events in FileCatalog |
| `--offline_status` | `[]` | QA predicates (see below) |

### Other

| Argument | Default | Description |
|----------|---------|-------------|
| `--first_run` / `--last_run` | Dataset default | Override run range |
| `--prefix` | `./` | Output directory |
| `--l0_report` | `summary` | `none`, `summary`, or `detailed` |

## Scripts

Pre-configured scripts in `scripts/`. Run with:

```bash
source setup.sh
cd scripts && bash run3pp.sh
```

### Summary

| Script | MB Bit | Min Events | QA | Notes |
|--------|--------|------------|-----|-------|
| `run2pp.sh` | 10 | 1M | Auto GOLDEN | Uses `_auto` QA columns |
| `run2auau.sh` | 10 | 100K | EMCAL GOLDEN | Less stringent |
| `run3pp.sh` | 12 | 1M | All GOLDEN | Includes min scaler cut |
| `run3auau.sh` | 12 | 100K | `<> BAD` | More permissive |
| `run3oo.sh` | 12 | 100K | All GOLDEN | Includes sEPD |

### run2pp.sh

Run 2 pp. Uses MB bit 10, automatic QA (`emcal_auto`, etc.), 1M event threshold.

### run2auau.sh

Run 2 Au+Au. MB bit 10, only requires `emcal == GOLDEN`, 100K events.

### run3pp.sh

Run 3 pp. MB bit 12, requires all calos GOLDEN, adds `--min_scalers 12:S:100_000`.

### run3auau.sh

Run 3 Au+Au. MB bit 12, uses `<> BAD` (accepts GOOD or GOLDEN), 100K events.

### run3oo.sh

Run 3 O+O. Same as run3pp but includes sEPD subsystem and 100K event threshold.

## Active GRLs

In `grls/`:

- `run2pp_ana509_2024p022_v001_dst_calofitting_grl.list`
- `run2auau_ana509_2024p022_v001_dst_calofitting_grl.list`
- `run3pp_new_newcdbtag_v008_dst_calofitting_grl.list`
- `run3auau_new_newcdbtag_v008_dst_calofitting_grl.list`
- `run3oo_new_newcdbtag_v008_dst_calofitting_grl.list`


## Output Files

Each run produces:

1. **`*_grl.list`** — Run numbers, one per line
2. **`*_run_details.json`** — Per-run info (events, scalers, QA status, etc.)
3. **`*_cutflow_report.json`** — Runs/events removed at each cut

## Subsystem → VGTM Mapping

| Subsystem | VGTM Bit(s) | QA Columns |
|-----------|-------------|------------|
| `intt` | 1 | `intt` |
| `tpc` | 0, 2 | `tpc` |
| `mvtx` | 4 | `mvtx` |
| `emcal` | 5 | `emcal`, `emcal_auto` |
| `sepd` | 8 | `sepd` |
| `zdc` | 8 | `zdc` |
| `hcal` | 9 | `ihcal`, `ohcal`, `*_auto` |
| `mbd` | 10 | `mbd` |
| `tpot` | 12 | `tpot` |
| `ll1` | 13 | — |

## QA Predicates

Use with `--offline_status`:

```bash
--offline_status "emcal == GOLDEN" "ohcal <> BAD" "ihcal IN (GOOD,GOLDEN)"
```

Operators: `==`, `<>`, `IN`, `NOT IN`, `IS NULL`, `IS NOT NULL`

Status values: `GOLDEN`, `BAD`, `QUESTIONABLE`

For calorimeters, both manual (`emcal`) and auto (`emcal_auto`) columns exist.

## Cutflow

**L0 (DAQ):** runtype → VGTMs → magnet → run length → scaledown → livetime → scalers

**L1 (QA):** FileCatalog merge → min events → offline status

## Databases

| Database | Host |
|----------|------|
| DAQ | `sphnxdaqdbreplica.sdcc.bnl.gov` |
| FileCatalog | `sphnxdbreplica.sdcc.bnl.gov` |
| Production | `sphnxproddbreplica.sdcc.bnl.gov` |

Requires SDCC network or VPN.

## Troubleshooting

- **"RUNLISTGEN_EXE is not set"** — Run `source setup.sh`
- **"OFFLINE_MAIN is not set"** — sPHENIX environment not loaded
- **"No DAQ/FileCatalog rows"** — Check network, verify tag/dsttype exists

## Contact

Questions? Contact Tanner.
