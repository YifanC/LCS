__author__ = 'matthias'
from services.communication import *
from services.data import *
from services.tcp import *
import signal

def sigint_handler(signal, frame):
    print "stopping assembler"
    print 'signal: ' + str(signal)
    assembler.stop()

    raise SystemExit(1)

SERVER = "131.225.237.27"
PORT = 33487

signal.signal(signal.SIGINT, sigint_handler)
data = LaserData()
client = TCP(SERVER, PORT)

assembler = Consumer("assembler")
assembler.start()
assembler.color = False
#assembler.open_logfile()

print "------------ Wait for Hello ------------"
ready = False
assembler.timeout = 5
while not ready:
    ready = assembler.recv_hellos()

print "--------------- Start DAQ --------------"
while True:
    [source_id, state] = assembler.recv(data)
    data.writeBinary('test.bin')

    # only write if new encoder data arrived
    if source_id == 2:
        client.send_client(data)
        data.writeTxt()
    print "trigger " + str(data.count_trigger)
