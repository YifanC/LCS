__author__ = 'matthias'

import serial
from devices.feedtrough import *


LinearMotor = Feedtrough("linear_actuator", 1)
RotaryMotor = Feedtrough("rotary_actuator", 2)

LinearMotor.comport = "/dev/pts/14"
LinearMotor.com_init()
print LinearMotor.name

LinearMotor.getInfo(True)

RotaryMotor.com = LinearMotor.com

RotaryMotor.com_write("hello")
repl = RotaryMotor.com_recv(100)
print repl

LinearMotor.com_write("bla")
print LinearMotor.isMoving()

LinearMotor.homeAxis()

LinearMotor.com_close()
