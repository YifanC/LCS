__author__ = 'matthias'
from services.tcp import *
from services.data import *


server = TCP("localhost", port_server=33487, port_client=33488)  # Just for local tests
data = LaserData()

server.start_server()
for i in range(100):
    data = server.recv_server()
    print data

server.stop_server()