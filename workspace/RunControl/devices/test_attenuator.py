__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()
att.color = False

att.comport = "/dev/ttyUSB9"
att.com_init()
att.enableMotor()
att.home(monitor=True, display=True)
att.setTransmission(0.95, monitor=True, display=True)
att.moveRelative(-1300, monitor=True, display=True)
att.zero(monitor=True,display=True)
att.disableMotor()
att.com_close()
#att.com_close()
#
# att.getParameter("acceleration")
#att.getInfo(True)



