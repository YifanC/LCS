__author__ = 'matthias'

from base.base import *

class Laser(MotorControl):
    def __init__(self):
        self.name = "laser"
        self.state = 0
        self.comport = "/dev/ttyUSB0"
        self.comBaudrate = 9600
        self.InfoInstruction = "SE"
        self.InfoMsgLength = 100
        self.StandartMsgLength = 10
        self.comEnd = "\r"
