__author__ = 'matthias'

from base.base import *
from base.device import *
from laser_error import ErrorCodeLaser


class Laser(Device):
    def __init__(self, RunNumber):
        self.name = "laser"
        super(Laser, self).__init__(name=self.name, logit=True, RunNumber=RunNumber)
        self.state = 0
        self.comBaudrate = 9600
        self.comTimeout = 2
        self.InfoInstruction = "SE"
        self.InfoMsgLength = 10
        self.StandartMsgLength = 10
        self.comDefaultReplyLength = 10
        self.comPrefix = ""
        self.comEnd = "\r"

        self.comGetCommand = ""
        self.comSetCommand = " "
        self.comReplyPrefix = "\r"

        self.comEcho = True

        self.InstructionSet = {"getStatus": "SE",
                               "getShots": "SC",
                               "start": "ST",
                               "stop": "ST 0",
                               "Shutter": "SH",
                               "openShutter_": "SH 1",
                               "closeShutter": "SH 0",
                               "singleShot": "SS",
                               "setPulseDivision": "PD"}


    """ The procedure for shooting the laser is the following:
    1. start laser (warm up period of 25min)
    2. check if status is ready
    3. set repetition rate (zero for single shot)
    4. open shutter
    (5. only in single shot mode: shoot)
    6. close shutter
    7. stop laser"""

    def getStatus(self):
        reply = self.getParameter("getStatus")
        if int(reply) != 0:
            self.printError(ErrorCodeLaser.ErrorDict[int(reply)])
        elif int(reply) == 0:
            self.printMsg("Laser status is good")
        else:
            self.printError("reply not understood --> exiting.")
            sys.exit(-1)

    def getShots(self):
        reply = self.getParameter("getShots")
        self.printMsg(reply)
        return int(reply)

    def start(self):
        self.setParameter("start", 1)
        # msg = self.InstructionSet["start"]
        # self.com_write(msg)

    def stop(self):
        msg = self.InstructionSet["stop"]
        self.com_write(msg)

    def openShutter(self):
        self.setParameter("Shutter", 1)

        # msg = self.InstructionSet["openShutter"]
        #self.com_write(msg)


    def closeShutter(self):
        self.setParameter("Shutter", 0)

    def singleShot(self):
        msg = self.InstructionSet["singleShot"]
        reply = self.com_write(msg)
        self.printMsg(reply)


    def setRate(self, rate):
        """ Set repetition rate in Hz of the laser. rate = 0 enters the single shot mode """
        if rate > 10:
            self.printError("Too high repetition rate")
            return -1
        if rate == 0:
            pulse_division = 0
        else:
            pulse_division = int(10 / rate)

        self.setParameter("setPulseDivision", format(pulse_division, "03"))


    def checkParameter(self, parameter, value, echo):
        expected_echo = self.InstructionSet[parameter] + self.comSetCommand + str(value)
        if echo.replace("\r", "") == expected_echo:
            return 0
        else:
            return -1

    def reply_filter(self, msg):
        return msg.replace("\r", "")
