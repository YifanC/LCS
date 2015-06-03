__author__ = 'matthias'
__author__ = 'matthias'

from base.base import *


class Attenuator(Motor):
    def __init__(self):
        self.name = "aperture"
        self.state = 0
        self.comport = None
        self.comBaudrate = 9600
        self.InfoInstruction = ""
        self.InfoMsgLength = 100
        self.StandartMsgLength = 10
        self.comEnd = "\r"

        self.InstructionSet = {"getInfo": None,
                               "startSpeed": None,
                               "endSpeed": None,
                               "acceleration": None,
                               "decceleration": None,
                               "movementCurrent": None,
                               "idleCurrent": None,
                               "breakMovement": None,
                               "stopMovement": None,
                               "isMoving": None,
                               "getPosition": None,
                               "moveAbsolute": None,
                               "moveRelative": None}

        self.comDefaultReplyLength = 10
        self.comInfoReplyLength = 100

        self.comPrefix = "A1"
        self.comSetCommand = "s"
        self.comGetCommand = "t"
        self.comEnd = "\r"

    def turnOn(self):
        pass


