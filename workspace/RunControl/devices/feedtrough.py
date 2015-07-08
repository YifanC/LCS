__author__ = 'matthias'

from base.base import *


class Feedtrough(Motor):
    def __init__(self, name, axis):
        self.name = name
        super(Feedtrough, self).__init__(name=self.name)
        self.state = 0
        self.comport = "/dev/ttyUSB0"
        self.comBaudrate = 9600
        self.InfoInstruction = None
        self.InfoMsgLength = 800
        self.StandartMsgLength = 10
        self.comEnd = "\n"

        # TODO: Implement this class into new Motor class
        self.InstructionSet = {"getInfo": "PR ALL"}
        self.comInfoReplyLength = 800

        self.axis = axis
        self.homeSwitch = "S1"

        self.MICROSTEPS = 256

        if self.name.find("linear") >= 0:
            self.ACCELERATION = 50000
            self.DECELLERATION = 50000
            self.INITIALVELOCITY = 1000
            self.ENDVELOCITY = 15000
            self.HOLDCURRENT = 10
            self.RUNCURRENT = 80

            self.HOMINGVELOCITY = 8000
            self.MAX_HOMING_OVERSHOOT = -18000

        elif self.name.find("rotary") >= 0:
            self.ACCELERATION = 30000
            self.DECELLERATION = 30000
            self.INITIALVELOCITY = 1000
            self.ENDVELOCITY = 15000
            self.HOLDCURRENT = 5
            self.RUNCURRENT = 80

            self.HOMINGVELOCITY = 15000
            self.MAX_HOMING_OVERSHOOT = -18000

        else:
            self.printError("Could not recognize axis type, please provide manual configuration")
            self.ACCELERATION = None
            self.DECELLERATION = None
            self.INITIALVELOCITY = None
            self.ENDVELOCITY = None
            self.HOLDCURRENT = None
            self.RUNCURRENT = None

            self.HOMINGVELOCITY = None
            self.MAX_HOMING_OVERSHOOT = None

    def com_write(self, msg):
        """ write message to, overwriting the base.com_write function adding the axis preamble """
        try:
            self.com.isOpen()
            self.com.write(str(self.axis) + msg + self.comEnd)
        except:
            self.printError("Could not write message \"" + msg + "\" to com port (" + self.com.portstr + ")")

    def initAxis(self):
        self.setParameter("A", self.ACCELERATION)
        self.setParameter("D", self.DECELLERATION)
        self.setParameter("VI", self.INITIALVELOCITY)
        self.setParameter("VM", self.ENDVELOCITY)

        self.setParameter("HC", self.HOLDCURRENT)
        self.setParameter("RC", self.RUNCURRENT)

        # disable unused switches
        self.setParameter("S3", "0,0,0")
        self.setParameter("S4", "0,0,0")

        self.setLimitSwitches(0)


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

        self.printMsg("Set " + self.homeSwitch + " as home switch")
        set = self.setParameter(self.homeSwitch, "1,1,0")  # set counterclockwise endswitch as home switch
        time.sleep(0.5)
        # check if S1 was set correctly
        if set != 0:
            self.printMsg("home switch was not set correctly")
            self.setHomeSwitch(attempts + 1)

    def homeAxis(self):
        # check if manual homing was performed
        counts = int(self.getParameter("R1"))
        if counts < 1:
            self.printMsg("Manual homing was not performed, aporting homing procedure")
            return -1

        self.setParameter("VM", self.HOMINGVELOCITY)
        self.setHomeSwitch(0)
        self.printMsg("Homing")
        self.sendCommand("HM", 1)

        # monitor the movement and abort in the movement goes out too far from the known zero position
        self.printMsg("Position:")
        while self.isMoving():
            pos = int(self.getPosition())
            if pos < self.MAX_HOMING_OVERSHOOT:
                self.stopMovement()
                self.setParameter("R1", 0)
                self.setLimitSwitches(0)
                self.printError(" Position went to far over home switch, movement was stopped")
                return -1
            self.printMsg("\r" + str(pos), nonewline=True)
            time.sleep(0.1)

        self.setLimitSwitches(0)
        self.setParameter("VM", self.ENDVELOCITY)
        self.setParameter("R1", counts + 1)