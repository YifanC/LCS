__author__ = 'matthias'
from comserial import *

class Motor(ComSerial):
    """" At the moment only an idea of a nice classe """

    def __init__(self, name=''):
        super(Motor, self).__init__(name=name)
        self.InstructionSet = {"getInfo": None,
                               "getName": None,
                               "maxSpeed": None,
                               "startSpeed": None,
                               "acceleration": None,
                               "deceleration": None,
                               "movementCurrent": None,
                               "idleCurrent": None,
                               "breakMovement": None,
                               "stopMovement": None,
                               "isMoving": None,
                               "getPosition": None,
                               "moveAbsolute": None,
                               "moveRelative": None}

        self.comDefaultReplyLength = None
        self.comInfoReplyLength = None

        self.comPrefix = ""  # this string is put in front of any message transmitted
        self.comSetPrefix = ""  # this string is sent in front of a setParameter(para, value) call
        self.comSetCommand = None  # this string is sent in between of a setParameter(para, value) call
        self.comGetCommand = None  # this string is sent in font of the getParameter(para) call
        self.comReplyPrefix = ""  # is used to determine the reply length when the device sends back an echo
        self.comReplyEnd = ""  # is used to determine the reply length when the device sends back an echo
        self.comEnd = ""  # is added to any message sent to the device

    def getName(self, display=True):
        name = self.getParameter("getName")
        if display == True:
            self.printMsg("Name: " + str(name))

        return str(name)


    def getInfo(self, display=False):
        if self.InstructionSet['getInfo'] != None and self.comInfoReplyLength != None:
            self.com_write(self.InstructionSet["getInfo"])
            reply = self.com_recv(self.comInfoReplyLength)
            if display == True:
                self.printMsg("Info: " + reply)

            return reply

        else:
            self.printError("Info instruction or answer length is not defined")

    def getParameter(self, parameter):
        msg = self.comGetCommand + self.InstructionSet[parameter]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
        if reply == "":
            self.printError("No reply received")

        return reply

    def getPosition(self):
        msg = self.comGetCommand + self.InstructionSet["getPosition"]
        self.com_write(msg)
        reply = self.com_recv(self.comDefaultReplyLength)
        return self.convertPosition(reply)

    def monitorPosition(self, endPosition, display=True, delta=10):
        time.sleep(0.1)
        pos = self.getPosition()
        while self.isMoving():
            if display is True:
                self.printMsg("Position: " + str(pos))
                pos = self.getPosition()
                time.sleep(0.1)

        pos = self.getPosition()
        if display is True:
            self.printMsg("Position: " + str(pos))
        # check if position was reached
        if (endPosition - delta) <= pos <= (endPosition + delta):
            return 0
        else:
            return -1

    def setParameter(self, parameter, value, check=True, echo=False, attempts=1):
        """ Function sends the defined string + value to the device.
            Arguments:  check:      If true calls the function checkParameter which should be defined by the device class
                        echo:       The set command expects an echo from the device, confirming the transmission, this
                                    is not confirming that tha value was set.
                        attempts:   Number of tries for setting the parameter before giving up """

        string = "Set " + parameter + "=" + str(value)
        if attempts > 0:
            msg = self.comSetPrefix + self.InstructionSet[parameter] + self.comSetCommand + str(value)
            reply = self.com_write(msg, echo=echo)

            if check is True:
                if self.checkParameter(parameter, value, reply) == 0:
                    # self.printMsg(string + bcolors.OKGREEN + " -> OK" + bcolors.ENDC, True)
                    self.printMsg(string + " -> OK", False)
                    return 0
                else:  # retry
                    self.printError(string + " failed --> trying again")
                    self.setParameter(parameter, value, check=check, echo=echo, attempts=attempts - 1)
            else:
                return 0
        else:
            self.printError(string + " faild too many time --> quitting")

    def checkParameter(self, parameter, value, echo):
        """ This function has to be defined by the device class to specify how a setParameter() call will check that the
        parameter was configured properly. """
        self.printError("No checkParameter function implemented! --> exiting")
        self.com.flushInput()
        self.com.flushOutput()
        sys.stdout.flush()
        sys.exit(-1)

    def convertPosition(self, value):
        """  If the read values from the getPosition command is not sent back in a standard format, this function can be
         defined to convert the values into a readable format for latter use."""
        return value


    def stopMovement(self):
        msg = self.InstructionSet["stopMovement"]
        self.com_write(msg)

    def moveRelative(self, value, monitor=False, display=False, delta=10):
        self.printMsg("Moving relative: " + str(value) + " steps")
        if display is True:
            monitor = True

        value = int(value)
        msg = self.InstructionSet["moveRelative"] + self.comSetCommand + str(value)

        # get current position for monitoring
        if monitor is True:
            pos_start = self.getPosition()

        self.com_write(msg)
        if monitor is True:
            return self.monitorPosition(value + pos_start, display, delta)
        return 0

    def moveAbsolute(self, value, monitor=False, display=False, delta=10):
        """ Moving motor to an absolute position, the movement can be monitored if an getPosition and a isMoving function
        is supplied. """
        self.printMsg("Moving absolute to: " + str(value) + " steps")
        if display is True:
            monitor = True

        value = int(value)
        msg = self.InstructionSet["moveAbsolute"] + self.comSetCommand + str(value)
        self.com_write(msg)

        if monitor is True:
            return self.monitorPosition(value, display, delta)
        return 0
