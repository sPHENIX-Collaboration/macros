from datetime import datetime
from bisect import bisect_right


class EnvironmentHistory:

    def __init__(self, filename):

        self.times = []
        self.pressures = []
        self.temperatures = []

        start_time = datetime.strptime(
            "2025-01-01 00:00:00",
            "%Y-%m-%d %H:%M:%S"
        ).timestamp()

        with open(filename, "r") as f:

            next(f)

            for line in f:

                fields = line.split()

                if len(fields) != 4:
                    continue

                date_string = fields[0] + " " + fields[1]

                timestamp = datetime.strptime(
                    date_string,
                    "%Y-%m-%d %H:%M:%S"
                ).timestamp()

                if timestamp < start_time:
                    continue

                if fields[2] == "NULL" or fields[3] == "NULL":
                    continue

                self.times.append(timestamp)
                self.pressures.append(float(fields[2]))
                self.temperatures.append(float(fields[3]))


    def get(self, timestamp):

        index = bisect_right(self.times, timestamp) - 1

        if index < 0:
            return None, None

        return self.temperatures[index], self.pressures[index]


    
