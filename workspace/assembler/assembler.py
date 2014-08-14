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


debug = 1

import datetime as DT
import time


# timing definitions
StartTime = DT.datetime.now()
if debug == 1: print StartTime

MaxIdleTime = 0.1 # minutes
MaxIdleTime = DT.timedelta(seconds=MaxIdleTime*60)

# messaging definitions
IDrunControlMsg = 0
IDrunControlData = 1 
IDencoderData = 2

# start server
ctx = zmq.Context()
server = ctx.socket(zmq.REP)
server.bind("ipc://kvmsg_selftest.ipc")


reply = RCCommunication(0)




RunControlAlive = False
EncoderAlive = False
# make shure both clients are alive
if debug: print "DEBUG INFO: waiting for clients to connect"
while(not(RunControlAlive and EncoderAlive)):
	
	# wait for message from clients 
	msg = RCCommunication.recvData(server)

	if msg.ID == 1: 
		RunControlAlive = True
		if debug: print "DEBUG INFO: Run Control alive"
	if msg.ID == 2: 
		EncoderAlive = True
		if debug: print "DEBUG INFO: Encoder alive"
	
	msg.sendAck(server)











# recieve data from run control

# recieve data from encoder

# recieve message from run control

# check consistency

# write to file

# exit
