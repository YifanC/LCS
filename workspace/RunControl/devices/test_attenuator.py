__author__ = 'matthias'

from attenuator import *

att = Attenuator()
att.comTimeout = 2
att.comport = "/dev/ttyUSB9"
att.com_init()
att.color = False
#
#  att.getParameter("acceleration")
att.getInfo(True)

while att.isMoving():
    print att.getPosition()

print att.getPosition()


