__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()
att.comTimeout = 2
att.comport = "/dev/pts/10"
att.com_init()
#
#  att.getParameter("acceleration")
#att.getInfo(True)

att.setParameter("acceleration", 10000)
att.com_close()
