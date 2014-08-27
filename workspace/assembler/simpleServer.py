from RCCommunication import *
import sys
import datetime as DT
debug = 1

# start server
ctx = zmq.Context()
server = ctx.socket(zmq.REP)
server.bind("ipc:///tmp/feed-laser.ipc")

Assembler = RCCommunication()

while True:
	data = LaserData()
	Assembler.recvData(server,data)
	Assembler.sendAck(server)
	print data.dump()



