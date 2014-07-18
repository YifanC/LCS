#!/usr/bin/env python

from FeedthroughMotorControls import *
import os
import sys

initFeedtrough()
initAxis1()

#NIterations = int(sys.argv[1])

HomeAxis(1)
Acceleration = ReadParameter(ROTARYAXIS, "A")
Decceleration = ReadParameter(ROTARYAXIS, "D")
MaxSpeed = ReadParameter(ROTARYAXIS, "VM")
MinSpeed = ReadParameter(ROTARYAXIS, "VI")

Move = 200000

TMove = CalcMovementTime(Move,Acceleration,Decceleration,MaxSpeed,MinSpeed)
print TMove