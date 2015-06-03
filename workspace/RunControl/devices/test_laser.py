__author__ = 'matthias'

from devices.laser import *

laser = Laser()
laser.comTimeout = 2
laser.comport = "/dev/pts/10"
laser.com_init()
#
# att.getParameter("acceleration")
#att.getInfo(True)

laser.getShots()
laser.com_close()
