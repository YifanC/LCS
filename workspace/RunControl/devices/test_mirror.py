__author__ = 'matthias'

from devices.mirror import *


m1_axis1 = Mirror("mirror211")
m1_axis1.com_init()

m1_axis1.writeRegister(77, 34)
m1_axis1.getSerial()
m1_axis1.readRegister(77)
m1_axis1.home()
time.sleep(5)

m1_axis1.getPosition()

m1_axis1.com_close()
