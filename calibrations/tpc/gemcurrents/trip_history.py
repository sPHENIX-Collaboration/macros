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
    #
    # A real TPC channel dropping essentially to zero constitutes
    # the beginning of a trip.
    # ==============================================================

    TRIP_ONSET_VMON_MAX = 10.0

    # ==============================================================
    # Field-restoration criteria
    #
    # The total voltage across every GEM stack should again be
    # correct, and each supply should have reached its presently
    # commanded voltage.
    #
    # Balanced recovery deliberately preserves the total stack
    # voltage while temporarily changing the partition between
    # individual electrodes.
    # ==============================================================

    DELTA_SET_MAX = 1.0
    DELTA_CHAN_MAX = 5.0
    DELTA_MON_MAX = 10.0

    # ==============================================================
    # Gain-restoration criteria
    #
    # The entire TPC must return to the voltage configuration that
    # existed at the last known good sample immediately before the
    # first detected trip.
    #
    # V0Set comparison is kept tighter than VMon.
    # ==============================================================

    GAIN_V0SET_TOLERANCE = 1.0
    GAIN_VMON_TOLERANCE = 5.0

    # The eight module channels
    MODULE_LAYERS = {
        "G1", "G2", "G3", "G4",
        "T1", "T2", "T3", "T4"
    }

    # Reference supplies used to determine the desired total
    # GEM-stack voltage.
    #
    # S.00.IFC is deliberately excluded.
    REFERENCE_CHANNELS = [
        "N.00.IFC",
        "N.00.OFC",
        "S.00.OFC"
    ]

    # ==============================================================
    # Constructor
    # ==============================================================

    def __init__(self, begin_time, end_time):

        self.begin_time = float(begin_time)
        self.end_time = float(end_time)

        # Raw Prometheus data
        self.v0set_metrics = []
        self.vmon_metrics = []
        self.status_metrics = []

        # Time-indexed lookup tables:
        #
        #   self.v0set_by_time[timestamp][channel] = voltage
        #   self.vmon_by_time[timestamp][channel]  = voltage
        #
        self.v0set_by_time = {}
        self.vmon_by_time = {}
        self.status_by_time = {}

        # ----------------------------------------------------------
        # Intervals during which the corresponding condition is bad.
        #
        # FieldOK = 0 inside field_bad_intervals
        # GainOK  = 0 inside gain_bad_intervals
        # ----------------------------------------------------------

        self.field_bad_intervals = []
        self.gain_bad_intervals = []

        self._load_voltage_history()
        self._build_time_tables()
        self._fill_voltage_gaps()
        self._build_startup_gain_targets()
        self._find_bad_intervals()

    # ==============================================================
    # Public interface
    # ==============================================================

    def get(self, timestamp):
        """
        Return:

            FieldOK, GainOK

        for the requested Unix timestamp.
        """

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
    # Fill gaps in the Prometheus 10-second history
    #
    # Missing monitoring samples are replaced by the immediately
    # preceding complete voltage state.  This prevents a monitoring
    # gap from being interpreted as a physical change in the TPC.
    #
    # Every contiguous gap is reported so that long gaps can be
    # identified and inspected afterward.
    # ==============================================================

    def _fill_voltage_gaps(self):

        timestamps = sorted(
            set(self.v0set_by_time.keys()) &
            set(self.vmon_by_time.keys()) &
            set(self.status_by_time.keys())
        )

        # Hmmm...
        if len(timestamps) < 2:
            return

        # Prometheus is requested on a 10-second grid.
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

            # ------------------------------------------------------
            # Carry the last observed complete state forward through
            # every missing 10-second sample.
            # ------------------------------------------------------

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
    # Build fallback gain targets for runs that begin with a trip
    #
    # Normally gain restoration is judged against the V0Set values
    # immediately preceding the trip.  If the Prometheus history
    # begins with a channel already tripped, that reference does not
    # exist.
    #
    # Healthy peer channels provide fallback targets:
    #
    #   G1       all healthy G1 channels
    #   G3       all healthy G3 channels
    #   G4 R1    all healthy R1.G4 channels
    #   G4 R2    all healthy R2.G4 channels
    #   G4 R3    all healthy R3.G4 channels
    #
    # No fallback target is defined for G2 because its operating
    # voltage is deliberately module dependent.
    # ==============================================================

    def _build_startup_gain_targets(self):

        self.startup_gain_targets = {}

        timestamps = sorted(
            set(self.v0set_by_time.keys()) &
            set(self.vmon_by_time.keys()) &
            set(self.status_by_time.keys())
        )

        if not timestamps:
            return

        # Use the first complete Prometheus snapshot.
        timestamp = timestamps[0]

        v0set = self.v0set_by_time[timestamp]
        status = self.status_by_time[timestamp]

        g1_values = []
        g3_values = []
        g4_values = {
            "R1": [],
            "R2": [],
            "R3": []
        }

        for channel, value in v0set.items():

            if not self._is_real_tpc_channel(channel):
                continue

            fields = channel.split(".")

            if len(fields) != 4:
                continue

            side, sector, radial, layer = fields

            # Ignore anyone who is already tripped.
            if channel not in status:
                continue

            if (status[channel] & self.mskON) == 0:
                continue

            if layer == "G1":
                g1_values.append(value)

            elif layer == "G3":
                g3_values.append(value)

            elif layer == "G4" and radial in g4_values:
                g4_values[radial].append(value)

        if g1_values:
            self.startup_gain_targets["G1"] = (
                statistics.median(g1_values)
            )

        if g3_values:
            self.startup_gain_targets["G3"] = (
                statistics.median(g3_values)
            )

        for radial in ("R1", "R2", "R3"):

            if g4_values[radial]:

                self.startup_gain_targets[
                    "G4_" + radial
                ] = statistics.median(
                    g4_values[radial]
                )

        print(
            "TPC_STARTUP_GAIN_TARGETS",
            self.startup_gain_targets
        )


    def _repair_startup_gain_reference(self, reference_v0set):

        for channel in reference_v0set:

            fields = channel.split(".")

            if len(fields) != 4:
                continue

            side, sector, radial, layer = fields

            if layer == "G1":
                target = self.startup_gain_targets.get("G1")

            elif layer == "G3":
                target = self.startup_gain_targets.get("G3")

            elif layer == "G4":
                target = self.startup_gain_targets.get(
                    "G4_" + radial
                )

            else:
                continue

            if target is None:
                continue

            # A large disagreement means the saved "pre-trip"
            # state was already part of the trip/recovery.
            if abs(reference_v0set[channel] - target) > 10.0:

                print(
                    "TPC_STARTUP_GAIN_REFERENCE",
                    "channel=",
                    channel,
                    "old=",
                    reference_v0set[channel],
                    "target=",
                    target
                )

                reference_v0set[channel] = target
        
    # ==============================================================
    # Channel identification
    # ==============================================================

    @staticmethod
    def _is_real_tpc_channel(channel):

        return (
            channel.startswith("N.") or
            channel.startswith("S.")
        )

    def _module_name(self, channel):
        """
        Return module name for channels such as:

            S.04.R3.G4  ->  S.04.R3

        Return None for stripe/reference/etc. channels.
        """

        fields = channel.split(".")

        if len(fields) != 4:
            return None

        side, sector, radial, layer = fields

        if layer not in self.MODULE_LAYERS:
            return None

        return side + "." + sector + "." + radial

    # ==============================================================
    # Trip onset
    # ==============================================================

    def _trip_condition(self, timestamp):

        values = self.vmon_by_time[timestamp]
        statii = self.status_by_time[timestamp]

        trip_found = False

        #  First check for a low voltage value
        for channel, value in values.items():

            # Ignore unused CAEN channels such as CHANNEL10.
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

        # Trip onset
        for channel, value in statii.items():
        
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
    # Development diagnostics while bad
    # ==============================================================

    def _report_low_channels(self, timestamp):

        values = self.vmon_by_time[timestamp]

        low_channels = []

        for channel, value in values.items():

            if not self._is_real_tpc_channel(channel):
                continue

            if abs(value) < self.TRIP_ONSET_VMON_MAX:
                low_channels.append((channel, value))

        if not low_channels:
            return

        print("BAD state:", timestamp)

        for channel, value in low_channels:
            print("   ", channel, value)

    # ==============================================================
    # Build module collection for one timestamp
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
    # Field restoration
    # ==============================================================

    def _field_restoration_condition(self, timestamp):

        v0set = self.v0set_by_time[timestamp]
        vmon = self.vmon_by_time[timestamp]

        # ----------------------------------------------------------
        # Determine nominal total GEM-stack voltage.
        # ----------------------------------------------------------

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

        # ----------------------------------------------------------
        # Check every GEM module.
        # ----------------------------------------------------------

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

                # Supply should have reached its CURRENT command.
                #
                # During balanced recovery this does NOT require
                # return to nominal gain voltages.
                if (
                    abs(mon_voltage - set_voltage) >=
                    self.DELTA_CHAN_MAX
                ):
                    return False

            # Requested stack total should be correct.
            if (
                abs(sum_v0set - vtarget) >=
                self.DELTA_SET_MAX
            ):
                return False

            # Actual monitored stack total should be correct.
            if (
                abs(sum_vmon - vtarget) >=
                self.DELTA_MON_MAX
            ):
                return False

        return True

    # ==============================================================
    # Capture last-known-good full-TPC state
    # ==============================================================

    def _capture_last_good_state(self, timestamp):

        reference_v0set = {}
        reference_vmon = {}
        reference_status = {}

        v0set = self.v0set_by_time[timestamp]
        vmon = self.vmon_by_time[timestamp]
        status = self.status_by_time[timestamp]

        for channel, value in v0set.items():

            if not self._is_real_tpc_channel(channel):
                continue

            if channel not in vmon:
                continue

            if channel not in status:
                continue

            reference_v0set[channel] = value
            reference_vmon[channel] = vmon[channel]
            reference_status[channel] = status[channel]

        return reference_v0set, reference_vmon, reference_status

    # ==============================================================
    # Gain restoration
    # ==============================================================

    def _gain_restoration_condition(
        self,
        timestamp,
        reference_v0set,
        reference_vmon,
        reference_status
    ):

        current_v0set = self.v0set_by_time[timestamp]
        current_vmon = self.vmon_by_time[timestamp]
        current_status = self.status_by_time[timestamp]

        # ----------------------------------------------------------
        # Compare EVERY real TPC channel against the entire
        # last-known-good state.
        #
        # This automatically handles clustered trips:
        # even a channel that falls after the first detected trip
        # must eventually return to its previous good state.
        # ----------------------------------------------------------

        for channel in reference_v0set.keys():

            if channel not in current_v0set:
                return False

            if channel not in current_vmon:
                return False

            if (
                abs(
                    current_v0set[channel] -
                    reference_v0set[channel]
                )
                >= self.GAIN_V0SET_TOLERANCE
            ):
                print("GAIN_FAIL", timestamp, channel, "V0Set", reference_v0set[channel], "->", current_v0set[channel], "delta=", current_v0set[channel] - reference_v0set[channel])
                return False

            if (
                abs(
                    current_vmon[channel] -
                    reference_v0set[channel]
                )
                >= self.GAIN_VMON_TOLERANCE
            ):
                print("GAIN_FAIL", timestamp, channel, "VMon", reference_vmon[channel], "->", current_vmon[channel], "delta=", current_vmon[channel] - reference_vmon[channel])
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

        # ----------------------------------------------------------
        # State
        # ----------------------------------------------------------

        bad = False

        bad_start = None

        previous_timestamp = None

        # Frozen full-TPC state immediately before trip onset.
        reference_v0set = None
        reference_vmon = None
        reference_status = None

        # ----------------------------------------------------------
        # Field restoration state
        # ----------------------------------------------------------

        field_restored = False

        field_restore_count = 0
        field_restore_start = None

        # ----------------------------------------------------------
        # Gain restoration state
        # ----------------------------------------------------------

        gain_restore_count = 0
        gain_restore_start = None

        # ==========================================================
        # Loop over detailed 10-second samples
        # ==========================================================

        for timestamp in timestamps:

            # ------------------------------------------------------
            # GOOD STATE
            # ------------------------------------------------------

            if not bad:

                if self._trip_condition(timestamp):

                    bad = True
                    bad_start = timestamp

                    field_restored = False

                    field_restore_count = 0
                    field_restore_start = None

                    gain_restore_count = 0
                    gain_restore_start = None

                    # ------------------------------------------------
                    # The timestamp immediately preceding the first
                    # observed trip is our last-known-good state.
                    # ------------------------------------------------

                    if previous_timestamp is not None:

                        (
                            reference_v0set,
                            reference_vmon,
                            reference_status
                        ) = self._capture_last_good_state(
                            previous_timestamp
                        )
                        
                        self._repair_startup_gain_reference(
                            reference_v0set
                        )
                    else:

                        # Query began already bad.
                        #
                        # There is no preceding sample from which to
                        # establish gain reference conditions.
                        reference_v0set = None
                        reference_vmon = None
                        reference_status = None

                    print(
                        "TPC_TRIP_ONSET",
                        "time=",
                        timestamp
                    )

            # ------------------------------------------------------
            # BAD / RECOVERY STATE
            # ------------------------------------------------------

            else:

                self._report_low_channels(timestamp)

                # ==================================================
                # FIELD RESTORATION
                # ==================================================

                field_condition = (
                    self._field_restoration_condition(timestamp)
                )

                if field_condition:

                    if field_restore_count == 0:
                        field_restore_start = timestamp

                    field_restore_count += 1

                    if (
                        not field_restored and
                        field_restore_count >=
                        self.RESTORE_SAMPLES
                    ):

                        field_restored = True

                        # Record the FIRST of the six stable samples.
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

                    # Until FieldRestoration has actually been
                    # established, consecutive-sample counting resets.
                    if not field_restored:
                        field_restore_count = 0
                        field_restore_start = None

                # ==================================================
                # GAIN RESTORATION
                # ==================================================

                if (
                    reference_v0set is not None and
                    reference_vmon is not None
                ):

                    gain_condition = (
                        self._gain_restoration_condition(
                            timestamp,
                            reference_v0set,
                            reference_vmon,
                            reference_status
                        )
                    )

                else:

                    gain_condition = False

                if gain_condition:

                    if gain_restore_count == 0:
                        gain_restore_start = timestamp

                    gain_restore_count += 1

                    if (
                        gain_restore_count >=
                        self.RESTORE_SAMPLES
                    ):

                        # ------------------------------------------
                        # Gain restoration is the final transition
                        # back to the GOOD state.
                        #
                        # As with FieldRestoration, use the FIRST
                        # of the six stable samples.
                        # ------------------------------------------

                        gain_restore_time = gain_restore_start

                        self.gain_bad_intervals.append(
                            (
                                bad_start,
                                gain_restore_time
                            )
                        )

                        # In the physically expected ordering,
                        # GainRestored implies FieldRestored.
                        #
                        # If numerical details somehow cause Gain
                        # to pass before Field, force FieldOK at the
                        # same point rather than permit the
                        # nonsensical state:
                        #
                        #     FieldOK = 0
                        #     GainOK  = 1
                        #
                        if not field_restored:

                            field_restored = True

                            self.field_bad_intervals.append(
                                (
                                    bad_start,
                                    gain_restore_time
                                )
                            )

                            print(
                                "TPC_FIELD_RESTORED",
                                "time=",
                                gain_restore_time,
                                "dt=",
                                gain_restore_time - bad_start,
                                "forced_by_gain=1"
                            )

                        print(
                            "TPC_GAIN_RESTORED",
                            "time=",
                            gain_restore_time,
                            "dt=",
                            gain_restore_time - bad_start
                        )

                        # ------------------------------------------
                        # Return to normal operation.
                        # ------------------------------------------

                        bad = False
                        bad_start = None

                        reference_v0set = None
                        reference_vmon = None
                        reference_status = None

                        field_restored = False

                        field_restore_count = 0
                        field_restore_start = None

                        gain_restore_count = 0
                        gain_restore_start = None

                else:

                    gain_restore_count = 0
                    gain_restore_start = None

            previous_timestamp = timestamp

        # ==========================================================
        # Query ended while still bad
        # ==========================================================

        if bad:

            # If FieldRestoration was never achieved, FieldOK
            # remains false through the end of the query.
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

            # Gain is certainly still bad.
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
                self.end_time - bad_start
            )
            
