#!/usr/bin/env python

import sys
import time
import serial


sgn = lambda x: (x > 0) - (x < 0)

ROTARYAXIS = 2
LINEARAXIS = 1

DictAxis = {LINEARAXIS: "Linear Axis", ROTARYAXIS: "Rotary Axis"}

# one full evolution of the motor has:
# - steps: 200
# - microsteps: 200*MS (200*256=51200)
MICROSTEPS = 256

ONETURN = 200 * MICROSTEPS  # steps of on full turn

ROTARY_MAXVELOCITY = 15000
ROTARY_MINVELOCITY = 1000
ROTARY_ACCELERATION = 30000
ROTARY_DECELERATION = 30000
ROTARY_HOMING_SPEED = 15000
ROTARY_MAX_HOMING_OVERSHOOT = 0

LINEAR_HOMING_SPEED = 8000
LINEAR_MAX_HOMING_OVERSHOOT = -18000

TURNSPERDEGHOR = ONETURN / (3.6)
TURNSPERDEGVER = ONETURN / 14.501537
k = 0

# ########################## Feed-trough control ################################
def initRotaryAxis():
    print "----------- " + DictAxis[ROTARYAXIS] + " Initialization -----------"
    # set microstep resolution to maximum
    SetParameter(ROTARYAXIS, "MS", MICROSTEPS)

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

    SetLimitSwitches(ROTARYAXIS, 0)
    ##tstart = (maxVelocity-initialVelocity)/acceleration


def initLinearAxis():
    print "----------- " + DictAxis[LINEARAXIS] + " Initialization -----------"
    # set motor settling time in ms

    # set microstep resolution to maximum
    SetParameter(LINEARAXIS, "MS", MICROSTEPS)

    # set acceleration and deceleration to 100000 microsteps/s
    SetParameter(LINEARAXIS, "A", 50000)
    SetParameter(LINEARAXIS, "D", 50000)

    # set start speed and end speed of the movement
    SetParameter(LINEARAXIS, "VI", 1000)
    SetParameter(LINEARAXIS, "VM", 15000)

    # set holding and run current in percent
    SetParameter(LINEARAXIS, "HC", 10)
    SetParameter(LINEARAXIS, "RC", 80)

    # one can only define one home switch, so make shure no other
    # switch is defined as home switch
    SetParameter(LINEARAXIS, "S3", "0,0,0")

    SetLimitSwitches(LINEARAXIS, 0)


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

    SendCommand(LINEARAXIS, "MR", str(inst))


##while AxisMoving(2):
##        print 'Axis2 Position: ' + getPosition(2)

def moveHorizontal(inst):
    if sgn(inst) == 1:
        print 'Axis1: going left ' + str(inst) + ' microsteps'
    else:
        print 'Axis1: going right ' + str(-inst) + ' microsteps'

    SendCommand(ROTARYAXIS, "MR", str(inst))


##while AxisMoving(1):
##        print 'Axis1 Position: ' + getPosition(1)

def moveAbsoluteHorizintal(pos):
    print "Going to " + str(pos)
    SendCommand(ROTARYAXIS, "MA", str(pos))


def getPosition(AxisNr):
    reply = ReadParameter(AxisNr, "P")
    return int(reply)


def AxisMoving(AxisNr):
    # returns: 1 if axis is moving, 0 if not. And -1 for non-identifiable answer
    reply = ReadParameter(AxisNr, "MV")
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
        print DictAxis[AxisNr] + " Set S2 as home switch"
        setOK = SetParameter(ROTARYAXIS, "S2", "1,1,0")  # set counterclockwise endswitch as home switch
        time.sleep(0.5)
        if setOK != 0:
            SetHomeSwitch(ROTARYAXIS, Attempts + 1)

    if AxisNr == LINEARAXIS:
        print DictAxis[AxisNr] + " Set S2 as home switch"
        setOK = SetParameter(LINEARAXIS, "S2", "1,1,0")  # set lower endswitch as home switch
        time.sleep(0.5)
        if setOK != 0:
            SetHomeSwitch(LINEARAXIS, Attempts + 1)


def SetLimitSwitches(AxisNr, Attempts):
    time.sleep(0.5)
    if Attempts == 4:
        print "Setting the limit switch failed 5 times -> exiting"
        sys.exit()

    print DictAxis[AxisNr] + ": Set S1 and S2 as limit switches"
    setOK = SetParameter(AxisNr, "S1", "2,1,0")  # set counterclockwise endswitch as home switch
    setOK += SetParameter(AxisNr, "S2", "3,1,0")  # set counterclockwise endswitch as home switch
    time.sleep(0.5)
    # check if S2 was set correctly
    if setOK != 0:
        print DictAxis[AxisNr] + " limit switches were not set correctly"
        SetLimitSwitches(AxisNr, Attempts + 1)


def HomeAxis(AxisNr):
    if AxisNr == ROTARYAXIS:
        HOMING_SPEED = ROTARY_HOMING_SPEED
        MAX_HOMING_OVERSHOOT = ROTARY_MAX_HOMING_OVERSHOOT
        return -1
    elif AxisNr == LINEARAXIS:
        HOMING_SPEED = LINEAR_HOMING_SPEED
        MAX_HOMING_OVERSHOOT = LINEAR_MAX_HOMING_OVERSHOOT
    else:
        print "Axis not recognised"
        return -1

    # check if manual calibration of zero position was performed
    Counts = int(ReadParameter(AxisNr, "R1"))
    if Counts < 1:
        print " " + DictAxis[AxisNr] + ": Manual homing was not performed, aporting homing procedure."
        return -1

    # reduce speed so we move slowly into endswitch
    InitialMaxVelocity = int(ReadParameter(AxisNr, "VM"))
    SetParameter(AxisNr, "VM", HOMING_SPEED)

    # start homing procedure
    SetHomeSwitch(AxisNr, 0)
    print " Homing Axis " + DictAxis[AxisNr]
    SendCommand(AxisNr, "HM", 1)

    # monitor the movement and abort in the movement goes out too far from the known zero position
    while AxisMoving(AxisNr):
        pos = int(getPosition(AxisNr))
        if pos < MAX_HOMING_OVERSHOOT:
            StopMovement(AxisNr)
            SetParameter(AxisNr, "R1", 0)
            SetLimitSwitches(AxisNr, 0)
            print " Position went to far over home switch, movement was stopped"
            return -1
        print "\r" + " Position: " + str(pos),
        sys.stdout.flush()
        time.sleep(0.1)

    # go back to normal operation
    SetLimitSwitches(AxisNr, 0)
    SetParameter(AxisNr, "VM", str(InitialMaxVelocity))
    SetParameter(AxisNr, "R1", Counts + 1)
    return 0


def ReadParameter(AxisNr, ParameterStr):
    serFeedtrough.write("\n" + str(AxisNr) + "PR " + ParameterStr + "\n")
    replyStr = serFeedtrough.readline()
    return replyStr[0:-2]  # crop last two characters = "\r\n"


def SetParameter(AxisNr, ParameterStr, Value):
    print " SetParameter: " + DictAxis[AxisNr] + " " + ParameterStr + "=" + str(Value),
    serFeedtrough.write("\n" + str(AxisNr) + ParameterStr + "=" + str(Value) + "\n")
    time.sleep(0.1)
    SetValue = ReadParameter(AxisNr, ParameterStr)

    if SetValue.replace(" ", "") == str(Value):
        print "-> OK"
        return 0
    else:
        print "-> FAILIURE: Controller setting is: " + SetValue.replace(" ", "")
        return -1


def MonitorParameter(AxisNr, ParameterStr):
    param = ReadParameter(AxisNr, ParameterStr)
    print ParameterStr + "=" + param


def SendCommand(AxisNr, CommandString, Attribute=""):
    print " Command sent to " + DictAxis[AxisNr] + ": " + CommandString + " " + str(Attribute) + "\n"
    serFeedtrough.write("\n" + str(AxisNr) + CommandString + " " + str(Attribute) + "\n")


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
    SendCommand(AxisNr, "\x1B")
    print "Stop " + DictAxis[AxisNr]
    Move = ReadParameter(AxisNr, "MV")
    print DictAxis[AxisNr] + " MV = " + Move


####################### End Feed-trough control ################################
