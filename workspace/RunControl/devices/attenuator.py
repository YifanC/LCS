__author__ = 'matthias'

from base.motor import *
from math import cos, acos, sqrt, degrees, radians, floor


class Attenuator(Motor):
    def __init__(self, RunNumber=0):
        self.name = "attenuator"
        super(Attenuator, self).__init__(name=self.name, RunNumber=RunNumber)
        self.state = 0
        self.comport = None
        self.comBaudrate = 38400
        self.comTimeout = 0.5
        self.comEcho = True
        self.InfoInstruction = "p"

        self.comEnd = "\r"
        self.color = True

        # config file
        self.config_load()

        self.InstructionSet = {"getInfo": "p",
                               "getName": "n",
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
                               "reset@home": "zr",
                               "setHome": "h",
                               "onoff": "en",
                               "DEBUG": "xx"}

        self.comDefaultReplyLength = 100
        self.comInfoReplyLength = 300

        self.comPrefix = ""
        self.comSetCommand = " "
        self.comGetCommand = ""
        self.comReplyPrefix = ""
        self.comEnd = "\r"

        self.microsteps = 2
        self.stepsperdegree = 43.333
        self.offsetZeroTrans = 0

    """" Calibration of Attenuator: When the attenuator is initialized the attenuator goes to its hardware home position
     defined by a switch. This is the reference point for every position. The minimum of laser transmission has an
     arbitrary offset to this position and has to be defined by hand (offsetZeroTrans variable). This can be done by using 
     the setZero() function when the zero transmission position is found by hand. The function also stores the value in a file
     so it is available for future use.
     Normal Operation: (config file is automatically loaded)
     1. init com port
     2. init controller
     3. enable motor
     4. home
     5. set transmission
     6. disable motor
     7. close com port """

    def init(self):
        # load configuration
        self.microsteps = int(self.config.MICROSTEP_RESOLUTION)
        self.offsetZeroTrans = self.config.ZERO_TRANSMISSION_OFFSET

        # write configuration
        self.setParameter("microsteps", self.microsteps)

    def getName(self):
        """" get the name of the devcie if it is not consistens wit att... abort """
        self.com_write(self.InstructionSet["getName"])
        reply = self.com_recv(self.comInfoReplyLength)

        if reply[:3] == "att":
            self.printMsg("Device name: " + str(reply))
            return 0
        else:
            self.printError("Device name: " + str(reply) + " not recognised --> quitting")
            sys.exit(-1)

    def enableMotor(self):
        self.printMsg("Motor on", True)
        return self.setParameter("onoff", 1)

    def disableMotor(self):
        self.printMsg("Motor off", True)
        return self.setParameter("onoff", 0)

    def getParameter(self, parameter):
        """"the only way to get a parameter value os to ask for an information string from the attenuator. The string
        looks like this: USB: [1] a=[2] d=[3] s=[4] wm=[5] ws=[6] wt=[7] r=[8] en:[9] zr:[10] zs:[11]
        The function finds the desired parameter value of the device by looking for the parameter according to the
        Instruction set in this reply string. finding zs will probably not work because of the missing " " in the end """
        reply = self.getInfo()

        special = set(("en", "zs", "zr"))
        if self.InstructionSet[parameter] in special:
            instruction = self.InstructionSet[parameter] + ":"
        else:
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
        msg = self.InstructionSet["isMoving"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)

        if reply[:1] is "0":
            return False
        else:
            return True

    def setZero(self, value=None):
        """ Set the current position (if no value is supplied) as the new zero transmission position """

        # if self.isMoving():
        # self.printError("motor is moving, will not set zero position")
        # return -1
        if value == None:
            pos = self.getPosition()
            self.printMsg("New zero transmission position offset is " + str(pos))
            self.offsetZeroTrans = pos
        else:
            self.printMsg("New zero transmission position offset is " + str(value))
            self.offsetZeroTrans = value

        self.config.ZERO_TRANSMISSION_OFFSET = self.offsetZeroTrans
        #self.config_dump()

    def home(self, monitor=False, display=False):
        """ Go to the home switch (hardware switch) and reset position counter """
        self.printMsg("Going to hardware home switch")
        msg = self.InstructionSet["hardwareHome"]
        self.com_write(msg)

        if monitor is True:
            return self.monitorPosition(self.offsetZeroTrans, display)
        else:
            return 0

    def getTransmission(self):
        """" Get the transmission coefficient based on the current position """
        pos = self.getPosition() - self.offsetZeroTrans
        angle = pos / (self.stepsperdegree * self.microsteps)
        transmission = (1. - cos(radians(angle)) ** 2)

        return transmission

    def setTransmission(self, value, monitor=True, display=True):
        """" Go to the position according to the transmission for the required position"""

        value = float(value)

        if 0. >= value >= 1.:
            self.printError("Transmission value out of bounds")
            return -1

        # numner of steps to be driven from the zero transmission position
        steps = floor(degrees(acos(sqrt(1. - value))) * self.stepsperdegree * self.microsteps)

        position = steps + self.offsetZeroTrans
        reachedposition = self.moveAbsolute(position, monitor, display)

        if reachedposition == 0:
            self.printMsg("Set transmission to: " + str(value * 100) + "%")
            return 0
        else:
            self.printError("Could not set transmission")
            return -1

    def checkParameter(self, parameter, value, echo):
        SetValue = self.getParameter(parameter)
        if SetValue == str(value):
            return 0
        else:
            return -1
