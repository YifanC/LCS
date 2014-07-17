#!/usr/bin/env python

import sys, time, glob
import serial

sgn = lambda x : (x>0) - (x<0)


MICROSTEPS = 256                # one full evolution of the motor has:
                                #   - steps: 200 
                                #   - microsteps: 200*MS (200*256=51200)        
ONETURN = 200*MICROSTEPS        # steps of on full turn

MAXVELOCITY = 1000              # maximum velocity in microsteps/s

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
        serFeedtrough.write("\n1A=10000\n")
        serFeedtrough.write("\n1D=10000\n")
        
        # set start speed and end speed of the movement
        serFeedtrough.write("\n1VI=1000\n")
        serFeedtrough.write("\n1VM=20000\n")

        # set holding and run current in percent
        serFeedtrough.write("\n1HC=5\n")
        serFeedtrough.write("\n1RC=80\n")


        #reply = getInfo(2)
        ##maxVelocity = float(readValue(reply,"VM"))
        
        ##initialVelocity = float(readValue(reply,"VI"))
        ##acceleration =  float(readValue(reply,"A"))
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

        
        #reply = getInfo(2)
        ##maxVelocity = float(readValue(reply,"VM"))
        
        ##initialVelocity = float(readValue(reply,"VI"))
        ##acceleration =  float(readValue(reply,"A"))
        ##tstart = (maxVelocity-initialVelocity)/acceleration
        
def readValue(feed, inst):
        # read the value of the given string from the feed obtained by getInfo
        n = len(inst) + 3
        start = feed.find(inst + " = ")
        end = feed.find('\n',start)
        
        ##if inst in feed[start:end]:     
        ##        print inst + " = " + feed[start+n:end]
        ##        return float(feed[start+n:end])
        ##else: sys.exit('Error while reading setup')

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


def HomeAxis(AxisNr):
  # define homing procedure
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


def ReadParameter(AxisNr, ParameterStr):
  serFeedtrough.write("\n"+ str(AxisNr) + "PR " + ParameterStr + "\n")
  replyStr = serFeedtrough.readline()
  replyValue = int(replyStr)
  return replyValue

def SetParameter(AxisNr, ParameterStr, Value):
  serFeedtrough.write("\n"+ str(AxisNr) + ParameterStr + "=" + str(Value) + "\n")

def MonitorParameter(AxisNr,ParameterStr):
  param = ReadParameter(AxisNr,ParameterStr)
  print ParameterStr + "=" + str(param)


# open serial ports (select ports)
def initFeedtrough():
   try:
      global serFeedtrough
      serFeedtrough = serial.Serial('/dev/ttyUSB0', 9600, 8, 'N', 1, timeout=1)
   except:
      print "Error opening com port. Quitting."
      sys.exit(0)

   print "Opening " + serFeedtrough.portstr
   print serFeedtrough.isOpen()
   print serFeedtrough.portstr


####################### End Feed-trough control ################################
