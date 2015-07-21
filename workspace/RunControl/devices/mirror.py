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
        # self.config_setfile()
        #self.config_load()

        self.InstructionSet = {"getName": 50,
                               "stopMovement": 23,
                               "home": 1,
                               "status": 54,
                               "readRegister": 35,
                               "setRegister": 35,
                               "getPosition": 60,
                               "moveAbsolute": 20,  # no absolute movement implemented in hardware
                               "moveRelative": 21}  # positive direction closes / negative opens

        # Just a guess! TODO: Get the axis right
        self.dict_axis = {"horizontal": 1,
                          "vertical": 2}

        self.axis = 1

    def com_send(self, command, instruction=0):
        if instruction == 0:
            instruction = [0, 0, 0, 0]
        else:
            if len(instruction) != 4:
                self.printError("data frame has wrong size")

        msg = struct.pack("<" + 6*"B", self.axis, command, instruction[0], instruction[1], instruction[2], instruction[3])
        reply = self.com_write(msg, echo=True)
        reply = self.translate_reply(reply)
        self.printMsg(reply)

        return reply

    def translate_reply(self, reply):
        r = [0, 0, 0, 0, 0, 0]
        for i in range(6):
            r[i] = ord(reply[i])

        self.printDebug(r)
        return r

    def setParameter(self, parameter):
        pass

    def getParameter(self, parameter, value):
        pass

    def setSerial(self,serial):
        address = 0
        self.writeRegister(address, serial)
        return 0

    def getSerial(self):
        address = 0
        return self.readRegister(address)

    def readRegister(self, register_adr):
        if 0 > register_adr > 127:
            self.printError("Address out of bounds")
            return -1
        instruction = [register_adr, 0, 0, 0]
        reply = self.com_send(self.InstructionSet["readRegister"], instruction)
        self.printMsg(reply[2])
	return reply[2]

    def writeRegister(self, register_adr, value):
        if 0 > register_adr > 127:
            self.printError("Address out of bounds")
            return -1

        if type(value) is not int:
            self.printError("value has wrong format")
            return -1

        if 255 < value < 0:
            self.printError("value is out of bounds")
            return -1

        instruction = [register_adr | 128, value, 0, 0]
        self.com_send(self.InstructionSet["readRegister"], instruction)
        return 0

    def moveAbsolute(self, value, monitor=False, display=False, delta=10):
        pass

    def moveRelative(self, value, monitor=False, display=False, delta=10):
        pass

    def getPosition(self):
        pass

    def isMoving(self):
        pass
