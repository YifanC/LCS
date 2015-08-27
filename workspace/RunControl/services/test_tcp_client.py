__author__ = 'matthias'
import time

from services.tcp import *
from services.data import *

data = LaserData()

#client = TCP("131.225.237.31",33487)

client = TCP("131.225.237.27", port_server=33488, port_client=33488)  # Just for local tests

# try to connect to the server
while client.start_client() is False:
    time.sleep(1)

i = 0
while True:
    """ Run forever """
    data.count_trigger = i
    if client.send_client(data) is False:
        """ Try to reconnect forever if the server dies  """
        while client.start_client() is False:
            time.sleep(1)

    i += 1
    time.sleep(1)

client.stop_client()
