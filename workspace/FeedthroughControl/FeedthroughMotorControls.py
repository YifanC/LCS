#!/usr/bin/env python

import sys
import time
import serial


sgn = lambda x: (x > 0) - (x < 0)

ROTARYAXIS = 1
LINEARAXIS = 2

# one full evolution of the motor has:
# - steps: 200
# - microsteps: 200*MS (200*256=51200)
MICROSTEPS = 256

ONETURN = 200 * MICROSTEPS  # steps of on full turn

ROTARY_MAXVELOCITY = 15000
ROTARY_MINVELOCITY = 1000
ROTARY_ACCELERATION = 30000
ROTARY_DECELERATION = 30000

TURNSPERDEGHOR = ONETURN / (3.6)
TURNSPERDEGVER = ONETURN / 14.501537
k = 0

########################### Feed-trough control ################################
def initAxis1():
    print "----------- Axis 1 (Rotary) Initialization -----------"
    # first signal on the connection
    serFeedtrough.write("\n")

    # set microstep resolution to maximum
    SetParameter(ROTARYAXIS,"MS", MICROSTEPS)

    # set acceleration and deceleration to 100000 microsteps/s
    SetParameter(ROTARYAXIS, "A", ROTARY_ACCELERATION)
    SetParameter(ROTARYAXIS, "D", ROTARY_DECELERATION)

    # set start speed and end speed of the movement
    SetParameter(ROTARYAXIS, "VI", ROTARY_MINVELOCITY)
    SetParameter(ROTARYAXIS, "VM", ROTARY_MAXVELOCITY)
    # max velocity is stable up to 80000 M/s


    # set holding and run current in percent
    SetParameter(ROTARYAXIS, "HC", 5)
    SetParameter(ROTARYAXIS, "RC", 80)

    # one can only define one home switch, so make shure no other
    # switch is defined as home switch

    SetParameter(ROTARYAXIS, "S3", "0,0,0")

    SetLimitSwitch(ROTARYAXIS, 0)
    ##tstart = (maxVelocity-initialVelocity)/acceleration

def initAxis2():
    print '----------- Axis 2 (Linear) Initialization -----------'
    # first signal on the connection
    serFeedtrough.write("\n")

    # set motor settling time in ms
    #serFeedtrough.write("\n2MT=2000\n")

    # set microstep resolution to maximum
    serFeedtrough.write("\n2MS" + str(MICROSTEPS) + "\n")

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
    serFeedtrough.write("\n\n" + str(AxisNr) + "PR AL\n")
    reply = serFeedtrough.read(850)
    print reply
    return reply


def moveVertical(inst):
    if sgn(inst) == 1:
        print 'Axis2: going up ' + str(inst) + ' microsteps'
    else:
        print 'Axis2: going down ' + str(-inst) + ' microsteps'

    serFeedtrough.write('\n2MR ' + str(inst) + '\n')


##while AxisMoving(2):
##        print 'Axis2 Position: ' + getPosition(2)

def moveHorizontal(inst):
    if sgn(inst) == 1:
        print 'Axis1: going left ' + str(inst) + ' microsteps'
    else:
        print 'Axis1: going right ' + str(-inst) + ' microsteps'

    serFeedtrough.write('\n1MR ' + str(inst) + '\n')


##while AxisMoving(1):
##        print 'Axis1 Position: ' + getPosition(1)

def moveAbsoluteHorizintal(pos):
    print "Going to " + str(pos)

    serFeedtrough.write('\n1MA ' + str(pos) + '\n')


def getPosition(AxisNr):
    serFeedtrough.write("\n" + str(AxisNr) + "PR P\n")
    reply = serFeedtrough.read(25)
    return reply


def AxisMoving(AxisNr):
    # returns: 1 if axis is moving, 0 if not. And -1 for non-identifiable answer
    serFeedtrough.write("\n" + str(AxisNr) + "PR MV\n")
    reply = serFeedtrough.readline()
    if ('0' in reply):
        return 0
    elif ('1' in reply):
        return 1
    else:
        return -1


def SetHomeSwitch(AxisNr, Attempts):
    time.sleep(0.5)
    # exit criterion for recursive
    if Attempts == 4:
        print "Setting the HomeSwitch failed 5 times -> exiting"
        sys.exit()

    if AxisNr == ROTARYAXIS:
        print "Axis 1: Set S2 as home switch"
        setOK = SetParameter(ROTARYAXIS, "S2", "1,1,0")  # set counterclockwise endswitch as home switch
        time.sleep(0.5)
        if setOK != 0:
            SetHomeSwitch(ROTARYAXIS, Attempts + 1)


def SetLimitSwitch(AxisNr, Attempts):
    time.sleep(0.5)
    if Attempts == 4:
        print "Setting the limit switch failed 5 times -> exiting"
        sys.exit()

    if AxisNr == ROTARYAXIS:
        print "Axis 1: Set S1 and S2 as limit switches"
	setOK = SetParameter(ROTARYAXIS, "S1", "2,1,0")  # set counterclockwise endswitch as home switch
        setOK += SetParameter(ROTARYAXIS, "S2", "3,1,0")  # set counterclockwise endswitch as home switch
        time.sleep(0.5)
        # check if S2 was set correctly
        if setOK != 0:
            print "Axis" + str(AxisNr) + " limit switches were not set correctly"
            SetLimitSwitch(ROTARYAXIS, Attempts + 1)


def HomeAxis(AxisNr):
    if AxisNr == ROTARYAXIS:
        InitialMaxVelocity = int(ReadParameter(ROTARYAXIS, "VM"))
        SetParameter(ROTARYAXIS, "VM", 15000)  # reduce spped so we move slowly into endswitch
        SetHomeSwitch(ROTARYAXIS, 0)
        serFeedtrough.write("\n" + str(AxisNr) + "HM 1\n")
        # 1 - Slew at VM in the minus direction and Creep at VI in the plus direction.
        # 2 - Slew at VM in the minus direction and Creep at VI in the minus direction.
        # 3 - Slew at VM in the plus direction and Creep at VI in the minus direction.
        # 4 - Slew at VM in the plus direction and Creep at VI in the plus direction.

        while AxisMoving(AxisNr):
            print "Homing Axis " + str(AxisNr)
            time.sleep(0.5)
            pos = getPosition(AxisNr)
        print "Axis " + str(AxisNr) + " homed at position: " + pos
        SetLimitSwitch(ROTARYAXIS, 0)
        SetParameter(ROTARYAXIS, "VM", str(InitialMaxVelocity))
    elif AxisNr == LINEARAXIS:
        print "will not home linear axis: no procedure defined yet!"
        return -1


def ReadParameter(AxisNr, ParameterStr):
    serFeedtrough.write("\n" + str(AxisNr) + "PR " + ParameterStr + "\n")
    replyStr = serFeedtrough.readline()
    return replyStr[0:-2] # crop last two characters = "\r\n"


def SetParameter(AxisNr, ParameterStr, Value):
    print " SetParameter: " + str(AxisNr) + ParameterStr + "=" + str(Value),
    serFeedtrough.write("\n" + str(AxisNr) + ParameterStr + "=" + str(Value) + "\n")
    time.sleep(0.1)
    SetValue = ReadParameter(AxisNr, ParameterStr)

    if SetValue.replace(" ","") == str(Value):
        print "-> OK"
        return 0
    else:
        print "-> FAILIURE: Controller setting is: " + SetValue.replace(" ","")
        return -1

def MonitorParameter(AxisNr, ParameterStr):
    param = ReadParameter(AxisNr, ParameterStr)
    print ParameterStr + "=" + param


# open serial ports (select ports)
def initFeedtrough(COMPort):
    try:
        global serFeedtrough
        serFeedtrough = serial.Serial('/dev/ttyUSB' + str(COMPort), 9600, 8, 'N', 1, timeout=1)
    except:
        print "Error opening com port. Quitting."
        sys.exit(0)

    print "Opening " + serFeedtrough.portstr
    print serFeedtrough.isOpen()
    print serFeedtrough.portstr


def CalcMovementTimeDefault(Microsteps):
    T = CalcMovementTimeGeneral(abs(Microsteps), ROTARY_MAXVELOCITY, ROTARY_MINVELOCITY, ROTARY_ACCELERATION,
                                ROTARY_DECELERATION)
    return T


def CalcMovementTimeGeneral(Microsteps, MaxSpeed, MinSpeed, Acceleration, Deceleration):
    # calculates the movement time according with a trapezoidal model with the given
    # parameters.

    FMicrostepts = float(Microsteps)
    FMaxSpeed = float(MaxSpeed)
    FMinSpeed = float(MinSpeed)
    FAcceleration = float(Acceleration)
    FDecceleration = float(Deceleration)

    TAcceleration = (FMaxSpeed - FMinSpeed) / FAcceleration
    StepsAcceleration = (FMaxSpeed + FMinSpeed) * TAcceleration * 0.5
    TDeceleration = (FMaxSpeed - FMinSpeed) / FDecceleration

    StepsDeceleration = (FMaxSpeed + FMinSpeed) * TDeceleration * 0.5
    TConstantSpeed = (abs(Microsteps) - (StepsAcceleration + StepsDeceleration)) / FMaxSpeed
    TTotal = TAcceleration + TDeceleration + TConstantSpeed

    return TTotal


def StopMovement(AxisNr):
    serFeedtrough.write(str(AxisNr) + "\x1B")
    print "Stop Axis " + str(AxisNr)
    Move = ReadParameter(AxisNr, "MV")
    print "Axis " + str(AxisNr) + " MV = " + Move


####################### End Feed-trough control ################################
