__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()
att.comTimeout = 2
att.comport = "/dev/ttyUSB9"
att.com_init()
#
#  att.getParameter("acceleration")
att.getInfo(True)

while att.isMoving():
    print att.getPosition()

print att.getPosition()


