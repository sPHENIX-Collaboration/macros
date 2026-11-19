#!/usr/bin/env python3
"""Prepare SDCC Condor jobs and merge an adaptive forward-drift LUT."""

import argparse
import json
import math
import os
from pathlib import Path
import re
import shlex
import shutil
import subprocess
import sys
import tempfile


def require(condition, message):
    if not condition:
        raise ValueError(message)


def load_json(path):
    with Path(path).open() as stream:
        return json.load(stream)


def write_json(path, value):
    with Path(path).open("x") as stream:
        json.dump(value, stream, indent=2, allow_nan=False)
        stream.write("\n")


def environment(config):
    release = config.get("release", "ana.578")
    require(isinstance(release, str) and re.fullmatch(r"[A-Za-z0-9_.-]+", release),
            "release must be an sPHENIX release name, such as ana.578")
    prefix = config.get("install_prefix") or os.environ.get("MYINSTALL", "")
    require(isinstance(prefix, str), "install_prefix must be a path")
    lines = ["set -eo pipefail",
             "unset MYINSTALL LD_LIBRARY_PATH ROOT_INCLUDE_PATH PYTHONPATH LD_PRELOAD",
             "source /opt/sphenix/core/bin/sphenix_setup.sh -n " + shlex.quote(release)]
    if prefix:
        prefix = str(Path(prefix).expanduser().resolve(strict=True))
        require(Path(prefix).is_dir(), "install_prefix must be a directory")
        lines += ["export MYINSTALL=" + shlex.quote(prefix),
                  'export PATH="$MYINSTALL/bin:$PATH"',
                  'export LD_LIBRARY_PATH="$MYINSTALL/lib:$MYINSTALL/lib64:${LD_LIBRARY_PATH:-}"',
                  'export ROOT_INCLUDE_PATH="$MYINSTALL/include:${ROOT_INCLUDE_PATH:-}"']
    return "\n".join(lines)


def run(setup, command, **kwargs):
    return subprocess.run(["/bin/bash", "-c", setup + '\nexec "$@"', "lut", *map(str, command)],
                          check=True, **kwargs)


def generator_arguments(config):
    options = config.get("generator_options")
    require(isinstance(options, dict), "generator_options must be a JSON object")
    forbidden = {"--output", "--adaptive-root-first", "--adaptive-root-end",
                 "--force", "--check-inputs", "--help"}
    result = []
    for option, value in options.items():
        require(re.fullmatch(r"--[a-z0-9-]+", option) and option not in forbidden,
                "Unsupported or helper-owned option: " + option)
        if isinstance(value, bool):
            if value:
                result.append(option)
        else:
            require(isinstance(value, (str, int, float)), "Invalid value for " + option)
            require(not isinstance(value, float) or math.isfinite(value), "Nonfinite value for " + option)
            result += [option, str(value)]
    # Require explicit grid sizes so the split never depends on copied CLI defaults.
    grid = []
    for option, minimum in (("--phi-nodes", 3), ("--radial-nodes", 2), ("--longitudinal-nodes", 2)):
        value = options.get(option)
        require(type(value) is int and value >= minimum,
                "generator_options must specify an integer " + option + " >= " + str(minimum))
        grid.append(value)
    return result, 2 * grid[0] * (grid[1] - 1) * (grid[2] - 1)


def partition(roots, jobs):
    require(0 < jobs <= roots, "jobs must be positive and no larger than the root count")
    return [(roots * index // jobs, roots * (index + 1) // jobs) for index in range(jobs)]


def shard(work, index):
    return work / "shards" / ("shard_%05d.root" % index)


def done(work, index):
    return work / "logs" / ("shard_%05d.done" % index)


def megabytes(value):
    match = re.fullmatch(r"([1-9][0-9]*)(MB|GB)", value)
    require(match, "Memory and disk requests must be positive MB or GB quantities")
    return int(match[1]) * (1024 if match[2] == "GB" else 1)


def prepare(args):
    config = load_json(args.config)
    require(isinstance(config, dict) and not (set(config) - {"release", "install_prefix", "generator_options"}),
            "Settings must contain release, install_prefix and generator_options only")
    options, roots = generator_arguments(config)
    ranges = partition(roots, args.jobs)
    setup = environment(config)
    work = args.work_dir.expanduser().resolve()
    require(not work.exists(), "Choose a new work directory; existing directories are preserved")
    require(not any(char in str(work) for char in '\n\r"$'),
            'Work directory must not contain newlines, double quotes or "$"')
    require(megabytes(args.max_memory) >= megabytes(args.memory),
            "max-memory must be at least the starting memory")
    megabytes(args.disk)

    probe = 'import json,shutil; print("LUT_TOOLS:"+json.dumps([shutil.which(x) for x in '
    probe += '["PHGarfieldForwardMapGenerator", "PHGarfieldCompactMapMerger"]]))'
    result = run(setup, ["/usr/bin/python3", "-c", probe], capture_output=True, text=True)
    tools = json.loads(next(line.removeprefix("LUT_TOOLS:") for line in result.stdout.splitlines()
                            if line.startswith("LUT_TOOLS:")))
    require(all(tools), "Install the forward LUT generator and merger; set install_prefix in the settings")
    help_text = run(setup, [tools[0], "--help"], capture_output=True, text=True).stdout
    require(all(flag in help_text for flag in ("--check-inputs", "--adaptive-root-first", "--adaptive-root-end")),
            "The selected generator does not support this workflow; use the forward LUT PR installation")
    print("Checking the gas, fields and generation settings...", flush=True)
    with tempfile.TemporaryDirectory(prefix="forward-lut-inputs-") as temporary:
        target = Path(temporary) / "check.root"
        result = run(setup, [tools[0], *options, "--output", target, "--check-inputs"],
                     capture_output=True, text=True)
        require(not target.exists(), "Input-only check unexpectedly generated a LUT")

    work.mkdir(parents=True)
    (work / "shards").mkdir()
    (work / "logs").mkdir()
    shutil.copyfile(Path(__file__).resolve(), work / "lut_jobs.py")
    shutil.copyfile(args.config, work / "settings.json")
    (work / "preflight.log").write_text(result.stdout + result.stderr)
    plan = dict(generator=tools[0], merger=tools[1], options=options, setup=setup,
                root_count=roots, ranges=ranges)
    write_json(work / "plan.json", plan)
    (work / "shards.txt").write_text("".join(str(shard(work, index)) + "\n" for index in range(len(ranges))))
    worker = work / "worker.sh"
    worker.write_text("#!/bin/bash\n" + setup + "\nexec /usr/bin/python3 " +
                      shlex.quote(str(work / "lut_jobs.py")) + ' _worker "$1"\n')
    worker.chmod(0o755)
    submit = work / "generate.sub"
    # The event log lives on the submit host; GPFS does not support its locking.
    event_log = "/tmp/forward-lut-%s-$(Cluster).events" % os.getuid()
    submit.write_text(
        'universe = vanilla\nexecutable = %s\narguments = $(Process)\ninitialdir = %s\n'
        'output = %s/logs/shard_$(Process).out\nerror = %s/logs/shard_$(Process).err\n'
        'log = %s\nrequest_cpus = 1\nrequest_memory = %s\n'
        'retry_request_memory_increase = 2048MB\nretry_request_memory_max = %s\nrequest_disk = %s\n'
        'requirements = (OpSysMajorVer == 9)\ngetenv = False\nshould_transfer_files = NO\n'
        'on_exit_hold = (ExitBySignal == true) || (ExitCode != 0)\nperiodic_release = False\nqueue %d\n'
        % (worker, work, work, work, event_log, args.memory, args.max_memory, args.disk, len(ranges)))
    print("Prepared %d jobs covering all %d roots. Input-check messages: %s" %
          (len(ranges), roots, work / "preflight.log"))
    print("Submit: " + shlex.join(["condor_submit", str(submit)]))
    print("After all jobs finish: " + shlex.join(["python3", str(work / "lut_jobs.py"), "merge", str(work)]))


def worker(index):
    work = Path(__file__).resolve().parent
    plan = load_json(work / "plan.json")
    require(0 <= index < len(plan["ranges"]), "Invalid job index")
    first, end = plan["ranges"][index]
    output = shard(work, index)
    if done(work, index).is_file():
        require(output.is_file(), "Completion marker exists but its shard is missing")
        return
    require(not output.exists(), "This shard already exists without a completion marker; inspect it before retrying")
    command = [plan["generator"], *plan["options"], "--adaptive-root-first", str(first),
               "--adaptive-root-end", str(end), "--output", str(output)]
    # worker.sh has already selected the frozen environment.
    subprocess.run(command, check=True)
    require(output.is_file() and output.stat().st_size > 0, "Generator did not write the requested shard")
    done(work, index).touch(exist_ok=False)


def merge(work):
    work = work.expanduser().resolve(strict=True)
    plan = load_json(work / "plan.json")
    missing = [index for index in range(len(plan["ranges"]))
               if not done(work, index).is_file() or not shard(work, index).is_file()]
    require(not missing, "Unfinished/failed shards: " + ", ".join(map(str, missing[:20])) +
            (" ..." if len(missing) > 20 else "") + "; inspect logs and condor_q before merging")
    output = work / "runtime.root"
    require(not output.exists(), "runtime.root already exists; refusing to overwrite it")
    run(plan["setup"], [plan["merger"], "--input-list", work / "shards.txt", "--output", output])
    print("Complete simulation LUT: " + str(output))


def main():
    if len(sys.argv) == 3 and sys.argv[1] == "_worker":
        worker(int(sys.argv[2]))
        return
    parser = argparse.ArgumentParser(description=__doc__)
    commands = parser.add_subparsers(dest="command", required=True)
    prep = commands.add_parser("prepare", help="Check inputs and prepare jobs; does not submit")
    prep.add_argument("--config", required=True, type=Path, help="JSON generation settings")
    prep.add_argument("--work-dir", required=True, type=Path, help="New directory visible on submit and worker hosts")
    prep.add_argument("--jobs", type=int, default=200, help="Number of independent jobs (default 200)")
    prep.add_argument("--memory", default="4GB", help="Starting memory per job (default 4GB)")
    prep.add_argument("--max-memory", default="16GB", help="Memory ceiling for 2GB increases (default 16GB)")
    prep.add_argument("--disk", default="4GB", help="Requested scratch space per job (default 4GB)")
    collect = commands.add_parser("merge", help="Merge successful shards into runtime.root")
    collect.add_argument("work_dir", type=Path)
    args = parser.parse_args()
    if args.command == "prepare":
        prepare(args)
    else:
        merge(args.work_dir)


if __name__ == "__main__":
    try:
        main()
    except subprocess.CalledProcessError as error:
        if error.stdout:
            print(error.stdout, file=sys.stderr)
        if error.stderr:
            print(error.stderr, file=sys.stderr)
        print("LUT command failed (exit %s); see output above." % error.returncode, file=sys.stderr)
        sys.exit(1)
    except (OSError, ValueError, KeyError, StopIteration) as error:
        print("LUT workflow: " + str(error), file=sys.stderr)
        sys.exit(1)
