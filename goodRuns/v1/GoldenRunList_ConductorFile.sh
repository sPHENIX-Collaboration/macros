#!/bin/bash

##############################################################################################################################################################################
# Golden Run List Generation Script
#
# Purpose:
#   This script compiles a final list of "Golden Runs" for sPHENIX analysis. The selection criteria are:
#     • Run contains ≥1M events (and runnumber ≥47289 unless 'noRunNumberLimit' is specified).
#     • Calorimeter QA designates EMCal, IHCal, and OHCal as "Golden."
#     • Runtime exceeds 5 minutes.
#     • MBD (minimum bias) livetime is above 80%.
#     • (Optional) Runs missing bad tower maps can be removed if 'removeRunsWithMissingMaps' is given.
#     • (Default) Runs without a magnet_on='t' entry in 'magnet_info' are excluded, **unless** you specify 'addNoMagnet'.
#   After constructing this list, the script may also create DST .list files and examine which runs succeeded or failed in generating those lists.
#
# Main Steps:
#   1) FileCatalog extraction (≥1M events; run≥47289 unless 'noRunNumberLimit' is set).
#   2) Calo QA filtering for EMCal/IHCal/OHCal "Golden" classification.
#   3) Runtime >5 minutes.
#   4) MBD livetime >80%.
#   5) Optionally remove runs missing bad tower maps.
#   6) Remove magnet-off runs unless 'addNoMagnet' is specified.
#   7) Produce final run list, optionally create DST .list files, remove any problem segments if requested, then record success/fail.
#   8) If 'noRunNumberLimit' is given, summarize no-limit vs. run≥47289 scenarios.
#
# Usage:
#   ./GoldenRunList_ConductorFile.sh [removeRunsWithMissingMaps] [dontGenerateFileLists] [noRunNumberLimit] [addNoMagnet] [removeBadSegments]
#     - removeRunsWithMissingMaps : Exclude runs missing bad tower maps.
#     - dontGenerateFileLists     : Omit creation of DST .list files entirely.
#     - noRunNumberLimit          : Omit the runnumber≥47289 cutoff.
#     - addNoMagnet               : **Include** runs lacking magnet_on='t' in magnet_info (i.e., skip magnet-off removal).
#     - removeBadSegments         : After DST lists are created, remove specific "bad" segments from each run’s .list file.
##############################################################################################################################################################################

########################################
# GLOBAL STYLES
########################################
BOLD="\e[1m"
RESET="\e[0m"
GREEN="\e[32m"
CYAN="\e[36m"
MAGENTA="\e[35m"
YELLOW="\e[33m"

########################################
# FUNCTIONS
########################################

# Prints an error message and exits. If 'dontGenerateFileLists' is set,
# it clarifies that no DST lists would have been generated.
error_exit() {
    echo -e "${BOLD}${YELLOW}[ERROR]:${RESET} $1"
    if $DONT_GENERATE_FILELISTS; then
        echo "Note: 'dontGenerateFileLists' was provided, so no DST lists would have been generated."
    fi
    exit 1
}

# ------------------------------------------------------------------------------
# PARSE ARGUMENTS
# By default:
#   - Runs with missing maps are kept (unless removeRunsWithMissingMaps).
#   - DST file lists are created (unless dontGenerateFileLists).
#   - Run≥47289 enforced (unless noRunNumberLimit).
#   - Magnet-off runs are removed (unless addNoMagnet).
#   - Problematic segments are *not* removed (unless removeBadSegments).
# ------------------------------------------------------------------------------
parse_arguments() {
    REMOVE_MISSING_MAPS=false
    DONT_GENERATE_FILELISTS=false
    NO_RUNNUMBER_LIMIT=false
    ADD_NO_MAGNET=false          # If false => magnet-off runs are removed.
    REMOVE_BAD_SEGMENTS=false    # If true => remove problematic segments from run-specific DST .list

    for arg in "$@"; do
        case "$arg" in
            removeRunsWithMissingMaps)
                REMOVE_MISSING_MAPS=true
                echo -e "${BOLD}${CYAN}Argument detected:${RESET} Removing runs missing bad tower maps."
                ;;
            dontGenerateFileLists)
                DONT_GENERATE_FILELISTS=true
                echo -e "${BOLD}${CYAN}Argument detected:${RESET} Will not generate DST lists."
                ;;
            noRunNumberLimit)
                NO_RUNNUMBER_LIMIT=true
                echo -e "${BOLD}${CYAN}Argument detected:${RESET} No run number lower limit will be applied."
                ;;
            addNoMagnet)
                ADD_NO_MAGNET=true
                echo -e "${BOLD}${CYAN}Argument detected:${RESET} Including runs with magnet_off (or missing)."
                ;;
            removeBadSegments)
                REMOVE_BAD_SEGMENTS=true
                echo -e "${BOLD}${CYAN}Argument detected:${RESET} Will remove problematic DST segments from each run’s .list files."
                ;;
        esac
    done

    if ! $REMOVE_MISSING_MAPS; then
        echo "No removal option detected: Missing-map runs will be kept."
    fi
    if $ADD_NO_MAGNET; then
        echo "Runs with magnet_off or missing will be INCLUDED (via addNoMagnet)."
    else
        echo "Runs with magnet_off or missing will be EXCLUDED by default."
    fi
    if ! $REMOVE_BAD_SEGMENTS; then
        echo "No bad-segment removal requested: All segments remain."
    fi

    echo "----------------------------------------"
}

# Removes pre-existing lists, FileLists, and old .list files in ../dst_list.
clean_previous_data() {
    echo -e "${BOLD}${GREEN}Cleaning old data (dst_list contents, intermediate files)...${RESET}"

    rm -rf list/*
    rm -rf FileLists/*
    rm -f "${workplace}/../dst_list/"*.list

    rm -f "${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts.txt"
    rm -f "${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts_ge47289.txt"
    rm -rf "${workplace}/../dst_list_scratch"

    echo "All old data removed. Starting fresh."
    echo "----------------------------------------"
}

# Creates or verifies directories: FileLists/, list/, and ../dst_list.
setup_directories() {
    echo -e "${BOLD}${GREEN}Setting up directories...${RESET}"
    base_path="${workplace}/.."
    mkdir -p FileLists/
    mkdir -p "${base_path}/dst_list"
    mkdir -p list/

    echo "Directories ready under ${base_path}/"
    echo "----------------------------------------"
}

# Captures current working directory in 'workplace'.
set_workplace() {
    workplace=$(pwd)
    echo -e "${BOLD}Working directory:${RESET} $workplace"
    echo "----------------------------------------"
}

# (Step 1) Extract runs from FileCatalog with ≥1M events. If noRunNumberLimit
# is false => also require run≥47289. Then intersect with Production_write
# for Golden EMCal/IHCal/OHCal. Stores final in 'list/Full_ppGoldenRunList.txt'.
extract_initial_runs() {
    echo -e "${BOLD}${MAGENTA}Step 1:${RESET} Extracting initial runs from databases..."

    python_script=$(cat <<EOF
import pyodbc
import sys

no_limit = "NO_LIMIT" in sys.argv

try:
    fc_conn = pyodbc.connect("DSN=FileCatalog;UID=phnxrc;READONLY=True")
except:
    print("TOTAL_RUNS:0")
    sys.exit(1)

fc_cursor = fc_conn.cursor()

if no_limit:
    query = """
    SELECT runnumber
    FROM datasets
    WHERE dsttype='DST_CALO_run2pp' AND dataset='ana462_2024p010_v001'
    GROUP BY runnumber
    HAVING SUM(events) >= 1000000;
    """
else:
    query = """
    SELECT runnumber
    FROM datasets
    WHERE dsttype='DST_CALO_run2pp' AND dataset='ana462_2024p010_v001'
    GROUP BY runnumber
    HAVING SUM(events) >= 1000000 AND runnumber >= 47289;
    """

fc_cursor.execute(query)
all_runs = [row.runnumber for row in fc_cursor.fetchall()]
all_runs.sort()

with open('list/list_runnumber_all.txt', 'w') as f:
    for r in all_runs:
        f.write(f"{r}\\n")
print(f"TOTAL_RUNS:{len(all_runs)}")

fc_conn.close()

try:
    prod_conn = pyodbc.connect("DSN=Production_write")
except:
    print("COMBINED_GOLDEN_RUNS:0")
    sys.exit(1)

prod_cursor = prod_conn.cursor()
detectors = ['emcal', 'ihcal', 'ohcal']
golden_runs = set(all_runs)

for d in detectors:
    prod_cursor.execute(f"SELECT runnumber FROM goodruns WHERE ({d}_auto).runclass='GOLDEN'")
    detector_golden = {row.runnumber for row in prod_cursor.fetchall()}
    golden_runs = golden_runs.intersection(detector_golden)

golden_runs = sorted(golden_runs)

with open('list/Full_ppGoldenRunList.txt', 'w') as f:
    for r in golden_runs:
        f.write(f"{r}\\n")
print(f"COMBINED_GOLDEN_RUNS:{len(golden_runs)}")
prod_conn.close()
EOF
    )

    # If noRunNumberLimit => pass NO_LIMIT so we skip run≥47289
    if $NO_RUNNUMBER_LIMIT; then
        python_output=$(python3 <(echo "$python_script") NO_LIMIT)
    else
        python_output=$(python3 <(echo "$python_script"))
    fi

    total_runs=$(echo "$python_output" | grep 'TOTAL_RUNS' | cut -d':' -f2)
    combined_golden_runs=$(echo "$python_output" | grep 'COMBINED_GOLDEN_RUNS' | cut -d':' -f2)

    echo "Summary after initial extraction:"
    echo "Total initial runs: ${total_runs:-0}"
    echo "Runs after Calo QA: ${combined_golden_runs:-0}"
    echo "----------------------------------------"

    if [[ "${total_runs:-0}" -eq 0 ]]; then
        error_exit "No runs found after initial extraction. No data matches your criteria."
    fi
}

# (Step 2) Validate that 'Full_ppGoldenRunList.txt' exists and is non-empty.
validate_golden_list() {
    echo -e "${BOLD}${MAGENTA}Step 2:${RESET} Validating golden run list..."
    if [[ ! -f "list/Full_ppGoldenRunList.txt" ]]; then
        error_exit "list/Full_ppGoldenRunList.txt not found. Possibly no runs qualify."
    fi
    echo "Golden run list found."
    echo "----------------------------------------"
}

# Helper to sum events from .evt in the filelist table (psql queries).
# Groups runs in batches of 100 for efficiency.
get_actual_events_from_evt() {
    input_file=$1
    total_events=0
    batch_size=100
    run_numbers=()

    while IFS= read -r runnumber; do
        [[ -z "$runnumber" ]] && continue
        run_numbers+=("$runnumber")
        if [[ ${#run_numbers[@]} -ge $batch_size ]]; then
            run_list=$(IFS=,; echo "${run_numbers[*]}")
            run_numbers=()
            query="SELECT SUM(lastevent - firstevent + 1)
                   FROM filelist
                   WHERE runnumber IN ($run_list)
                     AND filename LIKE '%GL1_physics_gl1daq%.evt';"
            result=$(psql -h sphnxdaqdbreplica -d daq -t -c "$query")
            events=$(echo "$result" | xargs)
            if [[ "$events" =~ ^[0-9]+$ ]]; then
                total_events=$(echo "$total_events + $events" | bc)
            fi
        fi
    done < "$input_file"

    # Leftover runs
    if [[ ${#run_numbers[@]} -gt 0 ]]; then
        run_list=$(IFS=,; echo "${run_numbers[*]}")
        query="SELECT SUM(lastevent - firstevent + 1)
               FROM filelist
               WHERE runnumber IN ($run_list)
                 AND filename LIKE '%GL1_physics_gl1daq%.evt';"
        result=$(psql -h sphnxdaqdbreplica -d daq -t -c "$query")
        events=$(echo "$result" | xargs)
        if [[ "$events" =~ ^[0-9]+$ ]]; then
            total_events=$(echo "$total_events + $events" | bc)
        fi
    fi

    echo "$total_events"
}

# Announces the incremental cuts to be applied next.
apply_incremental_cuts_header() {
    echo "----------------------------------------"
    echo -e "${BOLD}${MAGENTA}Applying incremental cuts:${RESET} runtime, livetime, and missing bad tower maps"
    echo "----------------------------------------"
}

mbd_scaledown_cut() {
    # 1. Input file: the full golden run list from earlier steps.
    input_file="list/Full_ppGoldenRunList.txt"

    # 2. Define output files.
    output_file_mbdscaledown_v1="list/list_runnumber_mbdscaledown_v1.txt"
    bad_file_mbdscaledown_v1="list/list_runnumber_bad_mbdscaledown_v1.txt"

    # Clear the output files before processing.
    > "$output_file_mbdscaledown_v1"
    > "$bad_file_mbdscaledown_v1"

    total_runs_mbdscaledown_v1=0
    runs_dropped_mbdscaledown_v1=0

    total_input_runs=$(wc -l < "$input_file")
    processed=0

    echo "========================================"
    echo "[START] Processing MBD Scaledown Cut for $total_input_runs runs from $input_file"
    echo "========================================"

    while IFS= read -r runnumber; do
        ((processed++))
        # Skip empty lines.
        [[ -z "$runnumber" ]] && continue

        echo "[INFO] Processing run: $runnumber (Run $processed of $total_input_runs)"

        # Build a query to retrieve the scaledown factor for index 10.
        query=$(printf "SELECT scaledown10 FROM gl1_scaledown WHERE runnumber = %d;" "$runnumber")
        echo "[DEBUG] Executing query for run $runnumber:"
        echo "$query"

        # Execute the query using psql.
        result=$(psql -h sphnxdaqdbreplica -d daq -t -c "$query")
        echo "[DEBUG] Query result for run $runnumber: $result"

        # Remove any extra whitespace.
        scaled_val=$(echo "$result" | xargs)
        echo "[DEBUG] Run $runnumber: scaledown10 value = '$scaled_val'"

        # If the scaled factor equals -1, mark the run as trigger OFF.
        if [[ "$scaled_val" == "-1" ]]; then
            echo "[INFO] Run $runnumber has scaledown factor -1: MBD trigger OFF. Skipping run."
            echo "$runnumber" >> "$bad_file_mbdscaledown_v1"
            (( runs_dropped_mbdscaledown_v1++ ))
        else
            echo "[INFO] Run $runnumber accepted (scaledown factor = $scaled_val)."
            echo "$runnumber" >> "$output_file_mbdscaledown_v1"
            (( total_runs_mbdscaledown_v1++ ))
        fi

        # Optional progress update every 100 runs.
        if (( processed % 100 == 0 )); then
            echo "[PROGRESS] Processed $processed out of $total_input_runs runs so far..."
        fi

    done < "$input_file"

    echo "========================================"
    echo "[SUMMARY] MBD Scaledown Cut Results:"
    echo "          Accepted runs: $total_runs_mbdscaledown_v1"
    echo "          Dropped runs:  $runs_dropped_mbdscaledown_v1"
    echo "          Dropped run numbers:"
    cat "$bad_file_mbdscaledown_v1"
    echo "========================================"
    # The next stage will now use the accepted runs stored in $output_file_mbdscaledown_v1.
}


# (Step 3) Applies runtime >5min by checking (ertimestamp - brtimestamp).
# Also prints progress in blocks of 100 runs processed.
runtime_cut() {
    input_file="list/list_runnumber_mbdscaledown_v1.txt"
    output_file_duration_v1="list/list_runnumber_runtime_v1.txt"
    > "$output_file_duration_v1"

    total_runs_duration_v1=0
    runs_dropped_runtime_v1=0

    total_input_runs=$(wc -l < "$input_file")
    processed=0

    while IFS= read -r runnumber; do
        ((processed++))
        [[ -z "$runnumber" ]] && continue

        query="SELECT EXTRACT(EPOCH FROM (ertimestamp - brtimestamp))
               FROM run
               WHERE runnumber = ${runnumber};"
        result=$(psql -h sphnxdaqdbreplica -d daq -t -c "$query" | tr -d '[:space:]')
        duration="$result"

        if [[ "$duration" =~ ^[0-9]+(\.[0-9]+)?$ ]] && (( $(echo "$duration > 300" | bc -l) )); then
            echo "$runnumber" >> "$output_file_duration_v1"
            (( total_runs_duration_v1++ ))
        else
            (( runs_dropped_runtime_v1++ ))
        fi

        # Print progress every 100 runs
        if (( processed % 100 == 0 )); then
            echo "  [Runtime Cut] Processed $processed / $total_input_runs runs so far..."
        fi
    done < "$input_file"

    echo "After runtime cut (>5 mins): $total_runs_duration_v1 runs remain."
    echo "Dropped due to runtime: $runs_dropped_runtime_v1"
    echo "----------------------------------------"
}

# (Step 4) Applies MBD livetime >80% by checking (live / raw *100).
# Again prints progress every 100 runs.
livetime_cut() {
    input_file="list/list_runnumber_runtime_v1.txt"
    output_file_livetime_v1="list/list_runnumber_livetime_v1.txt"
    bad_file_livetime_v1="list/list_runnumber_bad_livetime_v1.txt"
    > "$output_file_livetime_v1"
    > "$bad_file_livetime_v1"

    total_runs_livetime_v1=0
    runs_dropped_livetime_v1=0

    total_input_runs=$(wc -l < "$input_file")
    processed=0

    while IFS= read -r runnumber; do
        ((processed++))
        [[ -z "$runnumber" ]] && continue

        index_to_check=10
        query="SELECT raw, live
               FROM gl1_scalers
               WHERE runnumber = ${runnumber}
                 AND index = ${index_to_check};"
        result=$(psql -h sphnxdaqdbreplica -d daq -t -c "$query")

        index_pass=false
        while IFS='|' read -r raw live; do
            raw=$(echo "$raw" | xargs)
            live=$(echo "$live" | xargs)
            if [[ "$raw" =~ ^[0-9]+$ && "$live" =~ ^[0-9]+$ && "$raw" -ne 0 ]]; then
                ratio=$(echo "scale=2; $live / $raw * 100" | bc -l)
                if (( $(echo "$ratio >= 80" | bc -l) )); then
                    index_pass=true
                fi
            fi
        done <<< "$result"

        if $index_pass; then
            echo "$runnumber" >> "$output_file_livetime_v1"
            (( total_runs_livetime_v1++ ))
        else
            echo "$runnumber" >> "$bad_file_livetime_v1"
            (( runs_dropped_livetime_v1++ ))
        fi

        if (( processed % 100 == 0 )); then
            echo "  [Livetime Cut] Processed $processed / $total_input_runs runs so far..."
        fi
    done < "$input_file"

    echo "After livetime cut (>80%): $total_runs_livetime_v1 runs remain."
    echo "Dropped due to livetime: $runs_dropped_livetime_v1"
    echo "----------------------------------------"
}

# (Step 5) If removeRunsWithMissingMaps => remove runs not found in the known
# bad tower map set. Otherwise keep them. Writes to pre-magnet file.
# Prints progress every 100 runs.
missing_bad_tower_maps_step() {
    input_file="list/list_runnumber_livetime_v1.txt"
    pre_magnet_file="FileLists/Full_ppGoldenRunList_Version1_preMagnet.txt"
    cp "$input_file" "$pre_magnet_file"

    bad_tower_runs_file="list/list_runs_missing_bad_tower_maps.txt"
    available_bad_tower_runs=$(find /cvmfs/sphenix.sdcc.bnl.gov/calibrations/sphnxpro/cdb/CEMC_BadTowerMap \
                               -name "*p0*" | cut -d '-' -f2 | cut -dc -f1 | sort | uniq)
    echo "$available_bad_tower_runs" > list/available_bad_tower_runs.txt

    total_input_runs=$(wc -l < "$input_file")
    processed=0

    # We'll store "available_bad_tower_runs.txt" lines in an array for membership checks
    mapfile -t available_map_array < list/available_bad_tower_runs.txt
    declare -A avail_map
    for runmap in "${available_map_array[@]}"; do
        avail_map["$runmap"]=1
    done

    > "$bad_tower_runs_file"

    # Identify runs missing maps
    while IFS= read -r runnumber; do
        ((processed++))
        [[ -z "$runnumber" ]] && continue

        if [[ -z "${avail_map[$runnumber]}" ]]; then
            echo "$runnumber" >> "$bad_tower_runs_file"
        fi

        if (( processed % 100 == 0 )); then
            echo "  [Bad Tower Step] Processed $processed / $total_input_runs runs so far..."
        fi
    done < "$input_file"

    total_runs_with_bad_tower=$(grep -Fxf list/available_bad_tower_runs.txt "$input_file" | wc -l)
    total_runs_missing_bad_tower=$(wc -l < "$bad_tower_runs_file")

    echo "Runs with bad tower maps: $total_runs_with_bad_tower"
    echo "Runs missing bad tower maps: $total_runs_missing_bad_tower"
    echo "List of missing map runs: $bad_tower_runs_file"
    echo "----------------------------------------"

    export total_runs_missing_bad_tower
    rm list/available_bad_tower_runs.txt

    if $REMOVE_MISSING_MAPS; then
        echo "Removing runs missing bad tower maps..."
        grep -Fxf "$bad_tower_runs_file" -v "$pre_magnet_file" > tmp && mv tmp "$pre_magnet_file"
        echo "Removal complete."
        echo "----------------------------------------"
    fi

    cp "$pre_magnet_file" "${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts.txt"
    echo "Final run list (pre-magnet step) stored in ${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts.txt"
    echo "----------------------------------------"

    # If noRunNumberLimit => also create ge47289 final
    if $NO_RUNNUMBER_LIMIT; then
        awk '$1 >= 47289' "$pre_magnet_file" > FileLists/Full_ppGoldenRunList_ge47289_Version1.txt
        cp FileLists/Full_ppGoldenRunList_ge47289_Version1.txt \
           "${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts_ge47289.txt"

        if $REMOVE_MISSING_MAPS; then
            missing_maps_ge47289=$(grep -Fxf FileLists/Full_ppGoldenRunList_ge47289_Version1.txt \
                                       "$bad_tower_runs_file" | wc -l)
            export missing_maps_ge47289
        fi
    fi
}

# (Step 6) By default, exclude any runs whose magnet_on != 't'.
# If 'addNoMagnet' is true => we skip this check, keeping magnet-off runs.
# Print progress in blocks of 100 processed.
magnet_check_step() {
    if [[ "$ADD_NO_MAGNET" == true ]]; then
        echo "addNoMagnet argument was provided: skipping magnet check => keeping magnet-off runs..."
        mv FileLists/Full_ppGoldenRunList_Version1_preMagnet.txt FileLists/Full_ppGoldenRunList_Version1.txt
        return
    fi

    echo "Step 6: Removing runs where magnet_on != 't'..."

    pre_magnet_file="FileLists/Full_ppGoldenRunList_Version1_preMagnet.txt"
    if [[ ! -f "$pre_magnet_file" ]]; then
        echo "[ERROR] No 'preMagnet' file found: $pre_magnet_file"
        return
    fi

    magnet_off_file="list/list_runs_no_magnet.txt"
    > "$magnet_off_file"

    final_list_magnet="FileLists/Full_ppGoldenRunList_Version1.txt"
    > "$final_list_magnet"

    total_runs_magnet_ok=0
    runs_dropped_magnet=0

    total_input_runs=$(wc -l < "$pre_magnet_file")
    processed=0

    while IFS= read -r runnumber; do
        ((processed++))
        [[ -z "$runnumber" ]] && continue

        query="SELECT magnet_on
               FROM magnet_info
               WHERE runnumber=${runnumber};"
        result=$(psql -h sphnxdaqdbreplica -d daq -t -c "$query" | tr -d '[:space:]')

        if [[ "$result" == "t" ]]; then
            echo "$runnumber" >> "$final_list_magnet"
            (( total_runs_magnet_ok++ ))
        else
            echo "$runnumber" >> "$magnet_off_file"
            (( runs_dropped_magnet++ ))
        fi

        if (( processed % 100 == 0 )); then
            echo "  [Magnet Check] Processed $processed / $total_input_runs runs so far..."
        fi
    done < "$pre_magnet_file"

    echo "Magnet On check: $total_runs_magnet_ok runs kept (magnet_on='t')."
    echo "Dropped (magnet_off or missing): $runs_dropped_magnet"
    echo "List of dropped runs: $magnet_off_file"
    echo "----------------------------------------"

    # Overwrite final list with magnet-checked version
    cp "$final_list_magnet" "${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts.txt"

    export total_runs_magnet_off=$runs_dropped_magnet
}

# Creates local .list file from the final text file in FileLists/.
# If nothing is found => warns. Otherwise produce "Full_ppGoldenRunList_Version1.list".
create_list_file() {
    echo "Creating final .list file from the final run list..."

    if [[ ! -f "FileLists/Full_ppGoldenRunList_Version1.txt" ]]; then
        if [[ -f "FileLists/Full_ppGoldenRunList_Version1_preMagnet.txt" ]]; then
            mv FileLists/Full_ppGoldenRunList_Version1_preMagnet.txt FileLists/Full_ppGoldenRunList_Version1.txt
        fi
    fi

    if [[ -f "FileLists/Full_ppGoldenRunList_Version1.txt" ]]; then
        cp "FileLists/Full_ppGoldenRunList_Version1.txt" Full_ppGoldenRunList_Version1.list
        echo ".list file created: Full_ppGoldenRunList_Version1.list"
    else
        echo "[WARNING] Could not find 'FileLists/Full_ppGoldenRunList_Version1.txt' to create .list!"
    fi
    echo "----------------------------------------"
}

# Removes old DST lists from ../dst_list to avoid confusion.
clean_old_dst_lists() {
    echo "Removing any old DST lists from the parent 'dst_list' directory..."
    rm -f "${workplace}/../dst_list/"*.list
    echo "Done removing old .list files in dst_list/."
    echo "----------------------------------------"
}

# If 'dontGenerateFileLists' is not set => run CreateDstList.pl on the final .list
# to build run-specific .list files. If the .list is missing => skip.
generate_dst_lists() {
    if $DONT_GENERATE_FILELISTS; then
        echo "[INFO] Skipping DST list generation due to 'dontGenerateFileLists'."
        return
    fi

    echo "Generating DST lists for the main scenario..."
    cd "${workplace}/../dst_list"

    list_path="${workplace}/Full_ppGoldenRunList_Version1.list"
    if [[ ! -f "$list_path" ]]; then
        echo "[WARNING] Could not find final .list file at: $list_path"
        echo "No DST lists will be created."
    else
        CreateDstList.pl --build ana462 --cdb 2024p010_v001 DST_JET_run2pp --list "$list_path"
        echo "DST lists generated under ${workplace}/../dst_list"
    fi

    if $NO_RUNNUMBER_LIMIT; then
        echo "No separate scratch folder is used for the ≥47289 subset."
    fi

    echo "----------------------------------------"
    cd "$workplace"
}

remove_problematic_segments() {
    if ! $REMOVE_BAD_SEGMENTS; then
        echo "[INFO] Skipping problematic-segment removal (removeBadSegments not set)."
        return
    fi

    # Internal Boolean: if true, generate the bad segment file using the embedded Python script.
    # If false (the default), use the pre-generated file.
    generateBadSegmentFile=false

    if $generateBadSegmentFile; then
        # Define where the problematic segments file will be generated.
        segments_file="list/list_problematic_segments.txt"

        echo "========================================"
        echo "[INFO] Generating problematic segments file using embedded Python script... this may take some time..."
        echo "[DEBUG] Starting Python script execution..."
        python3 <<'EOF' > "$segments_file"
import os
import sys
import subprocess
import argparse

# Define a debug print function that flushes immediately.
def dprint(*args, **kwargs):
    print(*args, **kwargs)
    sys.stdout.flush()

# Create the "lists" folder for the primary output file.
lists_dir = os.path.join(os.getcwd(), "lists")
os.makedirs(lists_dir, exist_ok=True)
dprint("Created/verified 'lists' directory:", lists_dir)

# Create the "list" folder for the segments output file.
list_dir = os.path.join(os.getcwd(), "list")
os.makedirs(list_dir, exist_ok=True)
dprint("Created/verified 'list' directory:", list_dir)

parser = argparse.ArgumentParser()
parser.add_argument(
    '-i', '--log-dir',
    type=str,
    default='/sphenix/data/data02/sphnxpro/run2pp/calologs/ana446_2024p007',
    help='Location of the log directory.'
)
# Output file for logs (stays in the lists folder)
parser.add_argument(
    '-o', '--output',
    type=str,
    default=os.path.join(lists_dir, 'bad-calologs-ana446-2024p007.list'),
    help='Output list file of all files that have an error.'
)
# Output file for segments (placed in the list folder)
parser.add_argument(
    '-o2', '--output-segments',
    type=str,
    default=os.path.join(list_dir, 'list_problematic_segments.txt'),
    help='Output list file of all segments that have an error.'
)

args = parser.parse_args()

if __name__ == '__main__':
    log_dir = os.path.realpath(args.log_dir)
    output = os.path.realpath(args.output)
    output_segments = os.path.realpath(args.output_segments)

    dprint(f'Log Dir: {log_dir}')
    dprint(f'Output: {output}')
    dprint(f'Output Segments: {output_segments}')

    # Remove the output file if it already exists.
    if os.path.exists(output):
        os.remove(output)
        dprint(f'File {output} deleted successfully.')

    # Generate the list of log files that contain "Error".
    command = f'''while read d; do
        echo "Processing directory: $d"
        /direct/sphenix+u/anarde/.cargo/bin/rg -l "Error" $d >> {output}
        echo "Current log count in {output}: $(wc -l {output})"
    done < <(readlink -f {log_dir}/*)'''
    dprint("Running command to generate error log file...")
    subprocess.run(['bash', '-c', command], check=True)
    dprint("Completed generating error log file.")

    # Extract only the run-segments from the list of log files.
    command = f"awk -F'/' '{{print $NF}}' {output} | cut -d'-' -f2,3 | cut -d'.' -f1 | sort > {output_segments}"
    dprint("Running command to extract run-segments...")
    subprocess.run(['bash', '-c', command], check=True)
    dprint("Extracted run-segments into", output_segments)
EOF
    else
        segments_file="/sphenix/user/patsfan753/tutorials/tutorials/CaloDataAnaRun24pp/runListGeneration/bad-ana446-2024p007-segments.list"
        echo "========================================"
        echo "[INFO] Using pre-generated bad segments file: $segments_file"
    fi

    echo "========================================"
    echo "[INFO] Problematic segments file is: $segments_file"
    if [[ ! -f "$segments_file" ]]; then
        echo "[WARNING] Could not find segments file at $segments_file. No segments removed."
        return
    fi

    echo "[INFO] Starting removal of problematic segments from DST lists..."
    echo "========================================"

    while IFS= read -r line; do
        [[ -z "$line" ]] && continue

        echo "[DEBUG] Processing segment line: '$line'"
        # Each line is expected to be of the form "00046010-00003".
        run_part="${line%-*}"   # e.g. 00046010
        seg_part="${line#*-}"    # e.g. 00003
        echo "[DEBUG] Extracted run number: '$run_part'"
        echo "[DEBUG] Extracted segment number: '$seg_part'"

        # Determine the corresponding DST .list file to modify.
        listfile="${workplace}/../dst_list/dst_jet_run2pp-${run_part}.list"
        echo "[DEBUG] Looking for DST list file: $listfile"
        if [[ -f "$listfile" ]]; then
            # The DST list lines are expected to contain something like:
            # DST_JET_run2pp_ana462_2024p010_v001-00046010-00003.root
            pattern="${run_part}-${seg_part}.root"
            echo "[DEBUG] Searching for pattern '$pattern' in $listfile"
            lines_to_remove=$(grep "$pattern" "$listfile")
            if [[ -n "$lines_to_remove" ]]; then
                echo "[INFO] Found the following lines in $listfile matching pattern '$pattern':"
                echo "$lines_to_remove"
                grep -v "$pattern" "$listfile" > "${listfile}.tmp" && mv "${listfile}.tmp" "$listfile"
                echo "[INFO] Removed segment '$pattern' from $listfile."
            else
                echo "[INFO] No matching lines found for segment '$pattern' in $listfile. Skipping removal."
            fi
        else
            echo "[WARNING] DST list file $listfile does not exist. Skipping segment '$seg_part' for run '$run_part'."
        fi
        echo "----------------------------------------"
    done < "$segments_file"

    echo "[INFO] Done removing problematic segments."
    echo "========================================"
}


# After DST .list files are generated, check which runs successfully produced
# run-specific .list files (DST_JET_run2pp-xxxx.list).
# Summarize success/fail in text files.
apply_createDstList_cut() {
    echo "Collecting CreateDST File List success/failure for the main scenario..."

    if $DONT_GENERATE_FILELISTS; then
        echo "[INFO]: 'dontGenerateFileLists' was provided, so no DST creation was done for main scenario."
        export total_runs_createDst_success=0
        export runs_dropped_createDst=0
        export actual_events_after_createDst=0
        export total_runs_after_createDst=0
        return
    fi

    final_stage4_file="FileLists/Full_ppGoldenRunList_Version1.txt"
    if [[ ! -f "$final_stage4_file" ]]; then
        echo "[ERROR]: Cannot find final stage file: $final_stage4_file"
        export total_runs_createDst_success=0
        export runs_dropped_createDst=0
        export actual_events_after_createDst=0
        export total_runs_after_createDst=0
        return
    fi

    success_file="list/list_runnumber_createDstSuccess.txt"
    failure_file="list/list_runnumber_createDstFailure.txt"
    > "$success_file"
    > "$failure_file"

    mapfile -t final_stage_runs < "$final_stage4_file"

    base_path="${workplace}/../dst_list"
    created_run_nums=()

    # Gather runs for which a dst_jet_run2pp-xxxx.list was indeed produced.
    for f in "${base_path}/dst_jet_run2pp-"*.list; do
        [ -e "$f" ] || continue
        bn=$(basename "$f" .list)
        runnum_str=${bn#dst_jet_run2pp-}
        if [[ "$runnum_str" =~ ^0*([0-9]+)$ ]]; then
            runnum=${BASH_REMATCH[1]}
            created_run_nums+=("$runnum")
        fi
    done

    total_runs_createDst_success=0
    runs_dropped_createDst=0
    declare -A in_created
    for rn in "${created_run_nums[@]}"; do
        in_created["$rn"]=1
    done

    for runnumber in "${final_stage_runs[@]}"; do
        if [[ -n "${in_created[$runnumber]}" ]]; then
            echo "$runnumber" >> "$success_file"
            (( total_runs_createDst_success++ ))
        else
            echo "$runnumber" >> "$failure_file"
            (( runs_dropped_createDst++ ))
        fi
    done

    echo "Runs with successful .list creation: $total_runs_createDst_success"
    echo "Runs with no .list file: $runs_dropped_createDst"
    echo "List of runs that failed:  $failure_file"
    echo "List of runs that succeeded: $success_file"
    echo "----------------------------------------"

    # Summation of events for runs that succeeded
    actual_events_after_createDst=$(get_actual_events_from_evt "$success_file")
    total_runs_after_createDst=$total_runs_createDst_success

    cp "$success_file" "FileLists/Full_ppGoldenRunList_Version1_DSTsuccess.txt"

    export total_runs_createDst_success
    export runs_dropped_createDst
    export actual_events_after_createDst
    export total_runs_after_createDst

    # If noRunNumberLimit => do the same for the run≥47289 scenario
    if $NO_RUNNUMBER_LIMIT; then
        echo "Collecting CreateDST File List success/failure for the '≥47289' scenario..."
        final_stage4_file_ge47289="FileLists/Full_ppGoldenRunList_ge47289_Version1.txt"
        if [[ ! -f "$final_stage4_file_ge47289" ]]; then
            echo "[ERROR]: Cannot find the ge47289 final list: $final_stage4_file_ge47289"
            export total_runs_createDst_success_ge47289=0
            export runs_dropped_createDst_ge47289=0
            export actual_events_after_createDst_ge47289=0
            export total_runs_after_createDst_ge47289=0
        else
            success_file_ge47289="list/list_runnumber_createDstSuccess_ge47289.txt"
            failure_file_ge47289="list/list_runnumber_createDstFailure_ge47289.txt"
            > "$success_file_ge47289"
            > "$failure_file_ge47289"

            mapfile -t final_stage_runs_ge47289 < "$final_stage4_file_ge47289"

            base_path_scratch="${workplace}/../dst_list"
            created_run_nums_ge47289=()
            for f in "${base_path_scratch}/dst_jet_run2pp-"*.list; do
                [ -e "$f" ] || continue
                bn=$(basename "$f" .list)
                runnum_str=${bn#dst_jet_run2pp-}
                if [[ "$runnum_str" =~ ^0*([0-9]+)$ ]]; then
                    runnum=${BASH_REMATCH[1]}
                    created_run_nums_ge47289+=("$runnum")
                fi
            done

            total_runs_createDst_success_ge47289=0
            runs_dropped_createDst_ge47289=0
            declare -A in_created_ge47289
            for rn in "${created_run_nums_ge47289[@]}"; do
                in_created_ge47289["$rn"]=1
            done

            for runnumber in "${final_stage_runs_ge47289[@]}"; do
                if [[ -n "${in_created_ge47289[$runnumber]}" ]]; then
                    echo "$runnumber" >> "$success_file_ge47289"
                    (( total_runs_createDst_success_ge47289++ ))
                else
                    echo "$runnumber" >> "$failure_file_ge47289"
                    (( runs_dropped_createDst_ge47289++ ))
                fi
            done

            echo "≥47289 scenario: runs with successful .list creation: $total_runs_createDst_success_ge47289"
            echo "≥47289 scenario: runs with no .list file: $runs_dropped_createDst_ge47289"
            echo "≥47289 scenario: List of runs that failed:  $failure_file_ge47289"
            echo "≥47289 scenario: List of runs that succeeded: $success_file_ge47289"
            echo "----------------------------------------"

            actual_events_after_createDst_ge47289=$(get_actual_events_from_evt "$success_file_ge47289")
            total_runs_after_createDst_ge47289=$total_runs_createDst_success_ge47289

            cp "$success_file_ge47289" "FileLists/Full_ppGoldenRunList_ge47289_Version1_DSTsuccess.txt"

            export total_runs_createDst_success_ge47289
            export runs_dropped_createDst_ge47289
            export actual_events_after_createDst_ge47289
            export total_runs_after_createDst_ge47289
        fi
    fi
}

compute_event_counts() {
    pre_magnet_file="FileLists/Full_ppGoldenRunList_Version1_preMagnet.txt"
    final_file="FileLists/Full_ppGoldenRunList_Version1.txt"

    #############################################
    # Stage 1: Initial Extraction
    #############################################
    actual_events_before_cuts=$(get_actual_events_from_evt 'list/list_runnumber_all.txt')
    total_runs_before_cuts=$(wc -l < 'list/list_runnumber_all.txt')
    actual_events_initial=$actual_events_before_cuts

    #############################################
    # Stage 2: Calo QA (Golden Run List)
    #############################################
    actual_events_calo_qa=$(get_actual_events_from_evt 'list/Full_ppGoldenRunList.txt')
    runs_after_calo_qa=$(wc -l < 'list/Full_ppGoldenRunList.txt')

    ####################################################
    # Stage 3: MBD Scaledown Cut (MBD Trigger On Check)
    ####################################################
    actual_events_mbd=$(get_actual_events_from_evt 'list/list_runnumber_mbdscaledown_v1.txt')
    runs_after_mbd=$(wc -l < 'list/list_runnumber_mbdscaledown_v1.txt')
    dropped_runs_mbd=$(wc -l < 'list/list_runnumber_bad_mbdscaledown_v1.txt')
    actual_events_mbd_dropped=$(get_actual_events_from_evt 'list/list_runnumber_bad_mbdscaledown_v1.txt')

    #####################################
    # Stage 4: Runtime Cut (>5 minutes)
    #####################################
    actual_events_after_runtime=$(get_actual_events_from_evt 'list/list_runnumber_runtime_v1.txt')
    runs_after_runtime=$(wc -l < 'list/list_runnumber_runtime_v1.txt')

    ##################################
    # Stage 5: Livetime Cut (>80%)
    ##################################
    actual_events_after_livetime=$(get_actual_events_from_evt 'list/list_runnumber_livetime_v1.txt')
    runs_after_livetime=$(wc -l < 'list/list_runnumber_livetime_v1.txt')

    ##########################################
    # Stage 6: Pre-Magnet (Bad Tower Removal)
    ##########################################
    if [[ -f "$pre_magnet_file" ]]; then
        actual_events_after_badtower=$(get_actual_events_from_evt "$pre_magnet_file")
        runs_after_badtower=$(wc -l < "$pre_magnet_file")
    else
        actual_events_after_badtower=0
        runs_after_badtower=0
    fi

    ###################################
    # Stage 7: Final (Magnet On Check)
    ###################################
    if [[ -f "$final_file" ]]; then
        actual_events_after_magnet=$(get_actual_events_from_evt "$final_file")
        runs_after_magnet=$(wc -l < "$final_file")
    else
        actual_events_after_magnet=0
        runs_after_magnet=0
    fi

    ##############################################
    # Stage 8: DST Creation Success (Final Output)
    ##############################################
    if [[ -f FileLists/Full_ppGoldenRunList_Version1_DSTsuccess.txt ]]; then
        actual_events_after_createDst=$(get_actual_events_from_evt "FileLists/Full_ppGoldenRunList_Version1_DSTsuccess.txt")
        runs_after_createDst=$(wc -l < "FileLists/Full_ppGoldenRunList_Version1_DSTsuccess.txt")
    else
        actual_events_after_createDst=0
        runs_after_createDst=0
    fi

    # Use Stage 1 (initial extraction) as the baseline.
    STAGE1_RUNS=$total_runs_before_cuts
    [[ "$STAGE1_RUNS" -eq 0 ]] && STAGE1_RUNS=1
    [[ "$actual_events_before_cuts" -eq 0 ]] && actual_events_before_cuts=1

    # Compute run-based percentages
    percent_runs_calo_qa=$(echo "scale=2; 100.0*$runs_after_calo_qa/$STAGE1_RUNS" | bc)
    percent_runs_mbd=$(echo "scale=2; 100.0*$runs_after_mbd/$STAGE1_RUNS" | bc)
    percent_runs_runtime=$(echo "scale=2; 100.0*$runs_after_runtime/$STAGE1_RUNS" | bc)
    percent_runs_livetime=$(echo "scale=2; 100.0*$runs_after_livetime/$STAGE1_RUNS" | bc)
    percent_runs_badtower=$(echo "scale=2; 100.0*$runs_after_badtower/$STAGE1_RUNS" | bc)
    percent_runs_magnet=$(echo "scale=2; 100.0*$runs_after_magnet/$STAGE1_RUNS" | bc)
    percent_runs_createDst=$(echo "scale=2; 100.0*$runs_after_createDst/$STAGE1_RUNS" | bc)

    # Compute event-based percentages
    percent_actual_events_calo_qa=$(echo "scale=2; 100.0*$actual_events_calo_qa/$actual_events_before_cuts" | bc)
    percent_actual_events_mbd=$(echo "scale=2; 100.0*$actual_events_mbd/$actual_events_before_cuts" | bc)
    percent_actual_events_after_runtime=$(echo "scale=2; 100.0*$actual_events_after_runtime/$actual_events_before_cuts" | bc)
    percent_actual_events_after_livetime=$(echo "scale=2; 100.0*$actual_events_after_livetime/$actual_events_before_cuts" | bc)
    percent_actual_events_after_badtower=$(echo "scale=2; 100.0*$actual_events_after_badtower/$actual_events_before_cuts" | bc)
    percent_actual_events_after_magnet=$(echo "scale=2; 100.0*$actual_events_after_magnet/$actual_events_before_cuts" | bc)
    percent_events_after_createDst=$(echo "scale=2; 100.0*$actual_events_after_createDst/$actual_events_before_cuts" | bc)

    # Export variables for use in the summary
    export STAGE1_RUNS
    export runs_after_calo_qa runs_after_mbd runs_after_runtime runs_after_livetime runs_after_badtower runs_after_magnet runs_after_createDst
    export actual_events_before_cuts total_runs_before_cuts actual_events_initial
    export actual_events_calo_qa actual_events_mbd actual_events_after_runtime actual_events_after_livetime actual_events_after_badtower actual_events_after_magnet actual_events_after_createDst
    export percent_runs_calo_qa percent_runs_mbd percent_runs_runtime percent_runs_livetime percent_runs_badtower percent_runs_magnet percent_runs_createDst
    export percent_actual_events_calo_qa percent_actual_events_mbd percent_actual_events_after_runtime percent_actual_events_after_livetime percent_actual_events_after_badtower percent_actual_events_after_magnet percent_events_after_createDst
}
print_final_totals() {
    # Path to the final golden run list
    final_list="${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts.txt"
    
    # Check that the final list exists
    if [[ ! -f "$final_list" ]]; then
        echo "[ERROR] Final golden run list not found: $final_list"
        return 1
    fi

    total_runtime=0
    total_live=0

    echo "========================================"
    echo "[INFO] Calculating final totals for all runs in the final golden run list..."
    echo "========================================"

    while IFS= read -r run; do
        # Skip empty lines.
        [[ -z "$run" ]] && continue

        # Query the run duration (in seconds) for this run.
        runtime=$(psql -h sphnxdaqdbreplica -d daq -t -c "SELECT EXTRACT(EPOCH FROM (ertimestamp - brtimestamp)) FROM run WHERE runnumber = $run;" | xargs)
        # If runtime is not a valid number, treat it as zero.
        if ! [[ "$runtime" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
            runtime=0
        fi

        # Query the MBD live count for this run at index=10.
        live=$(psql -h sphnxdaqdbreplica -d daq -t -c "SELECT live FROM gl1_scalers WHERE runnumber = $run AND index = 10;" | xargs)
        if ! [[ "$live" =~ ^[0-9]+$ ]]; then
            live=0
        fi

        total_runtime=$(echo "$total_runtime + $runtime" | bc)
        total_live=$(echo "$total_live + $live" | bc)

    done < "$final_list"

    echo "========================================"
    echo "FINAL TOTALS:"
    echo "Total running time for all runs (in seconds): $total_runtime"
    echo "Total MBD N&S>=1 trigger live counts:       $total_live"
    echo "========================================"
}


final_summary() {
    echo -e "${BOLD}${MAGENTA}========================================${RESET}"
    echo -e "${BOLD}${MAGENTA}Final Summary (Version 1)${RESET}"
    echo -e "${MAGENTA}----------------------------------------${RESET}"
    printf "%-50s | %-35s | %-25s\n" "Stage" ".evt File Events" "Runs"
    echo "--------------------------------------------------|-------------------------------------|-------------------------"

    if $NO_RUNNUMBER_LIMIT; then
        stage1_label="≥1M events"
    else
        stage1_label="≥47289 & ≥1M events"
    fi

    # Stage 1: Initial Extraction
    printf "%-50s | %-35s | %-25s\n" \
        "1) $stage1_label" \
        "${actual_events_initial} (100%)" \
        "${STAGE1_RUNS} (100%)"

    # Stage 2: Golden EMCal/HCal
    printf "%-50s | %-35s | %-25s\n" \
        "2) Golden EMCal/HCal" \
        "${actual_events_calo_qa} (${percent_actual_events_calo_qa}%)" \
        "${runs_after_calo_qa} (${percent_runs_calo_qa}%)"

    # Stage 3: MBD Trigger (Scaledown Cut)
    printf "%-50s | %-35s | %-25s\n" \
        "3) MBD Trigger (Scaledown Cut)" \
        "${actual_events_mbd} (${percent_actual_events_mbd}%)" \
        "${runs_after_mbd} (${percent_runs_mbd}%)"
    # Also print a separate line for the dropped runs due to a scaled value of -1:
    printf "%-50s | %-35s | %-25s\n" \
        "    Dropped (MBD Trigger Off)" \
        "${actual_events_mbd_dropped}" \
        "${dropped_runs_mbd}"

    # Stage 4: Runtime Cut (>5 minutes)
    printf "%-50s | %-35s | %-25s\n" \
        "4) > 5 minutes runtime" \
        "${actual_events_after_runtime} (${percent_actual_events_after_runtime}%)" \
        "${runs_after_runtime} (${percent_runs_runtime}%)"

    # Stage 5: MB Livetime >80%
    printf "%-50s | %-35s | %-25s\n" \
        "5) MB livetime > 80%" \
        "${actual_events_after_livetime} (${percent_actual_events_after_livetime}%)" \
        "${runs_after_livetime} (${percent_runs_livetime}%)"

    # Stage 6: Pre-Magnet (Bad Tower Map Availability)
    if $REMOVE_MISSING_MAPS; then
        step_label="6) Bad Tower Map Available (Removed)"
    else
        step_label="6) Bad Tower Map Available (Not Removed)"
    fi
    printf "%-50s | %-35s | %-25s\n" \
        "$step_label" \
        "${actual_events_after_badtower} (${percent_actual_events_after_badtower}%)" \
        "${runs_after_badtower} (${percent_runs_badtower}%)"

    # Stage 7: Final (Magnet On Check)
    if [[ "$ADD_NO_MAGNET" == false ]]; then
        printf "%-50s | %-35s | %-25s\n" \
            "7) Magnet On" \
            "${actual_events_after_magnet} (${percent_actual_events_after_magnet}%)" \
            "${runs_after_magnet} (${percent_runs_magnet}%)"
    fi

    # Stage 8: DST Creation Success
    printf "%-50s | %-35s | %-25s\n" \
        "8) CreateDST File List Success" \
        "${actual_events_after_createDst} (${percent_events_after_createDst}%)" \
        "${runs_after_createDst} (${percent_runs_createDst}%)"

    # Additional notes (e.g. missing maps, magnet-off runs)
    if $REMOVE_MISSING_MAPS; then
        map_note="Removed from final DST"
    else
        map_note="Kept in final DST"
    fi
    printf "%-50s | %-35s | %-25s\n" \
        "Missing Map Runs" \
        "-" \
        "${total_runs_missing_bad_tower} (${map_note})"

    if [[ "$ADD_NO_MAGNET" == false ]]; then
        if [[ -z "$total_runs_magnet_off" ]]; then
            total_runs_magnet_off=0
        fi
        mag_note="Removed"
        printf "%-50s | %-35s | %-25s\n" \
            "No Magnet-On Runs" \
            "-" \
            "${total_runs_magnet_off} (${mag_note})"
    fi

    echo "================================================="
    echo -e "${BOLD}${MAGENTA}========================================${RESET}"
    echo ""
    if $REMOVE_MISSING_MAPS; then
        echo "Final golden run list (missing maps removed):"
    else
        echo "Final golden run list (with runs missing maps included):"
    fi
    echo " --> ${workplace}/../dst_list/Final_RunNumbers_After_All_Cuts.txt"
    echo "Done."

    # (Optional) Additional comparison table if NO_RUNNUMBER_LIMIT is true…
    if $NO_RUNNUMBER_LIMIT; then
        echo ""
        echo "----------------------------------------"
        echo -e "${BOLD}${MAGENTA}Additional Comparison (Before & After Cuts):${RESET} No-limit vs. ≥47289 scenario"
        echo ""
        cat <<EOCOMPARISON

Metric                                  | NoLimit(All)                | ≥47289                    | ΔValue           | %Lost
--------------------------------------------------------------------------------------------------------------
EOCOMPARISON

        ev_all_initial=${actual_events_before_cuts:-0}
        ev_ge_initial=${actual_events_before_cuts_ge47289:-0}
        diff_ev_initial=$(echo "$ev_all_initial - $ev_ge_initial" | bc)
        if (( ev_all_initial > 0 )); then
            perc_ev_lost_initial=$(echo "scale=2; 100.0*$diff_ev_initial/$ev_all_initial" | bc)
        else
            perc_ev_lost_initial=0
        fi

        run_all_initial=${total_runs_before_cuts:-0}
        run_ge_initial=${total_runs_before_cuts_ge47289:-0}
        diff_run_initial=$(echo "$run_all_initial - $run_ge_initial" | bc)
        if (( run_all_initial > 0 )); then
            perc_run_lost_initial=$(echo "scale=2; 100.0*$diff_run_initial/$run_all_initial" | bc)
        else
            perc_run_lost_initial=0
        fi

        printf "%-40s | %-20s | %-20s | %-20s | %-20s\n" \
          "Events Before All Cuts" "$ev_all_initial" "$ev_ge_initial" "$diff_ev_initial" "${perc_ev_lost_initial}%"
        printf "%-40s | %-20s | %-20s | %-20s | %-20s\n" \
          "Runs Before All Cuts"   "$run_all_initial" "$run_ge_initial" "$diff_run_initial" "${perc_run_lost_initial}%"

        echo ""
        # After All Cuts => DST
        ev_all=${actual_events_after_createDst:-0}
        ev_ge=${actual_events_after_createDst_ge47289:-0}
        diff_ev=$(echo "$ev_all - $ev_ge" | bc)
        if (( ev_all > 0 )); then
            perc_ev_lost=$(echo "scale=2; 100.0*$diff_ev/$ev_all" | bc)
        else
            perc_ev_lost=0
        fi

        run_all=${runs_after_createDst:-0}
        run_ge=${runs_after_createDst_ge47289:-0}
        diff_run=$(echo "$run_all - $run_ge" | bc)
        if (( run_all > 0 )); then
            perc_run_lost=$(echo "scale=2; 100.0*$diff_run/$run_all" | bc)
        else
            perc_run_lost=0
        fi

        printf "%-40s | %-20s | %-20s | %-20s | %-20s\n" \
          "Events After All Cuts" "$ev_all" "$ev_ge" "$diff_ev" "${perc_ev_lost}%"
        printf "%-40s | %-20s | %-20s | %-20s | %-20s\n" \
          "Runs After All Cuts"   "$run_all" "$run_ge" "$diff_run" "${perc_run_lost}%"

        echo ""
        echo "≥47289: DST successes = ${runs_after_createDst_ge47289:-0}, events = ${actual_events_after_createDst_ge47289:-0}"

        if $REMOVE_MISSING_MAPS; then
            missing_maps_all=${total_runs_missing_bad_tower:-0}
            missing_maps_ge47289=${missing_maps_ge47289:-0}

            diff_maps=$(echo "$missing_maps_all - $missing_maps_ge47289" | bc)
            if (( missing_maps_all > 0 )); then
                perc_maps_lost=$(echo "scale=2; 100.0*$diff_maps/$missing_maps_all" | bc)
            else
                perc_maps_lost=0
            fi

            echo ""
            echo "Differences in Missing Bad Tower Map Runs:"
            printf "%-40s | %-20s | %-20s | %-20s | %-20s\n" \
              "Missing Map Runs" \
              "$missing_maps_all" \
              "$missing_maps_ge47289" \
              "$diff_maps" \
              "${perc_maps_lost}%"
        fi
    fi
}

########################################
# MAIN EXECUTION FLOW
########################################

echo -e "${BOLD}${GREEN}========================================${RESET}"
echo -e "${BOLD}${GREEN}Starting the Golden Run List Generation${RESET}"
echo -e "${BOLD}${GREEN}========================================${RESET}"

parse_arguments "$@"
set_workplace
setup_directories
clean_previous_data

extract_initial_runs
validate_golden_list

apply_incremental_cuts_header
mbd_scaledown_cut
runtime_cut
livetime_cut
missing_bad_tower_maps_step

magnet_check_step

create_list_file
clean_old_dst_lists

if ! $DONT_GENERATE_FILELISTS; then
    generate_dst_lists
    # ------------------------------------------------------------------------
    # NEW STEP: remove_problematic_segments, if 'removeBadSegments' was set
    # This must happen AFTER we generate the .list files, or there is nothing to edit!
    # ------------------------------------------------------------------------
    remove_problematic_segments
fi

apply_createDstList_cut
compute_event_counts
final_summary
print_final_totals
