__author__ = 'matthias'

from devices.feedtrough import *

LinearMotor = Feedtrough("linear actuator", 1)
RotaryMotor = Feedtrough("rotary actuator", 2)

LinearMotor.comport = "/dev/pts/26"
LinearMotor.com_init()

RotaryMotor.com = LinearMotor.com



RotaryMotor.com_write("hello")
LinearMotor.com_write("bla")

RotaryMotor.setParameter("MV", 1)

LinearMotor.com_close()