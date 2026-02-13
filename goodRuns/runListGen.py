#!/usr/bin/env python3

import sys
import os
import re
import argparse
from typing import Any, Dict, List, Optional, Tuple, Callable
import json

import psycopg2
import psycopg2.extras

from dataclasses import dataclass

DB_DAQ = dict(dbname="daq", host="sphnxdaqdbreplica.sdcc.bnl.gov", user="phnxro", port=5432)
DB_FILEDB = dict(dbname="FileCatalog", host="sphnxdbreplica.sdcc.bnl.gov", user="argouser", port=5432)
DB_PROD = dict(dbname="Production", host="sphnxproddbreplica.sdcc.bnl.gov", user="phnxrc", port=5432)

# DEFAULT_L0ARGS = {
#     # these will be set by dataset
#     "first_run": None,
#     "last_run": None,

#     "min_run_length": 300,
#     "require_magnet_on": True,
#     "runtypes": ["physics"],
    
#     # requested VGTMs (from --subsystems); by default require *all* requested vgtms present
#     "require_vgtms_all": True,

#     "scalar_bits": [12, 10, 14],
#     "scaledown_bits": [12, 10, 14],
#     "require_scaledown_bits": [10],
#     "livetime_bits": [10],
#     "min_livetime": 0.70,
#     "min_scalar_bits": [10],
#     "min_scaled": None,
#     "min_live": None,
#     "min_raw": None,
# }

def parse_subsystems(subsystems: List[str]) -> Dict[str, List[Any]]:
    subsystem_map: Dict[str, List[Any]] = {
        "vgtms": [],   # ll1 always included
        "offlineQA": [],
    }

    for ss in subsystems:
        s = ss.lower()
        if s == "intt":
            subsystem_map["vgtms"].extend([1])
            subsystem_map["offlineQA"].extend(["intt"])
        elif s == "tpc":
            subsystem_map["vgtms"].extend([0, 2])
            subsystem_map["offlineQA"].extend(["tpc"])
        elif s == "mvtx":
            subsystem_map["vgtms"].extend([4])
            subsystem_map["offlineQA"].extend(["mvtx"])
        elif s == "emcal":
            subsystem_map["vgtms"].extend([5])
            subsystem_map["offlineQA"].extend(["emcal", "emcal_auto"])
        elif s == "sepd":
            subsystem_map["vgtms"].extend([8])
            subsystem_map["offlineQA"].extend(["sepd"])
        elif s == "zdc":
            subsystem_map["vgtms"].extend([8])
            subsystem_map["offlineQA"].extend(["zdc"])
        elif s == "hcal":
            subsystem_map["vgtms"].extend([9])
            subsystem_map["offlineQA"].extend(["ihcal", "ohcal", "ihcal_auto", "ohcal_auto"])
        elif s == "mbd":
            subsystem_map["vgtms"].extend([10])
            subsystem_map["offlineQA"].extend(["mbd"])
        elif s == "tpot":
            subsystem_map["vgtms"].extend([12])
            subsystem_map["offlineQA"].extend(["tpot"])
        elif s == "ll1":
            subsystem_map["vgtms"].extend([13])
        else:
            print(f"Warning: Unknown subsystem '{ss}'", file=sys.stderr)

    subsystem_map["vgtms"] = sorted(set(subsystem_map["vgtms"]))
    subsystem_map["offlineQA"] = sorted(set(subsystem_map["offlineQA"]))
    return subsystem_map

_MANUAL_AUTO_PAIRS = {
    "emcal": "emcal_auto",
    "ohcal": "ohcal_auto",
    "ihcal": "ihcal_auto",
}

@dataclass(frozen=True)
class RunRanges:
    constants: Dict[str, int]
    datasets: Dict[str, Tuple[int, int]]

class RunnumberRange:
    _PATTERN = re.compile(r"\bstatic\s+const\s+int\s+([A-Z0-9_]+)\s*=\s*([0-9]+)\s*;")

    def __init__(self, path: str = ""):
        if not path:
            offline_main = os.environ.get("OFFLINE_MAIN")
            if not offline_main:
                raise RuntimeError("OFFLINE_MAIN is not set")
            path = os.path.join(offline_main, "include", "phool", "RunnumberRange.h")

        self.path = path
        self.ranges: RunRanges = self._load()
        self.datasets = self.ranges.datasets

    def _load(self) -> RunRanges:
        constants: Dict[str, int] = {}
        with open(self.path, "r", encoding="utf-8") as f:
            for raw in f:
                line = raw.split("//", 1)[0]
                m = self._PATTERN.search(line)
                if m:
                    name, val = m.group(1), int(m.group(2))
                    constants[name] = val

        def require(name: str) -> int:
            if name not in constants:
                raise KeyError(f"Missing constant {name} in {self.path}")
            return constants[name]

        datasets = {
            "run2pp": (require("RUN2PP_FIRST"), require("RUN2PP_LAST")),
            "run2auau": (require("RUN2AUAU_FIRST"), require("RUN2AUAU_LAST")),
            "run3auau": (require("RUN3AUAU_FIRST"), require("RUN3AUAU_LAST")),
            "run3pp": (require("RUN3PP_FIRST"), require("RUN3PP_LAST")),
            "run3oo": (require("RUN3PP_LAST")+1, 100_000),
        }
        return RunRanges(constants=constants, datasets=datasets)

    def get_range(self, dataset: str) -> Tuple[int, int]:
        try:
            # if dataset == "run3oo":
            #     mins = self.ranges.datasets["run3pp"][1] + 1
            #     maxs = 100_000
            #     return (mins, maxs)

            return self.ranges.datasets[dataset]
        except KeyError:
            raise KeyError(f"Unknown dataset '{dataset}'. Known: {sorted(self.ranges.datasets)}")

    def get_constant(self, name: str) -> int:
        try:
            return self.ranges.constants[name]
        except KeyError:
            raise KeyError(f"Unknown constant '{name}'. Known: {sorted(self.ranges.constants)}")

# -------------------------------------------------------------------------
# DAQ base query (NO L0 cuts in WHERE; we do them in Python for cutflow)
# -------------------------------------------------------------------------

DAQ_BASE_SQL = """
WITH
gtm_global AS (
    SELECT
        g.runnumber,
        string_agg(g.vgtm::text, ',' ORDER BY g.vgtm) AS vgtm_list
    FROM gtm g
    WHERE g.global = true
      AND g.vgtm = ANY(%(vgtms)s)
    GROUP BY g.runnumber
),
magnet AS (
    SELECT
        m.runnumber,
        m.current,
        m.magnet_on
    FROM magnet_info m
),
run_info AS (
    SELECT
        r.runnumber,
        r.runtype,
        CAST(EXTRACT(EPOCH FROM r.ertimestamp) AS BIGINT)
      - CAST(EXTRACT(EPOCH FROM r.brtimestamp) AS BIGINT) AS run_length
    FROM run r
),
scaledown_array AS (
    SELECT
        s.runnumber,
        array_agg(ROW(b.bit, b.scaledown) ORDER BY b.bit) AS scaledowns
    FROM gl1_scaledown s
    JOIN LATERAL (
        SELECT
            bit,
            (ARRAY[
                s.scaledown00, s.scaledown01, s.scaledown02, s.scaledown03,
                s.scaledown04, s.scaledown05, s.scaledown06, s.scaledown07,
                s.scaledown08, s.scaledown09, s.scaledown10, s.scaledown11,
                s.scaledown12, s.scaledown13, s.scaledown14, s.scaledown15,
                s.scaledown16, s.scaledown17, s.scaledown18, s.scaledown19,
                s.scaledown20, s.scaledown21, s.scaledown22, s.scaledown23,
                s.scaledown24, s.scaledown25, s.scaledown26, s.scaledown27,
                s.scaledown28, s.scaledown29, s.scaledown30, s.scaledown31,
                s.scaledown32, s.scaledown33, s.scaledown34, s.scaledown35,
                s.scaledown36, s.scaledown37, s.scaledown38, s.scaledown39,
                s.scaledown40, s.scaledown41, s.scaledown42, s.scaledown43,
                s.scaledown44, s.scaledown45, s.scaledown46, s.scaledown47,
                s.scaledown48, s.scaledown49, s.scaledown50, s.scaledown51,
                s.scaledown52, s.scaledown53, s.scaledown54, s.scaledown55,
                s.scaledown56, s.scaledown57, s.scaledown58, s.scaledown59,
                s.scaledown60, s.scaledown61, s.scaledown62, s.scaledown63
            ])[bit + 1] AS scaledown
        FROM generate_series(0, 63) AS bit
        WHERE bit = ANY(%(scaledown_bits)s)
    ) AS b ON true
    GROUP BY s.runnumber
),
scalar_rows AS (
    SELECT
        gs.runnumber,
        gs."index" AS bit,
        gs.raw,
        gs.live,
        gs.scaled,
        CASE
          WHEN gs.raw > 0 THEN (gs.live::FLOAT / gs.raw::FLOAT)
          ELSE NULL
        END AS livetime
    FROM (
        SELECT DISTINCT ON (runnumber, "index")
            runnumber, "index", raw, live, scaled, updated
        FROM gl1_scalers
        WHERE "index" = ANY(%(scalar_bits)s)
        ORDER BY runnumber, "index", updated DESC
    ) gs
),
scalar_array AS (
    SELECT
        sr.runnumber,
        array_agg(ROW(sr.bit, sr.raw, sr.live, sr.scaled) ORDER BY sr.bit) AS scalars
    FROM scalar_rows sr
    GROUP BY sr.runnumber
)
SELECT
    r.runnumber,
    r.runtype,
    r.run_length,
    g.vgtm_list,
    m.current AS magnet_current,
    m.magnet_on,
    sa.scaledowns,
    sc.scalars
FROM run_info r
LEFT JOIN gtm_global       g  ON g.runnumber  = r.runnumber
LEFT JOIN magnet           m  ON m.runnumber  = r.runnumber
LEFT JOIN scaledown_array  sa ON sa.runnumber = r.runnumber
LEFT JOIN scalar_array     sc ON sc.runnumber = r.runnumber
WHERE
    (%(first_run)s IS NULL OR r.runnumber >= %(first_run)s)
    AND (%(last_run)s IS NULL OR r.runnumber <= %(last_run)s)
ORDER BY r.runnumber;
""".strip()

FILECATALOG_SQL = """
SELECT
    runnumber,
    SUM(events)        AS events_sum,
    MIN(firstevent)    AS firstevent_min,
    MAX(lastevent)     AS lastevent_max,
    BOOL_AND(status = 1) AS all_ok
FROM datasets
WHERE dataset = %(dataset)s
  AND tag = %(tag)s
  AND dsttype = %(dsttype)s
GROUP BY runnumber
ORDER BY runnumber;
""".strip()

def _expr_status_reason_prefer_manual(manual_col: str, auto_col: str) -> Tuple[str, str]:
    status = f"""
  CASE
    WHEN ({manual_col}).runclass = 'BAD' AND COALESCE(NULLIF(({manual_col}).notes,''), '') = '' THEN
      CASE
        WHEN ({auto_col}).runclass = 'BAD' AND COALESCE(NULLIF(({auto_col}).notes,''), '') = '' THEN NULL
        ELSE ({auto_col}).runclass
      END
    WHEN ({manual_col}).runclass IS NOT NULL
         AND ({auto_col}).runclass IS NOT NULL
         AND ({manual_col}).runclass <> ({auto_col}).runclass THEN ({manual_col}).runclass
    ELSE COALESCE(({manual_col}).runclass, ({auto_col}).runclass)
  END AS {manual_col}_status
""".rstrip()

    reason = f"""
  CASE
    WHEN ({manual_col}).runclass = 'BAD' AND COALESCE(NULLIF(({manual_col}).notes,''), '') = '' THEN NULLIF(({auto_col}).notes,'')
    WHEN ({manual_col}).runclass IS NOT NULL
         AND ({auto_col}).runclass IS NOT NULL
         AND ({manual_col}).runclass <> ({auto_col}).runclass THEN NULLIF(({manual_col}).notes,'')
    ELSE COALESCE(NULLIF(({manual_col}).notes,''), NULLIF(({auto_col}).notes,''))
  END AS {manual_col}_reason
""".rstrip()
    return status, reason

def _expr_status_reason_simple(col: str) -> Tuple[str, str]:
    status = f"""
  CASE
    WHEN ({col}).runclass = 'BAD' AND COALESCE(NULLIF(({col}).notes,''), '') = '' THEN NULL
    ELSE ({col}).runclass
  END AS {col}_status
""".rstrip()

    reason = f"  NULLIF(({col}).notes,'') AS {col}_reason"
    return status, reason

def build_production_triage_query(qa_cols: List[str]) -> str:
    seen = set()
    cols: List[str] = []
    for c in qa_cols:
        c = c.strip()
        if not c or c in seen:
            continue
        seen.add(c)
        cols.append(c)

    select_bits: List[str] = ["  runnumber"]

    for manual, auto in _MANUAL_AUTO_PAIRS.items():
        if manual in cols or auto in cols:
            st, rs = _expr_status_reason_prefer_manual(manual, auto)
            select_bits.append(st)
            select_bits.append(rs)

            stA, rsA = _expr_status_reason_simple(auto)
            select_bits.append(stA)
            select_bits.append(rsA)

    for c in cols:
        if c in _MANUAL_AUTO_PAIRS or c in _MANUAL_AUTO_PAIRS.values():
            continue
        st, rs = _expr_status_reason_simple(c)
        select_bits.append(st)
        select_bits.append(rs)

    select_clause = ",\n".join(select_bits)

    return f"""
SELECT
{select_clause}
FROM goodruns
WHERE runnumber BETWEEN %(first_run)s AND %(last_run)s
ORDER BY runnumber;
""".strip()

def query_rows(conninfo: Dict[str, Any], sql: str, params: Optional[Dict[str, Any]] = None) -> List[Dict[str, Any]]:
    with psycopg2.connect(**conninfo) as conn:
        with conn.cursor() as cur:
            cur.execute(sql, params or {})
            cols = [d.name for d in cur.description]
            out = []
            for row in cur.fetchall():
                out.append({cols[i]: row[i] for i in range(len(cols))})
            return out

# -------------------------------------------------------------------------
# Helpers to parse DAQ composite arrays returned as strings (robust)
# -------------------------------------------------------------------------

def fmt_int(n: Optional[int]) -> str:
    if n is None:
        return "-"
    return f"{int(n):,}"
def fmt_int_list(lst: Optional[List[int]]) -> str:
    if lst is None:
        return "-"
    # if this is a single int in a list, just print that
    if type(lst) == int or (isinstance(lst, list) and len(lst) == 1):
        return fmt_int(lst[0]) if isinstance(lst, list) else fmt_int(lst)

    return ", ".join(fmt_int(x) for x in lst)

def fmt_pct(x: Optional[float]) -> str:
    if x is None:
        return "-"
    return f"{100.0*x:5.1f}%"

def fmt_stage(s: str, width: int = 44) -> str:
    s = s.replace("\n", " ").strip()
    if len(s) <= width:
        return s.ljust(width)
    return (s[: width - 1] + "…")

def events_sum_fc_only(rows: List[Dict[str, Any]], events_by_run: Dict[int, int]) -> int:
    """Only sum events for runs that exist in FileCatalog."""
    tot = 0
    for r in rows:
        tot += int(events_by_run.get(int(r["runnumber"]), 0))
    return tot

def print_section(title: str) -> None:
    print(f"\n== {title} ==")

_tuple4_re = re.compile(r"\((\d+),(\d+),(\d+),(\d+)\)")
_tuple2_re = re.compile(r"\((\d+),(-?\d+)\)")

def parse_scalar_array(val: Any) -> Dict[int, Dict[str, int]]:
    """
    Return {bit: {"raw":..., "live":..., "scaled":...}}
    Handles:
      - string like "{(12,1,2,3),(10,...)...}" or "(12,1,2,3)..."
      - list of tuples [(12,1,2,3), ...]
    """
    out: Dict[int, Dict[str, int]] = {}
    if val is None:
        return out
    if isinstance(val, list):
        for t in val:
            if not t or len(t) < 4:
                continue
            bit, raw, live, scaled = t[0], t[1], t[2], t[3]
            out[int(bit)] = {"raw": int(raw), "live": int(live), "scaled": int(scaled)}
        return out
    s = str(val)
    for bit, raw, live, scaled in _tuple4_re.findall(s):
        out[int(bit)] = {"raw": int(raw), "live": int(live), "scaled": int(scaled)}
    return out

def parse_scaledown_array(val: Any) -> Dict[int, int]:
    """
    Return {bit: scaledown}
    Handles string or list[(bit, scaledown)]
    """
    out: Dict[int, int] = {}
    if val is None:
        return out
    if isinstance(val, list):
        for t in val:
            if not t or len(t) < 2:
                continue
            out[int(t[0])] = int(t[1])
        return out
    s = str(val)
    for bit, sd in _tuple2_re.findall(s):
        out[int(bit)] = int(sd)
    return out

def parse_vgtm_list(val: Any) -> List[int]:
    if not val:
        return []
    if isinstance(val, list):
        return [int(x) for x in val]
    s = str(val).strip()
    if not s:
        return []
    return [int(x) for x in s.split(",") if x.strip()]

def mb_scaled_sum(rows: List[Dict[str, Any]], bit: int) -> int:
    tot = 0
    bits = int(bit)
    # first = True
    for r in rows:
        scal = parse_scalar_array(r.get("scalars"))
        keys = list(scal.keys())
        
        if bits in scal:
            tot += int(scal[bits]["scaled"])
            # tot += int(scal[bit]["scaled"])
    return tot

def events_sum(rows: List[Dict[str, Any]], events_by_run: Optional[Dict[int, int]] = None) -> int:
    """
    Sum events for these rows.
    - If rows already have 'events_sum', use it.
    - Else if events_by_run is provided, sum events_by_run[runnumber] where available.
    """
    if not rows:
        return 0
    if "events_sum" in rows[0]:
        return int(sum((r.get("events_sum", 0) or 0) for r in rows))

    if not events_by_run:
        return 0

    tot = 0
    for r in rows:
        run = int(r["runnumber"])
        tot += int(events_by_run.get(run, 0))
    return tot

def cutflow_line(label: str, rows: List[Dict[str, Any]], events_by_run: Optional[Dict[int, int]] = None) -> Dict[str, Any]:
    return {
        "stage": label,
        "runs": len(rows),
        "events": events_sum(rows, events_by_run=events_by_run),
        "mb_scaled": mb_scaled_sum(rows, bit=12),
    }

def print_cutflow_table(
    lines: List[Dict[str, Any]],
    title: str,
    *,
    stage_width: int = 44,
    include_removed: bool = True,
    include_eff: bool = True,
) -> None:
    print_section(title)


    hdr = f"{'Stage':{stage_width}s} {'Runs':>8s} {'Removed':>8s} {'Eff':>6s} {'Events':>14s}"
    # get mb scalers in lines
    mb_bits = set()
    for ln in lines:
        mb = ln.get("mb_scaled", [])
        # print(ln)
        if isinstance(mb, list):
            for i in range(len(mb)):
                mb_bits.add(i)
    for b in sorted(mb_bits):
        hdr += f" {'MB_scaled_bit'+str(b):>12s}"

    print(hdr)
    print("-" * len(hdr))

    prev_runs: Optional[int] = None
    for ln in lines:
        runs = int(ln.get("runs", 0))
        removed = (prev_runs - runs) if (prev_runs is not None) else 0
        eff = (runs / prev_runs) if (prev_runs and prev_runs > 0) else None

        stage = fmt_stage(str(ln.get("stage", "")), width=stage_width)
        ev = ln.get("events", 0)
        mb = ln.get("mb_scaled", 0)
        col_removed = f"{fmt_int(removed):>8s}" if include_removed else ""
        col_eff = f"{fmt_pct(eff):>6s}" if include_eff else ""
        mb_str = fmt_int_list(mb) if mb else ""
        output= f"{stage:{stage_width}s} {fmt_int(runs):>8s} {col_removed} {col_eff} {fmt_int(ev):>14s}"
        for b in sorted(mb_bits):
            mb_val = mb[b] if (isinstance(mb, list) and b < len(mb)) else 0
            output += f" {fmt_int(mb_val):>12s}"
        print(output)
        # print(
            # f"{stage:{stage_width}s} "
        #     f"{fmt_int(runs):>8s} "
        #     f"{col_removed} "
        #     f"{col_eff} "
        #     f"{fmt_int(ev):>14s} "
        #     # f"{fmt_int_list(mb):>12s}"
        #     # f"{mb_str:>12s}"
            
        # )

        prev_runs = runs

# -------------------------------------------------------------------------
# L0 cut application + cutflow
# -------------------------------------------------------------------------

def apply_l0_cuts(
    daq_rows: List[Dict[str, Any]],
    l0args: Dict[str, Any],
    requested_vgtms: List[int],
    events_by_run: Optional[Dict[int, int]] = None,
) -> Tuple[List[Dict[str, Any]], Dict[str, Any]]:
    """
    Apply L0 selection in Python and produce a cutflow report.

    Notes
    - events_* fields are computed using events_sum(..., events_by_run=...)
      which will sum FileCatalog events ONLY for runs present in events_by_run.
    - If l0args["_store_rows_after"] is True, each step will include "rows_after"
      (list of runnumbers that survive that step) to enable accurate FC-only per-step
      event accounting without recomputing cuts.
    - If l0args["_store_removed_runs"] is False, the report will NOT store per-cut
      removed run lists (useful to reduce memory/noise for summary mode).
    """

    steps: List[Dict[str, Any]] = []

    store_rows_after = bool(l0args.get("_store_rows_after", False))
    store_removed_runs = bool(l0args.get("_store_removed_runs", True))

    def _mb(rows: List[Dict[str, Any]]) -> int:
        vals= [int(mb_scaled_sum(rows, bit=b)) for b in l0args.get("min_bias_triggers", [])]
        return vals

    def _ev(rows: List[Dict[str, Any]]) -> int:
        return events_sum(rows, events_by_run=events_by_run)

    def run_step(
        name: str,
        rows: List[Dict[str, Any]],
        keep_fn: Callable[[Dict[str, Any]], bool],
    ) -> List[Dict[str, Any]]:
        before_n = len(rows)
        before_mb = _mb(rows)
        before_ev = _ev(rows)

        kept: List[Dict[str, Any]] = []
        removed_runs: List[int] = []

        for r in rows:
            if keep_fn(r):
                kept.append(r)
            else:
                if store_removed_runs:
                    removed_runs.append(int(r["runnumber"]))

        after_n = len(kept)
        after_mb = _mb(kept)
        after_ev = _ev(kept)

        # subract list element-wise if we have per-cut lists, else just total diff

        step: Dict[str, Any] = {
            "cut": name,
            "before": before_n,
            "after": after_n,
            "removed": before_n - after_n,

            "eff": (after_n / before_n) if before_n > 0 else None,

            "mb_scaled_before": before_mb,
            "mb_scaled_after": after_mb,
            "mb_scaled_removed": [before_mb[i] - after_mb[i] for i in range(len(before_mb))] if (isinstance(before_mb, list) and isinstance(after_mb, list) and len(before_mb) == len(after_mb)) else None,

            "events_before": before_ev,
            "events_after": after_ev,
            "events_removed": before_ev - after_ev,
        }

        if store_removed_runs:
            step["removed_runs"] = removed_runs

        if store_rows_after:
            # store only runnumbers to keep report lightweight
            step["rows_after"] = [int(x["runnumber"]) for x in kept]

        steps.append(step)
        return kept

    rows = daq_rows

    # 0) runtype filter
    runtypes = l0args.get("runtypes")
    if runtypes:
        want = set(runtypes)
        rows = run_step(
            "runtype in " + ",".join(sorted(want)),
            rows,
            lambda r: (r.get("runtype") in want),
        )

    # 1) require vgtms present (all requested)
    if requested_vgtms and l0args.get("require_vgtms_all", True):
        req = set(int(x) for x in requested_vgtms)

        def _keep_vgtm(r: Dict[str, Any]) -> bool:
            have = set(parse_vgtm_list(r.get("vgtm_list")))
            return req.issubset(have)

        rows = run_step(f"require_vgtms_all: {sorted(req)}", rows, _keep_vgtm)

    # 2) magnet
    if l0args.get("require_magnet_on", False):
        def _keep_mag(r: Dict[str, Any]) -> bool:
            cur = r.get("magnet_current")
            return (r.get("magnet_on") is True) and (cur is not None) and (float(cur) >= 4.5e7)
        rows = run_step("magnet_on && current>=4.5e7", rows, _keep_mag)

    # 3) run length
    min_rl = l0args.get("min_run_length")
    if min_rl is not None:
        min_rl_i = int(min_rl)
        rows = run_step(
            f"run_length >= {min_rl_i}",
            rows,
            lambda r: (r.get("run_length") is not None and int(r["run_length"]) >= min_rl_i),
        )

    # 4) require_scaledown_bits (scaledown != -1)
    req_sd = l0args.get("require_scaledown_bits") or []
    if req_sd:
        req_bits = [int(b) for b in req_sd]

        def _keep_sd(r: Dict[str, Any]) -> bool:
            sds = parse_scaledown_array(r.get("scaledowns"))
            for b in req_bits:
                if b not in sds:
                    return False
                if int(sds[b]) == -1:
                    return False
            return True

        rows = run_step(f"require_scaledown_bits != -1: {req_bits}", rows, _keep_sd)

    # 5) livetime cut
    min_lt = l0args.get("min_livetime") # list
    lt_bits = l0args.get("livetime_bits")  # list
    for i in range(len(lt_bits)):
        b_i = int(lt_bits[i])
        # get this bits min_lt from min_lt list
        this_min_lt = min_lt[i] if (i < len(min_lt)) else None

        def _keep_lt_bit(r: Dict[str, Any]) -> bool:
            scal = parse_scalar_array(r.get("scalars"))
            if b_i not in scal:
                return False
            raw = int(scal[b_i]["raw"])
            live = int(scal[b_i]["live"])
            if raw <= 0:
                return False
            if (float(live) / float(raw)) < this_min_lt:
                return False
            return True
        rows = run_step(f"livetime(bit={b_i}) >= {this_min_lt}", rows, _keep_lt_bit)
    
    # if (min_lt is not None) and lt_bits:
    #     bits = [int(b) for b in lt_bits]
    #     min_lt_f = float(min_lt)

    #     def _keep_lt(r: Dict[str, Any]) -> bool:
    #         scal = parse_scalar_array(r.get("scalars"))
    #         for b in bits:
    #             if b not in scal:
    #                 return False
    #             raw = int(scal[b]["raw"])
    #             live = int(scal[b]["live"])
    #             if raw <= 0:
    #                 return False
    #             if (float(live) / float(raw)) < min_lt_f:
    #                 return False
    #         return True

    #     rows = run_step(f"livetime(bits={bits}) >= {min_lt_f}", rows, _keep_lt)

    # 6) min scalar thresholds
    min_raw = l0args.get("min_raw")
    min_live = l0args.get("min_live")
    min_scaled = l0args.get("min_scaled")
    ms_bits = l0args.get("min_scalar_bits") or []
    # for b in ms_bits:
    for i in range(len(ms_bits)):
        b_i = int(ms_bits[i])
        raw_i = min_raw[i] if (min_raw and i < len(min_raw)) else None
        live_i = min_live[i] if (min_live and i < len(min_live)) else None
        scaled_i = min_scaled[i] if (min_scaled and i < len(min_scaled)) else None

        def _keep_ms_bit(r: Dict[str, Any]) -> bool:
            scal = parse_scalar_array(r.get("scalars"))
            if b_i not in scal:
                return False
            if raw_i is not None and int(scal[b_i]["raw"]) < int(raw_i):
                return False
            if live_i is not None and int(scal[b_i]["live"]) < int(live_i):
                return False
            if scaled_i is not None and int(scal[b_i]["scaled"]) < int(scaled_i):
                return False
            return True

        rows = run_step(
            f"min_scalar_bits(bit={b_i}) raw>={raw_i} live>={live_i} scaled>={scaled_i}",
            rows,
            _keep_ms_bit,
        )

    # if (min_raw is not None or min_live is not None or min_scaled is not None) and ms_bits:
    #     bits = [int(b) for b in ms_bits]

    #     min_raw_i = int(min_raw) if min_raw is not None else None
    #     min_live_i = int(min_live) if min_live is not None else None
    #     min_scaled_i = int(min_scaled) if min_scaled is not None else None

    #     def _keep_mins(r: Dict[str, Any]) -> bool:
    #         scal = parse_scalar_array(r.get("scalars"))
    #         for b in bits:
    #             if b not in scal:
    #                 return False
    #             if min_raw_i is not None and int(scal[b]["raw"]) < min_raw_i:
    #                 return False
    #             if min_live_i is not None and int(scal[b]["live"]) < min_live_i:
    #                 return False
    #             if min_scaled_i is not None and int(scal[b]["scaled"]) < min_scaled_i:
    #                 return False
    #         return True

    #     rows = run_step(
    #         f"min_scalar_thresholds(bits={bits}) raw>={min_raw_i} live>={min_live_i} scaled>={min_scaled_i}",
    #         rows,
    #         _keep_mins,
    #     )

    report: Dict[str, Any] = {
        "initial": len(daq_rows),
        "final": len(rows),
        "eff_total": (len(rows) / len(daq_rows)) if daq_rows else None,

        "mb_scaled_initial": _mb(daq_rows),
        "mb_scaled_final": _mb(rows),

        "events_initial": _ev(daq_rows),
        "events_final": _ev(rows),

        "steps": steps,
    }
    return rows, report

# -------------------------------------------------------------------------
# QA predicate parsing/eval: --requireQA "ohcal_auto <> BAD" ...
# -------------------------------------------------------------------------

_OPS = {"=", "==", "!=", "<>", "IN", "NOT IN", "IS NULL", "IS NOT NULL"}

def _get_subsystems_from_predicates(preds: List[str]) -> List[str]:
    subsystems: Set[str] = set()
    for raw in preds:
        expr = raw.strip()
        if not expr:
            continue

        upper = expr.upper()
        field = ""
        if " IS NOT NULL" in upper:
            field = expr[:upper.index(" IS NOT NULL")].strip()
        elif " IS NULL" in upper:
            field = expr[:upper.index(" IS NULL")].strip()
        elif " NOT IN " in upper:
            parts = re.split(r"\bNOT\s+IN\b", expr, flags=re.IGNORECASE, maxsplit=1)
            if len(parts) != 2:
                continue
            field = parts[0].strip()
        elif re.search(r"\bIN\b", upper):
            parts = re.split(r"\bIN\b", expr, flags=re.IGNORECASE, maxsplit=1)
            if len(parts) != 2:
                continue
            field = parts[0].strip()
        else:
            m = re.search(r"(==|!=|<>|=)", expr)
            if not m:
                continue
            field = expr[:m.start()].strip()

        if field.endswith("_status"):
            subsystem = field[: -len("_status")]
            subsystems.add(subsystem)
        elif field.endswith("_reason"):
            subsystem = field[: -len("_reason")]
            subsystems.add(subsystem)
        else:
            subsystems.add(field)

    return sorted(subsystems)

def _strip_outer_quotes(s: str) -> str:
    s = s.strip()
    if (len(s) >= 2) and ((s[0] == s[-1]) and s[0] in ("'", '"')):
        return s[1:-1]
    return s

def compile_qa_predicates(preds: List[str]) -> List[Callable[[Dict[str, Any]], bool]]:
    """
    Each predicate string is evaluated on a merged row dict.
    Field maps to '<field>_status' unless it already ends with _status/_reason/etc.
    """
    compiled: List[Callable[[Dict[str, Any]], bool]] = []

    for raw in preds:
        expr = raw.strip()
        if not expr:
            continue

        # Normalize multiword ops first
        upper = expr.upper()
        if " IS NOT NULL" in upper:
            field = expr[:upper.index(" IS NOT NULL")].strip()
            op = "IS NOT NULL"
            value = None
        elif " IS NULL" in upper:
            field = expr[:upper.index(" IS NULL")].strip()
            op = "IS NULL"
            value = None
        elif " NOT IN " in upper:
            parts = re.split(r"\bNOT\s+IN\b", expr, flags=re.IGNORECASE, maxsplit=1)
            if len(parts) != 2:
                raise ValueError(f"Bad predicate (NOT IN): {expr}")
            field = parts[0].strip()
            op = "NOT IN"
            value = parts[1].strip()
        elif re.search(r"\bIN\b", upper):
            parts = re.split(r"\bIN\b", expr, flags=re.IGNORECASE, maxsplit=1)
            if len(parts) != 2:
                raise ValueError(f"Bad predicate (IN): {expr}")
            field = parts[0].strip()
            op = "IN"
            value = parts[1].strip()
        else:
            # binary ops: =, ==, !=, <>
            m = re.search(r"(==|!=|<>|=)", expr)
            if not m:
                raise ValueError(f"Bad predicate (no operator): {expr}")
            op = m.group(1)
            field = expr[:m.start()].strip()
            value = expr[m.end():].strip()

        # map field -> status key unless user explicitly provided suffix
        key = field
        if not (key.endswith("_status") or key.endswith("_reason")):
            key = f"{key}_status"

        def _make_fn(k: str, oper: str, v: Optional[str]) -> Callable[[Dict[str, Any]], bool]:
            operU = oper.upper()

            if operU in ("IS NULL", "IS NOT NULL"):
                want_null = (operU == "IS NULL")
                def fn(row: Dict[str, Any]) -> bool:
                    present = (k in row)
                    val = row.get(k) if present else None
                    is_null = (val is None)
                    return is_null if want_null else (not is_null)
                return fn

            if operU in ("IN", "NOT IN"):
                if v is None:
                    raise ValueError("IN/NOT IN requires a value list")
                txt = v.strip()
                # allow "(A,B,C)" or "A,B,C"
                if txt.startswith("(") and txt.endswith(")"):
                    txt = txt[1:-1]
                choices = [c.strip() for c in txt.split(",") if c.strip()]
                norm_choices = set(_strip_outer_quotes(c).upper() for c in choices)

                def fn(row: Dict[str, Any]) -> bool:
                    val = row.get(k)
                    if val is None:
                        return False
                    ok = (str(val).upper() in norm_choices)
                    return ok if operU == "IN" else (not ok)
                return fn

            # binary compare against string token; treat unquoted tokens as literals (e.g. BAD)
            if v is None:
                raise ValueError("Binary op requires value")
            rhs = _strip_outer_quotes(v).upper()

            def fn(row: Dict[str, Any]) -> bool:
                val = row.get(k)
                if val is None:
                    return False
                lhs = str(val).upper()
                if oper in ("=", "=="):
                    return lhs == rhs
                if oper in ("!=", "<>"):
                    return lhs != rhs
                return False

            return fn

        compiled.append(_make_fn(key, op, value))

    return compiled

def apply_requireQA(rows: List[Dict[str, Any]], preds: List[str]) -> Tuple[List[Dict[str, Any]], Dict[str, Any]]:
    fns = compile_qa_predicates(preds)
    if not fns:
        return rows, {"initial": len(rows), "final": len(rows), "removed": 0}

    kept: List[Dict[str, Any]] = []
    removed: List[int] = []
    for r in rows:
        ok = True
        for fn in fns:
            if not fn(r):
                ok = False
                break
        if ok:
            kept.append(r)
        else:
            removed.append(r["runnumber"])

    rep = {"initial": len(rows), "final": len(kept), "removed": len(rows) - len(kept), "removed_runs": removed}
    return kept, rep

# -------------------------------------------------------------------------
# Main
# -------------------------------------------------------------------------

def main() -> int:
    ap = argparse.ArgumentParser()

    ap.add_argument("--dataset", required=True)
    ap.add_argument("--tag", required=True)
    ap.add_argument("--dsttype", required=True)

    ap.add_argument("--subsystems", nargs="+", default=[])

    ap.add_argument("--triggers", nargs="+", default=[])
    ap.add_argument("--min_bias_triggers",  nargs="+", default=[10,12,14], help="Minimum bias trigger bits (default: [10,12,14])")
    
    ap.add_argument("--runtype", nargs="+", default=["physics"], help="Allowed runtype(s) (default: [physics])")
    ap.add_argument("--min_run_length", type=int, default=300, help="Minimum run length in seconds (default 300)")
    
    ap.add_argument(
        "--livetime_threshold",
        nargs="+",
        default=[],
        help="Livetime thresholds as BIT:THRESHOLD (e.g. 10:0.9 for MB bit 10 with 90% livetime)",
    )
    ap.add_argument("--min_scalers",
        nargs="+",
        default=[],
        help="Minimum scalar thresholds as BIT:[SLR]:n (e.g. 10:S:100 for MB bit 10 with min scaled 100)",
    )

    ap.add_argument("--require_magnet_on", type=bool, default=True, help="Require magnet to be ON with sufficient current (default True). Current threshold is 4.5e7.")
    ap.add_argument("--min_reco_events", type=int, default=1_000_000, help="Minimum events in FileCatalog to include run")
  
    ap.add_argument("--first_run", type=int, default=None, help="First run to include (overrides dataset default)" )
    ap.add_argument("--last_run", type=int, default=None, help="Last run to include (overrides dataset default)" )

    # arbitrary QA predicates
    ap.add_argument(
        "--offline_status",
        nargs="+",
        default=[],
        help='QA predicates, e.g. "ohcal_auto <> BAD" "ihcal_auto = GOLDEN" "emcal IN (GOOD,GOLDEN)"',
    )

    # L0 cutflow reporting
    ap.add_argument(
        "--l0_report",
        choices=["none", "summary", "detailed"],
        default="summary",
        help="Print L0 cutflow (based on Python-applied L0 cuts).",
    )
    ap.add_argument("--prefix", default="./", help="Output prefix for cutflow report (default: ./)")
    

    args = ap.parse_args()

    rr = RunnumberRange()
    if args.dataset not in rr.datasets:
        print(f"Unknown dataset '{args.dataset}'; known: {list(rr.datasets.keys())}", file=sys.stderr)
        return 1

    first, last = rr.datasets[args.dataset]
    if args.first_run is not None:
        if args.first_run < first:
            print(f"Warning: --first_run {args.first_run} is less than dataset default {first}")
        first = args.first_run
    if args.last_run is not None:
        if args.last_run > last:
            print(f"Warning: --last_run {args.last_run} is greater than dataset default {last}")
        last = args.last_run

    # parse livetime thresholds
    lt_bit_args : Dict[str, Any] = {
        "livetime_bits": [],
        "min_livetime": [],
    }
    if args.livetime_threshold:
        for lt_arg in args.livetime_threshold:
            if ":" not in lt_arg:
                print(f"Bad --livetime_threshold arg (missing ':'): {lt_arg}", file=sys.stderr)
                return 1
            bit_s, thr_s = lt_arg.split(":", 1)
            try:
                bit = int(bit_s)
                thr = float(thr_s)
            except Exception as e:
                print(f"Bad --livetime_threshold arg (bad int/float): {lt_arg}: {e}", file=sys.stderr)
                return 1
            lt_bit_args["livetime_bits"].append(bit)
            lt_bit_args["min_livetime"].append(thr)
    else:
        lt_bit_args = {
            "livetime_bits": None,
            "min_livetime": None,
        }
    
    #parse min_scalers
    min_scaler_args: Dict[str, Any] = {
        "min_scalar_bits": [],
        "min_scaled": [],
        "min_live": [],
        "min_raw": [],
    }
    if args.min_scalers:
        for ms_arg in args.min_scalers:
            if ":" not in ms_arg:
                print(f"Bad --min_scalers arg (missing ':'): {ms_arg}", file=sys.stderr)
                return 1
            bit_s, slr, thr_s = ms_arg.split(":", 2)
            try:
                bit = int(bit_s)
                thr = int(thr_s)
            except Exception as e:
                print(f"Bad --min_scalers arg (bad int): {ms_arg}: {e}", file=sys.stderr)
                return 1
            min_scaler_args["min_scalar_bits"].append(bit)
            if slr == "S":
                min_scaler_args["min_scaled"].append(thr)
                min_scaler_args["min_live"].append(None)
                min_scaler_args["min_raw"].append(None)
            elif slr == "L":
                min_scaler_args["min_live"].append(thr)
                min_scaler_args["min_scaled"].append(None)
                min_scaler_args["min_raw"].append(None)
            elif slr == "R":
                min_scaler_args["min_raw"].append(thr)
                min_scaler_args["min_live"].append(None)
                min_scaler_args["min_scaled"].append(None)
            else:
                print(f"Bad --min_scalers arg (bad S/L/R): {ms_arg}", file=sys.stderr)
                return 1
   
    # parse trigger selections
    selected_triggers = [int(b) for b in args.triggers]
    # add min bias triggers to required triggers if not already present
    for mb in args.min_bias_triggers:
        if mb not in selected_triggers:
            selected_triggers.append(mb)
    # add any livetime or min scaler bits to required triggers if not already present
    for b in (lt_bit_args.get("livetime_bits") or []):
        if b not in selected_triggers:
            selected_triggers.append(b)
    for b in (min_scaler_args.get("min_scalar_bits") or []):
        if b not in selected_triggers:
            selected_triggers.append(b)

    l0args: Dict[str, Any] = {
        "first_run": first,
        "last_run": last,
        "min_run_length": args.min_run_length,
        "require_magnet_on": args.require_magnet_on,
        "runtypes": args.runtype,        

        "require_vgtms_all": True,

        "scalar_bits": selected_triggers,
        "scaledown_bits": selected_triggers,
        "require_scaledown_bits": selected_triggers,
        "min_bias_triggers": args.min_bias_triggers,

        "livetime_bits": lt_bit_args["livetime_bits"],
        "min_livetime": lt_bit_args["min_livetime"],
        
        "min_scalar_bits": min_scaler_args["min_scalar_bits"],
        "min_scaled": min_scaler_args["min_scaled"],
        "min_live": min_scaler_args["min_live"],
        "min_raw": min_scaler_args["min_raw"],
    }

    l0args["_store_rows_after"] = (args.l0_report == "detailed")
    l0args["_store_removed_runs"] = (args.l0_report == "detailed")

    subsystem_map = parse_subsystems(args.subsystems)
    qa_cols = list(subsystem_map["offlineQA"])
    extraQA = _get_subsystems_from_predicates(args.offline_status)
    for eqa in extraQA:
        if eqa not in qa_cols:
            qa_cols.append(eqa)

    # --- header (compact, informative)
    print_section("RunListGen")
    print(f"================================================")
    print(f"Dataset      : {args.dataset} (range {first}-{last})")
    print(f"Tag/DST      : {args.tag} / {args.dsttype}")
    print(f"Subsystems   : {', '.join(args.subsystems) if args.subsystems else '(none)'}")
    print(f"================================================")
    print(f"L0 Configuration:")
    print(f"\tRuntype in {args.runtype}")
    print(f"\tRequire VGTMs: {l0args.get('require_vgtms_all', False)} with requested VGTMs {subsystem_map['vgtms']}")
    print(f"\tRun length >= {args.min_run_length} sec")
    print(f"\tMagnet on with current >= 4.5e7: {args.require_magnet_on}")
    print(f"\tMin bias triggers: {args.min_bias_triggers}")
    if lt_bit_args["livetime_bits"]:
        print(f"\tLivetime bits: {lt_bit_args['livetime_bits']} with thresholds {lt_bit_args['min_livetime']}")
    if min_scaler_args["min_scalar_bits"]:
        print(f"\tMin scaler bits: {min_scaler_args['min_scalar_bits']} with thresholds (scaled: {min_scaler_args['min_scaled']}, live: {min_scaler_args['min_live']}, raw: {min_scaler_args['min_raw']})")
    print(f"\tReport level: {args.l0_report}")
    print(f"================================================")
    print(f"L1 Configuration:")    
    print(f"\tQA columns   : {', '.join(qa_cols) if qa_cols else '(none)'}")
    if args.offline_status:
        print(f"\tOffline QA status    : {len(args.offline_status)} predicates")
        for p in args.offline_status:
            print(f"\t\t{p}")
    print(f"\tTriggers     : {', '.join(str(b) for b in selected_triggers)} (including min bias {args.min_bias_triggers})")
    if args.min_reco_events:
        print(f"\tReco min events: {args.min_reco_events:,}")
    print(f"================================================")

    # --- FileCatalog
    fc_rows = query_rows(
        DB_FILEDB,
        FILECATALOG_SQL,
        {"dataset": args.dataset, "tag": args.tag, "dsttype": args.dsttype},
    )
    if not fc_rows:
        print("No FileCatalog rows returned.", file=sys.stderr)
        return 2
    events_by_run = {int(r["runnumber"]): int(r["events_sum"]) for r in fc_rows}

    # --- DAQ base rows (no L0 cuts in SQL)
    daq_base_rows = query_rows(
        DB_DAQ,
        DAQ_BASE_SQL,
        {
            "vgtms": subsystem_map["vgtms"],
            "scalar_bits": l0args.get("scalar_bits", []),
            "scaledown_bits": l0args.get("scaledown_bits", []),
            "first_run": l0args.get("first_run"),
            "last_run": l0args.get("last_run"),
        },
    )
    if not daq_base_rows:
        print("No DAQ rows returned.", file=sys.stderr)
        return 3

    
    # Apply L0 cuts in python (for cutflow + for actual selection)
    daq_rows, l0_report = apply_l0_cuts(
        daq_base_rows,
        l0args,
        subsystem_map["vgtms"],
        events_by_run=events_by_run,
    )

    # --- Production triage rows
    prod_sql = build_production_triage_query(qa_cols)
    prod_rows = query_rows(
        DB_PROD,
        prod_sql,
        {"first_run": l0args["first_run"], "last_run": l0args["last_run"]},
    )
    if not prod_rows:
        print("No Production triage rows returned.", file=sys.stderr)
        return 4

    # --- counts summary
    print_section("Inputs")
    print(f"FileCatalog  : {len(fc_rows):,} runs")
    print(f"DAQ (base)   : {len(daq_base_rows):,} runs")
    print(f"DAQ (L0 pass): {len(daq_rows):,} runs")
    # print(daq_rows)
    print(f"Production   : {len(prod_rows):,} runs")
   
    # --- L0 cutflow table (FC-matched events shown)
    if args.l0_report != "none":
        l0_lines: List[Dict[str, Any]] = []
        
        l0_lines.append({
            "stage": "DAQ base (runnumber range)",
            "runs": len(daq_base_rows),
            "events": events_sum_fc_only(daq_base_rows, events_by_run),
            "mb_scaled": [int(mb_scaled_sum(daq_base_rows, bit=b)) for b in l0args.get("min_bias_triggers", [])],
        })

        for st in l0_report["steps"]:
            if "rows_after" in st:
                ev = sum(events_by_run.get(rn, 0) for rn in st["rows_after"])
            else:
                # fallback: events_sum() already uses events_by_run, so this is still FC-matched
                ev = int(st.get("events_after", 0))

            l0_lines.append({
                "stage": st["cut"],
                "runs": int(st["after"]),
                "events": int(ev),
                # "mb_scaled": [int(st.get("mb_scaled_after", 0))] if st.get("mb_scaled_after") is not None else None,
                "mb_scaled": [int(mb_scaled_sum(daq_rows, bit=b)) for b in l0args.get("min_bias_triggers", [])],
            })

        print_cutflow_table(l0_lines, title="L0 Cutflow (DAQ cuts; FC-matched events shown)")

        if args.l0_report == "detailed":
            print_section("L0 cut failures (run lists)")
            for st in l0_report["steps"]:
                removed = st.get("removed_runs", [])
                if not removed:
                    continue
                n = len(removed)
                head_n = min(20, n)
                head = removed[:head_n]
                tail = removed[-head_n:] if n > 2 * head_n else []
                msg = f"- {st['cut']}: removed {n:,} runs; head={head}"
                if tail:
                    msg += f"; tail={tail}"
                print(msg)

    # # --- write L0 JSON
    # if args.l0_report != "none":
    #     try:
    #         with open(args.save_cutflow_steps, "w", encoding="utf-8") as f:
    #             json.dump(l0_report, f, indent=2)
    #         print_section("Artifacts")
    #         print(f"L0 cutflow JSON: {args.save_cutflow_steps}")
    #     except Exception as e:
    #         print(f"Failed to write L0 report JSON: {e}", file=sys.stderr)

    # --- merge on common runs
    fc_run_set = set(int(r["runnumber"]) for r in fc_rows)
    daq_run_set = set(int(r["runnumber"]) for r in daq_rows)
    prod_run_set = set(int(r["runnumber"]) for r in prod_rows)

    run_list = sorted(fc_run_set & daq_run_set & prod_run_set)

    fc_by = {int(r["runnumber"]): r for r in fc_rows}
    daq_by = {int(r["runnumber"]): r for r in daq_rows}
    prod_by = {int(r["runnumber"]): r for r in prod_rows}

    merged_rows: List[Dict[str, Any]] = [{**fc_by[r], **daq_by[r], **prod_by[r]} for r in run_list]

    print_section("Merged")
    print(f"Common runs  : {len(run_list):,} (FC ∩ DAQ(L0) ∩ PROD)")
    print(f"Merged rows  : {len(merged_rows):,}")

    # --- Post-L0 -> QA cutflow (start where events are meaningful)
    post_lines: List[Dict[str, Any]] = []
    post_lines.append(cutflow_line("Merged: FC ∩ DAQ(L0) ∩ PROD", merged_rows))

    filtered_rows = merged_rows

    # reco_min_events cut
    if args.min_reco_events and args.min_reco_events > 0:
        filtered_rows = [r for r in filtered_rows if int(r.get("events_sum", 0) or 0) >= int(args.min_reco_events)]
        post_lines.append(cutflow_line(f"min_reco_events >= {args.min_reco_events:,}", filtered_rows))

    # requireQA predicates
    if args.offline_status:
        filtered_rows, _rep = apply_requireQA(filtered_rows, args.offline_status)
        post_lines.append(cutflow_line("offline status predicates", filtered_rows))

    print_cutflow_table(post_lines, title="Cutflow (Merged -> reco_min_events -> QA)")

    # Totals after QA filtering
    total_events = int(sum(int(r.get("events_sum", 0) or 0) for r in filtered_rows))
    # total_mb_scaled = mb_scaled_sum(filtered_rows, bit=10)
    total_mb_scaled = [int(mb_scaled_sum(filtered_rows, bit=b)) for b in l0args.get("min_bias_triggers", [])]
    print_section("Final Totals")
    print(f"Runs         : {len(filtered_rows):,}")
    print(f"Events       : {total_events:,}")
    print(f"MB_scaled    : "+ ", ".join(f"bit {b}: {mb}" for b, mb in zip(l0args.get("min_bias_triggers", []), total_mb_scaled)))

    # --- outputs
    grl_file = f"{args.prefix}{args.dataset}_{args.tag}_{args.dsttype.lower()}_grl.list"
    with open(grl_file, "w", encoding="utf-8") as gf:
        for r in filtered_rows:
            gf.write(f"{int(r['runnumber'])}\n")

    run_detail_file = f"{args.prefix}{args.dataset}_{args.tag}_{args.dsttype.lower()}_run_details.json"
    with open(run_detail_file, "w", encoding="utf-8") as rdf:
        json.dump(filtered_rows, rdf, indent=2)

    cut_flow_report_file = f"{args.prefix}{args.dataset}_{args.tag}_{args.dsttype.lower()}_cutflow_report.json"
    try:
        with open(cut_flow_report_file, "w", encoding="utf-8") as cff:
            json.dump({"l0_cutflow": l0_report, "post_l0_cutflow": post_lines}, cff, indent=2)
        print_section("Artifacts")
        print(f"Cutflow report JSON: {cut_flow_report_file}")
    except Exception as e:
        print(f"Failed to write cutflow report JSON: {e}", file=sys.stderr)
    


    print_section("Outputs")
    print(f"GRL          : {grl_file}")
    print(f"Run details  : {run_detail_file}")

    return 0



if __name__ == "__main__":
    raise SystemExit(main())
