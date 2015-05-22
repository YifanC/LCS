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

class Motor(Device):
    def __init__(self, comport):
        self.com = serial.Serial()
        self.comport = comport

    def device_type(self):
        pass

    def init_com(self):
        """ Tries to open com port specified in comport """
        try:
            port = "/dev/pts/" + str(self.comport)
            self.com = serial.Serial(port, 9600, 8, 'N', 1, timeout=0.1)
        except:
            self.printMsg("opening fcom port (" + port + ") failed --> quitting")
            sys.exit(1)

        self.printMsg("Com port (" + self.com.portstr + ") opened")

    def close_com(self):
        """ closes com port """
        try:
            self.com.close()

        except:
            self.printMsg("Com port could not be closed")
            sys.exit(1)

        self.printMsg("Com port (" + self.com.port + ") closed")

    def com_write(self, msg):
        """ write message to comport """
        try:
            self.com.isOpen()
            self.com.write(msg)
        except:
            self.printMsg("Could not write to com port")
            sys.exit(1)

    def com_recv(self,msg_length):
        """ read message to comport """
        try:
            self.com.isOpen()
            msg = self.com.read(msg_length)
        except:
            self.printMsg("Could not read to com port")
            sys.exit(1)
        return msg