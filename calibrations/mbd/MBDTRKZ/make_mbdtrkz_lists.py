#!/usr/bin/env python3
"""
make_mbdtrkz_lists.py <runnumber> [--nevents N] [--trkseg S] [--trkdst clus|tracks]
                      [--listdir DIR]

Writes clus.list or tracks.list (controlled by --trkdst), and fit.list covering
N events starting at block S (each block is N events).

  --nevents N          events per block (default 1000, must be a multiple of
                       EVT_PER_SEED=1000)
  --trkdst clus|tracks source DST type: 'clus' reads from dst_clus/<runnumber>.list
                       and writes clus.list; 'tracks' reads from dst_tracks/<runnumber>.list
                       and writes tracks.list (default: tracks)
  --listdir DIR        base directory containing dst_clus/, dst_tracks/, dst_fit/
                       (default: ~/sphenix/sphenix_bbc/run2025/lists/run3pp)

Example:
  --nevents 100000 --trkdst clus    ->  clus segs   0-99,  fit seg 0
  --nevents 100000 --trkdst tracks  ->  tracks segs 100-199, fit seg 1
  --nevents 200000                  ->  tracks segs 200-399, fit segs 2-3

Returns 0 on success, 1 if any expected DSTs are missing.
"""

import os
import re
import sys

EVT_PER_SEED =   1000   # events per track/clus/track DST file
EVT_PER_FIT  = 100000   # events per fit DST file
SEEDS_PER_FIT = EVT_PER_FIT // EVT_PER_SEED   # = 100


def get_seg(filename):
    """Return the 5-digit trailing segment number from a DST filename, or None."""
    m = re.search(r'-(\d{5})\.root', filename)
    return int(m.group(1)) if m else None


def lines_in_range(listfile, lo, hi):
    """Return stripped non-empty lines whose segment number is in [lo, hi]."""
    result = []
    with open(listfile) as fh:
        for line in fh:
            line = line.strip()
            if not line:
                continue
            seg = get_seg(line)
            if seg is not None and lo <= seg <= hi:
                result.append(line)
    return result


def parse_args():
    args = sys.argv[1:]
    runnumber  = None
    nevents    = EVT_PER_FIT   # default: one fit-file block
    trkseg     = 0
    trkdst     = 'tracks'      # default DST type
    listdir    = os.path.expanduser('~/sphenix/sphenix_bbc/run2025/lists/run3pp')
    i = 0
    while i < len(args):
        if args[i] in ('--nevents', '-nevents'):
            if i + 1 >= len(args):
                print('ERROR: --nevents requires an argument', file=sys.stderr)
                sys.exit(1)
            nevents = int(args[i + 1])
            i += 2
        elif args[i] in ('--trkseg', '-trkseg'):
            if i + 1 >= len(args):
                print('ERROR: --trkseg requires an argument', file=sys.stderr)
                sys.exit(1)
            trkseg = int(args[i + 1])
            i += 2
        elif args[i] in ('--trkdst', '-trkdst'):
            if i + 1 >= len(args):
                print('ERROR: --trkdst requires an argument', file=sys.stderr)
                sys.exit(1)
            trkdst = args[i + 1]
            if trkdst not in ('clus', 'tracks'):
                print(f'ERROR: --trkdst must be "clus" or "tracks", got "{trkdst}"',
                      file=sys.stderr)
                sys.exit(1)
            i += 2
        elif args[i] in ('--listdir', '-listdir'):
            if i + 1 >= len(args):
                print('ERROR: --listdir requires an argument', file=sys.stderr)
                sys.exit(1)
            listdir = os.path.expanduser(args[i + 1])
            i += 2
        elif not args[i].startswith('-'):
            if runnumber is not None:
                print('ERROR: unexpected argument', file=sys.stderr)
                sys.exit(1)
            runnumber = int(args[i])
            i += 1
        else:
            print(f'ERROR: unknown option {args[i]}', file=sys.stderr)
            sys.exit(1)

    if runnumber is None:
        print(f'Usage: {sys.argv[0]} <runnumber> [--nevents N] [--trkseg S] [--trkdst clus|tracks] [--listdir DIR]',
              file=sys.stderr)
        sys.exit(1)

    if nevents <= 0 or nevents % EVT_PER_SEED != 0:
        print(f'ERROR: --nevents must be a positive multiple of {EVT_PER_SEED}',
              file=sys.stderr)
        sys.exit(1)

    return runnumber, nevents, trkseg, trkdst, listdir


def main():
    runnumber, nevents, trkseg, trkdst, listdir = parse_args()

    tracks_per_block = nevents // EVT_PER_SEED
    fits_per_block  = (nevents + EVT_PER_FIT - 1) // EVT_PER_FIT  # ceiling div

    src_dir  = 'dst_clus' if trkdst == 'clus' else 'dst_tracks'
    out_name = 'clus.list' if trkdst == 'clus' else 'tracks.list'

    trk_list = os.path.join(listdir, src_dir, f'{runnumber}.list')
    fit_list = os.path.join(listdir, 'dst_fit', f'{runnumber}.list')

    for f in (trk_list, fit_list):
        if not os.path.isfile(f):
            print(f'ERROR: list file not found: {f}', file=sys.stderr)
            sys.exit(1)

    # segment starts
    trk_start = trkseg
    trk_end   = trk_start + tracks_per_block - 1
    fit_lo    = trk_start // SEEDS_PER_FIT
    fit_hi    = trk_end   // SEEDS_PER_FIT

    trk_lines = lines_in_range(trk_list, trk_start, trk_end)
    fit_lines = lines_in_range(fit_list, fit_lo,    fit_hi)

    trk_segs = len({get_seg(l) for l in trk_lines})
    fit_segs = len({get_seg(l) for l in fit_lines})

    rc = 0
    if trk_segs != tracks_per_block:
        print(f'ERROR: need {tracks_per_block} {trkdst} segments [{trk_start},{trk_end}], '
              f'found {trk_segs}', file=sys.stderr)
        rc = 1
    if fit_segs != fits_per_block:
        print(f'ERROR: need {fits_per_block} fit file(s) for segments [{fit_lo},{fit_hi}], '
              f'found {fit_segs}', file=sys.stderr)
        rc = 1
    if rc:
        sys.exit(1)

    with open(out_name, 'w') as f:
        f.write('\n'.join(trk_lines) + '\n')
    with open('fit.list', 'w') as f:
        f.write('\n'.join(fit_lines) + '\n')

    print(f'{out_name}: {len(trk_lines)} files ({trk_segs} unique segments)')
    print(f'fit.list:  {len(fit_lines)} files (segs {fit_lo}-{fit_hi})')


if __name__ == '__main__':
    main()
