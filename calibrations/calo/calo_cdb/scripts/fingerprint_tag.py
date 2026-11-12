#!/usr/bin/env python3
# Fingerprint of the plotted objects of one global tag of one period: one short hash per plot and run.
# Two tags are compared live in the browser (compare.html of the period) by comparing their fingerprints,
# so no comparison has to be made in advance for any pair of tags.
# Reads <web>/<period>/<tag>/plots/runs/run<N>.root and plots/summary/summary.root (written by drawCDBCaloQA.C).
# usage: fingerprint_tag.py <webdir> <period> <tag>
# output: <web>/<period>/<tag>/plots/fingerprint.json
import hashlib
import json
import os
import sys
from multiprocessing import get_context

import numpy as np
import ROOT

ROOT.gROOT.SetBatch(True)


def values(o):
    # the plotted numbers of one object: bin contents, graph points (x and y), or fit parameters
    if o.InheritsFrom("TH2Poly"):
        return np.array([o.GetBinContent(i) for i in range(1, o.GetNumberOfBins() + 1)], dtype=float)
    if o.InheritsFrom("TProfile"):
        return np.array([o.GetBinContent(i) for i in range(1, o.GetNbinsX() + 1)])
    if o.InheritsFrom("TH1"):
        a = o.GetArray()
        a.reshape((o.GetNcells(),))
        arr = np.array(a).astype(float)
        return arr if o.GetDimension() > 1 else arr[1:o.GetNbinsX() + 1]
    if o.InheritsFrom("TGraph"):
        n = o.GetN()
        x, y = o.GetX(), o.GetY()
        x.reshape((n,))
        y.reshape((n,))
        return np.concatenate([np.array(x, dtype=float), np.array(y, dtype=float)])
    if o.InheritsFrom("TF1"):
        return np.array([o.GetParameter(i) for i in range(o.GetNpar())])
    return np.array([])


def fingerprint(f):
    # plot key -> hash of all its objects; values rounded to single precision (relative ~1e-7),
    # so the same plot from the same inputs gives the same hash
    out = {}
    for k in f.GetListOfKeys():
        n = k.GetName()
        if not n.endswith("_meta"):
            continue
        key = n[:-5]
        h = hashlib.md5()
        for i in range(len(json.loads(f.Get(n).GetTitle())["opts"])):
            o = f.Get(f"{key}_{i}")
            h.update(values(o).astype(np.float32).tobytes() if o else b"missing")
        out[key] = h.hexdigest()[:10]
    return out


def one_run(path):
    f = ROOT.TFile.Open(path)
    fp = fingerprint(f)
    f.Close()
    return int(os.path.basename(path)[3:-5]), fp


def main():
    webdir, period, tag = sys.argv[1:4]
    d = os.path.join(webdir, period, tag, "plots")
    files = sorted(os.path.join(d, "runs", f) for f in os.listdir(os.path.join(d, "runs"))
                   if f.startswith("run") and f.endswith(".root"))
    with get_context("spawn").Pool(4) as pool:
        runs = dict(pool.map(one_run, files, chunksize=20))
    out = {"period": period, "tag": tag, "runs": {str(r): runs[r] for r in sorted(runs)}, "summary": {}}
    s = os.path.join(d, "summary", "summary.root")
    if os.path.exists(s):
        f = ROOT.TFile.Open(s)
        out["summary"] = fingerprint(f)
        f.Close()
    path = os.path.join(d, "fingerprint.json")
    json.dump(out, open(path, "w"), separators=(",", ":"))
    print(f"fingerprint {period} {tag}: {len(runs)} runs, {len(out['summary'])} summary plots -> {path}")


if __name__ == "__main__":
    main()
