from RCCommunication import *
import sys
import datetime as DT
debug = 1

# start server
ctx = zmq.Context()
server = ctx.socket(zmq.REP)
server.bind("ipc://kvmsg_selftest.ipc")

Assembler = RCCommunication()

data = LaserData()
print data.dump()

data = Assembler.recvData(server,data)
Assembler.sendAck(server)
print data.dump()
data = Assembler.recvData(server,data)
Assembler.sendAck(server)
print data.dump()



