__author__ = 'matthias'

import serial
from devices.feedtrough import *


LinearMotor = Feedtrough("linear_actuator", 1)
RotaryMotor = Feedtrough("rotary_actuator", 2)
LinearMotor.color = False
LinearMotor.comport = "/dev/ttyUSB0"
LinearMotor.com_init()
RotaryMotor.com = LinearMotor.com
LinearMotor.color = False


RotaryMotor.getInfo(True)
LinearMotor.getInfo(True)




#LinearMotor.homeAxis()

