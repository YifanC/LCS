__author__ = 'matthias'

# Should become a script that turns on the laser as soon as it detects that the keys on the laser were turned

from devices.laser import *

laser = Laser(0)
laser.color = False
laser.comTimeout = 0.5
laser.com_init()


laser.getStatus()

laser.start()
