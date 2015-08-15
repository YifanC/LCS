__author__ = 'matthias'
import time

from services.tcp import *
from services.data import *

data = LaserData()

#client = TCP("131.225.237.31",33487)

client = TCP("localhost", port_server=37888, port_client=37999)  # Just for local tests

# try to connect to the server
while client.start_client() is False:
    time.sleep(1)


for i in range(100,120):
    data.count_trigger = i
    client.send_client(data)
    time.sleep(2)

client.stop_client()