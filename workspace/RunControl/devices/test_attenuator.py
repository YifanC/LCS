__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()
att.comTimeout = 2

att.comport = "/dev/ttyUSB9"
#att.com_init()
att.color = False


att.config.microstep_resolution = 3.0
att.init()
att.setZero(100.)

#
# att.getParameter("acceleration")
#att.getInfo(True)



