from trip_history import TripHistory

trip = TripHistory(1765769865-12*3600, 1765773701+12*3600)

print("V0Set series:", len(trip.v0set_metrics))
print("VMon series:", len(trip.vmon_metrics))
print("Field bad intervals: ",trip.field_bad_intervals)
print("Gain  bad intervals: ",trip.gain_bad_intervals)
