#!/usr/bin/env python3
# CDB status per run and domain for the CDBCaloQA map.
#   run     : entry of the tag for this run only
#   range   : entry of the tag shared by a range of runs
#   all     : single entry of the tag valid for all runs
#   default : no entry for the run, fallback to <domain>_default
#   missing : domain used by other runs of this period but not by this run, or looked up by the job with no file in the tag
# a file changed since the previous QA snapshot of the run is listed in <out status>_changes.txt
# usage: cdbstatus.py <runfiles.txt> <gt.json> <snapshot dir> <out status.txt> [job log pattern with {run}]
# output line: <run> <domain> <status> <first run of the entry> <end run of the entry, exclusive> <inserted>
import json
import os
import re
import sys

import ROOT

MAXIOV = 9223372036854775807


def load_tag(fname):
    tag = {}
    for p in json.load(open(fname))["payload_lists"]:
        tag[p["payload_type"]] = [(i["minor_iov"], i["minor_iov_end"] if i["minor_iov_end"] is not None else MAXIOV,
                                   i["payload_url"], (i.get("inserted") or "-")[:16]) for i in p["payload_iov"]]
    return tag


def lookup(tag, domain, run):
    iovs = [i for i in tag.get(domain, []) if i[0] <= run < i[1]]
    return max(iovs) if iovs else None


def tag_entry(tag, b, r):
    # what the tag gives run r for payload b: (payload name used, status, first run, end run, url, inserted)
    for d in (b, b + "_default"):
        iov = lookup(tag, d, r)
        if iov:
            hi = "inf" if iov[1] == MAXIOV else iov[1]
            if d.endswith("_default"):
                status = "default"
            elif iov[0] == 0 and iov[1] == MAXIOV:
                status = "all"
            elif iov[1] - iov[0] == 1:
                status = "run"
            else:
                status = "range"
            return d, status, iov[0], hi, iov[2], iov[3]
    return None, "missing", "-", "-", "", "-"


def md5(url):
    # CDB file names start with the md5 checksum of the file: <md5>_<name>.root
    # (a few payloads, e.g. EMCPROFILE, have no md5 prefix: the whole file name is used)
    fn = os.path.basename(url)
    h = fn.split("_", 1)[0]
    return h if len(h) == 32 and all(c in "0123456789abcdef" for c in h) else fn


def used_files(fname):
    f = ROOT.TFile.Open(fname)
    out = {}
    for k in f.GetListOfKeys():
        if k.GetName().startswith("cdb_"):
            out[k.GetName()[4:]] = k.ReadObj().GetTitle()
    f.Close()
    return out


def notfound(logpattern, runs):
    # domains the job looked up but the tag has no file for (CDBInterface prints "no file found for <domain>")
    out = set()
    for r, _ in runs:
        log = logpattern.format(run=r)
        if os.path.exists(log):
            out |= set(re.findall(r"no file found for (\S+) or ", open(log, errors="replace").read()))
    return out


def write_tagonly(out, changes, tag, tagonly, r, prev, snap):
    # payloads shown from the tag only; their files are added to the run snapshot
    for b in tagonly:
        d, status, lo, hi, url, ins = tag_entry(tag, b, r)
        if d:
            if prev.get(d) and md5(prev[d]) != md5(url):
                changes.write(f"{r} {b} {os.path.basename(prev[d])} {os.path.basename(url)}\n")
            snap[d] = url
        out.write(f"{r} {b} {status} {lo} {hi} {ins}\n")


def main():
    runfiles, gtjson, snapdir, outname = sys.argv[1:5]
    tag = load_tag(gtjson)
    runs = [(int(l.split()[0]), l.split()[1]) for l in open(runfiles) if l.strip()]
    # runs without output (job failed): no files used, the payloads are checked in the tag below
    used = {r: (used_files(f) if f != "-" else None) for r, f in runs}
    base = lambda d: d[:-len("_default")] if d.endswith("_default") else d
    alldomains = {base(d) for u in used.values() if u for d in u}
    if len(sys.argv) > 5:
        alldomains |= notfound(sys.argv[5], runs)
    alldomains = sorted(alldomains)
    # every other payload of the tag is shown from the tag (the pages hide the ones in notes/cdb_hide.txt)
    tagonly = sorted({base(d) for d in tag} - set(alldomains))
    os.makedirs(snapdir, exist_ok=True)
    # payloads whose file changed since the previous QA pass: <run> <payload> <old file> <new file>
    changes = open(outname.replace(".txt", "_changes.txt"), "w")
    with open(outname, "w") as out:
        for r, _ in runs:
            snapfile = os.path.join(snapdir, f"cdb_run{r}.json")
            if used[r] is None:
                # no output: what the tag gives this run for each payload of the period
                tagged = {}
                for b in alldomains:
                    for d in (b, b + "_default"):
                        iov = lookup(tag, d, r)
                        if iov:
                            tagged[d] = iov[2]
                            break
                for b in alldomains:
                    lo, hi, ins = "-", "-", "-"
                    d = b if b in tagged else (b + "_default" if b + "_default" in tagged else None)
                    if d is None:
                        status = "missing"
                    else:
                        iov = lookup(tag, d, r)
                        lo, hi, ins = iov[0], ("inf" if iov[1] == MAXIOV else iov[1]), iov[3]
                        if d.endswith("_default"):
                            status = "default"
                        elif iov[0] == 0 and iov[1] == MAXIOV:
                            status = "all"
                        elif iov[1] - iov[0] == 1:
                            status = "run"
                        else:
                            status = "range"
                    out.write(f"{r} {b} {status} {lo} {hi} {ins}\n")
                prev = json.load(open(snapfile)) if os.path.exists(snapfile) else {}
                write_tagonly(out, changes, tag, tagonly, r, prev, tagged)
                json.dump(tagged, open(snapfile, "w"), indent=1)
                continue
            prev = json.load(open(snapfile)) if os.path.exists(snapfile) else {}
            byb = {base(d): d for d in used[r]}
            for b in alldomains:
                lo, hi, ins = "-", "-", "-"
                if b not in byb:
                    status = "missing"
                else:
                    d = byb[b]
                    url = used[r][d]
                    iov = lookup(tag, d, r)
                    if iov:
                        lo, hi, ins = iov[0], ("inf" if iov[1] == MAXIOV else iov[1]), iov[3]
                    # compare md5 checksums with the previous QA pass of this run
                    if prev.get(d) and md5(prev[d]) != md5(url):
                        changes.write(f"{r} {b} {os.path.basename(prev[d])} {os.path.basename(url)}\n")
                    if d.endswith("_default"):
                        status = "default"
                    elif iov and iov[0] == 0 and iov[1] == MAXIOV:
                        status = "all"
                    elif iov and iov[1] - iov[0] == 1:
                        status = "run"
                    else:
                        status = "range"
                out.write(f"{r} {b} {status} {lo} {hi} {ins}\n")
            write_tagonly(out, changes, tag, tagonly, r, prev, used[r])
            json.dump(used[r], open(snapfile, "w"), indent=1)
    changes.close()
    print(f"wrote {outname}: {len(runs)} runs, {len(alldomains)} domains + {len(tagonly)} from the tag only")


if __name__ == "__main__":
    main()
