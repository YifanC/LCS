__author__ = 'matthias'

""" Script performing a horizontal scan of the TPC with the cold mirror at a constant speed while shooting the laser.
    After an initialization stage the scan is performed over and over again under user control until stopped. """

from devices.feedtrough import *
from devices.laser import *

DEBUG = False

# Definitions
START_HORIZONTAL = 10000
START_VERTICAL = 10000
WIDTH_HORIZONTAL = 50000

SCAN_SPEED = 10000
LASER_REPETITION = 1  # in Hz

# Initialize
LinearMotor = Feedtrough("linear_actuator")
RotaryMotor = Feedtrough("rotary_actuator")
Laser = Laser()

LinearMotor.comDryRun = False
Laser.comDryRun = True
LinearMotor.com_init()
LinearMotor.color = False
Laser.color = False
RotaryMotor.com = LinearMotor.com
Laser.com_init()
LinearMotor.initAxis()
RotaryMotor.initAxis()
# Reference Run for encoder done?

# Home axis
print "homing axes"
# LinearMotor.homeAxis()
#eRotaryMotor.homeAxis()

# goto first position
print "going to first positions "

#RotaryMotor.moveRelative(START_HORIZONTAL)

# setting up the laser to fire at 1Hz (assuming the laser is started somewhere else)
Laser.setRate(1)
LinearMotor.setParameter("VM", LinearMotor.config.ENDVELOCITY)
stop = False

while stop is False:
    # goto start position and set scan speed
    LinearMotor.setParameter("VM", SCAN_SPEED)
    LinearMotor.moveRelative(START_VERTICAL, monitor=True)

    # actual scanning with laser
    Laser.openShutter()
    LinearMotor.moveRelative(WIDTH_HORIZONTAL, monitor=True)
    Laser.closeShutter()

    # go back to default speed
    LinearMotor.setParameter("VM", LinearMotor.config.ENDVELOCITY)

    # logics for scanning again or aborting    
    again = raw_input("scan again [(y)es] / (n)o?")
    if again == "y" or again == "":
        print "scanning again"
    elif again == "n":
        print "stopping"
        stop = True

    # anyway go to home position
    LinearMotor.homeAxis()



# finalize
