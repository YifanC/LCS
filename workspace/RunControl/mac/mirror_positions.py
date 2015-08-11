__author__ = 'matthias'

import numpy as np
import argparse
from devices.mirror import *

""" Little script which steps trough T-OMG mirror positions in x and y spanned by array defined trough:
    stepsize_x:     stepsize for mirror_x
    stepsize_y:     stepsize for mirror_y
    steps_x:
    steps_y:
    offset_x:
    offset_y:

    Procedure:
        1. Home both axes
        2. Go to array position (0,0), corresponding to positions (offset_x, offset_y)
        3. loop over x positions until end of row is reached
        4. home x axis
        5. move to second row positions
        6. goto 3.
    """

parser = argparse.ArgumentParser(description='scanning mirror positions script')

parser.add_argument('-n1', action='store', dest='name1_str', required=True,
                    help='specify mirror number 1 (e.g. 221)', type=str)

parser.add_argument('-n2', action='store', dest='name2_str', required=True,
                    help='specify mirror number 2 (e.g. 222)(', type=str)

parser.add_argument('-s', action='store', dest='start', nargs=2,
                    help='specify start indices', type=int)

parser.add_argument('-D', action='store_true', default=False,
                    dest='debug',
                    help='print out debug messages')

arguments = parser.parse_args()

# Arguments
DEBUG = arguments.debug
print DEBUG
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
mirrorX.com_init()
mirrorY.com = mirrorX.com

mirrorX.getSerial()
mirrorY.getSerial()

mirror221_midpoint = 4102
mirror222_midpoint = -19981
# Scanning array definitions.

mirror221_width = 1000
mirror222_width = 0.001 # scan only in x direction

offset_x = mirror221_midpoint - mirror221_width/2
offset_y = mirror222_midpoint - mirror222_width/2

steps_x = 10
steps_y = 1

stepsize_x = mirror221_width / steps_x
stepsize_y = mirror222_width / steps_y

x = offset_x + stepsize_x * np.linspace(0, steps_x - 1, steps_x)
y = offset_y + stepsize_y * np.linspace(0, steps_y - 1, steps_y)

# Positions
xx, yy = np.meshgrid(x, y)

# Relative Steps
xx_step = np.diff(xx, n=1, axis=1)
yy_step = np.diff(yy, n=0, axis=0)

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
        print "step " + str(idy * steps_x + idx + 1) + "/" + str(steps_x * steps_y)
        print " step x: ", str(idx), " y: ", str(idy)
        print " set position     x: ", xx[idy, idx], " y:", yy[idy, idx]
        mirrorX.moveAbsolute(xx[idy, idx],monitor=True)
        pos_x = mirrorX.getPosition()
        pos_y = mirrorY.getPosition()
        print " mirror positions x: ", pos_x, " y:", pos_y
        raw_input("Press Enter to continue...")

print "finished"

mirrorX.com_close()
