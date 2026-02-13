
# Active Good Run Lists

All active GRLs can be found in the `grls/` directory. Each GRL is named according to the pattern:

```bash
{dataset}_{tag}_{dsttype}_grl.list
```


You will not need to run the generator yourself unless you want to create a custom GRL with different cuts. If you do want to generate your own GRL, see the instructions below. 

**Note: The scripts in `scripts/` are pre-configured to produce the GRLs currently in `grls/`. They use extensive memory thorough psql queries. If something is wrong with a current GRL, let me know and I can fix the script and re-run it.**


## Current GRLs

| Dataset | Tag | DST Type | MB Bit | Events | Runs | Name |
|---------|-----|----------|--------|--------|------|-------|
| `run2pp` | `ana509_2024p022_v001` | `DST_CALOFITTING` | 10 | 19,822,421,631 | 1,564 | `run2pp_ana509_2024p022_v001_dst_calofitting_grl.list` |
| `run2auau` | `ana509_2024p022_v001` | `DST_CALOFITTING` | 10 | 759,481,790 | 110 | `run2auau_ana509_2024p022_v001_dst_calofitting_grl.list` |
| `run3pp` | `new_newcdbtag_v008` | `DST_CALOFITTING` | 12 | 6,527,498,042 | 788 | `run3pp_new_newcdbtag_v008_dst_calofitting_grl.list` |
| `run3auau` | `new_newcdbtag_v008` | `DST_CALOFITTING` | 12 | 10,979,359,411 | 1,086 | `run3auau_new_newcdbtag_v008_dst_calofitting_grl.list` | 
| `run3oo` | `ana536_2025p010_v001` | `DST_CALOFITTING` | 12 | 2,574,888,458 | 119 | `run3oo_new_newcdbtag_v008_dst_calofitting_grl.list` |



# sPHENIX Good Run List Generator

Generates Good Run Lists (GRLs) by querying the DAQ, FileCatalog, and Production databases and applying quality cuts.

## Quick Start (Expert only)

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
bash scripts/run3pp.sh
```

## Setup

Requires being on SDCC network and the sPHENIX environment. Set up the environment:

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

If you need to get a specific production tag, use this command to get all tags of a given dataset:
```bash
CreateDstList.pl --dataset <run 2 the hills> --printtags
```

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
