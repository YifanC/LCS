import matplotlib.pyplot as plt
import numpy as np
import plotly.plotly as py
from matplotlib.legend_handler import HandlerLine2D

py.sign_in("maluethi", "vjbjy0lj5w")

def read_datafile(file_name):
    # the skiprows keyword is for heading, but I don't know if trailing lines
    # can be specified
    data = np.loadtxt(file_name, delimiter=';', skiprows=0)
    return data


sysTimeRaw = read_datafile("systemtime_heavyload.log")
sysTime = sysTimeRaw[:, 1] + sysTimeRaw[:, 2]/1E6
deltaSysTime = (sysTime[1:] - sysTime[:-1])*1000

plt.plot(deltaSysTime,"-x")
plt.xlabel("Trigger Number")
plt.ylabel("$\Delta$t [ms]")
plt.title("Time Difference Between Consecutive Triggers")
# plt.legend([hDeltaTrigger,
# hDeltaTimestamp],
# ["Recorded System Time", "Encoder Timer"],
# handler_map={hDeltaTrigger: HandlerLine2D(numpoints=1),
# hDeltaTimestamp: HandlerLine2D(numpoints=1)})

plt.grid()
fig = plt.gcf()
plt.show()
plot_url = py.plot_mpl(fig, filename='trigger_times')
