#!/usr/bin/env python

from FeedthroughMotorControls import *
from LaserControl import *
import os
import sys
import subprocess
import time
import signal

COMLaser = 3
COMMotorControl = 10
NIterations = int(sys.argv[1])

initFeedtrough(COMMotorControl)
initLaserCOM(COMLaser)
initAxis1()

startLaser()


#subprocess.Popen("cd ~/workspace/RotaryEncoder/",shell=True)
#proc = subprocess.Popen("./2encoder > log" + str(sys.argv[2]) +  ".txt",shell=True,stdin=subprocess.PIPE,stderr=subprocess.PIPE)
#time.sleep(2)
#proc.stdin.write('n\n')
#proc.stdin.flush()
#time.sleep(2)
#proc.stdin.write("n\n")
#proc.stdin.flush()


#Acceleration = ReadParameter(ROTARYAXIS, "A")
#Decceleration = ReadParameter(ROTARYAXIS, "D")
#MaxSpeed = ReadParameter(ROTARYAXIS, "VM")
#MinSpeed = ReadParameter(ROTARYAXIS, "VI")

# Do this for the reference run of the rotary encoder:
Move = 250000
moveHorizontal(Move)
time.sleep(CalcMovementTimeDefault(Move))

Move = -190000
moveHorizontal(Move)
time.sleep(CalcMovementTimeDefault(Move))
time.sleep(1)

deltaT = 1.5
Move = 300000

MaxSpeed = 15000
SetParameter(1,"VM",MaxSpeed)
SHOOT10Hz()
print "start shooting"
for i in range(2*NIterations):
	time.sleep(4)
	if i%2 == 0:
		HomeAxis(1)
		openShutter()
		moveHorizontal(Move)
		time.sleep(CalcMovementTimeGeneral(Move,10000,10000,MaxSpeed,1000)+deltaT)
		closeShutter()
		print "reached middle of" + str(i)
	elif i%2 == 1:
		openShutter()
		moveHorizontal(-(Move-50000))
		time.sleep(CalcMovementTimeGeneral((Move-50000),10000,10000,MaxSpeed,1000)+deltaT)
		closeShutter()
		print "reach end of" + str(i)
singleModeLaser()
closeShutter()
HomeAxis(1)
print "Test finished"
