import struct # for packing integers
import zmq
import sys


ID_RunControl = 1 
ID_Encoder = 2
debug = 1

# dict for the definitions
ID = {ID_RunControl:"Run Control Data:", ID_Encoder:"Encoder Data:"}

class RCCommunication(object):
    """

    """
    ID = 99

    def __init__(self, ID=99,ControlStep=0):
        assert isinstance(ID, int)
        
        self.ID = ID
        
    def sendEncoderData(self, socket, LaserData):
        """packs and sends data from the encoder"""
        ID_string = struct.pack('i', self.ID)
        Msg_string = struct.pack('f'*3, LaserData.pos_rot,LaserData.pos_lin,LaserData.count_trigger) 
        socket.send_multipart([ID_string, Msg_string])

    def sendRCData(self, socket, LaserData):
        """packs and sends data from run control"""
        ID_string = struct.pack('!i', self.ID)
        Msg_string = struct.pack('i'+'f'*8, LaserData.laserid,LaserData.pos_att,LaserData.pos_iris,LaserData.count_run,
                                 LaserData.count_run,LaserData.pos_tomg_1_axis1,LaserData.pos_tomg_1_axis2,LaserData.pos_tomg_2_axis1,LaserData.pos_tomg_2_axis2) 
        socket.send_multipart([ID_string, Msg_string])

    @classmethod
    def recvData(self,socket,Data):
        """receives data and according to the message ID unpacks the data. The function returns
        a LaserData object filled with this data."""
        
        # receive message and identify data
        ID_string, Message_String = socket.recv_multipart()
        
        print ID_string , Message_String


        ID = struct.unpack('i', ID_string)[0]
        

        if ID == ID_Encoder: # data from run control
            if debug: print "DEBUG: Received data from run control"
            Data.pos_rot,Data.pos_lin,Data.count_trigger = struct.unpack('f'*3,Message_String)    
        elif ID == ID_RunControl: #data from encoder
            if debug: print "DEBUG: Received data from encoder"
            (Data.laserid,Data.pos_att,Data.pos_iris,Data.count_run,Data.count_run,Data.pos_tomg_1_axis1,
            Data.pos_tomg_1_axis2,Data.pos_tomg_2_axis1,Data.pos_tomg_2_axis2) =  struct.unpack('i'+'f'*8,Message_String)
        else:
            print "Exiting: Message from unidentified client received (ID was " + str(ID) + ")"
            sys.exit()
        
        return ID

    @classmethod
    def sendAck(self, socket):
        socket.send("OK")
    @classmethod
    def recvAck(self, socket):
        com = socket.recv()
        if com == "OK":
            return com
        else:
            return "Fail"
        

class LaserData(object):
    def __init__(self,laserid=-1,pos_rot=-8888.,pos_lin=-9999.,pos_att=-8888.,pos_iris=-9999.,
        time=0,count_trigger=-1,count_run=-1,count_laser=-1,
        pos_tomg_1_axis1=-66666.,pos_tomg_1_axis2=-77777.,pos_tomg_2_axis1=-88888.,pos_tomg_2_axis2=-99999.):
        
        self.laserid = laserid          # which laser system: 1 or 2
        self.pos_rot = pos_rot          # Position Rotary Heidenhain Encoder 
        self.pos_lin = pos_lin          # Position Linear Heidenhain Encoder 
        self.pos_att = pos_att          # Position Attenuator Watt Pilot 
        self.pos_iris = pos_iris        # Position Iris Standa 
        self.time = time                # System Time of Laser Server 
        self.count_trigger = count_trigger          # Trigger Counter by Heidenhain Encoder
        self.count_run = count_run                  # Run Counter of step in calibration run 
        self.count_laser = count_laser              # Number of pulses shot with UV laser
        self.pos_tomg_1_axis1 = pos_tomg_1_axis1    # Motorized Mirror Zaber T-OMG at box, axis 1
        self.pos_tomg_1_axis2 = pos_tomg_1_axis2    # Motorized Mirror Zaber T-OMG at box, axis 2
        self.pos_tomg_2_axis1 = pos_tomg_2_axis1    # Motorized Mirror Zaber T-OMG at flange, axis 1
        self.pos_tomg_2_axis2 = pos_tomg_2_axis2    # Motorized Mirror Zaber T-OMG at flange, axis 2

    def __str__(self):
        '''display the data in a nice format'''
        mstr = """DATA INFO
 Record Time: {time}\n Laser ID: {laserid}\n Rotary Position: {pos_rot}\n Linear Position: {pos_lin}\n Attenuator Position: {pos_att}
 Iris Position: {pos_iris}\n Encoder Trigger Count: {count_trigger}\n Run Control Counter: {count_run}\n Laser Shot Count: {count_laser}
 Laser Box Mirror Position: x={pos_tomg_1_axis1} y={pos_tomg_1_axis2}\n Feedtrough Mirror Position: x={pos_tomg_2_axis1} y={pos_tomg_2_axis2}\n""".format(
                    time=self.time,
                    laserid=self.laserid,
                    pos_rot=self.pos_rot,
                    pos_lin=self.pos_lin,
                    pos_att=self.pos_att,
                    pos_iris=self.pos_iris,
                    count_trigger=self.count_trigger,
                    count_run=self.count_run,
                    count_laser=self.count_laser,
                    pos_tomg_1_axis1=self.pos_tomg_1_axis1,
                    pos_tomg_1_axis2=self.pos_tomg_1_axis2,
                    pos_tomg_2_axis1=self.pos_tomg_2_axis1,
                    pos_tomg_2_axis2=self.pos_tomg_2_axis2
                    )
        return mstr

    def dump(self):
        '''return the data from laser in a list'''
        return [    self.laserid,
                    self.pos_rot,
                    self.pos_lin,
                    self.pos_att,
                    self.pos_iris,
                    self.time,
                    self.count_trigger,
                    self.count_run,
                    self.count_laser,
                    self.pos_tomg_1_axis1,
                    self.pos_tomg_1_axis2,
                    self.pos_tomg_2_axis1,
                    self.pos_tomg_2_axis2 ]

    def dumptest(self):
        '''return the data from laser in a list'''
        return vars(self)['__dict__']
             
    def writeBinary(self,fileID):
        '''write the data to a binary file'''
        LaserdataList = self.dump()
        packed_data = struct.pack('i'+'f'*(len(LaserdataList)-1),*LaserdataList) # first an int then floats follow
        f = open(fileID, 'wb')
        f.write(packed_data)
        f.close()

