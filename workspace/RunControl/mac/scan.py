__author__ = 'matthias'

""" Script performing a horizontal scan of the TPC with the cold mirror at a constant speed while shooting the laser.
    After an initialization stage the scan is performed over and over again under user control until stopped. """

from devices.feedtrough import *
from devices.laser import *

# Definitions
START_HORIZONTAL = 10000
START_VERTICAL = 10000
END_HORIZONTAL = 1000

SCAN_SPEED = 10000
LASER_REPETITION = 1  # in Hz

# Initialize
LinearMotor = Feedtrough("linear_actuator")
RotaryMotor = Feedtrough("rotary_actuator")
Laser = Laser()

LinearMotor.comDryRun = True
Laser.comDryRun = True
LinearMotor.com_init()
#RotaryMotor.com = LinearMotor.com
Laser.com_init()

# Reference Run for encoder done?

# Home axis
print "homing axes"
#LinearMotor.homeAxis()
#eRotaryMotor.homeAxis()

# goto first position
print "going to first positions "
LinearMotor.moveRelative(START_VERTICAL)
#RotaryMotor.moveRelative(START_HORIZONTAL)
Laser.setParameter("setRate", 10, check=False)
stop = False
while stop is False:
    time.sleep(1)
    stop = True
    # scan again?

# finalize