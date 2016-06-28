__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()
att.color = False       # just for the use with bpython
att.comport = "/dev/ttyUSB9"

# startup
att.com_init()
#att.getName()
att.enableMotor()
#att.home(monitor=True, display=True)

# go somewhere
#att.moveAbsolute(754, monitor=True, display=True)

# this is the new zero transmission position
#att.setZero()

# go to some transmission position
#att.setTransmission(0.95, monitor=True, display=True)

# go relative somewhere
#att.moveRelative(-1300, monitor=True, display=True)

# finalize
att.disableMotor()




