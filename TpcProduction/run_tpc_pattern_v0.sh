#!/bin/bash
set -eo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

export USER="$(id -u -n)"
export LOGNAME=${USER}
export HOME=/sphenix/u/${LOGNAME}

software_release=${V0_SOFTWARE_RELEASE:-new}
source /opt/sphenix/core/bin/sphenix_setup.sh -n "${software_release}"
local_install=${V0_LOCAL_INSTALL:-none}
if [[ "${local_install}" != "none" && "${local_install}" != "NONE" ]]; then
  source /opt/sphenix/core/bin/setup_local.sh "${local_install}"
fi
set -u

process_id=${1?Error: process ID is not given}
files_per_job=${2:-1}
events_per_input_file=${3:-500}
total_files=${4:-0}

campaign_tag=${CAMPAIGN_TAG:-tpc_pattern_v0}
output_base_dir=${OUTPUT_BASE_DIR:-${SCRIPT_DIR}/output}
input_dst_filelist=${INPUT_DST_FILELIST:?Error: INPUT_DST_FILELIST is not set}
completed_dir=${COMPLETED_OUTPUT_DIR:-${output_base_dir}/${campaign_tag}/completed}
pre_track_pt_min=${V0_PRE_TRACK_PT_MIN:-0.05}
pre_track_dca_xy_min=${V0_PRE_TRACK_DCA_XY_MIN:--1.0}
pre_pair_dca_max=${V0_PRE_PAIR_DCA_MAX:-10.0}
pre_lproj_min=${V0_PRE_LPROJ_MIN:--1.0}
pre_cos_theta_min=${V0_PRE_COS_THETA_MIN:--2.0}
pre_track_quality_max=${V0_PRE_TRACK_QUALITY_MAX:--1.0}
pre_track_npoints_min=${V0_PRE_TRACK_NPOINTS_MIN:-0}
pair_pca_z_max=${V0_PAIR_PCA_Z_MAX:--1.0}
pair_pca_dz_max=${V0_PAIR_PCA_DZ_MAX:--1.0}
pair_decay_radius_min=${V0_PAIR_DECAY_RADIUS_MIN:--1.0}
pair_alpha_abs_max=${V0_PAIR_ALPHA_ABS_MAX:--1.0}
pair_dca_max=${V0_PAIR_DCA_MAX:--1.0}
pair_dira_min=${V0_PAIR_DIRA_MIN:--2.0}
use_final_track_helix=${V0_USE_FINAL_TRACK_HELIX:-false}
point_order=${V0_POINT_ORDER:-auto}
fit_method=${V0_FIT_METHOD:-helix}
kalman_sigma_rphi_cm=${V0_KALMAN_SIGMA_RPHI_CM:-0.03}
kalman_sigma_r_cm=${V0_KALMAN_SIGMA_R_CM:-0.03}
kalman_sigma_z_cm=${V0_KALMAN_SIGMA_Z_CM:-0.05}
reconstruct_pairs=${V0_RECONSTRUCT_PAIRS:-true}
write_same_sign_pairs=${V0_WRITE_SAME_SIGN_PAIRS:-false}
write_cluster_residual_tree=${V0_WRITE_CLUSTER_RESIDUAL_TREE:-false}
write_kalman_innovation_diagnostics=${5:-${V0_WRITE_KALMAN_INNOVATION_DIAGNOSTICS:-false}}
use_kalman_field_map=${V0_USE_KALMAN_FIELD_MAP:-true}
kalman_field_map=${V0_KALMAN_FIELD_MAP:-none}
kalman_uniform_propagator=${V0_KALMAN_UNIFORM_PROPAGATOR:-rk}
kalman_rk_max_step_cm=${V0_KALMAN_RK_MAX_STEP_CM:-5.0}
kalman_rk_step_tolerance=${V0_KALMAN_RK_STEP_TOLERANCE:-1.0e-4}
kalman_rk_max_step_trials=${V0_KALMAN_RK_MAX_STEP_TRIALS:-12}
kalman_rk_max_total_steps=${V0_KALMAN_RK_MAX_TOTAL_STEPS:-2000}
kalman_fast_field_jacobian=${V0_KALMAN_FAST_FIELD_JACOBIAN:-true}
kalman_fast_field_pca=${V0_KALMAN_FAST_FIELD_PCA:-true}
kalman_field_pca_refine_iterations=${V0_KALMAN_FIELD_PCA_REFINE_ITERATIONS:-6}
coarse_steps=${V0_COARSE_STEPS:-64}
pca_candidates=${V0_PCA_CANDIDATES:-32}
final_track_helix_max_upstream_cm=${V0_FINAL_TRACK_HELIX_MAX_UPSTREAM_CM:-80.0}
final_track_helix_downstream_margin_cm=${V0_FINAL_TRACK_HELIX_DOWNSTREAM_MARGIN_CM:-5.0}
print_timing=${V0_PRINT_TIMING:-false}
primary_vertex_x=${V0_PRIMARY_VERTEX_X:-0.0}
primary_vertex_y=${V0_PRIMARY_VERTEX_Y:-0.0}
primary_vertex_z=${V0_PRIMARY_VERTEX_Z:-0.0}
required_crossing=${V0_REQUIRED_CROSSING:-any}
require_same_crossing=${V0_REQUIRE_SAME_CROSSING:-false}
max_crossing_tier=${V0_MAX_CROSSING_TIER:--1}
crossing_decision_node=${V0_CROSSING_DECISION_NODE:-TPC_CROSSING_DECISIONS}
event_chunk_manifest=${V0_EVENT_CHUNK_MANIFEST:-}

if [[ "${kalman_field_map}" == "none" || "${kalman_field_map}" == "NONE" ]]; then
  kalman_field_map=""
fi
if [[ "${event_chunk_manifest}" == "none" || "${event_chunk_manifest}" == "NONE" ]]; then
  event_chunk_manifest=""
fi
case "${kalman_uniform_propagator}" in
  analytic|ANALYTIC)
    kalman_analytic_uniform=true
    ;;
  rk|RK)
    kalman_analytic_uniform=false
    ;;
  *)
    echo "Error: V0_KALMAN_UNIFORM_PROPAGATOR must be 'analytic' or 'rk', got '${kalman_uniform_propagator}'" >&2
    exit 2
    ;;
esac

case "${required_crossing}" in
  any|ANY|none|NONE)
    required_crossing_value=32767
    ;;
  *)
    if [[ ! "${required_crossing}" =~ ^-?[0-9]+$ ]]; then
      echo "Error: V0_REQUIRED_CROSSING must be an integer or 'any', got '${required_crossing}'" >&2
      exit 2
    fi
    required_crossing_value=${required_crossing}
    ;;
esac
if [[ ! "${max_crossing_tier}" =~ ^-?[0-9]+$ ]]; then
  echo "Error: V0_MAX_CROSSING_TIER must be an integer, got '${max_crossing_tier}'" >&2
  exit 2
fi

campaign_tag="${campaign_tag%%;*}"

cd "${SCRIPT_DIR}"

if [[ ! "${process_id}" =~ ^[0-9]+$ ]]; then
  echo "Error: process ID must be a decimal integer, got ${process_id}" >&2
  exit 2
fi
process_id=$((10#${process_id}))

if [[ ! "${files_per_job}" =~ ^[0-9]+$ ]]; then
  echo "Error: files_per_job must be a decimal integer, got ${files_per_job}" >&2
  exit 2
fi
files_per_job=$((10#${files_per_job}))
if ((files_per_job <= 0)); then
  echo "Error: files_per_job must be positive, got ${files_per_job}" >&2
  exit 2
fi

if [[ ! "${events_per_input_file}" =~ ^[0-9]+$ ]]; then
  echo "Error: events_per_input_file must be a decimal integer, got ${events_per_input_file}" >&2
  exit 2
fi
events_per_input_file=$((10#${events_per_input_file}))
if ((events_per_input_file <= 0)); then
  echo "Error: events_per_input_file must be positive, got ${events_per_input_file}" >&2
  exit 2
fi

if [[ ! "${total_files}" =~ ^[0-9]+$ ]]; then
  echo "Error: total_files must be a decimal integer, got ${total_files}" >&2
  exit 2
fi
total_files=$((10#${total_files}))

if [[ ! -f "${input_dst_filelist}" ]]; then
  echo "Error: DST file list not found: ${input_dst_filelist}" >&2
  exit 2
fi

padded_id=$(printf "%05d" "${process_id}")
job_outdir="${output_base_dir}/${campaign_tag}"
mkdir -p "${job_outdir}" "${completed_dir}" "${completed_dir}/.locks"
# Hold the job lock before writing its chunk list or ROOT output, including
# while validating an existing result. Concurrent retries share this lock.
if ! command -v flock >/dev/null 2>&1; then
  echo "Error: flock is required for idempotent V0 output publication" >&2
  exit 3
fi
exec {output_lock_fd}> "${completed_dir}/.locks/${padded_id}.lock"
flock "${output_lock_fd}"

require_exact_events=false
event_skip=0
file_start=0
if [[ -n "${event_chunk_manifest}" ]]; then
  if [[ ! -f "${event_chunk_manifest}" ]]; then
    echo "Error: event chunk manifest not found: ${event_chunk_manifest}" >&2
    exit 2
  fi
  manifest_row=$(awk -v wanted="$((process_id + 1))" '
    /^[[:space:]]*($|#)/ { next }
    { ++row }
    row == wanted { print; exit }
  ' "${event_chunk_manifest}")
  if [[ -z "${manifest_row}" ]]; then
    echo "Nothing to do: process=${process_id} is outside ${event_chunk_manifest}"
    exit 0
  fi
  IFS=$'\t' read -r file_start input_file event_skip nevents extra_field <<< "${manifest_row}"
  if [[ -z "${input_file}" || -z "${event_skip}" || -z "${nevents}" || -n "${extra_field}" ||
        ! "${file_start}" =~ ^[0-9]+$ || ! "${event_skip}" =~ ^[0-9]+$ ||
        ! "${nevents}" =~ ^[1-9][0-9]*$ ]]; then
    echo "Error: malformed event chunk row: ${manifest_row}" >&2
    exit 2
  fi
  event_skip=$((10#${event_skip}))
  file_start=$((10#${file_start}))
  require_exact_events=true
  nfiles=1
  chunk_list="${job_outdir}/dst_chunk_${padded_id}_fidx${file_start}_eskip${event_skip}_nev${nevents}.list"
  printf '%s\n' "${input_file}" > "${chunk_list}"
else
  available_files=$(awk '
    /^[[:space:]]*($|#)/ { next }
    { ++count }
    END { print count + 0 }
  ' "${input_dst_filelist}")
  if ((total_files <= 0 || total_files > available_files)); then
    total_files=${available_files}
  fi

  file_start=$((process_id * files_per_job))
  remaining=$((total_files - file_start))
  if [[ "${remaining}" -le 0 ]]; then
    echo "Nothing to do: process=${process_id} file_start=${file_start} total_files=${total_files}"
    exit 0
  fi

  nfiles=${files_per_job}
  if [[ "${remaining}" -lt "${nfiles}" ]]; then
    nfiles=${remaining}
  fi
  nevents=$((nfiles * events_per_input_file))
  chunk_list="${job_outdir}/dst_chunk_${padded_id}_fskip${file_start}_nfiles${nfiles}.list"
  awk -v first="$((file_start + 1))" -v last="$((file_start + nfiles))" '
    /^[[:space:]]*($|#)/ { next }
    {
      sub(/^[[:space:]]+/, "")
      sub(/[[:space:]]+$/, "")
      ++entry
    }
    entry >= first && entry <= last { print }
    entry > last { exit }
  ' "${input_dst_filelist}" > "${chunk_list}"
fi

if [[ "$(wc -l < "${chunk_list}")" -ne "${nfiles}" ]]; then
  echo "Error: chunk list has $(wc -l < "${chunk_list}") files, expected ${nfiles}" >&2
  exit 2
fi

macro_input="${chunk_list}"
if [[ -n "${event_chunk_manifest}" ]]; then
  # Fun4AllServer::skip requires an already-open input manager. AddListFile
  # opens lazily, so event chunks pass their single DST directly to fileopen.
  macro_input=$(head -n 1 "${chunk_list}")
fi

missing=0
while IFS= read -r dst_file; do
  if [[ "${dst_file}" == /* && ! -f "${dst_file}" ]]; then
    echo "Error: missing DST input file: ${dst_file}" >&2
    missing=$((missing + 1))
  elif [[ "${dst_file}" != /* && "${dst_file}" != *://* && "${dst_file}" == */* ]]; then
    echo "Error: ambiguous relative DST path: ${dst_file}" >&2
    echo "Use a file-catalog logical name or an explicit absolute path/URI." >&2
    missing=$((missing + 1))
  fi
done < "${chunk_list}"
if [[ "${missing}" -ne 0 ]]; then
  echo "Error: ${missing} missing DST input files in ${chunk_list}" >&2
  exit 2
fi

if [[ -n "${event_chunk_manifest}" ]]; then
  outroot="${job_outdir}/tpc_pattern_v0_${padded_id}_fidx${file_start}_eskip${event_skip}_nev${nevents}.root"
else
  outroot="${job_outdir}/tpc_pattern_v0_${padded_id}_fskip${file_start}_nfiles${nfiles}.root"
fi
# ROOT evaluates C++, so shell quoting alone is not sufficient. Accept only
# scalar literals and encode strings before building either ROOT expression.
float_pattern='^[+-]?([0-9]+([.][0-9]*)?|[.][0-9]+)([eE][+-]?[0-9]+)?$'
for name in pre_track_pt_min pre_track_dca_xy_min pre_pair_dca_max pre_lproj_min \
  pre_cos_theta_min pre_track_quality_max pair_pca_z_max pair_pca_dz_max \
  pair_decay_radius_min pair_alpha_abs_max pair_dca_max pair_dira_min \
  kalman_sigma_rphi_cm kalman_sigma_r_cm kalman_sigma_z_cm kalman_rk_max_step_cm \
  kalman_rk_step_tolerance final_track_helix_max_upstream_cm \
  final_track_helix_downstream_margin_cm primary_vertex_x primary_vertex_y primary_vertex_z
do
  value=${!name}
  if [[ ! "${value}" =~ ${float_pattern} ]]; then
    echo "Error: ${name} must be a numeric literal, got '${value}'" >&2
    exit 2
  fi
  # Ensure e.g. 08 is a decimal floating-point literal, not a C++ octal integer.
  if [[ "${value}" != *[.eE]* ]]; then
    printf -v "${name}" '%s.0' "${value}"
  fi
done
for name in nevents event_skip pre_track_npoints_min kalman_rk_max_step_trials \
  kalman_rk_max_total_steps kalman_field_pca_refine_iterations coarse_steps \
  pca_candidates required_crossing_value max_crossing_tier
do
  value=${!name}
  if [[ ! "${value}" =~ ^[+-]?(0|[1-9][0-9]*)$ || ${#value} -gt 11 ]]; then
    echo "Error: ${name} must be a decimal integer without leading zeros, got '${value}'" >&2
    exit 2
  fi
  if ((value < -2147483648 || value > 2147483647)); then
    echo "Error: ${name} is outside the C++ int range: ${value}" >&2
    exit 2
  fi
done
for name in use_final_track_helix reconstruct_pairs write_same_sign_pairs \
  write_cluster_residual_tree write_kalman_innovation_diagnostics use_kalman_field_map \
  kalman_fast_field_jacobian kalman_fast_field_pca print_timing require_same_crossing \
  kalman_analytic_uniform require_exact_events
do
  value=${!name}
  case "${value,,}" in
    true|1) printf -v "${name}" '%s' true ;;
    false|0) printf -v "${name}" '%s' false ;;
    *) echo "Error: ${name} must be true, false, 1, or 0, got '${value}'" >&2; exit 2 ;;
  esac
done
cpp_string()
{
  local value=$1
  value=${value//\\/\\\\}
  value=${value//\"/\\\"}
  value=${value//$'\n'/\\n}
  value=${value//$'\r'/\\r}
  value=${value//$'\t'/\\t}
  printf '"%s"' "${value}"
}

macro_call="Fun4All_TpcPatternRecoV0(${nevents}, $(cpp_string "${macro_input}"), $(cpp_string "${outroot}"), ${pre_track_pt_min}, ${pre_track_dca_xy_min}, ${pre_pair_dca_max}, ${pre_lproj_min}, ${pre_cos_theta_min}, ${use_final_track_helix}, $(cpp_string "${point_order}"), $(cpp_string "${fit_method}"), ${kalman_sigma_rphi_cm}, ${kalman_sigma_r_cm}, ${kalman_sigma_z_cm}, ${write_same_sign_pairs}, ${write_cluster_residual_tree}, ${use_kalman_field_map}, $(cpp_string "${kalman_field_map}"), ${kalman_rk_max_step_cm}, ${kalman_rk_step_tolerance}, ${kalman_rk_max_step_trials}, ${kalman_rk_max_total_steps}, ${kalman_fast_field_jacobian}, ${kalman_fast_field_pca}, ${kalman_field_pca_refine_iterations}, ${coarse_steps}, ${pca_candidates}, ${print_timing}, ${event_skip}, ${kalman_analytic_uniform}, ${pre_track_quality_max}, ${pre_track_npoints_min}, ${pair_pca_z_max}, ${pair_pca_dz_max}, ${pair_decay_radius_min}, ${pair_alpha_abs_max}, ${pair_dca_max}, ${pair_dira_min}, ${final_track_helix_max_upstream_cm}, ${final_track_helix_downstream_margin_cm}, ${write_kalman_innovation_diagnostics}, ${primary_vertex_x}, ${primary_vertex_y}, ${primary_vertex_z}, ${reconstruct_pairs}, ${required_crossing_value}, ${require_same_crossing}, ${max_crossing_tier}, $(cpp_string "${crossing_decision_node}"), ${require_exact_events})"
canonical_output="${completed_dir}/$(basename "${outroot}")"
validate_output()
{
  root.exe -l -b -q -e '.L Fun4All_TpcPatternRecoV0.C' \
    -e "gSystem->Exit(TpcPatternV0Output::validate($(cpp_string "$1"), ${nevents}, ${require_exact_events}) ? 0 : 3);"
}
if [[ -e "${canonical_output}" ]]; then
  if validate_output "${canonical_output}"; then
    echo "Validated completed output already exists; nothing to do: ${canonical_output}"
    exit 0
  fi
  echo "Error: existing output failed validation: ${canonical_output}" >&2
  echo "Inspect it and move it aside before retrying, or use a new campaign name." >&2
  exit 3
fi
completion_marker="${outroot}.complete"
rm -f "${completion_marker}"

echo "Running TPC pattern-reco V0 candidate tree:"
echo "  campaign=${campaign_tag}"
echo "  job=${process_id}"
echo "  input_dst_filelist=${input_dst_filelist}"
echo "  chunk_list=${chunk_list}"
echo "  macro_input=${macro_input}"
echo "  file_start=${file_start}"
echo "  event_skip=${event_skip}"
echo "  nfiles=${nfiles}"
echo "  events/input file=${events_per_input_file}"
echo "  nevents=${nevents}"
echo "  out=${outroot}"
echo "  preselection: pt>${pre_track_pt_min}, dca_xy_min=${pre_track_dca_xy_min}, pairDCA<${pre_pair_dca_max}, Lproj>${pre_lproj_min}, cosTheta>${pre_cos_theta_min}"
echo "  track selection: quality<${pre_track_quality_max}, npoints>=${pre_track_npoints_min}"
echo "  exact pair selection: |pca_z|<${pair_pca_z_max}, |pca1_z-pca2_z|<${pair_pca_dz_max}, decay_radius>${pair_decay_radius_min}, |alpha|<${pair_alpha_abs_max}, pairDCA<${pair_dca_max}, DIRA>${pair_dira_min}"
echo "  use_final_track_helix=${use_final_track_helix}"
echo "  point_order=${point_order}"
echo "  fit_method=${fit_method}"
echo "  kalman measurement sigmas: rphi=${kalman_sigma_rphi_cm} cm, r=${kalman_sigma_r_cm} cm, z=${kalman_sigma_z_cm} cm"
echo "  use_kalman_field_map=${use_kalman_field_map}"
echo "  kalman_field_map=${kalman_field_map:-<DST/default/uniform>}"
echo "  kalman_uniform_propagator=${kalman_uniform_propagator}"
echo "  kalman RK: max_step=${kalman_rk_max_step_cm} cm, tolerance=${kalman_rk_step_tolerance}, max_trials=${kalman_rk_max_step_trials}, max_total_steps=${kalman_rk_max_total_steps}"
echo "  kalman_fast_field_jacobian=${kalman_fast_field_jacobian}"
echo "  kalman_fast_field_pca=${kalman_fast_field_pca}"
echo "  kalman_field_pca_refine_iterations=${kalman_field_pca_refine_iterations}"
echo "  PCA search: coarse_steps=${coarse_steps}, candidates=${pca_candidates}"
echo "  FinalTrack helix search: measurement-anchored, upstream=${final_track_helix_max_upstream_cm} cm, downstream_margin=${final_track_helix_downstream_margin_cm} cm, span<1 turn"
echo "  print_timing=${print_timing}"
echo "  fixed primary vertex=(${primary_vertex_x}, ${primary_vertex_y}, ${primary_vertex_z}) cm"
echo "  software release=${software_release}, local install=${local_install}"
echo "  crossing selection: node=${crossing_decision_node}, required=${required_crossing}, same_pair=${require_same_crossing}, max_tier=${max_crossing_tier}"
echo "  reconstruct_pairs=${reconstruct_pairs}"
echo "  write_same_sign_pairs=${write_same_sign_pairs}"
echo "  write_cluster_residual_tree=${write_cluster_residual_tree}"
echo "  write_kalman_innovation_diagnostics=${write_kalman_innovation_diagnostics}"

# Load separately so ROOT's macro filename parser never sees argument strings.
root.exe -l -b -q -e '.L Fun4All_TpcPatternRecoV0.C' -e "${macro_call};"

if [[ -f "${outroot}" && -f "${completion_marker}" ]]; then
  mv -- "${outroot}" "${canonical_output}"
  rm -f "${completion_marker}"
  echo "Moved completed file to ${canonical_output}"
else
  echo "Error: run did not produce both ${outroot} and its completion marker" >&2
  exit 3
fi

echo "Done"
