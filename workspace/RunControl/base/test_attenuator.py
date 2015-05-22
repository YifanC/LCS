__author__ = 'matthias'

from attenuator import *

att = Attenuator()

att.comport = "/dev/ttyUSB9"
att.com_init()
att.getInfo(True)
att.com_close()
