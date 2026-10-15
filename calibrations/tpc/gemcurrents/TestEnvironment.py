from datetime import datetime
from environment_history import EnvironmentHistory

history = EnvironmentHistory(
    "/sphenix/user/hemmick/TemperatureAndPressure/tpc_gas_history.tsv"
)

test_time = datetime.strptime(
    "2025-12-25 11:03:45",
    "%Y-%m-%d %H:%M:%S"
).timestamp()
temperature, pressure = history.get(test_time)
print("Temperature:", temperature)
print("Pressure:", pressure)

test_time = datetime.strptime(
    "2025-12-25 11:04:45",
    "%Y-%m-%d %H:%M:%S"
).timestamp()
temperature, pressure = history.get(test_time)
print("Temperature:", temperature)
print("Pressure:", pressure)

test_time = datetime.strptime(
    "2025-12-25 11:05:45",
    "%Y-%m-%d %H:%M:%S"
).timestamp()
temperature, pressure = history.get(test_time)
print("Temperature:", temperature)
print("Pressure:", pressure)

test_time = datetime.strptime(
    "2025-12-25 11:06:45",
    "%Y-%m-%d %H:%M:%S"
).timestamp()
temperature, pressure = history.get(test_time)
print("Temperature:", temperature)
print("Pressure:", pressure)

test_time = datetime.strptime(
    "2025-12-25 11:07:45",
    "%Y-%m-%d %H:%M:%S"
).timestamp()
temperature, pressure = history.get(test_time)
print("Temperature:", temperature)
print("Pressure:", pressure)

