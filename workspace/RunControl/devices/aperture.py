__author__ = 'matthias'

from base.motor import *


class Aperture(Motor):
    def __init__(self):
        self.name = "aperture"
        super(Aperture, self).__init__(name=self.name)
        self.state = 0
        self.comport = None
        self.comBaudrate = 9600
        self.comTimeout = 1
        self.comEcho = False
        self.InfoInstruction = ""
        self.InfoMsgLength = 100
        self.StandartMsgLength = 15
        self.comEnd = "\r"

        # config file
        self.config_load()

        self.InstructionSet = {"getInfo": None,
                               "getName": "c",
                               "enableMotor": "mn",
                               "disableMotor": "mf",
                               "startSpeed": "l",
                               "endSpeed": "v",
                               "acceleration": "a",
                               "decceleration": None,
                               "microsteps": "r",
                               "movementCurrent": None,
                               "idleCurrent": None,
                               "breakMovement": "ms",
                               "stopMovement": None,
                               "isMoving": "s",
                               "getPosition": "p",
                               "limit1": "1",
                               "limit2": "2",  # limit2 == 1 --> fully open
                               "moveAbsolute": None,  # no absolute movement implemented in hardware
                               "moveRelative": "x",
                               "startMovement": "go",
                               "setDirection": "h"}  # positive direction closes / negative opens
        # from

        self.comDefaultReplyLength = 100
        self.comInfoReplyLength = 100

        self.comPrefix = "A1"
        self.comSetPrefix = "s"
        self.comSetCommand = ""
        self.comGetCommand = "t"
        self.comReplyPrefix = "1:"
        self.comReplyEnd = "\r\n"
        self.comEnd = "\r"

        """ Operation of aperture:
             1. init com port
             2. turn motor on (enableMotor)
             3. home axis to either limit switch (default: fully closed)
             4. open the aperture to the desired value (moveRelative)
             5. turn motor off (disableMotor)
             6. close com port

             steps 3. and 4. are performed in the init function, where the opening position is loaded from the
             config_aperture.json file
        """
        """ Comments:
             - Limit switches: There are two limit switches at the fully open and fully closed positions
             - Homing: The two limit switches can be used for homing
             - Position range: The position from either home position to the other limit position is about 2200 (with
               microsteps regime '01')
             - Position reading (getPosition) is unreliable, can't be used as a reference. Best for now is to always
               home first and then move from there to a defined position in the opposite direction (no turing around)
        """


    def init(self):
        default_position = self.config.APERTURE_POSITION

        self.checkName()
        self.home(where=1)
        self.moveRelative(default_position, display=True)



    def checkName(self):
        """ Checks the name return by the device, if it is inconsistent with the expected name it stop the execute
        of the script """
        name = self.getName()
        if name[:11] == "8SMC3-RS232":
            self.printMsg("Aperture recognized")
            return 0
        else:
            self.printMsg("Aperture not recognized, is this the right port number? -> quitting")
            sys.exit(-1)

    def enableMotor(self):
        self.printMsg("Motor on")
        msg = self.InstructionSet["enableMotor"]
        return self.com_write(msg, echo=True)

    def disableMotor(self):
        self.printMsg("Motor off")
        msg = self.InstructionSet["disableMotor"]
        return self.com_write(msg, echo=True)

    def isMoving(self):
        """ This function returns True if the motor is moving and False if it is not"""
        reply = self.getParameter("isMoving")

        if int(reply[:2]) > 0:
            return True
        else:
            return False

    def moveRelative(self, value, monitor=False, display=False, delta=10):
        self.printMsg("Moving relative: " + str(value) + " steps")
        if display is True:
            monitor = True
        value = int(value)
        # need to determine the sign and then set the direction (negative = open (0), positive = close (1))
        if value >= 0:
            self.setParameter("setDirection", 1, echo=True)
        elif value < 0:
            self.setParameter("setDirection", 0, echo=True)
        else:
            self.printError("Could not determine sign of direction")

        # we have to put together an uppercase hex string of the form "0F1010"
        hexstr_value = '{0:06x}'.format(abs(value)).upper()

        # write the steps to the controller (no movement yet)
        self.setParameter("moveRelative", hexstr_value, echo=True)

        msg = self.InstructionSet["startMovement"]
        self.com_write(msg, echo=True)

        # get current position for monitoring
        if monitor is True:
            pos_start = self.getPosition()

        if monitor is True:
            return self.monitorPosition(value + pos_start, display, delta)
        return 0

    def home(self, where=2):
        """ Homes attenuator to either end switch: if 1 is the fully closed position and 2 is the fully open position"""
        direction = {1: "open", 2: "closed"}
        if where == 1:
            msg = "mr0"
            limit_switch = "limit1"
        elif where == 2:
            msg = "mr1"
            limit_switch = "limit2"
        else:
            self.printError("Homing parameter out of bounds")
            return -1

        # if a limit switch is active the homing will not be performed, we need to move such that the switch will
        # deactivate
        switch = self.checkLimits()
        if switch > 0:
            if switch == 1:
                self.moveRelative(-100, monitor=True)
            elif switch == 2:
                self.moveRelative(100, monitor=True)
	time.sleep(1)
        # now we issue the homing command
        self.com_write(msg, echo=True)
        self.printMsg("going to fully " + direction[where] + " position")

	limits = 0
        while limits == 0:
	    limits = int(self.checkLimits())
            #pos = self.getPosition()
            #self.printMsg("pos: " + str(pos))
	
	self.printMsg("reached fully " + direction[limits] + " position")

        return 0

    def checkLimits(self):
        """ checks if any limit switches are active, if no return is 0, otherwise it returns which one of the switches is
        active (1 or 2) as an int """
        limit1 = int(self.getParameter("limit1"))
        limit2 = int(self.getParameter("limit2"))
        limits = [limit1, limit2]

        # check if any on the switches is active
        if 1 in limits:
            return 1 + limits.index(1)

        return 0


    def reply_filter(self, msg):
        """ Removes the repl_prefix and trailing '\n' and '\r's from the reply """
        # TODO: Move this to base class, can be useful for every device!
        prefix_length = len(self.comReplyPrefix)
	if msg[:prefix_length] == self.comReplyPrefix:
            return msg[prefix_length:].rstrip()
        else:
            self.printError("Reply prefix (" + str(msg[:prefix_length]) + ") not valid, aborting")
            # sys.exit(-1)

    def checkParameter(self, parameter, value, echo):
        return 0

    def convertPosition(self, value):
        return int(value, 16)

