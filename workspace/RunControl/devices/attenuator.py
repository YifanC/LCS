__author__ = 'matthias'

from base import *
from math import cos, acos, sqrt, degrees, radians, floor


class Attenuator(Motor):
    def __init__(self):
        self.name = "attenuator"
        self.state = 0
        self.comport = None
        self.comBaudrate = 38400
        self.comTimeout = 1
        self.comEcho = True
        self.InfoInstruction = "p"
        self.InfoMsgLength = 100
        self.StandardMsgLength = 10
        self.comEnd = "\r"

        self.InstructionSet = {"getInfo": "p",
                               "startSpeed": None,
                               "endSpeed": "s",
                               "acceleration": "a",
                               "decceleration": "d",
                               "microsteps": "r",
                               "movementCurrent": "wm",
                               "idleCurrent": "ws",
                               "breakMovement": "b",
                               "stopMovement": "st",
                               "isMoving": "o",
                               "getPosition": "o",
                               "moveAbsolute": "g",
                               "moveRelative": "m",
                               "hardwareHome": "zp",
                               "reset@Zero": "zr",
                               "setHome": "h"}

        self.comDefaultReplyLength = 200
        self.comInfoReplyLength = 100

        self.comPrefix = ""
        self.comSetCommand = " "
        self.comGetCommand = ""
        self.comEnd = "\r"

        self.microsteps = 2
        self.stepsperdegree = 43.333
        self.offsetZeroTrans = 0

    """" Calibration of Attenuator: When the attenuator is initialized the attenuator goes to its hardware home position
     defined by a switch. This is the reference point for every position. The minimum of laser transmission has an
     arbitrary offset to this position and has to be defined by hand (self.offsetZeroTrans)."""

    def init(self):
        self.microsteps = self.getParameter("microsteps")


    def getParameter(self, parameter):

        """"the only way to get a parameter value os to ask for an information string from the attenuator. The string
        looks like this: USB: [1] a=[2] d=[3] s=[4] wm=[5] ws=[6] wt=[7] r=[8] en:[9] zr:[10] zs:[11]
        The function finds the desired parameter value of the device by looking for the parameter according to the
        Instruction set in this reply string. finding zs will probably not work because of the missing " " in the end """
        reply = self.getInfo()

        instruction = self.InstructionSet[parameter] + "="

        startStringValue = reply.find(instruction) + len(instruction)
        endStrinValue = reply[startStringValue:].find(" ") + startStringValue

        parameterValue = reply[startStringValue:endStrinValue]
        return parameterValue

    def getPosition(self):
        """ the reply look as follows: [1]:[2] where [1] is a number indicating if the motor is moving or not (0 - idle,
        1,2,3 - moving), and [2] ist the absolute position."""
        msg = self.InstructionSet["getPosition"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
        semicolon_pos = reply.find(";")
        return float(reply[(semicolon_pos + 1):])

    def isMoving(self):
        """ This function returns True if the motor is moving and False if it is not"""
        msg = self.InstructionSet["getPosition"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)

        if reply[:1] is "0":
            return False
        else:
            return True

    def setZero(self, value=None):
        """ Set the current position (if no value is supplied) as the new zero transmission position """

        if self.isMoving():
            self.printError("motor is moving, will not set zero position")
            return -1
	if value == None:
        	pos = self.getPosition()
        	self.printMsg("New zero transmission position offset is " + str(pos))
        	self.offsetZeroTrans = pos
	else:
        	self.printMsg("New zero transmission position offset is " + str(value))
        	self.offsetZeroTrans = value		

    def home(self):
        """ Go to the home switch (hardware switch) and reset position counter """
        self.printMsg("Going to hardware home switch")
        msg = self.InstructionSet["hardwareHome"]
        self.com_write(msg)

    def zero(self):
        """ Go to the home switch (hardware switch) and reset position counter """
        self.printMsg("Going to zero transmission position")
        self.moveAbsolute(self.offsetZeroTrans)
	

    def getTransmission(self):
        pos = self.getPosition() - self.offsetZeroTrans
        angle = pos / (self.stepsperdegree * self.microsteps)
        transmission = (1. - cos(radians(angle))**2)

        return transmission

    def setTransmission(self, value, monitor=True, display=True):
        value = float(value)

        if 0. >= value >= 1.:
            self.printError("Transmission value out of bounds")
            return -1

        # numner of steps to be driven from the zero transmission position
        steps = floor(degrees(acos(sqrt(1. - value))) * self.stepsperdegree * self.microsteps)

        position = steps + self.offsetZeroTrans
        reachedposition = self.moveAbsolute(position, monitor, display)

        if reachedposition == 0:
	    self.printMsg("Set transmission to: " + str(value*100) + "%")
            return 0
	else:
            self.printError("Could not set transmission")
            return -1
        return 0
