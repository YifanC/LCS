__author__ = 'matthias'
import time

from services.tcp import *
from services.data import *

data = LaserData()

# client = TCP("131.225.237.31",33487)
SERVER = "localhost"
PORT_SERVER = 33487
PORT_CLIENT = 33488

client = TCP(SERVER, port_server=PORT_SERVER, port_client=PORT_CLIENT)  # Just for local tests

# try to connect to the server
client.printMsg("Starting client")
while client.start_client() is False:
    time.sleep(1)

while True:
    """ Run forever """
    try:
        data = client.recv_client()
    except Exception as ex:
        client.printError("Receive error: " + str(ex))
        time.sleep(1)

    print "trigger counter: ", data.count_trigger

client.stop_client()
