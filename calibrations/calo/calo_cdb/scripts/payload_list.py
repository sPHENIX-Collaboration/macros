# payload list of one global tag for the runs of a period, for the live tag comparison on compare.html
# per payload type: md5 (first 6 hex) of the file valid for each run, and its insert time (index into "dates", 3 base-36 digits)
# usage: payload_list.py <global tag json> <run list> <out json>
import heapq
import json
import sys

gtjson, runlist, out = sys.argv[1:4]
runs = sorted(int(x) for x in open(runlist).read().split())
B36 = "0123456789abcdefghijklmnopqrstuvwxyz"
dates, didx, types = [], {}, {}
MAXIOV = 9223372036854775807
for pl in json.load(open(gtjson))["payload_lists"]:
    ivs = sorted((p["minor_iov"], p["minor_iov_end"] if p["minor_iov_end"] is not None else MAXIOV,
                  p["checksum"][:6], (p.get("inserted") or "-")[:16].replace("T", " "))
                 for p in pl["payload_iov"])
    # sweep over the sorted runs: the valid file is the one with the latest start <= run whose IOV end is after the run
    h, t, act, k, found = [], [], [], 0, False
    for r in runs:
        while k < len(ivs) and ivs[k][0] <= r:
            heapq.heappush(act, (-ivs[k][0], k))
            k += 1
        while act and ivs[act[0][1]][1] <= r:
            heapq.heappop(act)
        if not act:
            h.append("------")
            t.append("---")
            continue
        found = True
        _, _, md5, ins = ivs[act[0][1]]
        if ins not in didx:
            didx[ins] = len(dates)
            dates.append(ins)
        i = didx[ins]
        h.append(md5)
        t.append(B36[i // 1296] + B36[i // 36 % 36] + B36[i % 36])
    if found:
        types[pl["payload_type"]] = {"h": "".join(h), "t": "".join(t)}
json.dump({"runs": runs, "dates": dates, "types": types}, open(out, "w"), separators=(",", ":"))
print(f"payloads {out}: {len(types)} payload types, {len(runs)} runs")
