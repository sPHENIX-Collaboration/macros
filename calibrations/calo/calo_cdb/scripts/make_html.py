#!/usr/bin/env python3
# Static HTML pages of CDBCaloQA for one running period. Plots are the PDFs of drawCDBCaloQA.C.
# usage: make_html.py <webdir> <period> <period title> <runfiles.txt> <cdbstatus.txt> <snapshot dir> <global tag>
# pages: <webdir>/index.html (periods) -> <period>/index.html (global tags) -> <period>/<tag>/ (runs, CDB, summaries)
import datetime
import fnmatch
import html
import json
import os
import sys
import urllib.request

STYLE = """
.savebar { font-size: 0.8em; color: #555; margin: 2px 0 0 4px; } .savebar a { margin-left: 8px; }
img.logo { height: 56px; vertical-align: middle; margin-right: 14px; }
body { font-family: Arial, sans-serif; margin: 20px; color: #222; }
h1 { color: #1a3e6e; margin-bottom: 4px; }
h2 { color: #1a3e6e; border-bottom: 2px solid #1a3e6e; padding-bottom: 3px; margin-top: 28px; }
.note { color: #555; font-size: 0.9em; }
.nav { margin: 10px 0; font-size: 1.1em; }
.nav a { color: #1a56a0; text-decoration: none; margin-right: 18px; }
.nav a:hover { text-decoration: underline; }
.toc a { display: inline-block; color: #1a56a0; text-decoration: none; margin-right: 14px; font-weight: bold; }
.grid a { display: inline-block; border: 1px solid #2e7d32; color: #1a56a0; padding: 3px 7px; margin: 2px; text-decoration: none; font-family: monospace; font-size: 1.05em; border-radius: 3px; }
.grid a:hover { background: #e8f5e9; }
.grid a.default { border-color: #ef6c00; background: #ffb74d; }
.grid a.missing { border-color: #c62828; background: #e57373; }
.grid a.updated { border-color: #1565c0; background: #90caf9; }
.plots { display: grid; grid-template-columns: repeat(auto-fill, minmax(min(380px, 100%), 1fr)); gap: 14px; }
.plot { text-align: center; min-width: 0; }
.plot p { margin: 4px 0; font-weight: bold; color: #333; }
.plot a.pdf { display: block; }
.plot canvas { width: 100%; height: auto; border: 1px solid #ccc; display: block; }
.plot.wide { grid-column: 1 / -1; }
.plot.wide canvas { width: auto; max-width: 100%; margin: 0 auto; }
table.cdb { border-collapse: collapse; font-size: 0.85em; }
table.cdb td, table.cdb th { border: 1px solid #ccc; padding: 2px 8px; text-align: left; }
table.cdb th { background: #1a3e6e; color: white; }
td.run { background: #81c784; } td.range { background: #ffe082; } td.all { background: #e0e0e0; } td.default { background: #ffb74d; }
td.missing { background: #e57373; } td.updated { background: #90caf9; }
.legend span { display: inline-block; padding: 2px 8px; margin-right: 8px; border-radius: 3px; }
.cmp { font-size: 0.8em; margin: 2px 0 4px; }
.cmp a.pdflink { margin-left: 10px; }
.frame .main { position: relative; }
.jsdraw { width: 100%; height: 340px; }
.jsratio { width: 100%; height: 170px; }
/* comparison legend inside the upper plot frame, top right: JSROOT pad margins are 10% of the pad */
.cleg { position: absolute; top: calc(10% + 12px); right: calc(10% + 12px); font-size: 0.8em; background: rgba(255, 255, 255, 0.85);
        padding: 2px 6px; pointer-events: none; text-align: left; }
.cleg div { white-space: nowrap; }
.ptext { position: absolute; top: calc(10% + 8px); left: calc(15% + 12px); font-size: 0.8em; background: rgba(255, 255, 255, 0.85);
         padding: 1px 4px; pointer-events: none; }
.upd { box-shadow: inset 0 0 0 100px rgba(30, 136, 229, 0.40); outline: 2px solid #1565c0; outline-offset: -2px; }
.maplab a { color: #000; text-decoration: none; padding: 0 3px; }
.maplab a:hover { text-decoration: underline; }
.grid a.upd { border-color: #1565c0; }
.grid a.empty { border-color: #000; background: #616161; color: #fff; }
.flag { color: #c62828; font-weight: bold; }
.qanote { background: #fff8e1; border-left: 4px solid #ffb300; padding: 4px 8px; }
"""

STATUS = [("run", "#81c784", "run: entry for this run only"),
          ("range", "#ffe082", "range: entry shared by a run range"),
          ("all", "#e0e0e0", "all: one entry for all runs"),
          ("default", "#ffb74d", "default: no entry for this run, fallback to _default"),
          ("missing", "#e57373", "missing"),
          ("updated", "#90caf9", "updated since last QA")]
# a change since the last QA pass is drawn as a see-through blue layer on top of the status colour
LEGEND = ('<div class="legend">' + "".join(f'<span style="background:{c}">{t}</span>' for s, c, t in STATUS if s != "updated")
          + '<span class="upd" style="background:#fff">updated since last QA (see-through blue on the status colour)</span></div>')

LOGO = '<img class="logo" src="/WWW/subsystem/CDBCaloQA/sphenix-logo-white-bg.png" alt="sPHENIX">'

RUN_SECTIONS = [
    ("MBD", [("mbd_q_selpmt", "Charge, selected PMTs, MB"), ("mbd_q_allpmt", "Charge, all PMTs, MB"),
             ("mbd_t_selpmt", "Time, selected PMTs, MB"), ("mbd_t_allpmt", "Time, all PMTs, q > 0, MB"),
             ("mbd_qavg_ns", "Average charge, south / north, MB"), ("mbd_npmt_ns", "N PMT q > 0.4, south / north, MB"),
             ("mbd_zvtx", "z-vertex, MB"), ("mbd_t0", "t0, MB")]),
    ("ZDC", [("zdc_e_ns", "Energy, south / north, ZDC trigger"), ("zdc_mod_south", "Compartment energy, south, ZDC trigger"),
             ("zdc_mod_north", "Compartment energy, north, ZDC trigger"), ("zdc_e_2d", "North vs south energy, MB")]),
    ("sEPD", [("sepd_q_seltile", "Charge, selected tiles, MB"), ("sepd_q_alltile", "Charge, all tiles, MB"),
              ("sepd_mip_map", "MIP peak per tile, MB"), ("sepd_qmean_map", "Mean charge per event per tile, MB"),
              ("sepd_hit_map", "Fraction of events with q &gt; 0.5 nMIP per tile, MB"),
              ("sepd_nhit_ns", "Tiles with q &gt; 0.5 nMIP per event, south / north, MB"),
              ("sepd_qsum_ns", "Charge sum per event, south / north, MB"), ("sepd_qsum_2d", "North vs south charge sum, MB")]),
] + [(ll, [(f"{l}_e", "Tower energy, with / without ZSCrossCalib, MB"), (f"{l}_zsxcalib_ieta", "ZSCrossCalib factor vs i&eta;, CDB file"),
           (f"{l}_occ", "Occupancy, MB"), (f"{l}_nonzs_frac", "Non-ZS fraction, MB"), (f"{l}_time", "Tower time, MB"),
           (f"{l}_time_mbdt0", "Tower time - MBD t0, MB"), (f"{l}_noise", "Noise + fit, MB"),
           (f"{l}_noise_ieta", "Noise vs i&eta;, MB")])
     for l, ll in (("cemc", "EMCal"), ("ihcal", "IHCal"), ("ohcal", "OHCal"))] + [
    ("Clusters, pi0, jets", [("clus_e", "EMCal cluster energy, per trigger"), ("clus_eta", "EMCal cluster &eta;, per trigger"),
                             ("pi0_mass", "&pi;<sup>0</sup> diphoton mass, MB"), ("jet_pt", "Jet p<sub>T</sub>, per trigger"),
                             ("jet_eta", "Jet &eta; above the p<sub>T</sub> cut, per trigger")]),
]

SUMMARY = [("MBD", [("mbd_qmean", "Mean PMT charge, MB"), ("mbd_tmean", "Mean PMT time, MB"), ("mbd_mip", "MIP peak, MB"),
                    ("mbd_t0", "t0, MB"), ("mbd_zvtx", "z-vertex, MB"), ("mbd_zvtx_rms", "z-vertex RMS, MB"),
                    ("mbd_qsum", "Mean charge sum, MB"), ("mbd_qsum_ns", "Mean charge sum, south / north, MB")]),
           ("ZDC", [("zdc_1n_ns", "One-neutron peak, south / north, ZDC trigger"), ("zdc_emean_ns", "Mean energy, south / north, ZDC trigger"),
                    ("zdc_mbmean_ns", "Mean energy above 12 GeV, south / north, MB"),
                    ("zdc_g_ns", "Main-bunch mean energy in the period's OHCal 30&ndash;70% band / median of the runs, south / north, MB |z| &lt; 10 cm"),
                    ("zdc_fm5_ns", "-5 ns bunch / main bunch, south / north, MB"),
                    ("zdc_fp5_ns", "+5 ns bunch / main bunch, south / north, MB"),
                    ("zdc_t0prof", "&lt;ZDC&gt; / &lt;ZDC main bunch&gt; vs MBD t0 - main peak, all runs, MB"),
                    ("zdc_bunchspec_south", "South spectra of the three bunches, all runs, MB"),
                    ("zdc_bunchspec_north", "North spectra of the three bunches, all runs, MB"),
                    ("zdc_periph", "Peripheral spectrum (OHCal &lt; 2 GeV), all runs, MB |z| &lt; 10 cm")]),
           ("sEPD", [("sepd_mip_ns", "Mean tile MIP peak, south / north, MB"),
                     ("sepd_qsum_mean_ns", "Mean charge sum, south / north, MB"),
                     ("sepd_nhit_mean_ns", "Mean tiles with q &gt; 0.5 nMIP, south / north, MB")])] + [
    ("Centrality", [("cent_frac_ref", "Fraction of MB events in 0&ndash;5% and 75&ndash;80%, relative to the sum of all runs")]),
] + [
    (ll, [(f"{l}_occ", "Mean occupancy, MB"), (f"{l}_time", "Mean tower time, MB"), (f"{l}_towsum", "Mean tower energy sum, MB"),
          (f"{l}_noise", "Noise, MB"), (f"{l}_zsxcalib_map", "Mean ZSCrossCalib factor per i&eta; vs run, CDB files"),
          (f"{l}_tsc_e", "Tower mean energy in the TSC fit window / i&eta; ring mean, vs i&phi;, all runs, MB"),
          (f"{l}_tsc_occ", "Tower fraction of events above the TSC fit window / i&eta; ring mean, vs i&phi;, all runs, MB")]) for l, ll in (("cemc", "EMCal"), ("ihcal", "IHCal"), ("ohcal", "OHCal"))]

RANK = {"run": 0, "range": 0, "all": 0, "updated": 1, "default": 2, "missing": 3}


def plot(src, title, wide=False, objfile=None, tags=()):
    # the PDF is drawn into a canvas by pdf.js, scaled to the width of its grid cell; click opens the PDF
    # objfile + tags: menu to overlay the same plot of another global tag (drawn live by JSROOT from objfile)
    ctl, hide = "", ""
    if objfile:
        key = os.path.basename(src)[:-4]
        key = key.split("_", 1)[1] if key.startswith("run") and key[3:4].isdigit() else key
        key = key[:-7] if key.endswith("_vs_run") else key
        # 2D maps: no colour, the comparison is the ratio map
        cols = ('' if key.endswith("_map") else ' colour <select class="col"><option>red</option><option>blue</option>'
                '<option>green</option><option>magenta</option></select>')
        menu = ('compare with <select class="tag"><option value="">none</option>'
                + "".join(f"<option>{html.escape(t)}</option>" for t in tags) + '</select>' + cols) if tags else ''
        ctl = (f'<div class="cmp" data-key="{key}" data-file="{objfile}">{menu}<a class="pdflink" href="{src}">PDF</a></div>'
               '<div class="frame"><div class="main"><div class="jsdraw"></div>'
               '<div class="cleg"></div></div><div class="jsratio"></div></div>')
        hide = ' style="display:none"'  # shown only when the objects are not in the ROOT file
    return (f'<div class="plot{" wide" if wide else ""}"><p>{title}</p>{ctl}'
            f'<a class="pdf" href="{src}"{hide}><canvas data-pdf="{src}"></canvas></a></div>')


PDFJS = """
<script src="https://cdnjs.cloudflare.com/ajax/libs/pdf.js/3.11.174/pdf.min.js"></script>
<script>
pdfjsLib.GlobalWorkerOptions.workerSrc = 'https://cdnjs.cloudflare.com/ajax/libs/pdf.js/3.11.174/pdf.worker.min.js';
function drawPdf(cv) {
  pdfjsLib.getDocument(cv.dataset.pdf).promise.then(doc => doc.getPage(1)).then(pg => {
    const w = cv.parentElement.clientWidth || 500;
    const v1 = pg.getViewport({scale: 1});
    const scale = Math.min(w / v1.width, cv.closest('.wide') ? 1.6 : 4) * (window.devicePixelRatio || 1);
    const v = pg.getViewport({scale: scale});
    cv.width = v.width; cv.height = v.height;
    pg.render({canvasContext: cv.getContext('2d'), viewport: v});
  });
}
const obs = new IntersectionObserver(es => es.forEach(e => { if (e.isIntersecting) { drawPdf(e.target); obs.unobserve(e.target); } }), {rootMargin: '400px'});
document.querySelectorAll('canvas[data-pdf]').forEach(c => obs.observe(c));
</script>
"""


STCODE = {s: i for i, (s, _, _) in enumerate(STATUS)}

CDBMAP_JS = """
<script>
(function () {
  const D = JSON.parse(document.getElementById('cdbdata').textContent);
  const cv = document.getElementById('cdbmap_cv'), tip = document.getElementById('cdbmap_tip');
  const lo = document.getElementById('cdbmap_lo'), hi = document.getElementById('cdbmap_hi');
  const ROWH = 18;
  let i0 = 0, i1 = D.runs.length;  // shown run index range [i0, i1)
  function draw() {
    const w = cv.parentElement.clientWidth, dpr = window.devicePixelRatio || 1;
    const h = D.pays.length * ROWH;
    cv.style.width = w + 'px'; cv.style.height = h + 'px';
    cv.width = w * dpr; cv.height = h * dpr;
    const c = cv.getContext('2d'); c.setTransform(dpr, 0, 0, dpr, 0, 0);
    const n = i1 - i0, cw = w / n;
    D.pays.forEach((p, j) => {
      for (let i = i0; i < i1; i++) {
        c.fillStyle = D.col[D.st[j][i]];
        c.fillRect((i - i0) * cw, j * ROWH, Math.max(cw, 1) + 0.5, ROWH - 2);
      }
      for (const k in D.chg[j]) {
        const i = +k;
        if (i < i0 || i >= i1) continue;
        c.fillStyle = 'rgba(30, 136, 229, 0.45)';
        c.fillRect((i - i0) * cw, j * ROWH, Math.max(cw, 3) + 0.5, ROWH - 2);
        c.strokeStyle = '#1565c0';
        c.strokeRect((i - i0) * cw + 0.5, j * ROWH + 0.5, Math.max(cw, 3) - 0.5, ROWH - 3);
      }
    });
    document.getElementById('cdbmap_x').innerHTML =
      '<span>' + D.runs[i0] + '</span><span>' + n + ' runs</span><span>' + D.runs[i1 - 1] + '</span>';
  }
  function cell(e) {
    const r = cv.getBoundingClientRect();
    const i = i0 + Math.floor((e.clientX - r.left) / r.width * (i1 - i0));
    const j = Math.floor((e.clientY - r.top) / ROWH);
    return (i >= i0 && i < i1 && j >= 0 && j < D.pays.length) ? [i, j] : null;
  }
  cv.addEventListener('mousemove', e => {
    const ij = cell(e);
    if (!ij) { tip.style.display = 'none'; return; }
    const [i, j] = ij;
    tip.innerHTML = '<b>Run ' + D.runs[i] + '</b><br>' + D.pays[j] + '<br>status: <span style="background:' + D.col[D.st[j][i]] + ';padding:0 8px;border-radius:3px">' + D.name[D.st[j][i]] + '</span>' +
      '<br>runs of the entry: ' + (D.iov[j][D.ii[j][i]] || '-') + '<br>inserted: ' + (D.ins[j][D.ni[j][i]] || '-') +
      (D.chg[j][i] !== undefined ? '<br><span class="upd" style="padding:0 6px">updated since last QA</span><br>file:<br>' + D.chg[j][i] +
       '<br>--&gt; ' + (D.files[j][D.fi[j][i]] || '-') : '<br>file: ' + (D.files[j][D.fi[j][i]] || '-'));
    tip.style.display = 'block';
    tip.style.left = (e.pageX + 14) + 'px'; tip.style.top = (e.pageY + 14) + 'px';
  });
  cv.addEventListener('mouseleave', () => tip.style.display = 'none');
  cv.addEventListener('click', e => { const ij = cell(e); if (ij) window.location = 'run_' + D.runs[ij[0]] + '.html'; });
  function setRange() {
    const a = parseInt(lo.value), b = parseInt(hi.value);
    let s = D.runs.findIndex(r => r >= (isNaN(a) ? 0 : a));
    let t = D.runs.length; while (t > 0 && D.runs[t - 1] > (isNaN(b) ? Infinity : b)) t--;
    if (s < 0 || s >= t) { s = 0; t = D.runs.length; }
    i0 = s; i1 = t; draw();
  }
  document.getElementById('cdbmap_go').addEventListener('click', setRange);
  document.getElementById('cdbmap_all').addEventListener('click', () => { lo.value = ''; hi.value = ''; setRange(); });
  [lo, hi].forEach(x => x.addEventListener('keydown', e => { if (e.key === 'Enter') setRange(); }));
  window.addEventListener('resize', draw);
  draw();
})();
</script>
"""


def cdb_map(runs, pays, status, iovs, ins, changed, cellfile):
    # interactive map: payload rows x run columns, hover shows the entry, click opens the run page
    data = {"runs": runs, "pays": pays, "col": [c for _, c, _ in STATUS], "name": [s for s, _, _ in STATUS],
            "st": [], "files": [], "fi": [], "iov": [], "ii": [], "ins": [], "ni": [], "chg": []}
    for d in pays:
        files, iovl, insl = [""], [""], [""]
        st, fi, ii, ni, chg = [], [], [], [], {}
        for r in runs:
            st.append(STCODE.get(status.get(r, {}).get(d, "missing"), STCODE["missing"]))
            f = cellfile.get((r, d), "")
            if f not in files:
                files.append(f)
            fi.append(files.index(f))
            v = html.unescape(iovs.get((r, d), ""))
            if v not in iovl:
                iovl.append(v)
            ii.append(iovl.index(v))
            t = ins.get((r, d), "")
            if t not in insl:
                insl.append(t)
            ni.append(insl.index(t))
            if (r, d) in changed:
                chg[len(st) - 1] = changed[(r, d)]
        data["st"].append(st)
        data["files"].append(files)
        data["fi"].append(fi)
        data["iov"].append(iovl)
        data["ii"].append(ii)
        data["ins"].append(insl)
        data["ni"].append(ni)
        data["chg"].append(chg)
    # payload name boxed in see-through blue when any run of it changed since the last QA pass
    labels = "".join(f'<div style="height:18px;line-height:16px"><a href="payload_{d}.html"'
                     f'{" class=upd" if any(k[1] == d for k in changed) else ""}>{d}</a></div>' for d in pays)
    return (f'{LEGEND}<p class="note">Hover over a cell for the run, status and file. Click a cell to open the run page, '
            'a payload name for the list of its files and runs.</p>'
            '<div style="margin:6px 0">runs from <input id="cdbmap_lo" size="7"> to <input id="cdbmap_hi" size="7"> '
            '<button id="cdbmap_go">zoom</button> <button id="cdbmap_all">all runs</button></div>'
            '<div style="display:flex;gap:8px;align-items:flex-start">'
            f'<div class="maplab" style="text-align:right;font-size:0.8em;white-space:nowrap">{labels}</div>'
            '<div style="flex:1;min-width:0"><canvas id="cdbmap_cv" style="cursor:pointer;display:block"></canvas>'
            '<div id="cdbmap_x" style="display:flex;justify-content:space-between;font-size:0.85em;font-family:monospace"></div></div></div>'
            '<div id="cdbmap_tip" style="display:none;position:absolute;background:#fff;border:1px solid #888;padding:5px 8px;'
            'font-size:0.85em;box-shadow:2px 2px 6px rgba(0,0,0,.25);pointer-events:none;z-index:10"></div>'
            f'<script type="application/json" id="cdbdata">{json.dumps(data, separators=(",", ":"))}</script><script src="{SHARED}cdbqa_cdbmap.js"></script>')


def run_ranges(idx, runs):
    # consecutive positions in the run list -> (first run, last run, number of runs)
    out = []
    for i in idx:
        if out and out[-1][1] == i - 1:
            out[-1][1] = i
        else:
            out.append([i, i])
    return [(runs[a], runs[b], b - a + 1) for a, b in out]


def payload_page(d, runs, status, iovs, ins, changed, cellfile, ptitle, gtag):
    # files of one payload with the runs using each, grouped by status
    groups = {}
    for i, r in enumerate(runs):
        s = status.get(r, {}).get(d, "missing")
        # run-specific entries have one file per run: one row for all of them
        f = "(one file per run, see the run pages)" if s == "run" else cellfile.get((r, d), "")
        groups.setdefault((f, s), []).append(i)
    body = (f'<h1>{LOGO}{d} &ndash; {html.escape(ptitle)} &ndash; {html.escape(gtag)}</h1>'
            '<div class="nav"><a href="index.html#cdbmap">&#8962; Run index</a></div>' + LEGEND + '<br>'
            )
    chg = [(r, changed[(r, d)]) for r in runs if (r, d) in changed]
    if chg:
        body += ('<h2>Updated since last QA</h2><table class="cdb"><tr><th>run</th><th>file</th></tr>'
                 + "".join(f'<tr><td class="upd"><a href="run_{r}.html">{r}</a></td><td>{html.escape(old)}<br>--&gt; '
                           f'{html.escape(cellfile.get((r, d), ""))}</td></tr>' for r, old in chg) + '</table>')
    body += ('<h2>Files and runs</h2><table class="cdb"><tr><th>status</th><th>file</th><th>runs of the entry</th><th>inserted</th>'
             '<th>N runs</th><th>runs (consecutive in the run list)</th></tr>')
    for (f, s), idx in sorted(groups.items(), key=lambda x: (-RANK.get(x[0][1], 0), runs[x[1][0]])):
        ivs = sorted({iovs.get((runs[i], d), "") for i in idx} - {""})
        iovtxt = ", ".join(ivs[:3]) + (f" ... ({len(ivs)} entries)" if len(ivs) > 3 else "")
        its = sorted({ins.get((runs[i], d), "") for i in idx} - {""})
        instxt = its[0] if len(its) == 1 else (f"{its[0]} ... {its[-1]}" if its else "")
        rr = ", ".join(f'<a href="run_{a}.html">{a}</a>' + (f' &ndash; <a href="run_{b}.html">{b}</a> ({n})' if n > 1 else "")
                       for a, b, n in run_ranges(idx, runs))
        body += (f'<tr><td class="{s}">{s}</td><td>{html.escape(f) or "-"}</td><td>{iovtxt}</td><td>{instxt}</td>'
                 f'<td>{len(idx)}</td><td style="font-family:monospace">{rr}</td></tr>')
    return body + '</table>'


CMPJS = """
<script type="module">
import { openFile, draw, cleanup, resize, createHistogram, createTGraph, gStyle, settings } from 'https://cdn.jsdelivr.net/npm/jsroot@7.7.4/modules/main.mjs';
import { getColor, ColorPalette } from 'https://cdn.jsdelivr.net/npm/jsroot@7.7.4/modules/base/colors.mjs';
settings.ToolBar = false;  // no JSROOT pad buttons: each saves one panel only; the save bar under a plot saves both panels
// upper panel and ratio panel share the x axis: no gap between them, x labels only on the ratio panel
const MARGIN = { top: gStyle.fPadTopMargin, bottom: gStyle.fPadBottomMargin, left: gStyle.fPadLeftMargin };
function margins(top, bottom, left) { gStyle.fPadTopMargin = top; gStyle.fPadBottomMargin = bottom; gStyle.fPadLeftMargin = left ?? MARGIN.left; }
// sizes are fractions of the pad height: the ratio panel uses k = (upper height) / (ratio height) times them,
// so labels, titles and ticks come out the same in pixels in both panels
const LEFT = 0.15, LS = 0.04, TS = 0.045;
function axstyle(ax, ay, k) {
  Object.assign(ax, { fLabelSize: LS * k, fTitleSize: TS * k, fLabelOffset: 0.008 * k, fTitleOffset: 1.05, fTickLength: 0.03 * k });
  Object.assign(ay, { fLabelSize: LS * k, fTitleSize: TS * k, fLabelOffset: 0.008, fTitleOffset: 1.45 / k, fTickLength: 0.02 });
  ax.fBits |= (1 << 12); ay.fBits |= (1 << 12);  // TAxis::kCenterTitle
}
// overlay of the same plot from another global tag: this tag as drawn in the PDF, the other tag in the chosen colour
// (single curve) or dashed in the same colours (several curves); below it the ratio other tag / this tag
const COLORS = { red: [2, 'red'], blue: [4, 'blue'], green: [8, 'green'], magenta: [6, 'magenta'] };
const files = {};
function file(path) { if (!files[path]) files[path] = openFile(path); return files[path]; }
async function objects(path, key) {
  const f = await file(path);
  const meta = JSON.parse((await f.readObject(key + '_meta')).fTitle);
  const objs = [];
  for (let i = 0; i < meta.opts.length; i++) objs.push(await f.readObject(key + '_' + i));
  return { meta, objs };
}
function isHist(o) { return o._typename.startsWith('TH1') || o._typename.startsWith('TProfile'); }
function isGraph(o) { return o._typename.startsWith('TGraph'); }
// bin content; a TProfile stores sums, the plotted value is sum / entries
function content(o, i) {
  return o._typename.startsWith('TProfile') ? (o.fBinEntries[i] ? o.fArray[i] / o.fBinEntries[i] : 0) : o.fArray[i];
}
// label step close to ROOT's choice for about n divisions (1, 2 or 5 times a power of ten)
function nice(r, n) {
  const raw = Math.abs(r) / n || 1, mag = Math.pow(10, Math.floor(Math.log10(raw))), m = raw / mag;
  return (m < 1.5 ? 1 : m < 3 ? 2 : m < 7 ? 5 : 10) * mag;
}
// edges of the panels at the join half a label step away from the nearest label, so no label sits on the join
// the edge moves only when the nearest label is closer than GAP pixels to it (px: frame height in pixels)
const GAP = 18;
function lowEdge(lo, hi, px) {
  const st = nice(hi - lo, 5), lab = Math.ceil(lo / st - 1e-9) * st, d = GAP / px * (hi - lo);
  return lab - lo < d ? lab - d : lo;
}
function highEdge(lo, hi, px) {
  const st = nice(hi - lo, 4), lab = Math.floor(hi / st + 1e-9) * st, d = GAP / px * (hi - lo);
  return hi - lab < d ? lab + d : hi;
}
function vis(h) {
  const a = h.fXaxis;
  return a.fFirst > 0 && a.fLast >= a.fFirst ? [a.fFirst, Math.min(a.fLast, a.fNbins)] : [1, a.fNbins];  // unzoomed: fFirst = fLast = 0
}
function edge(a, i) { return a.fXbins && a.fXbins.length ? a.fXbins[i] : a.fXmin + i * (a.fXmax - a.fXmin) / a.fNbins; }
// x range shown: the zoom of the PDF, or the full axis
function xshown(h) { const [i0, i1] = vis(h); return [edge(h.fXaxis, i0 - 1), edge(h.fXaxis, i1)]; }
function range(objs, logy) {
  let hi = 0, lo = Infinity;
  for (const o of objs) {
    if (!isHist(o)) continue;
    const [i0, i1] = vis(o);
    for (let i = i0; i <= i1; i++) { const c = content(o, i); hi = Math.max(hi, c); if (c > 0) lo = Math.min(lo, c); }
  }
  return logy ? [lo * 0.5, hi * 20] : [Math.min(0, lo), hi * 1.3];
}
// ratio other / this: bin by bin for histograms, point by point at the same x for graphs; fitted functions are skipped
function ratios(base, other, single, cnum) {
  const out = [];
  base.objs.forEach((b, i) => {
    const o = other.objs[i];
    if (!o) return;
    const col = single ? cnum : ((isGraph(b) ? b.fMarkerColor : b.fLineColor) || 1);
    if (isHist(b) && isHist(o)) {
      const n = b.fXaxis.fNbins, h = createHistogram('TH1D', n), vals = [];
      Object.assign(h.fXaxis, { fXmin: b.fXaxis.fXmin, fXmax: b.fXaxis.fXmax, fXbins: b.fXaxis.fXbins, fTitle: b.fXaxis.fTitle,
                                fNdivisions: b.fXaxis.fNdivisions });
      const [j0, j1] = vis(b), [xa, xb] = xshown(b);
      for (let j = 1; j <= n; j++) {
        const d = content(b, j);
        if (d) { h.fArray[j] = content(o, j) / d; if (j >= j0 && j <= j1) vals.push(h.fArray[j]); }
      }
      h.fEntries = vals.length; h.fTitle = '';
      Object.assign(h, { fLineColor: col, fMarkerColor: col, fMarkerStyle: 20, fMarkerSize: 0.6 });
      out.push({ obj: h, vals, xmin: xa, xmax: xb, zoom: j0 > 1 || j1 < n });
    } else if (isGraph(b) && isGraph(o)) {
      const m = new Map();
      for (let j = 0; j < o.fNpoints; j++) m.set(o.fX[j], o.fY[j]);
      const xs = [], ys = [];
      for (let j = 0; j < b.fNpoints; j++) if (b.fY[j] && m.has(b.fX[j])) { xs.push(b.fX[j]); ys.push(m.get(b.fX[j]) / b.fY[j]); }
      if (!xs.length) return;
      const g = createTGraph(xs.length, xs, ys);
      Object.assign(g, { fTitle: '', fLineColor: col, fMarkerColor: col, fMarkerStyle: 20, fMarkerSize: 0.5 });
      const ax = b.fHistogram ? b.fHistogram.fXaxis : null;
      out.push({ obj: g, vals: ys, xmin: ax ? ax.fXmin : Math.min(...xs), xmax: ax ? ax.fXmax : Math.max(...xs),
                 xtitle: ax ? ax.fTitle : '' });
    }
  });
  return out;
}
async function drawRatio(div, rs, logx, label) {
  cleanup(div);
  if (!rs.length) { div.style.display = 'none'; return; }
  div.style.display = '';
  const vals = rs.flatMap(r => r.vals).filter(v => isFinite(v));
  let lo = Math.max(Math.min(1, ...vals), -3), hi = Math.min(Math.max(1, ...vals), 5);
  if (hi - lo < 1e-6) { lo = 0.9; hi = 1.1; }
  { const p = 0.1 * (hi - lo); lo -= p; hi += p; }
  hi = highEdge(lo, hi, div.clientHeight * 0.6);  // frame = pad without the 40% bottom margin
  const xmin = Math.min(...rs.map(r => r.xmin)), xmax = Math.max(...rs.map(r => r.xmax));
  const first = rs[0].obj;
  if (isGraph(first)) {
    const h = createHistogram('TH1F', 100);
    // same x axis as the upper panel (range, title, labels)
    Object.assign(h.fXaxis, { fXmin: xmin, fXmax: xmax, fTitle: rs[0].xtitle || '' });
    h.fYaxis.fTitle = label; h.fMinimum = lo; h.fMaximum = hi; h.fTitle = ''; h.fLineWidth = 0;
    h.fBits |= (1 << 9); h.fXaxis.fBits |= (1 << 17);  // TH1::kNoStats, TAxis::kNoExponent = BIT(17), run numbers in full
    h.fXaxis.fNdivisions = 505;  // same x divisions as the upper panel
    first.fHistogram = h; first.fMinimum = lo; first.fMaximum = hi;
  } else {
    first.fMinimum = lo; first.fMaximum = hi; first.fYaxis.fTitle = label;
  }
  const hax = isGraph(first) ? first.fHistogram : first;
  const k = div.previousElementSibling.clientHeight / div.clientHeight;
  axstyle(hax.fXaxis, hax.fYaxis, k);
  hax.fYaxis.fNdivisions = 505;
  const lx = logx ? ';logx' : '';
  margins(0, 0.4, LEFT);
  const rp = await draw(div, first, (isGraph(first) ? 'ap' : 'p') + lx + ';tickx;ticky;nostat');
  margins(MARGIN.top, MARGIN.bottom, MARGIN.left);
  if (rs[0].zoom) await rp.getFramePainter().zoom(xmin, xmax);
  for (const r of rs.slice(1)) await draw(div, r.obj, 'p same');
  const one = createTGraph(2, [xmin, xmax], [1, 1]);
  Object.assign(one, { fLineStyle: 2, fLineColor: 1, fTitle: '' });
  await draw(div, one, 'l same');
}
// legend of a plot with several curves: each curve in its colour, then which tag is filled/solid and open/dashed
function multileg(base, thisTag, tag) {
  const mk = isGraph(base.objs[0]) || base.meta.opts[0] === 'p';
  const esc = t => String(t).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
  let leg = (base.meta.names || []).map((n, i) => {
    const o = base.objs[i];
    if (!o) return '';
    const c = getColor((mk ? o.fMarkerColor : o.fLineColor) || 1);
    return `<div style="color:${c}">${mk ? '&#9679;' : '&#9644;'} ${esc(n)}</div>`;
  }).join('');
  if (!tag) return leg;
  return leg + (mk ? `<div>filled: ${thisTag}</div><div>open: ${tag}</div>` : `<div>solid: ${thisTag}</div><div>dashed: ${tag}</div>`);
}
// 2D map (run x ieta) of another tag: drawn as the ratio other tag / this tag, bins matched by run number
// colours of the ratio: blue below 1 (darker further from 1), white exactly 1, red above 1 (darker further from 1)
const MAPNB = 25;
const MAPCOLS = (() => {
  const hex = v => Math.round(v).toString(16).padStart(2, '0'), c = [];
  for (let i = 0; i < MAPNB; i++) { const w = 200 * i / (MAPNB - 1); c.push('#' + hex(w) + hex(w) + 'ff'); }
  c.push('#ffffff');
  for (let i = 0; i < MAPNB; i++) { const w = 200 - 200 * i / (MAPNB - 1); c.push('#ff' + hex(w) + hex(w)); }
  return c;
})();
async function renderMap(box, base, other, thisTag, tag) {
  const div = box.querySelector('.jsdraw'), rdiv = box.querySelector('.jsratio');
  rdiv.style.display = 'none';
  const b = base.objs[0], o = other.objs[0];
  const nx = b.fXaxis.fNbins, ny = b.fYaxis.fNbins, onx = o.fXaxis.fNbins;
  const oi = new Map((other.meta.runs || []).map((r, i) => [r, i + 1]));
  let lo = Infinity, hi = -Infinity;
  for (let i = 1; i <= nx; i++) {
    const j = oi.get((base.meta.runs || [])[i - 1]);
    for (let y = 1; y <= ny; y++) {
      const k = i + (nx + 2) * y, vb = b.fArray[k], vo = j ? o.fArray[j + (onx + 2) * y] : 0;
      b.fArray[k] = (vb && vo) ? vo / vb : 0;  // no value in either tag: empty bin
      if (b.fArray[k]) { lo = Math.min(lo, b.fArray[k]); hi = Math.max(hi, b.fArray[k]); }
    }
  }
  // scale symmetric around 1, one narrow level around 1 for the white of identical values
  const d = Math.max(isFinite(lo) ? Math.abs(lo - 1) : 0, isFinite(hi) ? Math.abs(hi - 1) : 0, 1e-3), eps = 1e-6, lev = [];
  for (let i = 0; i <= MAPNB; i++) lev.push(1 - d + i * (d - eps) / MAPNB);
  for (let i = 0; i <= MAPNB; i++) lev.push(1 + eps + i * (d - eps) / MAPNB);
  b.fContour = lev; b.fBits |= (1 << 10);  // TH1::kUserContour
  b.fMinimum = 1 - d; b.fMaximum = 1 + d;
  b.fZaxis.fTitle = `${tag} / ${thisTag}`;
  b.fZaxis.fBits |= (1 << 17);  // TAxis::kNoExponent: ratios near 1 in full
  const keep = [gStyle.fPadTopMargin, gStyle.fPadBottomMargin, gStyle.fPadLeftMargin, gStyle.fPadRightMargin];
  Object.assign(gStyle, { fPadTopMargin: 0.08, fPadBottomMargin: 0.17, fPadLeftMargin: LEFT, fPadRightMargin: 0.19 });
  const hp = await draw(div, b, 'colz;tickx;ticky;nostat');
  hp.options.Palette = 0;  // the pad palette below, not a ROOT palette
  hp.getPadPainter().custom_palette = new ColorPalette(MAPCOLS);
  hp.getHistPalette(true);
  await hp.redraw();
  runLink(hp, box.querySelector('.cmp').dataset.file, base.meta.runs, [b]);
  [gStyle.fPadTopMargin, gStyle.fPadBottomMargin, gStyle.fPadLeftMargin, gStyle.fPadRightMargin] = keep;
  box.querySelector('.cleg').innerHTML = '';
  requestAnimationFrame(() => resize(div));
}
// summary plots vs run number: a click within 8 pixels of a point (graph x = run number) or on a map column (meta.runs)
// opens that run's page
function runLink(painter, path, runs, objs) {
  const at = path.indexOf('plots/summary/');
  if (!painter || at < 0) return;
  const div = painter.selectDom().node();
  div._runlink = { painter, dir: path.slice(0, at), runs, objs };
  if (div._runlinked) return;  // one listener per plot; a redraw only replaces what it reads
  div._runlinked = true;
  div.addEventListener('click', ev => {
    const L = div._runlink, fp = L.painter.getFramePainter(), svg = div.querySelector('svg');
    if (!fp || !svg) return;
    const b = svg.getBoundingClientRect();
    const x = ev.clientX - b.left - fp.getFrameX(), y = ev.clientY - b.top - fp.getFrameY();
    if (x < 0 || y < 0 || x > fp.getFrameWidth() || y > fp.getFrameHeight()) return;
    let run = 0;
    if (L.runs) {
      const ax = L.objs[0].fXaxis, v = fp.revertAxis('x', x);
      run = L.runs[Math.floor((v - ax.fXmin) / (ax.fXmax - ax.fXmin) * ax.fNbins)] || 0;
    } else {
      let best = 64;
      for (const g of L.objs.filter(isGraph)) {
        for (let i = 0; i < g.fNpoints; i++) {
          const dx = fp.grx(g.fX[i]) - x, dy = fp.gry(g.fY[i]) - y;
          if (dx * dx + dy * dy < best) { best = dx * dx + dy * dy; run = g.fX[i]; }
        }
      }
    }
    if (run) window.location = L.dir + 'run_' + Math.round(run) + '.html';
  });
}
// ROOT text of the plot label as HTML: sub/superscripts and a few Greek letters
function latex(s) {
  const greek = { eta: '&eta;', phi: '&phi;', gamma: '&gamma;', pi: '&pi;', Sigma: '&Sigma;', sigma: '&sigma;', mu: '&mu;' };
  return s.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')
    .replace(/#(it|bf)[{]([^}]*)[}]/g, '$2')
    .replace(/_[{]([^}]*)[}]/g, '<sub>$1</sub>').replace(/[\\^][{]([^}]*)[}]/g, '<sup>$1</sup>')
    .replace(/#([a-zA-Z]+)/g, (m, g) => greek[g] || m);
}
async function render1(box) {
  const ctl = box.querySelector('.cmp');
  const tsel = ctl.querySelector('.tag'), tag = tsel ? tsel.value : '';
  const colsel = ctl.querySelector('.col');
  const [cnum, ccss] = COLORS[colsel ? colsel.value : 'red'];
  const frame = box.querySelector('.frame');
  const pdf = box.querySelector('a.pdf');
  const div = box.querySelector('.jsdraw'), rdiv = box.querySelector('.jsratio');
  cleanup(div); cleanup(rdiv);
  box.querySelector('.cleg').innerHTML = '';
  const key = ctl.dataset.key, path = ctl.dataset.file;
  // "none": this tag alone, drawn the same way; the PDF when the objects are not in the ROOT file
  let base;
  try { base = await objects(path, key); } catch (e) { base = null; }
  if (!base || !base.objs.length) { frame.style.display = 'none'; pdf.style.display = ''; return; }
  frame.style.display = ''; pdf.style.display = 'none';
  const oldtext = box.querySelector('.ptext');
  if (oldtext) oldtext.remove();
  if (base.meta.text) box.querySelector('.main').insertAdjacentHTML('beforeend', '<div class="ptext">' + latex(base.meta.text) + '</div>');
  const other = tag ? await objects(ctl.dataset.other || ('../' + tag + '/' + path), key) : { meta: base.meta, objs: [] };
  if (base.objs[0]._typename.startsWith('TH2') && !tag) {
    rdiv.style.display = 'none';
    const keep = [gStyle.fPadTopMargin, gStyle.fPadBottomMargin, gStyle.fPadLeftMargin, gStyle.fPadRightMargin];
    Object.assign(gStyle, { fPadTopMargin: 0.08, fPadBottomMargin: 0.17, fPadLeftMargin: LEFT, fPadRightMargin: 0.19 });
    const hp2 = await draw(div, base.objs[0], 'colz;tickx;ticky;nostat' + (base.meta.logy ? ';logz' : ''));  // logy of a 2D histogram: log z
    runLink(hp2, path, base.meta.runs, base.objs);
    [gStyle.fPadTopMargin, gStyle.fPadBottomMargin, gStyle.fPadLeftMargin, gStyle.fPadRightMargin] = keep;
    addSavebar(box, frame);
    requestAnimationFrame(() => resize(div));
    return;
  }
  if (base.objs[0]._typename.startsWith('TH2')) {
    await renderMap(box, base, other, ctl.dataset.base || document.body.dataset.tag, tag);
    addSavebar(box, frame);
    return;
  }
  const single = base.objs.length === 1 || base.meta.opts.every((o, i) => i === 0 || o === 'l');
  other.objs.forEach((o, i) => {
    if (single) { o.fLineColor = cnum; o.fMarkerColor = cnum; } else { o.fLineStyle = 2; o.fMarkerStyle = 24; }
  });
  const first = base.objs[0];
  if (isHist(first)) {
    const [lo, hi] = range(base.objs.concat(other.objs), base.meta.logy);
    first.fMinimum = lo; first.fMaximum = hi;
  } else if (isGraph(first)) {
    // own axis histogram for the graphs: x range shared with the ratio panel, y range covering both tags
    const gs = base.objs.concat(other.objs).filter(isGraph);
    const xs = gs.flatMap(g => Array.from(g.fX.slice(0, g.fNpoints)));
    const ys = gs.flatMap(g => Array.from(g.fY.slice(0, g.fNpoints))).filter(v => isFinite(v));
    const old = first.fHistogram, h = createHistogram('TH1F', 100);
    if (old) { Object.assign(h.fXaxis, old.fXaxis); Object.assign(h.fYaxis, old.fYaxis); }
    let x0 = old ? old.fXaxis.fXmin : Math.min(...xs), x1 = old ? old.fXaxis.fXmax : Math.max(...xs);
    if (!old) { const p = 0.05 * (x1 - x0 || 1); x0 -= p; x1 += p; }
    let y0 = Math.min(...ys), y1 = Math.max(...ys); const q = 0.1 * (y1 - y0 || 1); y0 -= q; y1 += q;
    Object.assign(h.fXaxis, { fXmin: x0, fXmax: x1 });
    h.fXaxis.fBits |= (1 << 17); h.fBits |= (1 << 9); h.fTitle = ''; h.fXaxis.fNdivisions = 505; h.fLineWidth = 0;
    gs.forEach(g => { g.fMarkerSize = 0.5; });
    h.fMinimum = y0; h.fMaximum = y1; first.fMinimum = y0; first.fMaximum = y1;
    first.fHistogram = h;
  }
  const rs = ratios(base, other, single, cnum);
  // with a ratio panel: x labels and title only below, upper panel without bottom margin
  const xax = isHist(first) ? first.fXaxis : (first.fHistogram ? first.fHistogram.fXaxis : null);
  const yax = isHist(first) ? first.fYaxis : (first.fHistogram ? first.fHistogram.fYaxis : null);
  if (rs.length && xax && yax) {
    axstyle(xax, yax, 1); xax.fLabelSize = 0; xax.fTitleSize = 0; xax.fTitle = '';
    const g = isGraph(first) ? first.fHistogram : first;
    if (!base.meta.logy) { yax.fNdivisions = 505; g.fMinimum = lowEdge(g.fMinimum, g.fMaximum, div.clientHeight * (1 - MARGIN.top)); if (isGraph(first)) first.fMinimum = g.fMinimum; }
  }
  const o0 = base.meta.opts[0] === 'p' ? 'ap' : base.meta.opts[0];
  if (rs.length) margins(MARGIN.top, 0, LEFT);
  const up = await draw(div, first, o0 + (base.meta.logy ? ';logy' : '') + (base.meta.logx ? ';logx' : '') + ';tickx;ticky;nostat');
  margins(MARGIN.top, MARGIN.bottom, MARGIN.left);
  runLink(up, path, base.meta.runs, base.objs);
  if (isHist(first)) { const [j0, j1] = vis(first); if (j0 > 1 || j1 < first.fXaxis.fNbins) { const [xa, xb] = xshown(first); await up.getFramePainter().zoom(xa, xb); } }
  const same = o => (o === 'p' ? 'p same' : o === 'l' ? 'same' : o + ' same');
  for (let i = 1; i < base.objs.length; i++) await draw(div, base.objs[i], same(base.meta.opts[i]));
  for (let i = 0; i < other.objs.length; i++) await draw(div, other.objs[i], same(other.meta.opts[i]));
  const thisTag = ctl.dataset.base || document.body.dataset.tag;
  box.querySelector('.cleg').innerHTML = !tag ? (single ? '' : multileg(base, thisTag, tag)) : single
    ? `<div>&#9644; ${thisTag}</div><div style="color:${ccss}">&#9644; ${tag}</div>`
    : multileg(base, thisTag, tag);
  await drawRatio(rdiv, rs, base.meta.logx, 'ratio');
  addSavebar(box, frame);
  // redraw once laid out, so a plot drawn while the page was in the background still shows
  requestAnimationFrame(() => { resize(div); resize(rdiv); });
}
function addSavebar(box, frame) {
  if (frame.querySelector('.savebar')) return;
  frame.insertAdjacentHTML('beforeend', '<div class="savebar">save <a href="#" data-ext="pdf">PDF</a></div>');
  frame.querySelectorAll('.savebar a').forEach(a => a.addEventListener('click', ev => {
    ev.preventDefault();
    save(box, a.dataset.ext).catch(e => alert('save failed: ' + e));
  }));
}
// save bar: upper panel, ratio panel and legend of a plot in one SVG, or one PDF (jsPDF + svg2pdf, loaded on the first PDF)
const SAVE_LIBS = ['https://cdnjs.cloudflare.com/ajax/libs/jspdf/2.5.1/jspdf.umd.min.js',
                   'https://cdn.jsdelivr.net/npm/svg2pdf.js@2.2.3/dist/svg2pdf.umd.min.js'];
let libs = null;
const loadLibs = () => libs || (libs = SAVE_LIBS.reduce((p, u) => p.then(() => new Promise((ok, bad) => {
  const sc = document.createElement('script');
  sc.src = u; sc.onload = ok; sc.onerror = () => bad(new Error('cannot load ' + u));
  document.head.appendChild(sc);
})), Promise.resolve()));
function joinedSvg(box) {
  const parts = [box.querySelector('.jsdraw svg'), box.querySelector('.jsratio svg')].filter(Boolean);
  const top = parts[0].getBoundingClientRect(), W = Math.max(...parts.map(p => p.getBoundingClientRect().width));
  let y = 0, inner = '';
  parts.forEach(p => {
    const r = p.getBoundingClientRect(), c = p.cloneNode(true);
    c.setAttribute('x', 0); c.setAttribute('y', y); c.setAttribute('width', r.width); c.setAttribute('height', r.height);
    inner += new XMLSerializer().serializeToString(c);
    y += r.height;
  });
  // the legend is an HTML box over the upper panel: written into the SVG at the same place
  const leg = box.querySelector('.cleg'), lr = leg.getBoundingClientRect();
  [...leg.children].forEach((d, i) => {
    const t = d.textContent.replace(/&/g, '&amp;').replace(/</g, '&lt;');
    inner += `<text x="${lr.right - top.left - 6}" y="${lr.top - top.top + 15 * (i + 1)}" text-anchor="end" ` +
             `font-family="Arial, Helvetica, sans-serif" font-size="12" fill="${getComputedStyle(d).color}">${t}</text>`;
  });
  return `<svg xmlns="http://www.w3.org/2000/svg" width="${W}" height="${y}" viewBox="0 0 ${W} ${y}">` +
         `<rect width="${W}" height="${y}" fill="white"/>${inner}</svg>`;
}
function saveName(box, ext) {
  const ctl = box.querySelector('.cmp'), a = ctl.dataset.base || document.body.dataset.tag, tsel = ctl.querySelector('.tag');
  const b = tsel ? tsel.value : '';
  const run = (ctl.dataset.file.match(/run([0-9]+)[.]root/) || [])[1];
  return `${ctl.dataset.key}${run ? '_run' + run : ''}_${a}${b ? '_vs_' + b : ''}.${ext}`.replace(/[^A-Za-z0-9_.+-]/g, '_');
}
function download(blob, name) {
  const a = document.createElement('a');
  a.href = URL.createObjectURL(blob); a.download = name;
  document.body.appendChild(a); a.click(); a.remove();
  setTimeout(() => URL.revokeObjectURL(a.href), 10000);
}
async function save(box, ext) {
  const svg = joinedSvg(box);
  if (ext === 'svg') { download(new Blob([svg], { type: 'image/svg+xml' }), saveName(box, 'svg')); return; }
  await loadLibs();
  const el = new DOMParser().parseFromString(svg, 'image/svg+xml').documentElement;
  const w = +el.getAttribute('width'), h = +el.getAttribute('height');
  const doc = new window.jspdf.jsPDF({ orientation: w > h ? 'l' : 'p', unit: 'pt', format: [w, h] });
  el.style.position = 'absolute'; el.style.left = '-10000px';
  document.body.appendChild(el);  // svg2pdf reads computed styles: the element has to be in the page
  try { await doc.svg(el, { x: 0, y: 0, width: w, height: h }); } finally { el.remove(); }
  doc.save(saveName(box, 'pdf'));
}
// one plot at a time: the pad margins are set through the shared gStyle while a plot is drawn
let queue = Promise.resolve();
function render(box) { queue = queue.then(() => render1(box)).catch(e => console.error(e)); return queue; }
// every plot with a menu is drawn when it comes into view (comparison page: the two tags, other pages: this tag alone)
const auto = new IntersectionObserver(es => es.forEach(e => { if (e.isIntersecting) { auto.unobserve(e.target); render(e.target); } }),
                                      { rootMargin: '300px' });
document.querySelectorAll('.cmp').forEach(ctl => { const box = ctl.closest('.plot'); if (box) auto.observe(box); });
// a plot drawn before the page had its layout (zero width, JSROOT hides it) is redrawn once its box has a size
const sized = new ResizeObserver(es => es.forEach(e => { if (e.contentRect.width > 0 && e.target.querySelector('svg')) resize(e.target); }));
document.querySelectorAll('.jsdraw, .jsratio').forEach(d => sized.observe(d));
// live comparison page: its plots are added after loading, then drawn through these
window.cmpObserve = box => auto.observe(box);
window.cmpRender = render;
document.dispatchEvent(new Event('cmpready'));
document.querySelectorAll('.cmp').forEach(ctl => {
  const box = ctl.closest('.plot');
  ctl.querySelectorAll('select').forEach(sel => sel.addEventListener('change', () => render(box)));
});
</script>
"""


# style and scripts of the pages: one shared copy in the web folder (written by every make_html run), loaded by every page,
# so a change of the page code is a change of these files only
SHARED = "/WWW/subsystem/CDBCaloQA/"
PDFJS_TAGS = ('<script src="https://cdnjs.cloudflare.com/ajax/libs/pdf.js/3.11.174/pdf.min.js"></script>'
              f'<script src="{SHARED}cdbqa_pdf.js"></script>')


PERIODJS = """
<script>
if (document.getElementById('tagB').options.length > 1) document.getElementById('tagB').selectedIndex = 1;
document.getElementById('cmpgo').addEventListener('click', () => {
  const a = document.getElementById('tagA').value, b = document.getElementById('tagB').value;
  if (a === b) { document.getElementById('cmpmsg').textContent = 'pick two different tags'; return; }
  window.location = 'compare.html?a=' + encodeURIComponent(a) + '&b=' + encodeURIComponent(b);
});
</script>
"""


def script_body(block):
    # code of the last <script> element of a block
    return block[block.rindex("<script"):].split(">", 1)[1].rsplit("</script>", 1)[0]


def write_shared(webdir):
    files = {"cdbqa.css": STYLE, "cdbqa_pdf.js": script_body(PDFJS), "cdbqa_cmp.js": script_body(CMPJS),
             "cdbqa_cdbmap.js": script_body(CDBMAP_JS), "cdbqa_live.css": CMPCSS, "cdbqa_live.js": script_body(CMPLIVE),
             "cdbqa_period.js": script_body(PERIODJS)}
    for f, text in files.items():
        # temporary name per process: pass 2 of several tags can write these at the same time
        tmp = os.path.join(webdir, f"{f}.{os.getpid()}.new")
        open(tmp, "w").write(text)
        os.replace(tmp, os.path.join(webdir, f))


def page(title, body, clicknote=True):
    # clicknote: the "click a plot" hint, only on pages whose plots open their PDF
    cmp = f'<script type="module" src="{SHARED}cdbqa_cmp.js"></script>' if 'class="cmp"' in body else ""
    return (f'<!DOCTYPE html><html><head><meta charset="utf-8"><title>{title}</title><link rel="stylesheet" href="{SHARED}cdbqa.css"></head>'
            f'<body data-tag="{html.escape(PAGETAG)}">{body}<p class="note">Generated {datetime.datetime.now():%Y-%m-%d %H:%M}.'
            f'{(" Hover a plot for values, its PDF link opens the saved PDF." if cmp else " Click a plot to open the PDF.") if clicknote else ""}'
            f'</p>{PDFJS_TAGS}{cmp}</body></html>')


PAGETAG = ""  # global tag of the pages being written (for the comparison legend)

CDBAPI = "http://sphenix-cdb.apps.sdcc.bnl.gov/api/cdb_rest/globalTags"


def tag_dates():
    # global tag name -> (created, updated) from the CDB; empty when the CDB cannot be reached
    try:
        with urllib.request.urlopen(CDBAPI, timeout=60) as r:
            return {t["name"]: (t["created"][:16].replace("T", " "), t["updated"][:16].replace("T", " "))
                    for t in json.load(r)}
    except Exception as e:
        print(f"CDB global tag list not read: {e}")
        return {}


def period_page(webdir, period, ptitle):
    # period page: every global tag of the period with pages, and the tag comparisons
    pp = os.path.join(webdir, period)
    tags = sorted(t for t in os.listdir(pp) if os.path.exists(os.path.join(pp, t, "index.html")))
    dates = tag_dates()
    rows = ""
    for t in tags:
        td = os.path.join(pp, t)
        nrun = sum(1 for x in os.listdir(td) if x.startswith("run_") and x.endswith(".html"))
        when = datetime.datetime.fromtimestamp(os.path.getmtime(os.path.join(td, "index.html")))
        cr, up = dates.get(t, ("-", "-"))
        rows += (f'<tr><td><a href="{t}/index.html">{html.escape(t)}</a></td><td>{nrun}</td><td>{when:%Y-%m-%d %H:%M}</td>'
                 f'<td>{cr}</td><td>{up}</td></tr>')
    # comparison of two tags: made live in the browser from the fingerprints of the two tags (compare.html)
    opts = "".join(f"<option>{html.escape(t)}</option>" for t in tags)
    cmpbox = ('<h2>Compare two global tags</h2>'
              f'<div class="cmpsel">compare <select id="tagA">{opts}</select> with <select id="tagB">{opts}</select> '
              '<button id="cmpgo">compare</button> <span id="cmpmsg" class="note"></span></div>'
              f'<script src="{SHARED}cdbqa_period.js"></script>')
    body = (f'<h1>{LOGO}sPHENIX CDB Calo QA &ndash; {html.escape(ptitle)}</h1><div class="nav"><a href="../index.html">&#8962; All periods</a></div>'
            + cmpbox +
            '<h2>Global tags</h2><table class="cdb"><tr><th>global tag</th><th>runs</th><th>last QA pass</th>'
            '<th>created in CDB</th><th>updated in CDB</th></tr>' + rows + '</table>'
            '<p class="note">Click a global tag for its run-by-run QA and CDB status.</p>')
    open(os.path.join(pp, "index.html"), "w").write(page(f"CDB Calo QA - {ptitle}", body))
    compare_page(webdir, period, ptitle)


CMPCSS = """
.counts span { display: inline-block; margin-right: 22px; font-size: 1.05em; }
.counts b.ch { color: #c62828; } .counts b.same { color: #2e7d32; } .counts b.miss { color: #ef6c00; }
table.chg td.num { text-align: right; font-family: monospace; }
table.chg td.sec { color: #1a3e6e; font-weight: bold; }
.same p { margin: 3px 0; font-size: 0.9em; } .same b { color: #1a3e6e; display: inline-block; min-width: 90px; }
.plot p.note { font-weight: normal; color: #555; font-size: 0.8em; margin: 2px 0 0; }
#cmpstatus { font-size: 1.1em; margin: 14px 0; }
.spin { display: inline-block; width: 14px; height: 14px; border: 3px solid #ccc; border-top-color: #1a3e6e; border-radius: 50%;
        animation: spin 0.8s linear infinite; vertical-align: middle; margin-right: 8px; }
@keyframes spin { to { transform: rotate(360deg); } }
.cdbsum b.ch { color: #c62828; } .cdbsum b.same { color: #2e7d32; }
.cdbleg span { display: inline-block; width: 12px; height: 12px; margin: 0 4px 0 14px; vertical-align: middle; }
.cdbmap { position: relative; margin: 6px 0 10px; }
#cdbtip { position: absolute; display: none; background: #fff; border: 1px solid #999; padding: 4px 7px; font-size: 12px;
          pointer-events: none; white-space: nowrap; box-shadow: 0 1px 4px rgba(0,0,0,0.2); }
"""

CMPLIVE = """
<script>
// live comparison of two global tags: the fingerprints of both tags (one hash per plot and run) are compared here,
// the changed plots are then drawn from the plot files of both tags (tag A black / filled, tag B red / open, ratio B / A)
(async () => {
  const q = new URLSearchParams(location.search), A = q.get('a'), B = q.get('b');
  const st = document.getElementById('cmpstatus'), out = document.getElementById('cmpout');
  const esc = t => String(t).replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
  if (!A || !B) { st.textContent = 'choose two global tags on the period page'; return; }
  document.getElementById('cmptitle').textContent = A + ' vs ' + B;
  document.title += ' - ' + A + ' vs ' + B;
  st.innerHTML = '<span class="spin"></span>comparing ' + esc(A) + ' and ' + esc(B) + ' &hellip;';
  const load = async t => {
    const r = await fetch(encodeURIComponent(t) + '/plots/fingerprint.json', { cache: 'no-store' });
    if (!r.ok) throw new Error('no fingerprint for ' + t + ' yet: run macros/run_pass2.sh ' + PERIOD + ' ' + t);
    return r.json();
  };
  const loadp = t => fetch(encodeURIComponent(t) + '/plots/payloads.json', { cache: 'no-store' })
    .then(r => (r.ok ? r.json() : null), () => null);
  let fa, fb, pa, pb;
  try { [fa, fb, pa, pb] = await Promise.all([load(A), load(B), loadp(A), loadp(B)]); } catch (e) { st.textContent = e.message; return; }
  // CDB payloads: per payload type and run, 0 same file in both tags, 1 different file (md5), 2 only in A, 3 only in B
  const cdb = { rows: [], nsame: 0, cr: [] };
  const md5 = (p, t, i) => (p.types[t] ? p.types[t].h.substr(6 * i, 6) : '------');
  const ins = (p, t, i) => { const s = p.types[t] ? p.types[t].t.substr(3 * i, 3) : '---'; return s === '---' ? '' : p.dates[parseInt(s, 36)]; };
  if (pa && pb) {
    const ib = new Map(pb.runs.map((r, j) => [r, j]));
    cdb.cr = pa.runs.map((r, i) => [r, i, ib.get(r)]).filter(x => x[2] !== undefined);
    for (const t of [...new Set([...Object.keys(pa.types), ...Object.keys(pb.types)])].sort()) {
      const sv = [], n = [0, 0, 0, 0], da = new Set(), db = new Set();
      for (const [r, i, j] of cdb.cr) {
        const a = md5(pa, t, i), b = md5(pb, t, j);
        const s = a === b ? 0 : a === '------' ? 3 : b === '------' ? 2 : 1;
        sv.push(s); n[s]++;
        if (s && s !== 3) da.add(ins(pa, t, i));
        if (s && s !== 2) db.add(ins(pb, t, j));
      }
      if (n[0] === cdb.cr.length) cdb.nsame++;
      else cdb.rows.push({ t, sv, n, da: [...da].sort(), db: [...db].sort() });
    }
  }
  const CCOL = ['#d9d9d9', '#c62828', '#ef6c00', '#1565c0'];
  const dlist = d => (d.length ? d.slice(0, 2).join(', ') + (d.length > 2 ? ` &hellip; (${d.length} times)` : '') : '&ndash;');
  let hcdb = '<h2>CDB payloads</h2>';
  if (!pa || !pb) {
    hcdb += `<p class="note">no payload list for ${esc(!pa ? A : B)} yet: run macros/run_pass2.sh ${PERIOD} ${esc(!pa ? A : B)}</p>`;
  } else {
    hcdb += `<p class="cdbsum">${cdb.nsame + cdb.rows.length} payload types over ${cdb.cr.length} runs: ` +
            `<b class="same">${cdb.nsame}</b> use the same file (md5) in both tags for every run, <b class="ch">${cdb.rows.length}</b> differ</p>`;
    if (cdb.rows.length) {
      hcdb += `<p class="cdbleg"><span style="background:${CCOL[0]};margin-left:0"></span>same file<span style="background:${CCOL[1]}"></span>different file` +
              `<span style="background:${CCOL[2]}"></span>only in ${esc(A)}<span style="background:${CCOL[3]}"></span>only in ${esc(B)}</p>` +
              '<div class="cdbmap"><canvas id="cdbmap"></canvas><div id="cdbtip"></div></div>' +
              `<table class="cdb chg"><tr><th>payload type</th><th>runs with a different file</th><th>runs only in ${esc(A)}</th>` +
              `<th>runs only in ${esc(B)}</th><th>inserted (${esc(A)})</th><th>inserted (${esc(B)})</th></tr>` +
              cdb.rows.map(x => `<tr><td class="sec">${x.t}</td><td class="num">${x.n[1]}</td><td class="num">${x.n[2]}</td>` +
                                `<td class="num">${x.n[3]}</td><td>${dlist(x.da)}</td><td>${dlist(x.db)}</td></tr>`).join('') + '</table>';
    }
  }
  // map of the payload types that differ vs run; hover shows the files of both tags
  const drawcdb = () => {
    const cv = document.getElementById('cdbmap');
    if (!cv || !cv.getContext) return;
    const rows = cdb.rows, n = cdb.cr.length, L = 230, R = 12, T = 2, H = 16, BOT = 30;
    const W = Math.max(600, cv.parentElement.clientWidth || 1000), pw = W - L - R, ht = T + rows.length * H + BOT;
    const dpr = window.devicePixelRatio || 1;
    cv.width = W * dpr; cv.height = ht * dpr; cv.style.width = W + 'px'; cv.style.height = ht + 'px';
    const g = cv.getContext('2d');
    g.scale(dpr, dpr);
    g.font = '12px sans-serif';
    rows.forEach((row, k) => {
      const y = T + k * H;
      g.fillStyle = '#1a3e6e'; g.textAlign = 'right'; g.textBaseline = 'middle';
      g.fillText(row.t, L - 6, y + H / 2);
      g.fillStyle = CCOL[0]; g.fillRect(L, y + 1, pw, H - 2);
      row.sv.forEach((s, x) => { if (s) { g.fillStyle = CCOL[s]; g.fillRect(L + x * pw / n, y + 1, Math.max(pw / n, 1), H - 2); } });
    });
    const yb = T + rows.length * H, nt = Math.min(7, n);
    g.fillStyle = '#333'; g.textAlign = 'center'; g.textBaseline = 'top';
    for (let q = 0; q < nt; q++) {
      const x = Math.round(q * (n - 1) / Math.max(nt - 1, 1)), px = L + (x + 0.5) * pw / n;
      g.fillRect(px, yb, 1, 4);
      g.fillText(cdb.cr[x][0], px, yb + 6);
    }
    g.fillText('run number', L + pw / 2, yb + 18);
    const tip = document.getElementById('cdbtip');
    cv.onmousemove = ev => {
      const bx = cv.getBoundingClientRect(), mx = ev.clientX - bx.left, my = ev.clientY - bx.top;
      const k = Math.floor((my - T) / H), x = Math.floor((mx - L) / (pw / n));
      if (k < 0 || k >= rows.length || x < 0 || x >= n) { tip.style.display = 'none'; return; }
      const [r, i, j] = cdb.cr[x], t = rows[k].t, f = (p, ii) => { const m = md5(p, t, ii); return m === '------' ? 'none' : `md5 ${m}&hellip;, inserted ${ins(p, t, ii)}`; };
      tip.innerHTML = `<b>${t}</b>, run ${r}<br>${esc(A)}: ${f(pa, i)}<br>${esc(B)}: ${f(pb, j)}`;
      tip.style.display = 'block';
      tip.style.left = Math.min(mx + 14, W - tip.offsetWidth) + 'px'; tip.style.top = (my + 14) + 'px';
    };
    cv.onmouseleave = () => { tip.style.display = 'none'; };
  };
  const runs = Object.keys(fa.runs).filter(r => r in fb.runs).sort((x, y) => x - y);
  const onlyA = Object.keys(fa.runs).filter(r => !(r in fb.runs)), onlyB = Object.keys(fb.runs).filter(r => !(r in fa.runs));
  const sec = (m, k) => m[k] || ['other', k], ord = s => { const i = ORDER.indexOf(s); return i < 0 ? 99 : i; };
  // per-run plots: runs where the hash differs; plots in one tag only counted apart
  const chg = {}, keys = new Set(); let miss = 0;
  for (const r of runs) {
    const a = fa.runs[r], b = fb.runs[r];
    for (const k of new Set([...Object.keys(a), ...Object.keys(b)])) {
      if (!(k in a) || !(k in b)) { miss++; continue; }
      keys.add(k);
      if (a[k] !== b[k]) (chg[k] = chg[k] || []).push(r);
    }
  }
  const skeys = Object.keys(fa.summary).filter(k => k in fb.summary);
  const rows = [], figs_s = [], figs_r = [], same = {};
  const box = (id, title, key, f1, f2, pdf, note, sel) =>
    `<div class="plot" id="${id}"><p>${title}</p>` +
    `<div class="cmp" data-auto="1" data-key="${key}" data-file="${f1}" data-other="${f2}" data-base="${esc(A)}" style="display:none">` +
    `<select class="tag"><option selected>${esc(B)}</option></select><select class="col"><option>red</option></select></div>` +
    '<div class="frame"><div class="main"><div class="jsdraw"></div><div class="cleg"></div></div><div class="jsratio"></div></div>' +
    `<a class="pdf" href="${pdf}" style="display:none"></a><p class="note">${note}${sel || ''}</p></div>`;
  for (const k of skeys.sort((x, y) => ord(sec(NAMES.summary, x)[0]) - ord(sec(NAMES.summary, y)[0]))) {
    const [s, t] = sec(NAMES.summary, k);
    if (fa.summary[k] !== fb.summary[k]) {
      rows.push(['Summary: ' + s, t, 's_' + k, 'all runs']);
      figs_s.push(box('s_' + k, s + ': ' + t, k, `${A}/plots/summary/summary.root`, `${B}/plots/summary/summary.root`,
                      `${A}/plots/summary/${k}_vs_run.pdf`, 'summary vs run number', ''));
    } else (same['Summary: ' + s] = same['Summary: ' + s] || []).push([t, k, 's']);
  }
  for (const k of [...keys].sort((x, y) => ord(sec(NAMES.run, x)[0]) - ord(sec(NAMES.run, y)[0]))) {
    const [s, t] = sec(NAMES.run, k);
    if (chg[k]) {
      const rs = chg[k], r0 = rs[0];
      rows.push([s, t, 'r_' + k, rs.length + ' / ' + runs.length]);
      const sel = ` &mdash; run <select class="runsel" data-key="${k}">` + rs.map(r => `<option>${r}</option>`).join('') + '</select>';
      figs_r.push(box('r_' + k, s + ': ' + t, k, `${A}/plots/runs/run${r0}.root`, `${B}/plots/runs/run${r0}.root`,
                      `${A}/plots/runs/run${r0}_${k}.pdf`, `changed in ${rs.length} of ${runs.length} runs`, sel));
    } else (same[s] = same[s] || []).push([t, k, 'r']);
  }
  const nsame = Object.values(same).reduce((n, v) => n + v.length, 0);
  let h = `<p class="note">${runs.length} runs QA'd in both tags.</p>` +
          `<div class="counts"><span><b class="ch">${rows.length}</b> plots changed</span><span><b class="same">${nsame}</b> plots unchanged</span>` +
          (miss ? `<span><b class="miss">${miss}</b> run plots in one tag only</span>` : '') +
          (onlyA.length || onlyB.length ? `<span><b class="miss">${onlyA.length + onlyB.length}</b> runs in one tag only</span>` : '') + '</div>';
  h += hcdb;
  h += '<h2>Changed plots</h2>' + (rows.length
    ? '<table class="cdb chg"><tr><th>section</th><th>plot</th><th>runs changed</th></tr>' +
      rows.map((x, i) => `<tr><td class="sec">${i && rows[i - 1][0] === x[0] ? '' : x[0]}</td><td><a href="#${x[2]}">${x[1]}</a></td>` +
                         `<td class="num">${x[3]}</td></tr>`).join('') + '</table>'
    : '<p>none: every plot is the same in both tags</p>');
  h += '<h2>Unchanged plots</h2><p class="note">click a plot name to draw it for both tags</p><div class="same">' +
       Object.entries(same).map(([s, ts]) => `<p><b>${s}</b> ` + ts.map(([t, k, kind]) =>
         `<a href="#" class="showplot" data-key="${k}" data-kind="${kind}">${t}</a>`).join(' &middot; ') + '</p>').join('') + '</div>' +
       '<div id="shownsec" style="display:none"><h2>Opened plots</h2><div class="plots" id="shown"></div></div>';
  if (figs_s.length) h += '<h2>Changed: summary vs run number</h2><div class="plots">' + figs_s.join('') + '</div>';
  if (figs_r.length) h += '<h2>Changed: per-run plots</h2><div class="plots">' + figs_r.join('') + '</div>';
  out.innerHTML = h;
  st.textContent = '';
  drawcdb();
  // draw the changed plots (as they come into view); the run menu of a plot redraws it for that run
  const go = () => {
    out.querySelectorAll('.cmp[data-auto]').forEach(c => window.cmpObserve(c.closest('.plot')));
    out.querySelectorAll('select.runsel').forEach(runmenu);
    out.querySelectorAll('a.showplot').forEach(a => a.addEventListener('click', ev => {
      ev.preventDefault();
      const k = a.dataset.key, kind = a.dataset.kind, id = 'o' + kind + '_' + k;
      let b = document.getElementById(id);
      if (!b) {
        const [s, t] = sec(kind === 's' ? NAMES.summary : NAMES.run, k), r0 = runs[0];
        document.getElementById('shown').insertAdjacentHTML('beforeend', kind === 's'
          ? box(id, s + ': ' + t, k, `${A}/plots/summary/summary.root`, `${B}/plots/summary/summary.root`,
                `${A}/plots/summary/${k}_vs_run.pdf`, 'same in both tags', '')
          : box(id, s + ': ' + t, k, `${A}/plots/runs/run${r0}.root`, `${B}/plots/runs/run${r0}.root`,
                `${A}/plots/runs/run${r0}_${k}.pdf`, 'same in both tags',
                ` &mdash; run <select class="runsel" data-key="${k}">` + runs.map(r => `<option>${r}</option>`).join('') + '</select>'));
        document.getElementById('shownsec').style.display = '';
        b = document.getElementById(id);
        const m = b.querySelector('select.runsel');
        if (m) runmenu(m);
        window.cmpRender(b);
      }
      b.scrollIntoView({ behavior: 'smooth', block: 'center' });
    }));
  };
  function runmenu(sel) {
    sel.addEventListener('change', () => {
      const b = sel.closest('.plot'), c = b.querySelector('.cmp'), r = sel.value, k = sel.dataset.key;
      c.dataset.file = `${A}/plots/runs/run${r}.root`; c.dataset.other = `${B}/plots/runs/run${r}.root`;
      b.querySelector('a.pdf').href = `${A}/plots/runs/run${r}_${k}.pdf`;
      window.cmpRender(b);
    });
  }
  if (window.cmpObserve) go(); else document.addEventListener('cmpready', go, { once: true });
})();
</script>
"""


def compare_page(webdir, period, ptitle):
    # one page per period for any two of its global tags: compare.html?a=<tag A>&b=<tag B>
    run = {}
    for sec, plots in RUN_SECTIONS + [("Centrality", [("cent", "Centrality, MB events"), ("cent_mbd_qsum", "MBD charge sum, MB events")])]:
        for k, t in plots:
            run[k] = [sec, t]
    summ = {k: [sec, t] for sec, plots in SUMMARY for k, t in plots}
    order = [s for s, _ in SUMMARY] + [s for s, _ in RUN_SECTIONS] + ["Centrality"]
    body = (f'<link rel="stylesheet" href="{SHARED}cdbqa_live.css"><h1>{LOGO}'
            f'CDB Calo QA &ndash; {html.escape(ptitle)} &ndash; <span id="cmptitle"></span></h1>'
            f'<div class="nav"><a href="index.html">&#8962; All global tags of {period}</a></div>'
            '<div id="cmpstatus"></div><div id="cmpout"></div><div class="cmp" style="display:none"></div>'
            f'<script>const PERIOD = {json.dumps(period)}, NAMES = {json.dumps({"run": run, "summary": summ})}, '
            f'ORDER = {json.dumps(order)};</script><script src="{SHARED}cdbqa_live.js"></script>')
    open(os.path.join(webdir, period, "compare.html"), "w").write(page(f"CDB Calo QA - {ptitle}", body, clicknote=False))


QABASE = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def read_scan(path):
    # empty_scan.txt of pass 2: run, all events, MB events (-1: no histogram file), empty plot keys
    scan = {}
    if os.path.exists(path):
        for l in open(path):
            w = l.split()
            if len(w) == 4:
                scan[int(w[0])] = (float(w[1]), float(w[2]), [] if w[3] == "-" else w[3].split(","))
    return scan


def read_notes(period):
    # notes/<period>.txt, written by hand: <run> <note>, lines with # are comments
    notes = {}
    path = os.path.join(QABASE, "notes", f"{period}.txt")
    if os.path.exists(path):
        for l in open(path):
            w = l.split(None, 1)
            if len(w) == 2 and w[0].isdigit():
                notes[int(w[0])] = notes.get(int(w[0]), "") + (" " if int(w[0]) in notes else "") + w[1].strip()
    return notes


def log_reason(period, gtag, r, empty):
    # why the run has no events (empty) or what the job saw (empty plots), from the QA job log
    path = os.path.join(QABASE, "condor/log", f"cdbcaloqa_{period}_{gtag}_{r}.out")
    if not os.path.exists(path):
        return "no QA job log"
    read = dropped = proc = skim = None
    noinput = done = False
    for l in open(path, errors="replace"):
        if l.startswith("CDBCaloQAEventSelect: ") and "events read" in l:
            w = l.split()
            read, dropped = int(w[1]), int(w[4])
        elif "CaloStatusSkimmer::End Total events processed:" in l:
            proc = int(l.split()[-1])
        elif "CaloStatusSkimmer::End Total events skimmed:" in l:
            skim = int(l.split()[-1])
        elif ": no DST_" in l and l.rstrip().endswith(" input"):
            noinput = True
        elif l.startswith("All done"):
            done = True
    skimmed = (f"{skim} of {proc} events removed by CaloStatusSkimmer (calorimeter packets missing)"
               if proc and skim else "")
    if not empty:
        return skimmed
    if noinput:
        return "no input DST"
    if read is not None and read == dropped:
        return f"no MB-triggered event among the {read} events read"
    if proc and skim == proc:
        return f"all {proc} events removed by CaloStatusSkimmer (calorimeter packets missing)"
    if not done:
        return "the QA job did not finish"
    return skimmed or "reason not found in the QA job log"


def read_hide(period):
    # payloads not shown on the pages, written by hand, one per line, lines with # are comments:
    # notes/cdb_hide.txt for every period, notes/cdb_hide_<period>.txt for one period; names or shell patterns
    hide = set()
    for name in ("cdb_hide.txt", f"cdb_hide_{period}.txt"):
        path = os.path.join(QABASE, "notes", name)
        if os.path.exists(path):
            hide |= {l.split()[0] for l in open(path) if l.split() and not l.startswith("#")}
    return hide


def hidden(d, hide):
    # hide list entries are names or shell patterns (e.g. JET_RHO_*)
    return d in hide or any(fnmatch.fnmatchcase(d, p) for p in hide)


def main():
    webdir, period, ptitle, runfiles, statusfile, snapdir, gtag = sys.argv[1:8]
    # ZDC plots: ZDC Coincidence events; heavy ions only without an MBD N&S trigger (single-neutron stream)
    zsel = "ZDC Coincidence" + ("" if "pp" in period else ", no MBD trigger")
    for lst in (RUN_SECTIONS, SUMMARY):
        lst[:] = [(s, [(k, t.replace("ZDC trigger", zsel)) for k, t in pl]) for s, pl in lst]
    hide = read_hide(period)
    write_shared(webdir)
    runs = sorted(int(l.split()[0]) for l in open(runfiles) if l.strip())
    nooutput = {int(l.split()[0]) for l in open(runfiles) if l.strip() and l.split()[1] == "-"}
    changed = {}
    chfile = statusfile.replace(".txt", "_changes.txt")
    if os.path.exists(chfile):
        for l in open(chfile):
            r, d, old, new = l.split()
            if not hidden(d, hide):
                changed[(int(r), d)] = old
    status = {}
    iovs = {}
    ins = {}  # (run, payload) -> date the entry was inserted in the tag
    for l in open(statusfile):
        w = l.split()
        r, d, s, lo, hi = w[:5]
        if hidden(d, hide):
            continue
        status.setdefault(int(r), {})[d] = s
        if len(w) > 5 and w[5] != "-":
            ins[(int(r), d)] = w[5].replace("T", " ")
        iovs[(int(r), d)] = "" if lo == "-" else f"{lo} &ndash; {'&infin;' if hi == 'inf' else int(hi) - 1}"
    pdir = os.path.join(webdir, period, gtag)
    # flags: runs without events, the reason and the notes
    scan = read_scan(os.path.join(os.path.dirname(runfiles), "empty_scan.txt"))
    notes = read_notes(period)
    emptyrun = {r for r, (na, nmb, _) in scan.items() if nmb == 0 and r not in nooutput}
    reason = {r: log_reason(period, gtag, r, True) for r in sorted(emptyrun)}
    # only plots that were drawn are put on the pages
    # list the plot folders once instead of checking every PDF on the shared disk
    def listing(d):
        return set(os.listdir(d)) if os.path.isdir(d) else set()
    present = ({f"plots/runs/{f}" for f in listing(os.path.join(pdir, "plots/runs"))}
               | {f"plots/summary/{f}" for f in listing(os.path.join(pdir, "plots/summary"))})
    have = lambda p: p in present
    os.makedirs(pdir, exist_ok=True)

    # run pages
    global PAGETAG
    PAGETAG = gtag
    # other global tags QA'd for this period: offered in the comparison menus
    others = sorted(t for t in os.listdir(os.path.join(webdir, period)) if t != gtag
                    and os.path.isdir(os.path.join(webdir, period, t, "plots")))
    otherruns = {t: listing(os.path.join(webdir, period, t, "plots/runs")) for t in others}
    cellfile = {}  # (run, payload) -> CDB file name, for the interactive map and the payload pages
    mc = period.startswith("MC_")  # simulation: one run, its page is also the tag page
    for i, r in enumerate(runs):
        nav = '<a href="index.html">&#8962; Run index</a>'
        if mc:
            nav = f'<a href="../../mc.html">&#8962; MC samples</a><a href="../index.html">All global tags of {period}</a>'
        if i > 0:
            nav += f'<a href="run_{runs[i - 1]}.html">&#8592; {runs[i - 1]}</a>'
        if i + 1 < len(runs):
            nav += f'<a href="run_{runs[i + 1]}.html">{runs[i + 1]} &#8594;</a>'
        body = f'<h1>{LOGO}Run {r} &ndash; {html.escape(ptitle)} &ndash; {html.escape(gtag)}</h1><div class="nav">{nav}</div>'
        body += '<div class="toc"><a href="#CDB">CDB</a>' + "".join(f'<a href="#{n.split()[0]}">{n}</a>' for n, _ in RUN_SECTIONS) + '</div>'
        sections = [] if r in nooutput else list(RUN_SECTIONS)
        if r in nooutput:
            body += ('<p style="color:#c62828;font-weight:bold">The QA job of this run failed: a calibration is missing in the '
                     f'global tag (see the CDB table). No plots. The table shows what {html.escape(gtag)} gives this run.</p>')
        if r in emptyrun:
            body += f'<p class="flag">No event reached the QA, all plots are empty: {html.escape(reason[r])}.</p>'
        if r in notes:
            body += f'<p class="qanote">Note: {html.escape(notes[r])}</p>'
        used = {}
        snap = os.path.join(snapdir, f"cdb_run{r}.json")
        if os.path.exists(snap):
            used = json.load(open(snap))
        body += f'<h2 id="CDB">CDB files used (global tag {html.escape(gtag)})</h2>{LEGEND}<br><table class="cdb"><tr><th>payload</th><th>status</th><th>runs of the entry</th><th>inserted</th><th>md5</th><th>file</th></tr>'
        for d, s in sorted(status.get(r, {}).items()):
            url = used.get(d) or used.get(d + "_default") or ""
            fn = os.path.basename(url)
            cellfile[(r, d)] = fn
            h = fn.split("_", 1)[0]
            hasmd5 = len(h) == 32 and all(c in "0123456789abcdef" for c in h)
            md5 = h if hasmd5 else ""
            fname = fn.split("_", 1)[1] if hasmd5 else fn
            upd = (r, d) in changed
            if upd:
                old = changed[(r, d)]
                md5 = f"{old.split('_', 1)[0]}<br>--&gt; {md5}"
                fname = f"{html.escape(old.split('_', 1)[-1])}<br>--&gt; {html.escape(fname)}"
            else:
                fname = html.escape(fname)
            body += (f'<tr><td>{d}</td><td class="{s}{" upd" if upd else ""}">{s}{" (updated)" if upd else ""}</td>'
                     f'<td>{iovs.get((r, d), "")}</td><td>{ins.get((r, d), "")}</td>'
                     f'<td style="font-family:monospace">{md5}</td><td>{fname}</td></tr>')
        body += "</table>"
        if os.path.exists(os.path.join(pdir, f"plots/runs/run{r}_cent.pdf")):
            sections.append(("Centrality", [("cent", "Centrality, MB events"), ("cent_mbd_qsum", "MBD charge sum, MB events")]))
        for name, plots in sections:
            if not any(have(f"plots/runs/run{r}_{p}.pdf") for p, _ in plots):
                continue  # e.g. no ZDC in the simulation
            body += f'<h2 id="{name.split()[0]}">{name}</h2>'
            cmptags = [t for t in others if f"run{r}.root" in otherruns[t]]
            body += '<div class="plots">' + "".join(plot(f"plots/runs/run{r}_{p}.pdf", t, objfile=f"plots/runs/run{r}.root",
                                                         tags=cmptags) for p, t in plots
                                                    if have(f"plots/runs/run{r}_{p}.pdf")) + '</div>'
        open(os.path.join(pdir, f"run_{r}.html"), "w").write(page(f"Run {r} - CDB Calo QA - {gtag}", body))
        if mc:
            open(os.path.join(pdir, "index.html"), "w").write(page(f"Run {r} - CDB Calo QA - {gtag}", body))

    # period index
    body = f'<h1>{LOGO}sPHENIX CDB Calo QA &ndash; {html.escape(ptitle)} &ndash; {html.escape(gtag)}</h1>'
    body += f'<div class="nav"><a href="../../index.html">&#8962; All periods</a><a href="../index.html">All global tags of {period}</a></div>'
    if mc:
        body += (f'<p class="note">Input DST_CALO_CLUSTER, DST_MBD_EPD and DST_GLOBAL (no pileup), global tag {html.escape(gtag)}, '
                 f'10000 events, all of them MB (no trigger). Click the run for its QA plots.</p>')
    else:
        body += (f'<p class="note">Input DST_CALOFITTING pro001_pcdb001_v001, global tag {html.escape(gtag)}, '
                 f'at least 10000 events and 3000 MB events per run. {len(runs)} runs. Click a run for its QA plots.</p>')
    body += '<h2>Runs</h2>' + LEGEND + '<br><div class="grid">'
    for r in runs:
        worst = max(status.get(r, {"x": "run"}).values(), key=lambda s: RANK.get(s, 0))
        cls = (worst if RANK.get(worst, 0) > 0 else "") + (" upd" if any(k[0] == r for k in changed) else "")
        if r in emptyrun:
            cls, tip = "empty", f' title="no events: {html.escape(reason[r])}"'
        else:
            tip = ""
        body += f'<a class="{cls.strip()}" href="run_{r}.html"{tip}>{r}</a>'
    body += ('</div><p class="note">Run box colour: worst CDB status of the run, see-through blue if a payload changed since the last QA, '
             'dark grey if no event reached the QA (reason on hover).</p>')
    flagged = sorted(emptyrun | (set(notes) & set(runs)))
    if flagged:
        body += ('<h2 id="empty">Empty runs and notes</h2><table class="cdb"><tr><th>run</th><th>MB events</th>'
                 '<th>reason</th><th>note</th></tr>')
        for r in flagged:
            nmb = scan[r][1] if r in scan else -1
            body += (f'<tr><td><a href="run_{r}.html">{r}</a></td><td>{int(nmb) if nmb >= 0 else ""}</td>'
                     f'<td>{html.escape(reason.get(r, ""))}</td><td>{html.escape(notes.get(r, ""))}</td></tr>')
        body += '</table><p class="note">Notes: ' + html.escape(os.path.join(QABASE, "notes", f"{period}.txt")) + ', one line per run: run number, then the note.</p>'
    pays = sorted({d for st in status.values() for d in st})
    body += '<h2 id="cdbmap">CDB files used per run</h2>' + cdb_map(runs, pays, status, iovs, ins, changed, cellfile)
    # simulation: one run, no summary vs run
    if not mc:
        body += ('<p class="note">MB: the MB trigger of the period, OR the MBD vtx &lt; 10 cm trigger (bit 12) in the runs where '
                 'the MB trigger is prescaled by more than 200. The trigger of each run is written on its plots.</p>')
    for name, plots in ([] if mc else SUMMARY):
        if not any(have(f"plots/summary/{p}_vs_run.pdf") for p, _ in plots):
            continue  # e.g. no centrality for p+p
        body += f'<h2>Summary vs run number: {name}</h2>'
        cmptags = [t for t in others if os.path.exists(os.path.join(webdir, period, t, "plots/summary/summary.root"))]
        # maps (2D, not in summary.root) have no overlay of another tag
        maptags = [t for t in others if os.path.exists(os.path.join(webdir, period, t, "plots/summary/zsmap.root"))]
        # maps (2D, in zsmap.root): compared as the ratio map of the two tags
        body += '<div class="plots">' + "".join(
            plot(f"plots/summary/{p}_vs_run.pdf", t,
                 objfile="plots/summary/zsmap.root" if p.endswith("_map") else "plots/summary/summary.root",
                 tags=(maptags if have("plots/summary/zsmap.root") else ()) if p.endswith("_map") else cmptags)
            for p, t in plots if have(f"plots/summary/{p}_vs_run.pdf")) + '</div>'
    # one page per payload: the files it uses and their runs
    for d in pays:
        open(os.path.join(pdir, f"payload_{d}.html"), "w").write(
            page(f"{d} - CDB Calo QA - {ptitle} - {gtag}", payload_page(d, runs, status, iovs, ins, changed, cellfile, ptitle, gtag)))
    if not mc:
        open(os.path.join(pdir, "index.html"), "w").write(page(f"CDB Calo QA - {ptitle} - {gtag}", body))

    period_page(webdir, period, ptitle)

    top_pages(webdir)
    print(f"wrote {len(runs)} run pages to {pdir}")


# simulation samples (period folder MC_*): name on the MC page
MCNAMES = {"MC_pp": "pp Pythia MB Run28", "MC_AuAu": "AuAu Hijing MB Run31", "MC_OO": "OO Hijing MB Run35"}


def top_pages(webdir):
    # top index: Data and MC; data.html: every data period, mc.html: every simulation sample (directories with an index.html)
    periods = sorted(d for d in os.listdir(webdir) if os.path.exists(os.path.join(webdir, d, "index.html")))
    data = [d for d in periods if not d.startswith("MC_")]
    mc = [d for d in periods if d.startswith("MC_")]

    def mclink(d):
        tags = [t for t in os.listdir(os.path.join(webdir, d)) if os.path.exists(os.path.join(webdir, d, t, "index.html"))]
        return f"{d}/{tags[0]}/index.html" if len(tags) == 1 else f"{d}/index.html"
    head = (f'<h1>{LOGO}'
            f'sPHENIX CDB Calo QA</h1><p class="note">Run-by-run calorimeter and MBD QA of the CDB calibrations.</p>')
    body = head + '<div class="grid"><a href="data.html">Data</a><a href="mc.html">MC</a></div>'
    open(os.path.join(webdir, "index.html"), "w").write(page("sPHENIX CDB Calo QA", body))
    body = (head + '<div class="nav"><a href="index.html">&#8962; Data and MC</a></div>'
            '<h2>Data</h2><div class="grid">' + "".join(f'<a href="{d}/index.html">{d}</a>' for d in data) + '</div>')
    open(os.path.join(webdir, "data.html"), "w").write(page("sPHENIX CDB Calo QA - Data", body))
    body = (head + '<div class="nav"><a href="index.html">&#8962; Data and MC</a></div>'
            '<h2>MC</h2><div class="grid">'
            + "".join(f'<a href="{mclink(d)}">{html.escape(MCNAMES.get(d, d))}</a>' for d in mc) + '</div>')
    open(os.path.join(webdir, "mc.html"), "w").write(page("sPHENIX CDB Calo QA - MC", body))


if __name__ == "__main__":
    main()
