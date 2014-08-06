#!/usr/bin/env python

import sys, time, glob
import serial

sgn = lambda x : (x>0) - (x<0)

ROTARYAXIS = 1
LINEARAXIS = 2


MICROSTEPS = 256                # one full evolution of the motor has:
                                #   - steps: 200 
                                #   - microsteps: 200*MS (200*256=51200)        
ONETURN = 200*MICROSTEPS        # steps of on full turn


ROTARY_MAXVELOCITY = 20000
ROTARY_MINVELOCITY = 1000 
ROTARY_ACCELERATION = 40000
ROTARY_DECELERATION = 40000


TURNSPERDEGHOR = ONETURN/(3.6)
TURNSPERDEGVER = ONETURN/14.501537
k=0

########################### Feed-trough control ################################
def initAxis1():
        print "----------- Axis 1 (Rotary) Initialization -----------"
        # first signal on the connection        
        serFeedtrough.write("\n")       

        # set microstep resolution to maximum 
        serFeedtrough.write("\n1MS "+str(MICROSTEPS)+"\n")
        
        # set acceleration and deceleration to 100000 microsteps/s
        serFeedtrough.write("\n1A=" + str(ROTARY_ACCELERATION) + "\n")
        serFeedtrough.write("\n1D=" + str(ROTARY_DECELERATION) + "\n")
        
        # set start speed and end speed of the movement
        serFeedtrough.write("\n1VI=" + str(ROTARY_MINVELOCITY) + "\n")
        serFeedtrough.write("\n1VM=" + str(ROTARY_MAXVELOCITY) + "\n")
        # max velocity is stable up to 80000 M/s


        # set holding and run current in percent
        serFeedtrough.write("\n1HC=5\n")
        serFeedtrough.write("\n1RC=80\n")
	
	# one can only define one home switch, so make shure no other
	# switch is defined as home switch
	SetLimitSwitch(ROTARYAXIS,0)
	serFeedtrough.write("\n1S3=0,0,0\n")
        ##tstart = (maxVelocity-initialVelocity)/acceleration

def initAxis2():
        print '----------- Axis 2 (Linear) Initialization -----------'  
        # first signal on the connection        
        serFeedtrough.write("\n")       

        # set motor settling time in ms 
        #serFeedtrough.write("\n2MT=2000\n")

        # set microstep resolution to maximum 
        serFeedtrough.write("\n2MS"+str(MICROSTEPS)+"\n")
        
        # set acceleration and deceleration to 100000 microsteps/s
        serFeedtrough.write("\n2A=50000\n")
        serFeedtrough.write("\n2D=50000\n")
        
        # set start speed and end speed of the movement
        serFeedtrough.write("\n2VI=1000")
        serFeedtrough.write("\n2VM=40000\n")

        # set holding and run current in percent
        serFeedtrough.write("\n2HC=10\n")
        serFeedtrough.write("\n2RC=80\n")

        # one can only define one home switch, so make shure no other
        # switch is defined as home switch
        serFeedtrough.write("\n1S3=0,0,0\n")
        ##maxVelocity = float(readValue(reply,"VM"))
        
        ##initialVelocity = float(readValue(reply,"VI"))
        ##acceleration =  float(readValue(reply,"A"))
        ##tstart = (maxVelocity-initialVelocity)/acceleration

def getInfo(AxisNr):
        # obtain all the control parameters from the motor
        serFeedtrough.write("\n\n"+ str(AxisNr) + "PR AL\n")
        reply = serFeedtrough.read(850)
        print reply
        return reply

def moveVertical(inst):
        if sgn(inst) == 1: print 'Axis2: going up ' + str(inst) + ' microsteps'
        else: print 'Axis2: going down ' + str(-inst) + ' microsteps'
        
        serFeedtrough.write('\n2MR ' + str(inst) + '\n')
        
        ##while AxisMoving(2):
        ##        print 'Axis2 Position: ' + getPosition(2)

def moveHorizontal(inst):
        if sgn(inst) == 1: print 'Axis1: going left ' + str(inst) + ' microsteps'
        else: print 'Axis1: going right ' + str(-inst) + ' microsteps'
        
        serFeedtrough.write('\n1MR ' + str(inst) + '\n')
        
        ##while AxisMoving(1):
        ##        print 'Axis1 Position: ' + getPosition(1)

def moveAbsoluteHorizintal(pos):
        print "Going to " + str(pos)
        
        serFeedtrough.write('\n1MA ' + str(pos) + '\n')


def getPosition(AxisNr):
        serFeedtrough.write("\n"+ str(AxisNr) + "PR P\n")
        reply = serFeedtrough.read(25)
        return reply

def AxisMoving(AxisNr):
        # returns: 1 if axis is moving, 0 if not. And -1 for non-identifiable answer
        serFeedtrough.write("\n"+ str(AxisNr) + "PR MV\n")
        reply = serFeedtrough.readline()        
        if ('0' in reply): return 0
        elif('1' in reply): return 1    
        else: return -1 


def SetHomeSwitch(AxisNr,Attempts):
  time.sleep(0.5)
  # exit criterion for recursive 
  if Attempts == 4:
    print "Setting the HomeSwitch failed 5 times -> exiting"
    sys.exit()


  if AxisNr == ROTARYAXIS:
    print "Axis 1: Set S2 as home switch"
    SetParameter(ROTARYAXIS,"S2","1,1,0\n")    # set counterclockwise endswitch as home switch
    time.sleep(0.5)
    # check if S2 was set correctly
    S2SetValue = ReadParameter(ROTARYAXIS,"S2")
    print "S2 = " + S2SetValue[0:7] 
    if S2SetValue[0:7] != "1, 1, 0":
      print "Axis" + str(AxisNr) + " homeswitch S2 was not set correctly"
      SetHomeSwitch(ROTARYAXIS,Attempts+1)


def SetLimitSwitch(AxisNr,Attempts):
  time.sleep(0.5)
  if Attempts == 4:
    print "Setting the limit switch failed 5 times -> exiting"
    sys.exit()

  if AxisNr == ROTARYAXIS:
    print "Axis 1: Set S2 as limit switch"
    SetParameter(ROTARYAXIS,"S2","3,1,0\n")    # set counterclockwise endswitch as home switch
    time.sleep(0.5)
    # check if S2 was set correctly
    S2SetValue = ReadParameter(ROTARYAXIS,"S2")
    print 'S2 = ' + S2SetValue[0:7]
    if S2SetValue[0:7] != "3, 1, 0":
      print "Axis" + str(AxisNr) + " limit switch S2 was not set correctly"
      SetLimitSwitch(ROTARYAXIS,Attempts+1)


def HomeAxis(AxisNr):
	if AxisNr == ROTARYAXIS:
		InitialMaxVelocity = int(ReadParameter(ROTARYAXIS, "VM"))
		SetParameter(ROTARYAXIS,"VM",15000)       # reduce spped so we move slowly into endswitch
		SetHomeSwitch(ROTARYAXIS,0)
	elif AxisNr == LINEARAXIS:
		print "will not home linear axis: no procedure defined yet!"
		return -1
		# do whatever you need 

  	# do homing procedure
  	serFeedtrough.write("\n"+ str(AxisNr) + "HM 1\n")
  	# 1 - Slew at VM in the minus direction and Creep at VI in the plus direction.
  	# 2 - Slew at VM in the minus direction and Creep at VI in the minus direction.
  	# 3 - Slew at VM in the plus direction and Creep at VI in the minus direction.
  	# 4 - Slew at VM in the plus direction and Creep at VI in the plus direction.
  
  	while AxisMoving(AxisNr):
  		print "Homing Axis " + str(AxisNr)
  		time.sleep(0.5)
  	
	pos = getPosition(AxisNr)
  	print "Axis " + str(AxisNr) + " homed at position: " + pos
	
	if AxisNr == ROTARYAXIS: #redefine homeswitch to be the end switch again
		SetLimitSwitch(ROTARYAXIS,0)
		SetParameter(ROTARYAXIS,"VM",str(InitialMaxVelocity))

def ReadParameter(AxisNr, ParameterStr):
  serFeedtrough.write("\n"+ str(AxisNr) + "PR " + ParameterStr + "\n")
  replyStr = serFeedtrough.readline()
  return replyStr

def SetParameter(AxisNr, ParameterStr, Value):
  serFeedtrough.write("\n"+ str(AxisNr) + ParameterStr + "=" + str(Value) + "\n")

def MonitorParameter(AxisNr,ParameterStr):
  param = ReadParameter(AxisNr,ParameterStr)
  print ParameterStr + "=" + param



# open serial ports (select ports)
def initFeedtrough(COMPort):
   try:
      global serFeedtrough
      serFeedtrough = serial.Serial('/dev/ttyUSB'+str(COMPort), 9600, 8, 'N', 1, timeout=1)
   except:
      print "Error opening com port. Quitting."
      sys.exit(0)

   print "Opening " + serFeedtrough.portstr
   print serFeedtrough.isOpen()
   print serFeedtrough.portstr

def CalcMovementTimeDefault(Microsteps):
	T = CalcMovementTimeGeneral(abs(Microsteps),ROTARY_MAXVELOCITY, ROTARY_MINVELOCITY, ROTARY_ACCELERATION, ROTARY_DECELERATION)
	return T


def CalcMovementTimeGeneral(Microsteps,Acceleration,Deceleration,MaxSpeed,MinSpeed):
  # calculates the movement time according with a trapezoidal model with the given
  # parameters. N
	TAcceleration = (MaxSpeed-MinSpeed)/Acceleration
  	StepsAcceleration = (MaxSpeed+MinSpeed)*TAcceleration*0.5

	TDeceleration = (MaxSpeed-MinSpeed)/Acceleration
	StepsDeceleration = (MaxSpeed+MinSpeed)*TDeceleration*0.5
	TConstantSpeed = (abs(Microsteps) - (StepsAcceleration + StepsDeceleration))/MaxSpeed
  
	TTotal = TAcceleration+TDeceleration+TConstantSpeed

	return TTotal
def StopMovement(AxisNr):
  serFeedtrough.write(str(AxisNr)+"\x1B")
  print "Stop Axis " + str(AxisNr)
  Move = ReadParameter(AxisNr,"MV")
  print "Axis " + str(AxisNr) + " MV = " + Move



####################### End Feed-trough control ################################
