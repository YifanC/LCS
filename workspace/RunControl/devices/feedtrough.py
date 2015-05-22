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
        self.homeSwitch = "S1"

        self.MICROSTEPS = 256

    def com_write(self, msg):
        """ write message to, overwriting the base.com_write function adding the axis preamble """
        try:
            self.com.isOpen()
            self.com.write(str(self.axis) + msg + self.comEnd)
        except:
            self.printError("Could not write message \"" + msg + "\" to com port (" + self.com.portstr + ")")

    def initAxis(self):
        pass

    def setParameter(self, parameter, value):
        msg = parameter + "=" + str(value)
        self.com_write(msg)
        SetValue = self.getParameter(parameter)

        string = "Set " + parameter + "=" + str(value)

        if SetValue.replace(" ", "") == str(value):
            self.printMsg(string + bcolors.OKGREEN + " -> OK" + bcolors.ENDC)
            return 0
        else:
            self.printError(string + " failed")
            return -1

    def getParameter(self, parameter):
        msg = "PR " + parameter
        self.com_write(msg)
        reply = self.com_recv()
        return reply[:-2]

    def sendCommand(self, command, attribute=""):
        msg = (command + " " + str(attribute))
        self.com_write(msg)

    def stopMovement(self):
        self.sendCommand("\x1B")
        move = self.getParameter("MV")
        self.printMsg("Stopping MV=" + str(move))

    def isMoving(self):
        # returns: 1 if axis is moving, 0 if not. And -1 for non-identifiable answer
        reply = self.getParameter("MV")
        if ('0' in reply):
            return 0
        elif ('1' in reply):
            return 1
        else:
            return -1

    def getPosition(self):
        reply = self.getParameter("P")
        return int(reply)

    def moveRelative(self, microsteps):
        self.printMsg("Moving " + str(microsteps) + " microsteps")
        self.sendCommand("MR", microsteps)

    def moveAbsolute(self, microsteps):
        self.printMsg("Moving to position: " + str(microsteps))
        self.sendCommand("MA", microsteps)


    def setLimitSwitches(self, attempts):
        if attempts == 4:
            self.printError("Setting the limit switch failed 5 times -> exiting")
            sys.exit(-1)

        self.printMsg("Set S1 and S2 as limit switches")
        set = self.setParameter("S1", "2,1,0")  # set counterclockwise endswitch as home switch
        set += self.setParameter("S2", "3,1,0")  # set counterclockwise endswitch as home switch
        time.sleep(0.5)
        # check if S2 was set correctly
        if set != 0:
            self.printMsg("limit switches were not set correctly")
            self.setLimitSwitches(attempts + 1)

    def setHomeSwitch(self, attempts):
        if attempts == 4:
            self.printError("Setting the home switch failed 5 times -> exiting")
            sys.exit(-1)

        self.printMsg("Set " + self.homeSwitch +" as home switch")
        set = self.setParameter(self.homeSwitch, "1,1,0")  # set counterclockwise endswitch as home switch
        time.sleep(0.5)
        # check if S1 was set correctly
        if set != 0:
            self.printMsg("home switch was not set correctly")
            self.setHomeSwitch(attempts + 1)