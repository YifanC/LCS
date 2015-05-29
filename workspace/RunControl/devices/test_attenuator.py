__author__ = 'matthias'

from devices.attenuator import *

att = Attenuator()

att.comport = "/dev/pts/10"
att.com_init()
#att.getInfo(True)

att.setParameter("acceleration", 10000)
att.com_close()
