# power calibration plots for LCS2
# data taken from run 3165 (full scan, starting with low polar angle and swiping azimuthal angle)

import numpy as np
import matplotlib.pyplot as plt


from numpy.polynomial import polynomial as pol

# convert  steps to degrees, offset were defined based on
def steps2deg(steps):

    offset = 174000
    offset_deg = 28.8218
    steps_deg = 0.000178683

    steps -= offset
    deg = steps * steps_deg
    deg += offset_deg

    return deg


# polynomial fit
power_fit = np.array([0.25, 0.45, 0.55, 0.66, 0.88,0.95])
angle_fit = [150,120,100,80,60,30]
c = pol.polyfit(angle_fit, power_fit, 3)
print c
angles = np.linspace(0, 160, 100)

# plot fit parameters
plt.plot(angle_fit, power_fit, "ro")
plt.plot(angles, pol.polyval(angles, c), "r-") # Plot of the data and the fit
plt.show()


filename = "/home/matthias/workspace/LCS/workspace/RunControl/scans/config_LCS2_scan.csv"

polar_steps = np.cumsum( np.genfromtxt(filename, comments="#", delimiter=", ", dtype=int, usecols=[2]) )

#
deg = steps2deg(polar_steps)
power = pol.polyval(deg,c)
plt.plot(steps2deg(polar_steps))
plt.plot(power, "r-") # Plot of the data and the fit
plt.show()

