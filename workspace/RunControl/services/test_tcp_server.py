__author__ = 'matthias'
from services.tcp import *
from services.data import *

SERVER = ""
PORT_SERVER = 33488
PORT_CLIENT = 33488


server = TCP(SERVER, port_server=PORT_SERVER, port_client=PORT_CLIENT)  # Just for local tests
data = LaserData()
server.printMsg("Starting server")
server.start_server()
i = 0
while True:
    data.count_trigger = i
    server.send_server(data)
    print "trigger counter: ",data.count_trigger
    i += 1
    time.sleep(1)

server.stop_server()
