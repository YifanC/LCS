

# dummy RC client


from RCCommunication import *
import time
import sys

debug = 1

# configure / start up the client
print("RC Producer: Connecting to server")

ctx = zmq.Context()
client = ctx.socket(zmq.REQ)
client.connect("ipc://kvmsg_selftest.ipc")
client.setsockopt(zmq.LINGER, 0)

poller = zmq.Poller()
poller.register(client, zmq.POLLIN)

# let's see if the data assembler is alive?
msg = RCCommunication(1)
msg.ID = 1
msg.sendData(client)

if poller.poll(100*1000): # 100s timeout in milliseconds
	ack = msg.recvAck(client)
	if debug: print "DEBUG INFO: Data assembler alive"
else:
	raise IOError("Could not connect to data assembler")
	client.close()
	ctx.term()
	sys.exit(0)


# send initial data
msg.ID = 1
msg.ControlStep = 99
msg.sendData(client)
ack = msg.recvAck(client)
print ack
time.sleep(1)

# send some data
for request in range(10):

	msg.ID = 1
	msg.ControlStep = request
	msg.sendData(client)
	ack = msg.recvAck(client)
	print ack
	time.sleep(0.2)
