import zmq
import sys
import struct
from base.base import *

class communication(base):
    def __init__(self):
        self.name = name
        self.ID = {"Assembler": 0,
                   "RunControl": 1,
                   "Encoder": 2}
        self.id = -99
        self.state = 0


class server(communication):
    def __init__(self):
        ctx = zmq.Context()
        self.socket = ctx.socket(zmq.REP)
        self.socket.bind("ipc:///tmp/feed-laser.ipc")
        self.socket.setsockopt(zmq.LINGER, 0)
        self.poller = zmq.Poller()
        self.poller.register(self.socket, zmq.POLLIN)


    def startServer(self):
        # start up serve
        pass

    def stopServer(self):
        pass

    def recv_hello(self):
        reply = self.socket.recv()
        self.socket.send("OK")
        return reply

    def recv_data(self):
        pass

    def send_ack(self):
        pass


class client(communication):
    def __init__(self):
        ctx = zmq.Context()
        self.socket = ctx.socket(zmq.REQ)
        self.socket.bind("ipc:///tmp/feed-laser.ipc")
        self.socket.setsockopt(zmq.LINGER, 0)
        self.poller = zmq.Poller()
        self.poller.register(self.socket, zmq.POLLIN)

    def startClient(self):
        # start up server
        pass

    def stopClient(self):
        pass

    def send_hello(self):
        print self.socket.se
        self.socket.send("Hello World")
        reply = self.socket.recv()
        print reply

    def send_data(self):
        pass

    def recv_ack(self):
        pass


class RCCommunication(object):
    # dict for the definitions
    #ID = {ID_RunControl: "Run Control Data:", ID_Encoder: "Encoder Data:"}

    """

    """
    def __init__(self, ID=99, ControlStep=0):
        assert isinstance(ID, int)

        self.ID = -99
        self.Status = -99
        self.ID_RunControl = 1
        self.ID_Encoder = 2
        self.debug = 1

    def sendEncoderData(self, socket, LaserData):
        """packs and sends data from the encoder"""
        Info = struct.pack('i' * 2, self.ID, self.Status)

        Msg_string = struct.pack('l'*2 + 'f' * 3, LaserData.trigger_time_sec, LaserData.trigger_time_usec,
                                 LaserData.pos_rot, LaserData.pos_lin, LaserData.count_trigger)

        socket.send_multipart([Info, Msg_string])

    def sendRCData(self, socket, LaserData):
        """packs and sends data froi run control"""
        Info = struct.pack('i' * 2, self.ID, self.Status)

        Msg_string = struct.pack('i' + 'f' * 8, LaserData.laserid, LaserData.pos_att, LaserData.pos_iris,
                                 LaserData.count_run,
                                 LaserData.count_run, LaserData.pos_tomg_1_axis1, LaserData.pos_tomg_1_axis2,
                                 LaserData.pos_tomg_2_axis1, LaserData.pos_tomg_2_axis2)
        socket.send_multipart([Info, Msg_string])

    #@classmethod
    def recvData(self, socket, Data, ControlMSG=None):
        """receives data and according to the message ID unpacks the data. The function fills the supplied LaserData
        object with the received data. The return value is an identifier for the origin of the message. """

        # receive message and identify data
        Info, Message_String = socket.recv_multipart()

        ControlMSG.ID, ControlMSG.Status = struct.unpack('ii', Info)#[0]

        if ControlMSG.ID == self.ID_Encoder:  # data from run control
            if self.debug:
                print "DEBUG: Received data from encoder"
            Data.trigger_time_sec, Data.trigger_time_usec, Data.pos_rot, Data.pos_lin, Data.count_trigger = \
                struct.unpack('f'*2 + 'f' * 3, Message_String)

            # Something here is very strange: Serialization seems to be different from c and python. Floats and ints
            # work, but everythin else ends up crookend: So in some time we will have a problem here!
            Data.trigger_time_sec += 1431636031

        elif ControlMSG.ID == self.ID_RunControl:  # data from encoder
            if self.debug:
                print "DEBUG: Received data from run control"

            (Data.laserid, Data.pos_att, Data.pos_iris, Data.count_run, Data.count_run, Data.pos_tomg_1_axis1,
             Data.pos_tomg_1_axis2, Data.pos_tomg_2_axis1, Data.pos_tomg_2_axis2) = struct.unpack('i' + 'f' * 8,
                                                                                                  Message_String)
        else:
            print "Exiting: Message from unidentified client received (ID was " + str(ID) + ")"
            sys.exit()

        return ControlMSG.ID

    def sendAck(self, socket):
        socket.send("OK")

    def sendEnd(self, socket):
        socket.send("XX")

    def recvAck(self, socket):
        com = socket.recv()
        if com == "OK":
            return com
        else:
            return "Fail"
    def closeConnection(self, socket):
        pass