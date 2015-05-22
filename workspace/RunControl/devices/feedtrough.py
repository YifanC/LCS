__author__ = 'matthias'

from base.base import *

class Feedtrough(MotorControl):
    def __init__(self, name, axis):
        self.name = name
        self.state = 0
        self.comport = "/dev/ttyUSB0"
        self.comBaudrate = 9600
        self.InfoInstruction = "PR AL"
        self.InfoMsgLength = 800
        self.StandartMsgLength = 10
        self.comEnd = "\n"

        self.axis = axis

    def com_write(self, msg):
        """ write message to, overwriting the base.com_write function adding the axis preamble """
        try:
            self.com.isOpen()
            self.com.write(str(self.axis) + msg + self.comEnd)
        except:
            self.printError("Could not write message \"" + msg + "\" to com port (" + self.com.portstr + ")")


    def setParameter(self, parameter, value):
        msg = parameter + "=" + str(value)
        self.com_write(msg)
        SetValue = self.getParameter(parameter)
        print "now"
        time.sleep(4)

        string = "Set " + parameter + "=" + str(value)

        if SetValue.replace(" ", "") == str(value):
            self.printMsg(string + bcolors.OKGREEN + " -> OK" + bcolors.ENDC)
        else:
            print SetValue
            self.printError(string + " failed")
            return -1

    def getParameter(self, parameter):
        msg = "PR " + parameter
        self.com_write(msg)
        reply = self.com_recv()
        return reply