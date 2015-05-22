__author__ = 'matthias'

from attenuator import *

att = Attenuator()

att.comport = "/dev/ttyUSB9"
att.com_init()
att.getInfo()
att.com_close()
