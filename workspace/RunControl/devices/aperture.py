__author__ = 'matthias'

from base.base import *
from math import copysign


class Aperture(Motor):
    def __init__(self):
        self.name = "aperture"
        self.state = 0
        self.comport = None
        self.comBaudrate = 9600
        self.comTimeout = 1
        self.comEcho = False
        self.InfoInstruction = ""
        self.InfoMsgLength = 100
        self.StandartMsgLength = 15
        self.comEnd = "\r"

        self.InstructionSet = {"getInfo": None,
                               "getName": "c",
                               "enableMotor": "mn",
                               "disableMotor": "mf",
                               "startSpeed": "l",
                               "endSpeed": "v",
                               "acceleration": "a",
                               "decceleration": None,
                               "microsteps": "r",
                               "movementCurrent": None,
                               "idleCurrent": None,
                               "breakMovement": "ms",
                               "stopMovement": None,
                               "isMoving": "s",
                               "getPosition": "p",
                               "limit1": "1",
                               "limit2": "2",  # limit2 == 1 --> fully open
                               "moveAbsolute": None,  # no absolute movement implemented in hardware
                               "moveRelative": "x",
                               "startMovement": "go",
                               "setDirection": "h"}  # positive direction closes / negative opens
        # from

        self.comDefaultReplyLength = 100
        self.comInfoReplyLength = 100

        self.comPrefix = "A1"
        self.comSetPrefix = "s"
        self.comSetCommand = ""
        self.comGetCommand = "t"
        self.comReplyPrefix = "1:"
        self.comReplyEnd = "\r\n"
        self.comEnd = "\r"

    def init(self):
        self.checkName()

    def checkName(self):
        """ Checks the name return by the device, if it is inconsistent with the expected name it stop the execute
        of the script """
        name = self.getName()
        if name[:11] == "8SMC3-RS232":
            self.printMsg("Aperture recognized")
            return 0
        else:
            self.printMsg("Aperture not recovgnized, is this the right port number? -> quitting")
            sys.exit(-1)

    def enableMotor(self):
        self.printMsg("Motor on")
        msg = self.InstructionSet["enableMotor"]
        return self.com_write(msg, echo=True)

    def disableMotor(self):
        self.printMsg("Motor off")
        msg = self.InstructionSet["disableMotor"]
        return self.com_write(msg, echo=True)

    def isMoving(self):
        """ This function returns True if the motor is moving and False if it is not"""
        reply = self.getParameter("isMoving")

        if int(reply[:2]) > 0:
            return True
        else:
            return False

    def moveRelative(self, value, monitor=False, display=False, delta=10):
        self.printMsg("Moving relative: " + str(value) + " steps")
        if display is True:
            monitor = True
        value = int(value)
        # need to determine the sign and then set the direction (negative = open (0), positive = close (1))
        if value >= 0:
            self.setParameter("setDirection", 1, echo=True)
        elif value < 0:
            self.setParameter("setDirection", 0, echo=True)
        else:
            self.printError("Could not determine sign of direction")

        # we have to put together an uppercase hex string of the form "0F1010"
        hexstr_value = '{0:06x}'.format(abs(value)).upper()

        # write the steps to the controller (no movement yet)
        self.setParameter("moveRelative", hexstr_value, echo=True)

        msg = self.InstructionSet["startMovement"]
        self.com_write(msg, echo=True)

        # get current position for monitoring
        if monitor is True:
            pos_start = self.getPosition()

        if monitor is True:
            return self.monitorPosition(value + pos_start, display, delta)
        return 0

    def home(self, where=0):
        """ Homes attenuator to either end switch: if 0 is the fully closed position and 1 is the fully open position"""
        direction = {1: "open", 0: "closed"}
        if where == 0:
            msg = "mr1"
            limit_switch = "limit1"
        elif where == 1:
            msg = "mr0"
            limit_switch = "limit2"
        else:
            self.printError("Homing parameter out of bounds")

        self.com_write(msg, echo=True)
        self.printMsg("going to fully " + direction(where) + " position")
        while int(self.getPosition(limit_switch)) == 0:
            pos = self.getPosition()
            self.printMsg("pos: " + str(pos))

    def msg_filter(self, msg):
        """ Removes the repl_prefix and trailing '\n' and '\r's from the reply """
        # TODO: Move this to base class, can be useful for every device!
        prefix_length = len(self.comReplyPrefix)
        if msg[:prefix_length] == self.comReplyPrefix:
            return msg[prefix_length:].rstrip()
        else:
            self.printError("Reply prefix (" + str(msg[:prefix_length]) + ") not valid, aborting")
            # sys.exit(-1)

    def checkParameter(self, parameter, value, echo):
        print "Echo: " + str(echo)
        return 0

    def convertPosition(self, value):
        return int(value, 16)

