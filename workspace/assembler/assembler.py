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

# messaging definitions
IDrunControlData = 1 
IDencoderData = 2

# start server
ctx = zmq.Context()
server = ctx.socket(zmq.REP)
server.bind("ipc://kvmsg_selftest.ipc")
server.setsockopt(zmq.LINGER, 0)

# 
poller = zmq.Poller()
poller.register(server, zmq.POLLIN)


RunControlAlive = False
EncoderAlive = False

# make shure clients are alive
if debug: print "DATA ASSEMBLER INFO: Waiting for clients to connect"
while(not(RunControlAlive and EncoderAlive)):
	
	# wait 10 seconds to establish connection to clients
	if poller.poll(30*1000):	

		msg = RCCommunication.recvData(server)

		if msg.ID == IDrunControlData and RunControlAlive == False: 
			RunControlAlive = True
			if debug: print "DATA ASSEMBLER INFO: Run Control alive"
		if msg.ID == IDencoderData and EncoderAlive == False: 
			EncoderAlive = True
			if debug: print "DATA ASSEMBLER INFO: Encoder alive"
	
		msg.sendAck(server)
	else:
		exit("Could connect to clients, stopping.")


print "DATA ASSEMBLER INFO: All clients alive"


# recieve initial data from run control
msg = msg.recvData(server)

if msg.ID == IDrunControlData:
	RCData = msg.ControlStep
	msg.sendAck(server)
	if debug: 
		print "DATA ASSEMBLER INFO: Initial Run Control Data recieved"
else:
	msg.sendAck(server)
	exit("no initial data from run control")


f = open('workfile', 'w')

while True:
	try:
		msgData = RCCommunication.recvData(server)
		if msgData.ID == IDrunControlData: 
			# recieve message from run control
			RCData = msgData.ControlStep
			print "RC data"

		if msgData.ID == IDencoderData:
			# recieve data from encoder
			print "EC data"
			# write to file
			ECData = msgData.ControlStep

			dataline = str(DT.datetime.now()) + " RC: "+ str(RCData) +", EC:"+ str(ECData) + "\n"
			f.write(dataline)
		msgData.sendAck(server)

	except KeyboardInterrupt:
		f.close()
		exit("finished")






# check consistency

# write to file

# exit
 


def exit(ExitMessage):
 	print "DATA ASSEMBLER INFO: " + ExitMessage
 	server.close()
	ctx.term()
	sys.exit(0)