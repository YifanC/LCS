# dummy RC Producer


from RCCommunication import *
import time
import sys

debug = 1

# Socket to talk to server
print("Encoder Producer: Connecting to server")

ctx = zmq.Context()
Producer = ctx.socket(zmq.REQ)
Producer.connect("ipc:///tmp/feed-laser.ipc")
Producer.setsockopt(zmq.LINGER, 0)


# 
poller = zmq.Poller()
poller.register(Producer, zmq.POLLIN)

data = LaserData()



# let's see if the data assembler is alive?
com = RCCommunication(1)
com.ID = ID_Encoder
com.sendEncoderData(Producer, data)

if poller.poll(100 * 1000):  # 100s timeout in milliseconds
    com.recvAck(Producer)
    if debug: print "DEBUG INFO: Data assembler alive"
else:
    raise IOError("Could not connect to data assembler")
    Producer.close()
    ctx.term()
    sys.exit(0)


# wait a bit so the RC initial data can be sent
time.sleep(5)


# Do 10 requests, waiting each time for a response
for request in range(1000, 1400):
    data.count_trigger = request

    com.sendEncoderData(Producer, data)
    ack = com.recvAck(Producer)
    print ack, request
    time.sleep(0.005)
