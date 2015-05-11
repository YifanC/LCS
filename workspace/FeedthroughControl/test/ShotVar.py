#!/usr/bin/env python

from FeedthroughMotorControls import *
from LaserControl import *
import os
import sys
import subprocess
import time
import signal

COMLaser = 1
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

N=6
deltaT = 2
Move = 400000


SHOOT10Hz()

openShutter()
time.sleep(2)
closeShutter()

print "start shooting"
for i in range(2*NIterations):
	time.sleep(4)
	
	# going right
	if i%2 == 0:
		TMove = CalcMovementTime(Move)
		HomeAxis(1)
		openShutter()
		moveHorizontal(Move)
		
		for k in range(N):
			if k%2 == 0: 
				if (k != 0): openShutter()
				time.sleep(TMove/N)
			if k%2 == 1:
				closeShutter()
				time.sleep(TMove/N)
		time.sleep(deltaT)
		closeShutter()
		print "reached middle of" + str(i)

	# going left
	elif i%2 == 1:
		TMove = CalcMovementTime(Move)
		moveHorizontal(-Move)
		for k in range(N):
			if k%2 == 0: 
				openShutter()
				time.sleep(TMove/N)
			if k%2 == 1:
				closeShutter()
				time.sleep(TMove/N)
		time.sleep(deltaT)
		closeShutter()
		print "reached middle of" + str(i)

singleModeLaser()
closeShutter()
HomeAxis(1)
print "Test finished"
