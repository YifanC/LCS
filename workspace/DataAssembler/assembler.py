# 
# This code starts up an 0MQ Server which recieves
# data from the rotary encoder and the control script.
# The recieved data is then written to memory so the 
# SEB running can acces and send it to the assembler
#
# Recieved data:
# - Encoder:	Trigger Number, Timestamp, Rotary Position,
# Linear Position
# - Control:	Attenuator Position, Aperture Position

from RCCommunication import *
import sys

debug = 1


def ExitAssembler(exitMessage):
    print "DATA ASSEMBLER INFO: " + exitMessage
    Assembler.close()
    ctx.term()
    sys.exit(0)

# messaging definitions
ID_RunControl = 1
ID_Encoder = 2

# start up server
ctx = zmq.Context()
Assembler = ctx.socket(zmq.REP)
Assembler.bind("ipc:///tmp/feed-laser.ipc")
Assembler.setsockopt(zmq.LINGER, 0)

# 
poller = zmq.Poller()
poller.register(Assembler, zmq.POLLIN)

RunControlAlive = False
EncoderAlive = False

# com = communication instance, data = Laser data class, info = Message Header
com = RCCommunication()

data = LaserData()
info = ControlMSG()


# make sure clients are alive
if debug: print "DATA ASSEMBLER INFO: Waiting for clients to connect"
while (not (RunControlAlive and EncoderAlive)):

    # wait 30 seconds to establish connection to clients
    if poller.poll(30 * 1000):

        msgID = com.recvData(Assembler, data, info)
        info.dump()

        if info.ID == ID_RunControl and RunControlAlive == False:
            RunControlAlive = True
            if debug: print "DATA ASSEMBLER INFO: Run Control alive"
        if info.ID == ID_Encoder and EncoderAlive == False:
            EncoderAlive = True
            if debug: print "DATA ASSEMBLER INFO: Encoder alive"

        com.sendAck(Assembler)
    else:
        ExitAssembler("Could not connect to clients, stopping.")

print "DATA ASSEMBLER INFO: All clients alive"


# recieve initial data from run control
msgID = com.recvData(Assembler, data, info)

if info.ID == ID_RunControl:
    print data.dump()
    com.sendAck(Assembler)
    if debug:
        print "DATA ASSEMBLER INFO: Initial Run Control Data recieved"
else:
    com.sendAck(Assembler)
    ExitAssembler("no initial data from run control")


while True:
    try:
        com.recvData(Assembler, data, info)
        if info.ID == ID_RunControl:
            # recieve message from run control
            print "new RC data", info.Status

        if info.ID == ID_Encoder:
            # recieve data from encoder

            data.writeBinary('workfile.dat')
            print "new EC data", info.Status
            print str(data)
        # write to file
        com.sendAck(Assembler)

        if data.count_trigger == 1000:
            com.sendEnd(Assembler)
            break
        else:
            pass
    except KeyboardInterrupt:
        com.recvData(Assembler, data, info)
        com.sendEnd(Assembler)
        ExitAssembler("finished")






# check consistency

# write to file

# exit


