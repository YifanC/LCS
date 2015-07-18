__author__ = 'matthias'
from communication import *
from data import *
import signal

def sigint_handler(signal, frame):
    print "stopping assembler"
    print 'signal: ' + str(signal)
    assembler.stop()

    raise SystemExit(1)

signal.signal(signal.SIGINT, sigint_handler)
data = LaserData()

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
    assembler.recv(data)
    data.writeBinary('test.bin')
    data.writeTxt()
    print "trigger " + str(data.count_trigger)
