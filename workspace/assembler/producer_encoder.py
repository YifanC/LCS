

# dummy RC client


from RCCommunication import *
import time

debug = 1

# Socket to talk to server
print("Encoder Producer: Connecting to server")


ctx = zmq.Context()
client = ctx.socket(zmq.REQ)
client.connect("ipc://kvmsg_selftest.ipc")
client.setsockopt(zmq.LINGER, 0)


# 
poller = zmq.Poller()
poller.register(client, zmq.POLLIN)


# let's see if the data assembler is alive?
msg = RCCommunication(1)
msg.ID = 2
msg.sendData(client)

if poller.poll(100*1000): # 100s timeout in milliseconds
	ack = msg.recvAck(client)
	if debug: print "DEBUG INFO: Data assembler alive"
else:
	raise IOError("Could not connect to data assembler")
	socket.close()
	context.term()
	sys.exit(0)


# wait a bit so the RC initial data can be sent
time.sleep(1)



# Do 10 requests, waiting each time for a response
for request in range(1000,1400):

	msg.ID = 2
	msg.ControlStep = request
	msg.sendData(client)
	ack = msg.recvAck(client)
	print ack, request
	time.sleep(0.005)


 
