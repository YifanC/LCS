__author__ = 'matthias'

""" Script performing a horizontal scan of the TPC with the cold mirror at a constant speed while shooting the laser.
    After an initialization stage the scan is performed over and over again under user control until stopped. """

from devices.feedtrough import *
from devices.laser import *

# Definitions
START_HORIZONTAL = 0
START_VERTICAL = 0
END_HORIZONTAL = 1000

SCAN_SPEED = 10
LASER_REPETITION = 1  # in Hz

# Initialize
LinearMotor = Feedtrough("linear_actuator", 1)
RotaryMotor = Feedtrough("rotary_actuator", 2)
Laser = Laser()

#LinearMotor.com_init()
#RotaryMotor.com = LinearMotor.com
#Laser.com_init()

# Reference Run for encoder done?

# Home axis
print "homing axes"
LinearMotor.homeAxis()
RotaryMotor.homeAxis()

# goto first position
print "going to first positions "
LinearMotor.moveRelative(START_VERTICAL)
RotaryMotor.moveAbsolute(START_HORIZONTAL)

stop = False
while stop is False:
    pass

    # scan again?

# finalize