__author__ = 'matthias'

import serial
from abc import ABCMeta, abstractmethod
import sys
import time
import json
import os


class Device(object):
    def __init__(self, name, com):
        self.name = name
        self.com = com
        self.state = 0
        self.State = {0: "Not Initialized",
                      1: "Ready",
                      2: "Error"}
        # switch this to false if using bpython
        self.color = True

        self.config = None

    def printMsg(self, string, nonewline=False):
        if nonewline == True:
            print time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string,
        else:
            print time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string


    def printError(self, string):
        if self.color is True:
            print bcolors.FAIL + time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string + bcolors.ENDC
        else:
            print time.strftime('%H:%M ', time.localtime()) + self.name + " ERROR" +": " + string

    def config_setfile(self, filename=-1):
        if filename == -1:
            string = "config_" + str(self.name) + ".json"
            self.printMsg("Using default config file (devices/" + string + ")")
            self.configfilename = string
        else:
            self.printMsg("Using config file: " + str(filename))
            self.configfilename = str(filename)

    def config_load(self):
        with open(self.configfilename, 'r') as configfile:
            self.config = json.load(configfile, object_hook=self.Config)
            configfile.close()

    def config_dump(self):
        self.printMsg("Storing configuration")
        with open(self.configfilename, 'w') as configfile:
            json.dump(self.config.__dict__, configfile)
            configfile.close()

    class Config():
        """" Config class just to translate the json file into a dict """

        def __init__(self, f):
            self.__dict__ = f


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
                self.printError("Echo expected but was different / not received: " + reply)
                return -1

    def com_recv(self, msg_length=10):
        """ read message from comport """
        try:
            self.com.isOpen()
            msg = self.com.read(msg_length)
        except:
            self.printError("Could not read to com port")
            sys.exit(1)
        return self.msg_filter(msg)

    def msg_filter(self, msg):
        """ function which filters the output, should be defined in the device class if required. Useful for example if there
        is axis information which is sent in a reply."""
        return msg


    def printComStatus(self):
        self.printMsg(str(self.com))


class Motor(ComSerial):
    """" At the moment only an idea of a nice classe """

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
            # self.printMsg(string + bcolors.OKGREEN + " -> OK" + bcolors.ENDC, True)
            self.printMsg(string + " -> OK", False)
            return 0
        else:
            self.printError(string + " failed")
            return -1

    def stopMovement(self):
        msg = self.InstructionSet["stopMovement"]
        self.com_write(msg)

    def moveRelative(self, value, monitor=False, display=False, delta=10):
        self.printMsg("Moving relative: " + str(value) + " steps")
        if display is True:
            monitor = True

        value = int(value)
        msg = self.InstructionSet["moveRelative"] + self.comSetCommand + str(value)

        # get current position for monitoring
        if monitor is True:
            pos_start = self.getPosition()

        self.com_write(msg)
        if monitor is True:
            return self.monitorPosition(value + pos_start, display, delta)
        return 0

    def moveAbsolute(self, value, monitor=False, display=False, delta=10):
        """ Moving motor to an absolute position, the movement can be monitored if an getPosition and a isMoving function
        is supplied. """
        self.printMsg("Moving absolute to: " + str(value) + " steps")
        if display is True:
            monitor = True

        value = int(value)
        msg = self.InstructionSet["moveAbsolute"] + self.comSetCommand + str(value)
        self.com_write(msg)

        if monitor is True:
            return self.monitorPosition(value, display, delta)
        return 0


    def getPosition(self):
        msg = self.comGetCommand + self.InstructionSet["getPosition"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
        return reply

    def monitorPosition(self, endPosition, display=True, delta=10):
        time.sleep(0.1)
        pos = self.getPosition()
        while self.isMoving():
            if display is True:
                self.printMsg("Position: " + str(pos))
                pos = self.getPosition()
                time.sleep(0.1)

        pos = self.getPosition()
        if display is True:
            self.printMsg("Position: " + str(pos))
        # check if position was reached
        if (endPosition - delta) <= pos <= (endPosition + delta):
            return 0
        else:
            return -1


class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'
