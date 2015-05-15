import struct  # for packing integers
import zmq
import sys





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

class ControlMSG(object):
    ''' Control MSG dict:
                            -1 : Server started (initial message)
                            '''
    def __init__(self, ID=-1, Status=-1):
        self.ID = ID
        self.Status = Status
        pass

    def dump(self):
        print "Control Message: ID=", self.ID, " Status=", self.Status


class LaserData(object):

    ID_RunControl = 1
    ID_Encoder = 2
    debug = 1

    # dict for the definitions
    ID = {ID_RunControl: "Run Control Data:", ID_Encoder: "Encoder Data:"}

    def __init__(self, laserid=-1, trigger_time_sec=-999999, trigger_time_usec=-999999, pos_rot=-8888., pos_lin=-9999., pos_att=-8888.,
                 pos_iris=-9999.,
                 time=0, count_trigger=-1, count_run=-1, count_laser=-1,
                 pos_tomg_1_axis1=-66666., pos_tomg_1_axis2=-77777., pos_tomg_2_axis1=-88888.,
                 pos_tomg_2_axis2=-99999.):
        self.laserid = laserid  # which laser system: 1 or 2
        self.pos_rot = pos_rot  # Position Rotary Heidenhain Encoder
        self.pos_lin = pos_lin  # Position Linear Heidenhain Encoder
        self.pos_att = pos_att  # Position Attenuator Watt Pilot
        self.pos_iris = pos_iris  # Position Iris Standa
        self.trigger_time_sec = trigger_time_sec    # Epoch time (in seconds) of Laser Server at receive of Encoder data
        self.trigger_time_usec = trigger_time_usec  # Fraction to add to Epoch time in microseconds
        self.count_trigger = count_trigger  # Trigger Counter by Heidenhain Encoder
        self.count_run = count_run  # Run Counter of step in calibration run
        self.count_laser = count_laser  # Number of pulses shot with UV laser
        self.pos_tomg_1_axis1 = pos_tomg_1_axis1  # Motorized Mirror Zaber T-OMG at box, axis 1
        self.pos_tomg_1_axis2 = pos_tomg_1_axis2  # Motorized Mirror Zaber T-OMG at box, axis 2
        self.pos_tomg_2_axis1 = pos_tomg_2_axis1  # Motorized Mirror Zaber T-OMG at flange, axis 1
        self.pos_tomg_2_axis2 = pos_tomg_2_axis2  # Motorized Mirror Zaber T-OMG at flange, axis 2

    def __str__(self):
        '''display the data in a readable format'''
        mstr = """DATA INFO
 Laser Trigger Time: {time_sec}:{time_usec}\n Laser ID: {laserid}\n Encoder Trigger Count: {count_trigger}\n Rotary Position: {pos_rot} Linear Position: {pos_lin}\n Attenuator Position: {pos_att} \n Iris Position: {pos_iris}\n Run Control Counter: {count_run}\n Laser Shot Count: {count_laser}
 Laser Box Mirror Position: x={pos_tomg_1_axis1} y={pos_tomg_1_axis2}\n Feedtrough Mirror Position: x={pos_tomg_2_axis1} y={pos_tomg_2_axis2}\n""".format(
            time_sec=self.trigger_time_sec,
            time_usec=self.trigger_time_usec,
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
        '''return the data from laser in a list, order must be according to write_laser_struct.C '''
        return [self.laserid,
                self.pos_rot,
                self.pos_lin,
                self.pos_att,
                self.pos_iris,
                self.trigger_time_sec, # add time in usec here when its defined!
                self.count_trigger,
                self.count_run,
                self.count_laser,
                self.pos_tomg_1_axis1,
                self.pos_tomg_1_axis2,
                self.pos_tomg_2_axis1,
                self.pos_tomg_2_axis2]

    def writeBinary(self, fileID):
        '''write the data to a binary file'''
        LaserdataList = self.dump()
        packed_data = struct.pack('i' + 'f' * (len(LaserdataList) - 1),
                                  *LaserdataList)  # first an int then floats follow
        f = open(fileID, 'ab')
        f.write(packed_data)
        f.close()

