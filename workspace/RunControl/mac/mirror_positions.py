__author__ = 'matthias'

import numpy as np
import argparse
from devices.mirror import *

DEBUG = False

parser = argparse.ArgumentParser(description='scanning mirror positions script')

parser.add_argument('-n1', action='store', dest='name1_str', required=True,
                    help='specify mirror number', type=str)

parser.add_argument('-n2', action='store', dest='name2_str', required=True,
                    help='specify mirror number', type=str)

parser.add_argument('-s', action='store', dest='start', nargs=2,
                    help='specify start indices', type=int)

arguments = parser.parse_args()

if arguments.start is None:
    start_idx = 0
    start_idy = 0
else:
    [start_idx, start_idy] = arguments.start

# mirrors
name1 = "mirror" + arguments.name1_str
axis1 = int(arguments.name1_str[2])
name2 = "mirror" + arguments.name2_str
axis2 = int(arguments.name2_str[2])


mirrorX = Mirror(name1, axis1)  # this is x movement
mirrorY = Mirror(name2, axis2)  # this is y movement

# start com ports
#mirrorX.com_init()
mirrorY.com = mirrorX.com

mirrorX.getSerial()
mirrorY.getSerial()

# Scanning array definitions.
stepsize_x = 100
stepsize_y = 50

offset_x = -50000
offset_y = -50000

steps_x = 4
steps_y = 4

x = offset_x + stepsize_x * np.linspace(0, steps_x - 1, steps_x, dtype=int)
y = offset_y + stepsize_y * np.linspace(0, steps_y - 1, steps_y, dtype=int)

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
mirrorY.home()

for idy in range(start_idy, steps_y):
    print "home x "
    mirrorX.home()
    mirrorY.moveAbsolute(yy[idy, 0])
    for idx in range(start_idx, steps_x):
        print "step " + str(idy*steps_x + idx + 1) + "/" + str(steps_x * steps_y)
        print " step x: ", str(idx), " y: ", str(idy)
        print " set position x: ", xx[idy, idx], " y:", yy[idy, idx]
        mirrorX.moveAbsolute(xx[idy, idx])
        pos_x = mirrorX.getPosition()
        pos_y = mirrorY.getPosition()
        print "mirror positions x: ", pos_x, " y:",pos_y
        raw_input("Press Enter to continue...")

print "finished"

mirrorX.com_close()
