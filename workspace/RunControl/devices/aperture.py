__author__ = 'matthias'

from base.base import *


class Aperture(Motor):
    def __init__(self):
        self.name = "aperture"
        self.state = 0
        self.comport = None
        self.comBaudrate = 9600
        self.comTimeout = 0
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
                               "moveRelative": "x",  # positive direction closes / negative opens
                               "setDirection": "h"}

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

