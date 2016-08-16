#!/usr/bin/python
__author__ = 'matthias'

import numpy as np
import pandas
import matplotlib.pyplot as plt

import os
import argparse


parser = argparse.ArgumentParser(description='Little script to plot movement pattern of laser runs.')
parser.add_argument('-f', type=str, dest='filepath', required=True, help='path to data file (usaly Run-RUNNUMBER.txt)')
parser.add_argument('-e', nargs='+', type=int, dest='entries', required=False, help='entries for which to print the details')
args = parser.parse_args()

os.path.isfile(args.filepath)

data = pandas.read_csv(args.filepath, delim_whitespace=True)

id = int(data.iloc[1,0] -1)

# conversion factors for linear encoder: ticks to mm and mm to angle
lin_convert = 0.3499  # conversion from mm to deg
err_convert = 0.0002  # error on coversion from mm to deg
lin_tick = 0.00001  # conversion from tick to mm

# these offsets are defined in the laser coordinate system: origin in middle of mirror, x- axis parallel to wire plane,
# y-axis along drift and z-axis parallel to field cage tubes

lin_offset = [-273.28308088 + 60, - 266.60208631 + 60 + 1.161]  # offsets for both systems
rot_offset = [ -99.7919998169 + 3.1432, 0 + 18 - 180]
power_calib = [[4000,12500], [8000, 12500]] # first: power minimum, second: power maximum


# reading data
i = data.iloc[:,8]

thetha = data.iloc[:, 3] * lin_tick / lin_convert + lin_offset[id]    # polar angle
phi = data.iloc[:, 2]  + rot_offset[id]                                            # azimuthal angle

power = (data.iloc[:, 4] - power_calib[id][0]) / (power_calib[id][1] - power_calib[id][0]) * 100
time = data.iloc[:, 6] + data.iloc[:, 7] / 1000000 - data.iloc[0, 6]


if args.entries:
    for entry in args.entries:
        print "entry ", entry
        print " thetha ", thetha[entry]
        print " phi    ", phi[entry]
        print " power  ", power[entry]

# ------------ Plotting ------------
f, axarr = plt.subplots(3, sharex=True)

# Plots
axarr[0].plot(i, thetha, "*")
axarr[1].plot(i, phi, "*")
axarr[2].plot(i, power, "*")

# Axis settings
axarr[0].set_ylabel("Polar Angle [deg]")
axarr[1].set_ylabel("Azimuthal Angle [deg]")
axarr[2].set_ylabel("Laser Energy [%]")

for ax in axarr:
    ax.grid(True)

#plt.suptitle('Run ' + str(RunNumber))
plt.xlabel("Event Number")

plt.show()