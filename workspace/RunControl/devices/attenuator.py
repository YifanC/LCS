__author__ = 'matthias'

from base import *

class Attenuator(Motor):
    def __init__(self):
        self.name = "attenuator"
        self.state = 0
        self.comport = None
        self.comBaudrate = 38400
	self.comTimeout = 0.5
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

	"""" Calibration of Attenuator """



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
	return float(reply[(semicolon_pos + 1):-2])

    def isMoving(self):
	""" This function returns True if the motor is moving and False if it is not"""
        msg = self.InstructionSet["getPosition"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
	
	if reply[:1] is "0":
		return False
	else:
		return True

    def setZero(self):
	""" Set the current position as the new zero transmission position """
	pass

    def zero(self, really=0):
	# TODO: when driving to zero position a strange reply is sent, because of that the following echo has an additional
	# character. This will crash the program.
	""" Go to the home switch (hardware switch) and reset position counter """
	self.printMsg("Going to hardware home switch")
	msg = self.InstructionSet["hardwareHome"]
        self.com_write(msg)

    def getTransmission(self):
	pass

    def setTransmission(self, value):
	if 0 > value > 100:
		self.printError("Transmission value out of bounds")
		return -1
	return 0

