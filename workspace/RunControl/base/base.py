__author__ = 'matthias'

import serial
from abc import ABCMeta, abstractmethod
import sys
import time


class Device(object):

    dictState = {0: "Not Initialized",
                 1: "Ready",
                 2: "Error"}

    def __init__(self, name, com):
        self.name = name
        self.com = com
        self.state = 0

    def printMsg(self, string, nonewline=False):
        if nonewline == True:
            print time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string,
        else:
            print time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string


    def printError(self, string):
        print bcolors.FAIL + time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string + bcolors.ENDC


class ComSerial(Device):
    def __init__(self, comport):
        self.com = serial.Serial()
        self.comBaudrate = 9600
        self.comTimeout = 0.1
        self.comport = comport

	self.comEcho = False

        self.InfoInstruction = None
        self.InfoMsgLength = None
        self.StandardMsgLength = None

        self.comPrefix = ""
        self.comEnd = None
        self.NumberOfAxes = 1

    def com_init(self):
        """ Tries to open com port specified in comport """
        try:
            port = self.comport
            self.com = serial.Serial(port, self.comBaudrate, 8, 'N', 1, timeout=0.1)
        except:
            self.printError("opening fcom port (" + self.comport + ") failed --> quitting")
            sys.exit(1)

        self.printMsg("Com port (" + self.com.portstr + ") opened")

    def com_close(self):
        """ closes com port """
        try:
            self.com.close()

        except:
            self.printError("Com port could not be closed")
            sys.exit(1)

        self.printMsg("Com port (" + self.com.port + ") closed")

    def com_write(self, message):
        """ write message to comport """
	msg = self.comPrefix + message
        self.com.isOpen()
        self.com.write(msg + self.comEnd)

	if self.comEcho is True:
		reply = self.com_recv(len(msg))
		if reply == self.comPrefix + message:
			return 0
		else:
			self.printError("Echo expected but was different / not received: " + reply )
			return -1


    def com_recv(self, msg_length=10):
        """ read message from comport """
        try:
            self.com.isOpen()
            msg = self.com.read(msg_length)
        except:
            self.printError("Could not read to com port")
            sys.exit(1)
        return msg

    def printComStatus(self):
        self.printMsg(str(self.com))


class Motor(ComSerial):
    """" At tje moment only an idea of a nice classe """
    def __init__(self):
        self.InstructionSet = {"getInfo": None,
                               "maxSpeed": None,
                               "startSpeed": None,
                               "acceleration": None,
                               "deceleration": None,
                               "movementCurrent": None,
                               "idleCurrent": None,
                               "breakMovement": None,
                               "stopMovement": None,
                               "isMoving": None,
                               "getPosition": None,
                               "moveAbsolute": None,
                               "moveRelative": None}
	
        self.comDefaultReplyLength = None
        self.comInfoReplyLength = None

        self.comPrefix = None
        self.comSetCommand = None
        self.comGetCommand = None
        self.comEnd = None

    def getInfo(self, output=False):

        if self.InstructionSet['getInfo'] != None and self.comInfoReplyLength != None:

            self.com_write(self.InstructionSet["getInfo"])
            reply = self.com_recv(self.comInfoReplyLength)
            if output == True:
                self.printMsg("Info: " + reply)

            return reply

        else:
            self.printError("Info instruction or answer length is not defined")

    def getParameter(self, parameter):
        msg = self.comGetCommand + self.InstructionSet[parameter]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
        if reply == "":
            self.printError("No reply received")

        return reply

    def setParameter(self, parameter, value):
        msg = self.InstructionSet[parameter] + self.comSetCommand + str(value)
        self.com_write(msg)

        SetValue = self.getParameter(parameter)
        string = "Set " + parameter + "=" + str(value)

        if SetValue == str(value):
            self.printMsg(string + bcolors.OKGREEN + " -> OK" + bcolors.ENDC, True)
            return 0
        else:
            self.printError(string + " failed")
            return -1

    def stopMovement(self):
        msg = self.InstructionSet["stopMovement"]
        self.com_write(msg)

    def moveRelative(self, value):
        msg = self.InstructionSet["moveRelative"] + self.comSetCommand + str(value)
        self.com_write(msg)

    def moveAbsolute(self, value):
        msg = self.InstructionSet["moveAbsolute"] + self.comSetCommand + str(value)
        self.com_write(msg)

    def getPosition(self):
        msg = self.InstructionSet["getPosition"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
	return reply

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
