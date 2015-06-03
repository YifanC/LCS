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
                               "moveAbsolute": None,
                               "moveRelative": None}

        self.comDefaultReplyLength = 15
        self.comInfoReplyLength = 100

        self.comPrefix = "A1"
        self.comSetCommand = "s"
        self.comGetCommand = "t"
        self.comEnd = "\r"



    def turnOn(self):
        pass

    def isMoving(self):
        """ This function returns True if the motor is moving and False if it is not"""
        msg = self.InstructionSet["isMoving"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
	return reply


