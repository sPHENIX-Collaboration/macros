# Generate a forward-drift LUT with Condor

This SDCC helper splits generation into independent jobs and merges their ROOT
files. It requires Python 3.9+, Condor and the installed coresoftware executables
`PHGarfieldForwardMapGenerator` and `PHGarfieldCompactMapMerger`. Until
[coresoftware PR #4524](https://github.com/sPHENIX-Collaboration/coresoftware/pull/4524)
is merged, build its current branch.

Run these commands from this `ForwardLUT` directory.

1. Copy the example to any writable location. This settings path is only an example:

   ```sh
   mkdir -p "$HOME/tpc_forward_lut"
   cp example.json "$HOME/tpc_forward_lut/settings.json"
   ```

   Edit that copy. Set `install_prefix` to your
   coresoftware installation; an empty value uses `MYINSTALL` if set, otherwise
   the selected release. The example uses available CVMFS inputs and the GAP
   magnetic map; use the same magnetic payload and scale in Geant. Change the
   release or physics settings for your setup. Keep the three grid sizes explicit.
2. Choose a **new shared output directory** visible to the submit and worker hosts.
   `--work-dir` names that directory; the helper creates it for jobs, logs and
   generated LUT files. The settings file can live elsewhere. Replace both
   example paths below with your preferred locations:

   ```sh
   python3 lut_jobs.py prepare --config "$HOME/tpc_forward_lut/settings.json" \
     --jobs 200 --work-dir "$HOME/lut_run_01"
   ```

   This checks the inputs, writes the jobs and ordered shard list, and prints
   the exact submission and merge commands. It does not submit anything.
3. Run the printed `condor_submit` command. Check progress with `condor_q`;
   job stdout/stderr are under the work directory's `logs/`. After all jobs
   succeed, run the printed merge command. The result is `runtime.root`.

All jobs use identical saved settings and a copied helper. Keep the selected
software installation unchanged until they finish. Jobs start at 4 GB, increasing
by 2 GB after a memory eviction, up to 16 GB. `prepare --memory 4GB --max-memory 16GB`
sets the starting request and ceiling. This uses
[HTCondor's memory retry policy](https://htcondor.readthedocs.io/en/25.x/man-pages/htcondor-jdl.html#retry_request_memory_increase).
Each retry reruns its shard from the beginning. The request does not change the
generator's allocation caps. More jobs reduce the roots per job;
they do not change interpolation accuracy or the total refinement limits per root.

The helper refuses existing work directories and outputs. Other failures, or
memory evictions at the ceiling, remain held. Merging requires every shard to have completed
successfully; the coresoftware merger checks table compatibility and coverage.
This prepares LUTs only; it does not submit simulation or analysis campaigns.
