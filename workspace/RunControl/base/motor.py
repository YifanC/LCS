__author__ = 'matthias'
from device import *

class Motor(Device):
    """" At the moment only an idea of a nice classe """

    def __init__(self, name='', RunNumber=0):
        super(Motor, self).__init__(name=name, RunNumber=RunNumber, logit=True)
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
