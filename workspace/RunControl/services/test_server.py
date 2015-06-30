__author__ = 'matthias'
from communication import *
from data import *

data = LaserData()

assembler = Consumer("assembler")
assembler.start()
assembler.color = False

ready = False
while ready is False:
    ready = assembler.recv_hello()


while True:
    assembler.recv(data)
    # print data