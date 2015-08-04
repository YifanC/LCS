__author__ = 'matthias'

from base.base import *


class Laser(Device):
    def __init__(self):
        self.name = "laser"
        super(Laser, self).__init__(name=self.name)
        self.state = 0
        self.comport = "/dev/ttyUSB2"
        self.comBaudrate = 9600
        self.comTimeout = 2
        self.InfoInstruction = "SE"
        self.InfoMsgLength = 10
        self.StandartMsgLength = 10
        self.comPrefix = ""
        self.comEnd = "\r"

	self.comReplyPrefix = "\r"

	self.comEcho = True

        self.InstructionSet = {"getStatus": "SE",
                               "getShots": "SC",
                               "start": "ST 1",
                               "stop": "ST 0",
                               "openShutter": "SH 1",
                               "closeShutter": "SH 0",
                               "singleShot": "SS",
                               "setRate": "PD"}


    """ The procedure for shooting the laser is the following:
    1. start laser (warm up period of 25min)
    2. check if status is ready
    3. set repetition rate
    4. open shutter
    (5. only in single shot mode: shoot)
    6. close shutter
    7. stop laser"""

    def getStatus(self):
        msg = self.InstructionSet["getStatus"]ope
        self.com_write(msg)

        reply = self.com_recv(self.InfoMsgLength)
        self.printMsg(reply)
	if int(reply) != 0:
		self.printError("Laser error")
        # TODO: Implement return message disply

    def getShots(self):
        msg = self.InstructionSet["getShots"]
        self.com_write(msg)

        reply = self.com_recv(self.StandartMsgLength)
        self.printMsg(reply)
	return int(reply)

    def start(self):
        msg = self.InstructionSet["start"]
        self.com_write(msg)

    def stop(self):
        msg = self.InstructionSet["stop"]
        self.com_write(msg)

    def openShutter(self):las
        msg = self.InstructionSet["openShutter"]
        self.com_write(msg)

    def closeShutter(self):
        msg = self.InstructionSet["closeShutter"]
        self.com_write(msg)

    def singleShot(self):
        msg = self.InstructionSet["singleShot"]
        self.com_write(msg)

    def setRate(self, rate):
        """ Set repetition rate in Hz of the laser. rate = 0 enters the single shot mode """
        if rate > 10:
            self.printError("Too high repetition rate")
            return -1

        msg = self.InstructionSet["setRate"] + " " + str(rate)
        self.com_write(msg)
