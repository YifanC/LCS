import struct  # for packing integers
import time
import os


class LaserData(object):
    ID_RunControl = 1
    ID_Encoder = 2
    debug = 1

    # dict for the definitions
    ID = {ID_RunControl: "Run Control Data:", ID_Encoder: "Encoder Data:"}

    def __init__(self,
                 laserid=-int(1),
                 status=float(-1),
                 trigger_time_sec=float(-999999.),
                 trigger_time_usec=float(-999999.),
                 pos_rot=float(-8888.),
                 pos_lin=float(-9999.),
                 pos_att=float(-8888.),
                 pos_iris=float(-9999.),
                 count_trigger=float(-1.),
                 count_run=float(-1.),
                 count_laser=float(-1.),
                 pos_tomg_1_axis1=float(-66666.),
                 pos_tomg_1_axis2=float(-77777.),
                 pos_tomg_2_axis1=float(-88888.),
                 pos_tomg_2_axis2=float(-99999.),
                 RunNumber=-1):
        self.laserid = laserid  # which laser system: 1 or 2
        self.status = status
        self.pos_rot = pos_rot  # Position Rotary Heidenhain Encoder
        self.pos_lin = pos_lin  # Position Linear Heidenhain Encoder
        self.pos_att = pos_att  # Position Attenuator Watt Pilot
        self.pos_iris = pos_iris  # Position Iris Standa
        self.trigger_time_sec = trigger_time_sec  # Epoch time (in seconds) of Laser Server at receive of Encoder data
        self.trigger_time_usec = trigger_time_usec  # Fraction to add to Epoch time in microseconds
        self.count_trigger = count_trigger  # Trigger Counter by Heidenhain Encoder
        self.count_run = count_run  # Run Counter of step in calibration run
        self.count_laser = count_laser  # Number of pulses shot with UV laser
        self.pos_tomg_1_axis1 = pos_tomg_1_axis1  # Motorized Mirror Zaber T-OMG at box, axis 1
        self.pos_tomg_1_axis2 = pos_tomg_1_axis2  # Motorized Mirror Zaber T-OMG at box, axis 2
        self.pos_tomg_2_axis1 = pos_tomg_2_axis1  # Motorized Mirror Zaber T-OMG at flange, axis 1
        self.pos_tomg_2_axis2 = pos_tomg_2_axis2  # Motorized Mirror Zaber T-OMG at flange, axis 2

        self.path_data = os.getenv("LCS_DATA")
        self.path_binary = os.getenv("LCS_BINARYFILE")
        self.RunNumber = int(RunNumber)

    def __str__(self):
        '''display the data in a readable format'''
        mstr = """DATA INFO
 Laser ID: {laserid}\n Status: {status}\n Laser Trigger Time: {time_sec}:{time_usec}\n Encoder Trigger Count: {count_trigger}\n Rotary Position: {pos_rot} Linear Position: {pos_lin}\n Attenuator Position: {pos_att} \n Iris Position: {pos_iris}\n Run Control Counter: {count_run}\n Laser Shot Count: {count_laser}
 Laser Box Mirror Position: x={pos_tomg_1_axis1} y={pos_tomg_1_axis2}\n Feedtrough Mirror Position: x={pos_tomg_2_axis1} y={pos_tomg_2_axis2}\n""".format(
            time_sec=self.trigger_time_sec,
            time_usec=self.trigger_time_usec,
            laserid=self.laserid,
            status=self.status,
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
                self.status,
                self.pos_rot,
                self.pos_lin,
                self.pos_att,
                self.pos_iris,
                self.trigger_time_sec,
                self.trigger_time_usec,
                self.count_trigger,
                self.count_run,
                self.count_laser,
                self.pos_tomg_1_axis1,
                self.pos_tomg_1_axis2,
                self.pos_tomg_2_axis1,
                self.pos_tomg_2_axis2]

    def fill(self, data_list):
        """ Fill the data class according to the supplied list """
        self.laserid = data_list[0]
        self.status = data_list[1]
        self.pos_rot = data_list[2]
        self.pos_lin = data_list[3]
        self.pos_att = data_list[4]
        self.pos_iris = data_list[5]
        self.trigger_time_sec = data_list[6]
        self.trigger_time_usec = data_list[7]
        self.count_trigger = data_list[8]
        self.count_run = data_list[9]
        self.count_laser = data_list[10]
        self.pos_tomg_1_axis1 = data_list[11]
        self.pos_tomg_1_axis2 = data_list[12]
        self.pos_tomg_2_axis1 = data_list[13]
        self.pos_tomg_2_axis2 = data_list[14]

    def dump_time(self):
        pass

    def writeTxt(self):
        with open(self.path_data + "/" + "Run-" + str(self.RunNumber) + ".txt", "a+") as f:
            for item in self.dump():
                f.write("%s " % item)
            f.write("\n")
        f.close()

    def writeBinary(self, filename):
        ''' write the data to a binary file '''

        file_binary = self.path_binary + "laser.bin"
        f = open(file_binary, 'wt')
        f.write(self.pack())
        f.close()

    def pack(self):
        """ serialization routine to write data to binary file or send over tcp """
        LaserdataList = self.dump()
        serialized_data = struct.pack('i' + 'f' * (len(LaserdataList) - 1), *LaserdataList)
        return serialized_data

    def unpack(self, packed_data):
        """ de-serialization routine to read data from binary file or receive  over tcp """

        LaserdataList = self.dump()
        unpacked_data = struct.unpack('i' + 'f' * (len(LaserdataList) - 1), packed_data)
        return unpacked_data
