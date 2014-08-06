#!/usr/bin/env python

from FeedthroughMotorControls import *

import os
import sys
import subprocess
import time
import signal


deltaT = 5
Move = 90000
print CalcMovementTimeDefault(Move)

def CalcMovementTimeGeneral(Microsteps,MaxSpeed,MinSpeed,Acceleration,Deceleration):
  # calculates the movement time according with a trapezoidal model with the given
  # parameters. 

  FMicrostepts = float(Microsteps)
  FMaxSpeed = float(MaxSpeed)
  FMinSpeed = float(MinSpeed)
  FAcceleration = float(Acceleration)
  FDecceleration = float(Deceleration)


  TAcceleration = (FMaxSpeed-FMinSpeed)/FAcceleration
  print TAcceleration
  StepsAcceleration = (FMaxSpeed+FMinSpeed) * TAcceleration * 0.5
  TDeceleration = (FMaxSpeed-FMinSpeed)/FDecceleration
  
  StepsDeceleration = (FMaxSpeed+FMinSpeed) * TDeceleration * 0.5
  TConstantSpeed = (abs(Microsteps) - (StepsAcceleration + StepsDeceleration))/FMaxSpeed
  TTotal = TAcceleration + TDeceleration + TConstantSpeed

  return TTotal