_author__ = 'matthias'

import numpy as np
import argparse
from devices.mirror import *
from devices.laser import *

def shoot():
        laser.openShutter()
        time.sleep(3)
        laser.closeShutter()

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

mirror_X = Mirror(name1, axis1)  # this is x movement
mirror_Y = Mirror(name2, axis2)  # this is y movement

laser = Laser()

# start com ports
mirror_X.com_init()
mirror_Y.com = mirror_X.com
laser.com_init()

laser.setRate(1)

mirror_X.getSerial()
mirror_Y.getSerial()

mirror221_midpoint = 4102
mirror222_midpoint = -19981
# Scanning array definitions.

mirror221_width = 10000
mirror222_width = 20000 # scan only in x direction

offset_x = mirror221_midpoint - mirror221_width/2
offset_y = mirror222_midpoint - mirror222_width/2

steps_x = 5
steps_y = 5
steps_r = 10

stepsize_x = mirror221_width / steps_x
stepsize_y = mirror222_width / steps_y

print "steps:", stepsize_x, stepsize_y

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
mirror_X.home()
mirror_Y.home()

time.sleep(5)
print mirror_X.getPosition(), mirror_Y.getPosition()

mirror_X.moveRelative(1000)
mirror_Y.moveRelative(1000)

mirror_x = mirror221_midpoint
mirror_y = mirror222_midpoint

for idr in range(1,steps_r):
    print "home x ", idr
    mirror_x += idr * stepsize_x
    mirror_y += idr * stepsize_y
    for idy in range(2 * idr):
	mirror_y -= stepsize_y
	mirror_X.moveAbsolute(mirror_x)
	mirror_Y.moveAbsolute(mirror_y)
	print "pos:",mirror_X.getPosition(), mirror_Y.getPosition()
        print idy, mirror_x, mirror_y
        print "shooting !"
        shoot()
    for idy in range(2 * idr):
        mirror_x -= stepsize_x
        mirror_X.moveAbsolute(mirror_x)
        mirror_Y.moveAbsolute(mirror_y)
        print "pos:",mirror_X.getPosition(), mirror_Y.getPosition()
        print idy, mirror_x, mirror_y
        print "shooting !"
        shoot()
    for idy in range(2 * idr):
        mirror_y += stepsize_y
        mirror_X.moveAbsolute(mirror_x)
        mirror_Y.moveAbsolute(mirror_y)
        print "pos:",mirror_X.getPosition(), mirror_Y.getPosition()
        print idy, mirror_x, mirror_y
        print "shooting !" 
        shoot()
    for idy in range(2 * idr):
        mirror_x += stepsize_x
        mirror_X.moveAbsolute(mirror_x)
        mirror_Y.moveAbsolute(mirror_y)
        print "pos:",mirror_X.getPosition(), mirror_Y.getPosition()
        print mirror_x, mirror_y
        print "shooting !"
        shoot()

print "finished"
mirror_X.com_close()
