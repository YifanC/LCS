__author__ = 'matthias'

from devices.laser import *

laser = Laser()
laser.color = False
laser.comTimeout = 0.5
laser.comport = "/dev/ttyUSB4"
laser.com_init()


laser.getStatus()

laser.start()
