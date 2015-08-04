__author__ = 'matthias'
from base.base import *

class ComSerial(Base):
    def __init__(self, name=""):  # , comport):
        super(ComSerial, self).__init__(name=name)
        self.com = serial.Serial()
        self.comBaudrate = 9600
        self.comTimeout = 0.1
        self.comport = ""  # comport

        self.comEcho = False

        self.InfoInstruction = None
        self.InfoMsgLength = None
        self.StandardMsgLength = None

        self.comPrefix = ""
        self.comEnd = None
        self.NumberOfAxes = 1

        self.comDefaultReplyLength = None
        self.comInfoReplyLength = None

        self.comPrefix = ""  # this string is put in front of any message transmitted
        self.comSetPrefix = ""  # this string is sent in front of a setParameter(para, value) call
        self.comReplyPrefix = ""  # is used to determine the reply length when the device sends back an echo
        self.comReplyEnd = ""  # is used to determine the reply length when the device sends back an echo
        self.comEnd = ""  # is added to any message sent to the device



    def com_init(self):
        """ Tries to open com port specified in comport """
        try:
            port = self.comport
            self.com = serial.Serial(port, self.comBaudrate, 8, 'N', 1, timeout=0.1)

            # get rid of all the shit from a possible crash before
            self.com.flushInput()
            self.com.flushInput()
            time.sleep(1)
        except:
            self.printError("opening fcom port (" + str(self.comport) + ") failed --> quitting")
            sys.exit(1)

        self.printMsg("Com port (" + str(self.com.portstr) + ") opened")

    def com_close(self):
        """ closes com port """
        try:
            self.com.close()

        except:
            self.printError("Com port could not be closed")
            sys.exit(1)

        self.printMsg("Com port (" + self.com.port + ") closed")

    def com_write(self, message, echo=False):
        """ write message to comport """
        msg = self.comPrefix + message + self.comEnd
        self.com.isOpen()
        self.com.write(msg)

        if DEBUG is True:
            self.printDebug("String sent: " + msg + self.comEnd)

        if (self.comEcho is True) or (echo is True):
            reply = self.com_recv(len(self.comReplyPrefix + message + self.comReplyEnd))
            return reply

    def com_recv(self, msg_length=100):
        """ read message from comport """
        try:
            self.com.isOpen()
            msg = self.com.read(msg_length)
            if DEBUG is True:
                self.printDebug("Answer: " + msg)
        except:
            self.printError("Could not read to com port")
            sys.exit(1)
        return self.reply_filter(msg)

    def reply_filter(self, msg):
        """ function which filters the reply, should be defined in the device class if required. Useful for example if there
        is axis information which is sent in a reply."""
        return msg

    def msg_filter(self, msg):
        """ Should be over written if  """
        return self.comPrefix + msg + self.comEnd

    def printComStatus(self):
        self.printMsg(str(self.com))