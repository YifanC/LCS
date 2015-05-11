#!/usr/bin/env python

from FeedthroughMotorControls import *
import os
import sys
import subprocess
import time
import signal

COMMotorControl = 10

NIterations = int(sys.argv[1])

initFeedtrough(COMMotorControl)
initAxis1()


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
Move = 300000
moveHorizontal(Move)
time.sleep(CalcMovementTimeDefault(Move)+3)

Move = -150000
moveHorizontal(Move)
time.sleep(CalcMovementTimeDefault(Move)+5)



HomeAxis(1)
time.sleep(5)
Move = 200000
moveHorizontal(Move)
time.sleep(CalcMovementTimeDefault(Move))

Move = -100000
moveHorizontal(Move)
time.sleep(CalcMovementTimeDefault(Move))

deltaT = 5
Move = 100000

for i in range(NIterations):
	if i%2 == 0:
		moveHorizontal(Move)
		time.sleep(CalcMovementTimeDefault(Move)+deltaT) 
		moveHorizontal(Move)
		time.sleep(CalcMovementTimeDefault(Move)+deltaT)
		moveHorizontal(Move)
                time.sleep(CalcMovementTimeDefault(Move)+deltaT)
		moveHorizontal(Move)
                time.sleep(CalcMovementTimeDefault(Move)+deltaT)
	elif i%2 == 1:
		moveHorizontal(-Move)
		time.sleep(CalcMovementTimeDefault(Move)+deltaT)
		moveHorizontal(-Move)
		time.sleep(CalcMovementTimeDefault(Move)+deltaT)
		moveHorizontal(-Move)
                time.sleep(CalcMovementTimeDefault(Move)+deltaT)
		moveHorizontal(-Move)
                time.sleep(CalcMovementTimeDefault(Move)+deltaT)
HomeAxis(1)
#proc.send_signal(signal.SIGINT)
#time.sleep(1)
#proc.terminate()

print "Test finished"
