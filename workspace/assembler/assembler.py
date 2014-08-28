# 
# This code starts up an 0MQ Server which recieves
# data from the rotary encoder and the control script.
# The recieved data is then written to memory so the 
# SEB running can acces and send it to the assembler
#
# Recieved data:
#  - Encoder:	Trigger Number, Timestamp, Rotary Position,
#  				Linear Position
#  - Control:	Attenuator Position, Aperture Position

from RCCommunication import *
import sys
import datetime as DT
debug = 1

def ExitAssembler(ExitMessage):
	print "DATA ASSEMBLER INFO: " + ExitMessage
	Assembler.close()
	ctx.term()
	sys.exit(0)

# messaging definitions
ID_RunControl = 1 
ID_Encoder = 2

# start Assembler
ctx = zmq.Context()
Assembler = ctx.socket(zmq.REP)
Assembler.bind("ipc:///tmp/feed-laser.ipc")
Assembler.setsockopt(zmq.LINGER, 0)

# 
poller = zmq.Poller()
poller.register(Assembler, zmq.POLLIN)


RunControlAlive = False
EncoderAlive = False

com = RCCommunication()
data = LaserData()
# make shure clients are alive
if debug: print "DATA ASSEMBLER INFO: Waiting for clients to connect"
while(not(RunControlAlive and EncoderAlive)):
	
	# wait 10 seconds to establish connection to clients
	if poller.poll(30*1000):	

		msgID = com.recvData(Assembler,data)
		print msgID
		if msgID == ID_RunControl and RunControlAlive == False: 
			RunControlAlive = True
			if debug: print "DATA ASSEMBLER INFO: Run Control alive"
		if msgID == ID_Encoder and EncoderAlive == False: 
			EncoderAlive = True
			if debug: print "DATA ASSEMBLER INFO: Encoder alive"
	
		com.sendAck(Assembler)
	else:
		ExitAssembler("Could not connect to clients, stopping.")


print "DATA ASSEMBLER INFO: All clients alive"


# recieve initial data from run control
msgID = com.recvData(Assembler,data)

if msgID == ID_RunControl:
	print data.dump()
	com.sendAck(Assembler)
	if debug: 
		print "DATA ASSEMBLER INFO: Initial Run Control Data recieved"
else:
	com.sendAck(Assembler)
	ExitAssembler("no initial data from run control")


while True:
	try:
		msgID = com.recvData(Assembler,data)
		if msgID == ID_RunControl: 
			# recieve message from run control
			print "new RC data"
			print data.dump()

		if msgID == ID_Encoder:
			# recieve data from encoder
			
			data.writeBinary('workfile')
			print "new EC data"
			print data.dump()
			# write to file
			

		com.sendAck(Assembler)

	except KeyboardInterrupt:
		ExitAssembler("finished")






# check consistency

# write to file

# exit


