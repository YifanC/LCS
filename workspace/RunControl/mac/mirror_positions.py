__author__ = 'matthias'

import numpy as np
import argparse
from devices.mirror import *

parser = argparse.ArgumentParser(description='scanning mirror positions script')

parser.add_argument('-n', action='store', dest='name_str', required=True,
                    help='specify mirror number', type=str)

parser.add_argument('-s', action='store', dest='start', nargs=2,
                    help='specify start indices', type=int)

arguments = parser.parse_args()


if arguments.start is None:
    start_idx = 0
    start_idy = 0
else:
    [start_idx, start_idy] = arguments.start

# mirror
name = "mirror" + str(arguments.name_str)
axis = int(arguments.name_str[2])
mirror = Mirror(name, axis)

mirror.com_init()
mirror.getSerial()


# Scanning array definitions.
stepsize_x = 100
stepsize_y = 50

offset_x = 0
offset_y = 0

steps_x = 4
steps_y = 4

x = offset_x + stepsize_x * np.linspace(0, steps_x - 1, steps_x)
y = offset_y + stepsize_y * np.linspace(0, steps_y - 1, steps_y)

# Positions
xx, yy = np.meshgrid(x, y)

# Relative Steps
xx_step = np.diff(xx,n=1,axis=1)
yy_step = np.diff(yy,n=0,axis=0)

print "Absolute positions"
print "xx"
print xx
print "yy"
print yy

print "home y"

for idy in range(start_idy, steps_y):
    print "home x "
    for idx in range(start_idx, steps_x):
        print "step " + str(idy*steps_x + idx + 1) + "/" + str(steps_x * steps_y)
        print " step x: ", str(idx),
        print "y: ", str(idy)
        print " position x: ", xx[idy, idx],
        print "y:", yy[idy, idx]
        raw_input("Press Enter to continue...")

print "finished"