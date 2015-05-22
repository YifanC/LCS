__author__ = 'matthias'

from base.base import *

class Attenuator(MotorControl):
    def __init__(self):
        self.name = "attenuator"
        self.state = 0
        self.comport = "/dev/pts/18"
        self.comBaudrate = 38400
        self.InfoInstruction = "p"
        self.InfoMsgLength = 100
        self.StandartMsgLength = 10
        self.comEnd = "\r"
