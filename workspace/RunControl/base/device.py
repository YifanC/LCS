__author__ = 'matthias'

from comserial import *

class Device(ComSerial):
    def __init__(self, name='', RunNumber=0, logit=True):
        super(Device, self).__init__(name=name, RunNumber=RunNumber, logit=logit)
        self.InstructionSet = {"getInfo": None,
                               "getName": None}

        self.comSetCommand = None  # this string is sent in between of a setParameter(para, value) call
        self.comGetCommand = None  # this string is sent in font of the getParameter(para) call


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
