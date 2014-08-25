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
Assembler.recvData(server,data)
Assembler.sendAck(server)
print data.dump()
Assembler.recvData(server,data)
Assembler.sendAck(server)
print data.dump()



