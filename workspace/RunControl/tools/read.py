__author__ = 'matthias'

import numpy as np
import pandas
import matplotlib.pyplot as plt
# (*) To communicate with Plotly's server, sign in with credentials file
import plotly.plotly as py

# (*) Useful Python/Plotly tools
import plotly.tools as tls

RunNumber = 3165

data = pandas.read_csv("../data/Run-" + str(3165) + ".txt", delim_whitespace=True)

id = int(data.iloc[1,0] -1)

# conversion factors for linear encoder: ticks to mm and mm to angle
lin_convert = 0.3499  # conversion from mm to deg
err_convert = 0.0002  # error on coversion from mm to deg
lin_tick = 0.00001  # conversion from tick to mm

# these offsets are defined in the laser coordinate system: origin in middle of mirror, x- axis parallel to wire plane,
# y-axis along drift and z-axis parallel to field cage tubes

lin_offset = [0, - 266.60208631 + 60]  # offsets for both systems
rot_offset = [0, + 18 - 180]
power_calib = [[0,0], [8000, 12500]] # first: power minimum, second: power maximum


# reading data
i = data.iloc[:,8]
thetha = data.iloc[:, 3] * lin_tick / lin_convert + lin_offset[id]    # azimuthal angle
phi = data.iloc[:, 2]  + rot_offset[id]                                                 # polar angle
power = (data.iloc[:, 4] - power_calib[id][0]) / (power_calib[id][1] - power_calib[id][0]) * 100
time = data.iloc[:, 6] + data.iloc[:, 7] / 1000000 - data.iloc[0, 6]


# ------------ Plotting ------------
f, axarr = plt.subplots(3, sharex=True)

# Plots
axarr[0].plot(i, thetha)
axarr[1].plot(i, phi)
axarr[2].plot(i, power)

# Axis settings
axarr[0].set_ylabel("Polar Angle [deg]")
axarr[1].set_ylabel("Azimuthal Angle [deg]")
axarr[2].set_ylabel("Laser Energy [%]")

for ax in axarr:
    ax.grid(True)

plt.suptitle('Run ' + str(RunNumber))
plt.xlabel("Event Number")

plt.show()