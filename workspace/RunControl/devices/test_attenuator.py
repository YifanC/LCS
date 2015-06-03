__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()
att.color = False
att.comTimeout = 2

att.comport = "/dev/ttyUSB9"
att.com_init()



att.config.microstep_resolution = 3.0

att.setZero(444.)
att.setZero(423.)

#att.com_close()
#
# att.getParameter("acceleration")
#att.getInfo(True)



