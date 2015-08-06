__author__ = 'matthias'

import serial
from devices.feedtrough import *


LinearMotor = Feedtrough("linear_actuator", 2)
RotaryMotor = Feedtrough("rotary_actuator", 1)
LinearMotor.color = False
LinearMotor.comport = "/dev/ttyUSB0"
LinearMotor.com_init()
RotaryMotor.com = LinearMotor.com
LinearMotor.color = False
RotaryMotor.color = False

RotaryMotor.initAxis()
LinearMotor.initAxis()

RotaryMotor.getInfo(True)
LinearMotor.getInfo(True)




#LinearMotor.homeAxis()

