from RCCommunication import *
import sys
import datetime as DT



debug = 1

# start server
ctx = zmq.Context()
Producer = ctx.socket(zmq.REQ)
Producer.connect("ipc://kvmsg_selftest.ipc")
Producer.setsockopt(zmq.LINGER, 0)

com = RCCommunication()
com.ID = 1

data = LaserData()
print data.dump()

if com.ID == 1:

    data.pos_rot = 11.0
    data.pos_lin = 12.0
    data.count_trigger = 100000
    com.sendEncoderData(Producer,data)
    com.recvAck(Producer)

com.ID = 2

if com.ID == 2:
    data.laserid = 9999999
    data.pos_att = 5.43210
    com.sendRCData(Producer, data)
    com.recvAck(Producer)

