__author__ = 'matthias'

from base.base import *
import struct
from math import copysign


class Mirror(Motor):
    def __init__(self, name):
        self.name = name
        super(Mirror, self).__init__(name=self.name)
        self.state = 0
        self.comport = "/dev/ttyUSB6"
        self.comBaudrate = 9600
        self.comTimeout = 1
        self.comEcho = False
        self.InfoInstruction = ""
        self.InfoMsgLength = 100
        self.StandartMsgLength = 15
        # config file
        #self.config_setfile()
        #self.config_load()

        self.InstructionSet = {"getName": 50,
                               "stopMovement": 23,
                               "home": 1,
                               "status": 54,
                               "getPosition": 60,
                               "moveAbsolute": 20,  # no absolute movement implemented in hardware
                               "moveRelative": 21}  # positive direction closes / negative opens

        # Just a guess! TODO: Get the axis right
        self.dict_axis = {"horizontal": 1,
                          "vertical": 2}

        self.axis = 1

    def communicate(self, command, data=0):

        msg = struct.pack("<BBl", self.axis, command, data)
        reply = self.com_write(msg, echo=True)
        self.translate_reply(reply)
	self.printMsg(reply)

    def translate_reply(self, reply):
        r = [0,0,0,0,0,0]
        for i in range (6):
            r[i] = ord(reply[i])

        self.printMsg(r)
        return r

    def setParameter(self, parameter):
        pass

    def getParameter(self, parameter, value):
        pass

    def readSerialNumber(self):

        pass
    def moveAbsolute(self, value, monitor=False, display=False, delta=10):
        pass

    def moveRelative(self, value, monitor=False, display=False, delta=10):
        pass

    def getPosition(self):
        pass

    def isMoving(self):
        pass
