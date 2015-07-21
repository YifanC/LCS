__author__ = 'matthias'

from devices.mirror import *


m1_axis1 = Mirror()
m1_axis1.com_init()

m1_axis1.communicate(35)

m1_axis1.com_close()