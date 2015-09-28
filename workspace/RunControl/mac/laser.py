__author__ = 'matthias'

from devices.laser import *

laser = Laser(0)
laser.color = False
laser.comTimeout = 0.5
laser.com_init()


laser.getStatus()

laser.start()
