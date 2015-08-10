__author__ = 'matthias'

import serial
from devices.feedtrough import *

LinearMotor = Feedtrough("linear_actuator")
RotaryMotor = Feedtrough("rotary_actuator")

LinearMotor.color = False
#LinearMotor.comport = "/dev/ttyUSB0"
LinearMotor.com_init()
RotaryMotor.com = LinearMotor.com
LinearMotor.color = False
RotaryMotor.color = False

RotaryMotor.initAxis()
LinearMotor.initAxis()

RotaryMotor.getInfo(True)
LinearMotor.getInfo(True)




#LinearMotor.homeAxis()

