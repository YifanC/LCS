import struct # for packing integers

import zmq



class RCCommunication(object):
    """
    frame 0: message identifier
    frame 1: ControlStep 
    """
    ID = 99
    ControlStep = 0

    def __init__(self, ID=0,ControlStep=0):
        assert isinstance(ControlStep, int)
        assert isinstance(ID, int)
        
        self.ID = ID
        self.ControlStep = ControlStep

    def sendData(self, socket):
        """ 123"""
        ID_string = struct.pack('!i', self.ID)
        ControlStep_string = struct.pack('!l', self.ControlStep)
        socket.send_multipart([ID_string, ControlStep_string])

    @classmethod
    def recvData(cls, socket):
        """123"""
        ID_string, ControlStep_string = socket.recv_multipart()
        ID = struct.unpack('!i', ID_string)[0]
        ControlStep = struct.unpack('!l',ControlStep_string)[0]

        return cls(ID=ID,ControlStep=ControlStep)

    def sendAck(self, socket):
        socket.send("OK")

    def recvAck(self, socket):
        msg = socket.recv()
        if msg == "OK":
            return msg
        else:
            return "Fail"

