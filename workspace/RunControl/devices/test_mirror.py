__author__ = 'matthias'

from devices.mirror import *


m1_axis1 = Mirror("mirror211")
m1_axis1.com_init()
m1_axis1.com.timeout = 5
m1_axis1.getPosition()
m1_axis1.moveRelative(1000)
m1_axis1.getPosition()
m1_axis1.getSerial()
m1_axis1.home()
m1_axis1.getStatus()
time.sleep(5)

m1_axis1.getPosition()

m1_axis1.com_close()
