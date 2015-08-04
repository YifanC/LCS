__author__ = 'matthias'
from services.tcp import *
from services.data import *


server = TCP()
data = LaserData()

server.start_server()
for i in range(100):
    data = server.recv_server()

    print data