import requests


class TripHistory:

    TRIP_ONSET_VMON_MAX = 10.0
    DELTA_SET_MAX = 1.0
    DELTA_CHAN_MAX = 5.0
    DELTA_MON_MAX = 10.0
    RESTORE_SAMPLES = 6

    def __init__(self, begin_time, end_time):

        self.begin_time = begin_time
        self.end_time = end_time

        self.bad_intervals = []

        self._load_voltage_history()
        self._find_bad_intervals()


    def get(self, timestamp):

        for begin_bad, end_bad in self.bad_intervals:

            if begin_bad <= timestamp <= end_bad:
                return 1

        return 0


    def _load_voltage_history(self):
        self.v0set_metrics = self._query_prometheus('sphenix_tpc_hv_caen_V0Set')
        self.vmon_metrics  = self._query_prometheus('sphenix_tpc_hv_caen_VMon')
        

    def _query_prometheus(self, variable_name):
        params = {
            'query': variable_name,
            'start': self.begin_time,
            'end': self.end_time,
            'step': '10s'
        }
        
        resp = requests.get(
            "http://promspx01.sdcc.bnl.local:9090/api/v1/query_range",
            params=params
        )
        
        if resp.status_code != 200:
            return []
        
        result_json = resp.json()
        
        if result_json['status'] != 'success':
            return []
        
        return result_json['data']['result']



    def _find_bad_intervals(self):
    
        self.bad_intervals = []
    
        # --------------------------------------------------
        # Build lookup tables:
        #
        #   vmon_by_time[timestamp][channel] = value
        #   v0set_by_time[timestamp][channel] = value
        #
        # channel name is taken from Prometheus ch_name.
        # --------------------------------------------------
    
        vmon_by_time = {}
        v0set_by_time = {}
    
        for metric in self.vmon_metrics:
    
            labels = metric['metric']
            channel = labels.get('ch_name', 'unknown')
    
            for timestamp, value in metric['values']:
    
                timestamp = float(timestamp)
    
                if timestamp not in vmon_by_time:
                    vmon_by_time[timestamp] = {}
    
                vmon_by_time[timestamp][channel] = float(value)
    
        for metric in self.v0set_metrics:
    
            labels = metric['metric']
            channel = labels.get('ch_name', 'unknown')
    
            for timestamp, value in metric['values']:
    
                timestamp = float(timestamp)
    
                if timestamp not in v0set_by_time:
                    v0set_by_time[timestamp] = {}
    
                v0set_by_time[timestamp][channel] = float(value)
    
        # --------------------------------------------------
        # Only use timestamps for which we have both datasets.
        # --------------------------------------------------
    
        timestamps = sorted(
            set(vmon_by_time.keys()) &
            set(v0set_by_time.keys())
        )
    
        if not timestamps:
            print("No common VMon/V0Set timestamps")
            return
    
        # --------------------------------------------------
        # Helper: detector is tripped if ANY monitored
        # HV channel has collapsed below threshold.
        # --------------------------------------------------
    
        #def trip_condition(timestamp):
   
        #    values = vmon_by_time[timestamp]
    
        #    for channel, value in values.items():
    
        #        if abs(value) < self.TRIP_ONSET_VMON_MAX:
        #            return True
    
        #    return False
        def trip_condition(timestamp):
        
            values = vmon_by_time[timestamp]
            tripped = False
        
            for channel, value in values.items():
        
                # Ignore unused CAEN channels
                if not (channel.startswith("N.") or channel.startswith("S.")):
                    continue
        
                if abs(value) < self.TRIP_ONSET_VMON_MAX:
                    print(
                        "TRIP candidate:",
                        timestamp,
                        channel,
                        value
                    )
                    tripped = True
        
            return tripped
        # --------------------------------------------------
        # Prototype restoration condition.
        #
        # For now:
        #   every channel must satisfy
        #
        #       |VMon - V0Set| < DELTA_CHAN_MAX
        #
        # We can add the summed-module and Vtarget tests next.
        # --------------------------------------------------
        def restoration_condition(timestamp):
        
            vmon = vmon_by_time[timestamp]
            v0set = v0set_by_time[timestamp]
        
            # --------------------------------------------------
            # Determine the desired total module voltage.
            #
            # Use:
            #     N.00.IFC
            #     N.00.OFC
            #     S.00.OFC
            #
            # Deliberately exclude S.00.IFC.
            # --------------------------------------------------
        
            reference_channels = [
                "N.00.IFC",
                "N.00.OFC",
                "S.00.OFC"
            ]
        
            reference_values = []
        
            for channel in reference_channels:
        
                if channel not in v0set:
                    print(
                        "RESTORE: missing reference channel",
                        timestamp,
                        channel
                    )
                    return False
        
                reference_values.append(v0set[channel])
        
            vtarget = sum(reference_values) / len(reference_values)
        
            # --------------------------------------------------
            # Collect the 8 HV channels belonging to each
            # real TPC GEM module.
            #
            # Example:
            #
            #     S.04.R3.G1
            #     S.04.R3.G2
            #     ...
            #     S.04.R3.T4
            #
            # Dictionary will look like:
            #
            # modules["S.04.R3"]["G1"] = channel name
            # --------------------------------------------------
        
            modules = {}
        
            valid_layers = {
                "G1", "G2", "G3", "G4",
                "T1", "T2", "T3", "T4"
            }
        
            for channel in v0set.keys():
        
                if not (channel.startswith("N.") or
                        channel.startswith("S.")):
                    continue
        
                fields = channel.split(".")
        
                # Module channels have:
                #
                # side.sector.radial.layer
                #
                # e.g. S.04.R3.G4
                if len(fields) != 4:
                    continue
        
                side, sector, radial, layer = fields
        
                if layer not in valid_layers:
                    continue
        
                module = side + "." + sector + "." + radial
        
                if module not in modules:
                    modules[module] = {}
        
                modules[module][layer] = channel
        
            # --------------------------------------------------
            # Every module must simultaneously satisfy all
            # three restoration requirements.
            # --------------------------------------------------
        
            for module, channels in modules.items():
        
                # We require all eight channels.
                if len(channels) != 8:
                    print(
                        "RESTORE: incomplete module",
                        timestamp,
                        module,
                        "channels:",
                        len(channels)
                    )
                    return False
        
                sum_v0set = 0.0
                sum_vmon = 0.0
        
                for layer in valid_layers:
        
                    channel = channels[layer]
        
                    if channel not in vmon:
                        print(
                            "RESTORE: missing VMon",
                            timestamp,
                            channel
                        )
                        return False
        
                    set_voltage = v0set[channel]
                    mon_voltage = vmon[channel]
        
                    sum_v0set += set_voltage
                    sum_vmon += mon_voltage
        
                    # ------------------------------------------
                    # Criterion 2:
                    #
                    # Every individual HV channel must have
                    # reached its requested voltage.
                    # ------------------------------------------
        
                    if abs(mon_voltage - set_voltage) >= self.DELTA_CHAN_MAX:
                        return False
        
                # ----------------------------------------------
                # Criterion 1:
                #
                # Sum of requested voltages must correspond
                # to the proper module voltage.
                # ----------------------------------------------
        
                if abs(sum_v0set - vtarget) >= self.DELTA_SET_MAX:
                    return False
        
                # ----------------------------------------------
                # Criterion 3:
                #
                # Sum of monitored voltages must correspond
                # to the proper module voltage.
                # ----------------------------------------------
        
                if abs(sum_vmon - vtarget) >= self.DELTA_MON_MAX:
                    return False
        
            return True

        def report_low_channels(timestamp):
        
            values = vmon_by_time[timestamp]
        
            low_channels = []
        
            for channel, value in values.items():
        
                if not (channel.startswith("N.") or channel.startswith("S.")):
                    continue
        
                if abs(value) < self.TRIP_ONSET_VMON_MAX:
                    low_channels.append((channel, value))
        
            if low_channels:
                print("BAD state:", timestamp)
        
                for channel, value in low_channels:
                    print("   ", channel, value)
        # --------------------------------------------------
        # State machine
        # --------------------------------------------------
    
        bad = False
        bad_start = None
    
        restore_count = 0
        restore_start = None
    
        first_time = timestamps[0]
    
        # If we enter the query window already bad,
        # use query_begin as the beginning of the interval.
        if trip_condition(first_time):
    
            bad = True
            bad_start = self.begin_time
    
        for timestamp in timestamps:
    
            if not bad:
    
                if trip_condition(timestamp):
    
                    bad = True
                    bad_start = timestamp
    
                    restore_count = 0
                    restore_start = None
    
            else:
                report_low_channels(timestamp)
                if restoration_condition(timestamp):
    
                    if restore_count == 0:
                        restore_start = timestamp
    
                    restore_count += 1
    
                    if restore_count >= self.RESTORE_SAMPLES:
    
                        # Use the FIRST of the consecutive
                        # stable samples as the restoration time.
                        self.bad_intervals.append(
                            (bad_start, restore_start)
                        )
    
                        bad = False
                        bad_start = None
    
                        restore_count = 0
                        restore_start = None
    
                else:
    
                    restore_count = 0
                    restore_start = None
    
        # If we leave the query window still bad,
        # terminate the interval at query_end.
        if bad:
    
            self.bad_intervals.append(
                (bad_start, self.end_time)
            )
