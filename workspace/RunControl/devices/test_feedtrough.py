__author__ = 'matthias'

import serial
from devices.feedtrough import *


LinearMotor = Feedtrough("linear actuator", 1)
RotaryMotor = Feedtrough("rotary actuator", 1)

LinearMotor.comport = "/dev/pts/26"
LinearMotor.com_init()
print LinearMotor.name

RotaryMotor.com = LinearMotor.com



RotaryMotor.com_write("hello")
repl = RotaryMotor.com_recv(100)
print repl

LinearMotor.com_write("bla")
print LinearMotor.isMoving()

LinearMotor.homeAxis()

LinearMotor.com_close()
