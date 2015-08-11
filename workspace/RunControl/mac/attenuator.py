__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()
att.color = False       # just for the use with bpython

# startup
att.com_init()
#att.getName()
att.enableMotor()



