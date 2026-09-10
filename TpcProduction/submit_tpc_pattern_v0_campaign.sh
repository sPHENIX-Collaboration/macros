#!/bin/bash
set -euo pipefail

# See TPC_PATTERN_V0_WORKFLOW.md for inputs, fitting modes, cuts, and outputs.

if [[ $# -lt 2 || $# -gt 5 ]]; then
  echo "Usage: $0 <campaign> <dst_filelist> [files_per_job] [events_per_input_file] [total_files]"
  echo "Example: $0 tpc_pattern_v0_79513_good /path/to/good_dst_files.list 1 500"
  exit 1
fi

campaign="$1"
dst_filelist="$2"
files_per_job="${3:-1}"
events_per_input_file="${4:-500}"
total_files="${5:-0}"

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

if [[ "${dst_filelist}" != /* ]]; then
  dst_filelist="${SCRIPT_DIR}/${dst_filelist}"
fi

if [[ ! -f "${dst_filelist}" ]]; then
  echo "Error: DST file list not found: ${dst_filelist}" >&2
  exit 2
fi

if [[ "${files_per_job}" -le 0 ]]; then
  echo "Error: files_per_job must be positive, got ${files_per_job}" >&2
  exit 2
fi

if [[ "${events_per_input_file}" -le 0 ]]; then
  echo "Error: events_per_input_file must be positive, got ${events_per_input_file}" >&2
  exit 2
fi

available_files=$(awk '
  /^[[:space:]]*($|#)/ { next }
  { ++count }
  END { print count + 0 }
' "${dst_filelist}")
if [[ "${total_files}" -le 0 || "${total_files}" -gt "${available_files}" ]]; then
  total_files=${available_files}
fi

if [[ "${total_files}" -le 0 ]]; then
  echo "Error: no files to process in ${dst_filelist}" >&2
  exit 2
fi

invalid_inputs=0
while IFS= read -r input_name; do
  if [[ "${input_name}" == /* && ! -f "${input_name}" ]]; then
    echo "Error: explicit DST path does not exist: ${input_name}" >&2
    invalid_inputs=$((invalid_inputs + 1))
  elif [[ "${input_name}" != /* && "${input_name}" != *://* && "${input_name}" == */* ]]; then
    echo "Error: ambiguous relative DST path: ${input_name}" >&2
    echo "Use a file-catalog logical name or an explicit absolute path/URI." >&2
    invalid_inputs=$((invalid_inputs + 1))
  fi
done < <(awk -v limit="${total_files}" '
  /^[[:space:]]*($|#)/ { next }
  {
    sub(/^[[:space:]]+/, "")
    sub(/[[:space:]]+$/, "")
    print
    if (++count >= limit) { exit }
  }
' "${dst_filelist}")
if [[ "${invalid_inputs}" -ne 0 ]]; then
  echo "Error: ${invalid_inputs} invalid DST entries in ${dst_filelist}" >&2
  exit 2
fi

event_chunk_manifest="${V0_EVENT_CHUNK_MANIFEST:-none}"
if [[ "${event_chunk_manifest}" != "none" && "${event_chunk_manifest}" != "NONE" ]]; then
  if [[ "${event_chunk_manifest}" != /* ]]; then
    event_chunk_manifest="${SCRIPT_DIR}/${event_chunk_manifest}"
  fi
  if [[ ! -f "${event_chunk_manifest}" ]]; then
    echo "Error: event chunk manifest not found: ${event_chunk_manifest}" >&2
    exit 2
  fi
  n_jobs=$(awk '
    /^[[:space:]]*($|#)/ { next }
    { ++count }
    END { print count + 0 }
  ' "${event_chunk_manifest}")
  if [[ "${n_jobs}" -le 0 ]]; then
    echo "Error: event chunk manifest is empty: ${event_chunk_manifest}" >&2
    exit 2
  fi
else
  event_chunk_manifest="none"
  n_jobs=$(((total_files + files_per_job - 1) / files_per_job))
fi
output_base_dir="${V0_OUTPUT_BASE_DIR:-${SCRIPT_DIR}/output}"
pre_track_pt_min="${V0_PRE_TRACK_PT_MIN:-0.05}"
pre_track_dca_xy_min="${V0_PRE_TRACK_DCA_XY_MIN:--1.0}"
pre_pair_dca_max="${V0_PRE_PAIR_DCA_MAX:-10.0}"
pre_lproj_min="${V0_PRE_LPROJ_MIN:--1.0}"
pre_cos_theta_min="${V0_PRE_COS_THETA_MIN:--2.0}"
pre_track_quality_max="${V0_PRE_TRACK_QUALITY_MAX:--1.0}"
pre_track_npoints_min="${V0_PRE_TRACK_NPOINTS_MIN:-0}"
pair_pca_z_max="${V0_PAIR_PCA_Z_MAX:--1.0}"
pair_pca_dz_max="${V0_PAIR_PCA_DZ_MAX:--1.0}"
pair_decay_radius_min="${V0_PAIR_DECAY_RADIUS_MIN:--1.0}"
pair_alpha_abs_max="${V0_PAIR_ALPHA_ABS_MAX:--1.0}"
pair_dca_max="${V0_PAIR_DCA_MAX:--1.0}"
pair_dira_min="${V0_PAIR_DIRA_MIN:--2.0}"
use_final_track_helix="${V0_USE_FINAL_TRACK_HELIX:-false}"
point_order="${V0_POINT_ORDER:-auto}"
fit_method="${V0_FIT_METHOD:-helix}"
kalman_sigma_rphi_cm="${V0_KALMAN_SIGMA_RPHI_CM:-0.03}"
kalman_sigma_r_cm="${V0_KALMAN_SIGMA_R_CM:-0.03}"
kalman_sigma_z_cm="${V0_KALMAN_SIGMA_Z_CM:-0.05}"
reconstruct_pairs="${V0_RECONSTRUCT_PAIRS:-true}"
write_same_sign_pairs="${V0_WRITE_SAME_SIGN_PAIRS:-false}"
write_cluster_residual_tree="${V0_WRITE_CLUSTER_RESIDUAL_TREE:-false}"
write_kalman_innovation_diagnostics="${V0_WRITE_KALMAN_INNOVATION_DIAGNOSTICS:-false}"
use_kalman_field_map="${V0_USE_KALMAN_FIELD_MAP:-true}"
kalman_field_map="${V0_KALMAN_FIELD_MAP:-none}"
kalman_uniform_propagator="${V0_KALMAN_UNIFORM_PROPAGATOR:-rk}"
kalman_rk_max_step_cm="${V0_KALMAN_RK_MAX_STEP_CM:-5.0}"
kalman_rk_step_tolerance="${V0_KALMAN_RK_STEP_TOLERANCE:-1.0e-4}"
kalman_rk_max_step_trials="${V0_KALMAN_RK_MAX_STEP_TRIALS:-12}"
kalman_rk_max_total_steps="${V0_KALMAN_RK_MAX_TOTAL_STEPS:-2000}"
kalman_fast_field_jacobian="${V0_KALMAN_FAST_FIELD_JACOBIAN:-true}"
kalman_fast_field_pca="${V0_KALMAN_FAST_FIELD_PCA:-true}"
kalman_field_pca_refine_iterations="${V0_KALMAN_FIELD_PCA_REFINE_ITERATIONS:-6}"
coarse_steps="${V0_COARSE_STEPS:-64}"
pca_candidates="${V0_PCA_CANDIDATES:-32}"
final_track_helix_max_upstream_cm="${V0_FINAL_TRACK_HELIX_MAX_UPSTREAM_CM:-80.0}"
final_track_helix_downstream_margin_cm="${V0_FINAL_TRACK_HELIX_DOWNSTREAM_MARGIN_CM:-5.0}"
print_timing="${V0_PRINT_TIMING:-false}"
primary_vertex_x="${V0_PRIMARY_VERTEX_X:-0.0}"
primary_vertex_y="${V0_PRIMARY_VERTEX_Y:-0.0}"
primary_vertex_z="${V0_PRIMARY_VERTEX_Z:-0.0}"
software_release="${V0_SOFTWARE_RELEASE:-new}"
local_install="${V0_LOCAL_INSTALL:-none}"
required_crossing="${V0_REQUIRED_CROSSING:-any}"
require_same_crossing="${V0_REQUIRE_SAME_CROSSING:-false}"
max_crossing_tier="${V0_MAX_CROSSING_TIER:--1}"
crossing_decision_node="${V0_CROSSING_DECISION_NODE:-TPC_CROSSING_DECISIONS}"
request_memory_mb="${V0_REQUEST_MEMORY_MB:-2048}"
retry_memory_increase_mb="${V0_RETRY_MEMORY_INCREASE_MB:-2048}"
retry_memory_max_mb="${V0_RETRY_MEMORY_MAX_MB:-8192}"

for memory_value in "${request_memory_mb}" "${retry_memory_increase_mb}" "${retry_memory_max_mb}"; do
  if [[ ! "${memory_value}" =~ ^[1-9][0-9]*$ ]]; then
    echo "Error: Condor memory settings must be positive integer MB values, got '${memory_value}'" >&2
    exit 2
  fi
done
if [[ "${retry_memory_max_mb}" -lt "${request_memory_mb}" ]]; then
  echo "Error: V0_RETRY_MEMORY_MAX_MB must be at least V0_REQUEST_MEMORY_MB" >&2
  exit 2
fi

case "${kalman_uniform_propagator}" in
  analytic|ANALYTIC|rk|RK)
    ;;
  *)
    echo "Error: V0_KALMAN_UNIFORM_PROPAGATOR must be 'analytic' or 'rk', got '${kalman_uniform_propagator}'" >&2
    exit 2
    ;;
esac

case "${required_crossing}" in
  any|ANY|none|NONE)
    ;;
  *)
    if [[ ! "${required_crossing}" =~ ^-?[0-9]+$ ]]; then
      echo "Error: V0_REQUIRED_CROSSING must be an integer or 'any', got '${required_crossing}'" >&2
      exit 2
    fi
    ;;
esac
if [[ ! "${max_crossing_tier}" =~ ^-?[0-9]+$ ]]; then
  echo "Error: V0_MAX_CROSSING_TIER must be an integer, got '${max_crossing_tier}'" >&2
  exit 2
fi

mkdir -p "log/${campaign}" "log/tmp/${campaign}"
mkdir -p "${output_base_dir}/${campaign}" "${output_base_dir}/${campaign}/completed"
chmod +x "${SCRIPT_DIR}/run_tpc_pattern_v0.sh"

echo "Submitting TPC pattern-reco V0 campaign:"
echo "  campaign=${campaign}"
echo "  dst_filelist=${dst_filelist}"
echo "  total_files=${total_files}"
echo "  files/job=${files_per_job}"
echo "  events/input file=${events_per_input_file}"
echo "  event_chunk_manifest=${event_chunk_manifest}"
echo "  n_jobs=${n_jobs}"
echo "  memory=${request_memory_mb} MB, retry +${retry_memory_increase_mb} MB up to ${retry_memory_max_mb} MB"
echo "  output=${output_base_dir}/${campaign}/completed"
echo "  preselection: pt>${pre_track_pt_min}, dca_xy_min=${pre_track_dca_xy_min}, pairDCA<${pre_pair_dca_max}, Lproj>${pre_lproj_min}, cosTheta>${pre_cos_theta_min}"
echo "  track selection: quality<${pre_track_quality_max}, npoints>=${pre_track_npoints_min}"
echo "  exact pair selection: |pca_z|<${pair_pca_z_max}, |pca1_z-pca2_z|<${pair_pca_dz_max}, decay_radius>${pair_decay_radius_min}, |alpha|<${pair_alpha_abs_max}, pairDCA<${pair_dca_max}, DIRA>${pair_dira_min}"
echo "  use_final_track_helix=${use_final_track_helix}"
echo "  point_order=${point_order}"
echo "  fit_method=${fit_method}"
echo "  kalman measurement sigmas: rphi=${kalman_sigma_rphi_cm} cm, r=${kalman_sigma_r_cm} cm, z=${kalman_sigma_z_cm} cm"
echo "  use_kalman_field_map=${use_kalman_field_map}"
echo "  kalman_field_map=${kalman_field_map}"
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

condor_submit \
  -append "run_dir = ${SCRIPT_DIR}" \
  -append "campaign = ${campaign}" \
  -append "input_dst_filelist = ${dst_filelist}" \
  -append "files_per_job = ${files_per_job}" \
  -append "events_per_input_file = ${events_per_input_file}" \
  -append "total_files = ${total_files}" \
  -append "n_jobs = ${n_jobs}" \
  -append "output_base_dir = ${output_base_dir}" \
  -append "memory_initial_mb = ${request_memory_mb}" \
  -append "memory_retry_increment_mb = ${retry_memory_increase_mb}" \
  -append "memory_retry_max_mb = ${retry_memory_max_mb}" \
  -append "pre_track_pt_min = ${pre_track_pt_min}" \
  -append "pre_track_dca_xy_min = ${pre_track_dca_xy_min}" \
  -append "pre_pair_dca_max = ${pre_pair_dca_max}" \
  -append "pre_lproj_min = ${pre_lproj_min}" \
  -append "pre_cos_theta_min = ${pre_cos_theta_min}" \
  -append "pre_track_quality_max = ${pre_track_quality_max}" \
  -append "pre_track_npoints_min = ${pre_track_npoints_min}" \
  -append "pair_pca_z_max = ${pair_pca_z_max}" \
  -append "pair_pca_dz_max = ${pair_pca_dz_max}" \
  -append "pair_decay_radius_min = ${pair_decay_radius_min}" \
  -append "pair_alpha_abs_max = ${pair_alpha_abs_max}" \
  -append "pair_dca_max = ${pair_dca_max}" \
  -append "pair_dira_min = ${pair_dira_min}" \
  -append "use_final_track_helix = ${use_final_track_helix}" \
  -append "point_order = ${point_order}" \
  -append "fit_method = ${fit_method}" \
  -append "kalman_sigma_rphi_cm = ${kalman_sigma_rphi_cm}" \
  -append "kalman_sigma_r_cm = ${kalman_sigma_r_cm}" \
  -append "kalman_sigma_z_cm = ${kalman_sigma_z_cm}" \
  -append "use_kalman_field_map = ${use_kalman_field_map}" \
  -append "kalman_field_map = ${kalman_field_map}" \
  -append "kalman_uniform_propagator = ${kalman_uniform_propagator}" \
  -append "kalman_rk_max_step_cm = ${kalman_rk_max_step_cm}" \
  -append "kalman_rk_step_tolerance = ${kalman_rk_step_tolerance}" \
  -append "kalman_rk_max_step_trials = ${kalman_rk_max_step_trials}" \
  -append "kalman_rk_max_total_steps = ${kalman_rk_max_total_steps}" \
  -append "kalman_fast_field_jacobian = ${kalman_fast_field_jacobian}" \
  -append "kalman_fast_field_pca = ${kalman_fast_field_pca}" \
  -append "kalman_field_pca_refine_iterations = ${kalman_field_pca_refine_iterations}" \
  -append "coarse_steps = ${coarse_steps}" \
  -append "pca_candidates = ${pca_candidates}" \
  -append "final_track_helix_max_upstream_cm = ${final_track_helix_max_upstream_cm}" \
  -append "final_track_helix_downstream_margin_cm = ${final_track_helix_downstream_margin_cm}" \
  -append "print_timing = ${print_timing}" \
  -append "primary_vertex_x = ${primary_vertex_x}" \
  -append "primary_vertex_y = ${primary_vertex_y}" \
  -append "primary_vertex_z = ${primary_vertex_z}" \
  -append "software_release = ${software_release}" \
  -append "local_install = ${local_install}" \
  -append "required_crossing = ${required_crossing}" \
  -append "require_same_crossing = ${require_same_crossing}" \
  -append "max_crossing_tier = ${max_crossing_tier}" \
  -append "crossing_decision_node = ${crossing_decision_node}" \
  -append "event_chunk_manifest = ${event_chunk_manifest}" \
  -append "reconstruct_pairs = ${reconstruct_pairs}" \
  -append "write_same_sign_pairs = ${write_same_sign_pairs}" \
  -append "write_cluster_residual_tree = ${write_cluster_residual_tree}" \
  -append "write_kalman_innovation_diagnostics = ${write_kalman_innovation_diagnostics}" \
  tpc_pattern_v0.job
