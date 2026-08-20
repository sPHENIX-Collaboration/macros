#!/bin/bash
set -euo pipefail

# See TPC_PATTERN_V0_WORKFLOW.md for inputs, fitting modes, cuts, and outputs.

if [[ $# -lt 2 || $# -gt 4 ]]; then
  echo "Usage: $0 <campaign> <dst_filelist> [events_per_job] [max_jobs]"
  echo "Example: $0 tpc_v0_event_chunks output/dst.list 7 10000"
  exit 1
fi

campaign="$1"
dst_filelist="$2"
events_per_job="${3:-7}"
max_jobs="${4:-10000}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

if [[ "${dst_filelist}" != /* ]]; then
  dst_filelist="${SCRIPT_DIR}/${dst_filelist}"
fi
if [[ ! -f "${dst_filelist}" ]]; then
  echo "Error: DST file list not found: ${dst_filelist}" >&2
  exit 2
fi

manifest="${SCRIPT_DIR}/output/${campaign}/event_chunks.tsv"
python "${SCRIPT_DIR}/make_tpc_v0_event_chunks.py" \
  "${dst_filelist}" \
  "${manifest}" \
  --events-per-job "${events_per_job}" \
  --max-jobs "${max_jobs}"

n_jobs=$(wc -l < "${manifest}")
if [[ "${n_jobs}" -le 0 ]]; then
  echo "Error: event chunk manifest is empty: ${manifest}" >&2
  exit 2
fi

echo "Submitting ${n_jobs} non-overlapping event-chunk jobs"
V0_EVENT_CHUNK_MANIFEST="${manifest}" \
  "${SCRIPT_DIR}/submit_tpc_pattern_v0_campaign.sh" \
  "${campaign}" \
  "${dst_filelist}" \
  1 \
  100 \
  0
