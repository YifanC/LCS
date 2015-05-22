__author__ = 'matthias'

from base import *

class Feedtrough(Motor):
    def __init__(self):
        self.name = "linear motor"
        self.state = 0
        self.comport = "/dev/ttyUSB0"
        self.comBaudrate = 9600
        self.InfoInstruction = "1PR AL"
        self.InfoMsgLength = 800
        self.StandartMsgLength = 10
        self.comEnd = "\n"
