__author__ = 'matthias'

from base.motor import *


class Feedtrough(Motor):
    def __init__(self, name, axis=0):
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


        self.config_load()
        self.MICROSTEPS = None
        self.ACCELERATION = None
        self.DECELLERATION = None
        self.INITIALVELOCITY = None
        self.ENDVELOCITY = None
        self.HOLDCURRENT = None
        self.RUNCURRENT = None
        self.HOMINGVELOCITY = None
        self.MAX_HOMING_OVERSHOOT = None

        self.init()

    def init(self):
        self.axis = self.config.AXIS
        self.MICROSTEPS = self.config.MICROSTEPS
        self.ACCELERATION = self.config.ACCELERATION
        self.DECELLERATION = self.config.DECELLERATION
        self.INITIALVELOCITY = self.config.INITIALVELOCITY
        self.ENDVELOCITY = self.config.ENDVELOCITY
        self.HOLDCURRENT = self.config.HOLDCURRENT
        self.RUNCURRENT  = self.config.RUNCURRENT
        self.HOMINGVELOCITY = self.config.HOMINGVELOCITY
        self.MAX_HOMING_OVERSHOOT = self.config.MAX_HOMING_OVERSHOOT
        self.HOME_SWITCH = self.config.HOME_SWITCH
        self.HOME_DIRECTION = self.config.HOME_DIRECTION




    def com_write(self, msg):
        """ write message to, overwriting the base.com_write function adding the axis preamble """
        try:
            self.com.isOpen()
            self.printDebug(str(self.axis) + msg + self.comEnd)
            self.com.write(str(self.axis) + msg + self.comEnd)

        except:
            self.printError("Could not write message \"" + str(msg) + "\" to com port (" + str(self.com.portstr) + ")")

    def initAxis(self):
        self.setParameter("MS", self.MICROSTEPS)
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
            if self.color is True:
                self.printMsg(string + bcolors.OKGREEN + " -> OK" + bcolors.ENDC)
            else:
                self.printMsg(string + " -> OK")
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

        self.printMsg("Set " + self.HOME_SWITCH + " as home switch")
        set = self.setParameter(self.HOME_SWITCH, "1,1,0")  # set counterclockwise endswitch as home switch
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
        self.sendCommand("HM", self.HOME_DIRECTION)

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
