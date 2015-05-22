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

    def printMsg(self, string):
        print time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string

    def printError(self, string):
        print bcolors.WARNING + time.strftime('%H:%M ', time.localtime()) + self.name + ": " + string + bcolors.ENDC


class Motor(Device):
    def __init__(self, comport):
        self.com = serial.Serial()
        self.comBaudrate = 9600
        self.comport = comport
        self.InfoInstruction = None
        self.InfoMsgLength = None
        self.StandartMsgLength = None
        self.comEnd = None

    def device_type(self):
        pass

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

    def com_write(self, msg):
        """ write message to comport """
        try:
            self.com.isOpen()
            self.com.write(msg + self.comEnd)
        except:
            self.printError("Could not write message \"" + msg + "\" to com port (" + self.com.portstr + ")")

            sys.exit(1)

    def com_recv(self,msg_length):
        """ read message to comport """
        try:
            self.com.isOpen()
            msg = self.com.read(msg_length)
        except:
            self.printError("Could not read to com port")
            sys.exit(1)
        return msg

    def printComStatus(self):
        self.printMsg(str(self.com))

    def getInfo(self, output=False):
        """ If output is set an the answer from """
        if self.InfoInstruction != None and self.InfoMsgLength != None:
            self.com_write(self.InfoInstruction)
            infomsg = self.com_recv(self.InfoMsgLength)

            if output == True:
                self.printMsg("Info:\n" + infomsg)
        else:
            self.printError("Info instruction or answer length is not defined")

class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'