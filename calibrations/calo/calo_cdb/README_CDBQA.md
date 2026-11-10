# CDB calorimeter QA

Pages: https://sphenix-intra.sdcc.bnl.gov/WWW/subsystem/CDBCaloQA

## 1. Set up

```
source /opt/sphenix/core/bin/sphenix_setup.sh -n new
export CDBQA_BASE=/sphenix/user/$USER/cdbqa
```

`CDBQA_BASE` is your work area (histograms, logs).
Pages go to `/sphenix/WWW/subsystem/CDBCaloQA` unless you set `CDBQA_WEB`.

## 2. Test one run

```
root -l -b -q 'macros/Fun4All_CDBCaloQA.C("Run25OO", 82535, "newcdbtag", 200)'
```

## 3. Run a full period

```
scripts/make_qa_dag.sh Run25OO newcdbtag
```

Periods: `Run24pp`, `Run24AuAu`, `Run25pp`, `Run25AuAu`, `Run25OO`.

Histograms go to `$CDBQA_BASE`, pages to `/sphenix/WWW/subsystem/CDBCaloQA/<period>/<tag>`.

Trial run that does not touch the official pages: add `--test <dir>`, and everything goes to `<dir>`.

## 4. Check progress

```
tail $CDBQA_BASE/condor/dag/Run25OO_newcdbtag/qa.dag.dagman.out
```

The pages are written when all runs are done.
