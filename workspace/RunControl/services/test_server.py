__author__ = 'matthias'
from communication import *
from data import *

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
