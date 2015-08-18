__author__ = 'matthias'
from services.tcp import *
from services.data import *


server = TCP("localhost", port_server=33487, port_client=33488)  # Just for local tests
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