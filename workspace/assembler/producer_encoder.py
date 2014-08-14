

# dummy RC client


from RCCommunication import *
import time



# Socket to talk to server
print("Encoder Producer: Connecting to server")


ctx = zmq.Context()
client = ctx.socket(zmq.REQ)
client.connect("ipc://kvmsg_selftest.ipc")

msg = RCCommunication(1)

# Do 10 requests, waiting each time for a response
for request in range(1):

	msg.ID = 2
	msg.ControlStep = 99
	msg.sendData(client)


	ack = msg.recvAck(client)
	print ack

 
