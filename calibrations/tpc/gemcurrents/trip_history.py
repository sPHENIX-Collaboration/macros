#!/usr/bin/env python3

import requests
import statistics


class TripHistory:

    # ==============================================================
    # Prometheus / timing configuration
    # ==============================================================

    PROMETHEUS_URL = (
        "http://promspx01.sdcc.bnl.local:9090/api/v1/query_range"
    )

    PROMETHEUS_STEP = "10s"
    RESTORE_SAMPLES = 6

    mskON = 0b000000000001

    # ==============================================================
    # Trip onset
    # ==============================================================

    TRIP_ONSET_VMON_MAX = 10.0

    # More than one GEM tripped at the same sample is treated as a
    # beam-loss / massive-trip condition.  Such a run is rejected
    # from that point to the end rather than allowing a common-mode
    # standby state to masquerade as a recovered gain configuration.
    MASSIVE_TRIP_GEM_COUNT = 300  # 289 or greater defeats this feature...

    # ==============================================================
    # Field-restoration criteria
    #
    # These retain the existing FieldOK definition:
    #
    #   1. commanded total stack voltage is correct
    #   2. every supply has reached its CURRENT command
    #   3. monitored total stack voltage is correct
    #
    # Balanced recovery is therefore allowed.
    # ==============================================================

    DELTA_SET_MAX = 1.0
    DELTA_CHAN_MAX = 5.0
    DELTA_MON_MAX = 10.0

    MODULE_LAYERS = {
        "G1", "G2", "G3", "G4",
        "T1", "T2", "T3", "T4"
    }

    REFERENCE_CHANNELS = [
        "N.00.IFC",
        "N.00.OFC",
        "S.00.OFC"
    ]

    # ==============================================================
    # Gain-restoration criteria
    #
    # GainOK is now a MOVING-TARGET test.
    #
    # For each GEM channel:
    #
    #       W_i = N_i * V_i
    #
    # and W_i is compared with the live median of healthy peers.
    #
    # G1 : one global peer group, N = 1
    # G2 : one global peer group, module-dependent N
    # G3 : one global peer group, N = 1
    # G4 : separate R1/R2/R3 peer groups, N = 1
    #
    # T1-T4 do NOT enter GainOK.  They remain fully represented in
    # FieldOK, which tests the complete eight-channel stack.
    # ==============================================================

    GAIN_V0SET_TOLERANCE = 1.0
    GAIN_VMON_TOLERANCE = 5.0

    GEM_LAYERS = {"G1", "G2", "G3", "G4"}

    # Excluding the channel being tested, the maximum peer counts are
    # 71 for G1/G2/G3 and 23 for each radial G4 group.
    MIN_GAIN_PEERS = {
        "G1": 68,
        "G2": 68,
        "G3": 68,
        "G4_R1": 22,
        "G4_R2": 22,
        "G4_R3": 22,
    }

    # ==============================================================
    # G2 normalization
    #
    # Generated directly from the final stable
    # ISOBUTANE-OPERATE-GAIN-BALANCED.json configuration:
    #
    #       N_i = 285.6 V / V_i(reference)
    #
    # so that a healthy G2 population occupies a common W coordinate.
    # ==============================================================

    G2_REFERENCE_VOLTAGE = 285.6

    G2_NORMALIZATION = {
        "N.01.R1.G2": 0.950415973,
        "N.01.R2.G2": 1.032537961,
        "N.01.R3.G2": 0.998252359,
        "N.02.R1.G2": 1.019635844,
        "N.02.R2.G2": 1.015647226,
        "N.02.R3.G2": 1.025862069,
        "N.03.R1.G2": 0.992011115,
        "N.03.R2.G2": 1.059740260,
        "N.03.R3.G2": 1.004219409,
        "N.04.R1.G2": 0.968135593,
        "N.04.R2.G2": 0.989948007,
        "N.04.R3.G2": 1.004219409,
        "N.05.R1.G2": 0.966824645,
        "N.05.R2.G2": 1.013484741,
        "N.05.R3.G2": 1.038923245,
        "N.06.R1.G2": 0.979423868,
        "N.06.R2.G2": 1.036284470,
        "N.06.R3.G2": 0.989948007,
        "N.07.R1.G2": 0.957746479,
        "N.07.R2.G2": 1.016008538,
        "N.07.R3.G2": 1.005633803,
        "N.08.R1.G2": 0.974079127,
        "N.08.R2.G2": 1.026968716,
        "N.08.R3.G2": 1.005633803,
        "N.09.R1.G2": 0.997555012,
        "N.09.R2.G2": 1.018908313,
        "N.09.R3.G2": 0.998252359,
        "N.10.R1.G2": 0.956143288,
        "N.10.R2.G2": 0.995121951,
        "N.10.R3.G2": 1.028818444,
        "N.11.R1.G2": 1.013484741,
        "N.11.R2.G2": 1.011689692,
        "N.11.R3.G2": 1.040816327,
        "N.12.R1.G2": 0.983471074,
        "N.12.R2.G2": 1.064480060,
        "N.12.R3.G2": 1.030674847,
        "S.01.R1.G2": 1.008830802,
        "S.01.R2.G2": 1.024390244,
        "S.01.R3.G2": 1.025125628,
        "S.02.R1.G2": 0.981106149,
        "S.02.R2.G2": 1.037037037,
        "S.02.R3.G2": 0.985167299,
        "S.03.R1.G2": 0.994428969,
        "S.03.R2.G2": 1.023289144,
        "S.03.R3.G2": 1.048073394,
        "S.04.R1.G2": 0.990634755,
        "S.04.R2.G2": 0.987551867,
        "S.04.R3.G2": 1.019635844,
        "S.05.R1.G2": 1.000000000,
        "S.05.R2.G2": 0.970438328,
        "S.05.R3.G2": 0.967807523,
        "S.06.R1.G2": 0.989262210,
        "S.06.R2.G2": 0.964864865,
        "S.06.R3.G2": 0.977078344,
        "S.07.R1.G2": 1.012048193,
        "S.07.R2.G2": 1.000000000,
        "S.07.R3.G2": 0.983471074,
        "S.08.R1.G2": 0.976744186,
        "S.08.R2.G2": 0.962588473,
        "S.08.R3.G2": 0.964864865,
        "S.09.R1.G2": 0.949783838,
        "S.09.R2.G2": 1.049228508,
        "S.09.R3.G2": 1.017094017,
        "S.10.R1.G2": 1.012048193,
        "S.10.R2.G2": 1.014204545,
        "S.10.R3.G2": 1.000700771,
        "S.11.R1.G2": 0.961292494,
        "S.11.R2.G2": 1.017456359,
        "S.11.R3.G2": 0.996858639,
        "S.12.R1.G2": 0.974744027,
        "S.12.R2.G2": 0.984827586,
        "S.12.R3.G2": 0.975743082,
    }

    # --------------------------------------------------------------
    # One historical exception.
    #
    # The physics-run scan shows:
    #
    #   run 73061 : last observed early S.12.R3.G2 setting
    #   run 73816 : first observed final S.12.R3.G2 setting
    #
    # There are no operative physics runs between them in our list.
    #
    # Before 73816, S.12.R3.G2 used 288.9 V in the archived balanced
    # configuration.  From 73816 onward it belongs to the final 292.7 V
    # normalization already stored above.
    # --------------------------------------------------------------

    S12R3_G2 = "S.12.R3.G2"
    S12R3_FINAL_FIRST_RUN = 73816
    S12R3_EARLY_REFERENCE_VOLTAGE = 288.9

    # ==============================================================
    # Constructor
    # ==============================================================

    def __init__(self, begin_time, end_time, runnumber):

        self.begin_time = float(begin_time)
        self.end_time = float(end_time)
        self.runnumber = int(runnumber)

        self.v0set_metrics = []
        self.vmon_metrics = []
        self.status_metrics = []

        self.v0set_by_time = {}
        self.vmon_by_time = {}
        self.status_by_time = {}

        self.field_bad_intervals = []
        self.gain_bad_intervals = []

        self.expected_gem_channels = self._build_expected_gem_channels()

        self._load_voltage_history()
        self._build_time_tables()
        self._fill_voltage_gaps()
        self._find_bad_intervals()

    # ==============================================================
    # Public interface
    # ==============================================================

    def get(self, timestamp):

        timestamp = float(timestamp)

        field_ok = 1
        gain_ok = 1

        for begin_bad, end_bad in self.field_bad_intervals:
            if begin_bad <= timestamp <= end_bad:
                field_ok = 0
                break

        for begin_bad, end_bad in self.gain_bad_intervals:
            if begin_bad <= timestamp <= end_bad:
                gain_ok = 0
                break

        return field_ok, gain_ok

    # ==============================================================
    # Prometheus
    # ==============================================================

    def _query_prometheus(self, variable_name):

        params = {
            "query": variable_name,
            "start": self.begin_time,
            "end": self.end_time,
            "step": self.PROMETHEUS_STEP
        }

        response = requests.get(
            self.PROMETHEUS_URL,
            params=params
        )

        if response.status_code != 200:

            print(
                "TPC_PROMETHEUS_ERROR",
                variable_name,
                "HTTP",
                response.status_code
            )

            return []

        result_json = response.json()

        if result_json.get("status") != "success":

            print(
                "TPC_PROMETHEUS_ERROR",
                variable_name,
                "query unsuccessful"
            )

            return []

        return result_json["data"]["result"]

    def _load_voltage_history(self):

        self.v0set_metrics = self._query_prometheus(
            "sphenix_tpc_hv_caen_V0Set"
        )

        self.vmon_metrics = self._query_prometheus(
            "sphenix_tpc_hv_caen_VMon"
        )

        self.status_metrics = self._query_prometheus(
            "sphenix_tpc_hv_caen_Status"
        )

        print("V0Set series:", len(self.v0set_metrics))
        print("VMon series:", len(self.vmon_metrics))
        print("Status series:", len(self.status_metrics))

    # ==============================================================
    # Convert Prometheus output into time-indexed dictionaries
    # ==============================================================

    def _build_time_tables(self):

        for metric in self.v0set_metrics:

            channel = metric["metric"].get("ch_name", "unknown")

            for timestamp, value in metric["values"]:

                timestamp = float(timestamp)

                if timestamp not in self.v0set_by_time:
                    self.v0set_by_time[timestamp] = {}

                self.v0set_by_time[timestamp][channel] = float(value)

        for metric in self.vmon_metrics:

            channel = metric["metric"].get("ch_name", "unknown")

            for timestamp, value in metric["values"]:

                timestamp = float(timestamp)

                if timestamp not in self.vmon_by_time:
                    self.vmon_by_time[timestamp] = {}

                self.vmon_by_time[timestamp][channel] = float(value)

        for metric in self.status_metrics:

            channel = metric["metric"].get("ch_name", "unknown")

            for timestamp, value in metric["values"]:

                timestamp = float(timestamp)

                if timestamp not in self.status_by_time:
                    self.status_by_time[timestamp] = {}

                self.status_by_time[timestamp][channel] = int(float(value))

    # ==============================================================
    # Fill internal Prometheus gaps with last observation carried
    # forward.  V0Set, VMon and Status move together as one state.
    # ==============================================================

    def _fill_voltage_gaps(self):

        timestamps = sorted(
            set(self.v0set_by_time.keys()) &
            set(self.vmon_by_time.keys()) &
            set(self.status_by_time.keys())
        )

        if len(timestamps) < 2:
            return

        step = 10
        gap_number = 0

        for previous_time, next_time in zip(
            timestamps[:-1],
            timestamps[1:]
        ):

            gap_width = (
                int(round((next_time - previous_time) / step)) - 1
            )

            if gap_width <= 0:
                continue

            gap_number += 1

            print(
                "TPC_PROM_GAP",
                "gap=",
                gap_number,
                "width=",
                gap_width,
                "begin=",
                previous_time,
                "end=",
                next_time
            )

            for i in range(1, gap_width + 1):

                timestamp = previous_time + i * step

                self.v0set_by_time[timestamp] = (
                    self.v0set_by_time[previous_time].copy()
                )

                self.vmon_by_time[timestamp] = (
                    self.vmon_by_time[previous_time].copy()
                )

                self.status_by_time[timestamp] = (
                    self.status_by_time[previous_time].copy()
                )

    # ==============================================================
    # Channel helpers
    # ==============================================================

    @staticmethod
    def _is_real_tpc_channel(channel):

        return (
            channel.startswith("N.") or
            channel.startswith("S.")
        )

    @classmethod
    def _is_gem_channel(cls, channel):

        fields = channel.split(".")

        return (
            len(fields) == 4 and
            fields[3] in cls.GEM_LAYERS
        )

    @staticmethod
    def _build_expected_gem_channels():

        channels = []

        for side in ("N", "S"):

            for sector in range(1, 13):

                for radial in ("R1", "R2", "R3"):

                    for layer in ("G1", "G2", "G3", "G4"):

                        channels.append(
                            f"{side}.{sector:02d}.{radial}.{layer}"
                        )

        return channels

    def _module_name(self, channel):

        fields = channel.split(".")

        if len(fields) != 4:
            return None

        side, sector, radial, layer = fields

        if layer not in self.MODULE_LAYERS:
            return None

        return side + "." + sector + "." + radial

    def _gain_group(self, channel):

        fields = channel.split(".")

        if len(fields) != 4:
            return None

        side, sector, radial, layer = fields

        if layer in ("G1", "G2", "G3"):
            return layer

        if layer == "G4" and radial in ("R1", "R2", "R3"):
            return "G4_" + radial

        return None

    # ==============================================================
    # Normalization
    # ==============================================================

    def _normalization(self, channel):

        fields = channel.split(".")

        if len(fields) != 4:
            return None

        layer = fields[3]

        if layer != "G2":
            return 1.0

        if channel not in self.G2_NORMALIZATION:
            return None

        if (
            channel == self.S12R3_G2 and
            self.runnumber < self.S12R3_FINAL_FIRST_RUN
        ):
            return (
                self.G2_REFERENCE_VOLTAGE /
                self.S12R3_EARLY_REFERENCE_VOLTAGE
            )

        return self.G2_NORMALIZATION[channel]

    # ==============================================================
    # Trip onset
    # ==============================================================

    def _tripped_gem_channels(self, timestamp):

        vmon = self.vmon_by_time[timestamp]
        status = self.status_by_time[timestamp]

        tripped = set()

        for channel in self.expected_gem_channels:

            if channel in vmon:

                if abs(vmon[channel]) < self.TRIP_ONSET_VMON_MAX:
                    tripped.add(channel)

            if channel in status:

                if (status[channel] & self.mskON) == 0:
                    tripped.add(channel)

        return sorted(tripped)

    def _trip_condition(self, timestamp):

        vmon = self.vmon_by_time[timestamp]
        status = self.status_by_time[timestamp]

        trip_found = False

        # Preserve the original broad trip-onset test over all real TPC
        # HV channels.  Gain recovery itself is GEM-only.
        for channel, value in vmon.items():

            if not self._is_real_tpc_channel(channel):
                continue

            if abs(value) < self.TRIP_ONSET_VMON_MAX:

                print(
                    "TRIP candidate:",
                    timestamp,
                    channel,
                    value
                )

                trip_found = True

        for channel, value in status.items():

            if not self._is_real_tpc_channel(channel):
                continue

            if (value & self.mskON) == 0:

                print(
                    "TRIP candidate:",
                    timestamp,
                    channel,
                    value
                )

                trip_found = True

        return trip_found

    # ==============================================================
    # Build module collection for FieldOK
    # ==============================================================

    def _build_modules(self, timestamp):

        v0set = self.v0set_by_time[timestamp]

        modules = {}

        for channel in v0set.keys():

            if not self._is_real_tpc_channel(channel):
                continue

            module = self._module_name(channel)

            if module is None:
                continue

            layer = channel.split(".")[3]

            if module not in modules:
                modules[module] = {}

            modules[module][layer] = channel

        return modules

    # ==============================================================
    # Field restoration -- unchanged physics
    # ==============================================================

    def _field_restoration_condition(self, timestamp):

        v0set = self.v0set_by_time[timestamp]
        vmon = self.vmon_by_time[timestamp]

        reference_values = []

        for channel in self.REFERENCE_CHANNELS:

            if channel not in v0set:

                print(
                    "FIELD_RESTORE missing reference:",
                    timestamp,
                    channel
                )

                return False

            reference_values.append(v0set[channel])

        vtarget = (
            sum(reference_values) /
            len(reference_values)
        )

        modules = self._build_modules(timestamp)

        for module, channels in modules.items():

            if len(channels) != 8:

                print(
                    "FIELD_RESTORE incomplete module:",
                    timestamp,
                    module,
                    len(channels)
                )

                return False

            sum_v0set = 0.0
            sum_vmon = 0.0

            for layer in self.MODULE_LAYERS:

                if layer not in channels:
                    return False

                channel = channels[layer]

                if channel not in vmon:
                    return False

                set_voltage = v0set[channel]
                mon_voltage = vmon[channel]

                sum_v0set += set_voltage
                sum_vmon += mon_voltage

                if (
                    abs(mon_voltage - set_voltage) >=
                    self.DELTA_CHAN_MAX
                ):
                    return False

            if (
                abs(sum_v0set - vtarget) >=
                self.DELTA_SET_MAX
            ):
                return False

            if (
                abs(sum_vmon - vtarget) >=
                self.DELTA_MON_MAX
            ):
                return False

        return True

    # ==============================================================
    # Live normalized gain targets
    # ==============================================================

    def _healthy_gain_population(self, timestamp):

        v0set = self.v0set_by_time[timestamp]
        status = self.status_by_time[timestamp]

        groups = {
            "G1": [],
            "G2": [],
            "G3": [],
            "G4_R1": [],
            "G4_R2": [],
            "G4_R3": [],
        }

        for channel in self.expected_gem_channels:

            if channel not in v0set:
                continue

            if channel not in status:
                continue

            if (status[channel] & self.mskON) == 0:
                continue

            group = self._gain_group(channel)
            normalization = self._normalization(channel)

            if group is None or normalization is None:
                continue

            groups[group].append(
                (
                    channel,
                    normalization * v0set[channel]
                )
            )

        return groups

    def _gain_restoration_condition(self, timestamp):

        v0set = self.v0set_by_time[timestamp]
        vmon = self.vmon_by_time[timestamp]
        status = self.status_by_time[timestamp]

        groups = self._healthy_gain_population(timestamp)

        # ----------------------------------------------------------
        # Every GEM must be present, ON, near its live peer target,
        # and actually sitting near its own current command.
        # ----------------------------------------------------------

        for channel in self.expected_gem_channels:

            if channel not in v0set:
                return False

            if channel not in vmon:
                return False

            if channel not in status:
                return False

            if (status[channel] & self.mskON) == 0:
                return False

            group = self._gain_group(channel)
            normalization = self._normalization(channel)

            if group is None or normalization is None:
                return False

            # Exclusive peer median: do not let the channel being tested
            # vote on where its own target should be.
            peers = [
                w
                for peer_channel, w in groups[group]
                if peer_channel != channel
            ]

            if len(peers) < self.MIN_GAIN_PEERS[group]:
                return False

            target_w = statistics.median(peers)

            set_w = normalization * v0set[channel]
            mon_w = normalization * vmon[channel]

            if (
                abs(set_w - target_w) >=
                self.GAIN_V0SET_TOLERANCE
            ):
                return False

            if (
                abs(mon_w - set_w) >=
                self.GAIN_VMON_TOLERANCE
            ):
                return False

        return True

    # ==============================================================
    # Main state machine
    # ==============================================================

    def _find_bad_intervals(self):

        timestamps = sorted(
            set(self.v0set_by_time.keys()) &
            set(self.vmon_by_time.keys()) &
            set(self.status_by_time.keys())
        )

        if not timestamps:

            print("TPC_TRIP_HISTORY no common timestamps")
            return

        bad = False
        bad_start = None
        massive_trip = False

        field_restored = False
        field_restore_count = 0
        field_restore_start = None
        field_restore_time = None

        gain_restore_count = 0
        gain_restore_start = None

        for timestamp in timestamps:

            # ======================================================
            # GOOD STATE
            # ======================================================

            if not bad:

                if not self._trip_condition(timestamp):
                    continue

                bad = True
                bad_start = timestamp

                field_restored = False
                field_restore_count = 0
                field_restore_start = None
                field_restore_time = None

                gain_restore_count = 0
                gain_restore_start = None

                tripped_gems = self._tripped_gem_channels(timestamp)

                massive_trip = (
                    len(tripped_gems) >=
                    self.MASSIVE_TRIP_GEM_COUNT
                )

                print(
                    "TPC_TRIP_ONSET",
                    "time=",
                    timestamp,
                    "gem_count=",
                    len(tripped_gems)
                )

                if massive_trip:

                    print(
                        "TPC_MASSIVE_TRIP",
                        "time=",
                        timestamp,
                        "gem_count=",
                        len(tripped_gems)
                    )

                continue

            # ======================================================
            # MASSIVE TRIP
            #
            # Never allow a later common-mode voltage state to be
            # mistaken for gain recovery.
            # ======================================================

            if massive_trip:
                continue

            # ======================================================
            # FIELD RESTORATION
            # ======================================================

            field_condition = (
                self._field_restoration_condition(timestamp)
            )

            if field_condition:

                if field_restore_count == 0:
                    field_restore_start = timestamp

                field_restore_count += 1

                if (
                    not field_restored and
                    field_restore_count >= self.RESTORE_SAMPLES
                ):

                    field_restored = True
                    field_restore_time = field_restore_start

                    self.field_bad_intervals.append(
                        (
                            bad_start,
                            field_restore_time
                        )
                    )

                    print(
                        "TPC_FIELD_RESTORED",
                        "time=",
                        field_restore_time,
                        "dt=",
                        field_restore_time - bad_start
                    )

            else:

                if not field_restored:
                    field_restore_count = 0
                    field_restore_start = None

            # ======================================================
            # GAIN RESTORATION
            #
            # This is independent of any frozen pre-trip snapshot.
            # Each sample asks whether all GEMs agree with their live
            # healthy peer populations in normalized W coordinates.
            # ======================================================

            gain_condition = (
                self._gain_restoration_condition(timestamp)
            )

            if gain_condition:

                if gain_restore_count == 0:
                    gain_restore_start = timestamp

                gain_restore_count += 1

            else:

                gain_restore_count = 0
                gain_restore_start = None

            # GainOK may not return before FieldOK.
            if (
                field_restored and
                gain_restore_count >= self.RESTORE_SAMPLES
            ):

                gain_restore_time = max(
                    gain_restore_start,
                    field_restore_time
                )

                self.gain_bad_intervals.append(
                    (
                        bad_start,
                        gain_restore_time
                    )
                )

                print(
                    "TPC_GAIN_RESTORED",
                    "time=",
                    gain_restore_time,
                    "dt=",
                    gain_restore_time - bad_start
                )

                bad = False
                bad_start = None
                massive_trip = False

                field_restored = False
                field_restore_count = 0
                field_restore_start = None
                field_restore_time = None

                gain_restore_count = 0
                gain_restore_start = None

        # ==========================================================
        # Query ended while still bad
        # ==========================================================

        if bad:

            if not field_restored:

                self.field_bad_intervals.append(
                    (
                        bad_start,
                        self.end_time
                    )
                )

                print(
                    "TPC_FIELD_NOT_RESTORED",
                    "trip_time=",
                    bad_start,
                    "query_end=",
                    self.end_time,
                    "dt=",
                    self.end_time - bad_start
                )

            self.gain_bad_intervals.append(
                (
                    bad_start,
                    self.end_time
                )
            )

            print(
                "TPC_GAIN_NOT_RESTORED",
                "trip_time=",
                bad_start,
                "query_end=",
                self.end_time,
                "dt=",
                self.end_time - bad_start,
                "massive=",
                int(massive_trip)
            )
