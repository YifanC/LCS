import matplotlib.pyplot as plt
import numpy as np

import plotly.plotly as py
from matplotlib.legend_handler import HandlerLine2D
py.sign_in("maluethi", "vjbjy0lj5w")


def read_datafile(file_name):
    # the skiprows keyword is for heading, but I don't know if trailing lines
    # can be specified

    data = np.loadtxt(file_name, delimiter=':', skiprows=0)
    return data


sysTimeRaw = read_datafile("systemtime_stripped.log")
sysTime = sysTimeRaw[:, 0] + sysTimeRaw[:, 1]/1E6
deltaSysTime = (sysTime[1:] - sysTime[:-1])*1000

timeStamp = read_datafile("systemtime_timestamp.log")
deltaTimeStamp = timeStamp[1:] -timeStamp[:-1]


hDeltaTrigger, = plt.plot(deltaSysTime, "bx", label="System Time Difference")
hDeltaTimestamp, = plt.plot(deltaTimeStamp, "rx", label="Encoder Timer Difference")
plt.xlabel("Trigger Number")
plt.ylabel("Time Fifference [ms]")
plt.title("Recorded Subsequent Trigger Times")
# plt.legend([hDeltaTrigger,
#             hDeltaTimestamp],
#            ["Recorded System Time", "Encoder Timer"],
#            handler_map={hDeltaTrigger: HandlerLine2D(numpoints=1),
#                         hDeltaTimestamp: HandlerLine2D(numpoints=1)})
plt.grid()
fig = plt.gcf()
plt.show()

plot_url = py.plot_mpl(fig, filename='trigger_times')