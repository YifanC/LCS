__author__ = 'matthias'

from base.base import *

class Attenuator(Motor):
    def __init__(self):
        self.name = "attenuator"
        self.state = 0
        self.comport = None
        self.comBaudrate = 38400
        self.InfoInstruction = "p"
        self.InfoMsgLength = 100
        self.StandartMsgLength = 10
        self.comEnd = "\r"

        self.InstructionSet = {"getInfo": "p",
                               "startSpeed": None,
                               "endSpeed": "s",
                               "acceleration": "a",
                               "decceleration": "d",
                               "movementCurrent": "wm",
                               "idleCurrent": "ws",
                               "breakMovement": "b",
                               "stopMovement": "st",
                               "isMoving": None,
                               "getPosition": None,
                               "moveAbsolute": "g",
                               "moveRelative": "m"}

        self.comDefaultReplyLength = 10
        self.comInfoReplyLength = 100

        self.comPrefix = ""
        self.comSetCommand = " "
        self.comEnd = "\r"

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

