__author__ = 'matthias'
__author__ = 'matthias'

from base.base import *


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
                               "startSpeed": "l",
                               "endSpeed": "v",
                               "acceleration": "a",
                               "decceleration": None,
                               "microsteps": "r",
                               "movementCurrent": None,
                               "idleCurrent": None,
                               "breakMovement": "ms",
                               "stopMovement": None,
                               "isMoving": "ts",
                               "getPosition": "p",
                               "limit1": "1",
                               "limit2": "2",  # limit2 == 1 --> fully open
                               "moveAbsolute": None,
                               "moveRelative": None}

        self.comDefaultReplyLength = 15
        self.comInfoReplyLength = 100

        self.comPrefix = "A1"
        self.comSetCommand = "s"
        self.comGetCommand = "t"
        self.comReplyPrefix = "1:"
        self.comEnd = "\r"


    def turnOn(self):
        pass

    def isMoving(self):
        """ This function returns True if the motor is moving and False if it is not"""
        msg = self.InstructionSet["isMoving"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
        return reply

    def msg_filter(self, msg):
        """ Removes the repl_prefix and trailing '\n' and '\r's from the reply """
        # TODO: Move this to base class, can be useful for every device!
        prefix_length = len(self.comReplyPrefix)
        if msg[:prefix_length] == self.comReplyPrefix:
            return msg[prefix_length:].rstrip()
        else:
            self.printError("Reply prefix (" + str(msg[:prefix_length]) + ") not valid, aborting")
            sys.exit(-1)
