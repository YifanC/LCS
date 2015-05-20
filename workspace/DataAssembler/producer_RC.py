# dummy RC Producer


from RCCommunication import *
import time
import sys
import signal
import subprocess as sub


def CloseConnection():
    print "Shutting down zmq poller"
    Producer.close()
    ctx.term()





debug = 1
ID_RunControl = 1
# configure / start up the Producer
print("RC Producer: Connecting to server")

ctx = zmq.Context()
Producer = ctx.socket(zmq.REQ)
Producer.connect("ipc:///tmp/feed-laser.ipc")
Producer.setsockopt(zmq.LINGER, 0)

poller = zmq.Poller()
poller.register(Producer, zmq.POLLIN)

data = LaserData()

# let's see if the data assembler is alive?
com = RCCommunication(1)
com.ID = ID_RunControl
com.Status = -1
com.sendRCData(Producer, data)

if poller.poll(100 * 1000):  # 100s timeout in milliseconds
    ack = com.recvAck(Producer)
    if debug: print "DEBUG INFO: Data assembler alive"
else:
    raise IOError("Could not connect to data assembler")
    Producer.close()
    ctx.term()
    sys.exit(0)

# send initial data
print "sending initial data"
com.Status = 0
com.sendRCData(Producer, data)
ack = com.recvAck(Producer)
print ack
time.sleep(1)
request = 0
# send some data

stop = False
while(stop == False):
    time.sleep(1)
    request += 1
    data.count_run = request
    com.sendRCData(Producer, data)
    ack = com.recvAck(Producer)
    print ack
    if ack == "XX":
        CloseConnection()
        stop = True

print ack
time.sleep(0.2)
