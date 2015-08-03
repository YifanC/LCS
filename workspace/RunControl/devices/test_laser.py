__author__ = 'matthias'

from devices.laser import *

laser = Laser()
laser.comTimeout = 0.5
laser.comport = "/dev/ttyUSB3"

laser.com_init()
#
# att.getParameter("acceleration")
laser.getStatus()

laser.getShots()
laser.com_close()
