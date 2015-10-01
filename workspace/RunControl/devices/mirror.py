__author__ = 'matthias'

from base.motor import *
import struct
from devices.mirror_error import *
from math import copysign


class Mirror(Motor):
    def __init__(self, name, axis):
        self.name = name
        super(Mirror, self).__init__(name=self.name)

        self.axis = axis
        self.state = 0
        self.comBaudrate = 9600
        self.comTimeout = 1
        self.comEcho = False
        self.InfoInstruction = ""
        self.InfoMsgLength = 100
        self.StandartMsgLength = 15
        # config file

        self.config_load()

        self.InstructionSet = {"getName": 50,
                               "getParameter": 53,
                               "stopMovement": 23,
                               "home": 1,
                               "getStatus": 54,
                               "readRegister": 35,
                               "setRegister": 35,
                               "storePosition": 16,
                               "getPosition": 60,
                               "Reset": 0,
                               "FactoryReset": 36,
                               "Range": 44,
                               "HomeOffset": 47,
                               "Mode": 40,
                               "moveAbsolute": 20,  # no absolute movement implemented in hardware
                               "moveRelative": 21, }  # positive direction closes / negative opens

        """ The motor has backlash error, so approach position always from the same direction."""

        # Just a guess! TODO: Get the axis right
        self.dict_axis = {"horizontal": 1,
                          "vertical": 2}


    def com_send(self, command, instruction=0):
        if not self.comDryRun:
            self.com.flushInput()
        time.sleep(0.1)
        if instruction == 0:
            instruction = [0, 0, 0, 0]
        else:
            if len(instruction) != 4:
                self.printError("data frame has wrong size")

        self.printDebug("sending: " + str(command) + str(instruction))

        msg = struct.pack("<" + 6 * "B", self.axis, command, instruction[0], instruction[1], instruction[2],
                          instruction[3])
        reply = self.com_write(msg, echo=True)
        try:
            reply_trans = self.translate_reply(reply)[0]
        except IndexError:
            self.printError("Index error")
            reply_trans = 0
        self.printDebug(reply_trans)

        return reply_trans

    def translate_reply(self, reply):
        r = [0, 0, 0, 0, 0, 0]
        for i in range(6):
            r[i] = int(ord(reply[i]))

        if r[1] == 255:
            self.printError("device responded with error: " + str(r[2]))
            self.printError(ErrorCodeMirror.ErrorDict[r[2]])

        replyData = (int(256.0 ** 3.0) * r[5]) + (int(256.0 ** 2.0) * r[4]) + (int(256.0) * r[3]) + (r[2])
        if r[5] > 127:
            replyData -= int(256 ** 4.0)

        data = replyData
        msg_id = r[0]
        axis = r[1]
        self.printDebug("reply: " + str(r))
        self.printDebug("data: " + str(replyData))
        return data, axis, msg_id

    def getStatus(self):
        reply = self.com_send(self.InstructionSet["getStatus"])
        return reply


    def storePosition(self, adr=0):
        """ Store the current position, only possible when homing was performed beforehand """
        instruction = [adr, 0, 0, 0]
        self.com_send(self.InstructionSet["storePosition"], instruction)

    def home(self):
        self.printMsg("homing")
        self.com.timeout = 10
        reply = self.com_send(1)
        self.printDebug(reply)
        self.com.timeout = 1
        return 1

    def goto_default(self):
        self.home()
        time.sleept(5)
        default_position = self.config.DEFAULT_POSITION
        self.moveAbsolute(default_position)


    def setParameter(self, parameter, value=""):
        self.com_send(self.InstructionSet[parameter], value)

    def getParameter(self, parameter):
        reply = self.com_send(self.InstructionSet["getParameter"], [self.InstructionSet[parameter],0,0,0])
        return reply

    def setSerial(self, serial):
        address = 0
        self.writeRegister(address, serial)
        return 0

    def disableLED(self):
        setbits = pow(2, 14) + pow(2, 15)

        self.printMsg("Disableing LEDs")
        self.setParameter("Mode", self.translate_pos(setbits))
        return 0

    def getSerial(self):
        address = 0
        serial_number = self.readRegister(address)
        self.printMsg("serial number " + str(serial_number))
        if str(serial_number) != self.name[-3:]:
            self.printError("Name and serial do not match!")

        return serial_number

    def readRegister(self, register_adr):
        if 0 > register_adr > 127:
            self.printError("Address out of bounds")
            return -1
        instruction = [register_adr, 0, 0, 0]
        reply = self.com_send(self.InstructionSet["readRegister"], instruction)
        return int(reply) >> 8

    def writeRegister(self, register_adr, value):
        if 0 > register_adr > 127:
            self.printError("Address out of bounds")
            return -1

        if type(value) is not int:
            self.printError("value has wrong format")
            return -1

        if 255 < value < 0:
            self.printError("value is out of bounds")
            return -1

        instruction = [register_adr | 128, value, 0, 0]
        self.com_send(self.InstructionSet["readRegister"], instruction)
        return 0

    def moveAbsolute(self, value, monitor=False, display=False, delta=10):
        self.printDebug("moving absolute in ms:" + str(value))
        pos_list = self.translate_pos(int(value))
        self.com_send(self.InstructionSet["moveAbsolute"], pos_list)
        time.sleep(1)

    def moveRelative(self, value, monitor=False, display=False, delta=10):
        self.printDebug("moving relative in ms:" + str(value))
        pos_list = self.translate_pos(int(value))
        self.com_send(self.InstructionSet["moveRelative"], pos_list)

    def getPosition(self):
        pos = self.com_send(self.InstructionSet["getPosition"])
        self.printDebug("position: " + str(pos))
        return pos


    def translate_pos(self, Cmd_Data):
        if Cmd_Data < 0:
            Cmd_Data = 256 ** 4 + Cmd_Data

        Cmd_Byte_6 = Cmd_Data / 256 ** 3
        Cmd_Data = Cmd_Data - 256 ** 3 * Cmd_Byte_6
        Cmd_Byte_5 = Cmd_Data / 256 ** 2
        Cmd_Data = Cmd_Data - 256 ** 2 * Cmd_Byte_5
        Cmd_Byte_4 = Cmd_Data / 256
        Cmd_Data = Cmd_Data - 256 * Cmd_Byte_4
        Cmd_Byte_3 = Cmd_Data

        return [Cmd_Byte_3, Cmd_Byte_4, Cmd_Byte_5, Cmd_Byte_6]

    def isMoving(self):
        pass



