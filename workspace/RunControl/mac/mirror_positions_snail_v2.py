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

parser.add_argument('-s', action='store', dest='start',
                    help='specify startpoint', type=int)

parser.add_argument('-D', action='store_true', default=False,
                    dest='debug',
                    help='print out debug messages')

arguments = parser.parse_args()

# Arguments
DEBUG = arguments.debug
print DEBUG
if arguments.start is None:
    startpoint = 0
else:
    startpoint = arguments.start

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

mirror221_midpoint = -2323
mirror222_midpoint = 5777
# Scanning array definitions.

mirror221_width = 120000
mirror222_width = 240000

stepsize_x = 2000/2 # 1 step = 2 urad
stepsize_y = 4000/2 # 1 step = 1 urad

# the scan window
steps_r = 7

offset_x = 0
offset_y = 0

print "steps:", stepsize_x, stepsize_y

x = np.linspace(0, mirror221_width, mirror221_width/stepsize_x+1)-mirror221_width/2
y = np.linspace(0, mirror222_width, mirror222_width/stepsize_y+1)-mirror222_width/2

print x

print "homing..."
mirror_X.home()
mirror_Y.home()

#time.sleep(5)
print mirror_X.getPosition(), mirror_Y.getPosition()

runplan=[]

#strat at the center of the grid, we shift by offset during run
idx = int(len(x)/2)+offset_x
idy = int(len(y)/2)+offset_y

mirror_X.moveAbsolute(mirror221_midpoint)
mirror_Y.moveAbsolute(mirror222_midpoint)

raw_input("hit enter to continue")

for idr in range(1,steps_r):
    xx=idr+idx
    yy=idr+idy
    for i in range(0,2*idr):
        yy-=1
        runplan.append([xx,yy])
    for i in range(0,2*idr):
        xx-=1
        runplan.append([xx,yy])
    for i in range(0,2*idr):
        yy+=1
        runplan.append([xx,yy])
    for i in range(0,2*idr):
        xx+=1
        runplan.append([xx,yy])


print runplan

for i in range(startpoint,len(runplan)):
    mtptgtx = x[runplan[i][0]]+mirror221_midpoint
    mtptgty = y[runplan[i][1]]+mirror222_midpoint
    print "point ",i+1," of ",len(runplan)," is ",mtptgtx,mtptgty
    mirror_X.moveAbsolute(mtptgtx)
    mirror_Y.moveAbsolute(mtptgty)
    pos_x = mirror_X.getPosition()
    pos_y = mirror_Y.getPosition()
    print "Shooting at position", pos_x, pos_y
    shoot()
    
print "finished"
mirror_X.com_close()
